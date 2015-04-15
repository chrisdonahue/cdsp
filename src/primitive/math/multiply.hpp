#ifndef CDSP_PRIMITIVE_MATH_MULTIPLY
#define CDSP_PRIMITIVE_MATH_MULTIPLY

#include "operator_base.hpp"

namespace cdsp { namespace primitive { namespace math {
	class multiply : public operator_base {
	public:
		multiply() : operator_base() {};
		multiply(types::sample operand) : operator_base(operand) {};
		multiply(types::channel inputs_num) : operator_base(inputs_num) {};

		void perform(perform_signature_defaults);
	};
}}}

#endif