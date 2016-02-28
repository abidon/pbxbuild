#pragma once

#include "toolset.hpp"

namespace pbx
{
	class ClangToolsetStrategy : public pbx::ToolsetStrategy
	{
	public:
		virtual std::string c_compiler() const override;
		virtual std::string cxx_compiler() const override;
		virtual std::string objc_compiler() const override;
		virtual std::string objcxx_compiler() const override;
		virtual std::string linker() const override;
		
		virtual std::vector<std::string> c_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const override;
		virtual std::vector<std::string> cxx_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const override;
		virtual std::vector<std::string> linker_args(const std::map<std::string, std::string>& settings, const std::string& platform) const override;
		virtual std::vector<std::string> objc_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const override;
		virtual std::vector<std::string> objcxx_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const override;
		virtual std::string shared_library_flag() const override;
		
	private:
		static std::vector<std::string> _common_compiler_arg(const std::string& key, const std::string& val, const std::string& platform);
		static std::string _warning_arg(const std::string& name, const std::string& val);
	};
}
