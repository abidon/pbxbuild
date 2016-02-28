#pragma once

#include "array_wrapper.hpp"
#include "base_object.hpp"
#include "build_phases.hpp"
#include "configuration_list.hpp"
#include "container_item_proxy.hpp"
#include "../libpbxparser/value.hpp"

namespace pbx
{
	class Target;
	
	class TargetDependency : public pbx::BaseObject
	{
		std::string _target_ref;
		std::string _target_proxy_ref;
		
	public:
		TargetDependency(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		std::shared_ptr<const pbx::Target> target() const;
		std::shared_ptr<const pbx::ContainerItemProxy> target_proxy() const;
	};
	
	class Target : public pbx::BaseObject
	{
		std::string _build_configuration_list;
		pbx::ArrayWrapper<pbx::BuildPhase> _build_phases;
		pbx::ArrayWrapper<pbx::TargetDependency> _dependencies;
		std::string _name;
		std::string _product_name;
		
	public:
		Target(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		virtual ~Target() = 0;
		
		const pbx::ArrayWrapper<pbx::BuildPhase>& build_phases() const;
		std::shared_ptr<const pbx::ConfigurationList> configuration_list() const;
		const pbx::ArrayWrapper<pbx::TargetDependency>& dependencies() const;
		std::shared_ptr<const pbx::BuildConfiguration> get_configuration(const std::string& name) const;
		virtual bool is_relocated_executable() const;
		const std::string& name() const;
		virtual const std::string& product_name() const;
	};
	
	class AggregateTarget : public pbx::Target
	{
	public:
		AggregateTarget(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
	
	class NativeTarget : public pbx::Target
	{
		std::string _product_install_path;
		std::string _product_ref;
		std::string _product_type;
		
	public:
		NativeTarget(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		virtual bool is_relocated_executable() const override;
		virtual const std::string& product_name() const override;
	};
}
