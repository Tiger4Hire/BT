#pragma once
#include "BehaviorTree.h"
#include "Utils.h"
#include <exception>
#include <variant>

template <typename TargetObj, typename EvalType, typename... Args>
struct Switch
{
	std::variant<std::monostate, Args...> m_state;
	void Undo(TargetObj& tgt)
	{
		std::visit(overload{ [](std::monostate) {}, [&tgt](auto& state) { state.Undo(tgt); } }, m_state);
	}
	Progress Update(TargetObj& tgt)
	{
		if (m_state.index() == 0)
		{
			auto eval = EvalType()(tgt);
			m_state = Construct<Args...>()(eval);
		}
		return std::visit(overload{ [](std::monostate) { return FAIL; }, [&tgt](auto& state) { return state.Update(tgt); } }, m_state);
	}
};