//===----------------------------------------------------------------------===//
//                         DuckPGQ
//
// duckpgq/core/parser/property_graph_table.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/vector.hpp"
#include <duckdb/parser/tableref/basetableref.hpp>

namespace duckdb {

//! Represents a reference to a graph table from the CREATE PROPERTY GRAPH
class PropertyGraphTable {
public:
	//! Used for Copy
	PropertyGraphTable();
	//! Specify both the column and table name
	PropertyGraphTable(string table_name, vector<string> column_name,
		vector<string> label, string catalog_name = "",
		string schema = DEFAULT_SCHEMA);
	//! Specify both the column and table name with alias
	PropertyGraphTable(string table_name, string table_alias,
		vector<string> column_name, vector<string> label,
		string catalog_name = "", string schema = DEFAULT_SCHEMA);
	string table_name;
	string table_name_alias;

	//! The stack of names in order of which they appear (column_names[0], column_names[1], column_names[2], ...)
	vector<string> column_names;
	vector<string> column_aliases;

	vector<string> except_columns;

	vector<string> sub_labels;
	string main_label;

	string catalog_name;
	string schema_name;

	//! Associated with the PROPERTIES keyword not mentioned in the creation of table, equalling SELECT * in some sense
	bool all_columns = false;

	//! Associated with the NO PROPERTIES functionality
	bool no_columns = false;

	bool is_vertex_table = false;

	string discriminator;

	vector<string> source_fk;

	vector<string> source_pk;

	string source_catalog;
	string source_schema;
	string source_reference;

	shared_ptr<PropertyGraphTable> source_pg_table;

	vector<string> destination_fk;

	vector<string> destination_pk;

	string destination_catalog;
	string destination_schema;
	string destination_reference;

	shared_ptr<PropertyGraphTable> destination_pg_table;

public:
	string ToString() const;
	bool Equals(const PropertyGraphTable *other_p) const;

	shared_ptr<PropertyGraphTable> Copy() const;

	void Serialize(Serializer &serializer) const;

	static shared_ptr<PropertyGraphTable> Deserialize(Deserializer &deserializer);

	bool hasTableNameAlias() const {
		return !table_name_alias.empty();
	}

	string FullTableName() const {
		string full_table_name = catalog_name.empty() ? "" : catalog_name + ".";
		full_table_name += schema_name.empty() ? "" : schema_name + ".";
		full_table_name += table_name;
		return full_table_name;
	}

	unique_ptr<BaseTableRef> CreateBaseTableRef(const string &alias = "") const {
		auto base_table_ref = make_uniq<BaseTableRef>();
		base_table_ref->catalog_name = catalog_name;
		base_table_ref->schema_name = schema_name;
		base_table_ref->table_name = table_name;
		base_table_ref->alias = alias.empty() ? "" : alias;
		return base_table_ref;
	}

	bool IsSourceTable(const string &table_name);
};
} // namespace duckdb
