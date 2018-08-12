#include "RepeatUntil.h"
#include "Sequence.h"
#include "Switch.h"
#include "TupleHelper.h"
#include "std.h"
#include <gsl/gsl>
using namespace std;

enum StateId
{
	S1,
	S2,
	S3,
	Seq,
	Fail
};
StateId Next(StateId id)
{
	switch (id)
	{
	case S1:
		return S2;
	case S2:
		return S3;
	case S3:
		return Seq;
	case Seq:
		return Fail;
	case Fail:
		Expects(false);
	}
}

struct Stuff
{
	StateId m_state{ S1 };
};

struct GetState
{
	StateId operator()(const Stuff& stuff) { return stuff.m_state; }
};

struct NotFail
{
	bool operator()(const Stuff& stuff) { return stuff.m_state != Fail; }
};

struct NextState
{
	void Undo(Stuff&) {}
	Progress Update(Stuff& stuff)
	{
		stuff.m_state = Next(stuff.m_state);
		return SUCCESS;
	}
};

struct State1
{
	void Undo(Stuff& stuff) {}
	Progress Update(Stuff&)
	{
		cout << "State1\n";
		return SUCCESS;
	}
};

struct State2
{
	void Undo(Stuff& stuff) {}
	Progress Update(Stuff&)
	{
		cout << "State2\n";
		return SUCCESS;
	}
};

struct State3
{
	void Undo(Stuff& stuff) {}
	Progress Update(Stuff&)
	{
		cout << "State3\n";
		return SUCCESS;
	}
};

struct FailFast
{
	void Undo(Stuff&) {}
	Progress Update(Stuff&) { return FAIL; }
};

int main()
{
	using Seq1 = Sequence<Stuff, State1, State3>;
	using MySwitch = Switch<Stuff, GetState, State1, State2, State3, Seq1, FailFast>;
	using Seq2 = Sequence<Stuff, MySwitch, NextState>;
	using Repeater = RepeatIfUntilFail<Stuff, NotFail, Seq2>;
	BehaviorTree<Stuff, Repeater> behavior;
	Progress progress;
	while ((progress = behavior.Update()) == PENDING)
		/*WAIT*/;
	switch (progress)
	{
	case SUCCESS:
		cout << "Success\n";
		break;
	case FAIL:
		cout << "Fail\n";
		break;
	case PENDING:
	default:
		Expects(false);
	}
}