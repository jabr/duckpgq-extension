#include "duckdb/common/serializer/serializer.hpp"
#include "duckdb/common/serializer/deserializer.hpp"

#include "duckpgq/core/parser/path_element.hpp"
#include "duckpgq/core/parser/subpath_element.hpp"
#include "duckpgq/core/parser/path_reference.hpp"

namespace duckdb {

bool PathReference::Equals(const PathReference *other_p) const {
	if (!other_p) {
		return false;
	}
	if (path_reference_type != other_p->path_reference_type) {
		return false;
	}
	return true;
}
void PathReference::Serialize(Serializer &serializer) const {
	serializer.WriteProperty(100, "path_reference_type", path_reference_type);
}

unique_ptr<PathReference> PathReference::Deserialize(Deserializer &deserializer) {
	unique_ptr<PathReference> result;
	PGQPathReferenceType path_reference_type = PGQPathReferenceType::UNKNOWN;
	deserializer.ReadProperty(100, "path_reference_type", path_reference_type);
	switch (path_reference_type) {
	case PGQPathReferenceType::PATH_ELEMENT:
		result = PathElement::Deserialize(deserializer);
		break;
	case PGQPathReferenceType::SUBPATH:
		result = SubPath::Deserialize(deserializer);
		break;
	default:
		throw InternalException("Unknown path reference type in deserializer.");
	}
	result->path_reference_type = path_reference_type;
	return result;
}

template<>
const char* EnumUtil::ToChars<PGQPathReferenceType>(PGQPathReferenceType value) {
	switch(value) {
	case PGQPathReferenceType::PATH_ELEMENT:
		return "PATH_ELEMENT";
	case PGQPathReferenceType::SUBPATH:
		return "SUBPATH";
	case PGQPathReferenceType::UNKNOWN:
		return "UNKNOWN";
	default:
		throw NotImplementedException(StringUtil::Format("Enum value: '%d' not implemented", value));
	}
}

template<>
PGQPathReferenceType EnumUtil::FromString<PGQPathReferenceType>(const char *value) {
	if (StringUtil::Equals(value, "PATH_ELEMENT")) {
		return PGQPathReferenceType::PATH_ELEMENT;
	}
	if (StringUtil::Equals(value, "SUBPATH")) {
		return PGQPathReferenceType::SUBPATH;
	}
	if (StringUtil::Equals(value, "UNKNOWN")) {
		return PGQPathReferenceType::UNKNOWN;
	}
	throw NotImplementedException(StringUtil::Format("Enum value: '%s' not implemented", value));
}

} // namespace duckdb
