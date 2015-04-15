#ifndef CDSP_PRIMITIVE_ENVELOPE_SCHEDULE_LINEAR
#define CDSP_PRIMITIVE_ENVELOPE_SCHEDULE_LINEAR

#include "../primitive_base.hpp"

namespace cdsp { namespace primitive { namespace envelope {
	class schedule_linear : public primitive::base {
	public:
		void perform(perform_signature_defaults) {
			base::perform(perform_arguments);

			// ramp temporaries
			ramp* ramp_current = nullptr;
			types::index ramp_current_samples_remaining;
			types::sample ramp_current_rate;
			types::index ramp_samples_completed = 0;

			// assign temporaries if queue is not empty
			if (!queue.empty()) {
				ramp_current = queue.top();
				ramp_current_samples_remaining = ramp_current->sample_relative;
				ramp_current_rate = ramp_current->rate;
			}

			// fill buffer
			types::index samples_remaining = block_size_leq;
			types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
			while (!queue.empty() && samples_remaining) {
				*(output++) = value;
				samples_remaining--;

				value += ramp_current_rate;
				ramp_current_samples_remaining--;
				ramp_samples_completed++;

				if (ramp_current_samples_remaining == 0) {
					value = ramp_current->value_at;
					queue.pop();
					queue.samples_completed(ramp_samples_completed);
					ramp_samples_completed = 0;
					if (!queue.empty()) {
						ramp_current = queue.top();
						ramp_current_samples_remaining = ramp_current->sample_relative;
						ramp_current_rate = ramp_current->rate;
					}
				}
			}
			while (samples_remaining) {
				*(output++) = value;

				samples_remaining--;
			}

			// update ramp sample relative
			queue.samples_completed(ramp_samples_completed);
		};

		void reset() {
			queue.clear();
			value = value_initial;
		};

		void schedule_clear() {
			queue.clear();
		};

		void schedule(types::index sample_relative, types::sample value_at) {
			ramp ramp_new;
			ramp_new.type = ramp_type::linear;
			ramp_new.sample_relative = sample_relative;
			ramp_new.value_at = value_at;
			queue.insert(ramp_new);
		};

		void schedule(types::time time_relative, types::sample value_at) {
#ifdef CDSP_DEBUG_API
			if (!prepared) {
				throw exception::runtime("schedule_ramp_linear called with time parameter before prepare");
			}
#endif

			schedule(static_cast<types::index>(sample_rate * time_relative), value_at);
		};

	protected:
		enum ramp_type {
			linear,
			exponential
		};

		struct ramp {
			// scheduler_ramp will set these
			ramp_type type;
			types::index sample_relative;
			types::sample value_at;

			// schedule_pq subclasses will set these
			types::sample rate;
			ramp* prev;
			ramp* next;
		};

		class schedule_pq {
		public:
			virtual void insert(ramp& ramp_new) = 0;
			virtual ramp* top() = 0;
			virtual void pop() = 0;
			virtual void clear() = 0 {};
			virtual bool empty() = 0 { return true; };
			virtual void samples_completed(types::index samples_num) = 0 {};

		protected:
			schedule_pq(types::sample* _scheduler_value) : scheduler_value(_scheduler_value) {};

			types::sample* scheduler_value;
		};

		schedule_linear(schedule_pq& _schedule, types::sample _value_initial, types::sample _value_min, types::sample _value_max) :
			primitive::base(0, 1),
			queue(_schedule),
			value_initial(_value_initial),
			value(_value_initial),
			value_min(_value_min),
			value_max(_value_max)
		{};
		virtual ~schedule_linear() = 0;

		types::sample value_initial;
		types::sample value;
		types::sample value_min;
		types::sample value_max;

	private:
		schedule_pq& queue;
	};

	schedule_linear::~schedule_linear() {};

	class schedule_linear_dynamic : public schedule_linear {
	public:
		schedule_linear_dynamic(types::sample _value_initial, types::sample _value_min, types::sample _value_max) :
			schedule_pq(&value),
			schedule_linear(schedule_pq, _value_initial, _value_min, _value_max)
		{};

	private:
		class schedule_pq_dynamic : public schedule_linear::schedule_pq {
		public:
			schedule_pq_dynamic(types::sample* scheduler_value) : schedule_linear::schedule_pq(scheduler_value), head(nullptr) {};
			~schedule_pq_dynamic() {
				clear();
			};

			void insert(ramp& _ramp_new) {
				// allocate
				ramp* ramp_new = new ramp;
				*ramp_new = _ramp_new;

				// insert at head
				if (head == nullptr) {
					head = ramp_new;
					ramp_new->prev = nullptr;
					ramp_new->next = nullptr;
					rates_fix();
					return;
				}

				// insert in list
				ramp* ramp_prev = nullptr;
				ramp* ramp_current = head;
				while (ramp_current != nullptr) {
					if (ramp_current->sample_relative < ramp_new->sample_relative) {
						ramp_prev = ramp_current;
						ramp_current = ramp_current->next;
					}
					else if (ramp_current->sample_relative == ramp_new->sample_relative) {
						*ramp_current = *ramp_new;
					}
					else {
						break;
					}
				}

				// update head if necessary
				if (ramp_current == head) {
					head = ramp_new;
				}

				// update pointers
				ramp* ramp_next = ramp_current;
				ramp_new->prev = ramp_prev;
				ramp_new->next = ramp_next;
				if (ramp_prev != nullptr) {
					ramp_prev->next = ramp_new;
				}
				if (ramp_next != nullptr) {
					ramp_next->prev = ramp_new;
				}

				// fix rates
				rates_fix();
			};

			ramp* top() {
				return head;
			};

			void pop() {
				if (head == nullptr) {
					return;
				}

				ramp* ramp_old = head;
				head = ramp_old->next;
				delete ramp_old;
			};

			void clear() {
				ramp* ramp_current = head;
				while (ramp_current != nullptr) {
					ramp* ramp_next = ramp_current->next;
					delete ramp_current;
					ramp_current = ramp_next;
				}
			};

			bool empty() {
				return head == nullptr;
			};

			void samples_completed(types::index samples_num) {
				ramp* ramp_current = head;
				while (ramp_current != nullptr) {
					ramp_current->sample_relative -= samples_num;
					ramp_current = ramp_current->next;
				}
			};

		private:
			void rates_fix() {
				// fix head (special case)
				head->rate = (head->value_at - *scheduler_value) / static_cast<types::sample>(head->sample_relative);

				// fix list
				ramp* ramp_prev = head;
				ramp* ramp_current;
				while (ramp_prev != nullptr && ramp_prev->next != nullptr) {
					ramp_current = ramp_prev->next;
					ramp_current->rate = (ramp_current->value_at - ramp_prev->value_at) / static_cast<types::sample>(ramp_current->sample_relative - ramp_prev->sample_relative);
					ramp_prev = ramp_current;
				}
			};

			ramp* head;
		};

		schedule_pq_dynamic schedule_pq;
	};
}}}

#endif