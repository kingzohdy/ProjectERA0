
#include "stdafx.h"
#include "MotionCategoryInfo.h"
#include <assert.h>

CTaskInfo::CTaskInfo(void)
{
}

CTaskInfo::~CTaskInfo(void)
{
}


void CTaskInfo::InitDB(CString strDbPath)
{
	/*
	m_Tasks.clear();

	Ogre::DBFile dbfile;
	if( false == dbfile.Init(strDbPath.GetBuffer(), g_TaskDbDesc))
	{
	return;
	}

	for(int i = 0; i < (int) dbfile.GetNumRecord(); i++)
	{
	Ogre::DBRecord rec = dbfile.GetIthRecord(i);
	MotionTask mt;
	mt.strMotionName = rec.GetString(0);
	mt.strMotionDesc = rec.GetString(1);
	m_Tasks.push_back(mt);
	}
	*/
}

CString CTaskInfo::FindDescByMotionName(CString strMotionName)
{
	assert(0);
	strMotionName.MakeUpper();
	for(int i = 0; i < (int) m_Tasks.size(); i++)
	{
		CString strExistName = m_Tasks[i].strMotionName;
		strExistName.MakeUpper();
		if(strExistName == strMotionName)
		{
			return m_Tasks[i].strMotionDesc;
		}

	}
	return "";
}
/*
Ogre::DBFIELD_DESC_T g_CatgoryDesc[] =
{
{0, Ogre::DBFIELD_UINT32, 0, true},	//id
{1, Ogre::DBFIELD_STRING, 4, false},	//name
{2, Ogre::DBFIELD_UINT32, 8, false},	//type
{3, Ogre::DBFIELD_UINT32, 12, false},//parent
{0, Ogre::DBFIELD_UNKNOWN, 0, false}
};
*/

void CCatgoryInfo::InitCatgoryDb(CString strDbPath)
{
	m_Catgory.clear();
	m_MotionStoragy.clear();

	assert(0);
	/*
	Ogre::DBFile dbfile;
	if( false == dbfile.Init(strDbPath.GetBuffer(), g_CatgoryDesc))
	{
	return;
	}

	for(int i = 0; i < (int) dbfile.GetNumRecord(); i++)
	{
	Ogre::DBRecord rec = dbfile.GetIthRecord(i);
	CATTYPE type = (CATTYPE)rec.GetUInt(2);
	if(type == CT_FOLDER)
	{
	MotionCatgory cat;
	cat.iId = rec.GetUInt(0);
	cat.strCatgoryName = rec.GetString(1);
	cat.iParentId = rec.GetUInt(3);

	m_Catgory.push_back(cat);
	}
	else if (type == CT_ITEM)
	{
	MotionCatgoryStoragy motion;
	motion.iParentId = rec.GetUInt(3);
	motion.strMotionName = rec.GetString(1);

	m_MotionStoragy.push_back(motion);
	}
	}
	*/
}

void CCatgoryInfo::SaveCatgoryInfo(CString strPath)
{
	FILE* pFile = fopen(strPath.GetBuffer(), "w");
	for(int i = 0; i < (int)m_Catgory.size(); i++)
	{
		fprintf(pFile, "%d,%s,%d,%d\r\n"
			, m_Catgory[i].iId, m_Catgory[i].strCatgoryName.GetBuffer()
			, (int)CT_FOLDER, m_Catgory[i].iParentId);
	}

	for(int i = 0; i < (int)m_MotionStoragy.size(); i++)
	{
		fprintf(pFile, "%d,%s,%d,%d\r\n"
			, 0,  m_MotionStoragy[i].strMotionName.GetBuffer()
			, (int)CT_ITEM, m_MotionStoragy[i].iParentId);
	}

	fclose(pFile);
}

int	CCatgoryInfo::GetMotionCatId(CString strMotionName)
{
	strMotionName.MakeUpper();

	for(int i = 0; i < (int) m_MotionStoragy.size(); i++)
	{
		CString strExist = m_MotionStoragy[i].strMotionName;
		strExist.MakeUpper();
		if(strExist == strMotionName)
		{
			return m_MotionStoragy[i].iParentId;
		}
	}
	return 0;
}

MotionCatgoryStoragy* 	CCatgoryInfo::FindMotion(CString strMotionName)
{
	strMotionName.MakeUpper();

	for(int i = 0; i < (int) m_MotionStoragy.size(); i++)
	{
		CString strExist = m_MotionStoragy[i].strMotionName;
		strExist.MakeUpper();
		if(strExist == strMotionName)
		{
			return &m_MotionStoragy[i];
		}
	}
	return NULL;
}



int CCatgoryInfo::AddCatgory(CString strName, int iParent)
{
	int iNewId = 1;
	bool bParentExist = false;
	for(int i = 0; i < (int) m_Catgory.size(); i++)
	{
		if(m_Catgory[i].iId >= iNewId)
		{
			iNewId = m_Catgory[i].iId + 1;
		}

		if(m_Catgory[i].iId == iParent)
		{
			bParentExist = true;
		}
	}

	if(!bParentExist)
	{
		iParent = 0;
	}

	MotionCatgory mc;
	mc.iId = iNewId;
	mc.iParentId = iParent;
	mc.strCatgoryName = strName;

	m_Catgory.push_back(mc);
	return int(m_Catgory.size() - 1);
}

bool CCatgoryInfo::RemoveCatgory(int id)
{
	int iParent = 0;
	for(int i = 0; i < (int) m_Catgory.size(); i++)
	{
		if(m_Catgory[i].iId == id)
		{
			iParent = m_Catgory[i].iParentId;
			m_Catgory.erase(m_Catgory.begin() + i);

			break;
		}
	}

	for(int i = 0; i < (int)m_Catgory.size(); i++)
	{
		if(m_Catgory[i].iParentId == id)
		{
			m_Catgory[i].iParentId = iParent;
		}
	}

	for(int i = 0; i < (int)m_MotionStoragy.size(); i++)
	{
		if(m_MotionStoragy[i].iParentId == id)
		{
			m_MotionStoragy[i].iParentId = iParent;
		}
	}

	return true;
}

void CCatgoryInfo::GetChildCatgory(int iParent, std::vector<MotionCatgory*>& ret)
{
	for(int i = 0; i < (int) m_Catgory.size(); i++)
	{
		if(m_Catgory[i].iParentId == iParent)
		{
			ret.push_back(&m_Catgory[i]);
		}
	}
}

MotionCatgory* CCatgoryInfo::FindCatgory(int id)
{
	for(int i = 0; i < (int) m_Catgory.size(); i++)
	{
		if(m_Catgory[i].iId == id)
			return &m_Catgory[i];
	}

	return NULL;
}


bool CCatgoryInfo::IsChildOrSelf(int iFather, int iChild)
{
	int iCurr = iChild;
	bool ret = false;
	MotionCatgory* pCat = FindCatgory(iChild);
	while(pCat != NULL)
	{
		if(pCat->iId == iFather)
		{
			return true;
		}

		pCat = FindCatgory(pCat->iParentId);
	}

	return false;
}
