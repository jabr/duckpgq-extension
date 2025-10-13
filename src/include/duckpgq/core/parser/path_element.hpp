#pragma once
#include "duckpgq/core/parser/path_reference.hpp"

namespace duckdb {

enum class PGQMatchType : uint8_t {
	MATCH_VERTEX = 0,
	MATCH_EDGE_ANY = 1,
	MATCH_EDGE_LEFT = 2,
	MATCH_EDGE_RIGHT = 3,
	MATCH_EDGE_LEFT_RIGHT = 4
};
template<>
const char* EnumUtil::ToChars<PGQMatchType>(PGQMatchType value);
template<>
PGQMatchType EnumUtil::FromString<PGQMatchType>(const char *value);

class PathElement : public PathReference {
public:
	PGQMatchType match_type;

	std::string label;

	std::string variable_binding;

public:
	explicit PathElement(PGQPathReferenceType path_reference_type) : PathReference(path_reference_type) {
	}

	string ToString() const override;

	unique_ptr<PathReference> Copy() override;

	bool Equals(const PathReference *other_p) const override;

	void Serialize(Serializer &serializer) const override;

	static unique_ptr<PathReference> Deserialize(Deserializer &deserializer);
};
} // namespace duckdb
