#include "table_oscillator.hpp"

/*
	cdsp::primitives::tables::oscillator::base
*/

cdsp::primitives::tables::oscillator::base::base() :
	tables::base(),
	phase_initial(values::sample_zero, values::sample_zero, values::sample_one),
	frequency(values::sample_zero, values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
	parameter_plug_register("frequency");
}

cdsp::primitives::tables::oscillator::base::base(types::sample _phase_initial) :
	tables::base(),
	phase_initial(_phase_initial, values::sample_zero, values::sample_one),
	frequency(values::sample_zero, values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
	parameter_plug_register("frequency");
}

cdsp::primitives::tables::oscillator::base::base(types::sample _phase_initial, types::sample _frequency) :
	tables::base(),
	phase_initial(_phase_initial, values::sample_zero, values::sample_one),
	frequency(_frequency, values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
	parameter_plug_register("frequency");
}

void cdsp::primitives::tables::oscillator::base::table_set(types::index _table_length, const types::cont_32* _table) {
	tables::base::table_set(_table_length, _table);

#ifdef CDSP_DEBUG
	if (!cdsp::helpers::is_power_of_two(_table_length)) {
		throw cdsp::exceptions::runtime("cdsp::primitives::oscillators::table_interpolate_4: table_set called with a table_size that was not a power of two");
	}
#endif

	table_mask = table_length - 1;
	table_length_sample = static_cast<types::sample>(table_length);

	phase_reset();
};

void cdsp::primitives::tables::oscillator::base::prepare(types::cont_64 _sample_rate, types::index _block_size) {
	tables::base::prepare(_sample_rate, _block_size);

	// check to make sure we have a table to interpolate
#ifdef CDSP_DEBUG_DSP
	if (table == nullptr) {
		throw cdsp::exceptions::runtime("cdsp::primitives::tables::oscillator::base::prepare: perform called before set_table");
	}
#endif

	frequency.prepare(_sample_rate, _block_size);
}

void cdsp::primitives::tables::oscillator::base::phase_reset() {
#ifdef CDSP_DEBUG
	if (table == nullptr) {
		throw cdsp::exceptions::runtime("cdsp::primitives::tables::oscillator::base::phase_reset: phase_reset called before set_table");
	}
#endif

	table_index = phase_initial.value_get() * table_length_sample;
};

void cdsp::primitives::tables::oscillator::base::phase_initial_set(types::sample _phase_initial) {
	phase_initial.value_set(_phase_initial);
};

void cdsp::primitives::tables::oscillator::base::frequency_set(types::sample _frequency) {
	frequency.value_set(_frequency);
};

void cdsp::primitives::tables::oscillator::base::frequency_next_set(types::sample frequency_next) {
	frequency.value_next_set(frequency_next);
};

void cdsp::primitives::tables::oscillator::base::frequency_next_set(types::sample frequency_next, types::cont_64 delay_s) {
	frequency.value_next_set(frequency_next, delay_s);
};

/*
	cdsp::primitives::tables::oscillator::interpolate_4
*/

cdsp::primitives::tables::oscillator::interpolate_4::interpolate_4() :
	base()
{}

cdsp::primitives::tables::oscillator::interpolate_4::interpolate_4(types::sample _phase) :
	base(_phase)
{}

cdsp::primitives::tables::oscillator::interpolate_4::interpolate_4(types::sample _phase, types::sample _frequency) :
	base(_phase, _frequency)
{}

void cdsp::primitives::tables::oscillator::interpolate_4::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// get parameter values
	parameters::signal* frequency_plug;
	const types::sample* frequency_buffer;
	if ((frequency_plug = parameter_plug_get("frequency")) == nullptr) {
		frequency.perform(buffer, block_size_leq, offset_channel, offset_sample);
		frequency_buffer = buffer.channel_pointer_read(offset_channel, offset_sample);
	}
	else {
		types::channel frequency_plug_channel = frequency_plug->channel_get();
		frequency_plug->perform(buffer, block_size_leq, frequency_plug_channel, offset_sample);
		frequency_buffer = buffer.channel_pointer_read(frequency_plug_channel, offset_sample);
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