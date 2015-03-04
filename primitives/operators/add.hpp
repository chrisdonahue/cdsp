#ifndef CDSP_PRIMITIVES_OPERATORS_ADD
#define CDSP_PRIMITIVES_OPERATORS_ADD

#include "base.hpp"

namespace cdsp { namespace primitives { namespace operators {
	class add : public base {
	public:
		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0) {
			primitives::base::perform(buffer, block_size_leq, offset_channel, offset_sample);
		};
	};
}}}

#endif