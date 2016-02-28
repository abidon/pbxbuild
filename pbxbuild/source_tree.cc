#include "source_tree.hpp"

pbx::SourceTree pbx::to_source_tree(const std::string& str)
{
	if (str == "<group>")
		return pbx::SourceTree::Group;
	else if (str == "BUILT_PRODUCTS_DIR")
		return pbx::SourceTree::Build;
	return pbx::SourceTree::Absolute; // TODO
}