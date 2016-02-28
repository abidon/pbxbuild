#include "target.hpp"

#pragma mark -
#pragma mark Target
pbx::Target::Target(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_build_configuration_list(data.at("buildConfigurationList").string_value()),
_build_phases(project, data.at("buildPhases").array_value()),
_dependencies(project, data.at("dependencies").array_value()),
_name(data.at("name").string_value()),
_product_name(data.at("productName").string_value())
{}

pbx::Target::~Target()
{}

const pbx::ArrayWrapper<pbx::BuildPhase>& pbx::Target::build_phases() const
{
	return _build_phases;
}

std::shared_ptr<const pbx::ConfigurationList> pbx::Target::configuration_list() const
{
	return project().lock()->get_object<pbx::ConfigurationList>(_build_configuration_list);
}

const pbx::ArrayWrapper<pbx::TargetDependency>& pbx::Target::dependencies() const
{
	return _dependencies;
}

std::shared_ptr<const pbx::BuildConfiguration> pbx::Target::get_configuration(const std::string& name) const
{
	auto conf_list = configuration_list()->build_configurations();
	for (uint64_t i(0); i < conf_list.size(); ++i)
	{
		if (conf_list[i]->name() == name)
			return conf_list[i];
	}
	return nullptr;
}

bool pbx::Target::is_relocated_executable() const
{
	return false;
}

const std::string& pbx::Target::name() const
{
	return _name;
}

const std::string& pbx::Target::product_name() const
{
	return _product_name;
}

#pragma mark -
#pragma mark NativeTarget
pbx::NativeTarget::NativeTarget(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::Target(project, uid, data),
_product_install_path(data.count("productInstallPath") ? data.at("productInstallPath").string_value() : ""),
_product_ref(data.at("productReference").string_value()),
_product_type(data.at("productType").string_value())
{}

bool pbx::NativeTarget::is_relocated_executable() const
{
	return _product_type == "com.apple.product-type.framework" || _product_type == "com.apple.product-type.library.dynamic";
}

const std::string& pbx::NativeTarget::product_name() const
{
	auto product = project().lock()->get_object<pbx::FileReference>(_product_ref);
	return product->path();
}

#pragma mark -
#pragma mark AggregateTarget
pbx::AggregateTarget::AggregateTarget(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::Target(project, uid, data)
{}

#pragma mark -
#pragma mark TargetDependency
pbx::TargetDependency::TargetDependency(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_target_ref(data.at("target").string_value()),
_target_proxy_ref(data.at("targetProxy").string_value())
{}

std::shared_ptr<const pbx::Target> pbx::TargetDependency::target() const
{
	return project().lock()->get_object<pbx::Target>(_target_ref);
}

std::shared_ptr<const pbx::ContainerItemProxy> pbx::TargetDependency::target_proxy() const
{
	return project().lock()->get_object<pbx::ContainerItemProxy>(_target_proxy_ref);
}
