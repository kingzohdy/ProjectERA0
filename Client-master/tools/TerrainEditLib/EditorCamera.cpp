
#include "EditorCamera.h"
#include "OgreCamera.h"
#include "OgreGameScene.h"
#include "OgreRay.h"

#include "TerrainTile.h"
#include "TerrainEdit.h"

using namespace Ogre;

namespace TE
{
	extern float g_fTileSize;
	EditorCamera    g_Camera;

	EditorCamera::EditorCamera(void):m_Fov(60.0f), m_Aspect(800.0f/600.0f), m_NearPlane(1.0f),m_FarPlane(10000.0f), m_bEnable(true), m_bTopView(false)
	{
		m_pCurModel = NULL;
		m_pCameraNode = NULL;
		m_CameraType = AIRCRAFT;
		m_MouseMoveType = ROATE_CAMERA;

		m_bMouseMoveCamera = false;
		m_bBeginMove = false;

		m_Pos   = Ogre::Vector3(0.0f, 0.0f, -1.0f);
		m_ViewPitch = 0;
		m_ViewYaw = -60.0f;

		m_ScreenWidth = 800;
		m_ScreenHeight = 600;

		m_CameraVec = 2.0f;
	}

	EditorCamera::~EditorCamera(void)
	{
	}

	void EditorCamera::MouseMoveCamera()
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
				Ogre::Vector3 dpos = dx*m_dx + dy*m_dy;
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
	void  EditorCamera::SetPosition(float x,float y,float z)
	{  
		m_Pos.x = x;
		m_Pos.y = y;
		m_Pos.z = z;

		Update();
	};

	void EditorCamera::setTarget(float x, float y, float z)
	{
		Ogre::Vector3 dir, up, right;
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

	void EditorCamera::getCameraAxis(Ogre::Vector3 &dir, Ogre::Vector3 &up, Ogre::Vector3 &right)
	{
		dir.y = Sin(m_ViewYaw);
		dir.x = Sin(m_ViewPitch)*Cos(m_ViewYaw);
		dir.z = Cos(m_ViewPitch)*Cos(m_ViewYaw);

		right = Ogre::Vector3( Cos(m_ViewPitch),0.0f,-Sin(m_ViewPitch) );

		Normalize(right);
		up = CrossProduct(dir, right);
	}

	void EditorCamera::reset()
	{
		m_Pos = Ogre::Vector3(0, 500.0f, -500.0f);
		m_ViewPitch = 0;
		m_ViewYaw = -60.0f;
	}

	//镜头更新
	void EditorCamera::Update()
	{
		if( m_bMouseMoveCamera )
		{
			MouseMoveCamera();
		}

		Vector3 dir, up, right;
		getCameraAxis(dir, up, right);

		if( m_pCurModel && m_MouseMoveType == ROATE_CAMERA && m_bBeginMove&& m_bMouseMoveCamera)
		{
			Ogre::Vector3  curPos = m_Pos - m_pCurModel->GetModelData ()->pos ;

			float  length = curPos.length ();

			m_Pos = m_pCurModel->GetModelData ()->pos - dir*length ;
		}

		if(m_bEnable && g_pCamera)
		{
			if(m_bTopView)
			{
				g_pCamera->setOrtho(m_OrthSize);
				g_pCamera->setLookDirect(m_Pos, Vector3(0, -1.0f, 0), Vector3(0, 0, 1.0f));
				g_pCamera->update(0);
			}
			else
			{
				g_pCamera->setFov(m_Fov);
				g_pCamera->setLookDirect(m_Pos, dir, up);
			}
		}
	};

	//在地形上移动
	void EditorCamera::StrafeTerrain(float units,Terrain* terrain,float height)
	{    
		if( m_pCameraNode == NULL )
			return;

		Vector3 dir, up, right;
		getCameraAxis(dir, up, right);

		if( m_CameraType == LAND )
		{
			m_Pos += Ogre::Vector3(right.x, 0.0f, right.z) * units;

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
	void EditorCamera::MoveForward(float units)
	{
		Vector3 dir, up, right;
		getCameraAxis(dir, up, right);

		m_Pos += Ogre::Vector3(dir.x, 0.0f, dir.z) * units;

		m_pCameraNode->setPosition(m_Pos);
	};

	//向后移动; 参数( 移动距离 )	
	void EditorCamera::MoveBack(float units)
	{
		Vector3 dir, up, right;
		getCameraAxis(dir, up, right);

		m_Pos -= Ogre::Vector3(dir.x, 0.0f, dir.z) * units;
		m_pCameraNode->setPosition(m_Pos);
	};
	//
	void EditorCamera::Strafe(float units)
	{
		Vector3 dir, up, right;
		getCameraAxis(dir, up, right);

		if( m_CameraType == LAND )
			m_Pos += Ogre::Vector3(right.x, 0.0f, right.z) * units;

		if( m_CameraType == AIRCRAFT )
		{
			m_Pos += right * units;
		}

		m_pCameraNode->setPosition(m_Pos);
	}


	//行走Z	
	void  EditorCamera::WalkZ(float units)
	{
		m_Pos.z += units;
		m_pCameraNode->setPosition(m_Pos);
	};

	//在地形上行走
	void EditorCamera::WalkTerrain(float units,Terrain* terrain,float height)
	{ 
		if(GetAsyncKeyState(VK_CONTROL) & 0xff00)
		{
			units /= 5.0f;
		}

		Vector3 dir, up, right;
		getCameraAxis(dir, up, right);

		if( m_CameraType == LAND )
		{
			m_Pos += Ogre::Vector3(dir.x, 0.0f, dir.z) * units;
			g_Terrain.GetHeight(m_Pos.x, m_Pos.z, &m_Pos.y);
			m_Pos.y += height ;
		}

		if( m_CameraType == AIRCRAFT )
		{
			if(m_bTopView)
			{
				float t = units/10000.0f;
				if(t > 0.5f) t = 0.5f;
				if(t < -0.5f) t = -0.5f;
				m_OrthSize *= 1-t;
			}
			else m_Pos += dir * units;
		}

		m_pCameraNode->setPosition(m_Pos);
	};

	void EditorCamera::Pitch(float angle)
	{
		m_ViewYaw += angle;
		Update();
	}; 

	void EditorCamera::Yaw(float angle)
	{
		m_ViewPitch += angle;
		Update();
	};   

	void EditorCamera::Roll(float angle)
	{

	};  
	//根据镜头更新当前射线	
	void  EditorCamera::RefreshRay(HWND hwnd)
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

	void EditorCamera::SetOverView()
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
		SetPosition(w/2.0f, 20000.0f, h/2.0f);

		m_ViewYaw = -90.0f;
		m_ViewPitch = 0.0f;
		m_OrthSize = w;
	};

	void EditorCamera::enable()
	{
		m_bEnable = true;
	}

	void EditorCamera::disable()
	{
		m_bEnable = false;
	}

	void EditorCamera::saveData()
	{
		m_SaveData.pos = m_Pos;
		m_SaveData.pitch = m_ViewPitch;
		m_SaveData.yaw = m_ViewYaw;
		m_SaveData.znear = m_NearPlane;
		m_SaveData.zfar = m_FarPlane;
		m_SaveData.fov = m_Fov;
	}

	void EditorCamera::popSaveData()
	{
		m_Pos = m_SaveData.pos;
		m_ViewPitch = m_SaveData.pitch;
		m_ViewYaw = m_SaveData.yaw;
		m_NearPlane = m_SaveData.znear;
		m_FarPlane = m_SaveData.zfar;
		m_Fov = m_SaveData.fov;
	}

	void EditorCamera::SetMouseMoveInitPos(int x,int y)
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

		Ogre::Plane plane(Ogre::Vector3(0.0f,1.0f,0.0f),10000.0f);
		float dis;
		ray.intersectPlane(&plane,&dis);
		Ogre::Vector3 m_MoveCenterpos =ray.m_Origin + dis*ray.m_Dir;

		Ogre::Matrix4 mCameraView = g_pCamera->getViewMatrix();
		Ogre::Vector3 vdir    = Ogre::Vector3(mCameraView._13,mCameraView._23,mCameraView._33);
		Ogre::Vector3 vup	  = Ogre::Vector3(mCameraView._12,mCameraView._22,mCameraView._32);
		Ogre::Vector3 vbi	  = Ogre::Vector3(mCameraView._11,mCameraView._21,mCameraView._31);

		Ogre::Plane plane2(-vdir,m_MoveCenterpos);
		float dis2;
		ray2.intersectPlane(&plane2,&dis2);
		Ogre::Vector3 m_Movepos =ray2.m_Origin + dis2*ray2.m_Dir;

		Ogre::Vector3 delpos  = m_Movepos - m_MoveCenterpos;
		Ogre::Vector3 delposy = m_Movepos - m_MoveCenterpos;
		float dup = DotProduct(delposy,vup);
		float dbi = DotProduct(delpos,vbi);

		m_dx	=	dbi*vbi/2.0f;
		m_dy	=	dup*vup/2.0f;
	}
}



