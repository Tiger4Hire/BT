#pragma once
#include <exception>
#include <gsl/gsl>
#include <variant>

template <class... Ts>
struct overload : Ts...
{
	using Ts::operator()...;
};
template <class... Ts>
overload(Ts...)->overload<Ts...>;

template <std::size_t idx, typename Retval, typename... Args>
struct ConstructHelper;

template <std::size_t idx, typename Retval, typename First, typename... Args>
struct ConstructHelper<idx, Retval, First, Args...>
{
	Retval operator()(int i)
	{
		if (idx == i)
			return First();
		else
			return ConstructHelper<idx + 1, Retval, Args...>()(i);
	}
};

template <std::size_t idx, typename Retval, typename First>
struct ConstructHelper<idx, Retval, First>
{
	Retval operator()(int i)
	{
		Expects(idx == i);
		return First();
	}
};

template <typename... Args>
struct Construct
{
	typedef std::variant<std::monostate, Args...> RetvalType;
	RetvalType operator()(int selector)
	{
		return ConstructHelper<0, RetvalType, Args...>()(selector);
	}
};