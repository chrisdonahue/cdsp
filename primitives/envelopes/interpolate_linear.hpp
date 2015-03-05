#ifndef CDSP_PRIMITIVES_OPERATORS_BASE
#define CDSP_PRIMITIVES_OPERATORS_BASE

#include <tuple>
#include <vector>

#include "envelopes_base.hpp"

namespace cdsp { namespace primitives { namespace envelopes {
	class interpolate_linear : public base {
	public:
		interpolate();
		interpolate(types::sample _value_initial);
		interpolate(types::sample _value_initial, std::vector<point>& _points);

		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0) = 0;
	};
}}}

#endif
