#pragma once

#include <vector>
#include "webbrowser.h"

// CLauncherSelectView dialog

const int CUSTOM_SELECT_BUTTON_MSG = 0xDEAE;
class CLauncherSelectView : public CDialog
{
	DECLARE_DYNAMIC(CLauncherSelectView)

public:
	CLauncherSelectView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherSelectView();
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	void			SetCurSelectArea( int nArea );
	void			SetCurSelectServe( int nServe );

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
//	afx_msg void OnPaint();

private:
	// 设置界面显示的内容
	void			SetupGuiStyle();
	virtual BOOL	OnInitDialog();
private:	
	// 背景图
	CClient_Image		    m_imgBkgnd;
	CClient_Image			m_mskBkgnd;
	
	// 区域按钮
	CClient_Image			m_mskBigWorld1;
	CClient_Image			m_imgBigWorld1;
	CClient_Button			m_btnBigWorld1;

	CClient_Image			m_mskBigWorld2;
	CClient_Image			m_imgBigWorld2;
	CClient_Button			m_btnBigWorld2;

	CClient_Image			m_mskBigWorld3;
	CClient_Image			m_imgBigWorld3;
	CClient_Button			m_btnBigWorld3;

	CClient_Image			m_mskBigWorld4;
	CClient_Image			m_imgBigWorld4;
	CClient_Button			m_btnBigWorld4;

	// 服务器按钮
	CClient_Image		    m_mskServe1;
	CClient_Image		    m_imgServe1;
	CClient_Button		    m_btnServe1;

	CClient_Image		    m_mskServe2;
	CClient_Image		    m_imgServe2;
	CClient_Button		    m_btnServe2;

	CClient_Image		    m_mskServe3;
	CClient_Image		    m_imgServe3;
	CClient_Button		    m_btnServe3;

	CClient_Image		    m_mskServe4;
	CClient_Image		    m_imgServe4;
	CClient_Button		    m_btnServe4;

	CClient_Image		    m_mskServe5;
	CClient_Image		    m_imgServe5;
	CClient_Button		    m_btnServe5;

	CClient_Image		    m_mskServe6;
	CClient_Image		    m_imgServe6;
	CClient_Button		    m_btnServe6;

	CClient_Image		    m_mskServe7;
	CClient_Image		    m_imgServe7;
	CClient_Button		    m_btnServe7;
	
	// 确认选择按钮
	CClient_Image		    m_imgConfirmSelect;
	CClient_Button		    m_btnConfirmSelect;

	// 关闭按钮
	CClient_Image		    m_imgClose;
	CClient_Button		    m_btnClose;

	// 此为大区按钮集合
	typedef std::vector<CClient_Button* > LstImageButtons;
	LstImageButtons         m_vecAreaBtns;

	// 此为服务器按钮集合
	LstImageButtons         m_vecServeBtns;
	LstImageButtons			m_vecFunctionBtns;
	
	int						m_nCurSelectArea;
	int						m_nCurSelectServe;

// 	CPing*					m_pPing;
	static std::string		m_strPingInfo;
	std::string				m_strCurSelectIP;
	static void SetPingInfo( const char* pszInfo );

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnClose();
};
