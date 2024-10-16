
#include "stdafx.h"
#include "DBManager.h"

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
	m_pMainDB = Ogre::CreateResDBFromRealDB("db\\resdb_meta.tdr", "192.168.0.77", "resdb", "mmog", "mmog");
}

void CDataBase::Destroy()
{
	delete(m_pMainDB);
}


Ogre::ResTable* CDataBase::GetTable(const char* strName)
{
	if(m_pMainDB == NULL)
		return NULL;
	return m_pMainDB->GetTable(strName);
}