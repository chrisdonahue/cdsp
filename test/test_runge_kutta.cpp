#include "../src/cdsp.hpp"

using namespace cdsp;

#define PARAM_X (params[0])         /* 3 state variables x, y, and z */
#define STATE_Y (state[0])
#define DERIVATIVE_Y (derivatives[0])

static int params_num = 0;
static int state_num = 2;
static void y_t(types::sample* derivatives, const types::sample* state, const types::sample* params) {
	DERIVATIVE_Y = PARAM_X * sqrtf(STATE_Y);
};

int main(int argc, char* argv[])
{
	// create output buffer
	types::cont_64 sample_rate = 44100.0;
	types::index block_size = 1024;
	types::index output_buffer_length = block_size * 32;
	sample_buffer output_buffer(2, output_buffer_length);
	output_buffer.clear();

	// create state
	types::sample x0 = 0.0f, x1 = 10.0f, dx = 0.1f;
	int n = static_cast<int>((x1 - x0) / dx);
	types::sample y_prev = 0.0f;

	// create buffers
	sample_buffer sample_buffer_derivatives(1, 5 * state_num);
	types::sample* buffer_derivatives = sample_buffer_derivatives.channel_pointer_write(0);
	sample_buffer sample_buffer_state(1, n);
	types::sample* buffer_state = sample_buffer_state.channel_pointer_write(0);
	types::sample x;
	types::sample* y = buffer_state;

	// assign initial state
	x = 0.0f;
	*y = 1.0f; y++;

	// perform
	for (int i = 1; i < n; i++) {
		x = x0 + dx * (i - 1);
		*y = *(y - 1);
		helpers::runge_kutta_4(buffer_derivatives, state_num, &x, y_t, dx, y);
		y++;
	}
 
	// calculate error
	y = buffer_state;
	types::sample x_;
	types::sample y_estimated;
	types::sample y_exact;
	types::sample error = 0.0f;
	for (int i = 0; i < n; i += 10) {
		x_ = x0 + dx * i;
		y_estimated = y[i];
		y_exact = pow(x_ * x_ / 4 + 1, 2);
		error = y_estimated / y_exact - 1;
		continue;
	}
 
	return 0;
}