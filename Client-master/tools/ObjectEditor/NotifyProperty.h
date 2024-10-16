
#pragma once

class IHasNotifyTarget
{
public:
	virtual void SetNotifyTarget(HWND hTarget) = 0;
};


class CNotifiedProp :
	public CBCGPProp, public IHasNotifyTarget
{
	DECLARE_DYNAMIC(CNotifiedProp)
public:
	CNotifiedProp(const CString& strGroupName, DWORD dwData = 0,
		BOOL bIsValueList = FALSE);

	// Simple property
	CNotifiedProp(const CString& strName, const _variant_t& varValue, 
		LPCTSTR lpszDescr = NULL, DWORD dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL,
		LPCTSTR lpszValidChars = NULL, BOOL bHasButton = FALSE);

	CNotifiedProp(const CString& strName, const _variant_t& varValue, 
		BOOL bHasButton, HWND hNotifyTo);

	~CNotifiedProp(void);
	virtual void SetNotifyTarget(HWND hTarget) { m_hNotifyTo = hTarget; };
	virtual BOOL OnEndEdit();
	virtual BOOL OnEdit(LPPOINT lptClick);
	virtual void OnClickButton(CPoint point);
	virtual void OnSelectCombo();
	virtual BOOL OnUpdateValue();
	virtual void OnClickName(CPoint pt);

	virtual void OnDrawName (CDC* pDC, CRect rect);

	void DoDigit();


	bool	m_bRelated;

	bool	m_bEnableDigit;
	float	m_fIncStep;

	void SetFloatDigitRatio(float ratio) { m_fFloatDigitRatio = ratio; };
	float GetFloatDigitRatio()	{ return m_fFloatDigitRatio; };

	//oid SetIntDigitRatio(int ratio) { m_iIntDigitRatio = ratio; };
	void SetDigitRange(float max, float min) { m_fMax = max; m_fMin = min; };;
	//int GetIntDigitRatio()	{ return m_iIntDigitRatio; };
	float	m_fFloatDigitRatio;
	int		m_iIntDigitRatio;
	bool	m_bHasMax;
	float	m_fMax;

	bool	m_bHasMin;
	float	m_fMin;
protected:
	HWND m_hNotifyTo;
	_variant_t m_vOldVal;
};

struct PROPNM
{
	CBCGPProp* pProp;
	BOOL bCancel;
};


class CNotifiedColorProp : public CBCGPColorProp,  public IHasNotifyTarget
{
	DECLARE_DYNAMIC(CNotifiedColorProp)

public:

	// Simple property
	CNotifiedColorProp(const CString& strName, const COLORREF& color, 
		CPalette* pPalette = NULL, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);
	BOOL OnUpdateValue();
	virtual void SetNotifyTarget(HWND hTarget) { m_hNotifyTo = hTarget; };


protected:
	HWND m_hNotifyTo;

};
