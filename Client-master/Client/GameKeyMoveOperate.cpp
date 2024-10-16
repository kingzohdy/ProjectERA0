#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include "GameKeyMoveOperate.h"
#include "GameCamera.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GamePlayManager.h"
#include "GameCamera.h"
#include "proto_comm.h"
#include "../UILib/ui_framemgr.h"
#include "../UILib/ui_frame.h"
#include "OgreMath.h"
#include "GameMap.h"
#include "GameMaskMap.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_IControl.h"
#include "GhostSchema.h"
#include "GameFollowManager.h"
#include "GameMaxMap.h"
#include "MultiMapAutoFind.h"


extern FrameManager *g_pFrameMgr;

GameKeyMoveOperate::GameKeyMoveOperate()
{
	m_uLastChangeTime = m_uLastMoveTime = timeGetTime();
	m_isStop = true;
	m_fps = m_god = false;
	m_fLastAngle = HUGE_VAL;
	m_lastKeyCmd = HUGE_VAL;
}

GameKeyMoveOperate::~GameKeyMoveOperate()
{
}

GameKeyMoveOperate & GameKeyMoveOperate::getSingleton()
{
	static GameKeyMoveOperate singleton;
	return singleton;
}

void GameKeyMoveOperate::update()
{
	FollowCamera*	pMainCamera = GamePlayManager::getSingleton().getMainCamera();
	unsigned int	now = timeGetTime();
	float	vRelative = 0, hRelative = 0;
	bool	isKeyDown = false;
	if (g_pFrameMgr->getWndHandle() == ::GetForegroundWindow())
	{
		if (KEY_UP != 0)
		{
			if (GetKeyState(KEY_UP) & 0xFF00)
			{
				vRelative += 1;
				isKeyDown = true;
			}
			else
			{
				StopMove(KEYMOVE_UP);
			}
		}
		if (KEY_DOWN != 0)
		{
			if (GetKeyState(KEY_DOWN) & 0xFF00)
			{
				vRelative -= 1;
				isKeyDown = true;
			}
			else
			{
				StopMove(KEYMOVE_DOWN);
			}
		}
		if (KEY_RIGHT != 0)
		{
			if (GetKeyState(KEY_RIGHT) & 0xFF00)
			{
				hRelative += 1;
				isKeyDown = true;
			}
			else
			{
				StopMove(KEYMOVE_RIGHT);
			}
		}
		if (KEY_LEFT != 0)
		{
			if (GetKeyState(KEY_LEFT) & 0xFF00)
			{
				hRelative -= 1;
				isKeyDown = true;
			}
			else
			{
				StopMove(KEYMOVE_LEFT);
			}
		}
	}

	if (vRelative == 0 && hRelative == 0)
	{
		if (!isKeyDown)
		{
			m_lastKeyCmd = HUGE_VAL;
			MoveStop();
		}
	}
	else
	{
		MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if (pMainPlayer && pMainPlayer->getActState().GetState() != GSACT_MOVE)
		{
			m_fLastAngle = HUGE_VAL;
		}

		double keyCmd = atan2(vRelative, hRelative) * 180 / M_PI;
		if (!(m_lastKeyCmd - keyCmd > -0.001 && m_lastKeyCmd - keyCmd < 0.001))
		{
			do 
			{
				GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
				if (pSpell != NULL)
				{
					if (pSpell->getState() == GameSpell::STATE_COLLECT)
					{
						break;
					}
					tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
					if (attrib && attrib->m_isNoBreak)
					{
						break;
					}
					SkillManager::getSingleton().onStop();
				}
			} while (false);
			m_lastKeyCmd = keyCmd;
		}
		do 
		{
			if (!pMainPlayer->canControl() && pMainPlayer->m_pCarry->getCarryState() != eCarrying )
			{
				break;
			}
			if (SkillManager::getSingleton().getIntentSpell() != NULL)
			{
				break;
			}
			GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
			if (pSpell && !SkillManager::getSingleton().canMoveAttack(pSpell->getSpellId()))
			{
				break;
			}
			double fAngle = keyCmd - pMainCamera->getAngleLR();
			if (fAngle  - m_fLastAngle < 0.001 &&  fAngle - m_fLastAngle > -0.001)/// 角度未改变
			{
				m_uLastChangeTime = now;
				if (now < m_uLastMoveTime + 1000)
				{
					break;/// 如果间隔小于1秒则不寻路
				}
			}
			else if (now < m_uLastChangeTime + 0)
			{
				break;/// 改变方向至少也要0.3秒延迟
			}
			if (MoveStep(float(fAngle)))
			{
				m_fLastAngle = fAngle;
				m_uLastChangeTime = m_uLastMoveTime = now;
			}
		} while (false);
	}

	if (IsFPS())
	{
		while(ShowCursor(false) >= 0);
	}
}

bool GameKeyMoveOperate::MoveStep(float angle)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return true;
	}
	GameMap *map = pMainPlayer->getCurMap();
	if (map == NULL)
	{
		return true;
	}
	GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
	if (pSpell != NULL)
	{
		if (pSpell->getState() == GameSpell::STATE_COLLECT)
		{
			return false;
		}
		tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
		if (attrib && attrib->m_isNoBreak)
		{
			return false;
		}
	}

	if( pMainPlayer->isOnMonsterMachine() && pMainPlayer->getMonsterMachine()->isInCollectSkill() )
	{
		return false;
	}

	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
	if (mutilMapFinder.IsAutoMoving())
	{
		return false;
	}
	int nDis = (int) (pMainPlayer->getSpeed() * 1.5);
	Ogre::Point2D result;
	result.x =	(int) (pMainPlayer->GetPosition().x / 10 + nDis * Ogre::Cos(angle));
	result.y =	(int) (pMainPlayer->GetPosition().z / 10 + nDis * Ogre::Sin(angle));
	if( !GhostSchema::isGhost )
	{
		map->getMaskMap()->lineOfSightTest(
			Ogre::Point2D(pMainPlayer->GetPosition().x / 10, pMainPlayer->GetPosition().z / 10), result, GameMaskMap::maskStop, 0, &result);
	}
	SkillManager::getSingleton().onStop();

	// 若主角处于自动寻路的状态,则终止主角的自动寻路状态
	pMainPlayer->setAutoFind( false );
	if ( GameFollowManager::getSingleton().isWeiXing() )
	{
		GameFollowManager::getSingleton().cancelWeiXing();
	}

	if (pMainPlayer->CmdMoveTo(result.x, result.y) == MOVE_SUCCESS)
	{
		if ( pMainPlayer->canAutoTalkToNpc() )
		{
			pMainPlayer->endAutoTalkToNpc();
		}
		m_isStop = false;
		return true;
	}
	return false;
}

void GameKeyMoveOperate::MoveStop()
{
	if (m_isStop)
	{
		return;
	}
	m_isStop = true;
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	if (pMainPlayer->getActState().GetState() == GSACT_MOVE)
	{
		pMainPlayer->CmdMoveStop();
	}
}

void GameKeyMoveOperate::SetFPS(bool b /*= true*/)
{
	m_fps = b;
}

bool GameKeyMoveOperate::IsFPS()
{
	return m_fps;
}

void GameKeyMoveOperate::SetGod(bool b /*= true*/)
{
	m_god = b;
}

bool GameKeyMoveOperate::IsGod()
{
	return m_god;
}

void GameKeyMoveOperate::StartMove(int type, int key)
{
	switch (type)
	{
	case KEYMOVE_UP:
		KEY_UP = key & 0xff;
		break;
	case KEYMOVE_DOWN:
		KEY_DOWN = key & 0xff;
		break;
	case KEYMOVE_LEFT:
		KEY_LEFT = key & 0xff;
		break;
	case KEYMOVE_RIGHT:
		KEY_RIGHT = key & 0xff;
		break;
	}
}

void GameKeyMoveOperate::StopMove(int type)
{
	switch (type)
	{
	case KEYMOVE_UP:
		KEY_UP = 0;
		break;
	case KEYMOVE_DOWN:
		KEY_DOWN = 0;
		break;
	case KEYMOVE_LEFT:
		KEY_LEFT = 0;
		break;
	case KEYMOVE_RIGHT:
		KEY_RIGHT = 0;
		break;
	}
}

void GameKeyMoveOperate::reset()
{
	KEY_UP = KEY_DOWN = KEY_LEFT = KEY_RIGHT = 0;
}
