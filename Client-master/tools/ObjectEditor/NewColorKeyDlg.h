#pragma once
#include "OgreColourValue.h"
#include "OgreKeyFrameArray.h"
#include "ColorMoveCtrl.h"


// CNewColorKeyDlg dialog

class CNewColorKeyDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewColorKeyDlg)

public:
	CNewColorKeyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewColorKeyDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_NEWCOLORKEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	afx_msg LRESULT OnLineUpdateDot(WPARAM w, LPARAM l);

	COLORREF ColorFltToRef(Ogre::ColourValue flt);
	Ogre::ColourValue ColorRefToFlt(COLORREF ref);

	void UpdateInfo();
	CEdit m_editInfo;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CListCtrl m_KeyValueList;

	float m_fMidPoint;
	COLORREF m_color1;
	COLORREF m_color2;
	COLORREF m_color3;

	CColorMoveCtrl m_colorMove;

	Ogre::KeyFrameArray<Ogre::ColourValue> m_KeyValues;

	void RefreshList();
	void UpdateCtrl();
	afx_msg void OnBnClickedCheckRandomFrame();
};
