#ifndef CDSP_PRIMITIVES_ENVELOPES_BASE
#define CDSP_PRIMITIVES_ENVELOPES_BASE

#include <utility>
#include <vector>

#include "../primitives_base.hpp"

namespace cdsp { namespace primitives { namespace envelopes {
	template <types::disc_32_u points_num>
	class base : public primitives::base {
	protected:
		typedef std::pair<types::time, types::sample> point;

	public:
		base() :
			primitives::base(),
			value(values::sample_silence),
			value_initial(values::sample_silence),
			points(points_num),
			points_index_current(0)
		{
			channels_input_num_set(0);
			channels_output_num_set(1);
		};

		base(types::sample _value_initial) :
			primitives::base(),
			value(_value_initial),
			value_initial(_value_initial),
			points(points_num),
			points_index_current(0)
		{
			channels_input_num_set(0);
			channels_output_num_set(1);
		};

		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) = 0 {
			primitives::base::perform(buffer, block_size_leq, offset_channel, offset_sample);
		};

		void reset() {
			value = value_initial;
			points_index_current = 0;
		}

		void point_set(types::disc_32_u point_index, types::time length_s, types::sample value) {
			if (point_index >= points_num) {
				throw exceptions::runtime("cdsp::primitives::envelopes::base::point_set: specified point_index out of bounds");
			}
			points.at(point_index) = std::make_pair(length_s, value);
		};

	protected:
		types::sample value;

		types::sample value_initial;
		std::vector<point> points;
		types::disc_32_u points_index_current;
	};
}}}

#endif
