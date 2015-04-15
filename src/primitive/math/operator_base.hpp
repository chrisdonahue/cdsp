#ifndef CDSP_PRIMITIVES_OPERATORS_BASE
#define CDSP_PRIMITIVES_OPERATORS_BASE

#include "../primitive_base.hpp"

namespace cdsp { namespace primitive { namespace math {
	class operator_base : public primitive::base {
	public:
		operator_base();
		operator_base(types::sample operand);
		operator_base(types::channel inputs_num);

		virtual void perform(perform_signature_defaults) = 0;

		void operand_set(types::sample _operand);
		void inputs_num_set(types::channel inputs_num);

	protected:
		types::boolean operand_use;
		parameter::base<types::sample> operand;
	};
}}}

#endif