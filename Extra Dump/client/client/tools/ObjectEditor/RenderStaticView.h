
#pragma once
#include "InputHandler.h"
#include "ObjectEditorTypes.h"
#include "CameraCtrl.h"

class CRenderStatic;
class CCallBackFrameHandler
{
public:
	virtual void OnCallbackUpdate(float dtime) = 0;
	virtual void OnCallbackDraw() = 0;
};

class CRenderStaticHandler : public CInputHandler
{
public:
	virtual void Start();
	virtual void Shut();

	virtual void OnMouseMove(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnLButtonDown(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnRButtonDown(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnLButtonUp(UINT nFlags, CPoint point, HWND hViewprot);
	virtual void OnRButtonUp(UINT nFlags, CPoint point, HWND hViewprot);

	CRenderStatic* m_pRenderStatic;
	CPoint m_FixedPos;
};


class CRenderStatic : public CWnd, public CFrameJob
{
	DECLARE_DYNAMIC(CRenderStatic)

public:
	CRenderStatic();
	virtual ~CRenderStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	Ogre::Camera*			m_pCamera;
	CCameraCtrl					m_CameraCtrl;
	Ogre::SimpleGameScene*		m_pPreviewRoom;
	Ogre::MovableObject *		m_pPreviewObject;

	Ogre::NormalSceneRenderer*	m_pSceneRenderer;
	Ogre::RenderWindow*			m_pRenderTarget;
	float						m_fRatio;
	CString						m_strPath;

	CRenderStaticHandler		m_defHandler;
	CInputHandler*				m_pCurrHandler;
	CCallBackFrameHandler*		m_pCallBackHandler;

	bool						m_bSelfDraw;
	bool						m_bAutoCamera;
	bool						m_bDraw;
	bool						m_bDrawOnce;

	void	SetCallBackHandler(CCallBackFrameHandler* pHandler)	{ m_pCallBackHandler = pHandler; };
	void	SetSelfDraw(bool bSelfDraw);
	void	SetAutoCamera(bool bAutoCamera)			{ m_bAutoCamera = bAutoCamera; };
	void	SetInputHandler(CInputHandler* pHandler);

	void	StartRender();
	void	SetModel(CString path);
	void	SetModel(Ogre::MovableObject *pobj);
	void	ShutRender();

	CString	GetModPath()	{ return m_strPath; };

	void	Init();
	void	Update(Ogre::uint ldtick);
	void	Draw(bool bDebugMsgBox = false);
	void	DoFrame(bool bDebugMsgBox = false);
	virtual void DoJobFrame(int dtick);

	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);

	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


