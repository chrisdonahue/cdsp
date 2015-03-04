#ifndef CDSP_PRIMITIVES_OPERATORS_ADD
#define CDSP_PRIMITIVES_OPERATORS_ADD

#include "operators_base.hpp"

namespace cdsp { namespace primitives { namespace operators {
	class add : public base {
	public:
		using base::base;

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0);
	};
}}}

#endif