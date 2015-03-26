#ifndef CDSP_PRIMITIVES_TABLE_ABSTRACT
#define CDSP_PRIMITIVES_TABLE_ABSTRACT

#include "../../types.hpp"

namespace cdsp { namespace primitive { namespace table {
	class abstract {
	public:
		abstract();
		virtual ~abstract() = 0;
		void table_set(types::index _table_length, const types::sample* _table);

	protected:
		types::index table_length;
		const types::sample* table;
	};

	inline abstract::~abstract() {};
}}}

#endif