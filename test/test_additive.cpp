#include "../src/cdsp.hpp"

using namespace cdsp;

int main(int argc, char* argv[])
{
	// create output buffer
	types::cont_64 length_seconds = 2.0;
	types::cont_64 sample_rate = 44100.0;
	types::index block_size = 4096;
	types::index output_buffer_length = static_cast<types::index>(length_seconds * sample_rate);
	sample_buffer output_buffer(3, output_buffer_length);
	output_buffer.clear();

	// create wavetable
	types::index table_length = 2048;
	sample_buffer table(1, table_length);
	helper::generator::sine(table_length, table.channel_pointer_write(0));

	// create oscillators
	primitive::table::oscillator::interpolate_4 osc_1(values::sample_zero, static_cast<types::sample>(130.8127826503 * 2.0 / sample_rate));
	primitive::table::oscillator::interpolate_4 osc_2(values::sample_zero, static_cast<types::sample>(164.8137784564 * 2.0 / sample_rate));
	primitive::table::oscillator::interpolate_4 osc_3(values::sample_zero, static_cast<types::sample>(220.0000000000 * 2.0 / sample_rate));
	osc_1.table_set(table_length, table.channel_pointer_read(0));
	osc_2.table_set(table_length, table.channel_pointer_read(0));
	osc_3.table_set(table_length, table.channel_pointer_read(0));

	// create adder
	primitive::math::add adder(static_cast<types::channel>(3));

	// create gain
	primitive::math::multiply gain(static_cast<types::sample>(1.0 / 3.0));

	// create envelope
	primitive::envelope::schedule_linear_dynamic envelope(values::sample_zero, values::sample_zero, values::sample_one);

	// create gain
	primitive::math::multiply gain_2(static_cast<types::channel>(2));

	// prepare
	osc_1.prepare(sample_rate, block_size);
	osc_2.prepare(sample_rate, block_size);
	osc_3.prepare(sample_rate, block_size);
	adder.prepare(sample_rate, block_size);
	gain.prepare(sample_rate, block_size);
	envelope.prepare(sample_rate, block_size);
	gain_2.prepare(sample_rate, block_size);

	// schedule
	envelope.schedule(static_cast<types::time>(0.005), static_cast<types::sample>(1.0));
	envelope.schedule(static_cast<types::time>(0.22), static_cast<types::sample>(0.8));
	envelope.schedule(static_cast<types::time>(0.4), static_cast<types::sample>(0.8));
	envelope.schedule(static_cast<types::time>(2.0), static_cast<types::sample>(0.0));

	// perform
	types::index samples_remaining = output_buffer_length;
	types::index samples_completed = 0;
	types::index samples_current;
	while (samples_remaining > 0) {
		samples_current = samples_remaining >= block_size ? block_size : samples_remaining;

		osc_1.perform(output_buffer, samples_current, 0, samples_completed);
		osc_2.perform(output_buffer, samples_current, 1, samples_completed);
		osc_3.perform(output_buffer, samples_current, 2, samples_completed);
		adder.perform(output_buffer, samples_current, 0, samples_completed);
		gain.perform(output_buffer, samples_current, 0, samples_completed);
		envelope.perform(output_buffer, samples_current, 1, samples_completed);
		gain_2.perform(output_buffer, samples_current, 0, samples_completed);

		samples_remaining -= samples_current;
		samples_completed += samples_current;
	}

	// release
	osc_1.release();
	osc_2.release();
	osc_3.release();
	adder.release();
	gain.release();
	envelope.release();
	gain_2.release();

	// save
	helper::io::wav_file_save("test_additive.wav", sample_rate, 32, output_buffer, 0);

	return 0;
};