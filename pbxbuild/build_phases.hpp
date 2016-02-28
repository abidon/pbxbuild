#pragma once

#include "array_wrapper.hpp"
#include "base_object.hpp"
#include "build_file.hpp"

namespace pbx
{
	class BuildPhase : public pbx::BaseObject
	{
	protected:
		uint32_t _build_action_mask;
		pbx::ArrayWrapper<pbx::BuildFile> _files;
		bool _run_only_for_deployment_postprocessing;
		
	public:
		BuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		virtual ~BuildPhase() = 0;
		
		const pbx::ArrayWrapper<pbx::BuildFile>& files() const;
	};
	
	class CopyFilesBuildPhase : public pbx::BuildPhase
	{
		std::string _dest_path;
		uint64_t _dest_subfolder_spec;
		
	public:
		CopyFilesBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		const std::string& destination_path() const;
	};
	
	class HeadersBuildPhase : public pbx::BuildPhase
	{
	public:
		HeadersBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
	
	class LinkBuildPhase : public pbx::BuildPhase
	{
	public:
		LinkBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
	
	class ResourcesBuildPhase : public pbx::BuildPhase
	{
	public:
		ResourcesBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
	
	class ScriptBuildPhase : public pbx::BuildPhase
	{
		std::vector<std::string> _input_paths;
		std::vector<std::string> _output_paths;
		std::string _shell_path;
		std::string _shell_script;
		
	public:
		ScriptBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
	
	class SourcesBuildPhase : public pbx::BuildPhase
	{
	public:
		SourcesBuildPhase(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
}
