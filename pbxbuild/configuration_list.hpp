#pragma once

#include "array_wrapper.hpp"
#include "base_object.hpp"
#include "build_configuration.hpp"
#include "../libpbxparser/value.hpp"
#include <string>

namespace pbx
{
	class ConfigurationList : public pbx::BaseObject
	{
		pbx::ArrayWrapper<pbx::BuildConfiguration> _build_configurations;
		bool _default_is_visible;
		std::string _default_name;
		
	public:
		ConfigurationList(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		const pbx::ArrayWrapper<pbx::BuildConfiguration>& build_configurations() const;
	};
}
