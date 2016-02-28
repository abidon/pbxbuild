#pragma once

#include "base_object.hpp"
#include "../libpbxparser/value.hpp"
#include <cstdint>

namespace pbx {
	
	template <class Type>
	class ArrayWrapper : public pbx::BaseObject
	{
		pbx::Array _collection;
		
	public:
		ArrayWrapper(const std::shared_ptr<pbx::Project>& project, const pbx::Array& array);
		
		uint64_t size() const;
		
		std::shared_ptr<Type> operator[](const uint64_t idx);
		std::shared_ptr<const Type> operator[](const uint64_t idx) const;
	};
}

#include "array_wrapper.hxx"
