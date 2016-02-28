#include "project.hpp"
#include "../libpbxparser/plist_decoder.hpp"

#include "build_phases.hpp"
#include "configuration_list.hpp"
#include "container_item_proxy.hpp"
#include "file_element.hpp"
#include "target.hpp"

#include <fstream>
#include <iostream>

pbx::Project::Project() :
pbx::BaseObject(nullptr, "__RootProject")
{
	_prepare_factory();
}

pbx::Project::Project(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& info) :
pbx::BaseObject(project, uid)
{
	// TODO
	throw std::runtime_error("Nested projects are not yet supported. Aborting...");
}

pbx::Project::~Project()
{}

std::shared_ptr<const pbx::ConfigurationList> pbx::Project::configuration_list() const
{
	return get_object<pbx::ConfigurationList>(_build_configuration_list_ref);
}

std::shared_ptr<const pbx::BuildConfiguration> pbx::Project::get_configuration(const std::string& name) const
{
	auto list = configuration_list();
	for (uint64_t i(0); i < list->build_configurations().size(); ++i)
	{
		if (list->build_configurations()[i]->name() == name)
			return list->build_configurations()[i];
	}
	return nullptr;
}

std::shared_ptr<pbx::Target> pbx::Project::get_target(const std::string& name)
{
	for (size_t i(0); i < _targets->size(); ++i)
		if ((*_targets)[i]->name() == name)
			return (*_targets)[i];
	return std::shared_ptr<pbx::Target>();
}

std::shared_ptr<pbx::Project> pbx::Project::parse(const std::string& filename)
{
	std::ifstream file(filename);
	return parse(file);
}

std::shared_ptr<pbx::Project> pbx::Project::parse(std::istream& stream)
{
	std::shared_ptr<pbx::Project> project = std::make_shared<pbx::Project>();
	_parse(project, stream);
	return project;
}

void pbx::Project::_parse(std::shared_ptr<pbx::Project>& project, std::istream& stream)
{
	if (!stream)
		throw std::invalid_argument("Unable to open Xcode project from stream.");
	
	std::string content;
	content.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	
	pbx::Dictionary document = pbx::PlistDecoder::parse(content);
	project->_root_object = document.at("rootObject").string_value();
	for (auto _ : document.at("objects").dictionary_value())
	{
		project->_pool[_.first] = project->_factory.build(project, _.first, _.second.dictionary_value());
	}
	
	std::shared_ptr<pbx::Group> main_group = project->get_object<pbx::Group>(project->_main_group_ref);
	_resolve_ownership(main_group);
}

template <class Type>
std::shared_ptr<pbx::BaseObject> builder_fn(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& info) {
	return std::static_pointer_cast<pbx::BaseObject>(std::make_shared<Type>(project, uid, info));
}

void pbx::Project::_prepare_factory()
{
	_factory.register_builder("PBXAggregateTarget", builder_fn<pbx::AggregateTarget>);
	_factory.register_builder("PBXBuildFile", builder_fn<pbx::BuildFile>);
	_factory.register_builder("PBXContainerItemProxy", builder_fn<pbx::ContainerItemProxy>);
	_factory.register_builder("PBXCopyFilesBuildPhase", builder_fn<pbx::CopyFilesBuildPhase>);
	_factory.register_builder("PBXFrameworksBuildPhase", builder_fn<pbx::LinkBuildPhase>);
	_factory.register_builder("PBXFileReference", builder_fn<pbx::FileReference>);
	_factory.register_builder("PBXGroup", builder_fn<pbx::Group>);
	_factory.register_builder("PBXHeadersBuildPhase", builder_fn<pbx::HeadersBuildPhase>);
	_factory.register_builder("PBXNativeTarget", builder_fn<pbx::NativeTarget>);
	_factory.register_builder("PBXTargetDependency", builder_fn<pbx::TargetDependency>);
	_factory.register_builder("PBXResourcesBuildPhase", builder_fn<pbx::ResourcesBuildPhase>);
	_factory.register_builder("PBXShellScriptBuildPhase", builder_fn<pbx::ScriptBuildPhase>);
	_factory.register_builder("PBXSourcesBuildPhase", builder_fn<pbx::SourcesBuildPhase>);
	_factory.register_builder("XCBuildConfiguration", builder_fn<pbx::BuildConfiguration>);
	_factory.register_builder("XCConfigurationList", builder_fn<pbx::ConfigurationList>);
	
	_factory.register_builder("PBXProject", [=](const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& info) -> std::shared_ptr<pbx::BaseObject> {
		if (uid != _root_object)
			return builder_fn<pbx::Project>(project, uid, info);
		
		// Attributes hasScannedForEncodings and projectReferences are currently ignored
		project->_build_configuration_list_ref = info.at("buildConfigurationList").string_value();
		project->_compatibility_version = info.at("compatibilityVersion").string_value();
		project->_development_region = info.at("developmentRegion").string_value();
		
		for (auto it : info.at("knownRegions").array_value())
			project->_known_regions.insert(it.string_value());
		
		project->_main_group_ref = info.at("mainGroup").string_value();
		project->_products_group_ref = info.at("productRefGroup").string_value();
		project->_project_dir_path = info.at("projectDirPath").string_value();
		project->_project_root = info.at("projectRoot").string_value();
		project->_targets = std::make_shared<pbx::ArrayWrapper<pbx::Target>>(project, info.at("targets").array_value());
		
		return project;
	});
}

void pbx::Project::_resolve_ownership(std::shared_ptr<pbx::Group>& group)
{
	const uint64_t children_count = group->children().size();
	for (uint64_t i(0); i < children_count; ++i)
	{
		std::shared_ptr<pbx::FileElement> child = group->children()[i];
		child->set_parent(group);
		
		bool child_is_group = dynamic_cast<pbx::Group*>(child.get()) != nullptr;
		if (child_is_group)
		{
			std::shared_ptr<pbx::Group> group = std::static_pointer_cast<pbx::Group>(child);
			_resolve_ownership(group);
		}
	}
}
