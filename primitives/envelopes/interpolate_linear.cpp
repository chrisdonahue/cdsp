#include "interpolate_linear.hpp"

cdsp::primitives::envelopes::interpolate_linear::interpolate_linear() :
	base()
{
};

cdsp::primitives::envelopes::interpolate_linear::interpolate_linear(types::sample _value_initial) :
	base(_value_initial)
{
};

cdsp::primitives::envelopes::interpolate_linear::interpolate_linear(types::sample _value_initial, std::vector<point>& _points) :
	base(_value_initial, _points)
{
};

void cdsp::primitives::envelopes::interpolate_linear::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::disc_32_u offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);
};

void cdsp::primitives::envelopes::interpolate_linear::point_add(types::cont_64 delay_s, types::sample value) {
	base::point_add(delay_s, value);

	// get points size
	types::disc_32_u points_size = points.size();
	if (points_size == 0) {
		throw exceptions::runtime("cdsp::primitives::envelopes::interpolate_linear::point_add: point was not added to base class");
	}

	// calculate length of segment in samples
	types::index point_length = static_cast<types::index>(delay_s * sample_rate);

	// calculate linear increment
	types::sample point_increment;
	if (points_size == 1) {
		point_increment = (value - value_initial)/static_cast<types::sample>(point_length);
	}
	else {
		point_increment = (value - points[points_size - 2].second())/static_cast<types::sample>(point_length);
	}

	// add point increment
	points_increments.push_back(std::make_pair(point_length, point_increment));
};
