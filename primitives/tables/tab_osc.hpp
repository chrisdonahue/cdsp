#ifndef CDSP_PRIMITIVES_TABLES_TAB_OSC
#define CDSP_PRIMITIVES_TABLES_TAB_OSC

#include "../../helpers.hpp"
#include "oscillators_base.hpp"

namespace cdsp { namespace primitives { namespace tables {
	class tab_osc : public base {
	public:
		tab_osc();
		tab_osc(types::sample _phase);
		tab_osc(types::sample _phase, types::sample _frequency);

		void table_set(types::disc_32_u _table_length, const types::sample* _table);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0);

		void phase_reset();
		void phase_set(types::sample _phase);

		void frequency_set(types::sample frequency);
		void frequency_next_set(types::sample frequency_next);
		void frequency_next_set(types::sample frequency_next, types::cont_64 delay_s);

	protected:
		types::sample table_length;
		types::disc_32_u table_mask;
		const types::sample* table;

		types::sample phase;
		parameter::ramp_linear_automatic frequency;
	};
}}}

#endif
