#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "target.hpp"
#include "thread_pool.hpp"
#include "toolset.hpp"
#include "../pstream.h"

class Argh;

namespace pbx
{
	class Project;
	
	class PbxBuild
	{
		struct {
			std::string config;
			bool help;
			uint32_t jobs;
			std::string platform;
			std::string project;
			std::string target;
			std::string toolset;
		} _options;
		
		
		std::unique_ptr<pbx::ThreadPool> _concurrency;
		std::shared_ptr<pbx::Project> _project;
		std::string _project_name;
		std::string _project_path;
		std::unique_ptr<pbx::ToolsetStrategy> _toolset;
		std::string _usage;
		
	public:
		static std::string app_dir();
		static std::string derived_data_dir();
		static std::string detect_current_platform();
		
		PbxBuild(const int argc, const char** argv);
		
		std::string project_derived_data_dir() const;
		std::string project_products_dir() const;
		std::string project_products_include_dir() const;
		std::string project_source_dir() const;
		int run();
		std::string target_include_dir(const std::string& target_name) const;
		std::string target_intermediates_dir(const std::string& target_name) const;
		
	private:
		static void _check_proc_execution(redi::pstream& proc);
		static bool _has_up_to_date_product(const std::string& source, const std::string& result);
		
		void _build(const std::shared_ptr<const pbx::Target>& target);
		void _build_options_list(Argh& _);
		void _do_phase(const std::shared_ptr<const pbx::BuildPhase>& phase, const std::shared_ptr<const pbx::Target>& target, std::vector<std::string>& filelist);
		void _do_phase(const pbx::CopyFilesBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target);
		void _do_phase(const pbx::HeadersBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target);
		void _do_phase(const pbx::LinkBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target, const std::vector<std::string>& filelist);
		void _do_phase(const pbx::ResourcesBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target);
		void _do_phase(const pbx::ScriptBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target);
		void _do_phase(const pbx::SourcesBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target, std::vector<std::string>& filelist);
		void _ensure_path(const std::string& path);
		void _ensure_paths();
		void _parse_options(const int argc, const char** argv);
		void _parse_project();
		std::string _path_to_file(const std::shared_ptr<const pbx::FileReference>& file);
		std::string _resolve_default_project_name(const std::string& path = ".") const;
		std::string _resolve_project_name() const;
	};
}
