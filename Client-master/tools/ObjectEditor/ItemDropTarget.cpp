
#include "stdafx.h"
#include "ItemDropTarget.h"

static UINT CF_ITEM = RegisterClipboardFormat(_T("EEF_ITEM"));
BOOL CItemDragSource::OnRenderFileData(LPFORMATETC lpFormatEtc,CFile* pFile)
{
	if(lpFormatEtc->cfFormat == CF_ITEM)
	{
		pFile->Write((void*) &m_iItemID, sizeof(int));
		return TRUE;
	}
	COleDataSource::OnRenderFileData(lpFormatEtc, pFile);
	return FALSE;
}


void CItemDragSource::Drag(int id)
{
	m_iItemID = id;
	DelayRenderData(CF_ITEM);
	DoDragDrop();
}
// CItemDropTarget

IMPLEMENT_DYNAMIC(CItemDropTarget, COleDropTarget)
CItemDropTarget::CItemDropTarget()
{
}

CItemDropTarget::~CItemDropTarget()
{
}


BEGIN_MESSAGE_MAP(CItemDropTarget, COleDropTarget)
END_MESSAGE_MAP()



DROPEFFECT CItemDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(pDataObject->GetFileData(CF_ITEM) != NULL)
		return DROPEFFECT_COPY;
	return DROPEFFECT_NONE;
}

void CItemDropTarget::OnDragLeave(CWnd* pWnd)
{
	pWnd->Invalidate();
	COleDropTarget::OnDragLeave(pWnd);
}

DROPEFFECT CItemDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(pDataObject->GetFileData(CF_ITEM) != NULL)
		return DROPEFFECT_COPY;
	return DROPEFFECT_NONE;
}



BOOL CItemDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{

	CFile* pFile = pDataObject->GetFileData(CF_ITEM);
	if(pFile == NULL)
		return DROPEFFECT_NONE;

	int id;

	pFile->Read((void*)&id, sizeof(int));
	DROPNOTIFY dn;
	dn.hWnd = m_hWnd;
	dn.itemId = id;
	dn.dropPos = point;
	::SendMessage(m_hNotifyTo, EEM_DROPITEM, 0, (LPARAM)&dn);

	pWnd->Invalidate();
	return  DROPEFFECT_COPY;
}




// ----------------------------CItemDropPropListTarget--------------------- [10/18/2007]

CItemDropPropListTarget::CItemDropPropListTarget(void) : m_pLastProp(NULL)
{
}

CItemDropPropListTarget::~CItemDropPropListTarget(void)
{
}

DROPEFFECT CItemDropPropListTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CRuntimeClass* pRuntimeClass = pWnd->GetRuntimeClass();
	CString className = pRuntimeClass->m_lpszClassName;
	if(className == "CBCGPPropList")
	{
		CBCGPPropList* pPropList = (CBCGPPropList*)pWnd;
		CBCGPProp* pProp = GetHoveringProp(pPropList, point);
		if(pProp != NULL  && pProp != m_pLastProp)
		{
			pPropList->SetCurSel(pProp, TRUE);
			m_pLastProp = pProp;
		}

	}
	return  DROPEFFECT_COPY;
}

CBCGPProp* CItemDropPropListTarget::GetHoveringProp(CBCGPPropList* pList, CPoint point)
{
	int count = pList->GetPropertyCount();
	CBCGPProp* pRet = NULL;
	for(int i = 0; i < count; i++)
	{	
		if((pRet = GetHoveringSubProp(pList, pList->GetProperty(i), point)) != NULL)
		{
			return pRet;
		}
	}
	return NULL;
}

CBCGPProp* CItemDropPropListTarget::GetHoveringSubProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point)
{
	if(pProp->IsGroup() == FALSE)
	{
		CRect rect = pProp->GetRect();
		if(rect.PtInRect(point) == TRUE)
		{
			return pProp;
		}
	}
	else
	{
		int count = pProp->GetSubItemsCount();
		CBCGPProp* pRet = NULL;
		for(int i = 0; i < count; i++)
		{	
			if((pRet = GetHoveringSubProp(pList, pProp->GetSubItem(i), point)) != NULL)
			{
				return pRet;
			}
		}
	}
	return NULL;
}