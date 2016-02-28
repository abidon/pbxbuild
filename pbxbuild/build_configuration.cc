#include "build_configuration.hpp"

pbx::BuildConfiguration::BuildConfiguration(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_build_settings(),
_name(data.at("name").string_value())
{
	auto settings = data.at("buildSettings").dictionary_value();
	for (auto setting : settings)
	{
		if (setting.second.type() == pbx::Value::StringType)
		{
			_build_settings[setting.first] = setting.second.string_value();
		} else {
			std::string concat;
			for (auto v : setting.second.array_value())
				concat += v.string_value() + ",";
			concat.resize(concat.length() - 1, 0);
			_build_settings[setting.first] = concat;
		}
	}
}

const std::map<std::string, std::string>& pbx::BuildConfiguration::build_settings() const
{
	return _build_settings;
}

const std::string& pbx::BuildConfiguration::name() const
{
	return _name;
}
