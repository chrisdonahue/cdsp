#ifndef CDSP_PRIMITIVES_OSCILLATORS_PHASOR
#define CDSP_PRIMITIVES_OSCILLATORS_PHASOR

#include "../primitives_base.hpp"

namespace cdsp { namespace primitives { namespace oscillators {
	class phasor : public base {
	public:
		phasor();
		phasor(types::sample _phase, types::sample _frequency);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0);

		void phase_reset();
		void phase_set(types::sample _phase);

		void frequency_set(types::sample _frequency);
		void frequency_next_set(types::sample frequency_next);
		void frequency_next_set(types::sample frequency_next, types::cont_64 delay_s);

	protected:
		parameters::rate_block<types::sample> phase;
		parameters::ramp_linear_automatic frequency;
	};
}}}

#endif