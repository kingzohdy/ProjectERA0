
#pragma once

#include <windows.h>
#include "TerrainEditTypes.h"
#include "mterrain.h"
#include "gtitle.h"

namespace TE
{
class MCamera 
{
public:
	MCamera(void);
	~MCamera(void);

	void reset();
	void Update();

	void  SetCamera(Ogre::Camera* node){ m_pCameraNode = node ;}
	void setTarget(float x, float y, float z);
	void  SetPosition(float x,float y,float z);
	MVec3 GetPosition(){return m_Pos;};

	void Strafe(float units); // left/right
	void StrafeTerrain(float units,MTerrain* terrain,float height);
	void WalkTerrain(float units,MTerrain* terrain,float height = 0);
	void WalkZ(float units);
	void MoveForward(float units);
	void MoveBack(float units);
	void RefreshRay(HWND hwnd);	
	void SetMousePos(int x,int y)
	{
		m_mouseX = x;
		m_mouseY = y;
	}

	void SetMouseMoveInitPos(int x,int y);
	void BindObject(TerrModel*  pModel)
	{
		m_pCurModel = pModel;
	}

	void SetOverView();

	void enable();
	void disable();
	void saveData();
	void popSaveData();

private:
	void MouseMoveCamera();
	void Pitch(float angle); // rotate on right vector
	void Yaw(float angle);   // rotate on up vector
	void Roll(float angle);  // rotate on look vector
	void getCameraAxis(MVec3 &dir, MVec3 &up, MVec3 &right);

public:
	enum CAMERATYPE
	{ 
		LAND, 
		AIRCRAFT
	};
	enum MOUSE_MOVE_TYPE
	{
		ROATE_CAMERA, 
		MOVE_CAMERA
	};

	struct CameraSaveData
	{
		MVec3 pos;
		float pitch;
		float yaw;
		float znear;
		float zfar;
		float fov;
	};

	bool m_bEnable;
	bool m_bTopView;
	CameraSaveData m_SaveData;

	CAMERATYPE m_CameraType;

	Ogre::Camera *m_pCameraNode;

	Ogre::Ray    m_Ray;//
	int    m_mouseX,m_mouseY;
	int   m_ScreenWidth,m_ScreenHeight;

	//移动镜头类型
	MOUSE_MOVE_TYPE             m_MouseMoveType;
    
	MVec3 m_Pos;
	float m_ViewPitch;
    float m_ViewYaw;

	MVec3 m_dx;
	MVec3 m_dy;
	int m_initmoveposX;
	int m_initmoveposZ;

	float m_Fov;
	float m_OrthSize;
	float m_Aspect;
	float m_NearPlane;
	float m_FarPlane;

	//是否鼠标移动镜头
	bool   m_bMouseMoveCamera;
	//是否开始移动
	bool   m_bBeginMove;
	//镜头绑定物体
	TerrModel*                   m_pCurModel;
    //镜头移动速度
	float                            m_CameraVec;

	HWND			m_PreviewHWND;
};

};

extern TE::MCamera    g_Camera;