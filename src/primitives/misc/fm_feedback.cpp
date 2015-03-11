#include "fm_feedback.hpp"

cdsp::primitives::misc::fm_feedback::fm_feedback() :
	primitives::base(),
	i(values::sample_one), i_f(values::sample_zero),
	buffer_derivatives(1, 5),
	buffer_state(1, 1),
	buffer_parameters(5, 1)
{
	channels_input_num_set(1);
	channels_output_num_set(1);
}

void cdsp::primitives::misc::fm_feedback::perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel, types::index offset_sample) {
	base::perform(buffer, block_size_leq, offset_channel, offset_sample);

	// get parameter values
	// TODO: make buffers for params in prepare
	const types::sample* i_buffer;
	i.perform(buffer, block_size_leq, offset_channel, offset_sample);
	i_buffer = buffer.channel_pointer_read(offset_channel, offset_sample);

	// create state
	types::sample x;
	types::index table_index;
	types::sample table_index_fraction;
	types::sample inm1;
	types::sample in0;
	types::sample inp1;
	types::sample inp2;

	// perform
	types::index samples_remaining = block_size_leq;
	types::sample* output = buffer.channel_pointer_write(offset_channel, offset_sample);
	while (samples_remaining--) {
		// find current function value
		x = *(x_buffer++);
};

#define PARAM_T (parameters[0])
#define PARAM_I (parameters[1])
#define PARAM_MODULATOR (parameters[2])
#define PARAM_F_C (parameters[3])
#define PARAM_I_F (parameters[4])
#define STATE_Y (state[0])
#define DERIVATIVE_Y (derivatives[0])
void cdsp::primitives::misc::fm_feedback::ode(types::sample* derivatives, const types::sample* state, const types::sample* parameters) {
	DERIVATIVE_Y = PARAM_F_C * cosf(PARAM_MODULATOR + (values::two_pi_32 * PARAM_F_C * PARAM_T) + PARAM_I_F * STATE_Y);
};