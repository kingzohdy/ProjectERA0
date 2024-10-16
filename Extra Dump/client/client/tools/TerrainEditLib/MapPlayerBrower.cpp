
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include "EnvRegion.h"
#include "MapPlayerBrower.h"
#include "EditorCamera.h"

#include "OgreModel.h"
#include "OgreLight.h"
#include "OgrePhysicsScene.h"
#include "OgreRay.h"
#include "OgreResourceManager.h"
#include "OgreAnimTrackBone.h"
#include "OgreSkeleton.h"
#include "OgreCamera.h"
#include "AvatarEntity.h"
#include   <algorithm>   

using namespace Ogre;

namespace TE
{
	extern uint g_dwCurTime;
	extern std::string  g_path;
	extern Light *g_plightnode;
	const  float PI = 3.1415f;

	MapPlayerBrower::MapPlayerBrower(void) : m_bGo(false), m_bInterpolateMode(true), m_CurRegionIndex(0)
	{
		m_nProcessType = PLACE_PLYAER;

		AvatarEntity *pentity = new AvatarEntity;
		int attribs[24];
		memset(attribs, 0, sizeof(attribs));
		//attribs[0] = 2;
		//attribs[1] = 1;
		//attribs[2] = 2;
		//attribs[3] = 3;
		//attribs[4] = 2021601;
		//attribs[5] = 1181601;
		//attribs[6] = 1101601;
		//attribs[7] = 2121601;
		//attribs[8] = 2221601;
		//attribs[12] = 2429905;
		//attribs[13] = 3;

		attribs[0] = 1;
		attribs[4] = 2019900;
		/*attribs[6] = 1019900;*/
		pentity->updateAttrib(attribs);
		m_pPlayer = pentity;
		/*
		m_pPlayer = new Entity;
		m_pPlayer->load("character\\monster\\1001\\1001.omod", true);
		*/

		m_pPlayer->setInstanceAmbient(ColourValue(0.23f, 0.23f, 0.23f, 0));
		m_pPlayer->setUserType(1);
		m_pPlayer->playAnim(10100);

		m_pCamera = new FollowCamera(g_pCamera);
		m_pCamAnimMod = NULL;
	}

	MapPlayerBrower::~MapPlayerBrower(void)
	{
		delete m_pCamera;

		if(m_pPlayer->getScene()) m_pPlayer->detachFromScene();
		OGRE_RELEASE(m_pPlayer);
	}

	void MapPlayerBrower::MsgProc(TerrainEditMgr* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
	{
		if( WM_LBUTTONDOWN == msg )
		{
			pEditor->GetCamera()->RefreshRay((HWND)hWnd);
			Vector3 dir = pEditor->GetCamera()->m_Ray.m_Dir;
			Vector3 pos = pEditor->GetCamera()->m_Ray.m_Origin;

			g_Terrain.TestAndRay(dir , pos , m_vTargetPos);
			float dist = 0;


			if(g_Terrain.getPhysicsScene()->pick(Ray(pos,dir), &dist))
			{
				Vector3 t = pos + dir*dist;
				if(t.y > m_vTargetPos.y) m_vTargetPos = t;
			}

			m_RunLength = 0;
			m_StartPos = m_vCurPos;
			m_pPlayer->playAnim(10120);
			m_bGo = true;
		}
		else if(WM_KEYDOWN == msg)
		{
			switch(wParam)
			{
			case VK_F5:
				pEditor->SetCurEditEvent(pEditor->m_EditOption.mSaveCurEditEvent);
				break;
			case 'C':   ///< ²åÖµÄ£Ê½ÇÐ»»
				m_bInterpolateMode = !m_bInterpolateMode;
				break;
			case VK_ESCAPE:
				endCameraAnim();
				break;
			}
		}
		else if(WM_MOUSEWHEEL == msg)
		{
			float dist = -float(GET_WHEEL_DELTA_WPARAM(wParam))/WHEEL_DELTA * 400.0f;
			m_pCamera->PullInOut(dist);
		}
		else if(WM_MOUSEMOVE == msg)
		{
			static int PreMouseX = -1;
			int CurMouseX = (lParam&0xffff);

			if(wParam&MK_RBUTTON)
			{
				RECT rect;
				GetClientRect((HWND)hWnd, &rect);
				float angle = float(CurMouseX-PreMouseX)/(rect.right-rect.left) * 160.0f;
				m_pCamera->Rotate(angle);
			}

			PreMouseX = CurMouseX;
		}
	}

	void MapPlayerBrower::Update(TerrainEditMgr* pEditor , float dtime)
	{
		unsigned int dtick = TimeToTick(dtime);
		if(m_bGo)
		{
			m_RunLength += dtime*400.0f;
			Vector3 dpos = m_vTargetPos - m_StartPos;
			dpos.y = 0;
			float fulllen = dpos.length();

			if(m_RunLength >= fulllen)
			{
				m_bGo = false;
				m_pPlayer->playAnim(10100);
				m_vCurPos = m_vTargetPos;
			}
			else
			{
				dpos /= fulllen;
				m_vCurPos = m_StartPos + dpos*m_RunLength;
				g_Terrain.GetHeight(m_vCurPos.x , m_vCurPos.z , &m_vCurPos.y, TERR_PICK_VB_BSP);
				m_pPlayer->setPosition(m_vCurPos);

				Quaternion rot;
				rot.setRotateArc(Vector3(0,0,-1.0f), dpos);
				m_pPlayer->setRotation(rot);
			}
		}

		bool cam_updated = updateCameraAnim(dtick);

		size_t i;
		for(i=g_AreaSet.GetAreaSize(); i>1; i--)
		{
			BoxBound aabox = g_AreaSet.GetArea(i-1)->GetAABBox();
			if(aabox.isPointInXZ(m_vCurPos))
			{
				break;				
			}
		}
		i--;
		if(i != m_CurRegionIndex)
		{
			m_CurRegionIndex = i;
			AmbientInfo *pinfo = g_AreaSet.GetArea(i)->GetAmbientInfo();
			g_Terrain.m_pAmbientMgr->setCurAmbient(*pinfo, false);
		}

		m_pPlayer->update(dtick);

		if(!cam_updated)
		{
			Vector3 center = m_vCurPos;
			center.y += 100.0f;
			m_pCamera->updateCamera(center, dtick);
		}
	}

	void MapPlayerBrower::Init(TerrainEditMgr* pEditor)
	{
		if(m_pPlayer->getScene()) m_pPlayer->detachFromScene();

		m_vCurPos = pEditor->m_CurMousePos;
		m_pPlayer->setPosition(m_vCurPos);
		m_pPlayer->attachToScene(&g_Terrain);

		g_Camera.disable();
		m_pCamera->Reset(m_vCurPos);
		m_bGo = false;
		m_CurRegionIndex = 0;
	}

	void MapPlayerBrower::Release()
	{
		m_pPlayer->detachFromScene();
		g_Camera.enable();
	}

	void MapPlayerBrower::beginCameraAnim(const char *modelpath)
	{
		Resource *pres = ResourceManager::getSingleton().blockLoad(modelpath);
		if(pres==NULL || !IS_KIND_OF(ModelData, pres))
		{
			return;
		}

		m_pCamAnimMod  = new Model(static_cast<ModelData *>(pres));
		pres->release();
		m_pPlayer->show(false);
	}

	bool MapPlayerBrower::updateCameraAnim(unsigned int dtick)
	{
		if(m_pCamAnimMod == NULL)
		{
			return false;
		}

		static unsigned int s_CurTick = timeGetTime();
		dtick = timeGetTime() - s_CurTick;
		s_CurTick += dtick;
		m_pCamAnimMod->update(dtick);

		int boneid = m_pCamAnimMod->getSkeletonInstance()->findBoneID("Player01");
		if(boneid >= 0)
		{
			Matrix4 tm = m_pCamAnimMod->getSkeletonInstance()->getBoneTM(boneid);
			m_vCurPos = tm.getTranslate();
			m_pPlayer->setPosition(m_vCurPos);
			Ogre::Quaternion rot = m_pCamAnimMod->getSkeletonInstance()->getBoneRotation(boneid);
			m_pPlayer->setRotation(rot);
		}

		boneid = m_pCamAnimMod->getSkeletonInstance()->findBoneID("Camera01");
		if(boneid >= 0)
		{
			Matrix4 tm = m_pCamAnimMod->getSkeletonInstance()->getBoneTM(boneid);
			Vector3 dir(0, -1.0, 0), up(0, 0, 1.0f);
			tm.transformNormal(dir, dir);
			tm.transformNormal(up, up);
			g_pCamera->setLookDirect(tm.getTranslate(), dir, up);

			return true;
		}
		else
		{
			return false;
		}
	}

	void MapPlayerBrower::endCameraAnim()
	{
		m_pPlayer->show(true);

		OGRE_RELEASE(m_pCamAnimMod);
		ResourceManager::getSingleton().garbageCollect();
	}

	const float MY_PI = 3.1415926f;
	int FaceAngleFromVec(Vector3 vec)
	{
		float len = vec.length();
		if(len < 0.001f) return 0;

		double tmp = vec.x/len;
		double angle = acos(tmp) * 180 / MY_PI;
		if( vec.z > 0 ) angle = (360 - angle);

		return (int)angle;
	}

	int MapPlayerBrower::getPlayerDir()
	{
		Matrix4 tm = m_pPlayer->getWorldMatrix();	
		
		Vector3 vec;
		//tm.transformNormal(vec, Vector3(0, 0, -1.0f));
		tm.transformNormal(vec, Vector3(1.0f, 0, 0));
		vec.y = 0;

		int angle = FaceAngleFromVec(vec);
		return angle/2;		
	}

	int MapPlayerBrower::getCameraDir()
	{
		return int(m_pCamera->getAngleLR())/2;
	}
}