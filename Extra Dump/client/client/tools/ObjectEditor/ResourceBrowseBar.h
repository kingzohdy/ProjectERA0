
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
	CBCGPToolBar		m_wndToolBar;	//������

	CNormalSplitterWnd	m_wndSplitter;	//�ָ��
	CNormalSplitterWnd	m_wndSplitter2;

	CTreeCtrl			m_wndTree;		//���οؼ�
	CListCtrl			m_wndList;		//�б�ؼ�

	CString				m_strBaseDir;
	CString				m_strCurrentDir;
	CImageList			m_imgList;

	CChunkDetailBar*	m_pChunkDetail;			//ģ��Ԥ������ ����ϸ��
	CChunkAnsycLoader	m_ChunkAnsycLoader;		//�߳�
	DWORD				m_ListStyle;

	DWORD				m_ReadSeqId;

	bool				m_bPathCge;				//��ǰ·���ı��ˣ�
	bool				m_bStateTrans;
	TakeOnMode			m_eTakeOnMode;			//��ʾģʽ ͼ������б�
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
	//��ɫ��Դ
	afx_msg void OnBtnCharactor();
	afx_msg void OnUpdateBtnCharactor(CCmdUI* pCmdUI);

	//��Ч��ͼ��Դ
	afx_msg void OnBtnSpetex();
	afx_msg void OnUpdateBtnSpetex(CCmdUI* pCmdUI);

	//������Ч��Դ
	afx_msg void OnBtnPubeff();
	afx_msg void OnUpdateBtnPubeff(CCmdUI* pCmdUI);

	//ģ����Դ
	afx_msg void OnBtnModelRes();
	afx_msg void OnUpdateBtnModelRes(CCmdUI* pCmdUI);

	//Chunkdata��ԴĿ¼
	afx_msg void OnBtnChunkdataRes();
	afx_msg void OnUpdateBtnChunkdataRes(CCmdUI* pCmdUI);

	
	CString m_strDelCharactorRes;
	CString m_strDelSpeTexRes;
	CString m_strDelPubEffRes;
	CString m_strDelModelRes;
	CString m_strCurRootPath; 
	void OpenLocalDir(CString& dir);
};