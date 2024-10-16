
#include "stdafx.h"
#include "ItemBrowserBar.h"
#include "ObjectTypeHelper_Avatar.h"
#include "DBManager.h"
#include "ItemDropTarget.h"

#include "AvatarManager.h"
#include "AvatarTypes.h"
#include "OgreResDB.h"

#define IVMSG_READPIECE (WM_USER + 5000)

using namespace Ogre;

IMPLEMENT_DYNAMIC(CItemViewerBar, CBCGPDockingControlBar)
CItemViewerBar::CItemViewerBar() : m_bReading(false)
{
}

CItemViewerBar::~CItemViewerBar()
{
}


BEGIN_MESSAGE_MAP(CItemViewerBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_BTN_ITM_ITEMTYPE, OnItemType)
	ON_COMMAND(ID_BTN_ITM_ITEMPOS, OnItemPos)
	ON_COMMAND(ID_BTN_ITM_QUALITY, OnQuality)

	ON_COMMAND(ID_BTN_ITMS_TEXT, OnBtnText)
	ON_COMMAND(ID_BTN_ITMS_SEARCH, OnBtnSearch)

	ON_COMMAND(ID_BTN_ITM_NEWITEM, OnBtnNewItem)

	ON_COMMAND(ID_BTN_ITM_EDITITEM, OnBtnEditItem)
	ON_UPDATE_COMMAND_UI(ID_BTN_ITM_EDITITEM, OnUpdateBtnEditItem)

	ON_COMMAND(ID_BTN_ITM_DELITEM, OnBtnDeleteItem)
	ON_UPDATE_COMMAND_UI(ID_BTN_ITM_DELITEM, OnUpdateBtnDeleteItem)


	ON_CBN_SELCHANGE(ID_BTN_ITM_ITEMTYPE, OnItemType)
	ON_CBN_SELCHANGE(ID_BTN_ITM_ITEMPOS, OnItemPos)
	ON_CBN_SELCHANGE(ID_BTN_ITM_QUALITY, OnQuality)

	ON_NOTIFY(LVN_BEGINDRAG, ID_ITEMLIST, OnBeginDrag)
	ON_NOTIFY(LVN_ITEMACTIVATE, ID_ITEMLIST, OnItemActive)

	ON_MESSAGE(IVMSG_READPIECE, OnReadPiece)
END_MESSAGE_MAP()



// CItemViewerBar message handlers


int CItemViewerBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 工具条
	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_ITEMBROWSER))
	{
		return -1;
	}
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	//std::vector<char*> arr;
	//CAvatarCvtHelper cvt;

	//vt.EnumItemType(arr);
	CBCGPToolbarComboBoxButton btnType(ID_BTN_ITM_ITEMTYPE, 0, CBS_DROPDOWNLIST, 50);	//工具条上组合框按钮(类型)
	//btnType.AddItem(_T("全部"));
	//for(int i = 0; i < (int)arr.size(); i++)
	//	btnType.AddItem(arr[i]);

	//cvt.EnumSlot(arr);
	CBCGPToolbarComboBoxButton btnSlot(ID_BTN_ITM_ITEMPOS, 0, CBS_DROPDOWNLIST, 50);	//工具条上组合框按钮(位置)
	//btnSlot.AddItem(_T("全部"));
	//for(int i = 0; i < (int)arr.size(); i++)
	//	btnSlot.AddItem(arr[i]);

	//cvt.EnumQuality(arr);
	CBCGPToolbarComboBoxButton btnQuality(ID_BTN_ITM_QUALITY, 0, CBS_DROPDOWNLIST, 50);	//工具条上组合框按钮(品质)
	//btnQuality.AddItem(_T("全部"));
	//for(int i = 0; i < (int)arr.size(); i++)
	//	btnQuality.AddItem(arr[i]);

	//btnType.SelectItem(1);
	//btnSlot.SelectItem(1);
	//btnQuality.SelectItem(1);

	m_wndToolBar.ReplaceButton(ID_BTN_ITM_ITEMTYPE, btnType);
	m_wndToolBar.ReplaceButton(ID_BTN_ITM_ITEMPOS, btnSlot);
	m_wndToolBar.ReplaceButton(ID_BTN_ITM_QUALITY, btnQuality);

	//查找工具条
	if(!m_wndSearchBar.Create(this) 
		|| !m_wndSearchBar.LoadToolBar(IDR_TOOLBAR_ITEMSEARCH))
	{
		return -1;
	}
	//m_wndSearchBar.SetRouteCommandsViaFrame(FALSE);

	CBCGPToolbarEditBoxButton	editBtn(ID_BTN_ITMS_TEXT, 0, ES_AUTOHSCROLL, 0);
	m_wndSearchBar.ReplaceButton(ID_BTN_ITMS_TEXT, editBtn);


	// 创建列表框
	m_wndList.CreateEx(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES ,  WS_VISIBLE | LVS_REPORT, 
		CRect(0, 0, 100, 100), this, ID_ITEMLIST);
	m_wndList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 40, -1);
	m_wndList.InsertColumn(1, _T("名称"), LVCFMT_LEFT, 120, -1);
	m_wndList.InsertColumn(2, _T("类型"), LVCFMT_LEFT, 40, -1);
	m_wndList.InsertColumn(3, _T("质量"), LVCFMT_LEFT, 40, -1);
	//m_wndList.InsertColumn(4, _T("装备位置"), LVCFMT_LEFT, 60, -1);

	m_wndList.SendMessage (LVM_SETEXTENDEDLISTVIEWSTYLE, 0,LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_wndList.ShowWindow(SW_SHOW);

	//ReloadItems();
	return 0;
}

void CItemViewerBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		rect.right - rect.left, 24, SWP_NOZORDER);

	m_wndSearchBar.SetWindowPos(NULL, 0, 24,
		rect.right - rect.left, 24, SWP_NOZORDER);

	m_wndList.SetWindowPos(NULL, 0, 48,
		rect.right - rect.left, rect.bottom - rect.top - 48,
		SWP_NOZORDER);

}

void CItemViewerBar::OnBeginDrag(NMHDR* pHdr, LRESULT* pr)
{
	NMLISTVIEW *pNm = (NMLISTVIEW*)pHdr;
	CItemDragSource* pItemSrc = new CItemDragSource();

	LVITEM item;
	item.iItem = pNm->iItem;
	item.mask = LVIF_PARAM;
	m_wndList.GetItem(&item);

	//CString str;
	//str.Format("开始拖动物品 %d", (int)item.lParam);
	//SetWindowText(str.GetBuffer());
	CCvtHelper cvt;
	int itemid = 0;
	CString strID = m_wndList.GetItemText(pNm->iItem, 0);
	itemid = cvt.StringToInt(strID.GetBuffer());

	//pItemSrc->Drag((int)item.lParam);
	pItemSrc->Drag(itemid);	
	delete pItemSrc;
}

void CItemViewerBar::OnItemActive(NMHDR* pHdr, LRESULT* pr)
{
	//NMLISTVIEW *pNm = (NMLISTVIEW*)pHdr;
	//int index = pNm->iItem;
	//if(index >= 0)
	//{
	//	Item_T* pItem = (Item_T*)m_wndList.GetItemData(index);
	//	PostMainFrameMsg(EEM_EDITITEM, 0, (LPARAM)pItem->ItemID);
	//}

}

LRESULT CItemViewerBar::OnReadPiece(WPARAM wParam, LPARAM lParam)
{
	size_t start = (size_t)lParam;
	LoadItemPiece(start);
	return 0;
}

void CItemViewerBar::OnBtnNewItem()
{/*
	Item_T* pItem = NULL;
	size_t maxid = 0;

	AvatarManager *pmgr = AvatarManager::getSingletonPtr();
	int num = pmgr->m_pItemTable->FindRecord(NULL, 0, NULL);
	pItem = new Item_T[num];
	pmgr->m_pItemTable->FindRecord(pItem, num, NULL);

	for(int i = 0; i < num; i++)
	{
		if(pItem[i].id > (int)maxid)
			maxid = pItem[i].id;
	}
	delete[] pItem;

	PostMainFrameMsg(EEM_EDITITEM, 0, (LPARAM)(maxid + 1));
	*/
}

void CItemViewerBar::OnBtnEditItem()
{
	int index = m_wndList.GetSelectionMark();
	if(index >= 0)
	{
		LVITEM item;
		item.iItem = index;
		item.mask = LVIF_PARAM;
		m_wndList.GetItem(&item);
		PostMainFrameMsg(EEM_EDITITEM, 0, (LPARAM)item.lParam);
	}
}

void CItemViewerBar::OnUpdateBtnEditItem(CCmdUI* pUI)
{
	int index = m_wndList.GetSelectionMark();
	if(index >= 0)
		pUI->Enable(TRUE);
	else
		pUI->Enable(FALSE);

}

void CItemViewerBar::OnUpdateBtnDeleteItem(CCmdUI* pUI)
{
	int index = m_wndList.GetSelectionMark();
	if(index >= 0)
		pUI->Enable(TRUE);
	else
		pUI->Enable(FALSE);
}

void CItemViewerBar::OnBtnDeleteItem()
{
	//int index = m_wndList.GetSelectionMark();
	//if(index >= 0)
	//{
	//	Item_T* pItem = (Item_T*)m_wndList.GetItemData(index);
	//	int iDispID = pItem->model;
	//	size_t i;

	//	for(i = 0; i < CAvatarShow::ms_pItemDB->GetNumRecord(); i++)
	//	{
	//		Ogre::DBRecord rec = CAvatarShow::ms_pItemDB->GetIthRecord(i);
	//		Item_T* pCurItem = (Item_T*) rec.precord;

	//		if(pCurItem->ItemID == pItem->ItemID)
	//			break;
	//	}

	//	if(i < CAvatarShow::ms_pItemDB->GetNumRecord())
	//	{
	//		CAvatarShow::ms_pItemDB->RemoveIthRecord(i);
	//		for(i = 0; i < CAvatarShow::ms_pItemDisDB->GetNumRecord(); i++)
	//		{
	//			Ogre::DBRecord rec = CAvatarShow::ms_pItemDisDB->GetIthRecord(i);
	//			if(rec.GetUInt(ItemDisplayDB::ItemDisplayID) == iDispID)
	//				break;
	//		}
	//		if(i < CAvatarShow::ms_pItemDisDB->GetNumRecord())
	//			CAvatarShow::ms_pItemDisDB->RemoveIthRecord(i);
	//	}
	//}
	//ReloadItems();
}

void CItemViewerBar::OnItemType()
{
	ReloadItems();
}

void CItemViewerBar::OnItemPos()
{
	ReloadItems();
}

void CItemViewerBar::OnQuality()
{
	ReloadItems();
}

void CItemViewerBar::OnBtnText()
{
	//MessageBox("asdfasdf");
	ReloadItems();
}

void CItemViewerBar::OnBtnSearch()
{
	ReloadItems();
}

void CItemViewerBar::ReloadItems()
{
	m_wndList.DeleteAllItems();
	CAvatarCvtHelper cvt;
	std::vector<char*> arr;

	CBCGPToolbarComboBoxButton* pBtnType = (CBCGPToolbarComboBoxButton*) m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ITM_ITEMTYPE));
	if(pBtnType->GetCount() == 0)
	{
		cvt.EnumItemType(arr);
		pBtnType->RemoveAllItems();
		pBtnType->AddItem(_T("全部"));
		for(int i = 0; i < (int)arr.size(); i++)
			pBtnType->AddItem(cvt.Utf8ToGb( arr[i]));
		pBtnType->SelectItem(1);
	}

	CBCGPToolbarComboBoxButton* pBtnQuality = (CBCGPToolbarComboBoxButton*) m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ITM_QUALITY));
	if(pBtnQuality->GetCount() == 0)
	{
		cvt.EnumQuality(arr);
		pBtnQuality->AddItem(_T("全部"));
		for(int i = 0; i < (int)arr.size(); i++)
			pBtnQuality->AddItem(arr[i]);
		pBtnType->SelectItem(1);
	}
	LoadItemFromDB();
}

void CItemViewerBar::LoadItemPiece(size_t start)
{
	//CAvatarCvtHelper cvt;
	//size_t i = 0;
	//size_t dbpos = 0;
	//int listpos = m_wndList.GetItemCount();
	//ItemDB* pItemDB = CAvatarShow::ms_pItemDB;//CAvatar::m_pItemDB;

	//
	//
	//SetReadingState(true);

	//for(i = 0; i < 100; i++)
	//{
	//	dbpos = start + i;
	//	if(dbpos >= pItemDB->GetNumRecord())
	//		break;

	//	Ogre::DBRecord rec = pItemDB->GetIthRecord(dbpos);
	//	Item_T* pItem = (Item_T*) rec.precord;

	//	if(pItem == NULL)
	//		break;;

	//	if( FitFilter_ItemType(pItem) &&
	//		//FitFilter_ItemSlot(pItem) &&
	//		FitFilter_Quality(pItem) &&
	//		FitFilter_Name(pItem))
	//	{
	//		m_wndList.InsertItem((int)listpos, cvt.IntToString(pItem->ItemID), 0);
	//		m_wndList.SetItem((int)listpos, 1, LVIF_TEXT, rec.GetString(7), 0, 0, 0, 0);
	//		m_wndList.SetItem((int)listpos, 2, LVIF_TEXT, cvt.ItemTypeToString((ItemTypes)pItem->ItemType), 0, 0, 0, 0);
	//		m_wndList.SetItem((int)listpos, 3, LVIF_TEXT, cvt.ItemQualityToString(pItem->quality),0, 0, 0, 0);
	//		m_wndList.SetItemData(listpos, (DWORD_PTR)pItem);
	//		//SetItemParam(listpos, pItem);
	//		listpos++;
	//	}
	//}

	//if(i >= 100)
	//	PostMessage(IVMSG_READPIECE, 0, start + i);
	//else
	//	SetReadingState(false);
}

void CItemViewerBar::LoadItemFromDB()
{
	CAvatarCvtHelper cvt;
	if(CDataBase::GetInstance() == NULL) return;

	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("ItemInfo");
	if(pTable == NULL) return;

	int iCount = pTable->FindRecord(NULL, 0, NULL);
	ITEMDEF* pItemBuffer = new ITEMDEF[iCount];
	pTable->FindRecord(pItemBuffer, iCount, NULL);

	int index = 0;
	for (int i = 0; i < iCount; i++) 
	{
		ITEMDEF* pItem =& pItemBuffer[i];

		if( FitFilter_ItemType(pItem) &&
			FitFilter_Quality(pItem) &&
			FitFilter_Name(pItem))
		{
			m_wndList.InsertItem(index, cvt.IntToString(pItem->ItemID), 0);
			m_wndList.SetItem(index, 1, LVIF_TEXT, pItem->Name, 0, 0, 0, 0);
			m_wndList.SetItem(index, 2, LVIF_TEXT, cvt.ItemTypeToString(pItem->ItemType), 0, 0, 0, 0);
			m_wndList.SetItem(index, 3, LVIF_TEXT, cvt.ItemQualityToString(pItem->BuyPrice),0, 0, 0, 0);/*item quality*/
			LVITEM item;
			item.iItem = index;
			item.mask = LVIF_PARAM;
			item.lParam = pItem->ItemID;
			m_wndList.SetItem(&item);
			//SetItemParam(listpos, pItem);
			index++;
		}
	}
	delete pItemBuffer;
}

void CItemViewerBar::SetReadingState(bool state)
{
	if(m_bReading == state)
		return;
	m_bReading = state;

	if(m_bReading == true)
	{
		m_wndToolBar.EnableWindow(FALSE);
		m_wndSearchBar.EnableWindow(FALSE);
	}
	else
	{
		m_wndToolBar.EnableWindow(TRUE);
		m_wndSearchBar.EnableWindow(TRUE);
	}
}

void CItemViewerBar::SetItemParam(int index, long param)
{
	m_wndList.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, param, 0);
}

long CItemViewerBar::GetItemParam(int index)
{
	LVITEM item;
	item.mask = LVIF_PARAM;
	item.iItem = index;
	m_wndList.GetItem(&item);
	return (long)item.lParam;
}

bool CItemViewerBar::FitFilter_ItemType(ITEMDEF* pItem)
{
	CAvatarCvtHelper cvt;
	CBCGPToolbarComboBoxButton* pCombo = (CBCGPToolbarComboBoxButton* )m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ITM_ITEMTYPE));

	int iSel = pCombo->GetCurSel();

	if(iSel <= 0) // -1为没选，0为选全部
		return true;

	LPCTSTR strSel = pCombo->GetItem(iSel);

	if(cvt.StringToItemType((LPTSTR)strSel) == pItem->ItemType)
		return true;
	return false;
}

bool CItemViewerBar::FitFilter_ItemSlot(ITEMDEF* pItem)
{
	CAvatarCvtHelper cvt;
	CBCGPToolbarComboBoxButton* pCombo = 
		(CBCGPToolbarComboBoxButton* )m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ITM_ITEMPOS));

	int iSel = pCombo->GetCurSel();

	if(iSel <= 0) // -1为没选，0为选全部
		return true;

	LPCTSTR strSel = pCombo->GetItem(iSel);

	if(cvt.TypeFitSlot(pItem->ItemType, cvt.StringToSlot((LPTSTR)strSel)) == true)
		return true;
	return false;
}

bool CItemViewerBar::FitFilter_Quality(ITEMDEF* pItem)
{
	CAvatarCvtHelper cvt;
	CBCGPToolbarComboBoxButton* pCombo = 
		(CBCGPToolbarComboBoxButton* )m_wndToolBar.GetButton(m_wndToolBar.CommandToIndex(ID_BTN_ITM_QUALITY));

	int iSel = pCombo->GetCurSel();

	if(iSel <= 0) // -1为没选，0为选全部
		return true;

	LPCTSTR strSel = pCombo->GetItem(iSel);

	if(cvt.StringToItemQuality((LPTSTR)strSel) == pItem->BuyPrice)
		return true;
	return false;

}

bool CItemViewerBar::FitFilter_Name(ITEMDEF* pItem)
{

	const char* name = NULL;
	if(pItem->Name)
		name = pItem->Name;
	CBCGPToolbarEditBoxButton* pEditBtn = 
		(CBCGPToolbarEditBoxButton* )m_wndSearchBar.GetButton(m_wndSearchBar.CommandToIndex(ID_BTN_ITMS_TEXT));
	char subStr[128] = {0};
	pEditBtn->GetEditBox()->GetWindowText(subStr, 128);

	int id = -1;
	sscanf(subStr, "%d", &id);

	if(::strstr(name, subStr) != NULL || id == pItem->ItemID)
		return true;
	return false;
}

BOOL CItemViewerBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	static int i = 0;
	if(nID == ID_BTN_ITMS_TEXT)
	{
		if(nCode != 0 && nCode != -1)
		{
			TRACE("cmd ID_BTN_ITMS_TEXT: %d, %d\n", i++, nCode);
		}
	}
	return CBCGPDockingControlBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
