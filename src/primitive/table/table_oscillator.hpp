#ifndef CDSP_PRIMITIVES_TABLES_OSCILLATOR
#define CDSP_PRIMITIVES_TABLES_OSCILLATOR

#include "../primitive_base.hpp"
#include "table_abstract.hpp"
#include "../../helper.hpp"

namespace cdsp { namespace primitive { namespace table { namespace oscillator {
	template <class frequency_parameter_type=parameter_scheduled_default>
	class base :	public primitive::pluggable,
					public table::abstract
	{
	public:
		base() :
			primitive::pluggable(0, 1),
			table::abstract(),
			phase_initial(values::sample_zero, values::sample_zero, values::sample_one),
			frequency(values::sample_zero, values::sample_zero, values::sample_one)
		{};

		base(types::sample _phase) :
			primitive::pluggable(0, 1),
			table::abstract(),
			phase_initial(_phase_initial, values::sample_zero, values::sample_one),
			frequency(values::sample_zero, values::sample_zero, values::sample_one)
		{};

		base(types::sample _phase, types::sample _frequency) :
			primitive::pluggable(0, 1),
			table::abstract(),
			phase_initial(_phase, values::sample_zero, values::sample_one),
			frequency(_frequency, values::sample_zero, values::sample_one)
		{};

		~base() {};

		void table_set(types::index _table_length, const types::sample* _table) {
			table::abstract::table_set(_table_length, _table);

#ifdef CDSP_DEBUG_API
			if (!helpers::is_power_of_two(_table_length)) {
				throw exception::runtime("table length that was not a power of two");
			}
#endif

			table_mask = table_length - 1;
			table_length_sample = static_cast<types::sample>(table_length);
			phase_reset();
		};
		
		void prepare(prepare_signature) {
			primitive::pluggable::prepare(prepare_arguments);

#ifdef CDSP_DEBUG_API
			if (table == nullptr) {
				throw cdsp::exception::runtime("prepare called before table_set");
			}
#endif
			frequency.prepare(prepare_arguments);
		};

		void release() {
			frequency.release();
		};

		virtual void perform(perform_signature_defaults) = 0;

		void phase_reset() {
#ifdef CDSP_DEBUG_API
			if (table == nullptr) {
				throw cdsp::exception::runtime("table not set");
			}
#endif
			table_index = phase_initial.value_get() * table_length_sample;
		};

		void phase_initial_set(types::sample _phase_initial) {
			phase_initial.value_set(_phase_initial);
		};

		frequency_parameter_type& frequency_parameter_get() {
			return frequency;
		};

	protected:
		types::index table_mask;
		types::sample table_length_sample;

		types::sample table_index;
		parameter::rate_block<types::sample> phase_initial;
		frequency_parameter_type frequency;
	};

	template <class frequency_parameter_type=parameter_scheduled_default>
	class interpolate_4 : public base<frequency_parameter_type> {
	public:
		interpolate_4() : base<frequency_parameter_type>();
		interpolate_4(types::sample _phase) : base<frequency_parameter_type>(_phase);
		interpolate_4(types::sample _phase, types::sample _frequency) : base<frequency_parameter_type>(_phase, _frequency);

		void perform(perform_signature_defaults) {
			base::perform(perform_arguments);

			// get parameter values
			const types::sample* frequency_buffer;
			if (parameter_plugged("frequency")) {
				parameter::rate_audio::signal& frequency_plug = parameter_plugged_get("frequency");
				types::channel frequency_plug_channel = frequency_plug.channel_get();
				frequency_plug.perform(buffer, block_size_leq, frequency_plug_channel, offset_sample);
				frequency_buffer = buffer.channel_pointer_read(frequency_plug_channel, offset_sample);
			}
			else {
				frequency.perform(perform_arguments);
				frequency_buffer = buffer.channel_pointer_read(offset_channel, offset_sample);
			}

			// create state
			types::sample frequency;
			types::sample table_index_increment;
			types::index table_index_truncated;
			types::sample table_index_fraction;
			types::sample inm1;
			types::sample in0;
			types::sample inp1;
			types::sample inp2;

			// perform
			types::index samples_remaining = block_size_leq;
			types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
			while (samples_remaining--) {
				// get frequency
				frequency = *(frequency_buffer++);

				// calculate phase increment
				table_index_increment = frequency * table_length_sample;

				// truncate table index
				table_index_truncated = static_cast<types::index>(floor(table_index));

				// calculate table index fraction
				table_index_fraction = table_index - (static_cast<types::sample>(table_index_truncated));

				// retrieve table values
				inm1 = table[(table_index_truncated - 1) & table_mask];
				in0 = table[table_index_truncated & table_mask];
				inp1 = table[(table_index_truncated + 1) & table_mask];
				inp2 = table[(table_index_truncated + 2) & table_mask];

				// output
				*(output++) = static_cast<types::sample>(in0 + 0.5 * table_index_fraction * (inp1 - inm1 + table_index_fraction * (4.0 * inp1 + 2.0 * inm1 - 5.0 * in0 - inp2 + table_index_fraction * (3.0 * (in0 - inp1) - inm1 + inp2))));

				// increment phase
				table_index += table_index_increment;
			}

			// prevent phase from overflowing
			while (table_index > table_length_sample) {
				table_index -= table_length_sample;
			}
			while (table_index < 0.0) {
				table_index += table_length_sample;
			}
		};
	};
}}}}

#endif
