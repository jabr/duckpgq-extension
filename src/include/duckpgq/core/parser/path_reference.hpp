#pragma once

namespace duckdb {

enum class PGQPathReferenceType : uint8_t { PATH_ELEMENT = 0, SUBPATH = 1, UNKNOWN = 2 };
template<>
const char* EnumUtil::ToChars<PGQPathReferenceType>(PGQPathReferenceType value);
template<>
PGQPathReferenceType EnumUtil::FromString<PGQPathReferenceType>(const char *value);

class PathReference {
public:
	PGQPathReferenceType path_reference_type;

public:
	explicit PathReference(PGQPathReferenceType path_reference_type) : path_reference_type(path_reference_type) {
	}

	virtual ~PathReference() {
	}

	virtual string ToString() const = 0;

	virtual unique_ptr<PathReference> Copy() = 0;

	virtual bool Equals(const PathReference *other_p) const;

	DUCKDB_API virtual void Serialize(Serializer &serializer) const;

	DUCKDB_API static unique_ptr<PathReference> Deserialize(Deserializer &deserializer);
};

} // namespace duckdb
