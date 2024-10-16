
#include "stdafx.h"
#include "MotionBrowserBar.h"
#include "MotionDirHelper.h"
#include "SplitterGridView.h"
#include "EffectManager.h"
#include "ControlsHelper.h"
#include "ObjectEditor.h"
#include "ModelMotionInfo.h"
#include <shlobj.h>

#include "OgreEntity.h"

IMPLEMENT_DYNAMIC(CMotionBrowserBar, CBCGPDockingControlBar)
CMotionBrowserBar::CMotionBrowserBar()
{
	m_hCommon = NULL;
	m_hPrivate = NULL;
	m_hNotFinished = NULL;

	m_strMotionPath = g_strPublicMotionAddr;
}

CMotionBrowserBar::~CMotionBrowserBar()
{
	//for(int i = 0; i < (int)m_CommonMotions.size(); i++)
	//{
	//	delete m_CommonMotions[i];
	//}
	//m_CommonMotions.clear();

	//for(int i = 0; i < (int)m_PrivateMotions.size(); i++)
	//{
	//	delete m_PrivateMotions[i];
	//}
	//m_PrivateMotions.clear();
	m_imgList.DeleteImageList();
	m_wndSplitter.DestroyWindow();
	DestroyWindow();

}


BEGIN_MESSAGE_MAP(CMotionBrowserBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, ID_MOTIONBROTREE, OnTreeRClick)
	ON_NOTIFY(NM_DBLCLK, ID_MOTIONBROTREE, OnTreeDBClick)
	ON_NOTIFY(TVN_SELCHANGED, ID_MOTIONBROTREE, OnTreeSelectChanged)
	ON_NOTIFY(TVN_BEGINDRAG, ID_MOTIONBROTREE, OnTreeBeginDrag)
	ON_NOTIFY(TVN_ITEMEXPANDED, ID_MOTIONBROTREE, OnTreeExpand)

	//ON_COMMAND(ID_MOTIONBROWSER_PREVIEW, OnPreview)
	ON_COMMAND(ID_MOTIONBROWSER_EDIT, OnEdit)
	ON_COMMAND(ID_MOTIONBROWSER_DELETE, OnDelete)
	ON_COMMAND(ID_MOTIONBROWSER_COPYTOPRIVATE, OnCopyToPrivate)

	//ON_COMMAND(ID_MB_BTN_NEW_COMMON_MO, OnNewCommonMotion)
	//ON_COMMAND(ID_MB_BTN_NEW_PRIVATE_MO, OnNewPrivateMotion)
	ON_COMMAND(ID_MB_BTN_IMPORT_COMMON_TASK, OnImportCommonTask)
	ON_COMMAND(ID_MB_BTN_IMPORT_PRIVATE_TASK, OnImportPrivateTask)


	ON_COMMAND(ID_FOLDER_CREATE_SUB, OnFolderCreateSub)
	ON_COMMAND(ID_FOLDER_DELETE, OnFolderDelete)
	ON_COMMAND(ID_FOLDER_RENAME, OnFolderRename)

	ON_COMMAND(ID_BTN_PLANT_PRODUCT, OnPruduct)
	ON_COMMAND(ID_MB_EXPLORE, OnExplore)
	ON_COMMAND(ID_MB_LOADSHOWMOD, OnMbLoadshowmod)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



enum 
{ 
	ICON_COMMON_MOTION = 0, 
	ICON_COMMON_MOTION_TASK, 
	ICON_PRIVATE_MOTION, 
	ICON_PRIVATE_MOTION_TASK, 
	ICON_GRAY_MOTION,
	ICON_FOLDER 
};
static int iconid[] = 
{ 
	IDI_ICON_COMMON_MOTION, 
	IDI_ICON_COMMON_MOTION_TASK, 
	IDI_ICON_PRIVATE_MOTION, 
	IDI_ICON_PRIVATE_MOTION_TASK, 
	IDI_ICON_GRAY_MOTION,
	IDI_ICON_FOLDER
};

// CMotionBrowserBar message handlers

// 初始化窗口
int CMotionBrowserBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;


	// 工具条
	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MOTIONBROWSER))
	{
		return -1;
	}
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 图片框添加Icon资源
	m_imgList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 10);
	for(int i = 0; i < 6; i++)
	{
		HICON hIcon = ::LoadIcon(theApp.m_hInstance, MAKEINTRESOURCE(iconid[i]));
		m_imgList.Add(hIcon);
	}


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
	GetClientRect (rectClient);

	// 分割窗口
	m_wndSplitter.CreateStatic(this, 2, 1);
	m_wndSplitter.CreateView(0, 0,RUNTIME_CLASS(CSplitterGridView), CSize (100, 400),&context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (100, 300),&context2);
	m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	m_pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	m_pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	//创建树形控件
	m_wndTree.Create(
		WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS
		, CRect(0, 0, 100, 100), m_pGrid1, ID_MOTIONBROTREE);
	m_pGrid1->SetInnerCtrl(&m_wndTree);
	m_wndTree.SetImageList(&m_imgList, TVSIL_NORMAL);//带图标的树形控件

	//编辑框
	m_wndInfo.Create(ES_MULTILINE | ES_READONLY | WS_BORDER, CRect(0, 0, 100, 100), m_pGrid2, ID_MOTIONEDIT);
	m_wndInfo.ShowWindow(SW_SHOW);
	m_pGrid2->SetInnerCtrl(&m_wndInfo);

	m_wndSplitter.ShowWindow(SW_SHOW);

	HTREEITEM hMotion = m_wndTree.InsertItem("技能特效", ICON_FOLDER, ICON_FOLDER);
	HTREEITEM hShowMod = m_wndTree.InsertItem("场景特效", ICON_FOLDER, ICON_FOLDER);


	m_hPrivate = m_wndTree.InsertItem("模型自有技能特效", ICON_FOLDER, ICON_FOLDER, hMotion);
	m_PrivateMotion.m_hBascItem = m_hPrivate;
	//m_CommonDirMotion.SetBaseItem(g_strPublicMotionAddr, m_hPrivate);
	m_PrivateMotion.m_pTreeCtrl = &m_wndTree;
	m_PrivateMotion.m_iImageMotion = ICON_PRIVATE_MOTION;
	m_PrivateMotion.m_iImageCatgory = ICON_FOLDER;


	m_hCommon = m_wndTree.InsertItem("提交的公共技能特效", ICON_FOLDER, ICON_FOLDER, hMotion);
	m_CommonDirMotion.SetBaseItem(g_strPublicMotionAddr, m_hCommon);
	m_CommonDirMotion.m_pTreeCtrl = &m_wndTree;
	m_CommonDirMotion.m_iImageMotion = ICON_COMMON_MOTION_TASK;
	m_CommonDirMotion.m_iImageDir = ICON_FOLDER;
	m_CommonDirMotion.SetFileExt(".emo");

	m_hMaterial = m_wndTree.InsertItem("技能素材", ICON_FOLDER, ICON_FOLDER, hMotion);
	m_MaterialDirMotion.SetBaseItem(g_strMaterialMotionAddr, m_hMaterial);
	m_MaterialDirMotion.m_pTreeCtrl = &m_wndTree;
	m_MaterialDirMotion.m_iImageMotion = ICON_COMMON_MOTION_TASK;
	m_MaterialDirMotion.m_iImageDir = ICON_FOLDER;
	m_MaterialDirMotion.SetFileExt(".emo");

	m_hWork = m_wndTree.InsertItem("技能临时文件", ICON_FOLDER, ICON_FOLDER, hMotion);
	m_WorkDirMotion.SetBaseItem(g_strWorkMotionAddr, m_hWork);
	m_WorkDirMotion.m_pTreeCtrl = &m_wndTree;
	m_WorkDirMotion.m_iImageMotion = ICON_COMMON_MOTION_TASK;
	m_WorkDirMotion.m_iImageDir = ICON_FOLDER;
	m_WorkDirMotion.SetFileExt(".emo");

	m_hCommonMod = m_wndTree.InsertItem("提交的场景特效", ICON_FOLDER, ICON_FOLDER, hShowMod);
	m_CommonDirShowMod.SetBaseItem(g_strShowModelEffectAddr, m_hCommonMod);
	m_CommonDirShowMod.m_pTreeCtrl = &m_wndTree;
	m_CommonDirShowMod.m_iImageMotion = ICON_COMMON_MOTION_TASK;
	m_CommonDirShowMod.m_iImageDir = ICON_FOLDER;
	m_CommonDirShowMod.SetFileExt(".ent");

	m_hMaterialMod = m_wndTree.InsertItem("场景特效素材", ICON_FOLDER, ICON_FOLDER, hShowMod);
	m_MaterialDirShowMod.SetBaseItem(g_strMaterialShowModelEffectAddr, m_hCommonMod);
	m_MaterialDirShowMod.m_pTreeCtrl = &m_wndTree;
	m_MaterialDirShowMod.m_iImageMotion = ICON_COMMON_MOTION_TASK;
	m_MaterialDirShowMod.m_iImageDir = ICON_FOLDER;
	m_MaterialDirShowMod.SetFileExt(".ent");

	m_hWorkMod = m_wndTree.InsertItem("场景特效临时文件", ICON_FOLDER, ICON_FOLDER, hShowMod);
	m_WorkDirShowMod.SetBaseItem(g_strWorkShowModelEffectAddr, m_hCommonMod);
	m_WorkDirShowMod.m_pTreeCtrl = &m_wndTree;
	m_WorkDirShowMod.m_iImageMotion = ICON_COMMON_MOTION_TASK;
	m_WorkDirShowMod.m_iImageDir = ICON_FOLDER;
	m_WorkDirShowMod.SetFileExt(".ent");

	return 0;
}

void CMotionBrowserBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect ClientRect;
	GetClientRect(ClientRect);

	m_wndToolBar.SetWindowPos(NULL, 0, 0, ClientRect.Width(), 24, SWP_NOZORDER);
	m_wndSplitter.SetWindowPos(NULL, 0, 24, ClientRect.Width(), ClientRect.Height() - 24, SWP_NOZORDER);
	//m_wndInfo.SetWindowPos(NULL, 0, clientRect.Height() - 72, clientRect.Width(), 72, SWP_NOZORDER);
	// TODO: Add your message handler code here
}

// 右键点击树形控件节点(弹出菜单)
void CMotionBrowserBar::OnTreeRClick(NMHDR* pHdr, LRESULT* pResult)
{
	// 选中当前右键点击的节点
	POINT pos ;
	if(!GetCursorPos(&pos))
		return ;
	m_wndTree.ScreenToClient(&pos);

	HTREEITEM hSelItem = m_wndTree.GetSelectedItem();

	UINT uFlags;
	hSelItem = m_wndTree.HitTest(pos, &uFlags);
	if ((hSelItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_wndTree.Select(hSelItem, TVGN_CARET);

		POINT point;
		::GetCursorPos(&point);

		CMenu mnu;	

		HTREEITEM hParent = NULL;
		hParent = GetItemBasicParent(hSelItem);
		
		// 匹配item类型
		if(hParent == m_hCommon || hParent == m_hMaterial || hParent == m_hWork )
		{

			MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(hSelItem);
			if(pItemInfo == NULL)
				return;

			
			if(pItemInfo->type == MIT_MOTION)//m_CommonMotion.GetMotionNameByHandle(hSelItem) != "" )
			{
				mnu.LoadMenu(IDR_MENU_MOTIONBROWSER);
				mnu.GetSubMenu(0)->TrackPopupMenu(TPM_HORPOSANIMATION, point.x, point.y, this);	
			}
			else
			{
				mnu.LoadMenu(IDR_MENU_MOTIONBROWSER);
				mnu.GetSubMenu(3)->TrackPopupMenu(TPM_HORPOSANIMATION, point.x, point.y, this);	
			}
		}
		else if(hParent == m_hCommonMod || hParent == m_hMaterialMod || hParent == m_hWorkMod)
		{
			MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); 
			if(pItemInfo == NULL)
				return;

			if(pItemInfo->type == MIT_MOTION)
			{
				mnu.LoadMenu(IDR_MENU_MOTIONBROWSER);
				mnu.GetSubMenu(4)->TrackPopupMenu(TPM_HORPOSANIMATION, point.x, point.y, this);
			}
			else
			{
				mnu.LoadMenu(IDR_MENU_MOTIONBROWSER);
				mnu.GetSubMenu(3)->TrackPopupMenu(TPM_HORPOSANIMATION, point.x, point.y, this);
			}
		}
		else if(hParent == m_hNotFinished)
		{
			//if(m_CommonTaskView.GetMotionNameByHandle(hSelItem) != "")
			//{
			//	mnu.LoadMenu(IDR_MENU_MOTIONBROWSER);
			//	mnu.GetSubMenu(2)->TrackPopupMenu(TPM_HORPOSANIMATION, point.x, point.y, this);
			//}
		}
	}
}

void CMotionBrowserBar::OnTreeDBClick(NMHDR* pHdr, LRESULT* pResult)
{
	// 选中当前双击的节点
	POINT pos ;
	if(!GetCursorPos(&pos))
		return ;
	m_wndTree.ScreenToClient(&pos);

	HTREEITEM hSelItem = m_wndTree.GetSelectedItem();

	hSelItem = m_wndTree.GetSelectedItem();
	if (hSelItem != NULL)
	{

		HTREEITEM hParent = NULL;

		// 如果节点存在并且节点存在父节点
		if(hSelItem != NULL && (hParent = GetItemBasicParent(hSelItem) /*m_wndTree.GetParentItem(hSelItem)*/) != NULL)
		{
			// 匹配节点
			if (hParent == m_hCommon || hParent == m_hMaterial || hParent == m_hWork)
			{
				//CString strSelTitle = m_CommonMotion.GetMotionNameByHandle(hSelItem); //m_wndTree.GetItemText(hSelItem);
				MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(hSelItem);
				if(pItemInfo == NULL || pItemInfo->type != MIT_MOTION)
					return;

				CString strSelTitle = pItemInfo->strChunkName;
				CEffectMgr::GetInstance()->ClearModelMotion();
				CEffectMgr::GetInstance()->GetModelActor()->playMotion(strSelTitle.GetBuffer());

				CString strChunk = m_strMotionPath;
				strChunk.Append(strSelTitle);

				CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
				CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();

				if(pInfo->FindMotionByFileName(strChunk) >= 0)
				{
					SELECT_MOTION_STRUCT ms;
					ms.layer = -1;
					ms.bIsPublic = true;
					ms.strName = pItemInfo->strTitle;
					SendMainFrameMsg(EEM_SELECT_MOTION, 0, (LPARAM)&ms);
				}
			}
			// 节点是模型自由技能特效
			else if(hParent == m_hPrivate)
			{
				CString strSelTitle = m_PrivateMotion.GetMotionNameByHandle(hSelItem); //m_wndTree.GetItemText(hSelItem);
				CEffectMgr::GetInstance()->PlayMotion(strSelTitle.GetBuffer());

				SELECT_MOTION_STRUCT ms;
				ms.layer = -1;
				ms.bIsPublic = false;
				ms.strName = strSelTitle;
				SendMainFrameMsg(EEM_SELECT_MOTION, 0, (LPARAM)&ms);

			}
		}
	}
}

// 展开树形控件
void CMotionBrowserBar::OnTreeExpand(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;

	HTREEITEM hSelItem = pNm->itemNew.hItem;

	if(m_wndTree.GetItemState(hSelItem, TVIS_EXPANDED) != 0)
	{

		MotionItemInfo* pItemInfo = GetMotionInfo(hSelItem); //m_CommonDirMotion.GetInfoByHandle(hSelItem);
		if(pItemInfo == NULL)
			return;

		if(pItemInfo->type != MIT_DIR)
			return;

		//m_CommonDirMotion.UpdateDir(pItemInfo);
		GetMotionDirHelper(hSelItem)->UpdateDir(pItemInfo);
		//m_wndTree.Expand(hSelItem, TVE_EXPAND);
	}
}

// 拖动item
void CMotionBrowserBar::OnTreeBeginDrag(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pNm = (NMTREEVIEW*)pHdr;

	HTREEITEM hSelItem = pNm->itemNew.hItem;

	if (hSelItem != NULL)
	{

		HTREEITEM hParent = NULL;
		if(hSelItem != NULL && (hParent = GetItemBasicParent(hSelItem) /*m_wndTree.GetParentItem(hSelItem)*/) != NULL)
		{
			if (hParent == m_hCommon || hParent == m_hMaterial || hParent == m_hWork)
			{
				//CString strSelTitle = m_CommonMotion.GetMotionNameByHandle(hSelItem); //m_wndTree.GetItemText(hSelItem);
				//m_CommonMotion.DoItemDrag(hSelItem);
				//m_CommonMotion.m_catgoryInfo.SaveCatgoryInfo(".\\MotionCatgory.csv");

			}
			else if(hParent == m_hPrivate)
			{
				//CString strSelTitle = m_PrivateMotion.GetMotionNameByHandle(hSelItem); //m_wndTree.GetItemText(hSelItem);
				//CEffectMgr::GetInstance()->PlayMotion(strSelTitle.GetBuffer());

			}
		}
	}
}


// 改变选择
void CMotionBrowserBar::OnTreeSelectChanged(NMHDR* pHdr, LRESULT* pResult)
{
	NMTREEVIEW* pTreeHdr = (NMTREEVIEW*)pHdr;

	//for(int i = 0; i < (int) m_CommonMotions.size(); i++)
	//{
	//	if(pTreeHdr->itemNew.hItem == m_CommonMotions[i]->hItem)
	//	{
	//		CString strDesc = m_commonTask.FindDescByMotionName(m_CommonMotions[i]->strName);
	//		m_wndInfo.SetWindowText(strDesc.GetBuffer());
	//		break;
	//	}
	//}

	//CString strName = m_CommonMotion.GetMotionNameByHandle(pTreeHdr->itemNew.hItem);
	//CString strDesc = m_commonTask.FindDescByMotionName(strName);
	//m_wndInfo.SetWindowText(strDesc.GetBuffer());

}

void CMotionBrowserBar::OnPreview()
{
	//CString strSelTitle = GetCommonMotionName(m_wndTree.GetSelectedItem()); //m_wndTree.GetItemText(m_wndTree.GetSelectedItem());
	//CString strSelTitle = m_CommonMotion.GetMotionNameByHandle(m_wndTree.GetSelectedItem());
	//if(strSelTitle == "")
	//	return;

	MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetSelectedItem());
	if(pItemInfo == NULL)
		return;
	if(pItemInfo->type != MIT_MOTION)
		return;

	//CString strPlayName = pItemInfo->GetPlayName();

	//MessageBox(strSelTitle.GetBuffer());
	CEffectMgr::GetInstance()->ClearModelMotion();
	CEffectMgr::GetInstance()->GetModelActor()->playMotion(pItemInfo->strChunkName.GetBuffer());
}

void CMotionBrowserBar::OnEdit()
{
	//CString strSelTitle = m_CommonMotion.GetMotionNameByHandle(m_wndTree.GetSelectedItem());
	//if(strSelTitle == "")
	//	return;

	//CString strChunk = m_strMotionPath;
	//strChunk.Append(strSelTitle);

	MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetSelectedItem());
	if(pItemInfo == NULL)
		return;
	if(pItemInfo->type != MIT_MOTION)
		return;

	CString strChunk = pItemInfo->strChunkName;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();

	if(pInfo->FindMotionByFileName(strChunk) >= 0)
	{
		MessageBox("特效已经打开编辑");
	}

	if( true == pInfo->LoadSingleMotionFromFile(pmo, strChunk.GetBuffer()))
	{
		pmo->bIsPublic = true;

		pInfo->AddMotion(pmo);
		SendMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION | UE_EFFECT_LIST, 0);

		SELECT_MOTION_STRUCT ms;
		ms.layer = -1;
		ms.bIsPublic = true;
		ms.strName = pItemInfo->strTitle;
		SendMainFrameMsg(EEM_SELECT_MOTION, 0, (LPARAM)&ms);

	}
	else
	{
		MessageBox("找不到资源或者资源类型错误");
		delete pmo;
	}
}

void CMotionBrowserBar::OnDelete()
{
	//CString strSelTitle = m_CommonMotion.GetMotionNameByHandle(m_wndTree.GetSelectedItem());

	//if(strSelTitle == "")
	//	return;

	//CString strChunk = m_strMotionPath;
	//strChunk.Append(strSelTitle);

	MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetSelectedItem());
	if(pItemInfo == NULL)
		return;
	if(pItemInfo->type != MIT_MOTION)
		return;

	CString strChunk = pItemInfo->strChunkName;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int index = pInfo->FindMotionByFileName(strChunk.GetBuffer());

	if(index >=0 && index < (int)pInfo->m_motions.size())
	{
		if(MessageBox("选择的特效已经打开，将其关闭并同时删除所有包含的特效元素吗?", NULL, MB_YESNO) != IDYES)
		{
			return;
		}
		else
		{
			pInfo->DeleteMontion(index, true); 
			PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION | UE_EFFECT_LIST);
		}
	}

	//char strCurrDir[512];
	//::GetCurrentDirectory(512, strCurrDir);
	//CString strFilePath = strCurrDir;
	//strFilePath.Append("\\chunkdata\\");
	//strChunk.Replace('.', '\\');
	//strFilePath.Append(strChunk);

	//CString strMotionTail = strFilePath;
	//strMotionTail.Append(".emo");
	//FILE* pFile = NULL;
	//if((pFile = fopen(strMotionTail.GetBuffer(), "r")) != NULL)
	//{
	//	fclose(pFile);
	//	strFilePath = strMotionTail;
	//}
	//else
	//{
	//	strFilePath.Append(".chu");
	//}
	//

	CString strFilePath = pItemInfo->GetFilePath();
	char* pFilePath = new char[strFilePath.GetLength() + 2];
	ZeroMemory(pFilePath, strFilePath.GetLength() + 2);
	strcpy(pFilePath, strFilePath.GetBuffer());

	SHFILEOPSTRUCT fileOp;
	fileOp.pFrom = pFilePath;
	fileOp.pTo = NULL;
	fileOp.hwnd = GetSafeHwnd();
	fileOp.wFunc = FO_DELETE;
	fileOp.fFlags = 0;
	SHFileOperation(&fileOp);
	if(fileOp.fAnyOperationsAborted != TRUE)
	{
		m_wndTree.DeleteItem(m_wndTree.GetSelectedItem());
	}

	delete [] pFilePath;
}

void CMotionBrowserBar::OnExplore()
{
	MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetSelectedItem());
	if(pItemInfo == NULL)
		return;

	char buf[1024] = { 0 };
	GetCurrentDirectory(1024, buf);
	CString strCurDir = buf;
	if(strCurDir[strCurDir.GetLength() - 1] != '\\')
	{
		strCurDir.Append("\\");
	}
	strCurDir.Append(pItemInfo->GetFilePath() );

	CString param = " ";
	if(pItemInfo->type == MIT_MOTION)
		param = " /select,";
	param.Append(strCurDir);

	ShellExecute(NULL, _T("open"), _T("Explorer.exe"), 
		param.GetBuffer() , NULL, SW_SHOWDEFAULT);
}

void CMotionBrowserBar::OnCopyToPrivate()
{
	//CString strSelTitle = m_CommonMotion.GetMotionNameByHandle(m_wndTree.GetSelectedItem());
	//CString strChunk = m_strMotionPath;
	//strChunk.Append(strSelTitle);

	MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetSelectedItem());
	if(pItemInfo == NULL)
		return;
	if(pItemInfo->type != MIT_MOTION)
		return;

	CString strChunk = pItemInfo->strChunkName;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
	int index = pInfo->FindMotionByFileName(strChunk.GetBuffer());

	if(index >=0 && index < (int)pInfo->m_motions.size())
	{
		pInfo->m_motions[index]->bIsPublic = false;
		pInfo->m_motions[index]->filename = "";
		PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION | UE_MOTION_BRO_PRIVATE);
	}
	else
	{
		CModelMotionInfo::MOTION* pmo = new CModelMotionInfo::MOTION();
		bool bret = pInfo->LoadSingleMotionFromFile(pmo, strChunk.GetBuffer());
		if(bret == false)
		{
			CString msg;
			msg.Format("找不到资源“%s”或者资源类型错误", strChunk.GetBuffer());

			MessageBox(msg);
			delete pmo;
			return;
		}
		pmo->bIsPublic = false;
		pmo->filename = "";
		if(pInfo->AddMotion(pmo) < 0)
		{
			delete pmo;
			return;
		}

		PostMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION | UE_MOTION_BRO_PRIVATE | UE_EFFECT_LIST);
	}
}

void CMotionBrowserBar::OnMbLoadshowmod()
{
	if(IDNO == MessageBox("该操作将关闭当前编辑的 Show Model，继续吗？", NULL, MB_YESNO))
	{
		return;
	}
	HTREEITEM hSelItem = m_wndTree.GetSelectedItem();
	HTREEITEM hBase = GetItemBasicParent(hSelItem);

	if(hBase == m_hCommonMod || hBase == m_hMaterialMod || hBase == m_hWorkMod)
	{
		MotionItemInfo* pInfo =  GetSelectedMotionInfo();
		if(pInfo != NULL)
		{
			char* str = new char[256];
			strcpy(str, pInfo->strChunkName.GetBuffer());
			SendMainFrameMsg(EEM_REQ_LOAD_ACTOR, 0, (LPARAM)str);

		}
	}

}

void CMotionBrowserBar::OnFolderCreateSub()
{
	assert(0);
	/*
	HTREEITEM hSelItem = m_wndTree.GetSelectedItem();
	HTREEITEM hBase = GetItemBasicParent(hSelItem);
	if(hBase == m_hCommon)
	{
		//`int id = m_CommonMotion.GetCatgoryIdByHandle(hSelItem);
		CInputDlg dlg;
		dlg.SetDesc("请输入新类别名称");
		if(IDOK == dlg.DoModal())
		{
			//m_CommonMotion.m_catgoryInfo.AddCatgory(dlg.m_strText, id);
			//m_CommonMotion.UpdateTree();

			//HTREEITEM hNewItem = m_CommonMotion.GetCatgoryHandleById(id);
			//m_wndTree.EnsureVisible(hNewItem);
			//m_wndTree.Expand(hNewItem , TVE_EXPAND);
			//m_CommonMotion.m_catgoryInfo.SaveCatgoryInfo(".\\MotionCatgory.csv");
			MotionItemInfo* pItemInfo = GetSelectedMotionInfo(); //m_CommonDirMotion.GetInfoByHandle(hSelItem);
			if(pItemInfo == NULL || pItemInfo->type != MIT_DIR)
				return;

			CString strDir = pItemInfo->GetFilePath();
			strDir += dlg.m_strText;
			CCvtHelper cvt;

			SHCreateDirectoryExW(GetSafeHwnd(), cvt.GbToWide(strDir.GetBuffer()), NULL);

			GetSelectedMotionDirHelper()->UpdateDir(pItemInfo);
		}
	}*/
}

void CMotionBrowserBar::OnFolderDelete()
{
	//HTREEITEM hSelItem = m_wndTree.GetSelectedItem();
	//HTREEITEM hBase = GetItemBasicParent(hSelItem);
	//if(hBase == m_hCommon)
	//{
	//	int id = m_CommonMotion.GetCatgoryIdByHandle(hSelItem);
	//	int iParent = m_CommonMotion.m_catgoryInfo.FindCatgory(id)->iParentId;

	//	m_CommonMotion.m_catgoryInfo.RemoveCatgory(id);
	//	m_CommonMotion.UpdateTree();
	//	
	//	//m_CommonMotion.GetCatgoryHandleById(iParent);
	//	HTREEITEM hNewItem = m_CommonMotion.GetCatgoryHandleById(iParent);
	//	m_wndTree.EnsureVisible(hNewItem);
	//	m_wndTree.Expand(hNewItem , TVE_EXPAND);
	//	m_CommonMotion.m_catgoryInfo.SaveCatgoryInfo(".\\MotionCatgory.csv");
	//	
	//}
}

void CMotionBrowserBar::OnFolderRename()
{
	HTREEITEM hSelItem = m_wndTree.GetSelectedItem();
	HTREEITEM hBase = GetItemBasicParent(hSelItem);
	if(hBase == m_hCommon)
	{
		//int id = m_CommonMotion.GetCatgoryIdByHandle(hSelItem);
		//CInputDlg dlg;
		//dlg.SetDesc("请输入类别名称");
		//MotionCatgory* pCat = m_CommonMotion.m_catgoryInfo.FindCatgory(id);
		//if(pCat == NULL)
		//{
		//	MessageBox("不能修改该项的名字");
		//	return;
		//}
		//dlg.m_strText = pCat->strCatgoryName;
		//if(IDOK == dlg.DoModal())
		//{
		//	//m_CommonMotion.m_catgoryInfo.AddCatgory(dlg.m_strText, id);
		//	pCat->strCatgoryName = dlg.m_strText;
		//	m_CommonMotion.UpdateTree();
		//	MotionItemInfo* pItemInfo = m_CommonDirMotion.GetInfoByHandle(hSelItem);
		//	if(pItemInfo == NULL || pItemInfo->type != MIT_DIR)
		//		return;

		//	CString strDir = pItemInfo->GetFilePath();
		//	strDir += dlg.m_strText;
		//	CCvtHelper cvt;

		//	SHCreateDirectory(GetSafeHwnd(), cvt.GbToWide(strDir.GetBuffer()));

		//	m_CommonDirMotion.UpdateDir(pItemInfo);

		//}
		//HTREEITEM hNewItem = m_CommonMotion.GetCatgoryHandleById(id);
		//m_wndTree.EnsureVisible(hNewItem);
		//m_wndTree.Expand(hNewItem , TVE_EXPAND);
		//m_CommonMotion.m_catgoryInfo.SaveCatgoryInfo(".\\MotionCatgory.csv");


	}
}

void CMotionBrowserBar::OnPruduct()
{
	//HTREEITEM hSelItem = m_wndTree.GetSelectedItem();
	//HTREEITEM hBase = GetItemBasicParent(hSelItem);
	//if(hBase == m_hNotFinished)
	//{
	//	CString strMotionName = m_CommonTaskView.GetMotionNameByHandle(hSelItem);
	//	CModelMotionInfo::MOTION* pMotion = new CModelMotionInfo::MOTION();
	//	pMotion->bIsPublic = false;
	//	pMotion->filename = "";
	//	pMotion->name = strMotionName.GetBuffer();
	//	pMotion->fTime = 2.0f;
	//	pMotion->loop = Ogre::ML_ONCE;
	//	pMotion->docInfo.NewDoc();
	//	CEffectMgr::GetInstance()->m_modelMotion.AddMotion(pMotion);

	//	SendMainFrameMsg(EEM_UPDATA_LIST, UE_MOTION);

	//	SELECT_MOTION_STRUCT ms;
	//	ms.layer = -1;
	//	ms.bIsPublic = false;
	//	ms.strName = strMotionName;
	//	SendMainFrameMsg(EEM_SELECT_MOTION, 0, (LPARAM)&ms);
	//}
}

void CMotionBrowserBar::OnNewCommonMotion()
{
	//CModelMotionInfo* pMotionInfo = &CEffectMgr::GetInstance()->m_modelMotion;
	//p
}

void CMotionBrowserBar::OnNewPrivateMotion()
{
}

void CMotionBrowserBar::OnImportCommonTask()
{
	CString path = GetOpenFilePath(TRUE, "", "", "csv", "Excel导出配置(*.csv)|*.csv||", FALSE);
	if(path.GetLength() > 0)
	{
		if(IDYES == MessageBox("新任务单将会覆盖以前的任务单，继续吗？",NULL, MB_YESNO))
		{
			CString strSrcFile = path;
			CString strDstFile = "MotionTask.csv";

			//先读取再复制，有问题在覆盖之前先崩掉再说
			m_commonTask.InitDB(strSrcFile.GetBuffer());

			char* pSrcBuf = new char[strSrcFile.GetLength() + 2];
			ZeroMemory(pSrcBuf, strSrcFile.GetLength() + 2);
			strcpy(pSrcBuf, strSrcFile.GetBuffer());

			char* pDstBuf = new char[strDstFile.GetLength() + 2];
			ZeroMemory(pDstBuf, strDstFile.GetLength() + 2);
			strcpy(pDstBuf, strDstFile.GetBuffer());

			SHFILEOPSTRUCT fileop;
			ZeroMemory(&fileop, sizeof(fileop));

			fileop.hwnd = NULL;
			fileop.pFrom = pSrcBuf;
			fileop.pTo = pDstBuf;
			fileop.wFunc = FO_COPY;
			fileop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;

			SHFileOperation(&fileop);
			delete [] pSrcBuf;
			delete [] pDstBuf;


			UpdateExistingCommonMotion();
		}
	}
}

void CMotionBrowserBar::OnImportPrivateTask()
{
}


void CMotionBrowserBar::OnMainClientCreated()
{
	m_commonTask.InitDB(".\\MotionTask.csv");
	//m_CommonMotion.LoadCatgory(".\\MotionCatgory.csv");
}

void CMotionBrowserBar::OnMainActorLoaded()
{
}

HTREEITEM CMotionBrowserBar::GetItemBasicParent(HTREEITEM hItem)
{
	HTREEITEM hParent = hItem;
	while(hParent != m_hCommon 
		&& hParent != m_hPrivate 
		&& hParent != m_hNotFinished 
		&& hParent != m_hMaterial 
		&& hParent != m_hWork 
		&& hParent != m_hCommonMod 
		&& hParent != m_hMaterialMod
		&& hParent != m_hWorkMod
		&& hParent != NULL)
	{
		hParent = m_wndTree.GetParentItem(hParent);
	}

	return hParent;
}


CString	CMotionBrowserBar::GetSelectedCommonChunkFolder()
{
	MotionItemInfo* pItemInfo = m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetSelectedItem());
	if(pItemInfo == NULL)
		return g_strPublicMotionAddr;

	if(pItemInfo->type != MIT_DIR)
	{
		pItemInfo = m_CommonDirMotion.GetInfoByHandle(m_wndTree.GetParentItem(pItemInfo->hTreeItem));
		if(pItemInfo == NULL)
			return g_strPublicMotionAddr;
	}

	assert(pItemInfo->type == MIT_DIR);

	return pItemInfo->strChunkName;
}


void CMotionBrowserBar::UpdateTree()
{
	//m_wndTree.DeleteAllItems();

	//m_hNotFinished = m_wndTree.InsertItem("未完成的公共特效动画", 0, 0);

	UpdateExistingCommonMotion();

	UpdatePrivateMotion();

	m_wndTree.Expand(m_hPrivate, TVE_EXPAND);
	m_wndTree.Expand(m_hCommon, TVE_EXPAND);
	m_wndTree.Expand(m_hNotFinished, TVE_EXPAND);
}

void CMotionBrowserBar::UpdateExistingCommonMotion()
{
	if(GetSafeHwnd() == NULL)
		return;

	m_CommonDirMotion.SetBaseItem(g_strPublicMotionAddr, m_hCommon);
	m_MaterialDirMotion.SetBaseItem(g_strMaterialMotionAddr, m_hMaterial);
	m_WorkDirMotion.SetBaseItem(g_strWorkMotionAddr, m_hWork);

	m_CommonDirShowMod.SetBaseItem(g_strShowModelEffectAddr, m_hCommonMod);
	m_WorkDirShowMod.SetBaseItem(g_strWorkShowModelEffectAddr, m_hWorkMod);
	m_MaterialDirShowMod.SetBaseItem(g_strMaterialShowModelEffectAddr, m_hMaterialMod);

	MotionItemInfo* pItemInfo = m_CommonDirMotion.GetInfoByHandle(m_hCommon);
	if(pItemInfo != NULL && pItemInfo->type == MIT_DIR)
	{
		m_CommonDirMotion.UpdateDir(pItemInfo);
	}

	pItemInfo = m_MaterialDirMotion.GetInfoByHandle(m_hMaterial);
	if(pItemInfo != NULL && pItemInfo->type == MIT_DIR)
	{
		m_MaterialDirMotion.UpdateDir(pItemInfo);
	}

	pItemInfo = m_WorkDirMotion.GetInfoByHandle(m_hWork);
	if(pItemInfo != NULL && pItemInfo->type == MIT_DIR)
	{
		m_WorkDirMotion.UpdateDir(pItemInfo);
	}

	pItemInfo = m_CommonDirShowMod.GetInfoByHandle(m_hCommonMod);
	if(pItemInfo != NULL && pItemInfo->type == MIT_DIR)
	{
		m_CommonDirShowMod.UpdateDir(pItemInfo);
	}

	pItemInfo = m_WorkDirShowMod.GetInfoByHandle(m_hWorkMod);
	if(pItemInfo != NULL && pItemInfo->type == MIT_DIR)
	{
		m_WorkDirShowMod.UpdateDir(pItemInfo);
	}

	pItemInfo = m_MaterialDirShowMod.GetInfoByHandle(m_hMaterialMod);
	if(pItemInfo != NULL && pItemInfo->type == MIT_DIR)
	{
		m_MaterialDirShowMod.UpdateDir(pItemInfo);
	}
}


void CMotionBrowserBar::UpdateExistingCommonMotion2()
{
	//m_CommonMotion.ClearMotion();


	//WIN32_FIND_DATA find;
	//HANDLE hFileFind = ::FindFirstFile(".\\chunkdata\\effect\\public\\*.*", &find);
	//if(hFileFind == INVALID_HANDLE_VALUE)
	//{
	//	assert(0 && "FindFirstFile returns INVALID_HANDLE");
	//}
	//do
	//{
	//	if((find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
	//		continue;

	//	CString strTitle;
	//	CString strExt;
	//	GetFileNameParts(find.cFileName, strTitle, strExt);

	//	CString strChunk = m_strMotionPath;
	//	strChunk.Append(strTitle);
	//	Ogre::CHUNKTYPE_T chunktype = CChunkLoader::GetChunkType("chunkdata", strChunk.GetBuffer());

	//	if(chunktype == Ogre::CHUNK_MODELMOTION)
	//	{
	//		m_CommonMotion.AddMotion(strTitle, strChunk);
	//	}
	//}
	//while(FindNextFile(hFileFind, &find));
	//if(m_wndTree.GetSafeHwnd())
	//	m_wndTree.Expand(m_hCommon, TVE_EXPAND);

	//ResetCommonMotionIcon();

	//m_CommonTaskView.ClearMotion();

	//for(int i = 0; i < (int)m_commonTask.m_Tasks.size(); i++)
	//{
	//	CString strTaskMotionName = m_commonTask.m_Tasks[i].strMotionName;
	//	strTaskMotionName.MakeUpper();
	//	bool bExist = false;
	//	for(int k = 0; k < (int) m_CommonMotion.m_Motions.size(); k++)
	//	{
	//		CString strMotionName = m_CommonMotion.m_Motions[k]->strName;
	//		strMotionName.MakeUpper();
	//		if(strTaskMotionName == strMotionName)
	//		{
	//			bExist = true;
	//			break;
	//		}
	//	}

	//	if(bExist == false)
	//	{
	//		//m_wndTree.InsertItem(m_commonTask.m_Tasks[i].strMotionName, ICON_COMMON_MOTION_TASK, ICON_COMMON_MOTION_TASK, m_hNotFinished);

	//		m_CommonTaskView.AddMotion(m_commonTask.m_Tasks[i].strMotionName, "");
	//	}
	//}
}

void CMotionBrowserBar::UpdatePrivateMotion()
{
	if(CEffectMgr::GetInstance() == NULL && GetSafeHwnd() == NULL)
		return;

	m_PrivateMotion.ClearMotion();
	if(CEffectMgr::GetInstance() == NULL) return;

	CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->m_modelMotion;
	for(int i = 0; i < (int) pInfo->m_motions.size(); i++)
	{
		CModelMotionInfo::MOTION* pMotion = pInfo->m_motions[i];
		if(pMotion->bIsPublic == true)
			continue;

		CString strName = pMotion->name.c_str();
		m_PrivateMotion.AddMotion(strName, "");

	}
	//ResetCommonMotionIcon();
	m_wndTree.Expand(m_hPrivate, TVE_EXPAND);
}

void CMotionBrowserBar::ResetCommonMotionIcon()
{
	//for(int i = 0; i < (int)m_CommonMotion.m_Motions.size(); i++)
	//{
	//	CString strCommonMotionName = m_CommonMotion.m_Motions[i]->strName;
	//	strCommonMotionName.MakeUpper();

	//	bool bFound = false;
	//	for(int k = 0; k < (int) m_PrivateMotion.m_Motions.size(); k++)
	//	{
	//		CString strPrivateMotionName = m_PrivateMotion.m_Motions[k]->strName;
	//		strPrivateMotionName.MakeUpper();
	//		if(strPrivateMotionName == strCommonMotionName)
	//		{
	//			bFound = true;
	//			break;
	//		}
	//	}

	//	if(bFound == true)
	//	{
	//		m_wndTree.SetItemImage(m_CommonMotion.m_Motions[i]->hItem, ICON_GRAY_MOTION, ICON_GRAY_MOTION);
	//	}
	//	else
	//	{
	//		m_wndTree.SetItemImage(m_CommonMotion.m_Motions[i]->hItem, ICON_COMMON_MOTION, ICON_COMMON_MOTION);
	//	}
	//}
}

void CMotionBrowserBar::GetFileNameParts(CString strFileName, CString& strTitle, CString& strExt)
{
	int dot = strFileName.ReverseFind('.');

	if(dot < 0)
	{
		strTitle = strFileName;
		strExt = "";
		return;
	}

	strTitle = strFileName.Left(dot);
	strExt = strFileName.Right(strFileName.GetLength() - dot - 1);
}

MotionItemInfo*	CMotionBrowserBar::GetSelectedMotionInfo()
{
	return GetMotionInfo(m_wndTree.GetSelectedItem());
}

MotionItemInfo* CMotionBrowserBar::GetMotionInfo(HTREEITEM hItem)
{
	MotionItemInfo* pRet = m_CommonDirMotion.GetInfoByHandle(hItem);

	if(pRet == NULL)
	{
		pRet = m_MaterialDirMotion.GetInfoByHandle(hItem);
	}

	if(pRet == NULL)
	{
		pRet = m_WorkDirMotion.GetInfoByHandle(hItem);
	}

	if(pRet == NULL)
	{
		pRet = m_CommonDirShowMod.GetInfoByHandle(hItem);
	}

	if(pRet == NULL)
	{
		pRet = m_WorkDirShowMod.GetInfoByHandle(hItem);
	}

	if(pRet == NULL)
	{
		pRet = m_MaterialDirShowMod.GetInfoByHandle(hItem);
	}


	return pRet;
}

CMotionDirHelper* CMotionBrowserBar::GetMotionDirHelper(HTREEITEM hItem)
{
	//HTREEITEM hBase =  GetItemBasicParent(hItem);
	//if(hBase == m_hCommon)
	//	return &m_CommonDirMotion;
	//if(hBase == m_hMaterial)
	//	return &m_MaterialDirMotion;
	//if(hBase == m_hWork)
	//	return &m_WorkDirMotion;

	MotionItemInfo* pRet = m_CommonDirMotion.GetInfoByHandle(hItem);
	if(pRet != NULL)
		return &m_CommonDirMotion;

	pRet = m_MaterialDirMotion.GetInfoByHandle(hItem);
	if(pRet != NULL)
		return &m_MaterialDirMotion;

	pRet = m_WorkDirMotion.GetInfoByHandle(hItem);
	if(pRet != NULL)
		return &m_WorkDirMotion;

	pRet = m_CommonDirShowMod.GetInfoByHandle(hItem);
	if(pRet != NULL)
		return &m_CommonDirShowMod;

	pRet = m_WorkDirShowMod.GetInfoByHandle(hItem);
	if(pRet != NULL)
		return &m_WorkDirShowMod;

	pRet = m_MaterialDirShowMod.GetInfoByHandle(hItem);
	if(pRet != NULL)
		return &m_MaterialDirShowMod;

	return NULL;
}

CMotionDirHelper* CMotionBrowserBar::GetSelectedMotionDirHelper()
{
	return GetMotionDirHelper(m_wndTree.GetSelectedItem());
}


void CMotionBrowserBar::OnDestroy()
{
	m_CommonDirMotion.ClearData();
	m_MaterialDirMotion.ClearData();
	m_WorkDirMotion.ClearData();


	m_CommonDirShowMod.ClearData();
	m_MaterialDirShowMod.ClearData();
	m_WorkDirShowMod.ClearData();
}
