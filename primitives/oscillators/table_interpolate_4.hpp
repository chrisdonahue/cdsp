#ifndef CDSP_PRIMITIVES_OSCILLATORS_TABLE_INTERPOLATE_4
#define CDSP_PRIMITIVES_OSCILLATORS_TABLE_INTERPOLATE_4

#include "../../helpers.hpp"
#include "oscillators_base.hpp"

namespace cdsp { namespace primitives { namespace oscillators {
	class table_interpolate_4 : public base {
	public:
		table_interpolate_4();
		table_interpolate_4(types::sample _phase, types::sample _frequency);

		void table_set(types::disc_32_u _table_length, const types::sample* _table);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::disc_32_u offset_sample = 0);

	private:
		types::sample table_length;
		types::disc_32_u table_mask;
		const types::sample* table;
	};
}}}

#endif
