#ifndef CDSP_PRIMITIVES_TABLES_OSCILLATOR
#define CDSP_PRIMITIVES_TABLES_OSCILLATOR

#include "../primitive_base.hpp"
#include "table_abstract.hpp"
#include "../../helper.hpp"

namespace cdsp { namespace primitive { namespace table { namespace oscillator {
	class base :	public primitive::parameterized<parameter::rate_audio::schedule_ramp_static>,
					public table::abstract
	{
	public:
		base();
		base(types::sample _phase_initial);
		base(types::sample _phase_initial, types::sample _frequency);

		void table_set(types::index _table_length, const types::sample* _table);

		void prepare(prepare_signature);

		void phase_reset();
		void phase_initial_set(types::sample _phase_initial);

	protected:
		types::index table_mask;
		types::sample table_length_sample;

		types::sample table_index;
		parameter::rate_block<types::sample> phase_initial;
		parameter::rate_audio::schedule_ramp_static frequency;
	};

	class interpolate_4 : public base {
	public:
		interpolate_4();
		interpolate_4(types::sample _phase);
		interpolate_4(types::sample _phase, types::sample _frequency);

		void perform(perform_signature_defaults);
	};
}}}}

#endif
