#pragma once

#include "array_wrapper.hpp"
#include "base_object.hpp"
#include "configuration_list.hpp"
#include "file_element.hpp"
#include "object_factory.hpp"

#include <istream>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace pbx
{
	class Target;
	
	class Project : public pbx::BaseObject
	{
		pbx::ObjectFactory _factory;
		std::map<std::string, std::shared_ptr<pbx::BaseObject>> _pool;
		std::string _root_object;
		
		std::string _build_configuration_list_ref;
		std::string _compatibility_version;
		std::string _development_region;
		std::set<std::string> _known_regions;
		std::string _main_group_ref;
		std::string _products_group_ref;
		std::string _project_dir_path;
		std::string _project_root;
		std::shared_ptr<pbx::ArrayWrapper<pbx::Target>> _targets;
		
	public:
		static std::shared_ptr<Project> parse(const std::string& filename);
		static std::shared_ptr<Project> parse(std::istream& stream);
		
		Project();
		Project(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& info);
		~Project();
		
		std::shared_ptr<const pbx::ConfigurationList> configuration_list() const;
		
		std::shared_ptr<const pbx::BuildConfiguration> get_configuration(const std::string& name) const;
		
		template <typename T = pbx::BaseObject>
		std::shared_ptr<T> get_object(const std::string& uid)
		{
			static_assert(std::is_base_of<pbx::BaseObject, T>::value, "pbx::Project::get_object<T> template parameter must be a class inheriting from pbx::BaseObject");
			return std::dynamic_pointer_cast<T>(_pool.at(uid));
		}
		
		template <typename T = pbx::BaseObject>
		std::shared_ptr<const T> get_object(const std::string& uid) const
		{
			static_assert(std::is_base_of<pbx::BaseObject, T>::value, "pbx::Project::get_object<T> template parameter must be a class inheriting from pbx::BaseObject");
			return std::dynamic_pointer_cast<const T>(_pool.at(uid));
		}
		
		std::shared_ptr<pbx::Target> get_target(const std::string& name);
		
	private:
		static void _parse(std::shared_ptr<Project>& project, std::istream& stream);
		static void _resolve_ownership(std::shared_ptr<pbx::Group>& group);
		
		void _prepare_factory();
	};
}
