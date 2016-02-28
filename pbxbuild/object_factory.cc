#include "object_factory.hpp"

pbx::ObjectFactory::ObjectFactory()
{}

std::shared_ptr<pbx::BaseObject> pbx::ObjectFactory::build(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& info)
{
	std::string isa = info.at("isa").string_value();
	
	std::function<std::shared_ptr<pbx::BaseObject>(const std::shared_ptr<pbx::Project>&, const std::string&, const pbx::Dictionary&)> builder;
	
	try {
		builder = _builders.at(isa);
	} catch(const std::out_of_range& exc)
	{
		throw std::invalid_argument("No builder registered for isa " + isa);
	}
	
	return builder(project, uid, info);
}

void pbx::ObjectFactory::register_builder(const std::string& isa, const std::function<std::shared_ptr<pbx::BaseObject>(const std::shared_ptr<pbx::Project>&, const std::string&, const pbx::Dictionary&)>& builder)
{
	_builders[isa] = builder;
}
