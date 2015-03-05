#ifndef CDSP_PRIMITIVES_OSCILLATORS_BASE
#define CDSP_PRIMITIVES_OSCILLATORS_BASE

#include "../primitives_base.hpp"

namespace cdsp {
	namespace primitives {
		namespace oscillators {
			class base : public primitives::base {
			public:
				base();
				base(types::sample _phase, types::sample _frequency);

				void prepare(types::cont_64 _sample_rate, types::index _block_size);
				virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) = 0;

				void phase_set(types::sample _phase);

				void frequency_set(types::sample frequency);
				void frequency_next_set(types::sample frequency_next);
				void frequency_next_set(types::sample frequency_next, types::cont_64 delay_s);

			protected:
				types::sample phase;

				parameter::ramp_linear_automatic frequency;
			};
		}
	}
}

#endif
