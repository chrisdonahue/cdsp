#include "phasor.hpp"

cdsp::primitives::oscillators::phasor::phasor() :
	base(),
	phase(values::sample_zero, values::sample_one),
	frequency(values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
	parameter_plug_register("frequency");
};

cdsp::primitives::oscillators::phasor::phasor(types::sample _phase, types::sample _frequency) :
	base(),
	phase(_phase, values::sample_zero, values::sample_one),
	frequency(_frequency, values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
	parameter_plug_register("frequency");
};

void cdsp::primitives::oscillators::phasor::prepare(types::cont_64 _sample_rate, types::index _block_size) {
	base::prepare(_sample_rate, _block_size);

	frequency.prepare(_sample_rate, _block_size);
}

void cdsp::primitives::oscillators::phasor::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// retrieve phase
	types::sample _phase = phase.value_get();

	// retrieve frequency
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

	// create temporary state state
	types::sample phase_increment;

	// perform
	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	types::disc_32_u samples_num_remaining = block_size_leq;
	while (samples_num_remaining--) {
		phase_increment = *(frequency_buffer++);
		*(output++) = _phase;
		_phase += phase_increment;

		while (_phase > values::sample_one) {
			_phase -= values::sample_one;
		}
	}

	// store phase
	phase.value_set(_phase);
};

void cdsp::primitives::oscillators::phasor::phase_reset() {
	phase.value_set(values::sample_zero);
};

void cdsp::primitives::oscillators::phasor::phase_set(types::sample _phase) {
	phase.value_set(_phase);
};

void cdsp::primitives::oscillators::phasor::frequency_set(types::sample _frequency) {
	frequency.value_set(_frequency);
};

void cdsp::primitives::oscillators::phasor::frequency_next_set(types::sample frequency_next) {
	frequency.value_next_set(frequency_next);
};

void cdsp::primitives::oscillators::phasor::frequency_next_set(types::sample frequency_next, types::cont_64 delay_s) {
	frequency.value_next_set(frequency_next, delay_s);
};