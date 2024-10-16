
#include "stdafx.h"
#include "CameraCtrl.h"

#include "OgreRay.h"
#include "OgreMatrix4.h"
#include "OgrePlane.h"
#include "OgreCamera.h"

CCameraCtrl::CCameraCtrl(void)
{
	Reset();
	m_bIsKeyTrans = false;
	m_bProj = true;
	m_fOrthoSize = 1000.0f;
	m_mainNodePos.setElement(0.0f, 0.0f, 0.0f);
}

CCameraCtrl::~CCameraCtrl(void)
{
}

void CCameraCtrl::SetKeyTrans(bool trans)
{
	m_bIsKeyTrans = trans;
}

//在场景中相机的初始位置
void CCameraCtrl::ResetInScene(int nResId)
{
	switch(nResId)
	{
	case 1002:
		m_Viewpoint.x = 6000.0f;
		m_Viewpoint.y = 320.0f;
		m_Viewpoint.z = 6000.0f;
		break;
	case 1003:
		m_Viewpoint.x = 25272.0f;
		m_Viewpoint.y = 420.0f;
		m_Viewpoint.z = 24659.0f;
		break;
	}

	m_ViewPitch = 0.0f;
	m_ViewYaw = -21.0f;

	m_bInScene = true;
}

void CCameraCtrl::Reset()
{
	m_Viewpoint.x = 0.0f;
	m_Viewpoint.y = 200.0f;
	m_Viewpoint.z = -200.0f;
	m_ViewPitch = 0.0f;
	m_ViewYaw = -26.5f;

	m_bInScene = false;
}

void CCameraCtrl::Update(float dtime)
{
	float speed = 300.0f;
	if(IsKeyDown(VK_SHIFT))
		speed = 0.0f;

	float vel = speed * dtime;

	Ogre::Vector3 right, ahead;
	right.x = Ogre::Cos(m_ViewPitch);
	right.y = 0;
	right.z = -Ogre::Sin(m_ViewPitch);
	ahead.x = Ogre::Sin(m_ViewPitch) * Ogre::Cos(m_ViewYaw);
	ahead.y = Ogre::Sin(m_ViewYaw);
	ahead.z = Ogre::Cos(m_ViewPitch) * Ogre::Cos(m_ViewYaw);

	if(m_bIsKeyTrans)
	{
		if(IsKeyDown('W'))		m_Viewpoint += ahead*vel;
		if(IsKeyDown('S'))		m_Viewpoint -= ahead*vel;
		if(IsKeyDown('A'))		m_Viewpoint -= right*vel;
		if(IsKeyDown('D'))		m_Viewpoint += right*vel;
		if(IsKeyDown('Q'))		m_Viewpoint.y += vel;
		if(IsKeyDown('E'))		m_Viewpoint.y -= vel;
	}
}

//OnMouseWheel
void CCameraCtrl::ScaleTrans(int step)
{
	if(m_bProj == true)
	{	
		Ogre::Vector3 right, ahead;
		right.x = Ogre::Cos(m_ViewPitch);
		right.y = 0;
		right.z = -Ogre::Sin(m_ViewPitch);
		ahead.x = Ogre::Sin(m_ViewPitch) * Ogre::Cos(m_ViewYaw);
		ahead.y = Ogre::Sin(m_ViewYaw);
		ahead.z = Ogre::Cos(m_ViewPitch) * Ogre::Cos(m_ViewYaw);

		float speed = 0.09f * step;
		m_Viewpoint += ahead * speed;
	}
	else
	{
		m_fOrthoSize -= step * 0.05f;;
	}
}

void CCameraCtrl::MouseMoveTrans(float x, float y)
{
	//TRACE("Mouse Move\n");
	m_ViewPitch += x * 0.5f;
	m_ViewYaw -= y * 0.5f;

	if(m_ViewPitch < 0) m_ViewPitch += 360.0f;
	if(m_ViewPitch > 360.0f) m_ViewPitch -= 360.0f;
	if(m_ViewYaw < -80.0f) m_ViewYaw = -80.0f;
	if(m_ViewYaw > 80.0f) m_ViewYaw = 80.0f;

}

float CCameraCtrl::GetMainNodeToCameraDis()
{
	return m_fDis;
}

void CCameraCtrl::SetMainNodePos(Ogre::Vector3& pos)
{
	m_mainNodePos = pos;
}

Ogre::Vector3 CCameraCtrl::ComputeDirection()
{
	Ogre::Vector3 direction;
	direction.x = Ogre::Cos(m_ViewYaw) * Ogre::Sin(m_ViewPitch);
	direction.y = Ogre::Sin(m_ViewYaw);
	direction.z = Ogre::Cos(m_ViewYaw) * Ogre::Cos(m_ViewPitch);

	Ogre::Normalize(direction);
	return direction;
}

//alt旋转
void CCameraCtrl::MouseRollTrans(float x, float y)
{
	Ogre::Vector3 vp = m_Viewpoint,  aixs;
	float vel = 0.2f;

	m_Viewpoint -= m_mainNodePos;

	Ogre::Vector3 ahead;
	ahead.x = Ogre::Sin(m_ViewPitch) * Ogre::Cos(m_ViewYaw);
	ahead.y = Ogre::Sin(m_ViewYaw);
	ahead.z = Ogre::Cos(m_ViewPitch) * Ogre::Cos(m_ViewYaw);

	Ogre::Vector3 top;
	/*
	top.x = Ogre::Sin(m_ViewPitch)*Ogre::Cos(m_ViewYaw - 90);
	top.y = Ogre::Sin(m_ViewYaw- 90);
	top.z = Ogre::Cos(m_ViewPitch)*Ogre::Cos(m_ViewYaw- 90);
	*/
	top.x =	Ogre::Sin(m_ViewPitch) * Ogre::Cos(m_ViewYaw + 90);
	top.y = Ogre::Sin(m_ViewYaw + 90);
	top.z = Ogre::Cos(m_ViewPitch) * Ogre::Cos(m_ViewYaw + 90);

	Ogre::Vector3 right;
	/*
	right.x = Ogre::Cos(m_ViewPitch);
	right.y = 0;
	right.z = -Ogre::Sin(m_ViewPitch);
	*/
	right.x = Ogre::Sin(m_ViewPitch);
	right.y = 0;
	right.z = Ogre::Cos(m_ViewPitch);

	Ogre::Matrix4 rot;
	rot.makeRotateY(x * vel);
	m_ViewPitch += x * vel;
	m_Viewpoint = m_Viewpoint * rot; 

	Ogre::Ray ray;
	ray.m_Dir = ahead;
	ray.m_Origin = m_Viewpoint;
	ray.m_Range = 10000.0f;

	float r = 0;
	Ogre::Plane plan(Ogre::Vector3(0.0f, 1.0f, 0.0f), 0.0f);

	aixs.x = m_Viewpoint.z;
	aixs.y = 0;
	aixs.z = -m_Viewpoint.x;
	Ogre::Normalize(aixs);
	if(aixs.x == 0.0f && aixs.z == 0.0f)
	{
		aixs.x = -Ogre::Cos(m_ViewPitch);
		aixs.z = -Ogre::Sin(m_ViewPitch);
	}

	rot.makeRotateMatrix(aixs, -y * vel);

	if(m_ViewYaw - y * vel > - 80.0f && m_ViewYaw - y * vel < 80)
	{
		m_ViewYaw -= y * vel;
		m_Viewpoint = m_Viewpoint * rot;
	}

	m_Viewpoint += m_mainNodePos;
}

void CCameraCtrl::MouseShiftTrans(float x, float y)
{
	//TRACE("Mouse Shift\n");
	Ogre::Vector3 right, ahead, top;
	float vel = 0.001f;
	float r = m_Viewpoint.length();

	right.x = Ogre::Cos(m_ViewPitch);
	right.y = 0;
	right.z = -Ogre::Sin(m_ViewPitch);

	ahead.x = Ogre::Sin(m_ViewPitch) * Ogre::Cos(m_ViewYaw);
	ahead.y = Ogre::Sin(m_ViewYaw);
	ahead.z = Ogre::Cos(m_ViewPitch) * Ogre::Cos(m_ViewYaw);

	top.x = Ogre::Sin(m_ViewPitch)*Ogre::Cos(m_ViewYaw - 90);
	top.y = Ogre::Sin(m_ViewYaw- 90);
	top.z = Ogre::Cos(m_ViewPitch)*Ogre::Cos(m_ViewYaw- 90);


	m_Viewpoint -= right * vel * x * r;
	m_Viewpoint -= top * vel * y * r;

}

bool CCameraCtrl::IsKeyDown(int key)
{
	return (GetAsyncKeyState(key)&0xff00)!=0; 
}

void CCameraCtrl::UpdateCameraNode(Ogre::Camera* pCameraNode)
{

	static Ogre::Vector3 add(1.0f, 1.0f, 1.0f);
	Ogre::Vector3 dir;
	dir.y = Ogre::Sin(m_ViewYaw);
	dir.x = Ogre::Sin(m_ViewPitch)*Ogre::Cos(m_ViewYaw);
	dir.z = Ogre::Cos(m_ViewPitch)*Ogre::Cos(m_ViewYaw);

	//	m_Viewpoint += add;
	pCameraNode->setLookDirect(m_Viewpoint, dir, Ogre::Vector3(0, 1.0f, 0));

	if(m_bProj)
	{
		pCameraNode->setFov(45.0f);
		pCameraNode->setDepth(1.0f, 20000.0f);
	}
	else
	{
		pCameraNode->setOrtho(m_fOrthoSize);
		pCameraNode->setDepth(1.0f, 20000.0f);
	}

	m_fDis = sqrt((m_Viewpoint.x - m_mainNodePos.x) * (m_Viewpoint.x - m_mainNodePos.x) +
		(m_Viewpoint.y - m_mainNodePos.y) * (m_Viewpoint.y - m_mainNodePos.y) +
		(m_Viewpoint.z - m_mainNodePos.z) * (m_Viewpoint.z - m_mainNodePos.z));

	pCameraNode->update(0);
}

void CCameraCtrl::FontView()
{
	m_fOrthoSize = 1000.0f;
	m_ViewYaw = 0.0f;
	m_ViewPitch = 0.0f;

	m_Viewpoint = Ogre::Vector3(0.0f, 0.0f, -100.0f);

}

void CCameraCtrl::LeftView()
{
	m_fOrthoSize = 1000.0f;
	m_ViewYaw = 0.0f;
	m_ViewPitch = 90.0f;

	m_Viewpoint = Ogre::Vector3(-100.0f, 0.0f, 0.0f);
}

void CCameraCtrl::TopView()
{
	m_fOrthoSize = 1000.0f;
	m_ViewYaw = -90.0f;
	m_ViewPitch = 0.0f;

	m_Viewpoint = Ogre::Vector3(0.0f, 100.0f, 00.0f);
}

void CCameraCtrl::SeeFortyFive()
{
	if(m_bInScene)
	{
		m_fOrthoSize = 1000.0f;
		m_ViewYaw = -45.0f;
		m_ViewPitch = 0.0f;

		//		m_Viewpoint.setElement(6000.0f, 600.0f, 6000.0f);
	}
	else
	{
		m_fOrthoSize = 1000.0f;

		m_ViewPitch = 0.0f;
		m_ViewYaw = -45.0f;
	}
}