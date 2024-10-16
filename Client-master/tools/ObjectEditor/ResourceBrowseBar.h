
#pragma once
#include "ResourceReader.h"
#include "NormalSplitterWnd.h"


enum TakeOnMode
{
	TakeOnList,
	TakeOnIcon
};

enum SelectKind
{
	OpenFileSel,
	CharacterRes,
	SpeEffTexRes,
	PublicEffRes,
	ModelRes,
	ChunkData
};

class CBrowserBar : public CBCGPDockingControlBar, public CChunkAnsycLoaderReceiver		
{
	DECLARE_DYNAMIC(CBrowserBar)

public:
	CBrowserBar();
	virtual ~CBrowserBar();
	CBCGPToolBar		m_wndToolBar;	//工具条

	CNormalSplitterWnd	m_wndSplitter;	//分割窗口
	CNormalSplitterWnd	m_wndSplitter2;

	CTreeCtrl			m_wndTree;		//树形控件
	CListCtrl			m_wndList;		//列表控件

	CString				m_strBaseDir;
	CString				m_strCurrentDir;
	CImageList			m_imgList;

	CChunkDetailBar*	m_pChunkDetail;			//模型预览窗口 数据细节
	CChunkAnsycLoader	m_ChunkAnsycLoader;		//线程
	DWORD				m_ListStyle;

	DWORD				m_ReadSeqId;

	bool				m_bPathCge;				//当前路径改变了？
	bool				m_bStateTrans;
	TakeOnMode			m_eTakeOnMode;			//显示模式 图标或者列表？
	SelectKind			m_eSelectKind;			//
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnDropFiles(HDROP hDropInfo);			

	void CBrowserBar::OnBeginDrag(NMHDR* pHdr, LRESULT* pr);
	void CBrowserBar::OnIconViewUI(CCmdUI* pUi);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);


	void RefreshNode(CString path, HTREEITEM treeItem);
	bool HasSubDir(CString path);
	void OnRDownList(NMHDR* pNMHdr, LRESULT* pResult);
	void OnItemExpanded(NMHDR* pNMHdr, LRESULT* pResult);
	void OnSelectChanged(NMHDR* pNMHdr, LRESULT* pResult);

	void LoadDirTree(CString basedir);
	bool ValidateFileName(CString name);
	CString RemoveExtName(CString name);

	void OnChunkSelChange(NMHDR* pNMHdr, LRESULT* pResult);
	void RefreshList(CString path);
	CString GetSelectionChunk(CString);
	CString GetSelectionPath();

	CBitmap* LoadPreview(CString strPath);

	void OnReceiveAnsyncChunk(CChunkDetail* pReceivedChunk, DWORD data);

	afx_msg LRESULT OnReceiveAnsyncChunkMsg(WPARAM wParam, LPARAM lParam);

	std::vector<CChunkDetail*> m_ChunkBuf;

	afx_msg void OnMnuCopy();
	afx_msg void OnMnuPaste();
	afx_msg void OnMnuDelete();
	afx_msg void OnOpenExplorer();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();

	afx_msg void OnBtnList();
	afx_msg void OnUpdateBtnList(CCmdUI* pCmdUI);
	afx_msg void OnBtnIcon();
	afx_msg void OnUpdateBtnIcon(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();

	
	CString GetPathNextStr(CString& path);
	//角色资源
	afx_msg void OnBtnCharactor();
	afx_msg void OnUpdateBtnCharactor(CCmdUI* pCmdUI);

	//特效贴图资源
	afx_msg void OnBtnSpetex();
	afx_msg void OnUpdateBtnSpetex(CCmdUI* pCmdUI);

	//公共特效资源
	afx_msg void OnBtnPubeff();
	afx_msg void OnUpdateBtnPubeff(CCmdUI* pCmdUI);

	//模型资源
	afx_msg void OnBtnModelRes();
	afx_msg void OnUpdateBtnModelRes(CCmdUI* pCmdUI);

	//Chunkdata资源目录
	afx_msg void OnBtnChunkdataRes();
	afx_msg void OnUpdateBtnChunkdataRes(CCmdUI* pCmdUI);

	
	CString m_strDelCharactorRes;
	CString m_strDelSpeTexRes;
	CString m_strDelPubEffRes;
	CString m_strDelModelRes;
	CString m_strCurRootPath; 
	void OpenLocalDir(CString& dir);
};