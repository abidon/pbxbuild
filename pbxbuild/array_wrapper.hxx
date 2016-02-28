#pragma once

#include <type_traits>

template <class Type>
pbx::ArrayWrapper<Type>::ArrayWrapper(const std::shared_ptr<pbx::Project>& project, const pbx::Array& array) :
pbx::BaseObject(project, "_ArrayWrapper"),
_collection(array)
{
	static_assert(std::is_base_of<pbx::BaseObject, Type>::value, "ArrayWrapper<T> template parameter must be a class inheriting from pbx::BaseObject");
}

template <class Type>
uint64_t pbx::ArrayWrapper<Type>::size() const
{
	return _collection.size();
}

template <class Type>
std::shared_ptr<Type> pbx::ArrayWrapper<Type>::operator[](const uint64_t idx)
{
	if (idx >= _collection.size())
		throw std::out_of_range("ArrayWrapper: index is out of range.");
	
	return std::dynamic_pointer_cast<Type>(project().lock()->get_object(_collection[idx].string_value()));
}

template <class Type>
std::shared_ptr<const Type> pbx::ArrayWrapper<Type>::operator[](const uint64_t idx) const
{
	if (idx >= _collection.size())
		throw std::out_of_range("ArrayWrapper: index is out of range.");
	
	return std::const_pointer_cast<const Type>(std::dynamic_pointer_cast<Type>(project().lock()->get_object(_collection[idx].string_value())));
}
