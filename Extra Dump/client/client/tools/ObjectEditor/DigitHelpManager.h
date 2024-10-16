
#pragma once

class CDigitTarget
{
public:
	virtual void Increase(int step) = 0;
	virtual void Decrease(int step) = 0;
};

class CDigitHelpMgr
{
public:
	CDigitHelpMgr(void);
	~CDigitHelpMgr(void);

	static CDigitHelpMgr* ms_pInst;
	static CDigitHelpMgr*	CreateInstance()	{ return (ms_pInst = new CDigitHelpMgr()); };
	static CDigitHelpMgr*	GetInstance()		{ return ms_pInst; };
	static void				ReleaseInstance()	{ if(ms_pInst) delete ms_pInst; ms_pInst = NULL; };

	void IncreastTarget(int step);
	void DecreaseTarget(int step);

	void RequestHelp(CDigitTarget* pTarget);
	void ReleaseHelp();

	CDigitTarget* m_pCurrentTarget;
};
