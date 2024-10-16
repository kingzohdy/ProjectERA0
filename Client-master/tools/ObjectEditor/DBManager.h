
#pragma once
#include "OgrePrerequisites.h"

class DBDataManager;
class CDataBase
{
public:
	CDataBase(void);
	~CDataBase(void);

	static CDataBase*	ms_pInst;
	static CDataBase*	CreateInstance()	{ return (ms_pInst = new CDataBase()); };
	static CDataBase*	GetInstance()		{ return ms_pInst; };
	static void			ReleaseInstance()	{ if(ms_pInst) delete ms_pInst; ms_pInst = NULL; };

	void Initialize();
	void Destroy();

	Ogre::ResTable*	GetTable(const char* strName);
	Ogre::ResDB*		GetMainDB()				{ return m_pMainDB;};


	Ogre::ResDB* m_pMainDB;
	DBDataManager *m_pDBMgr;
};
