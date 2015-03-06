#ifndef CDSP_PRIMITIVES_TABLES_FUNCTION
#define CDSP_PRIMITIVES_TABLES_FUNCTION

#include "../../helpers.hpp"
#include "tables_base.hpp"

namespace cdsp { namespace primitives { namespace tables { namespace function {
	class base : public tables::base {
	public:
		base();
		base(types::sample x_value);
		base(types::sample x_value_min, types::sample x_value_max);
		base(types::sample x_value, types::sample x_value_min, types::sample x_value_max);

		void domain_set(types::sample x_value_min, types::sample x_value_max);
		void table_set(types::index _table_length, const types::sample* _table);

		void prepare(types::cont_64 _sample_rate, types::index _block_size);

		void x_set(types::sample _x);
		void x_next_set(types::sample x_next);
		void x_next_set(types::sample x_next, types::cont_64 delay_s);

	protected:
		parameters::ramp_linear_automatic x;
		types::sample x_m;
		types::sample x_b;
	};

	class interpolate_4 : public base {
	public:
		interpolate_4();
		interpolate_4(types::sample _phase);

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0);
	};
}}}}

#endif
