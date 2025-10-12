#pragma once

#include "duckpgq/common.hpp"

namespace duckdb {

class DuckpgqExtension : public Extension {
public:
	void Load(ExtensionLoader &loader) override;
	std::string Name() override;
	std::string Version() const override;
};

} // namespace duckdb
