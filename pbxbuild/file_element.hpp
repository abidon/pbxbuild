#pragma once

#include "array_wrapper.hpp"
#include "base_object.hpp"
#include "source_tree.hpp"
#include "../libpbxparser/value.hpp"

namespace pbx
{
	class Group;
	
	class FileElement : public pbx::BaseObject
	{
		std::string _name;
		std::weak_ptr<pbx::Group> _parent;
		pbx::SourceTree _source_tree;
		
	public:
		FileElement(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		const std::string& name() const;
		const std::shared_ptr<const pbx::Group> parent() const;
		void set_parent(std::shared_ptr<pbx::Group>& group);
		pbx::SourceTree source_tree() const;
	};
	
	class FileReference : public pbx::FileElement
	{
		std::string _explicit_file_type;
		std::string _file_encoding;
		std::string _last_known_file_type;
		std::string _path;
		
	public:
		FileReference(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		const std::string& path() const;
	};
	
	class Group : public pbx::FileElement
	{
		pbx::ArrayWrapper<pbx::FileElement> _children;
		
	public:
		Group(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		pbx::ArrayWrapper<pbx::FileElement>& children();
		const pbx::ArrayWrapper<pbx::FileElement>& children() const;
	};
	
	class VariableGroup : public pbx::Group
	{
	public:
		VariableGroup(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
	};
}
