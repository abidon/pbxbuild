#pragma once

#include "base_object.hpp"
#include "file_element.hpp"
#include "project.hpp"
#include "../libpbxparser/value.hpp"

#include <string>

namespace pbx
{
	class BuildFile : public pbx::BaseObject
	{
		std::string _file_ref;
		pbx::Dictionary _settings;
		
	public:
		BuildFile(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data);
		
		std::shared_ptr<const pbx::FileReference> file() const;
	};
}