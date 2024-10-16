#pragma once

#include "OgreVector3.h"

class CCameraCtrl
{
public:
	CCameraCtrl(void);
	~CCameraCtrl(void);

	float m_ViewPitch;
	Ogre::Vector3 m_Viewpoint;
	float m_ViewYaw;
	bool m_bIsKeyTrans;
	float m_fOrthoSize;

	float m_fDis; //相机到MainNode的距离
	Ogre::Vector3 m_mainNodePos; //绕 MainNode 旋转，非地图中默认为(0,0,0)

	float GetMainNodeToCameraDis();
	void SetMainNodePos(Ogre::Vector3& pos);
	Ogre::Vector3 GetMainNodePos() { return m_mainNodePos; }
	void SetKeyTrans(bool trans);
	bool GetKeyTrans() { return m_bIsKeyTrans;};

	Ogre::Vector3 ComputeDirection();
	void ResetInScene(int nResId = 1002);
	void Reset();
	void Update(float dtime);
	void MouseMoveTrans(float x, float y);
	void MouseRollTrans(float x, float y);
	void MouseShiftTrans(float x, float y);
	void ScaleTrans(int step);
	bool IsKeyDown(int key);

	bool IsProjMode() { return m_bProj; };
	void SetProj(bool bProj) { m_bProj = bProj; };

	void UpdateCameraNode(Ogre::Camera* pCameraNode);

	void FontView();
	void LeftView();
	void TopView();

	void SeeFortyFive(); //45度视角
	//调整为45度角后，会根据角色所在的位置和原来相机与角色的距离计算新的viewpoint坐标
	void SetViewPoint( Ogre::Vector3 vViewPos) { m_Viewpoint = vViewPos; };

	bool m_bInScene; //在地图场景中
	bool m_bProj;
};
