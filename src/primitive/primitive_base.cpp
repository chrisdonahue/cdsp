#include "primitive_base.hpp"

void cdsp::primitive::base::perform(perform_signature) {
	dsp::perform(buffer, block_size_leq, offset_channel, offset_sample);

#ifdef CDSP_DEBUG_DSP
	types::channel required_num = channels_input_num > channels_output_num ? channels_input_num : channels_output_num;
	if (buffer.channels_num_get() < required_num + offset_channel) {
		throw exception::runtime("cdsp::dsp::perform: insufficient number of channels");
	}
	// TODO: check buffer length in regardess to block_size_leq and offset_sample
#endif
};