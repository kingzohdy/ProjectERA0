#pragma once
#include "LineMoveCtrl.h"
#include "OgreKeyFrameArray.h"

#define FKD_LINEAR	1
#define FKD_LOG		2

// CNewFloatKeyDlg dialog

class CNewFloatKeyDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewFloatKeyDlg)

public:
	CNewFloatKeyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewFloatKeyDlg();

	float m_fMax;
	float m_fMin; 

	int m_Mode;
	CString m_strValueName;

	CLineMoveCtrl	m_lineMove;
	CEdit			m_editInfo;


	float ValueRealToVirtual(float realvalue);
	float ValueVirtualToReal(float virtualvalue);

	Ogre::KeyFrameArray<float> m_KeyValues;

// Dialog Data
	enum { IDD = IDD_DIALOG_NEWFLOATKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	void UpdateInfo();
	void UpdateCtrl();
	void RefreshList();
	afx_msg LRESULT OnLineUpdateDot(WPARAM w, LPARAM l);

	afx_msg void OnEnKillfocusEditStart();
	afx_msg void OnEnKillfocusEditMiddle();
	afx_msg void OnEnKillfocusEditFkinfo();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	float m_fCurrPos;
	float m_fCurrValue;
	CListCtrl m_KeyValueList;
	afx_msg void OnLvnItemchangedListPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangingListPos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditMiddle();
protected:
	virtual void OnOK();
};
