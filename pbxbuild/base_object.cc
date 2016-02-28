#include "base_object.hpp"

pbx::BaseObject::BaseObject(const std::shared_ptr<pbx::Project>& project, const std::string& uid) :
_project(project),
_uid(uid)
{}

pbx::BaseObject::~BaseObject()
{}

std::weak_ptr<pbx::Project>&
pbx::BaseObject::project()
{
	return _project;
}

const std::weak_ptr<pbx::Project>&
pbx::BaseObject::project() const
{
	return _project;
}

const std::string&
pbx::BaseObject::uid() const
{
	return _uid;
}
