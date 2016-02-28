#include "pbxbuild.hpp"

#include "project.hpp"
#include "../argh/argh.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include <iostream>
#include <thread>

static bool ends_with(const std::string& value, const std::string& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

pbx::PbxBuild::PbxBuild(const int argc, const char** argv)
{
	_parse_options(argc, argv);
	
	if (argc <= 1)
		_options.help = true;
}

std::string pbx::PbxBuild::app_dir()
{
	std::string home_dir(getenv("HOME"));
	
#if __APPLE__
	return home_dir + "/Library/Application Support/PBXBuild";
#else
	return home_dir + "/.pbxbuild";
#endif
}

void pbx::PbxBuild::_build(const std::shared_ptr<const pbx::Target>& target)
{
	_ensure_paths();
	
	// build dependencies
	for (uint64_t i(0); i < target->dependencies().size(); ++i)
	{
		const auto& dependency = target->dependencies()[i];
		_build(dependency->target());
	}
	if (target->dependencies().size() > 0)
		std::cout << ">> " << target->name() << " dependenc" << (target->dependencies().size() == 1 ? "y" : "ies") << " built. Resuming build.\n";
	else
		std::cout << ">> Building target " << target->name() << '\n';
	
	std::vector<std::string> filelist; // object files to be linked
	for (uint64_t i(0); i < target->build_phases().size(); ++i)
	{
		const auto& phase = target->build_phases()[i];
		_do_phase(phase, target, filelist);
	}
}

void pbx::PbxBuild::_build_options_list(Argh& _)
{
	_.addFlag(_options.help, "-help", "Display usage information");
	
	_.addOption<std::string>(_options.config, "Debug", "-configuration", "Specify the configuration list to use");
	_.addOption<std::string>(_options.project, _resolve_default_project_name(), "-project", "build the project NAME (automatic resolution if there's only one .xcodeproj in the working directory)");
	_.addOption<std::string>(_options.target, "", "-target", "build the target NAME");
	_.addOption<uint32_t>(_options.jobs, std::thread::hardware_concurrency(), "-jobs", "specify the maximum number of concurrent build operations");
	_.addOption<std::string>(_options.platform, detect_current_platform(), "-platform", "build platform (one of: desktop, ios, android)");
	_.addOption<std::string>(_options.toolset, "clang", "-toolset", "toolset to use (one of: clang, gcc)");
}

void pbx::PbxBuild::_check_proc_execution(redi::pstream &proc)
{
	std::string error(std::istreambuf_iterator<char>(proc.err()), {});
	if (!error.empty())
		std::cout << "An error may have occured during execution of command: " << proc.command() << '\n' << error << '\n';
}

std::string pbx::PbxBuild::derived_data_dir()
{
	return app_dir() + "/DerivedData";
}

std::string pbx::PbxBuild::detect_current_platform()
{
#if __APPLE__
	return "osx";
#elif __linux__ || __unix__
	return "linux";
#else
#	error Platform is not (yet) supported.
#endif
}

void pbx::PbxBuild::_do_phase(const std::shared_ptr<const pbx::BuildPhase>& phase, const std::shared_ptr<const pbx::Target>& target, std::vector<std::string>& filelist)
{
	const pbx::BuildPhase* phase_ptr = phase.get();
	if (!phase_ptr)
		throw std::invalid_argument("Unable to do phase <null>. Aborting.");
	
	{
		const pbx::CopyFilesBuildPhase* casted_phase_ptr = dynamic_cast<const pbx::CopyFilesBuildPhase*>(phase_ptr);
		if (casted_phase_ptr) {
			_do_phase(*casted_phase_ptr, target);
			return;
		}
	}
	
	{
		const pbx::HeadersBuildPhase* casted_phase_ptr = dynamic_cast<const pbx::HeadersBuildPhase*>(phase_ptr);
		if (casted_phase_ptr) {
			_do_phase(*casted_phase_ptr, target);
			return;
		}
	}
	
	{
		const pbx::LinkBuildPhase* casted_phase_ptr = dynamic_cast<const pbx::LinkBuildPhase*>(phase_ptr);
		if (casted_phase_ptr) {
			_do_phase(*casted_phase_ptr, target, filelist);
			return;
		}
	}
	
	{
		const pbx::ResourcesBuildPhase* casted_phase_ptr = dynamic_cast<const pbx::ResourcesBuildPhase*>(phase_ptr);
		if (casted_phase_ptr) {
			_do_phase(*casted_phase_ptr, target);
			return;
		}
	}
	
	{
		const pbx::ScriptBuildPhase* casted_phase_ptr = dynamic_cast<const pbx::ScriptBuildPhase*>(phase_ptr);
		if (casted_phase_ptr) {
			_do_phase(*casted_phase_ptr, target);
			return;
		}
	}
	
	{
		const pbx::SourcesBuildPhase* casted_phase_ptr = dynamic_cast<const pbx::SourcesBuildPhase*>(phase_ptr);
		if (casted_phase_ptr) {
			_do_phase(*casted_phase_ptr, target, filelist);
			return;
		}
	}
	
	throw std::invalid_argument("Unsupported type of build phase. Aborting.");
}

void pbx::PbxBuild::_do_phase(const pbx::CopyFilesBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target)
{
	_ensure_path(target_include_dir(target->name()));
	
	for (uint64_t i(0); i < phase.files().size(); ++i)
	{
		std::cout << "CopyFile " << _path_to_file(phase.files()[i]->file()) << std::endl;
		std::vector<std::string> args{"/usr/bin/env", "cp", _path_to_file(phase.files()[i]->file()), phase.destination_path()};
		redi::pstream proc(args, redi::pstream::pstdout|redi::pstream::pstderr);
		_check_proc_execution(proc);
	}
}

void pbx::PbxBuild::_do_phase(const pbx::HeadersBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target)
{
	_ensure_path(target_include_dir(target->name()));
	
	for (uint64_t i(0); i < phase.files().size(); ++i)
	{
		std::cout << "CopyHeader " << _path_to_file(phase.files()[i]->file()) << std::endl;
		std::vector<std::string> args{"/usr/bin/env", "cp", _path_to_file(phase.files()[i]->file()), target_include_dir(target->name())};
		redi::pstream proc(args, redi::pstream::pstdout|redi::pstream::pstderr);
		_check_proc_execution(proc);
	}
}

void pbx::PbxBuild::_do_phase(const pbx::LinkBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target, const std::vector<std::string>& filelist)
{
	std::cout << "Link " << filelist.size() << " object file" << (filelist.size() == 1 ? "" : "s") << " with " << phase.files().size() << " external " << (phase.files().size() == 1 ? "library" : "libraries") << '\n';
	std::vector<std::string> args{"/usr/bin/env", _toolset->linker(), "-o", project_products_dir() + '/' + target->product_name()};
	if (target->is_relocated_executable())
		args.insert(args.begin() + 2, _toolset->shared_library_flag());
	
	for (uint64_t i(0); i < phase.files().size(); ++i)
		args.insert(args.begin() + (target->is_relocated_executable() ? 3 : 2), _path_to_file(phase.files()[phase.files().size() - (i + 1)]->file()));
	
	auto conf = target->get_configuration(_options.config)->build_settings();
	auto proj_conf = _project->get_configuration(_options.config)->build_settings();
	conf.insert(proj_conf.begin(), proj_conf.end());
	auto linker_args = _toolset->linker_args(conf, _options.platform);
	for (auto it(linker_args.rbegin()); it != linker_args.rend(); it++)
		args.insert(args.begin() + (target->is_relocated_executable() ? 3 : 2), *it);
	
	for (auto object : filelist)
		args.insert(args.begin() + (target->is_relocated_executable() ? 3 : 2), object);
	
	std::cout << "args: ";
	for (auto arg : args)
		std::cout << arg << ", ";
	std::cout << '\n';
	
	redi::pstream proc(args, redi::pstream::pstdout|redi::pstream::pstderr);
	_check_proc_execution(proc);
	
	return;
}

void pbx::PbxBuild::_do_phase(const pbx::ResourcesBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target)
{
	for (uint64_t i(0); i < phase.files().size(); ++i)
	{
		std::cout << "CopyResource " << _path_to_file(phase.files()[i]->file()) << std::endl;
		std::vector<std::string> args{"/usr/bin/env", "cp", "-R", _path_to_file(phase.files()[i]->file()), project_products_dir()};
		redi::pstream proc(args, redi::pstream::pstdout|redi::pstream::pstderr);
		_check_proc_execution(proc);
	}
}

void pbx::PbxBuild::_do_phase(const pbx::ScriptBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target)
{
	throw std::runtime_error("Script build phase is not yet supported");
}

void pbx::PbxBuild::_do_phase(const pbx::SourcesBuildPhase& phase, const std::shared_ptr<const pbx::Target>& target, std::vector<std::string>& filelist)
{
	_ensure_path(target_intermediates_dir(target->name()));
	
	auto conf = target->get_configuration(_options.config)->build_settings();
	auto proj_conf = _project->get_configuration(_options.config)->build_settings();
	conf.insert(proj_conf.begin(), proj_conf.end());
	
	std::vector<std::string> ccomp_args = _toolset->c_compiler_args(conf, _options.platform);
	std::vector<std::string> cxxcomp_args = _toolset->cxx_compiler_args(conf, _options.platform);
	std::vector<std::string> objccomp_args = _toolset->objc_compiler_args(conf, _options.platform);
	std::vector<std::string> objcxxcomp_args = _toolset->objcxx_compiler_args(conf, _options.platform);
	
	for (uint64_t i(0); i < phase.files().size(); ++i)
	{
		const auto& build_file = phase.files()[i];
		std::cout << "BuildSource " << _path_to_file(build_file->file()) << std::endl;
		std::string filepath(_path_to_file(build_file->file()));
		
		std::string::size_type last_slash = filepath.find_last_of('/');
		std::string file_basename = filepath.substr(last_slash != std::string::npos ? last_slash + 1 : 0);
		
		std::string output_path = target_intermediates_dir(target->name()) + '/' + file_basename + ".o";
		filelist.push_back(output_path);
		
		if (_has_up_to_date_product(filepath, output_path))
			continue;
		
		std::vector<std::string> args{
			"-c", filepath,
			"-o", output_path,
			"-I", project_products_include_dir(),
			"-I", target_include_dir(target->name()),
			"-I", project_source_dir(),
		};
		
		if (target->is_relocated_executable())
			args.push_back("-fPIC");
		
		std::string::size_type last_dot = filepath.find_last_of('.');
		if (last_dot == std::string::npos)
			throw std::runtime_error("Unable to determine language of source file " + file_basename + ". Aborting");
		std::string extension = filepath.substr(last_dot+1);
		if (extension == "c")
		{
			args.insert(args.end(), ccomp_args.begin(), ccomp_args.end());
			args.insert(args.begin(), _toolset->c_compiler());
		}
		else if (extension == "cc" || extension == "cpp" || extension == "cxx")
		{
			args.insert(args.end(), cxxcomp_args.begin(), cxxcomp_args.end());
			args.insert(args.begin(), _toolset->cxx_compiler());
		}
		else if (extension == "m")
		{
			args.insert(args.end(), objccomp_args.begin(), objccomp_args.end());
			args.insert(args.begin(), _toolset->objc_compiler());
		}
		else if (extension == "mm")
		{
			args.insert(args.end(), objcxxcomp_args.begin(), objcxxcomp_args.end());
			args.insert(args.begin(), _toolset->objcxx_compiler());
		}
		else
			throw std::runtime_error("Unable to determine language of source file " + file_basename + ". Aborting");
		
		args.insert(args.begin(), "/usr/bin/env");
		
		std::cout << "args: ";
		for (auto arg : args)
			std::cout << arg << ", ";
		std::cout << '\n';
		
		redi::pstream proc(args, redi::pstream::pstdout|redi::pstream::pstderr);
		_check_proc_execution(proc);
	}
}

void pbx::PbxBuild::_ensure_path(const std::string& path)
{
	struct stat info;
	if(stat(path.c_str(), &info) != 0)
	{
		std::string::size_type last_slash = path.find_last_of('/');
		if (last_slash != std::string::npos)
			_ensure_path(path.substr(0, last_slash));
		mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	}
	else if ((info.st_mode & S_IFMT) != S_IFDIR)
		throw std::runtime_error("Path " + path + " exists, but it's not a directory.");
}

void pbx::PbxBuild::_ensure_paths()
{
	for (const auto& path : {project_derived_data_dir(), project_products_dir()})
		_ensure_path(path);
}

bool pbx::PbxBuild::_has_up_to_date_product(const std::string& source, const std::string& result)
{
	struct stat source_stat;
	struct stat result_stat;
	
	if (-1 == stat(source.c_str(), &source_stat))
	{
		std::cout << "Source file " << source << " can't be found\n";
		return false;
	}
	
	if (-1 == stat(result.c_str(), &result_stat))
		return false;
#ifdef __APPLE__
	return source_stat.st_mtimespec.tv_sec < result_stat.st_mtimespec.tv_sec;
#else
	return source_stat.st_mtim.tv_sec < result_stat.st_mtim.tv_sec;
#endif
}

void pbx::PbxBuild::_parse_options(const int argc, const char** argv)
{
	Argh argh;
	
	_build_options_list(argh);
	_usage = argh.getUsage();
	
	argh.parse(argc, argv);
}

std::string pbx::PbxBuild::_resolve_default_project_name(const std::string& path) const
{
	struct dirent* entry;
	DIR* dirp = opendir(path.c_str());
	
	std::string project_name;
	
	while ((entry = readdir(dirp)))
	{
		std::string name(entry->d_name);
		if (name == "." || name == ".." || name.length() < 11) // at least one character followed by the .xcodeproj extension
			continue;
		
		std::string::size_type pos = name.find(".xcodeproj");
		if (pos == name.length() - 10)
		{
			if (!project_name.empty())
				return "";
			project_name = name;
		}
	}
	
	return path.empty() ? "" : path + '/' + project_name;
}

std::string pbx::PbxBuild::_resolve_project_name() const
{
	if (_options.project.empty())
		return _resolve_default_project_name();
	
	if (ends_with(_options.project, ".pbxproj"))
		return _options.project;
	
	if (ends_with(_options.project, ".xcodeproj"))
		return _options.project + "/project.pbxproj";
	
	if (ends_with(_options.project, ".xcodeproj/"))
		return _options.project + "project.pbxproj";
	
	return _resolve_default_project_name(_options.project) + "/project.pbxproj"; // assumes _options.project is a path
}

std::string pbx::PbxBuild::_path_to_file(const std::shared_ptr<const pbx::FileReference>& file)
{
	switch (file->source_tree())
	{
		case pbx::SourceTree::Absolute:
		{
			return file->path();
		}
		case pbx::SourceTree::Build:
		{
			return project_products_dir() + '/' + file->path();
		}
		case pbx::SourceTree::Group:
		{
			auto path = file->path();
			
			for (auto it = file->parent(); it; it = it->parent())
				if (false == it->name().empty())
					path = it->name() + '/' + path;
			return project_source_dir() + '/' + path;
		}
		default:
		{
			throw "unsupported source tree";
		}
	}
}

std::string pbx::PbxBuild::project_derived_data_dir() const
{
	return derived_data_dir() + '/' + _project_name;
}

std::string pbx::PbxBuild::project_products_dir() const
{
	return project_derived_data_dir() + "/Build/Products/" + _options.config;
}

std::string pbx::PbxBuild::project_products_include_dir() const
{
	return project_products_dir() + "/usr/local/include";
}

std::string pbx::PbxBuild::project_source_dir() const
{
	std::string p = _project_path;
	
	while ((ends_with(p, ".xcodeproj") || ends_with(p, ".pbxproj")) && p.find_last_of('/') != std::string::npos)
		p = p.substr(0, p.find_last_of('/'));
	return p;
}

int pbx::PbxBuild::run()
{
	if (_options.help)
	{
		std::cout << "PBXBuild - Multiplatform Xcode project builder\n\n";
		std::cout << _usage << '\n';
		return 0;
	}
	
	if (_options.project.empty())
	{
		std::cerr << "Unable to determine which project to build. Aborting..." << '\n';
		return 1;
	}
	
	_project_path = _resolve_project_name();
	_project = pbx::Project::parse(_project_path);
	
	size_t last_slash = _project_path.find_last_of('/');
	size_t before_last_slash = _project_path.find_last_of("/", last_slash - 1);
	_project_name = _project_path.substr(before_last_slash != std::string::npos ? before_last_slash + 1 : 0, (last_slash - 11) - before_last_slash);
	
	if (!_project->get_target(_options.target))
	{
		std::cerr << "No target " << _options.target << " in project. Aborting..." << '\n';
		return 1;
	}
	
	_toolset.reset(toolset_strategy_factory(_options.toolset));
	if (nullptr == _toolset.get())
		throw std::runtime_error("No support for toolset named " + _options.toolset + ". Aborting.");
	
	_concurrency.reset(new pbx::ThreadPool(_options.jobs));
	
	_build(_project->get_target(_options.target));
	
	return 0;
}

std::string pbx::PbxBuild::target_include_dir(const std::string& target_name) const
{
	return project_products_include_dir() + '/' + target_name;
}

std::string pbx::PbxBuild::target_intermediates_dir(const std::string& target_name) const
{
	return project_derived_data_dir() + "/Build/Intermediates/" + _options.config + '/' + target_name;
}

int main(int argc, const char** argv)
{
	pbx::PbxBuild builder(argc, argv);
	
	try {
		return builder.run();
	} catch(const std::exception& e)
	{
		std::cerr << "Uncatched exception: " << e.what() << std::endl;
		return 1;
	}
}
