#pragma once
#include "BehaviorTree.hpp"
#include <gsl/gsl>
#include <stack>

template <typename TargetObj, class... Args>
class PDSA
{
	using State = std::variant<std::monostate, Args...>;
	std::stack<State> m_states;

public:
	Progress Update(TargetObj& tgt)
	{
		Expects(!m_states.empty());
		Progress result = m_states.back().Update(tgt, *this);
		switch (result)
		{
		case FAIL:
			while (!m_states.empty())
			{
				m_states.back().Undo(tgt);
				m_states.pop();
			}
			return FAIL;
		case SUCCESS:
			m_states.pop();
			if (m_states.empty())
				return SUCCESS;
			/*Fall through*/
		case PENDING:
			break;
		}
		return PENDING;
	}
	template <typename NewStateType, typename... LocalArgs>
	void push(LocalArgs&&... args)
	{
		m_states.push(State());
		State& v = m_states.top();
		v.template emplace<1, LocalArgs...>(std::forward<LocalArgs>(args)...);
	}
};