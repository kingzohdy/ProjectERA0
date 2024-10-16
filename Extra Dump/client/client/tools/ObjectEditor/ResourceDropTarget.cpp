
#include "stdafx.h"
#include "ResourceDropTarget.h"

BOOL ChunkDragSource::OnRenderFileData(LPFORMATETC lpFormatEtc,CFile* pFile)
{ 
	if(lpFormatEtc->cfFormat == CF_TEXT) 
	{
		long lLen = strPath.GetLength() + 1;
		pFile->Write(&lLen, sizeof(lLen));
		pFile->Write(strPath.GetBuffer(), lLen);

		lLen = strChunk.GetLength() + 1;
		pFile->Write(&lLen, sizeof(lLen));
		pFile->Write(strChunk.GetBuffer(), lLen);


		return TRUE;
	}
	COleDataSource::OnRenderFileData(lpFormatEtc,pFile); 
	return FALSE;
}


IMPLEMENT_DYNAMIC(CResDropTarget, COleDropTarget)
CResDropTarget::CResDropTarget()
{
	m_hNotifyTo = NULL;
}

CResDropTarget::~CResDropTarget()
{
}


BEGIN_MESSAGE_MAP(CResDropTarget, COleDropTarget)
END_MESSAGE_MAP()



DROPEFFECT CResDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_COPY;
}

void CResDropTarget::OnDragLeave(CWnd* pWnd)
{
	pWnd->Invalidate();
	//COleDropTarget::OnDragLeave(pWnd);
}

DROPEFFECT CResDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	//CRuntimeClass* pRuntimeClass = pWnd->GetRuntimeClass();
	//CString className = pRuntimeClass->m_lpszClassName;
	//if(className == "CBCGPPropList")
	//{
	//	CBCGPPropList* pProp = (CBCGPPropList*)pWnd;
	//	pProp->Invalidate();
	//	CDC* pDC = pProp->GetDC();
	//	DrawPropList(pProp, point, pDC);
	//	pDC->DeleteDC();
	//	
	//}
	return  DROPEFFECT_COPY;
}



BOOL CResDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{

	if(pDataObject->GetFileData(CF_TEXT) == NULL)
		return DROPEFFECT_NONE;


	long lLen;
	char buf[256];
	CFile* pFile = pDataObject->GetFileData(CF_TEXT);
	pFile->Read(&lLen, sizeof(lLen));
	pFile->Read(buf, lLen);
	CString filePath = buf;

	pFile->Read(&lLen, sizeof(lLen));
	pFile->Read(buf, lLen);
	pFile->Close();
	CString chunkName = buf;


	DROPNOTIFY dn;
	dn.hWnd = m_hWnd;
	dn.strPath = filePath.GetBuffer();
	dn.strChunk = chunkName.GetBuffer();
	dn.dropPos = point;
	::SendMessage(m_hNotifyTo, EEM_DROPRES, 0, (LPARAM)&dn);

	pWnd->Invalidate();
	return  DROPEFFECT_COPY;
}

//------------------------------------------------------------------------------------------------------------------------------

CResDropPropListTarget::CResDropPropListTarget(void)
{
}

CResDropPropListTarget::~CResDropPropListTarget(void)
{
}

DROPEFFECT CResDropPropListTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
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

CBCGPProp* CResDropPropListTarget::GetHoveringProp(CBCGPPropList* pList, CPoint point)
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

CBCGPProp* CResDropPropListTarget::GetHoveringSubProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point)
{
	if(pProp->IsGroup() == FALSE)
	{
		CRect rect = pProp->GetRect();
		if(rect.PtInRect(point) == TRUE && m_pLastProp != pProp)
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

void CResDropPropListTarget::DrawPropList(CBCGPPropList* pList, CPoint point, CDC* pDC)
{
	int count = pList->GetPropertyCount();
	for(int i = 0; i < count; i++)
	{
		DrawProp(pList, pList->GetProperty(i), point, pDC);
	}
}

void CResDropPropListTarget::DrawProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point, CDC* pDC)
{
	if(pProp->IsGroup() == FALSE)
	{
		CRect rect = pProp->GetRect();

		//pList->ScreenToClient(rect);
		if(rect.PtInRect(point) == TRUE && m_pLastProp != pProp)
		{

			CBrush brush(RGB(255, 0, 0));
			pDC->FillRect(rect, &brush);
			m_pLastProp = pProp;
		}
	}
	else
	{
		int count = pProp->GetSubItemsCount();
		for(int i = 0; i < count; i++)
		{
			DrawProp(pList, pProp->GetSubItem(i), point, pDC);
		}
	}
}