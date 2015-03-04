#include "base.hpp"

void cdsp::dsp::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::disc_32_u offset_sample) {
	CDSP_UNUSED_PARAMETER(buffer);
	CDSP_UNUSED_PARAMETER(block_size_leq);
	CDSP_UNUSED_PARAMETER(offset_channel);
	CDSP_UNUSED_PARAMETER(offset_sample);

#ifdef CDSP_DEBUG_DSP
	if (!prepared) {
		throw exceptions::runtime("cdsp::dsp: perform called while not prepared");
	}
#endif
};
