#ifndef CDSP_PRIMITIVES_TABLES_PHASOR
#define CDSP_PRIMITIVES_TABLES_PHASOR

#include "../primitive_base.hpp"
#include "table_abstract.hpp"

namespace cdsp { namespace primitive { namespace table { namespace phasor {
	class base :	public primitive::base,
					public table::abstract
	{
	public:
		base();

		void table_set(types::index _table_length, const types::sample* _table);

	protected:
		types::index table_mask;
		types::sample table_length_sample;
	};

	class interpolate_4 : public base {
	public:
		interpolate_4();

		void perform(sample_buffer& buffer, types::disc_32_u block_size_leq, types::channel offset_channel = 0, types::index offset_sample = 0);
	};
}}}}

#endif
