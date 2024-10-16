
#include "stdafx.h"
#include "MotionDirHelper.h"

CString MotionItemInfo::GetFilePath()
{
	CString ret = "*\\chunkdata\\";
	ret.Append(strChunkName);
	ret.Replace('.', '\\');

	if(type != MIT_DIR)
	{
		ret.Append(strExt);	
	}
	ret.SetAt(0,  '.');
	return ret;
}

CString	MotionItemInfo::GetPlayName()
{
	CString strMotionAddr = g_strPublicMotionAddr;
	strMotionAddr.MakeUpper();
	int iHeadLen = strMotionAddr.GetLength();

	CString strChunkHeader = strChunkName.Left(iHeadLen);
	strChunkHeader.MakeUpper();

	if (strMotionAddr == strChunkHeader) 
	{
		return strChunkName.Right(strChunkName.GetLength() - iHeadLen);
	}

	return strChunkName;
}

CMotionDirHelper::CMotionDirHelper(void)
{
}

CMotionDirHelper::~CMotionDirHelper(void)
{
}

void	CMotionDirHelper::ClearData()
{
	MOTIONMAP::iterator it = m_MotionItems.begin();
	for(; it != m_MotionItems.end(); ++it)
	{
		if(it->second->hTreeItem != m_hBascItem && m_pTreeCtrl->GetSafeHwnd())
		{
			m_pTreeCtrl->DeleteItem(it->second->hTreeItem);
		}
		delete it->second;
	}
	m_MotionItems.clear();
}

void CMotionDirHelper::UpdateDir(MotionItemInfo* pParentItem)
{
	HTREEITEM hChild = m_pTreeCtrl->GetChildItem(pParentItem->hTreeItem);

	//删除原来的item
	while(hChild != NULL)
	{
		HTREEITEM hTmp = hChild;
		hChild = m_pTreeCtrl->GetNextSiblingItem(hChild);

		MOTIONMAP::iterator it = m_MotionItems.find(hTmp);
		if(it != m_MotionItems.end())
		{
			delete it->second;
			m_MotionItems.erase(it);
		}

		m_pTreeCtrl->DeleteItem(hTmp);
	}

	if (pParentItem->type != MIT_DIR) 
	{
		return;
	}

	CString strPath = ".\\chunkdata\\";
	CString strTmp = pParentItem->strChunkName;
	strTmp.Replace(".", "\\");
	strPath += strTmp + "\\*.*";

	CFileFind finder;
	BOOL bWork = finder.FindFile(strPath.GetBuffer(), 0);
	while (bWork) 
	{
		bWork = finder.FindNextFile();
		if(finder.GetFileName() == "." || finder.GetFileName() == "..")
		{
			continue;
		}

		if(finder.IsDirectory())
		{
			HTREEITEM hItem = m_pTreeCtrl->InsertItem(finder.GetFileName().GetBuffer(), m_iImageDir, m_iImageDir, pParentItem->hTreeItem);
			MotionItemInfo* pNewItem = new MotionItemInfo();
			pNewItem->hTreeItem = hItem;
			pNewItem->strTitle = finder.GetFileName();
			pNewItem->strChunkName = pParentItem->strChunkName + finder.GetFileName() + ".";
			pNewItem->type = MIT_DIR;
			//pNewItem->strExt = m_strFileExt;

			m_MotionItems[hItem] = pNewItem;

			if(HasSubDir(finder.GetFilePath()) == TRUE)
			{
				HTREEITEM hIdnItem = m_pTreeCtrl->InsertItem("查找中...", m_iImageDir, m_iImageDir, hItem);
				MotionItemInfo* pNewIdnItem = new MotionItemInfo();
				pNewIdnItem->hTreeItem = hIdnItem;
				pNewIdnItem->strTitle = "";
				pNewIdnItem->strChunkName = "";
				pNewIdnItem->type = MIT_IDENTIFIER;

				m_MotionItems[hIdnItem] = pNewIdnItem;
			}

		}
		else
		{
			CString ext = GetFileExtFromName(finder.GetFilePath());
			ext.MakeUpper();

			if(ext != m_strFileExt)
				continue;

			MotionItemInfo* pNewItem = new MotionItemInfo();

			pNewItem->strTitle = finder.GetFileTitle();
			pNewItem->strChunkName = pParentItem->strChunkName + finder.GetFileTitle();
			pNewItem->type = MIT_MOTION;
			pNewItem->strExt = m_strFileExt;

			HTREEITEM hItem = m_pTreeCtrl->InsertItem(finder.GetFileTitle().GetBuffer(), m_iImageMotion, m_iImageMotion, pParentItem->hTreeItem);
			pNewItem->hTreeItem = hItem;

			m_MotionItems[hItem] = pNewItem;

		}
	}


}


bool CMotionDirHelper::HasSubDir(CString path)
{
	CFileFind fileFind;
	BOOL bWorking = fileFind.FindFile(path + "\\*.*", 0);
	while(bWorking)
	{
		bWorking = fileFind.FindNextFile();
		if(fileFind.GetFileName() == "." || fileFind.GetFileName() == "..")
			continue;
		//if(fileFind.IsDirectory())
		{
			return true;
		}

	}
	return false;
}

void CMotionDirHelper::SetBaseItem(CString strChunk, HTREEITEM hBaseItem)
{
	MOTIONMAP::iterator it = m_MotionItems.begin();
	for(; it != m_MotionItems.end(); ++it)
	{
		if(it->second->hTreeItem != m_hBascItem)
		{
			m_pTreeCtrl->DeleteItem(it->second->hTreeItem);
		}
		delete it->second;
	}
	m_MotionItems.clear();


	MotionItemInfo* pNewBaseItem = new MotionItemInfo();
	pNewBaseItem->hTreeItem = hBaseItem;
	pNewBaseItem->strChunkName = strChunk;
	pNewBaseItem->strTitle = "";
	pNewBaseItem->type = MIT_DIR;

	m_hBascItem = hBaseItem;
	m_MotionItems[m_hBascItem] = pNewBaseItem;

}

MotionItemInfo*	CMotionDirHelper::GetInfoByHandle(HTREEITEM hItem)
{
	MOTIONMAP::iterator it = m_MotionItems.find(hItem);

	if(it != m_MotionItems.end())
		return it->second;

	return NULL;
}

void CMotionDirHelper::SetFileExt(CString ext)
{
	m_strFileExt = ext;
	m_strFileExt.MakeUpper();
}

CString	CMotionDirHelper::GetFileExtFromName(CString path)
{
	int dot = path.ReverseFind('.');
	int sep = path.ReverseFind('\\');
	if(dot > sep)
	{
		return path.Right(path.GetLength() - dot);
	}
	return NULL;
}