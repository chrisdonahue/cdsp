#include "envelopes_base.hpp"

cdsp::primitives::envelopes::base::base() :
	primitives::base()
{
	channels_input_num_set(0);
	channels_output_num_set(1);
};

cdsp::primitives::envelopes::base::base(types::sample _value_initial) :
	primitives::base(),
	value(_value_initial),
	value_initial(_value_initial),
	value_increment(values::sample_zero)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
};

cdsp::primitives::envelopes::base::base(types::sample _value_initial, std::vector<point>& _points) :
	primitives::base(),
	value(_value_initial),
	value_initial(_value_initial),
	value_increment(values::sample_zero),
	points(_points)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
};

void cdsp::primitives::envelopes::base::point_add(types::cont_64 length_s, types::sample value) {
	points.push_back(std::make_tuple(length_s, value));
};
