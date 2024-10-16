
#include "stdafx.h"
#include "InputHandler.h"
#include "EditorManager.h"
#include "CameraCtrl.h"

Ogre::Vector3 WindowToViewport(HWND hWnd, POINT pt)
{
	Ogre::Vector3 ret;
	RECT rt;
	GetClientRect(hWnd, &rt);
	int x = pt.x;
	int y = rt.bottom - rt.top - pt.y;
	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	float fx = ((float)x - (float)width / 2.0f) / (float)width * 2.0f;
	float fy = ((float)y - (float)height / 2.0f) / (float)height * 2;

	ret.x = fx;
	ret.y = fy;
	ret.z = 1.0f;

	return ret;
}

CInputHandler::CInputHandler(void)
{
}

CInputHandler::~CInputHandler(void)
{
}

void CInputHandler::OnMouseMove(UINT nFlags, CPoint point, HWND hViewport)
{
}

void CInputHandler::OnLButtonDown(UINT nFlags, CPoint point, HWND hViewport)
{
}

void CInputHandler::OnRButtonDown(UINT nFlags, CPoint point, HWND hViewport)
{
}

void CInputHandler::OnLButtonUp(UINT nFlags, CPoint point, HWND hViewprot)
{
}

void CInputHandler::OnRButtonUp(UINT nFlags, CPoint point, HWND hViewprot)
{
}


// ----------------------------CDefInputHandler--------------------------- [1/4/2008]


void CDefInputHandler::OnMouseMove(UINT nFlags, CPoint point, HWND hViewport)
{
	if(!CEnginAgent::GetInstance())
		return ;
	CCameraCtrl* pCameraCtrl = CEnginAgent::GetInstance()->GetCameraCtrl();
	if(nFlags & MK_RBUTTON)
	{
		RECT rect;
		GetClientRect(hViewport, &rect);
		int cx = (rect.left + rect.right)/2;
		int cy = (rect.top + rect.bottom)/2;
		int w = (rect.right - rect.left)/2;
		int h = (rect.bottom - rect.top)/2;

		float x = 180.0f*float(point.x - cx)/w;
		float y = 180.0f*float(point.y - cy)/h;

		pCameraCtrl->MouseMoveTrans(x, y);

		if(x !=0 || y !=0)
		{
			POINT pt;
			pt.x = cx;
			pt.y = cy;

			::ClientToScreen(hViewport, &pt);
			::SetCursorPos(pt.x, pt.y);
		}
	}
	else if(nFlags & MK_LBUTTON)
	{
		RECT rect;
		GetClientRect(hViewport, &rect);
		int cx = (rect.left + rect.right)/2;
		int cy = (rect.top + rect.bottom)/2;
		int w = (rect.right - rect.left)/2;
		int h = (rect.bottom - rect.top)/2;

		POINT tmp;
		GetCursorPos( &tmp);

		float x = 180.0f*float(tmp.x - m_FixedPos.x)/w;
		float y = 180.0f*float(tmp.y - m_FixedPos.y)/h;

		if((GetAsyncKeyState(VK_CONTROL  )&0xff00)!=0)
		{
			pCameraCtrl->MouseRollTrans(x, y);
		}
		else
		{
			pCameraCtrl->MouseShiftTrans(x, y);
		}

		if(x !=0 || y !=0)
		{
			POINT pt;
			pt.x = cx;
			pt.y = cy;
			::ClientToScreen(hViewport, &pt);
			::SetCursorPos(m_FixedPos.x, m_FixedPos.y);
		}
	}
}

void CDefInputHandler::OnLButtonDown(UINT nFlags, CPoint point, HWND hViewport)
{
	POINT tmp;
	GetCursorPos(&tmp);
	m_FixedPos = tmp;

	PickObj(hViewport, point);
	//if(pNode)
	//{
	//	CMoveActorHandler* pHandler = new CMoveActorHandler(pNode);
	//	CActionInst::GetInstance()->m_input.SetCurrHandler(pHandler);
	//}
}

void CDefInputHandler::OnRButtonDown(UINT nFlags, CPoint point, HWND hViewport)
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->SetKeyTrans(true);
	RECT rect;
	GetClientRect(hViewport, &rect);
	int cx = (rect.left + rect.right)/2;
	int cy = (rect.top + rect.bottom)/2;
	POINT pt;
	pt.x = cx;
	pt.y = cy;
	ClientToScreen(hViewport, &pt);
	::SetCursorPos(pt.x, pt.y);
}

void CDefInputHandler::OnLButtonUp(UINT nFlags, CPoint point, HWND hViewprot)
{
}

void CDefInputHandler::OnRButtonUp(UINT nFlags, CPoint point, HWND hViewprot)
{
	CEnginAgent::GetInstance()->GetCameraCtrl()->SetKeyTrans(false);
}

Ogre::SceneNode* CDefInputHandler::PickObj(HWND hWnd, CPoint point)
{
	//Ogre::CameraNode* pCamera = CEnginAgent::GetInstance()->m_pCamera;
	//Ogre::Matrix4 viewInv = pCamera->GetViewMatrix();
	//viewInv.Inverse();
	//Ogre::Matrix4 proj = pCamera->GetProjectMatrix();

	//Ogre::Vector3 pTo, pFrom(0.0f, 0.0f, 0.0f);
	//pTo = WindowToViewport(hWnd, point);
	//pTo.x /= proj.GetRow3(0).x;
	//pTo.y /= proj.GetRow3(1).y;
	//pTo.z = 1.0f;
	//pTo = pTo * viewInv;
	//pFrom = pFrom * viewInv;

	//Ogre::WorldRay ray;
	//ray.m_Origin = Ogre::WorldPos(pFrom);
	//ray.m_Dir = pTo - pFrom;
	//Ogre::Normalize(ray.m_Dir);

	//CActionInst* pAction = CActionInst::GetInstance();
	//for(int i = 0; i < (int)pAction->m_actors.size(); i++)
	//{
	//	const Ogre::BoundVolume* pBound = pAction->m_actors[i].pModel->GetMainNode()->GetBounding();

	//	Ogre::SceneNode* pNode = pAction->m_actors[i].pModel->GetMainNode();
	//	float pt;
	//	if(pNode->IntersectRay( ray, &pt, 0))
	//	{
	//		CMoveActorHandler* pHandler = new CMoveActorHandler(pAction->m_actors[i].pModel->GetMainNode());
	//		Ogre::Vector3 offset = 
	//			pAction->m_actors[i].pModel->GetMainNode()->GetPosition().ToVector3() - (ray.m_Origin.ToVector3() + ray.m_Dir*pt);
	//		pHandler->m_vOffset =  offset;
	//		//Ogre::Plane plane(
	//		//	//Ogre::Vector3(0.0f, 1.0f, 0.0f),  pAction->m_actors[i].pModel->GetMainNode()->GetWorldMatrix().GetRow3(3) + offset);
	//		//	Ogre::Vector3(0.0f, 1.0f, 0.0f),  offset);
	//		//pHandler->m_movePlane = plane;
	//		CActionInst::GetInstance()->m_input.SetCurrHandler(pHandler);
	//		break;
	//		//return pAction->m_actors[i].pModel->GetMainNode();
	//	}
	//}
	return NULL;
}




