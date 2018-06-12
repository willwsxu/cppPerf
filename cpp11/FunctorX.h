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

template <typename Parent, typename Fun>
struct FunctorHandler : public FunctorImpl<typename Parent::R, typename Parent::P...>
{
	typedef typename Parent::R ResultType;
	FunctorHandler(const Fun& fun) : fun_(fun) {}
	FunctorHandler* Clone() const override
	{
		return new FunctorHandler(*this);
	}
	ResultType operator()(typename Parent::P...args) override
	{
		return fun_(forward<typename Parent::P>(args)...);
	}
private:
	Fun fun_;
};

template <typename ResultType, typename... Param>
class Functor
{
public:
	typedef ResultType  R;  // these typedefs are necessary for other class to refer to
	typedef Param...		P;
	using ImplType = <R, P...> Impl;
	Functor() = default;
	Functor(const Functor&) = default;
	Functor& operator=(const Functor&) = default;
	explicit Functor(std::unique_ptr<Impl> Impl) :m_impl(move(spImpl)) {}  // Impl typedef has to be delcared before use

	template <class Fun>
	Functor(const Fun& fun) : m_impl(new FunctorHandler<Functor, Fun>(fun)) {}

	R operator()(P...p)
	{
		return (*m_impl)(forward<P>...p);
	}

private:
	unique_ptr<Impl>	m_impl;
};
