#include "../src/cdsp.hpp"

using namespace cdsp;

int main(int argc, char* argv[])
{
	// create output buffer
	types::cont_64 sample_rate = 44100.0;
	types::index block_size = 4096;
	types::index output_buffer_length = block_size * 32;
	sample_buffer output_buffer(2, output_buffer_length);
	output_buffer.clear();

	// create wavetable
	types::index table_length = 1024;
	sample_buffer table(1, table_length);
	helper::generator::sine(table_length, table.channel_pointer_write(0));

	// create pm_voice
	abstraction::pm_voice pm_voice_1;
	pm_voice_1.table_set(table_length, table.channel_pointer_read(0));

	// prepare
	pm_voice_1.prepare(sample_rate, block_size);

	// set parameters
	pm_voice_1.f_m_set(50.0f / sample_rate);
	pm_voice_1.i_set(1.0f / values::two_pi_32);
	pm_voice_1.f_c_set(440.0f / sample_rate);
	pm_voice_1.a_set(0.5f);

	// perform
	types::index samples_completed = 0;
	types::index samples_remaining = output_buffer_length;
	while (samples_remaining > 0) {
		types::index block_size_current = block_size;
		if (samples_remaining < block_size) {
			block_size_current = samples_remaining;
		}

		pm_voice_1.perform(output_buffer, block_size_current, 0, samples_completed);

		samples_completed += block_size_current;
		samples_remaining -= block_size_current;
		
		if (samples_remaining == output_buffer_length / 2) {
			pm_voice_1.reset();
			pm_voice_1.f_m_set(100.0f / sample_rate);
			pm_voice_1.i_set(1.0f / values::two_pi_32);
			pm_voice_1.f_c_set(440.0f / sample_rate);
			pm_voice_1.a_set(0.5f);
		}
	}

	// resize
	output_buffer.resize(1, output_buffer_length);

	// save
	helper::io::wav_file_save("pm_voice_1_test.wav", sample_rate, 32, output_buffer);

	// release
	pm_voice_1.release();
}