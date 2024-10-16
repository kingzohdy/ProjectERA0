// OffLineDlg.h : ͷ�ļ�
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

// COffLineDlg �Ի���
class COffLineDlg : public CDialog
{
// ����
public:
	COffLineDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OFFLINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CExpDialog m_ExpDlg;
	CChatDialog	m_ChatDlg;
	CPetDialog m_PetDlg;
	CBoardDialog m_BoardDialog;
	CLoginDialog m_LoginDialog;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);


	// ����������
	Ogre::Root *m_pRoot;
	Ogre::SceneManager *m_pSceneManager;
	RenderUnit *m_pExpRenderUnit;
	RenderUnit *m_pPetRenderUnit;

	char	m_szTip[128];

	NOTIFYICONDATA m_pNid;

protected:
	void InitGUI ();
	void HideAll ();

	// ����
	CClient_Image m_BackGround;

	// ����
	CClient_Image m_ImgTalk;
	CClient_Button m_BtnTalk;

	// ����
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
