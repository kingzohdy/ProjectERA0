#pragma once

#include <vector>
#include "afxwin.h"
#include "afxcmn.h"
#include "GraphSetting.h"
#include "SystemSettingConfig.h"
#include <atlimage.h>
#include "BtnSt.h"
#include "BitmapSlider.h"

// CGraphSetting dialog

class CGraphSetting : public CDialog
{
	DECLARE_DYNAMIC(CGraphSetting)

public:
	CGraphSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphSetting();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	// 设置界面显示的内容
	void			SetupGuiStyle();
	virtual BOOL	OnInitDialog();
	void			InitDisplayMode();
	void			GetDisplayMode( size_t i, int& w, int& h );
	int				getScreenModeWidth( bool fullscreen, int mode );
	int				getScreenModeHeight( bool fullscreen, int mode );

public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedFullCheck();
	afx_msg void OnCbnSelchangeSampleCombo();
	afx_msg void OnBnClickedPreSetButton();
	afx_msg void OnBnClickedWindowCheck();
	afx_msg void OnBnClickedWideCheck();

private:
	CComboBox		m_combSample;

	CBitmap			m_Background; // 背景

	CImage			m_imgcheck[2];
	CButtonST		m_chkWindowMode;
	CButtonST		m_chkFullScreen;
	CButtonST		m_chkWideScreen;

	CImage			m_imgDefaultPreSetting[3];
	CButtonST		m_btnDefaultPreSetting;

	CBitmapSlider	m_bitmapSliderMusic;
	CBitmapSlider	m_bitmapSliderEffect;

	CImage			m_imgLowPreSetting[3];
	CButtonST		m_btnLowPreSetting;
	CImage			m_imgMediumPreSetting[3];
	CButtonST		 m_btnMediumPreSetting;
	CImage			m_imgHighPreSetting[3];
	CButtonST		m_btnHighPreSetting;
	CImage			m_imgDefaultSetting[3];
	CButtonST		m_btnDefaultSetting;

	CImage			m_imgClose[3];
	CButtonST		m_btnClose;

	CImage			m_imgOk[3];
	CButtonST		m_btnOk;

	CImage			m_imgCancel[3];
	CButtonST		m_btnCancel;

	CImage			m_imgMusic[3];
	CButtonST		m_btnMusic;

	CImage			m_imgSound[3];
	CButtonST		m_btnSound;


	SystemSettingConfig				m_SystemSettingConfig;
	static std::vector<DEVMODE>		s_AllModes;
	static DEVMODE					s_DeskMode;//当前桌面模式
	static std::vector<DEVMODE>		s_FullScreenModes;
	static std::vector<DEVMODE>		s_WideScreenModes;

public:
	afx_msg void OnBnClickedLowButton();
	afx_msg void OnBnClickedMediumButton();
	afx_msg void OnBnClickedHighButton();
	afx_msg void OnBnClickedDefaultButton();
	afx_msg void OnBnClickedConfirmButton();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnBnClickedCloseButton();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam);

};
