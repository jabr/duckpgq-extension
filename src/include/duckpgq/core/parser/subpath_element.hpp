
#pragma once

#include "duckpgq/core/parser/path_reference.hpp"

namespace duckdb {

enum class PGQPathMode : uint8_t { NONE, WALK, SIMPLE, TRAIL, ACYCLIC };
template<>
const char* EnumUtil::ToChars<PGQPathMode>(PGQPathMode value);
template<>
PGQPathMode EnumUtil::FromString<PGQPathMode>(const char *value);

class SubPath : public PathReference {

public:
	vector<unique_ptr<PathReference>> path_list;

	unique_ptr<ParsedExpression> where_clause;

	PGQPathMode path_mode;

	bool single_bind;
	int64_t lower, upper;

	string path_variable;

	// TODO cost_expr, default_value
public:
	explicit SubPath(PGQPathReferenceType path_reference_type) : PathReference(path_reference_type) {
		path_mode = PGQPathMode::NONE;
		single_bind = true;
		lower = 1;
		upper = 1;
	}

	string ToString() const override;

	unique_ptr<PathReference> Copy() override;

	bool Equals(const PathReference *other_p) const override;

	void Serialize(Serializer &serializer) const override;

	static unique_ptr<PathReference> Deserialize(Deserializer &deserializer);
};

} // namespace duckdb
