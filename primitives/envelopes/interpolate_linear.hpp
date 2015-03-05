#ifndef CDSP_PRIMITIVES_ENVELOPES_INTERPOLATE_LINEAR
#define CDSP_PRIMITIVES_ENVELOPES_INTERPOLATE_LINEAR

#include "envelopes_base.hpp"

namespace cdsp { namespace primitives { namespace envelopes {
	class interpolate_linear : public base {
	public:
		interpolate_linear();
		interpolate_linear(types::sample _value_initial);
		interpolate_linear(types::sample _value_initial, std::vector<point>& _points);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0);
	
		void point_add(types::cont_64 delay_s, types::sample value);

	private:
		std::vector<std::pair<types::index, types::sample> > points_increments;
		types::index point_samples_remaining;
		types::sample point_increment_current;
	};
}}}

#endif