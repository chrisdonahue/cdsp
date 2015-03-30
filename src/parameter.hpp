#ifndef CDSP_PARAMETER
#define CDSP_PARAMETER

#include <limits>
#include <unordered_map>

#include "base.hpp"
#include "exception.hpp"
#include "helper.hpp"
#include "types.hpp"
#include "values.hpp"

#define parameter_scheduled_default parameter::rate_audio::scheduler_ramp_dynamic

namespace cdsp { namespace parameter {
	template <typename T>
	class base {
	public:
		base() :
			value_initial_valid(values::boolean_false),
			value_valid(values::boolean_false),
			value_min_valid(values::boolean_false),
			value_max_valid(values::boolean_false)
		{};

		base(T _value) :
			value_initial(_value),
			value(_value),
			value_initial_valid(values::boolean_true),
			value_valid(values::boolean_true),
			value_min_valid(values::boolean_false),
			value_max_valid(values::boolean_false)
		{};

		base(T _value_min, T _value_max) :
			value_min(_value_min),
			value_max(_value_max),
			value_initial_valid(values::boolean_false),
			value_valid(values::boolean_false),
			value_min_valid(values::boolean_true),
			value_max_valid(values::boolean_true)
		{};

		base(T _value, T _value_min, T _value_max) :
			value_initial(_value),
			value(_value),
			value_min(_value_min),
			value_max(_value_max),
			value_initial_valid(values::boolean_true),
			value_valid(values::boolean_true),
			value_min_valid(values::boolean_true),
			value_max_valid(values::boolean_true)
		{};

		// pure virtual destructor to declare class as abstract
		virtual ~base() = 0;

		void reset() {
#ifdef CDSP_DEBUG_API
			if (!value_initial_valid) {
				throw exception::runtime("tried to reset parameter before specifying an initial value");
			}
#endif

			value = value_initial;
		};

		T value_initial_get() {
#ifdef CDSP_DEBUG_API
			if (!value_initial_valid) {
				throw exception::runtime("tried to retrieve initial value before setting it");
			}
#endif

			return value_initial;
		};

		void value_initial_set(T _value_initial) {
			value_initial = _value_initial;
			value_initial_set = values::boolean_true;
		};

		T value_get() {
#ifdef CDSP_DEBUG_API
			if (!value_valid) {
				throw exception::runtime("tried to retrieve value before setting it");
			}
#endif

			return value;
		};

		void value_set(T _value) {
			value = _value;
			value_valid = values::boolean_true;
			if (!value_initial_set) {
				value_initial = value;
				value_initial_set = values::boolean_true;
			}
		};

		types::boolean value_range_valid_get() {
			return value_min_valid && value_max_valid;
		};

		T value_min_get() {
#ifdef CDSP_DEBUG_API
			if (!value_min_valid) {
				// TODO: throw exception
			}
#endif

			return value_min;
		};

		void value_min_set(T _value_min) {
			value_min = _value_min;
			value_min_valid = true;
		};

		T value_max_get() {
#ifdef CDSP_DEBUG_API
			if (!value_max_valid) {
				// TODO: throw exception
			}
#endif

			return value_max;
		};

		void value_max_set(T _value_max) {
			value_max = _value_max;
			value_max_valid = true;
		};

		void value_range_set(T _value_min, T _value_max) {
			value_min = _value_min;
			value_min_valid = true;
			value_max = _value_max;
			value_max_valid = true;
		};

	protected:
		T value_initial;
		T value;
		T value_min;
		T value_max;

	private:
		types::boolean value_initial_valid;
		types::boolean value_valid;
		types::boolean value_min_valid;
		types::boolean value_max_valid;
	};

	// this is horrible... explanation:
		// i want cdsp::parameter::base::~base to be pure virtual (making the class abstract)
		// i can't have the declaration AND definition for a pure virtual function adjacent in GCC
		// i can't define templated class methods in a cpp file without explicit type instantiation
		// subclasses need base<T>::~base() to be defined for automatic inheritance
		// therefore it has to be here!
	template <typename T> inline base<T>::~base() {};

	template <typename T>
	class rate_block : public parameter::base<T> {
	public:
		rate_block() : parameter::base<T>() {};
		rate_block(T value_initial) : parameter::base<T>(value_initial) {};
		rate_block(T value_min, T value_max) : parameter::base<T>(value_min, value_max) {};
		rate_block(T value_initial, T value_min, T value_max) : parameter::base<T>(value_initial, value_min, value_max) {};
		~rate_block() {};
	};
	
	namespace rate_audio {
		class base : public parameter::base<types::sample>, public dsp {
		protected:
			base() : parameter::base<types::sample>() {};
			base(types::sample value_initial) : parameter::base<types::sample>(value_initial), dsp() {};
			base(types::sample value_min, types::sample value_max) : parameter::base<types::sample>(value_min, value_max), dsp() {};
			base(types::sample value_initial, types::sample value_min, types::sample value_max) : parameter::base<types::sample>(value_initial, value_min, value_max), dsp() {};
		};

		class scheduler_ramp : public base {
		public:
			void perform(perform_signature_defaults) {
				base::perform(perform_arguments);

				// ramp temporaries
				ramp* ramp_current = schedule.top();
				types::index ramp_current_samples_remaining = ramp_current->sample_relative;
				types::sample ramp_current_rate = ramp_current->rate;

				// fill buffer
				types::index samples_remaining = block_size_leq;
				types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
				while (samples_remaining--) {
					if (ramp_current_samples_remaining == 0) {
						schedule.pop();
						ramp_current = schedule.top();
						ramp_current_samples_remaining = ramp_current->sample_relative;
						ramp_current_rate = ramp_current->rate;
					}

					value += ramp_current_rate;
					*(output++) = value;

					ramp_current_samples_remaining--;
				}

				// update ramp sample relative
				ramp_current->sample_relative = ramp_current_samples_remaining;
			};

			void reset() {
				schedule.clear();
				value = value_initial;
			};

			void schedule_clear() {
				schedule.clear();
			};

			void schedule_ramp_linear(types::index sample_relative, types::sample value_at) {
				ramp ramp_new;
				ramp_new.type = ramp_type::linear;
				ramp_new.sample_relative = sample_relative;
				ramp_new.value_at = value_at;
				schedule.insert(ramp_new);
			};

			void schedule_ramp_linear(types::time time_relative, types::sample value_at) {
#ifdef CDSP_DEBUG_API
				if (!prepared) {
					throw exception::runtime("schedule_ramp_linear called with time parameter before prepare");
				}
#endif

				schedule_ramp_linear(static_cast<types::index>(sample_rate * time_relative), value_at);
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
				virtual void clear() = 0;

			protected:
				schedule_pq(types::sample* _scheduler_value) : scheduler_value(_scheduler_value) {};
				~schedule_pq() {
					clear();
				}

				types::sample* scheduler_value;
			};

			scheduler_ramp(schedule_pq& _schedule) : base(), schedule(_schedule) {};
			scheduler_ramp(schedule_pq& _schedule, types::sample _value_initial) : base(_value_initial), schedule(_schedule) {};
			scheduler_ramp(schedule_pq& _schedule, types::sample value_min, types::sample value_max) : base(value_min, value_max), schedule(_schedule) {};
			scheduler_ramp(schedule_pq& _schedule, types::sample _value_initial, types::sample value_min, types::sample value_max) : base(_value_initial, value_min, value_max), schedule(_schedule) {};
			virtual ~scheduler_ramp() = 0;

		private:
			schedule_pq& schedule;
		};

		class scheduler_ramp_dynamic : public scheduler_ramp {
		public:
			scheduler_ramp_dynamic() : schedule_pq(&value), scheduler_ramp(schedule_pq) {};
			scheduler_ramp_dynamic(types::sample _value_initial) : schedule_pq(&value), scheduler_ramp(schedule_pq, _value_initial) {};
			scheduler_ramp_dynamic(types::sample value_min, types::sample value_max) : schedule_pq(&value), scheduler_ramp(schedule_pq, value_min, value_max) {};
			scheduler_ramp_dynamic(types::sample _value_initial, types::sample value_min, types::sample value_max) : schedule_pq(&value), scheduler_ramp(schedule_pq, _value_initial, value_min, value_max) {};
			~scheduler_ramp_dynamic() {};

		private:
			class schedule_pq_dynamic : public scheduler_ramp::schedule_pq {
			public:
				schedule_pq_dynamic(types::sample* scheduler_value) : scheduler_ramp::schedule_pq(scheduler_value), head(nullptr) {};

				void insert(ramp& _ramp_new) {
					// allocate
					ramp* ramp_new = new ramp;
					*ramp_new = _ramp_new;

					// insert at head
					if (head == nullptr) {
						head = ramp_new;
						ramp_new->prev = nullptr;
						ramp_new->next = nullptr;
					}

					// insert in list
					ramp* ramp_current = head;
					while (ramp_current != nullptr) {
						if (ramp_current->value_at < ramp_new->value_at) {
							ramp_current = ramp_current->next;
						}
						else if (ramp_current->value_at == ramp_new->value_at) {
							*ramp_current = *ramp_new;
							return;
						}
						else {
							break;
						}
					}

					// update pointers
					ramp* ramp_prev = ramp_current->prev;
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
						delete ramp_current;
						ramp_current = ramp_current->next;
					}
				};

			private:
				void rates_fix() {
					// fix head (special case)
					head->rate = (*scheduler_value) / static_cast<types::sample>(head->sample_relative);

					// fix list
					ramp* ramp_current = head->next;
					while (ramp_current != nullptr && ramp_current->next != nullptr) {
						ramp_current->rate = (ramp_current->next->rate - ramp_current->rate) / static_cast<types::sample>(ramp_current->next->sample_relative);
					}
				};

				ramp* head;
			};

			schedule_pq_dynamic schedule_pq;
		};

		class signal : public base {
		public:
			signal(types::channel _channel) : base() {
				channel = _channel;
				range_map = false;
				parameter_m = values::sample_line_level;
				parameter_b = values::sample_silence;
			};

			signal(types::channel _channel, types::sample signal_min, types::sample signal_max, types::sample parameter_min, types::sample parameter_max) : base(parameter_min, parameter_max) {
				channel = _channel;
				if (signal_min == parameter_min && signal_max == parameter_max) {
					range_map = false;
					parameter_m = values::sample_line_level;
					parameter_b = values::sample_silence;
				}
				else {
					range_map = true;
					helper::range_map_linear(signal_min, signal_max, parameter_min, parameter_max, parameter_m, parameter_b);
				}
			};

			~signal() {};

			void perform(perform_signature_defaults) {
				base::perform(perform_arguments);

				if (range_map) {
					types::sample* output = buffer.channel_pointer_write(channel, offset_sample);
					types::index samples_remaining = block_size_leq;
					while (samples_remaining--) {
						*(output) = ((*output) * parameter_m) + parameter_b;
						output++;
					}
				}
			};

			types::channel channel_get() {
				return channel;
			};

			void channel_set(types::disc_32_u _channel) {
				channel = _channel;
			};

		private:
			types::channel channel;

			types::boolean range_map;
			types::sample parameter_m;
			types::sample parameter_b;
		};
	}
}}

#endif
