#pragma once

#define WM_USER_EDIT_END WM_USER + 2000
class CEditListCtrl;

class CListCtrlEdit : public CEdit
{
	// Construction
public:
	CListCtrlEdit();

	// Attributes
public:
	void  SetCtrlData(DWORD dwData);
	DWORD GetCtrlData();

	CEditListCtrl *m_pEditList;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CListCtrlEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	DWORD m_dwData;
	BOOL m_bExchange;
};

class CListComboBox : public CComboBox
{
public:
	CListComboBox(int si) : m_iSubItem(si) {}
private:
	CString			m_strCurrText;
	int             m_iSubItem;   ///< 记录该下拉框对应列表的列
	//{{AFX_MSG(CListComboBox)
	afx_msg void	OnCloseUp();
	afx_msg void	OnKillFocus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// CEditListCtrl

class CEditListCtrl : public CBCGPListCtrl
{
public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	void ShowEdit(BOOL bShow,int nItem,int nIndex,CRect rc = CRect(0,0,0,0));
	void ShowComboBox(BOOL bShow,int nItem,int nIndex,CRect rc = CRect(0,0,0,0));
	CListCtrlEdit    m_edit;
	CMap< int, int, CComboBox*, CComboBox*> m_pComboBoxMap;    ///< 因为每列的下拉选项是相同的，所以map的第一参数为列（从0开始）
	int m_nItem;
	int m_nSub;

	BOOL Key_Shift(int& nItem,int& nSub);
	BOOL Key_Ctrl(int& nItem,int &nSub);

public:
	void OnEditEnd(WPARAM ,LPARAM lParam = FALSE);
	CComboBox* GetComboBoxByColumn(int col);
};


