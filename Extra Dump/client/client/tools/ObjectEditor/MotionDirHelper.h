
#pragma once
#include <map>

enum MOTION_ITEM_TYPE
{
	MIT_MOTION,
	MIT_DIR,
	MIT_IDENTIFIER
};


class MotionItemInfo
{
public:
	CString strChunkName;
	CString strTitle;

	MOTION_ITEM_TYPE type;
	HTREEITEM hTreeItem;
	CString strExt;

	CString GetFilePath();
	CString	GetPlayName();
};


class CMotionDirHelper
{
public:
	CMotionDirHelper(void);
	virtual ~CMotionDirHelper(void);

	void	UpdateDir(MotionItemInfo* pParentItem);

	bool	HasSubDir(CString path);

	void	SetBaseItem(CString strChunk, HTREEITEM hBaseItem);

	MotionItemInfo*	GetInfoByHandle(HTREEITEM hItem);
	CString	GetFileExtFromName(CString path);

	HTREEITEM						m_hBascItem;
	CTreeCtrl*						m_pTreeCtrl;

	int								m_iImageMotion;
	int								m_iImageDir;
	int								m_iImageMotionGray;
	CString							m_strFileExt;

	void	SetFileExt(CString ext);
	void	ClearData();

	typedef std::map<HTREEITEM, MotionItemInfo*> MOTIONMAP;

	std::map<HTREEITEM, MotionItemInfo*>	m_MotionItems;
};
