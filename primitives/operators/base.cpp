#include "base.hpp"

cdsp::primitives::operators::base::base() :
	primitives::base(),
	operand_use(false),
	operand(values::sample_silence)
{
	channels_input_num_set(2);
	channels_output_num_set(1);
};

cdsp::primitives::operators::base::base(types::channel operands_num) :
	primitives::base(),
	operand_use(false),
	operand(values::sample_silence)
{
	channels_input_num_set(operands_num);
	channels_output_num_set(1);
};

cdsp::primitives::operators::base::base(types::sample _operand) :
	primitives::base(),
	operand_use(true),
	operand(_operand)
{
	channels_input_num_set(1);
	channels_output_num_set(1);
};

void cdsp::primitives::operators::base::operands_num_set(types::channel operands_num) {
	channels_input_num_set(operands_num);
};

void cdsp::primitives::operators::base::operand_set(types::sample _operand) {
	operand.value_set(_operand);
};

void cdsp::primitives::operators::base::operand_next_set(types::sample operand_next) {
	operand.value_next_set(operand_next);
};

void cdsp::primitives::operators::base::operand_next_set(types::sample operand_next, types::cont_64 delay_s) {
	operand.value_next_set(operand_next, delay_s);
};