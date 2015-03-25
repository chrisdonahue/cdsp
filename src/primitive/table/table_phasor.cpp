#include "table_phasor.hpp"

/*
	cdsp::primitive::table::phasor::base
*/

cdsp::primitive::table::phasor::base::base() :
	table::abstract()
{
	channels_input_num_set(1);
	channels_output_num_set(1);
};

void cdsp::primitive::table::phasor::base::table_set(types::index _table_length, const types::cont_32* _table) {
	table::abstract::table_set(_table_length, _table);

#ifdef CDSP_DEBUG
	if (!cdsp::helper::is_power_of_two(_table_length)) {
		throw cdsp::exception::runtime("provided table length is not a power of two");
	}
#endif

	table_mask = table_length - 1;
	table_length_sample = static_cast<types::sample>(table_length);
};

/*
	cdsp::primitive::table::phasor::interpolate_4
*/

cdsp::primitive::table::phasor::interpolate_4::interpolate_4() :
	base()
{};

void cdsp::primitive::table::phasor::interpolate_4::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// create state
	types::sample phase;
	types::sample table_index;
	types::index table_index_truncated;
	types::sample table_index_fraction;
	types::sample inm1;
	types::sample in0;
	types::sample inp1;
	types::sample inp2;

	// perform
	types::index samples_remaining = block_size_leq;
	const types::sample* input = buffer.channel_pointer_read(offset_channel, offset_sample);
	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	while (samples_remaining--) {
		// get phase
		phase = *(input++);

		// make phase positive TODO: figure out UNITBIT32 stuff in d_osc.c
		while (phase < values::sample_zero) {
			phase += values::sample_one;
		}

		// get table index
		table_index = phase * table_length_sample;

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
	}
};