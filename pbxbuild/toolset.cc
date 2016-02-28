#include "toolset.hpp"
#include "clang_toolset.hpp"

pbx::ToolsetStrategy::~ToolsetStrategy()
{}

pbx::ToolsetStrategy* pbx::toolset_strategy_factory(const std::string& name)
{
	if (name == "clang")
		return new pbx::ClangToolsetStrategy();
	return nullptr;
}

