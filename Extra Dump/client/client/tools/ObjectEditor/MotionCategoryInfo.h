#pragma once
#include <vector>

struct MotionTask
{
	CString strMotionName;
	CString strMotionDesc;
};

class CTaskInfo
{
public:
	CTaskInfo(void);
	~CTaskInfo(void);

	void InitDB(CString strDbPath);
	CString FindDescByMotionName(CString strMotionName);
	std::vector<MotionTask> m_Tasks;


};

enum CATTYPE
{
	CT_FOLDER = 0,
	CT_ITEM
};

struct MotionCatgory
{
	CString strCatgoryName;
	int iParentId;
	int iId;
};

struct MotionCatgoryStoragy
{
	CString strMotionName;
	int iParentId;
};


class CCatgoryInfo
{
public:
	void InitCatgoryDb(CString strDbPath);
	int	GetMotionCatId(CString strMotionName);
	MotionCatgoryStoragy* FindMotion(CString strMotionName);

	int AddCatgory(CString strName, int iParent);
	bool RemoveCatgory(int id);
	MotionCatgory* FindCatgory(int id);

	void SaveCatgoryInfo(CString strPath);

	void GetChildCatgory(int iParent, std::vector<MotionCatgory*>& ret);

	bool IsChildOrSelf(int iFather, int iChild);

	std::vector<MotionCatgory> m_Catgory;
	std::vector<MotionCatgoryStoragy> m_MotionStoragy;
};
