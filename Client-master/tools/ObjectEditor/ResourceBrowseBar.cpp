
#include "stdafx.h"
#include "ResourceBrowseBar.h"
#include "ResourceReaderBar.h"
#include "SplitterGridView.h"
#include "ResourceReaderBar_Dir.h"
#include "ResourceDropTarget.h"
#include "Clipboard_File.h"
#include "ControlsHelper.h"

#include "OgreResourceManager.h"
#include "OgreTexture.h"

#define EEBM_CHUNKREAD (WM_USER + 99)

const char *RES_BASE_DIR = ".";

IMPLEMENT_DYNAMIC(CBrowserBar, CBCGPDockingControlBar)
CBrowserBar::CBrowserBar()
{
	m_eSelectKind = ChunkData;
	m_eTakeOnMode = TakeOnIcon;
	m_pChunkDetail = NULL;
	m_ListStyle = LVS_ICON;
	m_ReadSeqId = 0;
	m_bStateTrans = false;
	m_bPathCge = false;
}

CBrowserBar::~CBrowserBar()
{
	if(m_pChunkDetail != NULL)
		delete m_pChunkDetail;
	m_ChunkAnsycLoader.End();
	m_wndTree.DestroyWindow();
	m_wndList.DestroyWindow();
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CBrowserBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	//ON_UPDATE_COMMAND_UI(IDB_ICONVIEW, OnIconViewUI)
	ON_WM_SIZE()

	ON_NOTIFY(TVN_ITEMEXPANDED, ID_DIRTREE, OnItemExpanded)
	ON_NOTIFY(TVN_SELCHANGED, ID_DIRTREE, OnSelectChanged)


	ON_NOTIFY(LVN_BEGINDRAG, ID_CHUNKLIST, OnBeginDrag)
	ON_NOTIFY(LVN_ITEMCHANGED, ID_CHUNKLIST, OnChunkSelChange)
	ON_NOTIFY(NM_RCLICK, ID_CHUNKLIST, OnRDownList)

	ON_WM_DROPFILES()
	ON_COMMAND(ID_MNU_COPY, OnMnuCopy)
	ON_COMMAND(ID_MNU_PASTE, OnMnuPaste)
	ON_COMMAND(ID_MNU_DELETE, OnMnuDelete)

	ON_COMMAND(ID_BRS_BTN_LIST, OnBtnList)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_LIST, OnUpdateBtnList)

	ON_COMMAND(ID_BRS_BTN_ICON, OnBtnIcon)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_ICON, OnUpdateBtnIcon)

	ON_COMMAND(ID_BRS_BTN_CHARACTOR, OnBtnCharactor)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_CHARACTOR, OnUpdateBtnCharactor)

	ON_COMMAND(ID_BRS_BTN_SPETEX, OnBtnSpetex)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_SPETEX, OnUpdateBtnSpetex)

	ON_COMMAND(ID_BRS_BTN_PUBEFF, OnBtnPubeff)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_PUBEFF, OnUpdateBtnPubeff)

	ON_COMMAND(ID_BRS_BTN_MODELRES, OnBtnModelRes)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_MODELRES, OnUpdateBtnModelRes)

	ON_COMMAND(ID_BRS_BTN_CHUNKDATA, OnBtnChunkdataRes)
	ON_UPDATE_COMMAND_UI(ID_BRS_BTN_CHUNKDATA, OnUpdateBtnChunkdataRes)

	ON_COMMAND(ID_BRS_BTN_OPENEXPLORER, OnOpenExplorer)
	ON_MESSAGE(EEBM_CHUNKREAD, OnReceiveAnsyncChunkMsg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CBrowserBar message handlers
void CBrowserBar::OnIconViewUI(CCmdUI* pUi)
{
	pUi->Enable(TRUE);
}

int CBrowserBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if(!m_wndBrowser.Create(0, CRect(0, 0, 100, 100), this))
	//{
	//	TRACE("Fail to create browser\n");
	//	return -1;
	//}
	//
	//m_wndBrowser.ShowWindow(SW_SHOW);
	//m_wndBrowser.UpdateWindow();
	//m_wndBrowser.EnableWindow(TRUE);
	//m_wndBrowser.SetBaseDir(".\\chunkdata");

	// 工具条
	if(!m_wndToolBar.Create(this) 
		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR_BROWSER))
	{
		return -1;
	}
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);


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

	//把主窗口分成2部分(pGrid21 pGrid22)
	m_wndSplitter2.CreateStatic(this, 2, 1);
	m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, 500), &context1);
	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, 200), &context2);
	m_wndSplitter2.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	CSplitterGridView* pGrid21 = (CSplitterGridView*)m_wndSplitter2.GetPane(0, 0);
	CSplitterGridView* pGrid22 = (CSplitterGridView*)m_wndSplitter2.GetPane(1, 0);


	//把pGrid21分成2部分(pGrid1 pGrid2)

	m_wndSplitter.CreateStatic(pGrid21, 2, 1);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, 200), &context1);
	m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CSplitterGridView), CSize (0, 400), &context2);
	//m_wndSplitter.MoveWindow(CRect(rectClient.left, rectClient.top + 24, rectClient.right, rectClient.bottom));
	pGrid21->SetInnerCtrl(&m_wndSplitter);
	CSplitterGridView* pGrid1 = (CSplitterGridView*)m_wndSplitter.GetPane(0, 0);
	CSplitterGridView* pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	//在pGrid1 创建树形控件
	m_wndTree.Create(WS_VISIBLE |  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS , 
		CRect(0, 0, 100, 100), pGrid1, ID_DIRTREE);
	pGrid1->SetInnerCtrl(&m_wndTree);

	//在pGrid2 创建列表框
	m_wndList.CreateEx(LVS_EX_FULLROWSELECT,  WS_VISIBLE | LVS_ICON, 
		CRect(0, 0, 100, 100), pGrid2, ID_CHUNKLIST);
	m_wndList.InsertColumn(0, "资源名", LVCFMT_LEFT, 200);
	pGrid2->SetInnerCtrl(&m_wndList);

	//m_wndList.InsertItem(LVIF_TEXT, 0, "asdfasdf", 0, 0, 0, 0);
	m_wndSplitter.ShowWindow(SW_SHOW);


	m_imgList.Create(64, 64, ILC_COLOR24 , 1, 1000);

	SetWindowText(_T("资源浏览器"));

	// 读取目录树
	m_strBaseDir = RES_BASE_DIR;
	LoadDirTree(RES_BASE_DIR);

	DragAcceptFiles(TRUE);//??

	//预览窗口
	pGrid22->SetInnerCtrl(NULL);
	m_pChunkDetail = new CChunkDirBar();
	m_pChunkDetail->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 100, 100), pGrid22, 1);

	pGrid22->SetInnerCtrl(m_pChunkDetail);
	m_pChunkDetail->ShowWindow(SW_SHOW);
	pGrid22->UpdateInnerCtrl();

	//开始线程
	m_ChunkAnsycLoader.SetNotifyTarget(this);
	m_ChunkAnsycLoader.Start();

	return 0;
}

void CBrowserBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	CRect frameRect;
	GetClientRect(frameRect);

	m_wndSplitter2.SetWindowPos(NULL, 0, 24,
		frameRect.Width(), frameRect.Height() - 24 , SWP_NOZORDER);

	m_wndToolBar.SetWindowPos(NULL, 0, 0,
		frameRect.right - frameRect.left, 24, SWP_NOZORDER);

	m_wndList.Arrange(LVA_DEFAULT);
}

void CBrowserBar::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_ChunkAnsycLoader.End();//结束线程
	__super::OnClose();
}

BOOL CBrowserBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	//if(m_wndBrowser.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;
	return CBCGPDockingControlBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//树形控件通告消息
void CBrowserBar::OnItemExpanded(NMHDR* pNMHdr, LRESULT* pResult)
{
	NMTREEVIEW* pInfo = (NMTREEVIEW*) pNMHdr;
	CString fullpath;
	HTREEITEM hItem = pInfo->itemNew.hItem;
	while(hItem != TVI_ROOT && hItem != NULL)
	{
		fullpath = m_wndTree.GetItemText(hItem) + "\\" + fullpath;
		hItem = m_wndTree.GetParentItem(hItem);
	}
	//fullpath = m_strBaseDir + "\\" + fullpath;

	/* 此函数与OnSelectChanged中都增加了这些代码
	** 将非chunkdata作为根目录的路径转换成以chunkdata\为根目录
	*/
	CString firststr;
	firststr = fullpath.Left(fullpath.Find("\\"));
	fullpath = m_strCurrentDir.Left(m_strCurrentDir.Find(firststr)) + fullpath;

	if(fullpath.Find("chunkdata") == -1)
	{
		fullpath = m_strCurRootPath + fullpath;
	}

	m_strCurrentDir = fullpath;//
	m_bPathCge = true;

	RefreshNode(fullpath, pInfo->itemNew.hItem); 
}

//当树形控件选择发生改变时 通告消息
void CBrowserBar::OnSelectChanged(NMHDR* pNMHdr, LRESULT* pResult)
{
	NMTREEVIEW* pInfo = (NMTREEVIEW*) pNMHdr;
	CString fullpath;
	HTREEITEM hItem = pInfo->itemNew.hItem;

	//获取完整路径名
	while(hItem != TVI_ROOT && hItem != NULL)
	{
		fullpath = m_wndTree.GetItemText(hItem) + "\\" + fullpath;
		hItem = m_wndTree.GetParentItem(hItem);
	}
	//m_strRelaDir = fullpath;
	//fullpath = m_strBaseDir + "\\" + fullpath;
	CString firststr;
	firststr = fullpath.Left(fullpath.Find("\\"));
	fullpath = m_strCurrentDir.Left(m_strCurrentDir.Find(firststr)) + fullpath;

	if(fullpath.Find("chunkdata") == -1)
	{
		fullpath = m_strCurRootPath + fullpath;
	}

	m_strCurrentDir = fullpath;
	m_bPathCge = true;
	//if(m_hMsgTo != NULL)
	//	::PostMessage(m_hMsgTo, DIRMSG_DIRCHANGED, 0, 0);

	if(m_pChunkDetail->GetSafeHwnd())
	{
		m_pChunkDetail->DestroyWindow();
	}
	if(m_pChunkDetail != NULL)
	{
		delete m_pChunkDetail;
	}

	RefreshList(m_strCurrentDir);



	//更新模型预览窗口
	CSplitterGridView* pGrid = (CSplitterGridView*)m_wndSplitter2.GetPane(1, 0);
	pGrid->SetInnerCtrl(NULL);

	m_pChunkDetail = new CChunkDirBar();
	m_pChunkDetail->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 100, 100), pGrid, 1);

	pGrid->SetInnerCtrl(m_pChunkDetail);
	m_pChunkDetail->ShowWindow(SW_SHOW);
	pGrid->UpdateInnerCtrl();
}

//当列表框文件选择发生改变时 通告消息
void CBrowserBar::OnChunkSelChange(NMHDR* pNMHdr, LRESULT* pResult)
{
	NMLISTVIEW* pnm = (NMLISTVIEW*)pNMHdr;
	if((pnm->uOldState&LVIS_SELECTED) == 0 
		&& (pnm->uNewState&LVIS_SELECTED) != 0 )
	{
		if(m_pChunkDetail != NULL)
		{
			if(m_pChunkDetail->GetSafeHwnd())
			{
				m_pChunkDetail->DestroyWindow();
			}
			delete m_pChunkDetail;
			m_pChunkDetail = NULL;
		}

		CSplitterGridView* pGrid = (CSplitterGridView*)m_wndSplitter2.GetPane(1, 0);
		pGrid->SetInnerCtrl(NULL);

		int iItem = pnm->iItem;
		m_pChunkDetail = m_ChunkBuf[iItem]->CreateBar(pGrid);
		if(m_pChunkDetail != NULL)
		{
			pGrid->SetInnerCtrl(m_pChunkDetail);
			pGrid->UpdateInnerCtrl();
		}
	}
}

//右键消息-弹出菜单
void CBrowserBar::OnRDownList(NMHDR* pNMHdr, LRESULT* pResult)
{
	POINT pt;
	GetCursorPos(&pt);

	//CBCGPPopupMenu popMnu;
	//popMnu.Create(this, pt.x, pt.y, ::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_BROWSER_POP)));
	CMenu mnu;
	mnu.LoadMenu(IDR_MENU_BROWSER_POP);
	CMenu* pSubMenu = mnu.GetSubMenu(0);
	pSubMenu->TrackPopupMenu(0, pt.x, pt.y, this);
}


// 拖动文件
void CBrowserBar::OnBeginDrag(NMHDR* pHdr, LRESULT* pr)
{
	NMLISTVIEW* pNm = (NMLISTVIEW*)pHdr;

	ChunkDragSource* pSrc = new ChunkDragSource();
	pSrc->strChunk = GetSelectionChunk(m_strCurrentDir);
	pSrc->strPath = GetSelectionPath();
	pSrc->DelayRenderFileData(CF_TEXT, NULL);
	DROPEFFECT de = pSrc->DoDragDrop();				

	delete pSrc;

}

// 获取选中文件完整文件名
CString CBrowserBar::GetSelectionPath()
{
	//int baselen = m_strBaseDir.GetLength();
	//int pathlen = m_strCurrentDir.GetLength();
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if(!pos)
		return "";
	int i = m_wndList.GetNextSelectedItem(pos);


	CString path = m_strCurrentDir + '\\' + m_wndList.GetItemText(i, 0);

	return path;
}
//获取文件名
CString CBrowserBar::GetSelectionChunk(CString curDir)
{
	int baselen = m_strBaseDir.GetLength();
	//	 = m_strCurrentDir.GetLength();
	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	if(!pos)
		return "";
	int i = m_wndList.GetNextSelectedItem(pos);

	CString path = curDir +m_wndList.GetItemText(i, 0);
	int pathlen = path.GetLength();
	if(pathlen < baselen + 5) // ".chu" 和一个'\\'
		return "";

	//CString ret = path.Right(pathlen - baselen - 1);
	CString ret = path;

	return ret;
}

//读取当前目录下的文件 
void CBrowserBar::LoadDirTree(CString basedir)
{
	m_strBaseDir = basedir;
	m_strCurrentDir = m_strBaseDir + "\\";
	HTREEITEM hItem = m_wndTree.InsertItem(m_strBaseDir, 0, 0); // 插入"." 跟节点
	RefreshNode(basedir, hItem);
	m_wndTree.Expand(hItem, TVE_EXPAND);
}

//刷新节点
void CBrowserBar::RefreshNode(CString path, HTREEITEM treeItem)
{
	HTREEITEM hItem;
	hItem = m_wndTree.GetChildItem(treeItem);
	while(hItem != NULL) 
	{
		HTREEITEM hnext = m_wndTree.GetNextItem(hItem, TVGN_NEXT);
		m_wndTree.DeleteItem(hItem);
		hItem = hnext;
	}

	std::vector<CString> dirPathes;
	CFileFind fileFind;
	BOOL bWorking = fileFind.FindFile(path + "\\*.*", 0);
	while(bWorking)
	{
		bWorking = fileFind.FindNextFile();
		if(fileFind.GetFileName() == "." || fileFind.GetFileName() == "..")
			continue;

		// 是否目录 
		if(fileFind.IsDirectory())
		{
			CString filen = fileFind.GetFileName();
			hItem = m_wndTree.InsertItem(fileFind.GetFileName(), treeItem, TVI_LAST);
			if(HasSubDir(fileFind.GetFilePath()))
			{
				m_wndTree.InsertItem("搜索中...", hItem, TVI_LAST);
			}
		}
	}
}

//是否存在子目录
bool CBrowserBar::HasSubDir(CString path)
{
	CFileFind fileFind;
	BOOL bWorking = fileFind.FindFile(path + "\\*.*", 0);
	while(bWorking)
	{
		bWorking = fileFind.FindNextFile();
		if(fileFind.GetFileName() == "." || fileFind.GetFileName() == "..")
			continue;
		if(fileFind.IsDirectory())
		{
			return true;
		}

	}
	return false;
}


// 文件名是否有效(能否打开)
bool CBrowserBar::ValidateFileName(CString name)
{
	int dot = name.ReverseFind('.'); 
	if (dot < 0) 
		return false;

	// 得到小写扩展名
	CString ext = name.Right(name.GetLength() - dot);
	ext = ext.MakeLower();

	// 把得到的扩展名与支持的扩展名数组匹配 
	for(int i = 0; i < g_nExten; i++)
	{
		if(ext == g_FileExten[i])
			return true;
	}
	return false;
}

//移除扩展名
CString CBrowserBar::RemoveExtName(CString name)
{
	CString ret;
	int dot = name.ReverseFind('.');
	if (dot < 0) 
		return "";

	return name.Left(dot);
}

//获取扩展名
CString GetFileExt(CString strFileName)
{
	int start = strFileName.GetLength() - 1;
	while(start >= 0 && strFileName.GetAt(start) != '/' && strFileName.GetAt(start) != '\\')
	{
		if(strFileName.GetAt(start) == '.')
		{
			char* buf = (char*)strFileName.GetBuffer();
			CString ret = buf + start + 1;
			return ret;
		}
		start--;
	}

	return "";
}

//	刷新列表框
void CBrowserBar::RefreshList(CString path)
{

	m_wndList.DeleteAllItems();//删除所有Item

	m_imgList.DeleteImageList();//清空图片列表
	m_imgList.Create(64, 64, ILC_COLOR24, 1, 100);
	m_wndList.SetImageList(&m_imgList, 0);

	m_ChunkAnsycLoader.ClearWork();
	for(int i = 0; i < (int)m_ChunkBuf.size(); i++)
		delete m_ChunkBuf[i];
	m_ChunkBuf.clear();

	CBitmap* pbmp = CBitmap::FromHandle(::LoadBitmap((HINSTANCE)AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LOGO_ICON)));
	int defid = m_imgList.Add(pbmp, RGB(1, 1, 1));//添加pbmp

	CFileFind fileFinder;
	CString strFileName;
	CString strFileExtension;

	m_ReadSeqId++;
	int index = 0;
	BOOL bWork = fileFinder.FindFile(m_strCurrentDir + "\\*.*", 0);
	while (bWork) 
	{
		bWork = fileFinder.FindNextFile();

		// 是否目录
		if(fileFinder.IsDirectory())
			continue;
		// ----test--------- [7/7/2008]
		//std::string chunk = fileFinder.GetFilePath().GetBuffer();
		//int pos = chunk.find_last_of('.');
		//if(pos < 0)
		//{
		//	MessageBox( chunk.c_str(), "pos < 0", 0);
		//	
		//}
		//else
		//{
		//	chunk = chunk.substr(0, pos);

		//	pos = chunk.find_last_of('\\');
		//	std::string basedir = chunk.substr(0, pos);
		//	std::string chunkname = chunk.substr(pos + 1, chunk.length() - pos - 1);
		//	MessageBox( basedir.c_str(), chunkname.c_str(), 0);
		//}
		// ------end test--------- [7/7/2008]

		strFileName = fileFinder.GetFileName();
		//strFileExtension = strFileName.Right(4);
		//if(strFileExtension.MakeUpper() != ".CHU")
		//	continue;

		// 文件名是否有效
		if(ValidateFileName(strFileName) == false)
			continue;

		//reader.ReadChunkInfo(ci->strPath , ci);
		//m_chunkInfo.push_back(ci);
		int iCurrIndex = defid;
		CString addr = GetRelativePathToCurDir(fileFinder.GetFilePath());
		CString ext = GetFileExt(fileFinder.GetFilePath());
		//CBitmap* pBmp = LoadPreview(fileFinder.GetFilePath());
		CChunkDetail* pChunkDetail = CChunkDetail::CreateDefaultChunkDetail(addr); //CChunkDetail::CreateChunkDetailFromChunk(addr);
		CBitmap* pBmp = pChunkDetail->GetIcon();
		m_ChunkBuf.push_back(pChunkDetail);
		m_ChunkAnsycLoader.AddNewWork(addr, ext, m_ReadSeqId);
		if(pBmp)
		{
			iCurrIndex = m_imgList.Add(pBmp, RGB(1,1,1));
			//delete pBmp;
		}
		int id = m_wndList.InsertItem(index, strFileName.GetBuffer(), iCurrIndex);

		index++;
	}
}


LRESULT CBrowserBar::OnReceiveAnsyncChunkMsg(WPARAM wParam, LPARAM lParam)
{
	CChunkDetail* pChunkDetail = (CChunkDetail*)lParam;
	DWORD srcid = (DWORD)wParam;
	bool bFound = false;
	if(srcid != m_ReadSeqId)
	{
		TRACE("Abandon Chunk: %s, currid=%d, chunkid=%d\n", pChunkDetail->m_strChunkName.GetBuffer(), m_ReadSeqId, srcid);
		return 0;
	}

	for(int i = 0; i < (int) m_ChunkBuf.size(); i++)
	{
		if(m_ChunkBuf[i]->GetChunkName() == pChunkDetail->GetChunkName())
		{
			int iSel = m_wndList.GetSelectionMark();
			if(i == iSel)
			{
				if(m_pChunkDetail != NULL)
				{
					delete m_pChunkDetail;
					m_pChunkDetail = NULL;
				}

				CSplitterGridView* pGrid = (CSplitterGridView*)m_wndSplitter2.GetPane(1, 0);
				pGrid->SetInnerCtrl(NULL);
			}

			delete m_ChunkBuf[i];
			m_ChunkBuf[i] = pChunkDetail;
			CBitmap* pBmp = pChunkDetail->GetIcon();
			int iCurrIndex = m_imgList.Add(pBmp, RGB(1,1,1));
			m_wndList.SetItem(i, 0, LVIF_IMAGE, NULL, iCurrIndex, 0, 0, 0, 0);
			bFound = true;
			break;
		}
	}
	return 0;
}

void CBrowserBar::OnReceiveAnsyncChunk(CChunkDetail* pReceivedChunk, DWORD data)
{
	this->PostMessage(EEBM_CHUNKREAD, (WPARAM)data, (LPARAM)pReceivedChunk);
}

//此函数已经移到CChunkDetailBarTex中去
CBitmap* CBrowserBar::LoadPreview(CString strPath)
{

	int baselen = m_strBaseDir.GetLength();
	if(baselen >= 3 && m_strBaseDir.GetAt(1) == ':')
	{
	}
	else
	{
		char tmp[256] = {0};
		::GetCurrentDirectory(256, tmp);
		baselen += strlen(tmp) + 1;
	}

	int pathlen = strPath.GetLength();
	if(pathlen < baselen + 5) // ".chu" 和一个'\\'
		return NULL;

	CString chunk = strPath.Right(pathlen - baselen - 1);
	chunk = RemoveExtName(chunk);
	chunk.Replace('\\', '.');
	chunk.Replace('/', '.');

	Ogre::Resource* pChk = NULL;
	CString fullpath = m_strBaseDir + chunk;
	pChk = Ogre::ResourceManager::getSingleton().blockLoad(fullpath.GetBuffer());

	/*
	Ogre::Resource* pPreviewChunk = ((Ogre::ComplexChunk*)pChk)->GetAttribByName("Preview");
	if(pPreviewChunk != NULL)
	{

	size_t datalen = 0;
	unsigned char* pData = (unsigned char*)pPreviewChunk->asRawData(datalen);
	CDib dib;
	if(TRUE == dib.LoadFromMem((char*)pData, datalen))
	{
	delete(pChk);
	return dib.CreateDib();
	}
	}*/

	if(IS_KIND_OF(Ogre::TextureData, pChk))
	{
		Ogre::TextureData* pTexChk = (Ogre::TextureData*) pChk;

		/*
		Ogre::ITexture* pPreviewTex = NULL;
		if(pTexChk->m_pTex->m_AnimFrames.size() == 0)
		pPreviewTex = pTexChk->m_pTex->m_pSysTex;
		else
		pPreviewTex = pTexChk->m_pTex->m_AnimFrames[0];

		if(pPreviewTex == NULL)
		return NULL;

		Ogre::TextureDX9* pTexD9 = (Ogre::TextureDX9*) pPreviewTex;
		IDirect3DDevice9* pDev = ((Ogre::RendererDX9*)CEnginAgent::GetInstance()->m_pEngine->GetRenderer())->m_pd3dDevice;

		IDirect3DTexture9* pTex;
		IDirect3DSurface9* pSurf;



		IDirect3DSurface9* pOldSur = NULL;
		pDev->GetRenderTarget(0, &pOldSur);

		DWORD oldfvf ;
		pDev->GetFVF(&oldfvf);
		DWORD oldCulMode;
		pDev->GetRenderState(D3DRS_CULLMODE, &oldCulMode);

		D3DXCreateTexture(pDev,64,64,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pTex);		
		pTex->GetSurfaceLevel(0,&pSurf);

		pDev->BeginScene();
		pDev->SetRenderTarget(0,pSurf );

		D3DVIEWPORT9 vp;
		pDev->GetViewport( &vp);
		struct QuadVertex
		{
		D3DXVECTOR4 pos;
		D3DXVECTOR2 tex;
		};

		QuadVertex v[4];
		v[0].pos = D3DXVECTOR4(-0.5f, -0.5f, 0.5f, 1.0f);
		v[0].tex = D3DXVECTOR2(0.0f,0.0f);

		v[1].pos = D3DXVECTOR4(-0.5f, (float)vp.Height,0.5f,1.0f);
		v[1].tex = D3DXVECTOR2(0.0f,1.0f);

		v[2].pos = D3DXVECTOR4( (float)vp.Width, 0.5f,0.5f,1.0f);
		v[2].tex = D3DXVECTOR2( 1.0f,0.0f);

		v[3].pos = D3DXVECTOR4( (float)vp.Width, (float)vp.Height,0.5f,1.0f);
		v[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pDev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0); //clear texture

		pDev->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX1);
		pDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
		pDev->SetTexture(0,pTexD9->GetSysTex());

		pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,(void*)v,sizeof(QuadVertex) );

		pDev->EndScene();

		pDev->SetFVF(oldfvf);
		pDev->SetRenderState(D3DRS_CULLMODE,oldCulMode);
		//pDev->SetTexture(0,pTexD9->GetSysTex());
		pDev->SetRenderTarget(0, pOldSur);


		ID3DXBuffer* pBuffer = NULL;

		D3DXSaveTextureToFileInMemory(&pBuffer, D3DXIFF_BMP,pTex,NULL);

		CDib dib;
		dib.LoadFromMem((char*)pBuffer->GetBufferPointer(), (int)pBuffer->GetBufferSize());

		OGRE_RELEASE(pSurf);
		OGRE_RELEASE(pOldSur);
		OGRE_RELEASE(pBuffer);
		OGRE_RELEASE(pTex);

		delete(pChk);

		return dib.CreateDib();
		*/

	}
	return NULL;

}

void CBrowserBar::OnDropFiles(HDROP hDropInfo)
{
	std::vector<CString> names;

	int iDropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目    
	for(int i = 0; i < iDropCount; i++)  
	{
		int iNameSize = DragQueryFile(hDropInfo, i, NULL, 0);//取得第i个拖动文件名所占字节数
		HANDLE hHeap = GetProcessHeap();
		char *pName = new char[iNameSize + 1];  
		if (pName == NULL)
		{  
			MessageBox("给文件名分配暂存空间时出错!", "错误信息", MB_ICONERROR); 
			return;
		}
		DragQueryFile(hDropInfo, i, pName, iNameSize + 1);//把文件名拷贝到缓冲区   
		CString strName = pName;
		names.push_back(strName);
		delete pName;
	}

	assert(0);

	/*
	CTexTransDlg dlg;
	dlg.m_transType = TTT_SINGLE;

	if(names.size() > 1)
	{
		if(IDOK != dlg.DoModal())
			return;
	}

	CString strPath = m_strCurrentDir;
	CString strName;
	//----------------------------
	CTextureConvert::ConvertTexture(dlg.m_transType, dlg.m_fRate, strPath, names);

	RefreshList(m_strCurrentDir);
	strName.Append(".texture");
	for(int i = 0; i < m_wndList.GetItemCount(); i++)
	{
		CString itemName = m_wndList.GetItemText(i, 0);
		if(itemName.MakeUpper() == strName.MakeUpper())
		{
			m_wndList.SetSelectionMark(i);
			m_wndList.SetHotItem(i);
			m_wndList.EnsureVisible(i, TRUE);
			break;
		}
	}*/

	CBCGPDockingControlBar::OnDropFiles(hDropInfo);
}
CString ExtractFileName(CString path)
{
	int pos = path.ReverseFind('\\');

	if(pos >= 0)
		return path.Right(path.GetLength() - pos - 1);
	return path;
}

char* CreateCombineNameBuffer(std::vector<CString>& names)
{
	int iBufLen = 1;
	for(int i = 0; i < (int)names.size(); i++)
	{
		iBufLen += names[i].GetLength() + 1;
	}

	char* pNameBuf = new char[iBufLen];
	ZeroMemory(pNameBuf, iBufLen);

	int start = 0;
	for(int i = 0; i < (int)names.size(); i++)
	{
		strcpy(pNameBuf + start, names[i].GetBuffer());
		start += (names[i].GetLength() + 1);
	}

	return pNameBuf;
}

void CBrowserBar::OnMnuCopy()	//拷贝菜单
{
	// TODO: Add your command handler code here
	std::vector<CString> files;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	while(pos)
	{
		int i = m_wndList.GetNextSelectedItem(pos);
		CString path = m_strCurrentDir + m_wndList.GetItemText(i, 0);
		files.push_back(path);
	}

	CFileChipboard bd;
	bd.CopyToChipboard(files);
}

void CBrowserBar::OnMnuPaste()	// 粘贴菜单
{
	CFileChipboard bd;
	std::vector<CString> files;

	bd.GetFromChipboard(files);
	if(files.size() <= 0)
	{
		return;
	}

	CString path = m_strCurrentDir ;
	//Source Names
	char* pNameBuf = CreateCombineNameBuffer(files);

	//Destination Names
	std::vector<CString> dstfiles;
	for(int i = 0; i < (int)files.size(); i++)
	{
		CString tmp = path + ExtractFileName(files[i]);
		dstfiles.push_back(tmp);
	}

	char* pDst = CreateCombineNameBuffer(dstfiles);

	//char* pDst = new char[path.GetLength() + 2];
	//ZeroMemory(pDst, path.GetLength() + 2);
	//strcpy(pDst, path.GetBuffer());


	SHFILEOPSTRUCT fileOp;
	fileOp.pFrom = pNameBuf;
	fileOp.pTo = pDst;
	fileOp.hwnd = GetSafeHwnd();
	fileOp.wFunc = FO_COPY;
	fileOp.fFlags = FOF_MULTIDESTFILES;

	SHFileOperation(&fileOp);

	delete [] pNameBuf;
	delete [] pDst;

	RefreshList(m_strCurrentDir);

}

void CBrowserBar::OnMnuDelete()	// 删除菜单
{
	std::vector<CString> files;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition();
	while(pos)
	{
		int i = m_wndList.GetNextSelectedItem(pos);
		CString path = m_strCurrentDir + m_wndList.GetItemText(i, 0);
		files.push_back(path);
	}

	char* pNameBuf = CreateCombineNameBuffer(files);

	SHFILEOPSTRUCT fileOp;
	fileOp.pFrom = pNameBuf;
	fileOp.pTo = NULL;
	fileOp.hwnd = GetSafeHwnd();
	fileOp.wFunc = FO_DELETE;
	fileOp.fFlags = 0;

	SHFileOperation(&fileOp);

	RefreshList(m_strCurrentDir);
}

// 打开文件夹 
void CBrowserBar::OnOpenExplorer()
{
	if(m_eSelectKind == CharacterRes || m_eSelectKind == SpeEffTexRes || m_eSelectKind == PublicEffRes || 
		m_eSelectKind == ModelRes || m_eSelectKind == ChunkData)
		m_bStateTrans = true;
	else if(m_eSelectKind == OpenFileSel)
		m_bStateTrans = false;
	if(m_bStateTrans)
	{
		//		m_wndTree.DeleteAllItems();
		//		OpenFullDir(m_strCurrentDir);
	}

	char buf[1024] = { 0 };

	GetCurrentDirectory(1024, buf);

	CString strCurDir = buf;

	if(strCurDir[strCurDir.GetLength() - 1] != '\\')
	{
		strCurDir.Append("\\");
	}
	strCurDir.Append(m_strCurrentDir );

	CString param = " ";
	param.Append(strCurDir);

	ShellExecute(NULL, _T("open"), _T("Explorer.exe"), 
		param.GetBuffer() , NULL, SW_SHOWDEFAULT);

	m_eSelectKind = OpenFileSel;
}

//列表显示
void CBrowserBar::OnBtnList()
{
	m_eTakeOnMode = TakeOnList;

	CSplitterGridView* pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndList.DestroyWindow();
	m_wndList.CreateEx(LVS_EX_FULLROWSELECT,  WS_VISIBLE | LVS_REPORT, 
		CRect(0, 0, 100, 100), pGrid2, ID_CHUNKLIST);
	m_wndList.InsertColumn(0, "资源名", LVCFMT_LEFT, 200);

	pGrid2->SetInnerCtrl(&m_wndList);
	pGrid2->UpdateInnerCtrl();

	RefreshList(m_strCurrentDir);

	m_ListStyle = LVS_REPORT;
}


void CBrowserBar::OnUpdateBtnList(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	if(m_ListStyle ==  LVS_REPORT && m_eTakeOnMode == TakeOnList)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

// 图标显示
void CBrowserBar::OnBtnIcon()
{
	m_eTakeOnMode = TakeOnIcon;

	CSplitterGridView* pGrid2 = (CSplitterGridView*)m_wndSplitter.GetPane(1, 0);

	m_wndList.DestroyWindow();
	m_wndList.CreateEx(LVS_EX_FULLROWSELECT,  WS_VISIBLE | LVS_ICON, 
		CRect(0, 0, 100, 100), pGrid2, ID_CHUNKLIST);
	m_wndList.InsertColumn(0, "资源名", LVCFMT_LEFT, 200);

	pGrid2->SetInnerCtrl(&m_wndList);
	pGrid2->UpdateInnerCtrl();

	RefreshList(m_strCurrentDir);
	m_ListStyle = LVS_ICON;
}

void CBrowserBar::OnUpdateBtnIcon(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	if(m_ListStyle ==  LVS_ICON && m_eTakeOnMode == TakeOnIcon)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CBrowserBar::OpenLocalDir(CString& dir)
{
	m_wndTree.DeleteAllItems();
	HTREEITEM hItem = m_wndTree.InsertItem(dir, TVI_ROOT, 0);

	RefreshNode(dir,hItem);
	m_wndTree.Expand(hItem,TVE_EXPAND);
	RefreshList(m_strCurrentDir);
}

CString CBrowserBar::GetPathNextStr(CString& path)
{
	if(path.IsEmpty())
		return "";
	path.Replace('.', '\\');
	CString str;
	static int pos1 = 0;
	static int pos2 = 0;
	pos2 = path.Find('\\', pos1);
	if(pos2 == -1)
	{
		pos1 = 0;
		pos2 = 0;
		return "";
	}
	str = path.Mid(pos1, pos2-pos1);
	pos1 = pos2 + 1;
	return str;
}

// 角色资源
void CBrowserBar::OnBtnCharactor()
{
	OpenLocalDir(CString(g_chCharactorResPath));

	m_eSelectKind = CharacterRes;
	m_bStateTrans = true;
}

void CBrowserBar::OnUpdateBtnCharactor(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	if(m_eSelectKind == CharacterRes)
	{
		pCmdUI->SetCheck(1);
		if(m_bPathCge)
		{
			//先去掉m_strDelCharactorRes根路径
			int pos = m_strCurrentDir.Find(m_strDelCharactorRes);
			pos += m_strDelCharactorRes.GetLength();
			CString strAdd = m_strCurrentDir.Mid(pos);

			m_bPathCge = false;
#ifdef DEBUG
			/***********************************************************************
			int tlayer = 0;
			std::vector<CString> test;
			while(m_charactorTreePath.IteratorPath(test, tlayer))
			{
			CString strLayer;
			strLayer.Format("%d",tlayer);
			afxDump<<strLayer<<" layer is: ";
			for(int i=0; i<test.size(); i++)
			{
			afxDump<<test[i]<<" ";
			}
			afxDump<<"\n";
			++tlayer;
			}
			***********************************************************************/
#endif
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

// 特效贴图资源
void CBrowserBar::OnBtnSpetex()
{
	OpenLocalDir(CString(g_chSpecialTextureResPath));

	m_eSelectKind = SpeEffTexRes;
	m_bStateTrans = true;
}

void CBrowserBar::OnUpdateBtnSpetex(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	if(m_eSelectKind == SpeEffTexRes)
	{
		pCmdUI->SetCheck(1);
		if(m_bPathCge)
		{
			int pos = m_strCurrentDir.Find(m_strDelSpeTexRes);
			pos += m_strDelSpeTexRes.GetLength();
			CString strAdd = m_strCurrentDir.Mid(pos);

			m_bPathCge = false;
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

// 公共特效资源
void CBrowserBar::OnBtnPubeff()
{
	OpenLocalDir(CString(g_chPublicEffectResPath));

	m_eSelectKind = PublicEffRes;
	m_bStateTrans = true;
}

void CBrowserBar::OnUpdateBtnPubeff(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else 
	{
		pCmdUI->Enable(TRUE);
	}
	if(m_eSelectKind == PublicEffRes)
	{
		pCmdUI->SetCheck(1);
		if(m_bPathCge)
		{
			int pos = m_strCurrentDir.Find(m_strDelPubEffRes);
			pos += m_strDelPubEffRes.GetLength();
			CString strAdd = m_strCurrentDir.Mid(pos);

			m_bPathCge = false;
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

// 模型资源按钮
void CBrowserBar::OnBtnModelRes()
{
	OpenLocalDir(CString(g_pszModelResPath));

	m_eSelectKind = ModelRes;
	m_bStateTrans = true;
}

void CBrowserBar::OnUpdateBtnModelRes(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	if(m_eSelectKind == ModelRes)
	{
		pCmdUI->SetCheck(1);
		if(m_bPathCge)
		{
			int pos = m_strCurrentDir.Find(m_strDelModelRes);
			pos += m_strDelModelRes.GetLength();
			CString strAdd = m_strCurrentDir.Mid(pos);

			m_bPathCge = false;
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

// 所有资源按钮
void CBrowserBar::OnBtnChunkdataRes()
{
	m_wndTree.DeleteAllItems();
	LoadDirTree(RES_BASE_DIR);

	m_eSelectKind = ChunkData;
	m_bStateTrans = true;
}

void CBrowserBar::OnUpdateBtnChunkdataRes(CCmdUI* pCmdUI)
{
	if(m_ChunkAnsycLoader.GetWorkCount() > 0)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	if(m_eSelectKind == ChunkData)
	{
		pCmdUI->SetCheck(1);
		if(m_bPathCge)
		{
			m_bPathCge = false;
#ifdef DEBUG
			/***********************************************************************
			int tlayer = 0;
			std::vector<CString> test;
			while(m_chunkdataTreePath.IteratorPath(test, tlayer))
			{
			CString strLayer;
			strLayer.Format("%d",tlayer);
			afxDump<<strLayer<<" layer is: ";
			for(int i=0; i<test.size(); i++)
			{
			afxDump<<test[i]<<" ";
			}
			afxDump<<"\n";
			++tlayer;
			}
			**********************************************************************/
#endif
		}
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

BOOL CBrowserBar::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == 'C' && (GetAsyncKeyState(VK_CONTROL)&0xff00)!=0 )
		{
			OnMnuCopy();
		}
		else if (pMsg->wParam == 'V' && (GetAsyncKeyState(VK_CONTROL)&0xff00)!=0 )
		{
			OnMnuPaste();
		}
		else if (pMsg->wParam == VK_DELETE)
		{
			OnMnuDelete();
		}
	}

	return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
}



void CBrowserBar::OnDestroy()
{
	__super::OnDestroy();

	m_ChunkAnsycLoader.ClearWork();
	for(int i = 0; i < (int)m_ChunkBuf.size(); i++)
		delete m_ChunkBuf[i];
	m_ChunkBuf.clear();
}
