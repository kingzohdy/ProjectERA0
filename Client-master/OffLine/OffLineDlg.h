// OffLineDlg.h : 头文件
//

#pragma once

#include "ExpDialog.h"
#include "Client_Image.h"
#include "Client_Button.h"

#include "ChatDialog.h"
#include "PetDialog.h"
#include "BoardDialog.h"
#include "LoginDialog.h"

namespace Ogre
{
	class Root;
	class SceneManager;
}

// COffLineDlg 对话框
class COffLineDlg : public CDialog
{
// 构造
public:
	COffLineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OFFLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CExpDialog m_ExpDlg;
	CChatDialog	m_ChatDlg;
	CPetDialog m_PetDlg;
	CBoardDialog m_BoardDialog;
	CLoginDialog m_LoginDialog;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);


	// 场景管理器
	Ogre::Root *m_pRoot;
	Ogre::SceneManager *m_pSceneManager;
	RenderUnit *m_pExpRenderUnit;
	RenderUnit *m_pPetRenderUnit;

	char	m_szTip[128];

	NOTIFYICONDATA m_pNid;

protected:
	void InitGUI ();
	void HideAll ();

	// 背景
	CClient_Image m_BackGround;

	// 聊天
	CClient_Image m_ImgTalk;
	CClient_Button m_BtnTalk;

	// 经验
	CClient_Image m_ImgExp;
	CClient_Button m_BtnExp;
public:
	afx_msg void OnBnClickedChat();
	afx_msg void OnBnClickedExp();
	virtual void OnFinalRelease();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPet();
	afx_msg void OnBnClickedBoard();
	afx_msg LRESULT onNetNotify(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam);

	void ToTray(BOOL bTray);
	afx_msg void OnTrayQuit();
public:
	afx_msg void OnBnClickedStartgame();
protected:
	virtual void OnCancel();
};
