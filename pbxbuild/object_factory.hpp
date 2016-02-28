#pragma once

#include "base_object.hpp"
#include "../libpbxparser/value.hpp"

#include <functional>

namespace pbx
{
	class ObjectFactory
	{
		std::map<std::string, std::function<std::shared_ptr<pbx::BaseObject>(const std::shared_ptr<pbx::Project>&,
																			 const std::string&,
																			 const pbx::Dictionary&)>> _builders;
		
	public:
		ObjectFactory();
		
		std::shared_ptr<pbx::BaseObject> build(const std::shared_ptr<pbx::Project>& project,
											   const std::string& uid, const pbx::Dictionary& info);
		
		void register_builder(const std::string& isa,
							  const std::function<std::shared_ptr<pbx::BaseObject>(const std::shared_ptr<pbx::Project>&, const std::string&, const pbx::Dictionary&)>& builder);
	};
}
