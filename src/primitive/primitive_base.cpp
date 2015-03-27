#include "primitive_base.hpp"

void cdsp::primitive::base::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	dsp::perform(buffer, block_size_leq, offset_channel, offset_sample);

#ifdef CDSP_DEBUG_DSP
	types::channel required_num = channels_input_num > channels_output_num ? channels_input_num : channels_output_num;
	if (buffer.channels_num_get() < required_num + offset_channel) {
		throw exception::runtime("cdsp::dsp::perform: insufficient number of channels");
	}
	// TODO: check buffer length in regardess to block_size_leq and offset_sample
#endif
};

template <>
void cdsp::primitive::parameterized<cdsp::parameter::rate_audio::base>::prepare(prepare_signature) {
	parameterized::prepare(prepare_arguments);
	for (auto it : parameters) {
		it.second->prepare(prepare_arguments);
	}
};

template <>
void cdsp::primitive::parameterized<cdsp::parameter::rate_audio::base>::release() {
	parameterized::release();
	for (auto it : parameters) {
		it.second->release();
	}
};