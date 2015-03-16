#include "../src/cdsp.hpp"

using namespace cdsp;

int main(int argc, char* argv[])
{
	// create output buffer
	types::cont_64 sample_rate = 44100.0;
	types::index block_size = 1024;
	types::index output_buffer_length = block_size * 64;
	sample_buffer output_buffer(2, output_buffer_length);
	output_buffer.clear();

	// create wavetable
	types::index table_length = 1024;
	sample_buffer table(1, table_length);
	helpers::generators::sine(table_length, table.channel_pointer_write(0));

	// create pm_voice
	abstraction::pm_voice pm_voice_1;
	pm_voice_1.table_set(table_length, table.channel_pointer_read(0));

	// create envelope
	primitives::envelopes::interpolate_linear<4> env;

	// create multiplier
	primitives::operators::multiply multiplier(static_cast<types::channel>(2));

	// prepare
	pm_voice_1.prepare(sample_rate, block_size);
	env.prepare(sample_rate, block_size);
	multiplier.prepare(sample_rate, block_size);

	// set parameters
	pm_voice_1.f_m_set(50.0f / sample_rate);
	pm_voice_1.i_set(1.0f / values::two_pi_32);
	pm_voice_1.f_c_set(440.0f / sample_rate);
	pm_voice_1.a_set(1.0f);
	env.point_set(0, 0.01, 1.0);
	env.point_set(1, 0.1, 0.8);
	env.point_set(2, 0.5, 0.8);
	env.point_set(3, 0.3, 0.0);

	// perform
	types::index samples_completed = 0;
	types::index samples_remaining = output_buffer_length;
	while (samples_remaining > 0) {
		types::index block_size_current = block_size;
		if (samples_remaining < block_size) {
			block_size_current = samples_remaining;
		}

		pm_voice_1.perform(output_buffer, block_size_current, 0, samples_completed);
		env.perform(output_buffer, block_size_current, 1, samples_completed);
		multiplier.perform(output_buffer, block_size_current, 0, samples_completed);

		samples_completed += block_size_current;
		samples_remaining -= block_size_current;
	}

	// save
	helpers::io::wav_file_save("pm_enveloped.wav", sample_rate, 32, output_buffer, 0);

	// set parameters
	pm_voice_1.f_m_set(100.0f / sample_rate);
	pm_voice_1.i_set(1.0f / values::two_pi_32);
	pm_voice_1.f_c_set(440.0f / sample_rate);
	pm_voice_1.a_set(1.0f);
	env.point_set(0, 0.5, 1.0);
	env.point_set(1, 0.01, 0.6);
	env.point_set(2, 0.6, 0.7);
	env.point_set(3, 0.2, 0.1);

	// reset
	pm_voice_1.reset();
	env.reset();

	// perform
	samples_completed = 0;
	samples_remaining = output_buffer_length;
	while (samples_remaining > 0) {
		types::index block_size_current = block_size;
		if (samples_remaining < block_size) {
			block_size_current = samples_remaining;
		}

		pm_voice_1.perform(output_buffer, block_size_current, 0, samples_completed);
		env.perform(output_buffer, block_size_current, 1, samples_completed);
		multiplier.perform(output_buffer, block_size_current, 0, samples_completed);

		samples_completed += block_size_current;
		samples_remaining -= block_size_current;
	}

	// save
	helpers::io::wav_file_save("pm_enveloped_2.wav", sample_rate, 32, output_buffer, 0);


	// release
	pm_voice_1.release();
 
	return 0;
}