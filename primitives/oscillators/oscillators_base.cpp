#include "oscillators_base.hpp"

cdsp::primitives::oscillators::base::base() :
	primitives::base(),
	phase(values::sample_zero),
	frequency(values::sample_zero, values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);

	// create parameters
	parameter_plug_register("frequency");
}

cdsp::primitives::oscillators::base::base(types::sample _phase, types::sample _frequency) :
	primitives::base(),
	phase(_phase),
	frequency(_frequency, values::sample_zero, values::sample_one)
{
	channels_input_num_set(0);
	channels_output_num_set(1);

	// create parameters
	parameter_plug_register("frequency");
}

void cdsp::primitives::oscillators::base::prepare(types::cont_64 _sample_rate, types::disc_32_u _block_size) {
	primitives::base::prepare(_sample_rate, _block_size);

	frequency.prepare(_sample_rate, _block_size);
}

void cdsp::primitives::oscillators::base::phase_set(types::sample _phase) {
	phase = _phase;
}

void cdsp::primitives::oscillators::base::frequency_set(types::sample _frequency) {
	frequency.value_set(_frequency);
}

void cdsp::primitives::oscillators::base::frequency_next_set(types::sample frequency_next) {
	frequency.value_next_set(frequency_next);
}

void cdsp::primitives::oscillators::base::frequency_next_set(types::sample frequency_next, types::cont_64 delay_s) {
	frequency.value_next_set(frequency_next, delay_s);
}