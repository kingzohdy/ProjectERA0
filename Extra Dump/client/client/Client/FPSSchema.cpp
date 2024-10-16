#include <Windows.h>
#include "FPSSchema.h"
#include "GameKeyMoveOperate.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "OgreMath.h"
#include "GameMap.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "GamePlayManager.h"
#include "../UILib/ui_framemgr.h"
#include "../UILib/ui_frame.h"

FPSCameraUpdate fpsCameraUpdate;

extern FrameManager *g_pFrameMgr;

static int cameraDist = 0;
static float cameraTimes = 1;

void fps_open(float times, int dist)
{
	cameraDist = dist;
	cameraTimes = times;
	RECT curRect;
	::GetWindowRect((HWND)(g_pFrameMgr->getWndHandle()), &curRect);
	POINT curPoint;
	curPoint.x = (curRect.right + curRect.left) / 2;
	curPoint.y = (curRect.bottom + curRect.top) / 2;
	::SetCursorPos(curPoint.x, curPoint.y);
	GameKeyMoveOperate::getSingleton().SetFPS();
	GamePlayManager::getSingleton().getMainCamera()->insertObserver(&fpsCameraUpdate);

	while(ShowCursor(false) >= 0);
}

void fps_close()
{
	GameKeyMoveOperate::getSingleton().SetFPS(false);

	while(ShowCursor(true) < 0);
}

bool FPSCameraUpdate::process(FollowCamera *camera, unsigned int tick)
{
	if (GameKeyMoveOperate::getSingleton().IsFPS())
	{
		unsigned int now = timeGetTime();
		bool isInputing = stricmp(g_pFrameMgr->GetFocusFrame() ? g_pFrameMgr->GetFocusFrame()->GetTypeName() : "", "EditBox") == 0;
		if (!isInputing && g_pFrameMgr->getWndHandle() == ::GetForegroundWindow())
		{
			if (now >= m_uLastRotateTime + ROTATE_PRE)
			{
				int nHeight = 0;
				int nAngleH = 0;
				int nAngleV = 0;
				if (GetKeyState(KEY_CAMERA_UP) & 0xFF00)
				{
					nHeight += 1;
				}
				if (GetKeyState(KEY_CAMERA_DOWN) & 0xFF00)
				{
					nHeight -= 1;
				}
				if (GetKeyState(KEY_CAMERA_LEFTR) & 0xFF00)
				{
					nAngleH -= 1;
				}
				if (GetKeyState(KEY_CAMERA_RIGHTR) & 0xFF00)
				{
					nAngleH += 1;
				}
				if (GetKeyState(KEY_CAMERA_UPR) & 0xFF00)
				{
					nAngleV += 1;
				}
				if (GetKeyState(KEY_CAMERA_DOWNR) & 0xFF00)
				{
					nAngleV -= 1;
				}
				if (nHeight != 0)
				{
					m_nOrigin += nHeight * int((now - m_uLastRotateTime) * (PLUMB_SPEED / 1000));
				}
				if (nAngleH != 0)
				{
					m_angleH += nAngleH * fmod((now - m_uLastRotateTime) * (ROTATE_SPEED * cameraTimes / 1000), 360);
				}
				if (nAngleV != 0)
				{
					m_angleV += nAngleV * fmod((now - m_uLastRotateTime) * (ROTATE_SPEED * cameraTimes / 1000), 360);
				}
				m_uLastRotateTime = now;
			}
			RECT curRect;
			::GetWindowRect((HWND)(g_pFrameMgr->getWndHandle()), &curRect);
			POINT curPoint, beforePoint;
			curPoint.x = (curRect.right + curRect.left) / 2;
			curPoint.y = (curRect.bottom + curRect.top) / 2;
			::GetCursorPos(&beforePoint);
			::SetCursorPos(curPoint.x, curPoint.y);

			m_angleH -= (curPoint.x - beforePoint.x) * 45 / (curRect.right - curRect.left);
			m_angleV += (curPoint.y - beforePoint.y) * 45 / (curRect.bottom - curRect.top);
			if (m_angleV > 89)
			{
				m_angleV = 89;
			}
			else if (m_angleV < -89)
			{
				m_angleV = -89;
			}
		}
		camera->SetAngle(m_angleH);
		Ogre::WorldPos targetPos;
		targetPos = ActorManager::getSingleton().getMainPlayer()->GetPosition();
		targetPos.y += ActorManager::getSingleton().getMainPlayer()->getOverHead() + m_nOrigin - int(Ogre::Sin(m_angleV) * cameraDist);
		targetPos.x -= int(Ogre::Cos(m_angleV) * Ogre::Sin(m_angleH) * cameraDist);
		targetPos.z -= int(Ogre::Cos(m_angleV) * Ogre::Cos(m_angleH) * cameraDist);

		camera->setGround( ActorManager::getSingleton().getMainPlayer()->getCurMap()->getShowGameMap() );
		Ogre::Vector3 dir;
		dir.y = Ogre::Sin(m_angleV);
		dir.x = Ogre::Cos(m_angleV) * Ogre::Sin(m_angleH);
		dir.z = Ogre::Cos(m_angleV) * Ogre::Cos(m_angleH);
		camera->GetCameraNode()->setLookDirect(targetPos, dir, Ogre::Vector3(0,1.0f,0));
		camera->GetCameraNode()->update(tick);
		camera->updateCamera(targetPos, 0);
		return true;
	}
	return false;
}

FPSCameraUpdate::FPSCameraUpdate()
{
	m_angleV = m_angleH = 0;
	m_nOrigin = 2000;
	m_uLastRotateTime = timeGetTime();
}
