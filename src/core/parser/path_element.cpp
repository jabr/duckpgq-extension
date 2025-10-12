#include "duckdb/common/serializer/serializer.hpp"
#include "duckdb/common/serializer/deserializer.hpp"
#include "duckpgq/core/parser/path_element.hpp"
#include "duckpgq/core/parser/path_reference.hpp"

namespace duckdb {

bool PathElement::Equals(const PathReference *other_p) const {
	if (!PathReference::Equals(other_p)) {
		return false;
	}
	auto other = (PathElement *)other_p;
	if (match_type != other->match_type) {
		return false;
	}
	if (label != other->label) {
		return false;
	}
	if (variable_binding != other->variable_binding) {
		return false;
	}

	return true;
}

void PathElement::Serialize(Serializer &serializer) const {
	serializer.WriteProperty(100, "match_type", match_type);
	serializer.WriteProperty(101, "label", label);
	serializer.WriteProperty(101, "variable_binding", variable_binding);
}

unique_ptr<PathReference> PathElement::Deserialize(Deserializer &deserializer) {
	auto result = make_uniq<PathElement>(PGQPathReferenceType::PATH_ELEMENT);
	deserializer.ReadProperty(100, "match_type", result->match_type);
	deserializer.ReadProperty(101, "label", result->label);
	deserializer.ReadProperty(102, "variable_binding", result->variable_binding);
	return std::move(result);
}

unique_ptr<PathReference> PathElement::Copy() {
	auto result = make_uniq<PathElement>(PGQPathReferenceType::PATH_ELEMENT);
	result->path_reference_type = path_reference_type;
	result->match_type = match_type;
	result->label = label;
	result->variable_binding = variable_binding;
	return std::move(result);
}
string PathElement::ToString() const {
	string result = "";
	switch (match_type) {
	case PGQMatchType::MATCH_VERTEX:
		result += "(" + variable_binding + ":" + label + ")";
		break;
	case PGQMatchType::MATCH_EDGE_ANY:
		result += "-[" + variable_binding + ":" + label + "]-";
		break;
	case PGQMatchType::MATCH_EDGE_LEFT:
		result += "<-[" + variable_binding + ":" + label + "]-";
		break;
	case PGQMatchType::MATCH_EDGE_RIGHT:
		result += "-[" + variable_binding + ":" + label + "]->";
		break;
	case PGQMatchType::MATCH_EDGE_LEFT_RIGHT:
		result += "<-[" + variable_binding + ":" + label + "]->";
		break;
	}
	return result;
}

template<>
const char* EnumUtil::ToChars<PGQMatchType>(PGQMatchType value) {
	switch(value) {
	case PGQMatchType::MATCH_VERTEX:
		return "MATCH_VERTEX";
	case PGQMatchType::MATCH_EDGE_ANY:
		return "MATCH_EDGE_ANY";
	case PGQMatchType::MATCH_EDGE_LEFT:
		return "MATCH_EDGE_LEFT";
	case PGQMatchType::MATCH_EDGE_RIGHT:
		return "MATCH_EDGE_RIGHT";
	case PGQMatchType::MATCH_EDGE_LEFT_RIGHT:
		return "MATCH_EDGE_LEFT_RIGHT";
	default:
		throw NotImplementedException(StringUtil::Format("Enum value: '%d' not implemented", value));
	}
}

template<>
PGQMatchType EnumUtil::FromString<PGQMatchType>(const char *value) {
	if (StringUtil::Equals(value, "MATCH_VERTEX")) {
		return PGQMatchType::MATCH_VERTEX;
	}
	if (StringUtil::Equals(value, "MATCH_EDGE_ANY")) {
		return PGQMatchType::MATCH_EDGE_ANY;
	}
	if (StringUtil::Equals(value, "MATCH_EDGE_LEFT")) {
		return PGQMatchType::MATCH_EDGE_LEFT;
	}
	if (StringUtil::Equals(value, "MATCH_EDGE_RIGHT")) {
		return PGQMatchType::MATCH_EDGE_RIGHT;
	}
	if (StringUtil::Equals(value, "MATCH_EDGE_LEFT_RIGHT")) {
		return PGQMatchType::MATCH_EDGE_LEFT_RIGHT;
	}
	throw NotImplementedException(StringUtil::Format("Enum value: '%s' not implemented", value));
}

} // namespace duckdb
