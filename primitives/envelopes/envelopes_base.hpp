#ifndef CDSP_PRIMITIVES_ENVELOPES_BASE
#define CDSP_PRIMITIVES_ENVELOPES_BASE

#include <utility>
#include <vector>

#include "../primitives_base.hpp"

namespace cdsp { namespace primitives { namespace envelopes {
	class base : public primitives::base {
	protected:
		typedef std::pair<types::sample, types::cont_64> point;

	public:
		base();
		base(types::sample _value_initial);
		base(types::sample _value_initial, std::vector<point>& _points);

		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0) = 0;

		void point_add(types::cont_64 delay_s, types::sample value);

	protected:
		types::sample value;

		types::sample value_initial;
		types::sample value_increment;
		std::vector<point> points;
		types::disc_32_u points_index_current;
	};
}}}

#endif
