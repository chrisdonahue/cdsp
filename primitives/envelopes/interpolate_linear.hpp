#ifndef CDSP_PRIMITIVES_ENVELOPES_INTERPOLATE_LINEAR
#define CDSP_PRIMITIVES_ENVELOPES_INTERPOLATE_LINEAR

#include "envelopes_base.hpp"

namespace cdsp { namespace primitives { namespace envelopes {
	class interpolate_linear : public base {
	public:
		interpolate_linear();
		interpolate_linear(types::sample _value_initial);
		interpolate_linear(types::sample _value_initial, std::vector<point>& _points);

		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0) = 0;
	
	private:
		std::vector<types::index, types::sample> points_increments;
	};
}}}

#endif
