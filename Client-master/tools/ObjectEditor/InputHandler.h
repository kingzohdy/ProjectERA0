#pragma once
#include "OgrePrerequisites.h"

class CInputHandler;

class CInputHandler
{
public:
	CInputHandler(void);
	virtual ~CInputHandler(void);

	virtual void Start(){};
	virtual void Shut(){};

	virtual void OnMouseMove(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnLButtonDown(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnRButtonDown(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnLButtonUp(UINT nFlags, CPoint point, HWND hViewprot);
	virtual void OnRButtonUp(UINT nFlags, CPoint point, HWND hViewprot);
};


class CDefInputHandler : public CInputHandler
{
public:
	virtual void Start() {};
	virtual void Shut() {};

	virtual void OnMouseMove(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnLButtonDown(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnRButtonDown(UINT nFlags, CPoint point, HWND hViewport);
	virtual void OnLButtonUp(UINT nFlags, CPoint point, HWND hViewprot);
	virtual void OnRButtonUp(UINT nFlags, CPoint point, HWND hViewprot);

	Ogre::SceneNode* PickObj(HWND hWnd, CPoint point);

	POINT m_FixedPos;
};



class CInputMgr
{
public:
	CInputMgr() { m_pHandler = &m_defHandler;	};

	void SetCurrHandler(CInputHandler* pHandler)	
	{ 
		if(m_pHandler)
			m_pHandler->Shut();

		m_pHandler = pHandler; 
		if(pHandler == NULL)
			m_pHandler = &m_defHandler;

		m_pHandler->Start();
	};

	CInputHandler* GetCurrHandler()					{ return m_pHandler; };

	CInputHandler* m_pHandler;
	CDefInputHandler m_defHandler;

};
