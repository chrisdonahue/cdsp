#ifndef CDSP_PRIMITIVES_TABLES_BASE
#define CDSP_PRIMITIVES_TABLES_BASE

#include "../primitives_base.hpp"

namespace cdsp { namespace primitives { namespace tables {
	class base {
	public:
		virtual ~base() = 0;
		void table_set(types::index _table_length, const types::sample* _table);

	protected:
		types::index table_length;
		const types::sample* table;
	};
}}}

#endif