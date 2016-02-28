#pragma once

#include <map>
#include <string>
#include <vector>

namespace pbx
{
	class ToolsetStrategy
	{
	public:
		virtual ~ToolsetStrategy() = 0;
		
		virtual std::string c_compiler() const = 0;
		virtual std::string cxx_compiler() const = 0;
		virtual std::string objc_compiler() const = 0;
		virtual std::string objcxx_compiler() const = 0;
		virtual std::string linker() const = 0;
		
		virtual std::vector<std::string> c_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const = 0;
		virtual std::vector<std::string> cxx_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const = 0;
		virtual std::vector<std::string> linker_args(const std::map<std::string, std::string>& settings, const std::string& platform) const = 0;
		virtual std::vector<std::string> objc_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const = 0;
		virtual std::vector<std::string> objcxx_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const = 0;
		virtual std::string shared_library_flag() const = 0;
	};
	
	pbx::ToolsetStrategy* toolset_strategy_factory(const std::string& name);
}
