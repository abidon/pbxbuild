#include "configuration_list.hpp"

pbx::ConfigurationList::ConfigurationList(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_build_configurations(project, data.at("buildConfigurations").array_value()),
_default_is_visible(data.count("defaultConfigurationIsVisible") && data.at("defaultConfigurationIsVisible").string_value() != "0"),
_default_name(data.count("defaultConfigurationName") ? data.at("defaultConfigurationName").string_value() : "")
{}

const pbx::ArrayWrapper<pbx::BuildConfiguration>& pbx::ConfigurationList::build_configurations() const
{
	return _build_configurations;
}
