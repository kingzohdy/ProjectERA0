
#include "stdafx.h"
#include "EffectCommand.h"
#include "EffectManager.h"

CEffectCommand::CEffectCommand(void)
{
}

CEffectCommand::~CEffectCommand(void)
{
}

CCommandMgr::CCommandMgr(void)
{
	m_bEnable = true;
	m_bForceNewCommand = true;
}

CCommandMgr::~CCommandMgr(void)
{
	std::deque<CEffectCommand*>::iterator it = m_RedoCmd.begin();
	for(; it != m_RedoCmd.end(); it++)
	{
		delete (*it);
	}

	it = m_UndoCmd.begin();
	for(; it != m_UndoCmd.end(); it++)
	{
		delete (*it);
	}

	m_RedoCmd.clear();
	m_UndoCmd.clear();
}

const int UNDO_LEN = 10;
const int REDO_LEN = 10;

void CCommandMgr::Undo()
{
	if(CanUndo())
	{
		CEffectCommand* pCmd = m_UndoCmd.front();
		CEffectCommand* pRedoCmd = pCmd->GetInversCommand();

		CEffectMgr::GetInstance()->ExecuteCommand(pCmd);

		delete pCmd;
		m_UndoCmd.pop_front();

		m_RedoCmd.push_front(pRedoCmd);
		if(m_RedoCmd.size() > REDO_LEN)
		{
			delete m_RedoCmd.back();
			m_RedoCmd.pop_back();
		}
	}
}

void CCommandMgr::Redo()
{
	if(CanRedo())
	{
		CEffectCommand* pCmd = m_RedoCmd.front();
		CEffectCommand* pUndoCmd = pCmd->GetInversCommand();

		CEffectMgr::GetInstance()->ExecuteCommand(pCmd);

		delete pCmd;
		m_RedoCmd.pop_front();

		m_UndoCmd.push_front(pUndoCmd);
		if(m_UndoCmd.size() > UNDO_LEN)
		{
			delete m_UndoCmd.back();
			m_UndoCmd.pop_back();
		}
	}
}

bool CCommandMgr::CanUndo()
{
	return m_UndoCmd.size() > 0;
}

bool CCommandMgr::CanRedo()
{
	return m_RedoCmd.size() > 0;
}

void CCommandMgr::OnCommandDone(CEffectCommand* pCommand)
{
	if(m_bEnable == false)
	{
		delete pCommand;
	}
	else if(pCommand->IsIdentical())
	{
		delete pCommand;
	}
	else if(!m_bForceNewCommand && m_UndoCmd.size() > 0 && m_UndoCmd.front()->Merge(pCommand, false) == true )
	{
		delete pCommand;
	}
	else
	{
		m_bForceNewCommand = false;
		while(m_RedoCmd.size() != 0)
		{
			delete m_RedoCmd.front();
			m_RedoCmd.pop_front();
		}

		m_UndoCmd.push_front(pCommand);
		if(m_UndoCmd.size() > UNDO_LEN)
		{
			delete m_UndoCmd.back();
			m_UndoCmd.pop_back();
		}
	}
}