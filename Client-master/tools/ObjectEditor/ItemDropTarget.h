
#pragma once

class CItemDragSource : public COleDataSource
{
public: 
	void Drag(int id);
	int m_iItemID;
protected: 
	BOOL OnRenderFileData(LPFORMATETC lpFormatEtc,CFile* pFile);
};


// CItemDropTarget command target
class CItemDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CItemDropTarget)

public:
	CItemDropTarget();
	virtual ~CItemDropTarget();

	struct DROPNOTIFY
	{
		int itemId;
		CPoint dropPos;
		HWND	hWnd;
	};

	HWND m_hNotifyTo;
	void SetNotifyTarget(HWND hWnd) {m_hNotifyTo = hWnd; };
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,	DROPEFFECT dropEffect, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
};


class CItemDropPropListTarget :
	public CItemDropTarget
{
public:
	CItemDropPropListTarget(void);
	~CItemDropPropListTarget(void);

	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	void DrawPropList(CBCGPPropList* pList, CPoint point, CDC* pDC);
	void DrawProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point,CDC* pDC);
	static CBCGPProp* GetHoveringProp(CBCGPPropList* pList, CPoint point);
	static CBCGPProp* GetHoveringSubProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point);

private:
	CBCGPProp* m_pLastProp;
};


