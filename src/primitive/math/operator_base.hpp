#ifndef CDSP_PRIMITIVES_OPERATORS_BASE
#define CDSP_PRIMITIVES_OPERATORS_BASE

#include "../primitive_base.hpp"

namespace cdsp { namespace primitive { namespace math {
	class operator_base : public primitive::base {
	public:
		operator_base();
		operator_base(types::channel inputs_num);
		operator_base(types::sample operand);

		void operands_num_set(types::channel inputs_num);

		void operand_set(types::sample _operand);
		void operand_next_set(types::sample operand_next);
		void operand_next_set(types::sample operand_next, types::cont_64 delay_s);

		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) = 0;

	protected:
		types::boolean operand_use;
		parameter::base<types::sample> operand;
	};
}}}

#endif