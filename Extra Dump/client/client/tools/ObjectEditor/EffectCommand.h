
#pragma once
#include <deque>

class CEffectCommand
{
public:
	CEffectCommand(void);
	~CEffectCommand(void);

	virtual CEffectUnit*	GetTarget() {return NULL;};
	virtual bool			NeedRefresh() { return false;};
	virtual CEffectCommand* GetInversCommand() { return NULL;};
	virtual bool			IsIdentical() { return false; };
	virtual bool			Merge(CEffectCommand* pCmd, bool bForceMerge) { return false;}

};

class CCommandMgr
{
public:
	CCommandMgr(void);
	~CCommandMgr(void);

	void Undo();
	void Redo();

	bool CanUndo();
	bool CanRedo();

	void OnCommandDone(CEffectCommand* pCommand);

	void SetEnable(bool bEnable)	{ m_bEnable = bEnable; };
	bool GetEnable()				{ return m_bEnable; }; 
	void ForceNewCommand(bool b = true) { m_bForceNewCommand = b; };

	bool m_bEnable;
	bool m_bForceNewCommand;
	std::deque<CEffectCommand*> m_UndoCmd;
	std::deque<CEffectCommand*> m_RedoCmd;
};

