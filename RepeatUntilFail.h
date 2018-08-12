#pragma once
#include "BehaviorTree.h"

template <class TargetObj, class Behavior>
struct RepeatUntilFail
{
	Behavior m_behavior;

	void Undo(TargetObj& tgt)
	{
		m_behavior.Undo(tgt);
	}

	Progress Update(TargetObj& tgt)
	{
		switch (m_behavior.Update(tgt))
		{
		case FAIL:
			return FAIL;
		case SUCCESS:
			m_behavior = Behavior();
			return PENDING;
		case PENDING:
			break;
		}
		return PENDING;
	}
};

template <class TargetObj, class Cond, class Behavior>
struct RepeatIfUntilFail
{
	Behavior m_behavior;

	void Undo(TargetObj& tgt)
	{
		m_behavior.Undo(tgt);
	}

	Progress Update(TargetObj& tgt)
	{
		switch (m_behavior.Update(tgt))
		{
		case FAIL:
			return FAIL;
		case SUCCESS:
			if (Cond()(tgt))
			{
				m_behavior = Behavior();
				return PENDING;
			}
			else
				return SUCCESS;
		case PENDING:
			break;
		}
		return PENDING;
	}
};
