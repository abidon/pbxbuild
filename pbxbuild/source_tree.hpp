#pragma once

#include <string>

namespace pbx
{
	enum class SourceTree
	{
		Absolute,
		Build,
		Developer,
		Group,
		Sdk,
		Source,
	};
	
	pbx::SourceTree to_source_tree(const std::string& str);
}
