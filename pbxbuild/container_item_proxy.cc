#include "container_item_proxy.hpp"
#include "project.hpp"

pbx::ContainerItemProxy::ContainerItemProxy(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_container_portal_ref(data.at("containerPortal").string_value()),
_proxy_type(static_cast<uint32_t>(data.at("proxyType").unsigned_value())),
_remote_global_id(data.at("remoteGlobalIDString").string_value()),
_remote_info(data.at("remoteInfo").string_value())
{}

std::shared_ptr<const pbx::Project> pbx::ContainerItemProxy::container_portal() const
{
	return project().lock()->get_object<pbx::Project>(_container_portal_ref);
}

std::shared_ptr<const pbx::BaseObject> pbx::ContainerItemProxy::remote_global() const
{
	return project().lock()->get_object<pbx::BaseObject>(_remote_global_id);
}

const std::string& pbx::ContainerItemProxy::remote_info() const
{
	return _remote_info;
}
