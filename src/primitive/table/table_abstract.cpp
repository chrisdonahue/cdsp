#include "table_abstract.hpp"

cdsp::primitive::table::abstract::abstract() :
	table_length(0),
	table(nullptr)
{};

void cdsp::primitive::table::abstract::table_set(types::index _table_length, const types::sample* _table) {
	table_length = _table_length;
	table = _table;
};