#include "build_phases.hpp"

#pragma mark -
#pragma mark BuildPhase
pbx::BuildPhase::BuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_build_action_mask(static_cast<uint32_t>(data.at("buildActionMask").unsigned_value())),
_files(project, data.at("files").array_value()),
_run_only_for_deployment_postprocessing(data.count("runOnlyForDeploymentPostprocessing") && "0" != data.at("runOnlyForDeploymentPostprocessing").string_value())
{ }

pbx::BuildPhase::~BuildPhase()
{}

const pbx::ArrayWrapper<pbx::BuildFile>& pbx::BuildPhase::files() const
{
	return _files;
}

#pragma mark -
#pragma mark CopyFilesBuildPhase
pbx::CopyFilesBuildPhase::CopyFilesBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BuildPhase(project, uid, data),
_dest_path(data.at("dstPath").string_value()),
_dest_subfolder_spec(data.at("dstSubfolderSpec").unsigned_value())
{}

const std::string& pbx::CopyFilesBuildPhase::destination_path() const
{
	return _dest_path;
}

#pragma mark -
#pragma mark HeadersBuildPhase
pbx::HeadersBuildPhase::HeadersBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BuildPhase(project, uid, data)
{}

#pragma mark -
#pragma mark LinkBuildPhase
pbx::LinkBuildPhase::LinkBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BuildPhase(project, uid, data)
{}

#pragma mark -
#pragma mark ResourcesBuildPhase
pbx::ResourcesBuildPhase::ResourcesBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BuildPhase(project, uid, data)
{}

#pragma mark -
#pragma mark ScriptBuildPhase
pbx::ScriptBuildPhase::ScriptBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BuildPhase(project, uid, data),
_input_paths(),
_output_paths(),
_shell_path(data.at("shellPath").string_value()),
_shell_script(data.at("shellScript").string_value())
{
	for (auto it : data.at("inputPaths").array_value())
		_input_paths.push_back(it.string_value());
	
	for (auto it : data.at("outputPaths").array_value())
		_output_paths.push_back(it.string_value());
}

#pragma mark -
#pragma mark SourcesBuildPhase
pbx::SourcesBuildPhase::SourcesBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BuildPhase(project, uid, data)
{}
