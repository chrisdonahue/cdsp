#ifndef CDSP_PRIMITIVES_OSCILLATORS_PHASOR
#define CDSP_PRIMITIVES_OSCILLATORS_PHASOR

#include "oscillators_base.hpp"

namespace cdsp { namespace primitives { namespace oscillators {
	class phasor : public base {
	public:
		phasor();
		phasor(types::sample _phase, types::sample _frequency);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0);
	};
}}}

#endif