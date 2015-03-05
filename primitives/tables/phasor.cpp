#include "phasor.hpp"

cdsp::primitives::oscillators::phasor::phasor() :
base()
{
};

cdsp::primitives::oscillators::phasor::phasor(types::sample _phase, types::sample _frequency) :
base(_phase, _frequency)
{
};

void cdsp::primitives::oscillators::phasor::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// get parameter values
	parameter::signal* frequency_plug;
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
	types::sample phase_increment;

	// loop
	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	types::disc_32_u samples_num_remaining = block_size_leq;
	while (samples_num_remaining--) {
		phase_increment = *(frequency_buffer++);
		*(output++) = phase;
		phase += phase_increment;

		if (phase > values::sample_one) {
			phase = values::sample_zero;
		}
	}
};
