#pragma once

#include <memory>
#include <string>

namespace pbx
{
	class Project;
	
	class BaseObject
	{
		std::weak_ptr<pbx::Project> _project;
		std::string _uid;
		
	public:
		BaseObject(const std::shared_ptr<pbx::Project>& project, const std::string& uid);
		virtual ~BaseObject() = 0;
		
		std::weak_ptr<pbx::Project>&
		project();
		
		const std::weak_ptr<pbx::Project>&
		project() const;
		
		const std::string&
		uid() const;
	};
}
