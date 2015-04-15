#include "noise.hpp"

void cdsp::primitive::generator::noise_white::perform(perform_signature) {
	primitive::base::perform(perform_arguments);

	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	types::index samples_remaining = block_size_leq;
	while (samples_remaining) {
		// from pd/d_osc.c
		*(output) = 
			static_cast<types::sample>((pseudorandom_state & 0x7fffffff) - 0x40000000) *
			static_cast<types::sample>(1.0 / 0x40000000);
		output++;

		pseudorandom_state = pseudorandom_state * 435898247 + 382842987;

		samples_remaining--;
	}
};