#include "table_interpolate_4.hpp"

cdsp::primitives::oscillators::table_interpolate_4::table_interpolate_4() :
	base(),
	table_length(0),
	table_mask(0),
	table(nullptr)
{
};

cdsp::primitives::oscillators::table_interpolate_4::table_interpolate_4(types::sample _phase, types::sample _frequency) :
	base(_phase, _frequency),
	table_length(values::sample_zero),
	table_mask(0),
	table(nullptr)
{
};

void cdsp::primitives::oscillators::table_interpolate_4::table_set(types::disc_32_u _table_length, const types::cont_32* _table) {
#ifdef CDSP_DEBUG
	if (!cdsp::helpers::is_power_of_two(_table_length)) {
		throw cdsp::exceptions::runtime("cdsp::primitives::oscillators::table_interpolate_4: table_set called with a table_size that was not a power of two");
	}
#endif

	table_length = static_cast<types::sample>(_table_length);
	table_mask = _table_length - 1;
	table = _table;
};

void cdsp::primitives::oscillators::table_interpolate_4::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::disc_32_u offset_sample) {
	primitives::base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// check to make sure we have a table to interpolate
#ifdef CDSP_DEBUG_DSP
	if (table == nullptr) {
		throw cdsp::exceptions::runtime("cdsp::primitives::oscillators::table_interpolate_4: perform called before set_table");
	}
#endif

	// get parameter values
	parameter::signal* frequency_plug;
	const types::sample* frequency_buffer;
	if ((frequency_plug = parameter_plug_get("frequency")) == nullptr) {
		frequency.perform(buffer, block_size_leq, offset_channel, offset_sample);
		frequency_buffer = buffer.channel_pointer_read(0, offset_sample);
	}
	else {
		types::channel frequency_plug_channel = frequency_plug->channel_get();
		frequency_plug->perform(buffer, block_size_leq, frequency_plug_channel, offset_sample);
		frequency_buffer = buffer.channel_pointer_read(frequency_plug_channel, offset_sample);
	}

	// create temporaries
	types::sample frequency_current;
	types::sample phase_increment;
	types::disc_32_u phase_truncated;
	types::sample fraction;
	types::sample inm1;
	types::sample in0;
	types::sample inp1;
	types::sample inp2;

	// perform
	for (types::disc_32_u i = 0; i < block_size_leq; i++) {
		// find current frequency (TODO: dezippered)
		frequency_current = frequency_buffer[i];

		// calculate phase increment
		phase_increment = frequency_current * table_length;

		// truncate phase
		phase_truncated = static_cast<types::disc_32_u>(floor(phase));

		// calculate fractional position
		fraction = phase - (static_cast<types::cont_32>(phase_truncated));

		// calculate table offset
		inm1 = table[(phase_truncated - 1) & table_mask];
		in0 = table[phase_truncated & table_mask];
		inp1 = table[(phase_truncated + 1) & table_mask];
		inp2 = table[(phase_truncated + 2) & table_mask];

		// calculate interpolated output
		for (types::disc_32_u channel = 0; channel < channels_output_num_get(); channel++) {
			buffer.channel_pointer_write(offset_channel + channel, offset_sample)[i] = static_cast<types::sample>(
				in0 + 0.5 * fraction * (inp1 - inm1 + 
				fraction * (4.0 * inp1 + 2.0 * inm1 - 5.0 * in0 - inp2 +
				fraction * (3.0 * (in0 - inp1) - inm1 + inp2)))
			);
		}

		// add phase increment
		phase += phase_increment;
	}

	// prevent phase from overflowing
	while (phase > table_length) {
		phase = phase - table_length;
	}
	while (phase < 0.0) {
		phase = phase + table_length;
	}
};