#include "duckdb/common/serializer/serializer.hpp"
#include "duckdb/common/serializer/deserializer.hpp"

#include "duckpgq/core/parser/subpath_element.hpp"
#include "duckpgq/core/parser/path_reference.hpp"
#include "duckpgq/core/parser/path_element.hpp"

namespace duckdb {

unique_ptr<PathReference> SubPath::Copy() {
	auto result = make_uniq<SubPath>(PGQPathReferenceType::SUBPATH);

	result->path_reference_type = path_reference_type;
	result->path_mode = path_mode;
	for (auto &path : path_list) {
		result->path_list.push_back(path->Copy());
	}
	if (where_clause) {
		result->where_clause = where_clause->Copy();
	}
	result->lower = lower;
	result->upper = upper;
	result->single_bind = single_bind;

	result->path_variable = path_variable;

	return std::move(result);
}
bool SubPath::Equals(const PathReference *other_p) const {
	if (!PathReference::Equals(other_p)) {
		return false;
	}
	auto other = (SubPath *)other_p;
	if (path_list.size() != other->path_list.size()) {
		return false;
	}

	for (idx_t i = 0; i < path_list.size(); i++) {
		if (!path_list[i]->Equals(other->path_list[i].get())) {
			return false;
		}
	}

	if (where_clause && other->where_clause.get()) {
		if (!ParsedExpression::Equals(where_clause, other->where_clause)) {
			return false;
		}
	}
	if ((where_clause && !other->where_clause.get()) || (!where_clause && other->where_clause.get())) {
		return false;
	}

	if (path_mode != other->path_mode) {
		return false;
	}
	if (lower != other->lower) {
		return false;
	}
	if (upper != other->upper) {
		return false;
	}
	if (single_bind != other->single_bind) {
		return false;
	}
	if (path_variable != other->path_variable) {
		return false;
	}
	return true;
}
void SubPath::Serialize(Serializer &serializer) const {
	serializer.WriteProperty(100, "path_mode", path_mode);
	serializer.WriteProperty(101, "path_list", path_list);
	serializer.WriteProperty(102, "single_bind", single_bind);
	serializer.WriteProperty(103, "lower", lower);
	serializer.WriteProperty(104, "upper", upper);
	serializer.WriteProperty(105, "where_clause", where_clause);
	serializer.WriteProperty(106, "path_variable", path_variable);
}

unique_ptr<PathReference> SubPath::Deserialize(Deserializer &deserializer) {
	auto result = make_uniq<SubPath>(PGQPathReferenceType::SUBPATH);
	deserializer.ReadProperty(100, "path_mode", result->path_mode);
	deserializer.ReadProperty(101, "path_list", result->path_list);
	deserializer.ReadProperty(102, "single_bind", result->single_bind);
	deserializer.ReadProperty(103, "lower", result->lower);
	deserializer.ReadProperty(104, "upper", result->upper);
	deserializer.ReadProperty(105, "where_clause", result->where_clause);
	deserializer.ReadProperty(106, "path_variable", result->path_variable);
	return std::move(result);
}
string SubPath::ToString() const {
	string result;
	path_variable.empty() ? result += "" : result += path_variable + " = ";
	if (path_list.size() == 1) {
		switch (path_list[0]->path_reference_type) {
		case PGQPathReferenceType::PATH_ELEMENT: {
			auto path_element = reinterpret_cast<PathElement *>(path_list[0].get());
			switch (path_element->match_type) {
			case PGQMatchType::MATCH_VERTEX:
				result += "(" + path_element->variable_binding + ":" + path_element->label +
				          (where_clause ? " WHERE " + where_clause->ToString() : "") + ")";
				break;
			case PGQMatchType::MATCH_EDGE_ANY:
				result += "-[" + path_element->variable_binding + ":" + path_element->label +
				          (where_clause ? " WHERE " + where_clause->ToString() : "") + "]-";
				break;
			case PGQMatchType::MATCH_EDGE_LEFT:
				result += "<-[" + path_element->variable_binding + ":" + path_element->label +
				          (where_clause ? " WHERE " + where_clause->ToString() : "") + "]-";
				break;
			case PGQMatchType::MATCH_EDGE_RIGHT:
				result += "-[" + path_element->variable_binding + ":" + path_element->label +
				          (where_clause ? " WHERE " + where_clause->ToString() : "") + "]->";
				break;
			case PGQMatchType::MATCH_EDGE_LEFT_RIGHT:
				result += "<-[" + path_element->variable_binding + ":" + path_element->label +
				          (where_clause ? " WHERE " + where_clause->ToString() : "") + "]->";
				break;
			}
			break;
		}
		case PGQPathReferenceType::SUBPATH:
			result += " " + path_list[0]->ToString();
			break;
		case PGQPathReferenceType::UNKNOWN:
		default:
			throw InternalException("Unknown PathReferenceType found.");
		}
	}
	if (lower != upper) {
		result += "{" + std::to_string(lower) + "," + std::to_string(upper) + "}";
	}
	return result;
}

template<>
const char* EnumUtil::ToChars<PGQPathMode>(PGQPathMode value) {
	switch(value) {
	case PGQPathMode::NONE:
		return "NONE";
	case PGQPathMode::WALK:
		return "WALK";
	case PGQPathMode::SIMPLE:
		return "SIMPLE";
	case PGQPathMode::TRAIL:
		return "TRAIL";
	case PGQPathMode::ACYCLIC:
		return "ACYCLIC";
	default:
		throw NotImplementedException(StringUtil::Format("Enum value: '%d' not implemented", value));
	}
}

template<>
PGQPathMode EnumUtil::FromString<PGQPathMode>(const char *value) {
	if (StringUtil::Equals(value, "NONE")) {
		return PGQPathMode::NONE;
	}
	if (StringUtil::Equals(value, "WALK")) {
		return PGQPathMode::WALK;
	}
	if (StringUtil::Equals(value, "SIMPLE")) {
		return PGQPathMode::SIMPLE;
	}
	if (StringUtil::Equals(value, "TRAIL")) {
		return PGQPathMode::TRAIL;
	}
	if (StringUtil::Equals(value, "ACYCLIC")) {
		return PGQPathMode::ACYCLIC;
	}
	throw NotImplementedException(StringUtil::Format("Enum value: '%s' not implemented", value));
}

} // namespace duckdb
