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
