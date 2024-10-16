
#pragma once
#include <vector>

#define EPLN_SELECTCHANGE	10
class CNotifiedProp;
// CExtPropList
struct EPLNMHDR
{
};

class CExtPropList : public CBCGPPropList
{
	DECLARE_DYNAMIC(CExtPropList)
	friend class CNotifiedProp;
public:
	//CExtPropList();
	virtual ~CExtPropList();
	virtual void OnChangeSelection (CBCGPProp* pNewSel, CBCGPProp* OldSel);

	void InsertRelateProp(CBCGPProp* pProp);
	void ClearProp();
	void RemoveAllExt();
	//virtual int OnDrawProperty (CDC* pDC, CBCGPProp* pProp) const;

	BOOL IsVSDotNetLook() { return m_bVSDotNetLook; };
	BOOL IsControlBarColors() { return m_bControlBarColors; };
	BOOL IsFocused()	{ return m_bFocused; };
	COLORREF GetGroupTextColor() { return m_clrGroupText; };

	std::vector<CBCGPProp*> m_relateProp;
protected:
	DECLARE_MESSAGE_MAP()
};


