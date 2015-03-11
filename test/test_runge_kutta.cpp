#include "../src/cdsp.hpp"

using namespace cdsp;

#define STATE_T (state[0])         /* 3 state variables x, y, and z */
#define STATE_Y (state[1])
#define DERIVATIVE_Y (derivatives[0])

static int params_num = 0;
static int state_num = 2;
static void y_t(types::sample* derivatives, const types::sample* state, const types::sample* params) {
	DERIVATIVE_Y = STATE_T * sqrtf(STATE_Y);
};

int main(int argc, char* argv[])
{
	// create output buffer
	types::cont_64 sample_rate = 44100.0;
	types::index block_size = 1024;
	types::index output_buffer_length = block_size * 32;
	sample_buffer output_buffer(2, output_buffer_length);
	output_buffer.clear();

	// create runge kutta buffers
	sample_buffer sample_buffer_derivatives(1, 5 * state_num);
	types::sample* buffer_derivatives = sample_buffer_derivatives.channel_pointer_write(0);

	// perform
	types::sample error = 0.0f;
	types::sample* y, x, y2;
	types::sample x0 = 0, x1 = 10, dx = 0.1;
	int i, n = 1 + (x1 - x0) / dx;
	sample_buffer buffer_y(1, n);
	y = buffer_y.channel_pointer_write(0);

	for (y[0] = 1, i = 1; i < n; i++)
		helpers::runge_kutta_4(buffer_derivatives, state_num, nullptr, y_t, dx, y);
 
	for (i = 0; i < n; i += 10) {
		x = x0 + dx * i;
		y2 = pow(x * x / 4 + 1, 2);
		error += y[i]/y2 - 1;
	}
 
	return 0;
}