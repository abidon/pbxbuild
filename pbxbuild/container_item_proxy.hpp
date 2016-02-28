#pragma once

#include "base_object.hpp"
#include "../libpbxparser/value.hpp"

namespace pbx
{
	class ContainerItemProxy : public pbx::BaseObject
	{
		std::string _container_portal_ref;
		uint32_t _proxy_type;
		std::string _remote_global_id;
		std::string _remote_info;
		
	public:
		ContainerItemProxy(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		std::shared_ptr<const pbx::Project> container_portal() const;
		std::shared_ptr<const pbx::BaseObject> remote_global() const;
		const std::string& remote_info() const;
	};
}
