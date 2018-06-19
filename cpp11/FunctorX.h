#pragma once

#include <iostream>
using namespace std;

template<class...T> struct type_list {};

template <typename R, typename...Param>
struct FunctorImpl
{
	virtual R operator()(Param...args) = 0;
	virtual FunctorImpl* Clone() const = 0;
	virtual ~FunctorImpl() {}
};

template <typename Parent, typename Fun, typename...Pack>
struct FunctorHandler : public FunctorImpl<typename Parent::R, Pack...>
{
	typedef typename Parent::R ResultType;
	FunctorHandler(const Fun& fun) : fun_(fun) {}
	FunctorHandler* Clone() const override
	{
		return new FunctorHandler(*this);
	}
	ResultType operator()(Pack...args) override
	{
		return fun_(forward<Pack>(args)...);
	}
private:
	Fun fun_;
};

template <typename ResultType, typename... Param>
class Functor
{
public:
	typedef ResultType  R;  // these typedefs are necessary for other class to refer to
	typedef type_list<Param...>		TL;  // cannot typedef Param...
	using Impl = FunctorImpl<R, Param...>;
	Functor() = default;
	Functor(const Functor&) = default;
	Functor& operator=(const Functor&) = default;
	explicit Functor(std::unique_ptr<Impl> Impl) :m_impl(move(spImpl)) {}  // Impl typedef has to be delcared before use

	template <class Fun>
	Functor(const Fun& fun) : m_impl(new FunctorHandler<Functor, Fun, Param...>(fun)) {}

	R operator()(Param...p)
	{
		return (*m_impl)(forward<Param>(p)...);
	}

private:
	unique_ptr<Impl>	m_impl;
};
