
#include "stdafx.h"
#include "MotionBrowserHelper.h"
#include <queue>
#include <map>

BOOL CMotionBrowserHelperSource::OnRenderFileData(LPFORMATETC lpFormatEtc,CFile* pFile)
{ 
	if(lpFormatEtc->cfFormat == CF_BITMAP) 
	{
		pFile->Write(&m_type, sizeof(CATTYPE));
		pFile->Write(&m_catId, sizeof(int));

		long lLen = m_strMotionName.GetLength() + 1;
		pFile->Write(&lLen, sizeof(lLen));
		pFile->Write(m_strMotionName.GetBuffer(), lLen);


		return TRUE;
	}
	COleDataSource::OnRenderFileData(lpFormatEtc,pFile); 
	return FALSE;
}


CMotionBrowserHelper::CMotionBrowserHelper(void)
{
}

CMotionBrowserHelper::~CMotionBrowserHelper(void)
{
	for (int i = 0; i < (int) m_Motions.size(); i++) 
	{
		delete m_Motions[i];
	}
	m_Motions.clear();
}


void CMotionBrowserHelper::LoadCatgory(CString strCatPath)
{
	m_catgoryInfo.InitCatgoryDb(strCatPath);
	UpdateCatgoryTree();
}

void CMotionBrowserHelper::AddMotion(CString strMotion, CString strMotionFile)
{
	int cat = m_catgoryInfo.GetMotionCatId(strMotion);
	HTREEITEM hParent = GetCatgoryHandleById(cat);

	MotionFileInfo* pMotionFileInfo = new MotionFileInfo();
	pMotionFileInfo->strChunkName = strMotionFile;
	pMotionFileInfo->strName = strMotion;
	if(m_pTreeCtrl->GetSafeHwnd())
		pMotionFileInfo->hItem = m_pTreeCtrl->InsertItem(strMotion.GetBuffer(), m_iImageMotion, m_iImageMotion, hParent);

	m_Motions.push_back(pMotionFileInfo);
}

void CMotionBrowserHelper::ClearMotion()
{
	for(int i = 0; i < (int) m_Motions.size(); i++)
	{
		if(m_pTreeCtrl->GetSafeHwnd())
		{
			m_pTreeCtrl->DeleteItem(m_Motions[i]->hItem);
		}
		delete m_Motions[i];
	}
	m_Motions.clear();
}

CString CMotionBrowserHelper::GetMotionNameByHandle(HTREEITEM hItem)
{
	for(int i = 0; i < (int) m_Motions.size(); i++)
	{
		if(m_Motions[i]->hItem == hItem)
			return m_Motions[i]->strName;
	}
	return "";
}

HTREEITEM CMotionBrowserHelper::GetMotionHandleByName(CString strName)
{
	strName.MakeUpper();
	for(int i = 0; i < (int) m_Motions.size(); i++)
	{
		CString strExist = m_Motions[i]->strName;
		strExist.MakeUpper();
		if(strExist == strName)
			return m_Motions[i]->hItem;
	}
	return NULL;
}

void CMotionBrowserHelper::UpdateTree()
{
	//吧Motion节点卸下，记录下选择
	HTREEITEM hSel = m_pTreeCtrl->GetSelectedItem();
	int iSelMotion = -1;
	for(int i = 0; i < (int) m_Motions.size(); i++)
	{
		if(hSel == m_Motions[i]->hItem)
		{
			iSelMotion = i;
		}
		m_pTreeCtrl->DeleteItem(m_Motions[i]->hItem);
	}

	UpdateCatgoryTree();

	//吧Motion重新挂上去
	for(int i = 0; i < (int) m_Motions.size(); i++)
	{
		HTREEITEM hParent = GetCatgoryHandleById(m_catgoryInfo.GetMotionCatId(m_Motions[i]->strName));
		m_Motions[i]->hItem = m_pTreeCtrl->InsertItem(m_Motions[i]->strName, m_iImageMotion, m_iImageMotion, hParent);
	}
	if(iSelMotion >= 0)
	{
		m_pTreeCtrl->SelectItem(m_Motions[iSelMotion]->hItem);
	}
}

void CMotionBrowserHelper::UpdateCatgoryTree()
{
	std::map<int, UINT> itemState;

	if(m_pTreeCtrl == NULL)
		return;

	//吧Catgory清空
	for(int i = 0; i < (int) m_Catgory.size(); i++)
	{
		//UINT state = m_pTreeCtrl->GetItemState(m_Catgory[i].hItem, TVIS_EXPANDED);
		//itemState[m_Catgory[i].iCatId] = state;
		m_pTreeCtrl->DeleteItem(m_Catgory[i].hItem);
	}
	m_Catgory.clear();


	//刷新Catgory结构
	std::queue<MotionCatgory*> cat;
	std::vector<MotionCatgory*> children;
	m_catgoryInfo.GetChildCatgory(0, children);
	for(int i = 0; i < (int)children.size(); i++)
	{
		cat.push(children[i]);
	}

	do 
	{
		if(cat.size() == 0)
		{
			break;
		}
		MotionCatgory* pCurrMotion = cat.front();
		cat.pop();

		CatgoryDisplayInfo cd;
		cd.iCatId = pCurrMotion->iId;
		cd.hItem = m_pTreeCtrl->InsertItem(pCurrMotion->strCatgoryName.GetBuffer()
			, m_iImageCatgory, m_iImageCatgory
			, GetCatgoryHandleById(pCurrMotion->iParentId)
			, TVI_FIRST);
		m_Catgory.push_back(cd);

		children.clear();
		m_catgoryInfo.GetChildCatgory(pCurrMotion->iId, children);
		for(int i = 0; i < (int)children.size(); i++)
		{
			cat.push(children[i]);
		}

	} 
	while(cat.size() != 0);

	//for(int i = 0; i < (int) m_Catgory.size(); i++)
	//{
	//	UINT state = 0;
	//	std::map<int, UINT>::iterator it = itemState.find(m_Catgory[i].iCatId);
	//	if(it != itemState.end())
	//	{
	//		state = it->second;
	//	}
	//	
	//	m_pTreeCtrl->SetItemState(m_Catgory[i].hItem, state, TVIS_EXPANDED);
	//}

}

HTREEITEM CMotionBrowserHelper::GetCatgoryHandleById(int id)
{
	for(int i = 0; i < (int)m_Catgory.size(); i++)
	{
		if(m_Catgory[i].iCatId == id)
		{
			return m_Catgory[i].hItem;
		}
	}
	return m_hBascItem;
}

int CMotionBrowserHelper::GetCatgoryIdByHandle(HTREEITEM hItem)
{
	for(int i = 0; i < (int)m_Catgory.size(); i++)
	{
		if(m_Catgory[i].hItem == hItem)
		{
			return m_Catgory[i].iCatId;
		}
	}
	return 0;
}

int CMotionBrowserHelper::GetSibingCatIdByHandle(HTREEITEM hItem)
{
	for(int i = 0; i < (int)m_Catgory.size(); i++)
	{
		if(m_Catgory[i].hItem == hItem)
		{
			return m_Catgory[i].iCatId;
		}
	}

	for(int i = 0; i < (int) m_Motions.size(); i++)
	{
		if(m_Motions[i]->hItem == hItem)
		{
			return m_catgoryInfo.GetMotionCatId(m_Motions[i]->strName);
		}
	}

	return 0;
}



void CMotionBrowserHelper::DoItemDrag(HTREEITEM hItem)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	SetCursor(LoadCursor(NULL, IDC_SIZEALL));
	int iTargetCat = 0;
	CATTYPE type = CT_FOLDER;
	int iSrcCat = 0;
	CString strSrcMotion;

	for(int i = 0; i < (int)m_Catgory.size(); i++)
	{
		if(m_Catgory[i].hItem == hItem)
		{
			type = CT_FOLDER;
			iSrcCat = m_Catgory[i].iCatId;
			break;
		}
	}

	for(int i = 0; i < (int)m_Motions.size(); i++)
	{
		if(m_Motions[i]->hItem == hItem)
		{
			type = CT_ITEM;
			strSrcMotion = m_Motions[i]->strName;
			break;
		}
	}

	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			{
				pos.x = LOWORD(msg.lParam);
				pos.y = HIWORD(msg.lParam);
				UINT flag;
				HTREEITEM hOn = m_pTreeCtrl->HitTest(pos, &flag);
				if(hOn != NULL && (TVHT_ONITEM & flag))
				{
					iTargetCat = GetSibingCatIdByHandle(hOn);
					m_pTreeCtrl->SelectItem(hOn);
				}
			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	if(type == CT_FOLDER)
	{
		MotionCatgory* pCat = m_catgoryInfo.FindCatgory(iSrcCat);
		if(!m_catgoryInfo.IsChildOrSelf(iSrcCat, iTargetCat))
		{
			pCat->iParentId = iTargetCat;
			UpdateTree();

			m_pTreeCtrl->EnsureVisible(GetCatgoryHandleById(pCat->iId));
		}
	}
	else
	{
		MotionCatgoryStoragy* pMotionStor =	m_catgoryInfo.FindMotion(strSrcMotion);
		if(pMotionStor != NULL)
		{
			pMotionStor->iParentId = iTargetCat;
		}
		else
		{
			MotionCatgoryStoragy mcs;
			mcs.iParentId = iTargetCat;
			mcs.strMotionName = strSrcMotion;
			m_catgoryInfo.m_MotionStoragy.push_back(mcs);
		}
		UpdateTree();

		HTREEITEM hCat = GetCatgoryHandleById(iTargetCat);
		m_pTreeCtrl->EnsureVisible(hCat);
		m_pTreeCtrl->Expand(hCat, TVE_EXPAND);

	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}