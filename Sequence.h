#pragma once
#include "BehaviorTree.h"
#include <tuple>

/*
	Sequence: run in sequence. Succeeds when all succed, fails on first failure.
	Failure of any triggers previous succesful behaviors to be undone.
*/

template <class TargetObj, class... Args>
struct Sequence
{
	constexpr size_t NumStates() { return std::tuple_size<std::tuple<Args...>>::value; }
	std::tuple<Args...> m_substates;
	size_t m_index{ 0 };
	void Undo(TargetObj& tgt)
	{
		auto undo_fn = [&tgt](auto& obj) { obj.Undo(tgt); };
		for (size_t idx = m_index; m_index < NumStates(); --m_index)
			visit_at(m_substates, m_index, undo_fn);
	}
	Progress Update(TargetObj& tgt)
	{
		if (m_index == NumStates())
			return SUCCESS;
		Progress retval;
		auto fn = [&retval, &tgt](auto& obj) { retval = obj.Update(tgt); };
		visit_at(m_substates, m_index, fn);
		switch (retval)
		{
		case SUCCESS:
			m_index++;
			return PENDING;
		case FAIL:
			return FAIL;
		case PENDING:
			return PENDING;
		}
	}
};
