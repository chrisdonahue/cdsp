#ifndef CDSP_PRIMITIVES_TABLES_OSCILLATOR
#define CDSP_PRIMITIVES_TABLES_OSCILLATOR

#include "../primitive_base.hpp"
#include "table_abstract.hpp"
#include "../../helper.hpp"

namespace cdsp { namespace primitive { namespace table { namespace oscillator {
	template <parameter::rate_audio frequency_type>
	class base :	public primitive::parameterized<parameter::rate_audio<types::sample> >, 
					public primitive::parameterized<frequency_type>,
					public table::abstract
	{
	public:
		base() :
			primitive::parameterized<parameter::rate_audio<types::sample> >(),
			primitive::parameterized<frequency_type>(),
			table::abstract(),
			phase(values::sample_zero, values::sample_zero, values::sample_one),
			frequency(static_cast<types::index>(1), values::sample_zero, values::sample_zero, values::sample_one) {
			parameter_expose("frequency", frequency);
		};
		//base(types::sample _phase_initial);
		//base(types::sample _phase_initial, types::sample _frequency);

		void table_set(types::index _table_length, const types::sample* _table);

		void prepare(prepare_signature);

		void phase_reset();
		void phase_initial_set(types::sample _phase_initial);

	protected:
		types::index table_mask;
		types::sample table_length_sample;

		types::sample table_index;
		parameter::rate_block<types::sample> phase;
		parameter::rate_block<types::sample> phase_initial;
		frequency_type frequency;
	};

	template <parameter::rate_audio frequency_type>
	class interpolate_4 : public base<frequency_type> {
	public:
		interpolate_4();
		interpolate_4(types::sample _phase);
		interpolate_4(types::sample _phase, types::sample _frequency);

		void perform(perform_signature_defaults);
	};
}}}}

#endif
