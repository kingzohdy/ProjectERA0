#pragma once
#include "afxcmn.h"


// CChatDialog dialog

class CChatDialog : public CDialog
{
	DECLARE_DYNAMIC(CChatDialog)

public:
	CChatDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChatDialog();

// Dialog Data
	enum { IDD = IDD_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM m_RoomParentItem;
	HTREEITEM m_FriendParentItem;
	CImageList	m_ImageList;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg LRESULT onNetNotify(WPARAM,LPARAM);
	afx_msg void OnBnClickedButtonSend();
	CString m_cstrSendMsg;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonClear();
	CTreeCtrl m_PlayerTree;
public:
	afx_msg void OnPrivate();
	afx_msg void OnNMRclickTreePlayer(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_cstrToName;
	CRichEditCtrl m_ShowRichCtrl;
	bool	m_bIsFriend;
	afx_msg void OnBnClickedButtonCleartoname();
	afx_msg void OnNMClickTreePlayer(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
