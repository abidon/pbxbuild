#include "clang_toolset.hpp"
#include "../rapidjson/include/rapidjson/document.h"
#include <iostream>
#include <sstream>

static std::vector<std::string> split(std::string str, char delimiter) {
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the string into a stream.
	std::string tok;
	
	while(getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}
	
	return internal;
}

std::string pbx::ClangToolsetStrategy::c_compiler() const
{
	return "clang";
}

std::vector<std::string> pbx::ClangToolsetStrategy::c_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const
{
	std::vector<std::string> arguments;
	for (auto entry : settings)
	{
		std::vector<std::string> args;
		if (entry.first == "GCC_C_LANGUAGE_STANDARD")
			args = {"-std=" + entry.second};
		
		if (args.empty())
			args = {_common_compiler_arg(entry.first, entry.second, platform)};
		
		if (entry.first == "PBXBUILD_CFLAGS")
		{
			rapidjson::Document d;
			d.Parse(entry.second.c_str());
			
			rapidjson::Value& platform_specific = d[platform.c_str()];
			if (false == platform_specific.IsArray())
			{
				std::cout << "Expecting a JSON array for key " << entry.first << "." << platform << '\n';
				continue;
			}
			
			for (uint64_t i(0); i < platform_specific.Size(); ++i)
			{
				auto& v = platform_specific[i];
				if (false == v.IsString())
				{
					std::cout << "Expecting a string for key " << entry.first << "." << platform << '[' << i << ']' << '\n';
				}
				args.push_back(v.GetString());
			}
		}
		
		if (args.empty())
		{
//			std::cout << "Ignored setting " << entry.first << ". It may impact your build.\n";
			continue;
		}
		
		arguments.insert(arguments.end(), args.begin(), args.end());
	}
	return arguments;
}

std::vector<std::string> pbx::ClangToolsetStrategy::_common_compiler_arg(const std::string& key, const std::string& val, const std::string& platform)
{
	if (key == "CLANG_WARN_BOOL_CONVERSION")
		return {_warning_arg("bool-conversion", val)};
	else if (key == "CLANG_WARN_CONSTANT_CONVERSION")
		return {_warning_arg("constant-conversion", val)};
	else if (key == "CLANG_WARN_DIRECT_OBJC_ISA_USAGE")
		return {_warning_arg("deprecated-objc-isa-usage", val)};
	else if (key == "CLANG_WARN_EMPTY_BODY")
		return {_warning_arg("empty-body", val)};
	else if (key == "CLANG_WARN_ENUM_CONVERSION")
		return {_warning_arg("enum-conversion", val)};
	else if (key == "CLANG_WARN_INT_CONVERSION")
		return {_warning_arg("int-conversion", val)};
	else if (key == "CLANG_WARN_OBJC_ROOT_CLASS")
		return {_warning_arg("objc-root-class", val)};
	else if (key == "CLANG_WARN_UNREACHABLE_CODE")
		return {_warning_arg("unreachable-code", val)};
	else if (key == "GCC_NO_COMMON_BLOCKS")
		return {"-fno-common"};
	else if (key == "GCC_OPTIMIZATION_LEVEL")
		return {"-O" + val};
	else if (key == "GCC_PREFIX_HEADER")
		return {"-include" + val};
	else if (key == "GCC_WARN_64_TO_32_BIT_CONVERSION")
		return {_warning_arg("shorten-64-to-32", val)};
	else if (key == "GCC_WARN_ABOUT_RETURN_TYPE")
		return {_warning_arg("return-type", val)};
	else if (key == "GCC_WARN_UNUSED_FUNCTION")
		return {_warning_arg("unused-function", val)};
	else if (key == "GCC_WARN_UNUSED_VARIABLE")
		return {_warning_arg("unused-variable", val)};
	else if (key == "OTHER_CFLAGS")
		return split(val, ',');
	return {};
}

std::string pbx::ClangToolsetStrategy::cxx_compiler() const
{
	return "clang++";
}

std::vector<std::string> pbx::ClangToolsetStrategy::cxx_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const
{
	std::vector<std::string> arguments;
	for (auto entry : settings)
	{
		std::vector<std::string> args;
		
		if (entry.first == "CLANG_CXX_LANGUAGE_STANDARD")
			args = {"-std=" + entry.second};
		else if (entry.first == "CLANG_CXX_LIBRARY")
			args = {"-stdlib=" + entry.second};
		
		if (args.empty())
			args = _common_compiler_arg(entry.first, entry.second, platform);
		
		if (entry.first == "PBXBUILD_CXXFLAGS")
		{
			rapidjson::Document d;
			d.Parse(entry.second.c_str());
			
			rapidjson::Value& platform_specific = d[platform.c_str()];
			if (false == platform_specific.IsArray())
			{
				std::cout << "Expecting a JSON array for key " << entry.first << "." << platform << '\n';
				continue;
			}
			
			for (uint64_t i(0); i < platform_specific.Size(); ++i)
			{
				auto& v = platform_specific[i];
				if (false == v.IsString())
				{
					std::cout << "Expecting a string for key " << entry.first << "." << platform << '[' << i << ']' << '\n';
				}
				args.push_back(v.GetString());
			}
		}
		
		if (args.empty())
		{
//			std::cout << "Ignored setting " << entry.first << ". It may impact your build.\n";
			continue;
		}
		arguments.insert(arguments.end(), args.begin(), args.end());
	}
	return arguments;
}

std::string pbx::ClangToolsetStrategy::linker() const
{
	return "clang++";
}

std::vector<std::string> pbx::ClangToolsetStrategy::linker_args(const std::map<std::string, std::string>& settings, const std::string& platform) const
{
	std::vector<std::string> args;
	
	for (auto entry : settings)
	{
		if (entry.first == "PBXBUILD_LINKER_OPTIONS")
		{
			rapidjson::Document d;
			d.Parse(entry.second.c_str());
			
			rapidjson::Value& platform_specific = d[platform.c_str()];
			if (false == platform_specific.IsArray())
			{
				std::cout << "Expecting a JSON array for key " << entry.first << "." << platform << '\n';
				continue;
			}
			
			for (uint64_t i(0); i < platform_specific.Size(); ++i)
			{
				auto& v = platform_specific[i];
				if (false == v.IsString())
				{
					std::cout << "Expecting a string for key " << entry.first << "." << platform << '[' << i << ']' << '\n';
				}
				args.push_back(v.GetString());
			}
		}
	}
	
	return args;
}

std::string pbx::ClangToolsetStrategy::objc_compiler() const
{
	return c_compiler();
}

std::vector<std::string> pbx::ClangToolsetStrategy::objc_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const
{
	auto c_args = c_compiler_args(settings, platform);
	c_args.push_back("-ObjC");
	return c_args;
}

std::string pbx::ClangToolsetStrategy::objcxx_compiler() const
{
	return cxx_compiler();
}

std::vector<std::string> pbx::ClangToolsetStrategy::objcxx_compiler_args(const std::map<std::string, std::string>& settings, const std::string& platform) const
{
	auto cxx_args = cxx_compiler_args(settings, platform);
	cxx_args.push_back("-ObjC++");
	return cxx_args;
}

std::string pbx::ClangToolsetStrategy::shared_library_flag() const
{
	return "--shared";
}

std::string pbx::ClangToolsetStrategy::_warning_arg(const std::string& name, const std::string& val)
{
	if (val == "YES")
		return "-W" + name;
	else if (val == "YES_ERROR")
		return "-Werror=" + name;
	return "";
}
