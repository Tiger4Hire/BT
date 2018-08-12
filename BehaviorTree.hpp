#pragma once
enum Progress
{
	FAIL,
	SUCCESS,
	PENDING
};

template <typename TargetObj, typename Behavior>
class BehaviorTree
{
	TargetObj m_obj;
	Behavior m_state;

public:
	Progress Update()
	{
		Progress retval = m_state.Update(m_obj);
		if (retval == FAIL)
			m_state.Undo(m_obj);
		return retval;
	}
};
