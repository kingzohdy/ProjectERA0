
#include ".\mcamera.h"
#include "OgreCamera.h"
#include "OgreGameScene.h"
#include "OgreRay.h"

#include "GTitle.h"
#include "MTerrainEdit.h"

using namespace TE;
using namespace Ogre;

extern float g_fTileSize;
MCamera    g_Camera;

MCamera::MCamera(void):m_Fov(60.0f), m_Aspect(800.0f/600.0f), m_NearPlane(1.0f),m_FarPlane(10000.0f), m_bEnable(true), m_bTopView(false)
{
	m_pCurModel = NULL;
	m_pCameraNode = NULL;
	m_CameraType = AIRCRAFT;
	m_MouseMoveType = ROATE_CAMERA;

	m_bMouseMoveCamera = false;
	m_bBeginMove = false;

	m_Pos   = MVec3(0.0f, 0.0f, -1.0f);
	m_ViewPitch = 0;
	m_ViewYaw = -60.0f;

	m_ScreenWidth = 800;
	m_ScreenHeight = 600;

	m_CameraVec = 2.0f;
}

MCamera::~MCamera(void)
{
}

void MCamera::MouseMoveCamera()
{
	int x,y;

	POINT ptCursor; 
	GetCursorPos( &ptCursor );
	ScreenToClient(m_PreviewHWND,&ptCursor);

	x = ptCursor.x ;	
	y = ptCursor.y ;

   if(m_mouseY!=y||m_mouseX!=x)			
   {			
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		if(m_MouseMoveType == ROATE_CAMERA)
		{
			float t1 = 180.0f*float( x - m_mouseX)/m_ScreenWidth;
			float t2 = 180.0f*float( y - m_mouseY)/m_ScreenHeight;

			m_ViewPitch += t1;
			m_ViewYaw -= t2;

			if(m_ViewPitch < 0) m_ViewPitch += 360.0f;
			if(m_ViewPitch > 360.0f) m_ViewPitch -= 360.0f;
			if(m_ViewYaw < -80.0f) m_ViewYaw = -80.0f;
			if(m_ViewYaw > 80.0f) m_ViewYaw = 80.0f;

			m_mouseX = x;
			m_mouseY = y;
		}
		else if(m_MouseMoveType == MOVE_CAMERA)
		{
			MVec3 dpos = dx*m_dx + dy*m_dy;
			//dpos.y = 0;
			m_Pos -= dpos;
			//m_Pos -= dx*m_dx + dy*m_dy;
			
			m_mouseX = x;
			m_mouseY = y;
			//float moveX = -dx*m_CameraVec;
			//float moveZ = -dy*m_CameraVec;

			//if( moveX != 0) Strafe( moveX );
			//if( moveZ < 0 ) MoveForward( -moveZ );
			//else if( moveZ > 0) MoveBack( moveZ);
		}
   }
}

//设置镜头位置	
void  MCamera::SetPosition(float x,float y,float z)
{  
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	Update();
};

void MCamera::setTarget(float x, float y, float z)
{
	MVec3 dir, up, right;
	getCameraAxis(dir, up, right);

	WorldRay ray;
	ray.m_Origin = m_Pos;
	ray.m_Dir = dir;
	float dist;
	if(g_Terrain.pickGround(ray, &dist))
	{
		m_Pos += Vector3(x,y,z) - (m_Pos+dist*dir);
	}

	Update();
}

void MCamera::getCameraAxis(MVec3 &dir, MVec3 &up, MVec3 &right)
{
	dir.y = Sin(m_ViewYaw);
	dir.x = Sin(m_ViewPitch)*Cos(m_ViewYaw);
	dir.z = Cos(m_ViewPitch)*Cos(m_ViewYaw);

	right = MVec3( Cos(m_ViewPitch),0.0f,-Sin(m_ViewPitch) );

	Normalize(right);
	up = CrossProduct(dir, right);
}

void MCamera::reset()
{
	m_Pos = MVec3(0, 500.0f, -500.0f);
	m_ViewPitch = 0;
	m_ViewYaw = -60.0f;
}

//镜头更新
void MCamera::Update()
{
	if( m_bMouseMoveCamera )
	{
		MouseMoveCamera();
	}

	Vector3 dir, up, right;
	getCameraAxis(dir, up, right);

	if( m_pCurModel && m_MouseMoveType == ROATE_CAMERA && m_bBeginMove&& m_bMouseMoveCamera)
	{
		MVec3  curPos = m_Pos - m_pCurModel->GetModelData ()->pos ;

		float  length = curPos.length ();

		m_Pos = m_pCurModel->GetModelData ()->pos - dir*length ;
	}

	if(m_bEnable && g_pCamera)
	{
		if(m_bTopView)
		{
			g_pCamera->setFov(10.0f);
			//g_pCamera->setOrtho(m_OrthSize);
			g_pCamera->setLookDirect(m_Pos, Vector3(0, -1.0f, 0), Vector3(0, 0, 1.0f));
		}
		else
		{
			g_pCamera->setFov(m_Fov);
			g_pCamera->setLookDirect(m_Pos, dir, up);
		}
	}
};

//在地形上移动
void MCamera::StrafeTerrain(float units,MTerrain* terrain,float height)
{    
	if( m_pCameraNode == NULL )
		return;

	Vector3 dir, up, right;
	getCameraAxis(dir, up, right);

	if( m_CameraType == LAND )
	{
		m_Pos += MVec3(right.x, 0.0f, right.z) * units;

		terrain->GetHeight(m_Pos.x , m_Pos.z ,&m_Pos.y);
		m_Pos.y += height;
	}

	if( m_CameraType == AIRCRAFT )
	{
		m_Pos += right * units;
	}

	if(m_pCameraNode) m_pCameraNode->setPosition(m_Pos);
}

//向前移动; 参数( 移动距离)	
void MCamera::MoveForward(float units)
{
	Vector3 dir, up, right;
	getCameraAxis(dir, up, right);

	m_Pos += MVec3(dir.x, 0.0f, dir.z) * units;

	m_pCameraNode->setPosition(m_Pos);
};

//向后移动; 参数( 移动距离 )	
void MCamera::MoveBack(float units)
{
	Vector3 dir, up, right;
	getCameraAxis(dir, up, right);

   	m_Pos -= MVec3(dir.x, 0.0f, dir.z) * units;
	m_pCameraNode->setPosition(m_Pos);
};
//
void MCamera::Strafe(float units)
{
	Vector3 dir, up, right;
	getCameraAxis(dir, up, right);

	if( m_CameraType == LAND )
		m_Pos += MVec3(right.x, 0.0f, right.z) * units;

	if( m_CameraType == AIRCRAFT )
	{
		m_Pos += right * units;
	}

   m_pCameraNode->setPosition(m_Pos);
}


//行走Z	
void  MCamera::WalkZ(float units)
{
	m_Pos.z += units;
   m_pCameraNode->setPosition(m_Pos);
};

//在地形上行走
void MCamera::WalkTerrain(float units,MTerrain* terrain,float height)
{ 
	if(GetAsyncKeyState(VK_CONTROL) & 0xff00)
	{
		units /= 5.0f;
	}

	Vector3 dir, up, right;
	getCameraAxis(dir, up, right);

	if( m_CameraType == LAND )
	{
		m_Pos += MVec3(dir.x, 0.0f, dir.z) * units;
		g_Terrain.GetHeight(m_Pos.x, m_Pos.z, &m_Pos.y);
		m_Pos.y += height ;
	}

	if( m_CameraType == AIRCRAFT )
	{
		m_Pos += dir * units;
	}

   m_pCameraNode->setPosition(m_Pos);
};
		
void MCamera::Pitch(float angle)
{
	m_ViewYaw += angle;
	Update();
}; 
	
void MCamera::Yaw(float angle)
{
	m_ViewPitch += angle;
	Update();
};   
	
void MCamera::Roll(float angle)
{

};  
//根据镜头更新当前射线	
void  MCamera::RefreshRay(HWND hwnd)
{
    POINT ptCursor;
      
	GetCursorPos( &ptCursor );
	ScreenToClient( hwnd , &ptCursor );

	WorldRay wray;
	g_pCamera->getViewRayByScreenPt( &wray, ptCursor.x, ptCursor.y );
	wray.getRelativeRay(m_Ray,Ogre::WorldPos(0,0,0));
    
	/*
	MMatrix mat = m_pCameraNode->getProjectMatrix();
	
	MVec3 v;
	
	v.x =  ( ( ( 2.0f * ptCursor.x ) / m_ScreenWidth  ) - 1 ) / mat._11;  
	v.y = -( ( ( 2.0f * ptCursor.y ) / m_ScreenHeight ) - 1 ) / mat._22;
	v.z =  1.0f;

	MMatrix matWorld,m,matView;
	matWorld.identity ();
	matView = m_pCameraNode->getViewMatrix ();
	m = matWorld * matView;   
	m.inverse ();

	// Transform the screen space pick ray into 3D space    
	m_Ray.dir .x  = v.x*m._11 + v.y*m._21 + v.z*m._31;        
	m_Ray.dir .y= v.x*m._12 + v.y*m._22 + v.z*m._32;       
	m_Ray.dir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
      
	m_Ray.pos .x = m._41;      
	m_Ray.pos .y = m._42;  
	m_Ray.pos.z = m._43;
	*/

};

void MCamera::SetOverView()
{
	if(m_bTopView)
	{
		popSaveData();
		m_bTopView = false;
		return;
	}

	m_bTopView = true;
	saveData();

	float w = g_TerrainEdit.GetEditData()->gw*TERRBLOCK_SIZE;
	float h = g_TerrainEdit.GetEditData()->gw*TERRBLOCK_SIZE;
	SetPosition(w/2.0f, 200000.0f, h/2.0f);

	m_ViewYaw = -90.0f;
	m_ViewPitch = 0.0f;
	m_OrthSize = w;
};

void MCamera::enable()
{
	m_bEnable = true;
}

void MCamera::disable()
{
	m_bEnable = false;
}

void MCamera::saveData()
{
	m_SaveData.pos = m_Pos;
	m_SaveData.pitch = m_ViewPitch;
	m_SaveData.yaw = m_ViewYaw;
	m_SaveData.znear = m_NearPlane;
	m_SaveData.zfar = m_FarPlane;
	m_SaveData.fov = m_Fov;
}

void MCamera::popSaveData()
{
	m_Pos = m_SaveData.pos;
	m_ViewPitch = m_SaveData.pitch;
	m_ViewYaw = m_SaveData.yaw;
	m_NearPlane = m_SaveData.znear;
	m_FarPlane = m_SaveData.zfar;
	m_Fov = m_SaveData.fov;
}

void MCamera::SetMouseMoveInitPos(int x,int y)
{
	m_initmoveposX = x;
	m_initmoveposZ = y;
	Ogre::WorldRay viewray;
	g_pCamera->getViewRayByScreenPt(&viewray,x,y);
	Ogre::Ray ray;
	viewray.getRelativeRay(ray,Ogre::WorldPos(0,0,0));

	Ogre::WorldRay viewray2;
	g_pCamera->getViewRayByScreenPt(&viewray2,x+2,y+2);
	Ogre::Ray ray2;
	viewray2.getRelativeRay(ray2,Ogre::WorldPos(0,0,0));

	Ogre::Plane plane(MVec3(0.0f,1.0f,0.0f),10000.0f);
	float dis;
	ray.intersectPlane(&plane,&dis);
	MVec3 m_MoveCenterpos =ray.m_Origin + dis*ray.m_Dir;

	Ogre::Matrix4 mCameraView = g_pCamera->getViewMatrix();
	MVec3 vdir    = MVec3(mCameraView._13,mCameraView._23,mCameraView._33);
	MVec3 vup	  = MVec3(mCameraView._12,mCameraView._22,mCameraView._32);
	MVec3 vbi	  = MVec3(mCameraView._11,mCameraView._21,mCameraView._31);

	Ogre::Plane plane2(-vdir,m_MoveCenterpos);
	float dis2;
	ray2.intersectPlane(&plane2,&dis2);
	MVec3 m_Movepos =ray2.m_Origin + dis2*ray2.m_Dir;

	MVec3 delpos  = m_Movepos - m_MoveCenterpos;
	MVec3 delposy = m_Movepos - m_MoveCenterpos;
	float dup = DotProduct(delposy,vup);
	float dbi = DotProduct(delpos,vbi);

	m_dx	=	dbi*vbi/2.0f;
	m_dy	=	dup*vup/2.0f;

	

}


