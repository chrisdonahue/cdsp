#ifndef CDSP_PRIMITIVES_OPERATORS_BASE
#define CDSP_PRIMITIVES_OPERATORS_BASE

#include "../primitives_base.hpp"

namespace cdsp { namespace primitives { namespace operators {
	class base : public primitives::base {
	public:
		base();
		base(types::channel operands_num);
		base(types::sample operand);

		void operands_num_set(types::channel operands_num);

		void operand_set(types::sample _operand);
		void operand_next_set(types::sample operand_next);
		void operand_next_set(types::sample operand_next, types::cont_64 delay_s);

		virtual void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0) = 0;

	protected:
		types::boolean operand_use;
		parameters::ramp_linear_automatic operand;
	};
}}}

#endif