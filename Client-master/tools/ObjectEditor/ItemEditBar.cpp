
#include "stdafx.h"
#include "ItemEditBar.h"
#include "SplitterGridView.h"
#include "EffectManager.h"
#include "NotifyProperty.h"
#include "AvatarShowInfo.h"
#include "ItemDropTarget.h"
#include "ObjectTypeHelper_Avatar.h"
#include "NewItemDlg.h"
#include "DBManager.h"
#include "EffectUnitEditBar.h"

#include "GameResDBTypes.h"
#include "AvatarConfig.h"
#include "AvatarManager.h"
#include "OgreEntity.h"
#include "OgreResDB.h"
#include "OgreResourceManager.h"
#include "OgreModel.h"
#include "OgreTexture.h"

#include "AvatarTypes.h"

using namespace Ogre;
int FieldNameToId(CString& fieldName);

IMPLEMENT_DYNAMIC(CItemEditPane, CWnd)
CItemEditPane::CItemEditPane() : m_iLastCheck(-1), m_bUpdating(false), m_iCurrID(-1)
{
	m_pPropGeosetA = NULL;
	m_pPropGeosetB = NULL;
	m_pPropGeosetC = NULL;
	m_pPropGeosetD = NULL;
	m_pPropGeosetE = NULL;

	m_pPropModel = NULL;
	m_pPropModel2 = NULL;
	m_pPropModel3 = NULL;
	m_pPropSkin = NULL;
	m_pPropSkin2 = NULL;
	m_pPropTexA = NULL;
	m_pPropTexB = NULL;
	m_pPropTexC = NULL;
	m_pPropTexD = NULL;
	m_pPropTexE = NULL;
	m_pPropTexF = NULL;
	m_pPropTexG = NULL;
	m_pPropTexH = NULL;

	m_pPropVisuals = NULL;
}

CItemEditPane::~CItemEditPane()
{
}


BEGIN_MESSAGE_MAP(CItemEditPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, ID_ITEMPVLIST, OnListItemChange)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_ITEMPVGEOLIST, OnGeoListItemChange)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_ITEMLIST_2, OnEquipItemChange)
	ON_NOTIFY(NM_DBLCLK, ID_ITEMLIST_2, OnEquipItemActive)
	ON_NOTIFY(LVN_BEGINDRAG, ID_ITEMLIST_2, OnBeginItemDrag)

	ON_MESSAGE(EEM_DROPRES, OnDropRes)
	ON_MESSAGE(EEN_PROP_UPDATE_VALUE, OnUpdatePropValue)
	ON_MESSAGE(EEN_PROP_CLICK_BUTTON, OnClickPropBtn)

	ON_COMMAND(ID_BTN_ITM_CLEAREQUIP, OnClearEquip)
	ON_COMMAND(ID_BTN_ITM_ADDITEM, OnAddItem)
	ON_COMMAND(ID_BTN_ITM_DELETE, OnDelItem)
	ON_COMMAND(ID_BTN_ITM_OVERWRITE, OnOverwriteItem)
END_MESSAGE_MAP()



// CItemEditPane message handlers


int CItemEditPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndToolBar.Create(this);

	m_wndToolBar.LoadToolBar(IDR_TOOLBAR_ITEM);

	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CreateSplitter(m_wndSplitter, this, m_pGrid1, m_pGrid2);
	CreateSplitter(m_wndSplitter2, m_pGrid2, m_pGrid21, m_pGrid22);
	m_pGrid2->SetInnerCtrl(&m_wndSplitter2);

	m_wndSplitter.SetColumnInfo(0, 150, 10);
	m_wndSplitter2.SetColumnInfo(0, 180, 10);

	m_wndListType.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, CRect(0, 0, 100, 100), m_pGrid1, ID_ITEMPVLIST);
	m_pGrid1->SetInnerCtrl(&m_wndListType);
	m_wndListType.SetExtendedStyle(m_wndListType.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_wndListType.InsertColumn(0, "物品类型", LVCFMT_LEFT, 120);

	//m_wndListGeoset.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, CRect(0, 0, 100, 100), m_pGrid21, ID_ITEMPVGEOLIST);
	//m_pGrid21->SetInnerCtrl(&m_wndListGeoset);
	//m_wndListGeoset.SetExtendedStyle(m_wndListGeoset.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	//m_wndListGeoset.InsertColumn(0, "显示模型", LVCFMT_LEFT, 120);
	//m_wndListGeoset.InsertColumn(1, "", LVCFMT_LEFT, 60);

	m_wndListItem.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT, CRect(0, 0, 100, 100), m_pGrid21, ID_ITEMLIST_2);
	m_pGrid21->SetInnerCtrl(&m_wndListItem);
	m_wndListItem.SetExtendedStyle(m_wndListItem.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_wndListItem.InsertColumn(0, "ID", LVCFMT_LEFT, 30);
	m_wndListItem.InsertColumn(1, "装备", LVCFMT_LEFT, 120);

	m_wndPropListMod.Create(WS_VISIBLE, CRect(0, 0, 100, 100), m_pGrid22, ID_ITEMPVPROP);
	m_wndPropListMod.EnableHeaderCtrl(FALSE);


	m_pGrid22->SetInnerCtrl(&m_wndPropListMod);

	m_wndSplitter.ShowWindow(SW_SHOW);

	m_dropTarget.Register(&m_wndPropListMod);
	m_dropTarget.SetNotifyTarget(GetSafeHwnd());

	return 0;
}

void CItemEditPane::CreateSplitter(CNormalSplitterWnd& wndSplitter, CWnd* pParent, CSplitterGridView* &pGrid1, CSplitterGridView* &pGrid2)
{
	CCreateContext context1;
	context1.m_pNewViewClass = RUNTIME_CLASS(CSplitterGridView);
	context1.m_pCurrentDoc = NULL;
	context1.m_pNewDocTemplate = NULL;
	context1.m_pLastView = NULL;
	context1.m_pCurrentFrame = NULL;

	CCreateContext context2;
	context2.m_pNewViewClass = RUNTIME_CLASS(CSplitterGridView);
	context2.m_pCurrentDoc = NULL;
	context2.m_pNewDocTemplate = NULL;
	context2.m_pLastView = NULL;
	context2.m_pCurrentFrame = NULL;

	CRect rectClient;
	pParent->GetClientRect (rectClient);

	wndSplitter.CreateStatic(pParent, 1, 2);
	wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context1);
	wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSplitterGridView), CSize (rectClient.Width () / 2, 0),&context2);
	wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top, rectClient.right, rectClient.bottom));
	pGrid1 = (CSplitterGridView*)wndSplitter.GetPane(0, 0);
	pGrid2 = (CSplitterGridView*)wndSplitter.GetPane(0, 1);
}

void CItemEditPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CWnd::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter.SetWindowPos(NULL, 0, 24,
		frameRect.right - frameRect.left, frameRect.bottom - frameRect.top - 24, SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);
}

void CItemEditPane::OnListItemChange(NMHDR* pHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnm = (NMLISTVIEW*) pHDR;
	if(m_iLastCheck == pnm->iItem)
		return;

	if(m_wndListType.GetCheck(pnm->iItem) == TRUE)
	{
		for(int i = 0; i < m_wndListType.GetItemCount(); i++)
		{
			if(i != pnm->iItem)
				m_wndListType.SetCheck(i, FALSE);
		}
		m_iLastCheck = pnm->iItem;
		CreateProp();
		UpdateItemList();
		CAvatarShowInfo* pAva = CEffectMgr::GetInstance()->m_avatarShowInfo;
		if(pAva->IsEnable()) pAva->UpdateActor();

		m_iCurrID = -1;
	}

	*pResult = 0;
}

void CItemEditPane::OnGeoListItemChange(NMHDR* pHDR, LRESULT* pResult)
{
	*pResult = 0;
}

void CItemEditPane::OnEquipItemChange(NMHDR* pHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnm = (NMLISTVIEW*) pHDR;

	if(((pnm->uOldState & LVIS_SELECTED) == 0) && ((pnm->uNewState & LVIS_SELECTED) != 0))
	{
		CAvatarShowInfo* pAva = CEffectMgr::GetInstance()->m_avatarShowInfo;
		if(pAva->IsEnable())
			pAva->UpdateActor();

		int id = (int)pnm->lParam;
		LoadItemById(id);
		OnUpdatePropValue(0, 0);
	}
}

bool IsRenderableResource(const char *path)
{
	const char *pdot = strrchr(path, '.');
	if(pdot == NULL) return false;

	if(stricmp(pdot, ".omod") == 0) return true;
	else if(stricmp(pdot, ".ent") == 0) return true;
	else return false;
}

bool IsTextureResource(const char *path)
{
	const char *pdot = strrchr(path, '.');
	if(pdot == NULL) return false;

	if(stricmp(pdot, ".otex")==0 || stricmp(pdot, ".dds")==0) return true;
	else return false;
}

LRESULT CItemEditPane::OnDropRes(WPARAM wParam, LPARAM lParam)
{
	CResDropTarget::DROPNOTIFY* pdn = (CResDropTarget::DROPNOTIFY*)lParam;
	if(pdn->hWnd == m_wndPropListMod.GetSafeHwnd())
	{
		if(IsFieldEnabled("Model") && m_pPropModel->GetRect().PtInRect(pdn->dropPos) && IsRenderableResource(pdn->strChunk))
		{
			setModel(m_pPropModel, pdn->strChunk, "Model", 1);
		}
		else if(IsFieldEnabled("Model2") && m_pPropModel2->GetRect().PtInRect(pdn->dropPos) && IsRenderableResource(pdn->strChunk))
		{
			setModel(m_pPropModel2, pdn->strChunk, "Model2", 2);
		}
		else if(IsFieldEnabled("Model3") && m_pPropModel3->GetRect().PtInRect(pdn->dropPos) && IsRenderableResource(pdn->strChunk))
		{
			setModel(m_pPropModel3, pdn->strChunk, "Model3", 3);
		}

		else if(IsFieldEnabled("TexA") && m_pPropTexA->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexA, pdn->strChunk, "TexA");

		else if(IsFieldEnabled("TexB") && m_pPropTexB->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexB, pdn->strChunk, "TexB");

		else if(IsFieldEnabled("TexC") && m_pPropTexC->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexC, pdn->strChunk, "TexC");

		else if(IsFieldEnabled("TexD") && m_pPropTexD->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexD, pdn->strChunk, "TexD");

		else if(IsFieldEnabled("TexE") && m_pPropTexE->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexE, pdn->strChunk, "TexE");

		else if(IsFieldEnabled("TexF") && m_pPropTexF->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexF, pdn->strChunk, "TexF");

		else if(IsFieldEnabled("TexG") && m_pPropTexG->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexG, pdn->strChunk, "TexG");

		else if(IsFieldEnabled("TexH") && m_pPropTexH->GetRect().PtInRect(pdn->dropPos) && IsTextureResource(pdn->strChunk))
			SetTexture(m_pPropTexH, pdn->strChunk, "TexH");
	}
	return 0;
}

LRESULT CItemEditPane::OnUpdatePropValue(WPARAM wParam, LPARAM lParam)
{
	PROPNM* pnm = (PROPNM*)lParam;
	USES_CONVERSION;
	UpdateDispProp(PROP_TO_DATA);

	Ogre::Model* pSkinActor = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();

	pSkinActor->showSkins(false);
	SkinShowBuilder skinshow(pSkinActor);
	for(int i = 0; i < (int)m_ognSkins.size(); i++)
	{
		skinshow.showSkin(m_ognSkins[i].GetBuffer());
	}

	if(IsFieldEnabled("GeosetA"))
		skinshow.showSkin(m_strGeosetA.GetBuffer());
	if(IsFieldEnabled("GeosetB"))
		skinshow.showSkin(m_strGeosetB.GetBuffer());
	if(IsFieldEnabled("GeosetC"))
		skinshow.showSkin(m_strGeosetC.GetBuffer());
	if(IsFieldEnabled("GeosetD"))
		skinshow.showSkin(m_strGeosetD.GetBuffer());
	if(IsFieldEnabled("GeosetE"))
		skinshow.showSkin(m_strGeosetE.GetBuffer());

	if(IsFieldEnabled("Model"))
	{
		setModel(m_pPropModel, m_strModel.GetBuffer(), "Model", 1);
	}
	if(IsFieldEnabled("Model2"))
	{
		setModel(m_pPropModel2, m_strModel2.GetBuffer(), "Model2", 2);
	}
	if(IsFieldEnabled("Model3"))
	{
		setModel(m_pPropModel3, m_strModel3.GetBuffer(), "Model3", 3);
	}


	if(IsFieldEnabled("TexA"))
		SetTexture(m_pPropTexA, m_strTexA.GetBuffer(), "TexA");

	if(IsFieldEnabled("TexB"))
		SetTexture(m_pPropTexB, m_strTexB.GetBuffer(), "TexB");

	if(IsFieldEnabled("TexC"))
		SetTexture(m_pPropTexC, m_strTexC.GetBuffer(), "TexC");

	if(IsFieldEnabled("TexD"))
		SetTexture(m_pPropTexD, m_strTexD.GetBuffer(), "TexD");

	if(IsFieldEnabled("TexE"))
		SetTexture(m_pPropTexE, m_strTexE.GetBuffer(), "TexE");

	if(IsFieldEnabled("TexF"))
		SetTexture(m_pPropTexF, m_strTexF.GetBuffer(), "TexF");

	if(IsFieldEnabled("TexG"))
		SetTexture(m_pPropTexG, m_strTexG.GetBuffer(), "TexG");

	if(IsFieldEnabled("TexH"))
		SetTexture(m_pPropTexH, m_strTexH.GetBuffer(), "TexH");

	if(IsFieldEnabled("Visuals"))
	{
		Entity *pentity = CEffectMgr::GetInstance()->GetModelActor();
		pentity->stopMotion(AVATAR_VISUAL_CLASS);
		if(!m_strVisuals.IsEmpty()) pentity->playMotion(m_strVisuals.GetBuffer(), false, AVATAR_VISUAL_CLASS);
	}

	return 0;
}

void CItemEditPane::OnBeginItemDrag(NMHDR* pHdr, LRESULT* pResult)
{
	NMLISTVIEW *pNm = (NMLISTVIEW*)pHdr;
	int iItem = m_wndListItem.GetSelectionMark();
	LVITEM item;
	item.iItem = iItem;
	item.mask = LVIF_PARAM;
	m_wndListItem.GetItem(&item);

	CItemDragSource* pItemSrc = new CItemDragSource();
	pItemSrc->Drag((int)item.lParam );
	delete pItemSrc;
}

void CItemEditPane::OnEquipItemActive(NMHDR* pHDR, LRESULT* pResult)
{
	//int iItem = m_wndListItem.GetSelectionMark();
	//LVITEM item;
	//item.iItem = iItem;
	//item.mask = LVIF_PARAM;
	//m_wndListItem.GetItem(&item);



	//if(iItem < 0)
	//	return;

}

LRESULT CItemEditPane::OnClickPropBtn(WPARAM w, LPARAM l)
{
	PROPNM* pnm = (PROPNM*)l;
	CCvtHelper cvt;

	return 0;
}

void CItemEditPane::OnOverwriteItem()
{
	if(m_iCurrID <= 0)
	{
		OnAddItem();
		return;
	}

	m_wndPropListMod.EndEditItem(TRUE);
	UpdateDispProp(PROP_TO_DATA);
	//在覆盖物品时，正常情况下，物品名和质量会被忽略，而采用原来的物品名和质量
	WriteToDb(m_iCurrID, "Unknown Object", 1);
}

void CItemEditPane::OnAddItem()
{
	m_wndPropListMod.EndEditItem(TRUE);
	UpdateDispProp(PROP_TO_DATA);

	CAvatarCvtHelper cvt;
	CNewItemDlg dlg;
	dlg.m_iItemID = 1;
	if(IDCANCEL == dlg.DoModal())
		return;

	WriteToDb(dlg.m_iItemID, dlg.m_ItemName.GetBuffer(), 0);

	UpdateItemList();
}

void CItemEditPane::OnDelItem()
{
	m_wndPropListMod.EndEditItem(TRUE);
	UpdateDispProp(PROP_TO_DATA);


	ITEMDISPLAYINFO itemDisp;
	ZeroMemory(&itemDisp, sizeof(ITEMDISPLAYINFO));
	itemDisp.id = m_iCurrID;

	Ogre::ResTable* pDspTab = CDataBase::GetInstance()->GetTable("ItemDisplayInfo");
	if(!pDspTab)
	{
		MessageBox("数据库打开失败");
		return;
	}

	if(pDspTab->DeleteRecord(&itemDisp) < 0)
	{
		MessageBox("物品信息写入失败");
	}

	UpdateItemList();
}

void CItemEditPane::OnClearEquip()
{

	m_strGeosetA = "";
	m_strGeosetB = "";
	m_strGeosetC = "";
	m_strGeosetD = "";
	m_strGeosetE = "";

	m_strModel = "";
	m_strModel2 = "";
	m_strModel3 = "";
	m_strSkin = "";
	m_strSkin2 = "";
	m_strTexA = "";
	m_strTexB = "";
	m_strTexC = "";
	m_strTexD = "";
	m_strTexE = "";
	m_strTexF = "";
	m_strTexG = "";
	m_strTexH = "";

	m_strVisuals = "";

	UpdateDispProp(DATA_TO_PROP);

	OnUpdatePropValue(0, 0);
}

void CItemEditPane::WriteToDb(int itemId, const char* name, int quality)
{
	Ogre::AvatarConfig *pReader = &CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow->m_Config;
	int typeIndex = GetCheckedItem();
	if(typeIndex < 0)
		return;
	int typeId = pReader->m_itemTypes[typeIndex]->m_iTypeId;
	Ogre::CItemTypeInfo* pTypeInfo = pReader->m_itemTypes[typeIndex];


	UpdateDispProp(PROP_TO_DATA);
	ITEMDISPLAYINFO itemDisp;
	ZeroMemory(&itemDisp, sizeof(ITEMDISPLAYINFO));
	itemDisp.id = itemId;
	itemDisp.flags = typeId;

	if(IsFieldEnabled("GeosetA"))	strcpy(itemDisp.geoset_a, m_strGeosetA);
	if(IsFieldEnabled("GeosetB"))	strcpy(itemDisp.geoset_b, m_strGeosetB);
	if(IsFieldEnabled("GeosetC"))	strcpy(itemDisp.geoset_c, m_strGeosetC);
	if(IsFieldEnabled("GeosetD"))	strcpy(itemDisp.geoset_d, m_strGeosetD);
	if(IsFieldEnabled("GeosetE"))	strcpy(itemDisp.geoset_e, m_strGeosetE);

	if(IsFieldEnabled("Model"))	strcpy(itemDisp.model, m_strModel);
	if(IsFieldEnabled("Skin"))	strcpy(itemDisp.skin, m_strSkin);
	if(IsFieldEnabled("Model2"))	strcpy(itemDisp.model2, m_strModel2);
	if(IsFieldEnabled("Skin2"))	strcpy(itemDisp.skin2, m_strSkin2);
	if(IsFieldEnabled("Model3")) strcpy(itemDisp.model3, m_strModel3);
	if(IsFieldEnabled("TexA"))	strcpy(itemDisp.tex_a, m_strTexA);
	if(IsFieldEnabled("TexB"))	strcpy(itemDisp.tex_b, m_strTexB);
	if(IsFieldEnabled("TexC"))	strcpy(itemDisp.tex_c, m_strTexC);
	if(IsFieldEnabled("TexD"))	strcpy(itemDisp.tex_d, m_strTexD);
	if(IsFieldEnabled("TexE"))	strcpy(itemDisp.tex_e, m_strTexE);
	if(IsFieldEnabled("TexF"))	strcpy(itemDisp.tex_f, m_strTexF);
	if(IsFieldEnabled("TexG"))	strcpy(itemDisp.tex_g, m_strTexG);
	if(IsFieldEnabled("TexH"))	strcpy(itemDisp.tex_h, m_strTexH);

	if(IsFieldEnabled("Visuals")) strcpy(itemDisp.visuals, m_strVisuals);


	Ogre::ResTable* pDspTab = CDataBase::GetInstance()->GetTable("ItemDisplayInfo");
	if(!pDspTab)
	{
		MessageBox("数据库打开失败");
		return;
	}

	if(pDspTab->InsertRecord(&itemDisp) <= 0)
	{
		if(pDspTab->UpdateRecord(&itemDisp) < 0)
		{
			MessageBox("物品信息写入失败");
		}
	}
}

void CItemEditPane::UpdateItemList()
{
	CAvatarCvtHelper cvt;
	Ogre::ResTable* pTable = CDataBase::GetInstance()->GetTable("ItemDisplayInfo");
	assert(pTable && " pTable is NULL in LoadItemFromDB");
	if(pTable == NULL)
		return;

	Ogre::AvatarConfig* pReader = &CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow->m_Config;
	int typeIndex = GetCheckedItem();
	if(typeIndex < 0)
		return;
	int typeId = pReader->m_itemTypes[typeIndex]->m_iTypeId;

	int iCount = pTable->FindRecord(NULL, 0, NULL);
	ITEMDISPLAYINFO* pItemBuffer = new ITEMDISPLAYINFO[iCount];
	pTable->FindRecord(pItemBuffer, iCount, NULL);
	int iSelectedType = typeId;

	m_wndListItem.DeleteAllItems();
	ITEMDEF itemdef;
	Ogre::ResTable *pitemtable = CDataBase::GetInstance()->GetTable("ItemDef");
	int index = 0;
	for (int i=0; i<iCount; i++)
	{
		ITEMDISPLAYINFO *pItem = &pItemBuffer[i];
		if(pItem->flags == iSelectedType)
		{
			m_wndListItem.InsertItem(index, cvt.IntToString(pItem->id), 0);

			itemdef.ItemID = pItem->id;
			CString name = "";
			if(pitemtable && pitemtable->FindRecord(&itemdef, 1, &itemdef)>0)
			{
				name = itemdef.Name;
			}
			m_wndListItem.SetItemText(index, 1, name.GetBuffer());
			m_wndListItem.SetItem(index, 0, LVIF_PARAM, NULL, 0, 0, 0, (LPARAM)pItem->id);
			index++;
		}
	}
	delete pItemBuffer;
}

void CItemEditPane::LoadItemById(int id)
{
	CAvatarCvtHelper cvt;
	Ogre::ResTable* pDspTable = CDataBase::GetInstance()->GetTable("ItemDisplayInfo");
	if(pDspTable == NULL)
		return;

	Ogre::AvatarConfig* pReader = &CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow->m_Config;
	int iCheckedItem = GetCheckedItem();
	if(iCheckedItem < 0) return;

	CItemTypeInfo* pType =  pReader->m_itemTypes[iCheckedItem];

	ITEMDISPLAYINFO itemDsp;
	itemDsp.id = id;
	int iFound = pDspTable->FindRecord(&itemDsp, 1, &itemDsp);

	m_strGeosetA = itemDsp.geoset_a;
	m_strGeosetB = itemDsp.geoset_b;
	m_strGeosetC = itemDsp.geoset_c;
	m_strGeosetD = itemDsp.geoset_d;
	m_strGeosetE = itemDsp.geoset_e;

	m_strModel = itemDsp.model;
	m_strModel2 = itemDsp.model2;
	m_strModel3 = itemDsp.model3;
	m_strSkin = itemDsp.skin;
	m_strSkin2 = itemDsp.skin2;

	m_strTexA = itemDsp.tex_a;
	m_strTexB = itemDsp.tex_b;
	m_strTexC = itemDsp.tex_c;
	m_strTexD = itemDsp.tex_d;
	m_strTexE = itemDsp.tex_e;
	m_strTexF = itemDsp.tex_f;
	m_strTexG = itemDsp.tex_g;
	m_strTexH = itemDsp.tex_h;

	m_strVisuals = itemDsp.visuals;

	UpdateDispProp(DATA_TO_PROP);

	m_iCurrID = id;
}

void CItemEditPane::SetTexture(CBCGPProp* pProp, LPCTSTR strTex, LPCTSTR fieldName)
{
	Ogre::SceneNode* pNode = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(IS_KIND_OF(Model, pNode))
	{
		AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
		AvatarConfig* pReader = &pAvaShow->m_Config;
		int iItemIndex = GetCheckedItem();
		CString strFieldName = fieldName;
		int id = FieldNameToId(strFieldName);
		if(id < 0)
			return;

		CItemTypeInfo *piteminfo = pReader->m_itemTypes[iItemIndex];
		int tex = piteminfo->m_FieldEnable[id].iTexID ;

		Ogre::Model* pActorNode = (Ogre::Model*) pNode;

		if(strTex[0])
		{
			int *pattrib = CEffectMgr::GetInstance()->m_avatarShowInfo->m_curAttrib;
			CTextParser *parser = pAvaShow->CreateTextParser(pattrib);
			parser->AddParam("field", strTex);
			std::string path = parser->ParseString(piteminfo->GetFieldPath(id));

			Ogre::TextureData *ptexsrc = (Ogre::TextureData *)ResourceManager::getSingleton().blockLoad(path.c_str());
			pActorNode->setTextureByID(tex, ptexsrc);
			if(ptexsrc) ptexsrc->release();

			char buffer[256];
			strcpy(buffer, path.c_str());
			char *pdot = strrchr(buffer, '.');
			if(pdot) strcpy(pdot, "_s.dds");

			TextureData *pspectex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
			if(pspectex == NULL)
			{
				strcpy(buffer, "shaders\\illumspec.dds");
				pspectex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(buffer));
			}
			pActorNode->setTextureByID(tex+10, pspectex);
			pspectex->release();
		}
		else
		{
			pActorNode->setTextureByID(tex, (Texture *)NULL);
			pActorNode->setTextureByID(tex+10, (Texture *)NULL);
		}

		pProp->SetValue(strTex);
	}
}

int  CItemEditPane::CreateNewItemDisplayID()
{
	Ogre::ResTable* pItemDspTable = CDataBase::GetInstance()->GetTable("ItemDisplayInfo");
	if(pItemDspTable == NULL)
		return -1;
	int itemcount = pItemDspTable->FindRecord(NULL, 0, NULL);
	ITEMDISPLAYINFO* pItems = new ITEMDISPLAYINFO[itemcount];
	pItemDspTable->FindRecord(pItems, itemcount, NULL);

	int maxid = 0;
	for(int i = 0; i < itemcount; i++)
	{
		if((int)pItems[i].id > maxid)
			maxid = pItems[i].id;
	}
	maxid++;

	delete pItems;

	return maxid;
}

bool CItemEditPane::IsOrginSkin(CString strSkinName)
{
	for(int i = 0; i < (int)m_ognSkins.size(); i++)
	{
		if(m_ognSkins[i] == strSkinName)
			return true;
	}
	return false;
}


void CItemEditPane::UpdateList()
{
	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;
	CCvtHelper cvt;

	CEffectMgr::GetInstance()->GetModelActor()->unbindAll(101);
	CEffectMgr::GetInstance()->GetModelActor()->unbindAll(102);

	m_wndListType.DeleteAllItems();
	m_iLastCheck = -1;

	for(int i = 0; i < (int)pReader->m_itemTypes.size(); i++)
	{
		m_wndListType.InsertItem(LVIF_TEXT, i, cvt.Utf8ToGb(pReader->m_itemTypes[i]->m_strName.c_str()), 0, 0, 0, 0);
	}
}

void CItemEditPane::UpdateSkins()
{
	m_bUpdating = true;

	//CreateProp();
	if(m_pPropGeosetA != NULL)
		AddSkinList(m_pPropGeosetA);
	if(m_pPropGeosetB != NULL)
		AddSkinList(m_pPropGeosetB);
	if(m_pPropGeosetC != NULL)
		AddSkinList(m_pPropGeosetC);
	if(m_pPropGeosetD != NULL)
		AddSkinList(m_pPropGeosetD);
	if(m_pPropGeosetE != NULL)
		AddSkinList(m_pPropGeosetE);

	Ogre::SceneNode* pNode = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(pNode == NULL)
		return;

	if(IS_KIND_OF(Model, pNode))
	{
		Ogre::Model* pActorNode = (Ogre::Model*)pNode;
		m_ognSkins.clear();
		for(size_t i = 0; i<pActorNode->getNumSkin(); i++)
		{
			MeshInstance *pmesh = pActorNode->getIthSkin(i);
			if(pmesh->isShow())
			{
				m_ognSkins.push_back((const char *)pmesh->getName());
			}
		}
	}

	m_bUpdating = false;
}

void CItemEditPane::AddSkinList(CBCGPProp* pProp)
{
	pProp->RemoveAllOptions();
	Ogre::SceneNode* pNode = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(pNode == NULL)
		return;

	if(IS_KIND_OF(Model, pNode))
	{
		Ogre::Model* pActorNode = (Ogre::Model*)pNode;
		for(int i = 0; i < (int)pActorNode->getNumSkin(); i++)
		{
			pProp->AddOption((const char*)(pActorNode->getIthSkin(i)->getName()));
		}
	}
}

void CItemEditPane::setModel(CBCGPProp* pProp, LPCTSTR strmodel, LPCTSTR fieldName, int anchor_index)
{
	Ogre::SceneNode* pNode = CEffectMgr::GetInstance()->GetModelActor()->getMainModel();
	if(!IS_KIND_OF(Model, pNode))
	{
		return;
	}

	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;
	int iItemIndex = GetCheckedItem();
	CString strFieldName = fieldName;
	int id = FieldNameToId(strFieldName);
	if(id < 0)
		return;

	CItemTypeInfo *piteminfo = pReader->m_itemTypes[iItemIndex];
	Ogre::Model* pActorNode = (Ogre::Model*) pNode;

	CEffectMgr::GetInstance()->GetModelActor()->unbindAll(id+100);
	if(strmodel[0])
	{
		int *pattrib = CEffectMgr::GetInstance()->m_avatarShowInfo->m_curAttrib;
		CTextParser *parser = pAvaShow->CreateTextParser(pattrib);
		parser->AddParam("field", strmodel);
		std::string path = parser->ParseString(piteminfo->GetFieldPath(id));

		Ogre::Entity* pModel = new Entity;
		if(pModel->load(path.c_str(), true))
		{
			CEffectMgr::GetInstance()->GetModelActor()->bindObject(GetAnchor(anchor_index), pModel, id+100);
		}
		OGRE_RELEASE(pModel);
	}

	pProp->SetValue(strmodel);
}

int CItemEditPane::GetAnchor(int mod)
{
	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;
	int iItemIndex = GetCheckedItem();
	int type = pReader->m_itemTypes[iItemIndex]->m_iTypeId;
	if(type < 0)
		return -1;

	for(int i = 0; i < (int)pReader->m_VarList.size(); i++)
	{
		if(pReader->m_VarList[i]->GetVarType() != CVarInfo::EQUIPVAR)
			continue;

		CEquipVarInfo* pVar = (CEquipVarInfo*)pReader->m_VarList[i];
		for(int j = 0; j < (int) pVar->m_supportType.size(); j++)
		{
			if(pVar->m_supportType[j] == type)
			{
				int ret = -1;
				if(mod == 1)
					ret = pVar->m_iAnchorPos[0];
				else if(mod==2)
					ret = pVar->m_iAnchorPos[1];
				else
					ret = pVar->m_iAnchorPos[2];

				if(ret == 0)
					ret = -1;

				return ret;
			}
		}

	}
	return -1;
}

int CItemEditPane::GetCheckedItem()
{
	for(int i = 0; i < m_wndListType.GetItemCount(); i++)
	{
		if(m_wndListType.GetCheck(i))
			return i;
	}
	return -1;
}


char* g_fieldName[] = 
{
	"ItemDisplayID",
	"Model",
	"Model2",
	"Model3",
	"Skin",
	"Skin2",
	"Icon",
	"GeosetA",
	"GeosetB",
	"GeosetC",
	"GeosetD",
	"GeosetE",
	"Flags",
	"TexA",//"TexArmUpper",
	"TexB",//"TexArmLower",
	"TexC",//"TexHands",
	"TexD",//"TexChestUpper",
	"TexE",//"TexChestLower",
	"TexF",//"TexLegUpper",
	"TexG",//"TexLegLower",
	"TexH",//"TexFeet",
	"Visuals"
};
int g_fieldCount = sizeof(g_fieldName)/sizeof(char *);
int FieldNameToId(CString& fieldName)
{
	for(int i = 0; i < g_fieldCount; i++)
	{
		if(fieldName == g_fieldName[i])
			return i;
	}
	return -1;
}

bool CItemEditPane::IsFieldEnabled(CString strFieldName)
{
	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;
	int id = FieldNameToId(strFieldName);
	if(id < 0)
		return false;
	int iCurrType = GetCheckedItem();
	if(iCurrType < 0)
		return false;
	if(pReader->m_itemTypes[iCurrType]->m_FieldEnable[id].bEnable)
	{
		return true;
	}
	return false;
}

bool CItemEditPane::AddProp(int iItemIndex, CNotifiedProp* pProp, CBCGPProp* pParent, CString strFieldName)
{
	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;
	int id = FieldNameToId(strFieldName);
	if(id < 0)
		return false;

	if(pReader->m_itemTypes[iItemIndex]->m_FieldEnable[id].bEnable)
	{
		pParent->AddSubItem(pProp);
		pProp->SetNotifyTarget(GetSafeHwnd());
		return true;
	}
	return false;
}

bool CItemEditPane::AddTexProp(int iItemIndex, CNotifiedProp* pProp, CBCGPProp* pParent, CString strFieldName)
{
	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;
	int id = FieldNameToId(strFieldName);
	if(id < 0)
	{
		delete pProp;
		return false;
	}

	if(pReader->m_itemTypes[iItemIndex]->m_FieldEnable[id].bEnable)
	{
		CString str = pProp->GetName();
		str.AppendFormat("(所在贴图: %d)", pReader->m_itemTypes[iItemIndex]->m_FieldEnable[id].iTexID );
		pProp->SetName(str.GetBuffer());
		pParent->AddSubItem(pProp);
		pProp->SetNotifyTarget(GetSafeHwnd());
		return true;
	}
	delete pProp;
	return false;
}
void CItemEditPane::CreateProp()
{
	m_wndPropListMod.RemoveAll();
	int iItemIndex = GetCheckedItem();
	if(iItemIndex < 0)
		return;

	CEffectMgr::GetInstance()->GetModelActor()->unbindAll(101);
	CEffectMgr::GetInstance()->GetModelActor()->unbindAll(102);

	AvatarManager* pAvaShow = CEffectMgr::GetInstance()->m_avatarShowInfo->m_pAvatarShow;
	AvatarConfig* pReader = &pAvaShow->m_Config;

	UpdateDispProp(CREATE_PROP);

	CBCGPProp* pSkin = new CBCGPProp("皮肤显示");
	//AddProp(iItemIndex,	m_pPropModel,		pMod,	"Model") == true)
	if(AddProp(iItemIndex,	m_pPropGeosetA,		pSkin,	"GeosetA")) AddSkinList(m_pPropGeosetA);
	else m_pPropGeosetA = NULL;
	if(AddProp(iItemIndex,	m_pPropGeosetB,		pSkin,	"GeosetB")) AddSkinList(m_pPropGeosetB);
	else m_pPropGeosetB = NULL;
	if(AddProp(iItemIndex,	m_pPropGeosetC,		pSkin,	"GeosetC")) AddSkinList(m_pPropGeosetC);
	else m_pPropGeosetC = NULL;
	if(AddProp(iItemIndex,	m_pPropGeosetD,		pSkin,	"GeosetD")) AddSkinList(m_pPropGeosetD);
	else m_pPropGeosetD = NULL;
	if(AddProp(iItemIndex,	m_pPropGeosetE,		pSkin,	"GeosetE")) AddSkinList(m_pPropGeosetE);
	else m_pPropGeosetE = NULL;

	CBCGPProp* pMod = new CBCGPProp("物体绑定");
	if(AddProp(iItemIndex,	m_pPropModel,		pMod,	"Model") == true)
	{
		CString name = m_pPropModel->GetName();
		name.AppendFormat( " (绑定点： %d)", GetAnchor(1));
		m_pPropModel->SetName(name.GetBuffer());
	}
	else m_pPropModel = NULL;

	if(!AddProp(iItemIndex,	m_pPropSkin,		pMod,	"Skin")) m_pPropSkin = NULL;

	if(AddProp(iItemIndex,	m_pPropModel2,		pMod,	"Model2") == true)
	{
		CString name = m_pPropModel2->GetName();
		name.AppendFormat( " (绑定点： %d)", GetAnchor(2));
		m_pPropModel2->SetName(name.GetBuffer());
	}
	else m_pPropModel2 = NULL;

	if(!AddProp(iItemIndex,	m_pPropSkin2,		pMod,	"Skin2")) m_pPropSkin2 = NULL;

	if(AddProp(iItemIndex,	m_pPropModel3,		pMod,	"Model3") == true)
	{
		CString name = m_pPropModel3->GetName();
		name.AppendFormat( " (绑定点： %d)", GetAnchor(3));
		m_pPropModel3->SetName(name.GetBuffer());
	}
	else m_pPropModel3 = NULL;

	CBCGPProp* pTex = new CBCGPProp("贴图替换");
	if(!AddTexProp(iItemIndex,	 m_pPropTexA,	pTex,	"TexA")) m_pPropTexA = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexB,	pTex,	"TexB")) m_pPropTexB = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexC,	pTex,	"TexC")) m_pPropTexC = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexD,	pTex,	"TexD")) m_pPropTexD = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexE,	pTex,	"TexE")) m_pPropTexE = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexF,	pTex,	"TexF")) m_pPropTexF = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexG,	pTex,	"TexG")) m_pPropTexG = NULL;
	if(!AddTexProp(iItemIndex,	 m_pPropTexH,	pTex,	"TexH")) m_pPropTexH = NULL;

	CBCGPProp *pEffect = new CBCGPProp("装备特效");
	if(!AddProp(iItemIndex, m_pPropVisuals, pEffect, "Visuals")) m_pPropVisuals = NULL;

	m_wndPropListMod.AddProperty(pSkin);
	m_wndPropListMod.AddProperty(pMod);
	m_wndPropListMod.AddProperty(pTex);
	m_wndPropListMod.AddProperty(pEffect);

}


void CItemEditPane::UpdateDispProp(DATA_UPDATA_TYPE type)
{
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropGeosetA,		&m_strGeosetA,		type, _T("显示模块A"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropGeosetB,		&m_strGeosetB,		type, _T("显示模块B"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropGeosetC,		&m_strGeosetC,		type, _T("显示模块C"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropGeosetD,		&m_strGeosetD,		type, _T("隐藏模块D"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropGeosetE,		&m_strGeosetE,		type, _T("隐藏模块E"));

	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropModel,			&m_strModel,		type, _T("模型1"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropModel2,			&m_strModel2,		type, _T("模型2"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropModel3,			&m_strModel3,		type, _T("模型3"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropSkin,			&m_strSkin,			type, _T("模型1皮肤"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropSkin2,			&m_strSkin2,		type, _T("模型2皮肤"));

	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexA,		&m_strTexA,		type, _T("区块A"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexB,		&m_strTexB,		type, _T("区块B"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexC,		&m_strTexC,		type, _T("区块C"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexD,		&m_strTexD,		type, _T("区块D"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexE,		&m_strTexE,		type, _T("区块E"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexF,		&m_strTexF,		type, _T("区块F"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexG,		&m_strTexG,		type, _T("区块G"));
	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropTexH,		&m_strTexH,		type, _T("区块H"));

	CEffectEditBar::PropUpdate_cstring((CBCGPProp**)&m_pPropVisuals,	&m_strVisuals,	type, _T("特效名字"));
}