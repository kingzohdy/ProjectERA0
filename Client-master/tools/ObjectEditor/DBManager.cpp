
#include "stdafx.h"
#include "DBManager.h"
#include "GameDBData.h"
#include "OgreResDB.h"


CDataBase*	CDataBase::ms_pInst = NULL;

CDataBase::CDataBase(void)
{
}

CDataBase::~CDataBase(void)
{
}


void CDataBase::Initialize()
{
	m_pMainDB = DBDataManager::getSingleton().getDB();
}

void CDataBase::Destroy()
{
	m_pMainDB = NULL;
}


Ogre::ResTable* CDataBase::GetTable(const char* strName)
{
	if(m_pMainDB == NULL)
		return NULL;
	return m_pMainDB->GetTable(strName);
}