#pragma once

#include <iostream>
#include <memory>
using namespace std;

template <typename R, typename Param1, typename Param2>
struct FunctorImpl
{
	virtual R operator()(Param1 p1, Param2 p2) = 0;
	virtual FunctorImpl* Clone() const = 0;
	virtual ~FunctorImpl() {}
};

template <typename Parent, typename Fun>
struct FunctorHandler : public FunctorImpl<typename Parent::R, typename Parent::P1, typename Parent::P2>
{
	typedef typename Parent::R ResultType;
	FunctorHandler(const Fun& fun) : fun_(fun) {}
	FunctorHandler* Clone() const override
	{
		return new FunctorHandler(*this);
	}
	ResultType operator()(typename Parent::P1 p1, typename Parent::P2 p2) override
	{
		return fun_(p1, p2);
	}
private:
	Fun fun_;
};

template <typename ResultType, typename Param1, typename Param2>
class Functor
{
public:
	typedef ResultType  R;  // these typedefs are necessary for other class to refer to
	typedef Param1		P1;
	typedef Param2		P2;
	typedef FunctorImpl<R, P1, P2> Impl;
	Functor() = default;
	Functor(const Functor&) = default;
	Functor& operator=(const Functor&) = default;
	explicit Functor(std::unique_ptr<Impl> Impl) :m_impl(move(spImpl)) {}  // Impl typedef has to be delcared before use

	template <class Fun>
	Functor(const Fun& fun) : m_impl(new FunctorHandler<Functor, Fun>(fun)) {}

	R operator()(P1 p1, P2 p2)
	{
		return (*m_impl)(p1, p2);
	}

private:
	unique_ptr<Impl>	m_impl;
};
