// ExtractorDlg.h : header file
//

#pragma once

#include "SelfExtractor.h"
#include "BmpProgCtrl.h"

#define WM_NOTIFY_MESSAGE				(WM_USER + 400)

// CExtractorDlg dialog
class CExtractorDlg : public CDialog
{
// Construction
public:
	CExtractorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EXTRACTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	// 开始更新
	virtual void startUpdate();
	// 解包
	// 参数: 要打补丁包的
	void unCompressedPackage( LPCTSTR szPath );

	void ShowOpenFileDialog(CString gamePath);

	// 打包回调
	static void AddCallBack(void* CallbackData, void* Data);

	BOOL ExtractorFile(LPCTSTR szFileName,LPCTSTR szPatchPath);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CSelfExtractor		m_SelfExtractor;
	CBmpProgCtrl		m_bmUpdateProgress;
public:
	afx_msg void OnBnClickedButton1();
};
