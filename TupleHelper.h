#pragma once
#include <tuple>

template <size_t I>
struct visit_impl
{
	template <typename T, typename F>
	static void visit(T& tup, size_t idx, F fun)
	{
		if (idx == I - 1)
			fun(std::get<I - 1>(tup));
		else
			visit_impl<I - 1>::visit(tup, idx, fun);
	}
};

template <>
struct visit_impl<0>
{
	template <typename T, typename F>
	static void visit(T& tup, size_t idx, F fun) { assert(false); }
};

template <typename F, typename... Ts>
void visit_at(std::tuple<Ts...>& tup, size_t idx, F fun)
{
	visit_impl<sizeof...(Ts)>::visit(tup, idx, fun);
}
