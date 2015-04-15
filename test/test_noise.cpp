#include "../src/cdsp.hpp"

using namespace cdsp;

int main(int argc, char* argv[])
{
	// create output buffer
	types::cont_64 length_seconds = 2.0;
	types::cont_64 sample_rate = 44100.0;
	types::index block_size = 4096;
	types::index output_buffer_length = static_cast<types::index>(length_seconds * sample_rate);
	sample_buffer output_buffer(1, output_buffer_length);
	output_buffer.clear();

	// create wavetable
	types::index table_length = 2048;
	sample_buffer table(1, table_length);
	helper::generator::sine(table_length, table.channel_pointer_write(0));

	// create noise gen
	primitive::generator::noise_white noise_gen;

	// prepare
	noise_gen.prepare(sample_rate, block_size);

	// perform
	types::index samples_remaining = output_buffer_length;
	types::index samples_completed = 0;
	types::index samples_current;
	while (samples_remaining > 0) {
		samples_current = samples_remaining >= block_size ? block_size : samples_remaining;

		noise_gen.perform(output_buffer, samples_current, 0, samples_completed);

		samples_remaining -= samples_current;
		samples_completed += samples_current;
	}

	// release
	noise_gen.release();

	// save
	helper::io::wav_file_save("test_noise.wav", sample_rate, 32, output_buffer, 0);

	return 0;
};