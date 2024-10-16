
#include "stdafx.h"
#include "ExtPropList.h"
#include "NotifyProperty.h"

// CExtPropList

IMPLEMENT_DYNAMIC(CExtPropList, CBCGPPropList)


CExtPropList::~CExtPropList()
{
}


BEGIN_MESSAGE_MAP(CExtPropList, CBCGPPropList)
END_MESSAGE_MAP()


void CExtPropList::OnChangeSelection (CBCGPProp* pNewSel, CBCGPProp* pOldSel)
{
	if((GetAsyncKeyState(VK_SHIFT)&0xff00)!=0)
	{
		InsertRelateProp(pOldSel);
	}
	else
	{
		ClearProp();
	}
	InsertRelateProp(pNewSel);
	Invalidate(TRUE);
}

void CExtPropList::InsertRelateProp(CBCGPProp* pProp)
{
	if(pProp == NULL)
		return;

	if(!pProp->IsKindOf(RUNTIME_CLASS(CNotifiedProp)))
		return;

	for(int i = 0; i < (int)m_relateProp.size(); i++)
	{
		if(m_relateProp[i] == pProp)
			return;
	}

	CNotifiedProp* pNotifiedProp = (CNotifiedProp*)pProp;
	pNotifiedProp->m_bRelated = true;
	TRACE("A Prop is Related\n");
	m_relateProp.push_back(pProp);
}

void CExtPropList::ClearProp()
{
	for(int i = 0; i < (int)m_relateProp.size(); i++)
	{
		((CNotifiedProp*)m_relateProp[i])->m_bRelated = false;
		TRACE("A Prop is Unrelated\n");
	}
	m_relateProp.clear();

}

void CExtPropList::RemoveAllExt()
{
	ClearProp();
	CBCGPPropList::RemoveAll();
}


//int CExtPropList::OnDrawProperty (CDC* pDC, CBCGPProp* pProp)
//{
//}

// CExtPropList message handlers

