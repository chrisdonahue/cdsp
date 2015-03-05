#include "multiply.hpp"

void cdsp::primitives::operators::multiply::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);

	if (operand_use) {
		types::sample multiplyend = operand.value_get();
		types::disc_32_u samples_remaining_num = block_size_leq;
		while (samples_remaining_num--) {
			*(output++) *= multiplyend;
		}
	}
	else {
		for (types::channel channel_input = 1; channel_input < channels_input_num_get(); channel_input++) {
			const types::sample* input = buffer.channel_pointer_read(offset_channel + channel_input, offset_sample);
			types::disc_32_u samples_remaining_num = block_size_leq;
			while (samples_remaining_num--) {
				*(output++) *= *(input++);
			}
		}
	}
};
