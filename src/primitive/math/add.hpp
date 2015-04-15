#ifndef CDSP_PRIMITIVE_MATH_ADD
#define CDSP_PRIMITIVE_MATH_ADD

#include "operator_base.hpp"

namespace cdsp { namespace primitive { namespace math {
	class add : public operator_base {
	public:
		add() : operator_base() {};
		add(types::sample operand) : operator_base(operand) {};
		add(types::channel inputs_num) : operator_base(inputs_num) {};

		void perform(perform_signature_defaults);
	};
}}}

#endif