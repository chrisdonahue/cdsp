#include "add.hpp"

void cdsp::primitives::operators::add::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0) {
	primitives::base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	if (operand_use) {

	}
	else {
		types::sample* output = buffer.channel_pointer_write(offset_sample);
		for (
	}
};