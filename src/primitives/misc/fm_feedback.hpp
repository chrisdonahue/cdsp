#ifndef CDSP_PRIMITIVES_MISC_FM_FEEDBACK
#define CDSP_PRIMITIVES_MISC_FM_FEEDBACK

#include <cmath>

#include "../../helpers.hpp"
#include "../primitives_base.hpp"

namespace cdsp { namespace primitives { namespace misc {
	class fm_feedback : public primitives::base {
	public:
		fm_feedback();

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0);

		void ode(types::sample* derivatives, const types::sample* state, const types::sample* params);
	private:
		parameters::ramp_linear_automatic i;
		parameters::ramp_linear_automatic i_f;

		sample_buffer buffer_derivatives;
		sample_buffer buffer_state;
		sample_buffer buffer_parameters;
	};
}}}

#endif