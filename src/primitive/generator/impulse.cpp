#include "impulse.hpp"

void cdsp::primitive::generator::impulse::perform(perform_signature) {
	primitive::base::perform(perform_arguments);

	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	types::index samples_remaining = block_size_leq;
	while (samples_remaining) {
		*(output) = static_cast<types::sample>(impulse_state);
		output++;

		impulse_state = values::boolean_false;

		samples_remaining--;
	}
};