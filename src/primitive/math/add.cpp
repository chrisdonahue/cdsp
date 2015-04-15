#include "add.hpp"

void cdsp::primitive::math::add::perform(perform_signature) {
	operator_base::perform(perform_arguments);

	if (operand_use) {
		types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
		types::sample addend = operand.value_get();
		types::index samples_remaining_num = block_size_leq;
		while (samples_remaining_num--) {
			*(output++) += addend;
		}
	}
	else {
		for (types::channel channel_input = offset_channel + 1; channel_input < offset_channel + channels_input_num_get(); channel_input++) {
			types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
			const types::sample* input = buffer.channel_pointer_read(channel_input, offset_sample);
			types::index samples_remaining_num = block_size_leq;
			while (samples_remaining_num--) {
				*(output++) += *(input++);
			}
		}
	}
};