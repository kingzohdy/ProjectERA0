
#pragma once
#include "RenderStaticView.h"
#include "RotateCoord.h"


// CDirectionEditDlg dialog

class CDirectionEditDlg : public CDialog, public CCallBackFrameHandler
{
	DECLARE_DYNAMIC(CDirectionEditDlg)

public:
	CDirectionEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirectionEditDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRenderStatic m_wndRenderStatic;
	float m_fHrot;
	float m_fVrot;
	float m_fx;
	float m_fy;
	float m_fz;
	float m_fAcclen;
	CSliderCtrl m_sldHrot;
	CSliderCtrl m_sldVrot;
	CSliderCtrl m_sldAcclen;

	CRotCood* m_pRotCoord;

	void UpdateDirByRot();
	void UpdateRotByDir();

	virtual void OnCallbackUpdate(float dtime);
	virtual void OnCallbackDraw();

	afx_msg void OnSldHrotTrace(NMHDR* pNm, LRESULT* pResult);
	afx_msg void OnSldVrotTrace(NMHDR* pNm, LRESULT* pResult);
	afx_msg void OnSldAcclenTrace(NMHDR* pNm, LRESULT* pResult);

	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnPaint();

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnKillfocusEditHrot();
	afx_msg void OnEnKillfocusEditVrot();
};
