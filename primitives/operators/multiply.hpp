#ifndef CDSP_PRIMITIVES_OPERATORS_MULTIPLY
#define CDSP_PRIMITIVES_OPERATORS_MULTIPLY

#include "operators_base.hpp"

namespace cdsp {
	namespace primitives {
		namespace operators {
			class multiply : public base {
			public:
				using base::base;

				void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0);
			};
		}
	}
}

#endif