#pragma once

#include <iostream>
using namespace std;

template<class...T> struct param_list {};

template <typename R, typename...Param>
struct FunctorImpl : param_list<Param...>
{
	virtual R operator()(Param&&...args) = 0;  // function template cannot be virtual
	virtual FunctorImpl* Clone() const = 0;
	virtual ~FunctorImpl() {}
};

// FunctorHandler and MemFunctorHandler are for internal use of functor
// May hide them
template <typename ResultType, typename Fun, typename...Pack>
struct FunctorHandler : public FunctorImpl<ResultType, Pack...>
{
	//typedef typename Parent::R ResultType;
	FunctorHandler(const Fun& fun) : fun_(fun) {}
	FunctorHandler* Clone() const override
	{
		return new FunctorHandler(*this);
	}
	ResultType operator()(Pack&&...args) override
	{
		return fun_(forward<Pack>(args)...);
	}
private:
	Fun fun_;
};
	
// Functor for member function
template <typename ResultType, typename PointerToObj, typename PointerToMemFn, typename...Pack>
struct MemFunctorHandler : public FunctorImpl<typename ResultType, Pack...>
{
	//typedef typename Parent::R ResultType;
	MemFunctorHandler(PointerToObj p, PointerToMemFn fun) : pObj_(p), pMemFn_(fun) {}
	MemFunctorHandler* Clone() const override
	{
		return new MemFunctorHandler(*this);
	}
	ResultType operator()(Pack&&...args) override
	{
		return (pObj_->*pMemFn_)(forward<Pack>(args)...);
	}
private:
	PointerToObj pObj_;     // not ObjectType *pObj, more generic, can be any type of pointer, smart pointer
	PointerToMemFn pMemFn_;
};

template <typename ResultType, typename... Param>
class Functor
{
public:
	typedef ResultType  R;  // these typedefs are necessary for other class to refer to
	using Impl = FunctorImpl<R, Param...>;
	Functor() = default;
	Functor(const Functor&) = default;  // copy constructor is not allowed if there is unique_ptr data memeber
	Functor& operator=(const Functor&) = default;

	// conflict with Functor(const Fun& fun)
	//explicit Functor(std::unique_ptr<Impl> spImpl) :m_impl(move(spImpl)) {}  // Impl typedef has to be delcared before use

	template <class Fun>
	Functor(const Fun& fun) : m_impl(new FunctorHandler<R, Fun, Param...>(fun)) {}

	template <class Obj, class Fun>
	Functor(const Obj& obj, const Fun& fun) : m_impl(new MemFunctorHandler<R, Obj, Fun, Param...>(obj, fun)) {}

	R operator()(Param&&...p)
	{
		return (*m_impl)(forward<Param>(p)...);
	}

private:
	shared_ptr<Impl>	m_impl;
};

template<typename Incoming, typename First, typename...Pack>
class BindFirst : public FunctorImpl<typename Incoming::R, Pack...>
{
	typedef typename Incoming::R ResultType;
public:
	BindFirst(const Incoming& fun, First bound) :fun_(fun), bound_(bound) {}
	BindFirst* Clone() const override
	{
		return new BindFirst(*this);
	}
	ResultType operator()(Pack&&...args) override
	{
		// Don't forward bound_ data member!!!
		return fun_(First(bound_), forward<Pack>(args)...);
	}

private:
	Incoming fun_;
	const First    bound_;  // make it const so it is not accidently moved
};