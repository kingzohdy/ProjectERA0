#pragma once


// CModelEditDlg dialog

class CModelEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CModelEditDlg)

public:
	CModelEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModelEditDlg();

// Dialog Data
	enum { IDD = IDD_MODEL_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCommand(WPARAM wParam);
	afx_msg void OnOptionCheckCommand(WPARAM wParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedCheckLinkmeshVisible();
	afx_msg void OnBnClickedCheckGrassVisible();
	afx_msg void OnBnClickedCheckGlobalmodelVisible();
	afx_msg void OnBnClickedCheckGlobalmodelLock();
	afx_msg void OnBnClickedRadioGlobalModel();
	afx_msg void OnBnClickedCheckModelSnapwater();
	afx_msg void OnBnClickedCheckBigLock();
	afx_msg void OnBnClickedCheckSmallLock();
	afx_msg void OnBnClickedCheckDetailLock();
	afx_msg void OnBnClickedCheckPlantLock();
	afx_msg void OnBnClickedCheckCliffAutolink();
	afx_msg void OnBnClickedCheckModelSnapgrid();
	afx_msg void OnEnChangeEditModelareaSize();
	afx_msg void OnBnClickedRadioBigModel();
	afx_msg void OnBnClickedButtonModelareaEdit();
	afx_msg void OnBnClickedRadioDetailModel();
	afx_msg void OnBnClickedRadioPlantModel();
};
