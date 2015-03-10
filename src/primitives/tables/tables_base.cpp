#include "tables_base.hpp"

cdsp::primitives::tables::base::base() :
	primitives::base(),
	table(nullptr)
{
	channels_input_num_set(0);
	channels_output_num_set(1);
};

void cdsp::primitives::tables::base::table_set(types::index _table_length, const types::sample* _table) {
	table_length = _table_length;
	table = _table;
};