#include "build_file.hpp"

pbx::BuildFile::BuildFile(const std::shared_ptr<pbx::Project>& project, const std::string& uid, const pbx::Dictionary& data) :
pbx::BaseObject(project, uid),
_file_ref(data.at("fileRef").string_value()),
_settings(data.count("settings") ? data.at("settings").dictionary_value() : pbx::Dictionary())
{}

std::shared_ptr<const pbx::FileReference> pbx::BuildFile::file() const
{
	return project().lock()->get_object<pbx::FileReference>(_file_ref);
}
