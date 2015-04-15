#ifndef CDSP_PRIMITIVE_GENERATOR_NOISE
#define CDSP_PRIMITIVE_GENERATOR_NOISE

#include "../primitive_base.hpp"

namespace cdsp { namespace primitive { namespace generator {
	class noise_white :	public primitive::base
	{
	public:
		noise_white() :
			primitive::base(0, 1),
			pseudorandom_state(307 * 1319)
		{};

		void perform(perform_signature_defaults);

	private:
		types::disc_32 pseudorandom_state;
	};
}}}

#endif
