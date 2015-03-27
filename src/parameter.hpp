#ifndef CDSP_PARAMETER
#define CDSP_PARAMETER

#include <limits>
#include <unordered_map>

#include "base.hpp"
#include "exception.hpp"
#include "helper.hpp"
#include "types.hpp"
#include "values.hpp"

namespace cdsp { namespace parameter {
	template <typename T>
	class base {
	public:
		base() :
			value_valid(values::boolean_false),
			value_min_valid(values::boolean_false),
			value_max_valid(values::boolean_false)
		{};

		base(T _value) :
			value(_value),
			value_valid(values::boolean_true),
			value_min_valid(values::boolean_false),
			value_max_valid(values::boolean_false)
		{};

		base(T _value_min, T _value_max) :
			value_valid(values::boolean_false),
			value_min(_value_min),
			value_min_valid(values::boolean_true),
			value_max(_value_max),
			value_max_valid(values::boolean_true)
		{};

		base(T _value, T _value_min, T _value_max) :
			value(_value),
			value_valid(values::boolean_true),
			value_min(_value_min),
			value_min_valid(values::boolean_true),
			value_max(_value_max),
			value_max_valid(values::boolean_true)
		{};

		// pure virtual destructor to declare class as abstract
		virtual ~base() = 0;

		types::boolean value_valid_get() {
			return value_valid;
		};

		T value_get() {
#ifdef CDSP_DEBUG
			if (!value_valid) {
				// TODO: throw exception
			}
#endif

			return value;
		};

		void value_set(T _value) {
			value = _value;
			value_valid = true;
		};

		types::boolean value_range_valid_get() {
			return value_min_valid && value_max_valid;
		};

		T value_min_get() {
#ifdef CDSP_DEBUG
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
#ifdef CDSP_DEBUG
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
		T value;
		T value_min;
		T value_max;

	private:
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
		public:
			base() : parameter::base<types::sample>() {};
			base(types::sample value_initial) : parameter::base<types::sample>(value_initial), dsp() {};
			base(types::sample value_min, types::sample value_max) : parameter::base<types::sample>(value_min, value_max), dsp() {};
			base(types::sample value_initial, types::sample value_min, types::sample value_max) : parameter::base<types::sample>(value_initial, value_min, value_max), dsp() {};
		};

		class schedule_ramp : public base {
		public:
			schedule_ramp() :
				base(), schedule_n(1), schedule(nullptr)
			{
				schedule_alloc();
			};

			schedule_ramp(types::index _schedule_n) :
				base(), schedule_n(_schedule_n), schedule(nullptr)
			{
				schedule_alloc();
			};

			schedule_ramp(types::index _schedule_n, types::sample value_initial) :
				base(value_initial), schedule_n(_schedule_n), schedule(nullptr)
			{
				schedule_alloc();
			};

			schedule_ramp(types::index _schedule_n, types::sample value_min, types::sample value_max) :
				base(value_min, value_max), schedule_n(_schedule_n), schedule(nullptr)
			{
				schedule_alloc();
			};

			schedule_ramp(types::index _schedule_n, types::sample value_initial, types::sample value_min, types::sample value_max) :
				base(value_initial, value_min, value_max), schedule_n(_schedule_n), schedule(nullptr)
			{
				schedule_alloc();
			};

			~schedule_ramp()
			{
				schedule_free();
			};

			void prepare(prepare_signature) {
			};

			void perform(perform_signature_defaults) {
			};

			void release() {
			};

			void schedule_resize(types::index _schedule_n) {
				if (_schedule_n != schedule_n) {
					schedule_n = _schedule_n;
					schedule_free();
					schedule_alloc();
				}
			};

			// REDO THIS WITH FIXED LENGTH LINKED LIST
			//virtual void schedule_ramp_linear(types::time relative, types::sample value_at) = 0;
			void schedule_ramp_linear(types::index sample_relative, types::sample value_at) {
				// determine position
				types::index ramp_new_destination = 0;
				ramp* ramp_current = schedule;
				while (ramp_new_destination < scheduled && ramp_current->sample_relative < sample_relative) {
					ramp_current = schedule + (ramp_new_destination++);
				}

				// reject schedule if its beyond the boundaries
				if (ramp_new_destination >= schedule_n) {
#ifdef CDSP_DEBUG_API
					throw cdsp::exception::runtime("attempted to schedule a ramp value when n were already scheduled");
#endif
					return;
				}

				// put new ramp on stack
				ramp ramp_new;
				ramp_new.type = ramp_type::linear;
				ramp_new.sample_relative = sample_relative;
				ramp_new.value_at = value_at;

				if (scheduled == schedule_n) {

				}

				// insert value if its within the boundaries
				types::index shift = scheduled == schedule_n ? schedule_n - 2 : scheduled;
				ramp* shift_ramp;
				ramp* shift_ramp_dest;
				while (shift >= ramp_new_destination) {
					shift_ramp = schedule + shift;
					shift_ramp_dest = schedule + shift + 1;

					*shift_ramp = *shift_ramp_dest;

					shift--;
				}

				// insert ramp_new
				*(schedule + ramp_new_destination) = ramp_new;
			};

			void schedule_ramp_linear(types::time time_relative, types::sample value_at) {
#ifdef CDSP_DEBUG_DSP
				if (!prepared) {
					throw cdsp::exception::runtime("schedule_ramp_linear called with a time parameter before prepare");
				}
				if (time_relative < values::time_zero) {
					throw cdsp::exception::runtime("schedule_ramp_linear called with a negative time parameter");
				}
#endif

				schedule_ramp_linear(static_cast<types::index>(time_relative * sample_rate), value_at);
			};

		protected:
			enum ramp_type {
				linear,
				exponential
			};

			struct ramp {
				ramp_type type;
				types::index sample_relative;
				types::cont_64 value_at;
			};

			types::index scheduled_num;

		private:
			inline void schedule_alloc() {
				schedule = reinterpret_cast<ramp*>(malloc(sizeof(ramp) * schedule_n));
			};

			inline void schedule_free() {
				if (schedule != nullptr) {
					free(schedule);
					schedule = nullptr;
				}
			};

			types::index schedule_n;
			ramp* schedule;
			types::index scheduled;
		};

		class schedule_ramp_static : public schedule_ramp {
		public:
			schedule_ramp_static() : schedule_ramp() {};
			schedule_ramp_static(types::index _schedule_n) : schedule_ramp() {};
			schedule_ramp_static(types::index _schedule_n, types::sample value_initial) : schedule_ramp(_schedule_n, value_initial) {};
			schedule_ramp_static(types::index _schedule_n, types::sample value_min, types::sample value_max) : schedule_ramp(_schedule_n, value_min, value_max) {};
			schedule_ramp_static(types::index _schedule_n, types::sample value_initial, types::sample value_min, types::sample value_max) : schedule_ramp(_schedule_n, value_initial, value_min, value_max) {};
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
					while (block_size_leq--) {
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

		/*
		class ramp_linear : public base {
		public:
			ramp_linear() : base() {};
			ramp_linear(types::sample value_initial) : base(value_initial), value_next(value), dezipper_samples_num(0), dezipper_increment(values::sample_silence) {};
			ramp_linear(types::sample value_min, types::sample value_max) : base(value_min, value_max), value_next(value), dezipper_samples_num(0), dezipper_increment(values::sample_silence) {};
			ramp_linear(types::sample value_initial, types::sample value_min, types::sample value_max) : base(value_initial, value_min, value_max), value_next(value), dezipper_samples_num(0), dezipper_increment(values::sample_silence) {};

			void perform(perform_signature_defaults) {
				base::perform(perform_arguments);
				*/
	#ifdef CDSP_DEBUG_DSP
				/*
				if (value_range_valid_get()) {
					types::index samples_remaining = block_size_leq;
					const types::sample* samples = buffer.channel_pointer_read(offset_channel, offset_sample);
					types::sample sample;
					while (samples_remaining--) {
						sample = *(samples++);
						if (sample < value_min_get() || sample > value_max_get()) {
							throw exception::runtime("cdsp::parameter::ramp_linear::perform: sample outside of specified range");
						}
					}
				}
				*/
	#endif
		/*
			};

			void value_set(types::sample _value) {
				base::value_set(_value);
				value_next = _value;
				dezipper_samples_num = 0;
				dezipper_increment = values::sample_silence;
			};

			void value_next_set(types::sample _value_next, types::cont_64 delay_s) {
	#ifdef CDSP_DEBUG
				if (!prepared) {
					throw exception::runtime("cdsp::parameter::ramp_linear::value_next_set: value_next_set called before prepare");
				}

				if (delay_s <= values::zero_64) {
					throw exception::runtime("cdsp::parameter::ramp_linear: value_next_set called with a negative delay_s");
				}
				else if (delay_s == values::zero_64) {
					return;
				}
				if (_value_next == value) {
					return;
				}
	#endif

				value_next = _value_next;
				types::cont_64 value_difference = static_cast<types::cont_64>(value_next - value);
				types::cont_64 ramp_length_samples = sample_rate * delay_s;
				types::cont_64 inverse = values::one_64 / ramp_length_samples;
				dezipper_samples_num = static_cast<types::disc_32_u>(ramp_length_samples);
				dezipper_increment = static_cast<types::sample>(inverse * value_difference);
			};

			void value_next_set(types::sample _value_next) {
	#ifdef CDSP_DEBUG
				if (!prepared) {
					throw exception::runtime("cdsp::parameter::ramp_linear::value_next_set: value_next_set called before prepare");
				}
	#endif

				if (_value_next == value) {
					return;
				}

				value_next = _value_next;
				types::cont_64 value_difference = static_cast<types::cont_64>(value_next - value);
				dezipper_samples_num = block_size;
				dezipper_increment = static_cast<types::sample>(block_size_inverse * value_difference);
			};

		protected:
			void _value_dezipper_start(types::disc_32_u& _dezipper_samples_num, types::sample& _dezipper_increment) {
				_dezipper_samples_num = dezipper_samples_num;
				_dezipper_increment = dezipper_increment;
			};

			void _value_dezipper_end(types::disc_32_u dezipper_samples_completed, types::sample _value) {
				value = _value;
				dezipper_samples_num -= dezipper_samples_completed;
				if (dezipper_samples_num <= 0) {
					value = value_next;
					dezipper_samples_num = 0;
					dezipper_increment = values::sample_silence;
				}
			};

			types::sample value_next;

			types::disc_32_u dezipper_samples_num;
			types::sample dezipper_increment;
		};

		class ramp_linear_manual : public ramp_linear {
		public:
			ramp_linear_manual() : ramp_linear() {};
			ramp_linear_manual(types::sample value_initial) : ramp_linear(value_initial) {};
			ramp_linear_manual(types::sample value_min, types::sample value_max) : ramp_linear(value_min, value_max) {};
			ramp_linear_manual(types::sample value_initial, types::sample value_min, types::sample value_max) : ramp_linear(value_initial, value_min, value_max) {};
			~ramp_linear_manual() {};

			void value_dezipper_start(types::disc_32_u& _dezipper_samples_num, types::sample& _dezipper_increment) {
				_value_dezipper_start(_dezipper_samples_num, _dezipper_increment);
			};

			void value_dezipper_end(types::disc_32_u dezipper_samples_completed, types::sample _value) {
				_value_dezipper_end(dezipper_samples_completed, _value);
			};
		};

		class ramp_linear_automatic : public ramp_linear {
		public:
			ramp_linear_automatic() : ramp_linear() {};
			ramp_linear_automatic(types::sample value_initial) : ramp_linear(value_initial) {};
			ramp_linear_automatic(types::sample value_min, types::sample value_max) : ramp_linear(value_min, value_max) {};
			ramp_linear_automatic(types::sample value_initial, types::sample value_min, types::sample value_max) : ramp_linear(value_initial, value_min, value_max) {};
			~ramp_linear_automatic() {};

			void prepare(prepare_signature) {
				ramp_linear::prepare(prepare_arguments);
				buffer.resize(1, _block_size);
				dezipper_buffer = buffer.channel_pointer_write(0);
			};

			void release() {
				ramp_linear::release();
				buffer.resize(0, 0);
			};

			void perform(perform_signature_defaults) {
				ramp_linear::perform(perform_arguments);

				types::sample* buffer_output = buffer.channel_pointer_write(offset_channel, offset_sample);
				types::disc_32_u buffer_samples_remaining = block_size_leq;

				// dezipper
				types::disc_32_u dezipper_samples_completed;
				types::disc_32_u dezipper_samples_remaining;
				types::sample _dezipper_increment;
				types::sample value_current = value;
				_value_dezipper_start(dezipper_samples_completed, _dezipper_increment);
				dezipper_samples_completed = dezipper_samples_completed > buffer_samples_remaining ? buffer_samples_remaining : dezipper_samples_completed;
				dezipper_samples_remaining = dezipper_samples_completed;
				while (dezipper_samples_remaining--) {
					*(buffer_output++) = value;
					value += _dezipper_increment;
					buffer_samples_remaining--;
				}
				_value_dezipper_end(dezipper_samples_completed, value_current);

				// fill rest of buffer
				while (buffer_samples_remaining--) {
					*(buffer_output++) = value;
				}
			};

		private:
			sample_buffer buffer;

			types::sample* dezipper_buffer;
		};*/
	}
}}

#endif
