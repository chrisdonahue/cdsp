#ifndef CDSP_PRIMITIVE_GENERATOR_IMPULSE
#define CDSP_PRIMITIVE_GENERATOR_IMPULSE

#include "../primitive_base.hpp"

namespace cdsp { namespace primitive { namespace generator {
	class impulse :	public primitive::base
	{
	public:
		impulse(types::boolean impulse_state=values::boolean_true) :
			primitive::base(0, 1)
		{};

		void trigger() {
			impulse_state = values::boolean_true;
		};

		void perform(perform_signature_defaults);

	private:
		types::boolean impulse_state;
	};
}}}

#endif
