#if !defined(AFX_BMPPROGCTRL_H__612712DC_DE51_4C23_95D3_A0B7DF296A42__INCLUDED_)
#define AFX_BMPPROGCTRL_H__612712DC_DE51_4C23_95D3_A0B7DF296A42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpProgCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpProgCtrl window

class CBmpProgCtrl : public CStatic
{
// Construction
public:
	CBmpProgCtrl(UINT bmBack, UINT bmFore);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpProgCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	//void GetRange(int &lower,int &upper);
	int GetPos();
	//int StepIt( );
	//int SetStep( int nStep );
	//int OffsetPos( int nPos );
	int SetPos( int nPos );
	void SetInfo(CString& info)	{ m_strInfo = info;}
	void SetRange( int nLower, int nUpper );
	virtual ~CBmpProgCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpProgCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CString		m_strInfo;
	int			m_nLower;
	int			m_nUpper;
	int			m_nStep;
	int			m_nPos;
	float		m_length;
	CRect		m_rect;
	CBitmap*	oldbmp;
	CBitmap		bmpBack;
	CBitmap		bmpFore;
	CDC*		pDCBack;
	CDC*		pDCFore;
	BITMAP		bmback;
	BITMAP		bmfore;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPPROGCTRL_H__612712DC_DE51_4C23_95D3_A0B7DF296A42__INCLUDED_)
