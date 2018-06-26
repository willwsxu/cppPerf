#pragma once

#include <unordered_map>

template<typename AbstractProduct,
	typename ProductIdType,
	typename ProductCreator = AbstractProduct *(*CALLBACK)(),
	template<typename,typename> class ErrorPolicy = DefaultError>
class Factory : public ErrorPolicy<AbstractProduct, ProductIdType>
{
public:
	bool Register(const ProductIdType& type, ProductCreator callback)
	{
		return factoryCallbacks.emplace(type, callback).second;
	}
	
	bool UnRegister(const ProductIdType& type)
	{
		return factoryCallbacks.erase(type)==1;
	}

	AbstractProduct * CreateObject(const ProductIdType& type)
	{
		auto found = factoryCallbacks.find(type);
		if (found == factoryCallbacks.end())
			return OnUnknownType(type);
		return factoryCallbacks[type]();
	}

	static Factory& Instance()
	{
		static Factory factory;
		return factory;
	}

private:
	using Mapper = std::unordered_map<ProductIdType, ProductCreator>;
	Mapper factoryCallbacks;
};

template<typename AbstractProduct, typename ProductIdType>
class DefaultError
{
protected:
	AbstractProduct* OnUnknownType(const ProductIdType& type)
	{
		return nullptr;
	}
};