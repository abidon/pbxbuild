#pragma once

#include "base_object.hpp"
#include "../libpbxparser/value.hpp"

namespace pbx
{
	class BuildConfiguration : public pbx::BaseObject
	{
		std::map<std::string, std::string> _build_settings;
		std::string _name;
		
	public:
		BuildConfiguration(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		const std::map<std::string, std::string>& build_settings() const;
		const std::string& name() const;
	};
}
