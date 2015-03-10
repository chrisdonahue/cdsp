#include "table_function.hpp"

/*
	cdsp::primitives::tables::function::base
*/

cdsp::primitives::tables::function::base::base() :
	tables::base(),
	x(values::sample_zero)
{
};

cdsp::primitives::tables::function::base::base(types::sample x_value) :
	tables::base(),
	x(x_value)
{
};

cdsp::primitives::tables::function::base::base(types::sample domain_min, types::sample domain_max) :
	tables::base(),
	x(domain_min, domain_max)
{
};

cdsp::primitives::tables::function::base::base(types::sample x_value, types::sample domain_min, types::sample domain_max) :
	tables::base(),
	x(x_value, domain_min, domain_max)
{
};

void cdsp::primitives::tables::function::base::domain_set(types::sample domain_min, types::sample domain_max) {
	x.value_range_set(domain_min, domain_max);
};

void cdsp::primitives::tables::function::base::table_set(types::index _table_length, const types::sample* _table) {
	tables::base::table_set(_table_length, _table);

#ifdef CDSP_DEBUG
	if (!x.value_range_valid_get()) {
		throw cdsp::exceptions::runtime("cdsp::primitives::tables::function::base::table_set: table_set called before domain_set");
	}

	if (_table_length < 4) {
		throw cdsp::exceptions::runtime("cdsp::primitives::tables::function::base::table_set: table_length was insufficient for 4-point interpolation");
	}
#endif

	helpers::range_map_linear(x.value_min_get(), x.value_max_get(), values::sample_one, static_cast<types::sample>(table_length - 3), x_m, x_b);
};

void cdsp::primitives::tables::function::base::prepare(types::cont_64 _sample_rate, types::index _block_size) {
	tables::base::prepare(_sample_rate, _block_size);

	x.prepare(_sample_rate, _block_size);
}

void cdsp::primitives::tables::function::base::x_set(types::sample _x) {
	x.value_set(_x);
};

void cdsp::primitives::tables::function::base::x_next_set(types::sample x_next) {
	x.value_next_set(x_next);
};

void cdsp::primitives::tables::function::base::x_next_set(types::sample x_next, types::cont_64 delay_s) {
	x.value_next_set(x_next, delay_s);
};

/*
	cdsp::primitives::tables::function::interpolate_4
*/

void cdsp::primitives::tables::function::interpolate_4::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// get parameter values
	parameters::signal* x_plug;
	const types::sample* x_buffer;
	if ((x_plug = parameter_plug_get("frequency")) == nullptr) {
		x.perform(buffer, block_size_leq, offset_channel, offset_sample);
		x_buffer = buffer.channel_pointer_read(offset_channel, offset_sample);
	}
	else {
		types::channel frequency_plug_channel = x_plug->channel_get();
		x_plug->perform(buffer, block_size_leq, frequency_plug_channel, offset_sample);
		x_buffer = buffer.channel_pointer_read(frequency_plug_channel, offset_sample);
	}

	// create state
	types::sample x;
	types::index table_index;
	types::sample table_index_fraction;
	types::sample inm1;
	types::sample in0;
	types::sample inp1;
	types::sample inp2;

	// perform
	types::index samples_remaining = block_size_leq;
	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	while (samples_remaining--) {
		// find current function value
		x = *(x_buffer++);

		// map range to table indices
		x = x * x_m + x_b;

		// calculate current table index
		table_index = static_cast<types::index>(floor(x));

		// check index
#ifdef CDSP_DEBUG_DSP
		if (table_index < 1 || table_index > table_length - 3) {
			throw exceptions::runtime("cdsp::primitives::tables::function::interpolate_4::perform: table_index was out of range");
		}
#endif

		// calculate fractional part
		table_index_fraction = x - table_index;

		// retrieve table values
		inm1 = table[(table_index - 1)];
		in0 = table[table_index];
		inp1 = table[(table_index + 1)];
		inp2 = table[(table_index + 2)];
		
		// output
		*(output++) = static_cast<types::sample>(in0 + 0.5 * table_index_fraction * (inp1 - inm1 + table_index_fraction * (4.0 * inp1 + 2.0 * inm1 - 5.0 * in0 - inp2 + table_index_fraction * (3.0 * (in0 - inp1) - inm1 + inp2))));
	}
};