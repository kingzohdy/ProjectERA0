
#include "stdafx.h"
#include "DigitHelpManager.h"


CDigitHelpMgr* CDigitHelpMgr::ms_pInst = NULL;
CDigitHelpMgr::CDigitHelpMgr(void) : m_pCurrentTarget(NULL)
{
}

CDigitHelpMgr::~CDigitHelpMgr(void)
{
}


void CDigitHelpMgr::IncreastTarget(int step)
{
	if(m_pCurrentTarget)
		m_pCurrentTarget->Increase(step);
}

void CDigitHelpMgr::DecreaseTarget(int step)
{
	if(m_pCurrentTarget)
		m_pCurrentTarget->Decrease(step);
}

void CDigitHelpMgr::RequestHelp(CDigitTarget* pTarget)
{
	m_pCurrentTarget = pTarget;
}

void CDigitHelpMgr::ReleaseHelp()
{
	m_pCurrentTarget = NULL;
}


