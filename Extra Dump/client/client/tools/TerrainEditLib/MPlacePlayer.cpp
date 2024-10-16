#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include "EnvRegion.h"
#include "MPlacePlayer.h"
#include "EditorCamera.h"

#include "OgreModel.h"
#include "OgreLight.h"
#include "OgrePhysicsScene.h"
#include "OgreRay.h"
#include   <algorithm>   

using namespace Ogre;

extern uint g_dwCurTime;
extern std::string  g_path;
extern Light *g_plightnode;
const  float PI = 3.1415f;

namespace TE
{
    MPlacePlayer::MPlacePlayer(void) : m_bGo(false), m_bInterpolateMode(true), m_CurRegionIndex(0)
    {
        m_nProcessType = PLACE_PLYAER;
		m_pPlayer = new Entity;
		m_pPlayer->load("character\\monster\\1001\\1001.omod", true);
		m_pPlayer->setInstanceAmbient(ColourValue(0.23f, 0.23f, 0.23f, 0));
		m_pPlayer->setUserType(1);

		m_pCamera = new FollowCamera;
    }

    MPlacePlayer::~MPlacePlayer(void)
    {
		delete m_pCamera;

		if(m_pPlayer->getScene()) m_pPlayer->detachFromScene();
        OGRE_RELEASE(m_pPlayer);
    }

    void MPlacePlayer::MsgProc(MTerrainEdit* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam)
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
			m_pPlayer->playAnim(1011);
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

    void MPlacePlayer::Update(MTerrainEdit* pEditor , float dtime)
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
				m_pPlayer->playAnim(1001);
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
		Vector3 center = m_vCurPos;
		center.y += 100.0f;
		m_pCamera->updateCamera(g_pCamera, center, dtick);
    }

    void MPlacePlayer::Init(MTerrainEdit* pEditor)
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

    void MPlacePlayer::Release()
    {
		m_pPlayer->detachFromScene();
		g_Camera.enable();
    }
}