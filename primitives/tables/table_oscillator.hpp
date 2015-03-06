#ifndef CDSP_PRIMITIVES_TABLES_OSCILLATOR
#define CDSP_PRIMITIVES_TABLES_OSCILLATOR

#include "../../helpers.hpp"
#include "tables_base.hpp"

namespace cdsp { namespace primitives { namespace tables { namespace oscillator {
	class base : public tables::base {
	public:
		base();
		base(types::sample _phase);
		base(types::sample _phase, types::sample _frequency);

		void table_set(types::index _table_length, const types::sample* _table);

		void prepare(types::cont_64 _sample_rate, types::index _block_size);

		void phase_reset();
		void phase_set(types::sample _phase);

		void frequency_set(types::sample frequency);
		void frequency_next_set(types::sample frequency_next);
		void frequency_next_set(types::sample frequency_next, types::cont_64 delay_s);

	protected:
		types::index table_mask;
		types::sample table_length_sample;

		types::sample phase;
		parameters::ramp_linear_automatic frequency;
	};

	class interpolate_4 : public base {
	public:
		interpolate_4();
		interpolate_4(types::sample _phase);
		interpolate_4(types::sample _phase, types::sample _frequency);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0);
	};
}}}}

#endif
