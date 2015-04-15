#include "operator_base.hpp"

cdsp::primitive::math::operator_base::operator_base() :
	primitive::base(0, 1),
	operand_use(values::boolean_true),
	operand(values::sample_silence)
{};

cdsp::primitive::math::operator_base::operator_base(types::sample _operand) :
	primitive::base(),
	operand_use(values::boolean_true),
	operand(_operand)
{};

cdsp::primitive::math::operator_base::operator_base(types::channel inputs_num) :
	primitive::base(inputs_num, 1),
	operand_use(values::boolean_false),
	operand(values::sample_silence)
{};

void cdsp::primitive::math::operator_base::inputs_num_set(types::channel inputs_num) {
	channels_input_num_set(inputs_num);
};

void cdsp::primitive::math::operator_base::operand_set(types::sample _operand) {
	operand.value_set(_operand);
};

void cdsp::primitive::math::operator_base::perform(perform_signature) {
	primitive::base::perform(perform_arguments);
};