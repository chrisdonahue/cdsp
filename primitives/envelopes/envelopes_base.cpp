#include "envelopes_base.hpp"

cdsp::primitives::envelopes::base::base() :
	primitives::base(),
	value(_value_initial),
	value_initial(_value_initial),
	value_increment(values::sample_zero),
	points(),
	points_index_current(0)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
};

cdsp::primitives::envelopes::base::base(types::sample _value_initial) :
	primitives::base(),
	value(_value_initial),
	value_initial(_value_initial),
	value_increment(values::sample_zero),
	points(),
	points_index_current(0)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
};

cdsp::primitives::envelopes::base::base(types::sample _value_initial, std::vector<point>& _points) :
	primitives::base(),
	value(_value_initial),
	value_initial(_value_initial),
	value_increment(values::sample_zero),
	points(),
	points_index_current(0)
{
	channels_input_num_set(0);
	channels_output_num_set(1);

	for (auto it : points) {
		point_add(it.first(), it.second());
	}
};

void cdsp::primitives::envelopes::base::point_add(types::cont_64 length_s, types::sample value) {
	points.push_back(std::make_pair(length_s, value));
};

void cdsp::primitives::envelopes::base::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::disc_32_u offset_sample) {
	primitives::base::perform(buffer, block_size_leq, offset_channel, offset_sample);
};
