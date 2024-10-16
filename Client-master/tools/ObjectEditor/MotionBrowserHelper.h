
#pragma once
#include "MotionCategoryInfo.h"

class CMotionBrowserHelperSource: public COleDataSource
{
public:
	CATTYPE m_type;
	int m_catId;
	CString m_strMotionName;
protected:
	BOOL OnRenderFileData(LPFORMATETC lpFormatEtc,CFile* pFile);
};


struct MotionFileInfo
{
	CString strName;
	CString strChunkName;
	HTREEITEM hItem;
};

struct CatgoryDisplayInfo
{
	int iCatId;
	HTREEITEM hItem;
	//UINT state;
};

class CMotionBrowserHelper
{
public:
	CMotionBrowserHelper(void);
	~CMotionBrowserHelper(void);

	void LoadCatgory(CString strCatPath);
	void AddMotion(CString strMotion, CString strMotionFile);
	void ClearMotion();
	CString GetMotionNameByHandle(HTREEITEM hItem);
	HTREEITEM GetCatgoryHandleById(int id);
	int GetCatgoryIdByHandle(HTREEITEM hItem);

	int GetSibingCatIdByHandle(HTREEITEM hItem);
	HTREEITEM GetMotionHandleByName(CString strName);

	void UpdateTree();
	void DoItemDrag(HTREEITEM hItem);



	CCatgoryInfo					m_catgoryInfo;
	std::vector<MotionFileInfo* >	m_Motions;
	std::vector<CatgoryDisplayInfo> m_Catgory;

	HTREEITEM						m_hBascItem;
	CTreeCtrl*						m_pTreeCtrl;

	int								m_iImageMotion;
	int								m_iImageCatgory;

private:
	void UpdateCatgoryTree();
};
