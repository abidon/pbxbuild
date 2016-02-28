#include "file_element.hpp"

pbx::FileElement::FileElement(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_name(data.count("name") ? data.at("name").string_value() : (data.count("path") ? data.at("path").string_value() : "")),
_source_tree(pbx::to_source_tree(data.at("sourceTree").string_value()))
{}

const std::string& pbx::FileElement::name() const
{
	return _name;
}

const std::shared_ptr<const pbx::Group> pbx::FileElement::parent() const
{
	return _parent.lock();
}

void pbx::FileElement::set_parent(std::shared_ptr<pbx::Group>& group)
{
	_parent = group;
}

pbx::SourceTree pbx::FileElement::source_tree() const
{
	return _source_tree;
}

pbx::FileReference::FileReference(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::FileElement(project, uid, data),
_explicit_file_type(data.count("explicitFileType") ? data.at("explicitFileType").string_value() : ""),
_file_encoding(data.count("fileEncoding") ? data.at("fileEncoding").string_value() : ""),
_last_known_file_type(data.count("lastKnownFileType") ? data.at("lastKnownFileType").string_value() : ""),
_path(data.at("path").string_value())
{}

const std::string& pbx::FileReference::path() const
{
	return _path;
}

pbx::Group::Group(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::FileElement(project, uid, data),
_children(project, data.at("children").array_value())
{}

pbx::ArrayWrapper<pbx::FileElement>& pbx::Group::children()
{
	return _children;
}

const pbx::ArrayWrapper<pbx::FileElement>& pbx::Group::children() const
{
	return _children;
}

pbx::VariableGroup::VariableGroup(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::Group(project, uid, data)
{}
