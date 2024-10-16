
#pragma once

class ChunkDragSource : public COleDataSource
{
public: 
	CString strPath; 
	CString strChunk;
protected: 
	BOOL OnRenderFileData(LPFORMATETC lpFormatEtc,CFile* pFile);
};

// CTextDropTarget command target
class CBCGPPropList;
class CBCGPProp;
class CResDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CResDropTarget)

public:
	CResDropTarget();
	virtual ~CResDropTarget();

public:
	struct DROPNOTIFY
	{
		LPCTSTR strPath;
		LPCTSTR strChunk;
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

class CResDropPropListTarget : public CResDropTarget
{
public:
	CResDropPropListTarget(void);
	~CResDropPropListTarget(void);

	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

	void DrawPropList(CBCGPPropList* pList, CPoint point, CDC* pDC);
	void DrawProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point,CDC* pDC);
	CBCGPProp* GetHoveringProp(CBCGPPropList* pList, CPoint point);
	CBCGPProp* GetHoveringSubProp(CBCGPPropList* pList, CBCGPProp* pProp, CPoint point);

private:
	CBCGPProp* m_pLastProp;
};