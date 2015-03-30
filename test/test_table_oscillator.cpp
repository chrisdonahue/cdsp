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

	// create oscillator
	primitive::table::oscillator::interpolate_4<> osc_1(values::sample_zero, static_cast<types::sample>(440.0 / sample_rate));

	// set table
	osc_1.table_set(table_length, table.channel_pointer_read(0));

	// prepare
	osc_1.prepare(sample_rate, block_size);

	// set next
	parameter::rate_audio::schedule_ramp_dynamic& osc_1_freq = osc_1.frequency_parameter_get();

	// perform
	types::index samples_remaining = output_buffer_length;
	types::index samples_completed = 0;
	types::index samples_current;
	while (samples_remaining > 0) {
		samples_current = samples_remaining >= block_size ? block_size : samples_remaining;

		osc_1.perform(output_buffer, samples_current, 0, samples_completed);

		samples_remaining -= samples_current;
		samples_completed += samples_current;
	}

	// release
	osc_1.release();

	// save
	helper::io::wav_file_save("test_table_oscillator.wav", sample_rate, 32, output_buffer, 0);
};