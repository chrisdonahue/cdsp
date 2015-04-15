#include "operator_base.hpp"

cdsp::primitive::math::operator_base::base::base() :
	primitive::base(),
	operand_use(false),
	operand(values::sample_silence)
{
	channels_input_num_set(2);
	channels_output_num_set(1);
};

cdsp::primitive::math::operator_base::base::base(types::channel operands_num) :
	primitive::base(),
	operand_use(false),
	operand(values::sample_silence)
{
	channels_input_num_set(operands_num);
	channels_output_num_set(1);
};

cdsp::primitive::math::operator_base::base::base(types::sample _operand) :
	primitive::base(),
	operand_use(true),
	operand(_operand)
{
	channels_input_num_set(1);
	channels_output_num_set(1);
};

void cdsp::primitive::math::operator_base::base::operands_num_set(types::channel operands_num) {
	channels_input_num_set(operands_num);
};

void cdsp::primitive::math::operator_base::base::operand_set(types::sample _operand) {
	operand.value_set(_operand);
};

void cdsp::primitive::math::operator_base::base::operand_next_set(types::sample operand_next) {
	operand.value_next_set(operand_next);
};

void cdsp::primitive::math::operator_base::base::operand_next_set(types::sample operand_next, types::cont_64 delay_s) {
	operand.value_next_set(operand_next, delay_s);
};

void cdsp::primitive::math::operator_base::base::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	primitive::base::perform(buffer, block_size_leq, offset_channel, offset_sample);
};
