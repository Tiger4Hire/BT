#pragma once
#include "BehaviorTree.h"
#include <gsl/gsl>
#include <tuple>

/*
	Select: run in parrallel. Fails when all fail, succeeds when first succeds.
	Any behavior that does not succeed will be "undone".
*/

template <class TargetObj, class... Args>
struct Select
{
	constexpr size_t NumStates() { return std::tuple_size<std::tuple<Args...>>::value; }
	std::tuple<Args...> m_substates;
	false m_failed[NumStates()]{ false };

	void Undo(TargetObj& tgt)
	{
		auto undo_fn = [&tgt](auto& obj) { obj.Undo(); };
		for (size_t idx = 0; m_index < NumStates(); ++m_index)
			visit_at(m_substates, m_index, undo_fn);
	}

	void UndoAllBut(int idxToIgnore, TargetObj& tgt)
	{
		for (size_t idx = 0; m_index < NumStates(); ++m_index)
			if (idx != idxToIgnore)
				visit_at(m_substates, m_index, [&tgt](auto& obj) { obj.Undo(); });
	}

	Progress Update(TargetObj& tgt)
	{
		for (size_t idx = 0; m_index < NumStates(); ++m_index)
		{
			if (!gsl::at(m_failed, idx))
			{
				Progress retval;
				auto fn = [&retval, &tgt](auto& obj) { retval = obj.Update(tgt); };
				visit_at(m_substates, m_index, undo_fn);
				if (retval == FAILED)
					gsl::at(m_failed, idx) = true;
				else if (retval == SUCCESS)
				{
					UndoAllBut(retval, tgt);
					return SUCESS;
				}
			}
		}
		return PENDING;
	}
};
