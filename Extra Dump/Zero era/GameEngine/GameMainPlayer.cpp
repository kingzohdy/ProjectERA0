#include "GameMainPlayer.h"
#include "GameMap.h"
#include "GameMaskMap.h"
#include "GameMsgPrint.h"
#include "ServerTimer.h"
#include "OgreVector2.h"
#include "GameActorManager.h"
#include "IAutoFind.h"
#include "ShowActor.h"
#include "OgreRect.h"
#include <Windows.h>
#include "GameSkill_Mgr.h"
#include "GameMsgPrint.h"
#include "ShowGameMap.h"
#include "GameSCTDamageManager.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Base.h"
#include "OgreScriptLuaVM.h"
#include "ogredistort.h"
#include "GhostSchema.h"
#include "CSNetManager.h"
#include "GameEffect_Mgr.h"
#include "GameSkill_Mgr.h"
#include "OgreLog.h"
#include "ogrebloom.h"

#include "OgreBackScene.h"
#include "OgreRoot.h"
#include "IChant_Base.h"
#include "AutoTalkNpc.h"

#include "ApexClient.h"

#include "GameDBData.h"

using namespace Ogre;

class PickItem
{
private:
	int itemid;

public:
	PickItem(int id) : itemid(id) {}
	void operator () ()
	{
		MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if (pMainPlayer == NULL)
		{
			return;
		}
		pMainPlayer->getContainer().send_PickItem(itemid);
	}
};

MainGamePlayer::MainGamePlayer() : m_LastCmdMoveTick(0), m_lastCached(false), m_bAutoFind( false ), m_nMemID( 0 ), m_bChangeSkillShortCut( false ), m_isAutoMove(false),
m_bCanControl(true), m_bUnaware(false), m_nLockCount(0), m_nPworldInstID(0), m_bAutoTalkNpc( false ), m_lastAutoTalkNpcId(0), m_moveTask(NULL),m_OneOffExp(0), m_DriverId(0), 
m_lastMoveToFlag(false), m_mustContinueAutoMove(false), m_nNormalMachinePworldClanID(0), m_nUnionMachinePworldClanID(0)
{
	setClanSame();
	RefreshCmdTime();
	m_Trump.initTrumpNetMessage();
	m_Rider.initRiderNetMessage();
	m_Pet.initPetNetMessage();
	setType( GAT_MAINPLAYER );
	strcpy(m_szPkTargetName,"");
	m_nWorldID = 0;
	m_nGmPermFlag = 0;
	m_OfflineExp = 0;
	m_MaxOfflineExp = 0;
	m_PworldExp = 0;
	m_MaxPworldExp = 0;
	m_nBattleBeginTime = 0;
	m_nBattleNum = 0;
	m_nBattleMaxNum = 0;
	m_nYestodayInfo = 0;
	m_nDailyUsedBull = 0;
	m_nColorFontEndTime = 0;
	m_nLoveColorType = 0;
	strcpy(m_szLoverName,"");
	m_loverID = 0;
}

MainGamePlayer::~MainGamePlayer()
{
	// 重置冒血显示信息
	SkillManager::getSingleton().getSpellControler()->clear();
	SCTDamageManager::getSingleton().clearDamageActor();
	m_Trump.releasePetNetMessage();
	m_Rider.releaseRiderNetMessage();
	m_Pet.releasePetNetMessage();

	if (DistortEffect::getSingletonPtr())
		DistortEffect::getSingleton().setGray(false);
	BloomEffect::getSingleton().stopBlend();
	BloomEffect::getSingleton().setGaussBlur(false);
	setAutoFind( false );
	delete m_moveTask;
}

void MainGamePlayer::OnReachPathTarget()
{
	GameActor::OnReachPathTarget();
	if ( getAutoFind() )
	{
		setAutoFind( false );
	}
	if (m_moveTask)
	{
		CmdMovePause();
		(*m_moveTask)();
		delete m_moveTask;
		m_moveTask = NULL;
	}
	m_lastMoveToFlag = false;
}

// 下次寻路和上次寻路时间间隔是否太短
bool MainGamePlayer::isTimeShort()
{
	return timeGetTime() < m_LastCmdMoveTick + 150;
}

void MainGamePlayer::breakBTypePre()
{
	BreakCurChant();

	if( getActState().GetState() == GSACT_COLLECTITEM )
	{
		GameMsgPrint::getSingleton().endCollect();
	}

	ActorManager::getSingleton().getScriptVM()->callString("util.BreakBlockPrepare('DropMonsterMachine')");

	SkillManager& skillMgr = SkillManager::getSingleton();
	GameSpell *pSpell = skillMgr.getSpellControler()->getCurSpell();

	if ( pSpell == NULL )
	{
		return;
	}

	int nSkillID = pSpell->getSpellId();
	if (skillMgr.canMoveAttack( nSkillID ))
	{
		return;
	}

	pSpell->Break();
}

int MainGamePlayer::CmdMoveTo(int to_x, int to_z, int dis )
{
	if (m_pCurMap == NULL)
	{
		return MOVE_STATUS_FORBID;
	}
	RefreshCmdTime();
	if (GhostSchema::isGhost)
	{
		int cur_x, cur_z;
		m_pMapMover->GetCurPos(cur_x, cur_z);
		GameMaskPath path;
		path.addPathPoint(cur_x, cur_z);
		path.addPathPoint(to_x, to_z);
		if ( getAutoFind() )
		{	
			m_pAutoLine->init( path );
		}
		m_pMapMover->StartOnPath(&path);
		if ( getActState().GetState() != GSACT_MOVE)
		{
			ChangeActState(GSACT_MOVE);
		}
		return MOVE_SUCCESS;
	}
	if (!canControl() && m_pCarry->getCarryState() != eCarrying )
	{
		return MOVE_STATUS_FORBID;
	}
	if( !m_MonsterMachine.canMove() )
	{
		return MOVE_STATUS_FORBID;
	}
	if( m_MonsterMachine.hasPatrolID() )
	{
		return MOVE_STATUS_FORBID;
	}
	unsigned int curtick = timeGetTime();
	if(false && curtick < m_LastCmdMoveTick + 150)
	{
		return MOVE_TIME_INTERVAL_SMALL;
	}
	int nMaskBit = MASK_STOP;
	if( getActState().GetState() == GSACT_FLY )
	{
		nMaskBit = MASK_STOP;
	}

	breakBTypePre();

	if( m_nSpeed == 0 )
	{
		return MOVE_SPEED_ZERO;
	}

	if (getShowActor()->getCurEntity()->hasAnimPlaying(10133) || getShowActor()->getCurEntity()->hasAnimPlaying(10137))
	{
		return MOVE_SPEED_ZERO;
	}

	if (!getBufStatus()->canMove())
	{
		return MOVE_STATUS_FORBID;
	}

	if( m_pSpecialMove->getType() != MOVETYPE_NOMAL ) 
	{
		return MOVE_TYPE_NOT_NORMAL;
	}

	if (isInSkillTetanic())
	{
		return MOVE_IN_TETANIC;
	}

	if (getActState().GetState() == GSACT_STALL) 
	{
		GameMsgPrint::getSingleton().showMidTip( MID_MOVE_FAIL_STALL );
		return MOVE_STATE_CANNOT_MOVE;
	}

	Point2D end(to_x, to_z);
	Point2D scale(m_pCurMap->getMaskMap()->m_lMaskPixelWidth, m_pCurMap->getMaskMap()->m_lMaskPixelHeight);
	Point2D s_EndGrid = end / scale;
	if( !m_pCurMap->getMaskMap()->canWalk(s_EndGrid.x, s_EndGrid.y,nMaskBit) )
	{
		if( !m_pCurMap->getMaskMap()->findPointWalkable( end, nMaskBit ) )
		{
			return MOVE_POINT_CANNOT_WALK;
		}else{
			to_x	= end.x;
			to_z	= end.y; 
		}
	}

	if( !CanChangeActState(GSACT_MOVE) )
	{
		return MOVE_STATE_CANNOT_MOVE;
	}

	m_MoveEndPos.x = m_MoveEndPos.y = m_MoveEndPos.z = 0;
	m_MoveEndDis = dis;

	int cur_x, cur_z;
	m_pMapMover->GetCurPos(cur_x, cur_z);

	if( cur_x == to_x && cur_z == to_z )
	{
		return MOVE_ALREADY_IN_DEST;
	}

	Point2D begin(cur_x, cur_z);
	Point2D s_BeginGrid = begin / scale;
	if (!m_pCurMap->getMaskMap()->canWalk(s_BeginGrid.x, s_BeginGrid.y, nMaskBit))
	{
		tagCSPkgBody msgbody;
		msgbody.MoveReq.MoveType = CS_MOVE_WALK;
		msgbody.MoveReq.MoveSeq = m_CurMoveSeq++;
		msgbody.MoveReq.x = cur_x;
		msgbody.MoveReq.y = cur_z;
		CSMOVEDATA &mdata = msgbody.MoveReq.Data;
		mdata.Walk.Total = 1;
		Point2D pt1, pt2;
		mdata.Walk.Points[0].x = to_x;
		mdata.Walk.Points[0].y = to_z;
		ServerTimer::getSingleton().getServerTime(msgbody.MoveReq.SvrTime.BenchTimeSec, msgbody.MoveReq.SvrTime.BenchTimeUSec);
		SendGameServerMsg(MOVE_CLT, msgbody);
		return MOVE_POINT_CANNOT_WALK;
	}

	if( dis != 0 )
	{
		Vector2 end( (float)to_x, (float)to_z );
		Vector2 begin( (float)cur_x, (float)cur_z );
		float length = (end-begin).length();
		if( length < dis && m_ActState.GetState() == GSACT_MOVE )
		{
			CmdMoveStop();
			return MOVE_DIST_INTERVAL_SMALL;
		}
		to_z = (int)(to_z + float(cur_z - to_z )/length*dis);
		to_x = (int)(to_x + float(cur_x - to_x )/length*dis);
		if( to_x == cur_x && to_z == cur_z )
		{
			return MOVE_ALREADY_IN_DEST;
		}
	}

	if (dis == 0 && m_lastMoveToFlag && getActState().GetState() == GSACT_MOVE)
	{
		if (m_lastMoveToX == to_x && m_lastMoveToY == to_z)
		{
			return MOVE_SUCCESS;
		}
	}
	m_mustContinueAutoMove = false;
	GameMaskPath path;
	int distance = (cur_x - to_x) * (cur_x - to_x) + (cur_z - to_z) * (cur_z - to_z);
	if (distance >= 1200 * 1200 || m_pCurMap->getMaskMap()->lineOfSightTest(Ogre::Point2D(cur_x, cur_z), Ogre::Point2D(to_x, to_z), DYN_MASK_STOP))
	{
		bool find = m_pCurMap->FindPath(path, cur_x, cur_z, to_x, to_z, nMaskBit, true );
		if (find && path.getNumPoint() < 2)
		{
			find = false;
		}
		if( !find )
		{
			if (!m_pCurMap->getMaskMap()->lineOfSightTest(Ogre::Point2D(cur_x, cur_z), Ogre::Point2D(to_x, to_z), MASK_STOP | DYN_MASK_STOP))
			{
				return MOVE_POINT_CANNOT_WALK;
			}
			path.reset(m_pCurMap->getMaskMap());
			path.addPathPoint(cur_x, cur_z);
			path.addPathPoint(to_x, to_z);
		}
	}
	else
	{
		nMaskBit = MASK_STOP | DYN_MASK_STOP;
		if( getActState().GetState() == GSACT_FLY )
		{
			nMaskBit = MASK_STOP | DYN_MASK_STOP;
		}
		if( !m_pCurMap->FindPath(path, cur_x, cur_z, to_x, to_z, nMaskBit, false ) )
		{
			return MOVE_POINT_CANNOT_WALK;
		}
	}

	size_t npoint = path.getNumPoint();
	assert(npoint > 0);
	if (npoint < 2)
	{
		return MOVE_POINT_CANNOT_WALK;
	}
	// 若主角当前处于自动寻路状态下,则获取自动寻路路线图的点集合
	if ( getAutoFind() )
	{	
		m_pAutoLine->init( path );
	}

	if( npoint - 1 > CS_MAX_MOVEPOINTS/2 )
	{
		m_MoveEndPos.x = to_x;
		m_MoveEndPos.y = to_z;
		path.limitPathPoint( CS_MAX_MOVEPOINTS/2 + 1 );
		npoint = CS_MAX_MOVEPOINTS/2 + 1;
	}

	if (curtick < m_LastCmdMoveTick + 150)
	{
		if (m_moving)
		{
			tagCSPkgBody msgbody;
			msgbody.MoveReq.MoveType = CS_MOVE_TURN;
			msgbody.MoveReq.MoveSeq = m_CurMoveSeq++;
			msgbody.MoveReq.x = cur_x;
			msgbody.MoveReq.y = cur_z;
			CSMOVEDATA &mdata = msgbody.MoveReq.Data;
			mdata.Walk.Total  = 0;

			ServerTimer::getSingleton().getServerTime(msgbody.MoveReq.SvrTime.BenchTimeSec, msgbody.MoveReq.SvrTime.BenchTimeUSec);
			SendGameServerMsg(MOVE_CLT, msgbody);
			m_LastCmdMoveTick = curtick;
			m_moving = false;
		}
		if (!m_lastCached)
		{
			m_lastCmdMoveX = cur_x;
			m_lastCmdMoveY = cur_z;
			ServerTimer::getSingleton().getServerTime(m_lastBenchTimeSec, m_lastBenchTimeUSec);
			m_lastCached = true;
		}
	}
	else if(!m_lastCached)
	{
		tagCSPkgBody msgbody;
		msgbody.MoveReq.MoveType = CS_MOVE_WALK;
		msgbody.MoveReq.MoveSeq = m_CurMoveSeq++;
		msgbody.MoveReq.x = cur_x;
		msgbody.MoveReq.y = cur_z;
		CSMOVEDATA &mdata = msgbody.MoveReq.Data;
		mdata.Walk.Total = int(npoint - 1);
		Point2D pt1, pt2;
		for(char i=1; i<= mdata.Walk.Total; i++)
		{
			pt1 = path.getPathPoint(i);
			mdata.Walk.Points[i-1].x = pt1.x;
			mdata.Walk.Points[i-1].y = pt1.y;
		}
		ServerTimer::getSingleton().getServerTime(msgbody.MoveReq.SvrTime.BenchTimeSec, msgbody.MoveReq.SvrTime.BenchTimeUSec);
		SendGameServerMsg(MOVE_CLT, msgbody);
		m_LastCmdMoveTick = curtick;
		m_moving = true;
	}

	Point2D pt = path.getPathPoint(path.getNumSegment());
	m_lastMoveToFlag = true;
	m_lastMoveToX = pt.x;
	m_lastMoveToY = pt.y;

	m_pMapMover->StartOnPath(&path);

	if ( getActState().GetState() != GSACT_MOVE)
	{
		ChangeActState(GSACT_MOVE);
	}

	for( int i = 0; i < PET_MAX_FIGHT_SHI; i ++ )
	{
		GameActor* pActor = m_Pet.getPetActor( i );
		if( pActor != NULL )
		{
			m_Pet.setMoveToPoint( to_x + MIN_DIST, to_z + MIN_DIST );
		}
	}
	clearMoveTask();
	return MOVE_SUCCESS;
}

bool MainGamePlayer::canWalk( int to_x, int to_y )
{
	if (m_pCurMap == NULL)
	{
		return false;
	}
	bool bLong = true;
	int dis = 0;

	int nMaskBit = MASK_STOP | DYN_MASK_STOP;
	if( getActState().GetState() == GSACT_FLY )
	{
		nMaskBit = MASK_STOP | DYN_MASK_STOP;
	}
	
	Point2D end( to_x, to_y );
	Point2D scale(m_pCurMap->getMaskMap()->m_lMaskPixelWidth, m_pCurMap->getMaskMap()->m_lMaskPixelHeight);
	Point2D s_EndGrid = end / scale;

	if( !m_pCurMap->getMaskMap()->canWalk(s_EndGrid.x, s_EndGrid.y,nMaskBit) )
	{
		if( !m_pCurMap->getMaskMap()->findPointWalkable( end, nMaskBit,1 ) )
		{
			return false;
		}
	}

	GameMaskPath path;
	int cur_x, cur_z;
	m_pMapMover->GetCurPos(cur_x, cur_z);
	if( !m_pCurMap->FindPath(path, cur_x, cur_z, to_x, to_y, nMaskBit, bLong ) )
	{
		return false;
	}

	return true;
}

bool MainGamePlayer::canDelayMove()
{
	if ( GhostSchema::isGhost )
	{
		return false;
	}

	unsigned int curtick = timeGetTime();
	// TODO: ZM那儿去掉延迟限制
	/*if(false && curtick < m_LastCmdMoveTick + 150)
	{
		return true;
	}*/

	if( m_nSpeed == 0 )
	{
		return true;
	}

	if (getShowActor()->getCurEntity()->hasAnimPlaying(10133) || getShowActor()->getCurEntity()->hasAnimPlaying(10137) )
	{
		return true;
	}

	return false;
}

bool MainGamePlayer::canMove()
{
	if ( GhostSchema::isGhost )
	{
		return true;
	}

	unsigned int curtick = timeGetTime();
	if(false && curtick < m_LastCmdMoveTick + 150)
	{
		return false;
	}

	if( m_nSpeed == 0 )
	{
		return false;
	}

	if ( getShowActor()->getCurEntity()->hasAnimPlaying(10133) || getShowActor()->getCurEntity()->hasAnimPlaying(10137) )
	{
		return false;
	}

	if( m_pSpecialMove->getType() != MOVETYPE_NOMAL ) 
	{
		return false;
	}

	if( isInSkillTetanic() || getActState().GetState() == GSACT_STALL ) 
	{
		return false;
	}

	if( getActState().GetState() == GSACT_AUTOFLY || getActState().GetState() == GSACT_FLY )
	{
		return false;
	}

	if( !CanChangeActState(GSACT_MOVE) )
	{
		return false;
	}

	return true;
}
bool MainGamePlayer::isInPilState()
{
	return SkillManager::getSingleton().getSpellControler()->getCurPilSpell();
}

void MainGamePlayer::CmdMovePause()
{
	int cur_x, cur_z;
	m_pMapMover->GetCurPos(cur_x, cur_z);
	if( getActState().GetState() != GSACT_DEAD )
	{
		ChangeActState(GSACT_STAND);
	}
	m_pMapMover->StopOnPoint( cur_x, cur_z );
	if ( getAutoFind() )
	{
		m_mustContinueAutoMove = true;
	}

	if (m_moving)
	{
		m_moving = false;
		tagCSPkgBody msgbody;
		msgbody.MoveReq.MoveType = CS_MOVE_TURN;
		msgbody.MoveReq.MoveSeq = m_CurMoveSeq++;
		msgbody.MoveReq.x = cur_x;
		msgbody.MoveReq.y = cur_z;
		CSMOVEDATA &mdata = msgbody.MoveReq.Data;
		mdata.Walk.Total  = 0;

		ServerTimer::getSingleton().getServerTime(msgbody.MoveReq.SvrTime.BenchTimeSec, msgbody.MoveReq.SvrTime.BenchTimeUSec);
		SendGameServerMsg(MOVE_CLT, msgbody);
		m_LastCmdMoveTick = timeGetTime();;
	}
}

void MainGamePlayer::CmdMoveStop()
{
	if ( getAutoFind() )
	{
		setAutoFind( false );
	}
	if (canAutoTalkToNpc())
	{
		endAutoTalkToNpc();
	}
	m_lastMoveToFlag = false;
	clearMoveTask();
	CmdMovePause();
}

void MainGamePlayer::UpdateFreedomPet( unsigned int dtick )
{
	m_Pet.UpdateFreedomPetAnim( dtick );
}

void MainGamePlayer::Update(unsigned int dtick)
{
	GhostSchema::update();
	if (m_pCurMap && m_pCurMap->getMaskMap())
	{
		m_pCurMap->getMaskMap()->UpdateSearchNodes();
	}
	GameActor::Update( dtick );
	m_Container.Update( dtick );

	trySendCacheMove();

	if (getActState().GetState() != GSACT_MOVE)
	{
		ContinueAutoFind();
	}

	int selectID = this->GetSelectTarget();
	Entity *ent = NULL;
	ShowActor *showActor = NULL;
	GameActor *actor = ActorManager::getSingleton().FindActor(selectID);
	
	if (actor)
	{
		showActor = actor->getShowActor();

		if (showActor)
		{
			ent = actor->getShowActor()->getCurEntity();
			if (ent)
				ent->setDrawbackScene(true);
		}
	}

	ent = NULL;
	showActor = NULL;
	showActor = getShowActor();
	if (showActor)
	{
		ent = showActor->getCurEntity();

		if (ent)
			ent->setDrawbackScene(true);

		// 设置不被裁剪
		ent->SetForceShadowMapCull(false);
	}

	ent = NULL;
	if (m_Rider.isOnHorse())
	{
		Ogre::ShowActor* pRideShowActor =m_Rider.m_pShowActor;
		if ( pRideShowActor != NULL )
		{
			ent = pRideShowActor->getCurEntity();
			if ( ent != NULL )
			{
				ent->setDrawbackScene(true);
			}
		}
		
		ent = NULL;
		GameActor *owner = m_Rider.getOwner();
		if (owner)
		{
			Ogre::ShowActor* pOwnerShowAcor = owner->getShowActor();
			if ( pOwnerShowAcor != NULL )
			{
				ent = pOwnerShowAcor->getCurEntity();
				if (ent)
					BackGameScene::getSingleton().onAttachObject(ent);
			}
		}
	}
	if (!m_isAFK && getActState().GetState() != GSACT_MOVE && !isInFight())
	{
		int mapid = 0;
		if (getCurMap())
		{
			mapid = getCurMap()->getRealId();
		}
		if (mapid >= GAME_PWORLD_MAPID)
		{
			const tagPworldDef *PworldDef = DBDataManager::getSingleton().getPworldDef(mapid / 100);
			if (PworldDef && PworldDef->BattleID != 0)
			{
				unsigned int now = timeGetTime();
				if (m_lastCmdTime + 3 * 60 * 1000 < now)
				{
					m_lastCmdTime += 1;
					setAFK(true);
				}
			}
		}
	}
	if( getOwnMonster() > 0 )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( getOwnMonster() );
		if( pActor )
		{
			m_convoyInfo.m_nMonsterMapId = pActor->getMapId();
			m_convoyInfo.m_nMonsterPosX = pActor->GetPosition().x/10;
			m_convoyInfo.m_nMosnterPoxY = pActor->GetPosition().z/10;
		}
	}
	if ( GetConvoy() ) 
	{
		const tagConvoyTask& convoyInfo = getConvoyInfo();
		if ( getOwnMonster() != 0 && ActorManager::getSingleton().FindActor( getOwnMonster() ) != NULL )
		{
			GameActor* pMonster = ActorManager::getSingleton().FindActor( getOwnMonster() );
			const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pMonster->getResId() );	
			const GameXYZ& pMainPos		= GetPosition() / 1000;
			const GameXYZ& pMonsterPos	= pMonster->GetPosition() / 1000;
			unsigned long nDist = ( pMonsterPos.x - pMainPos.x ) * ( pMonsterPos.x - pMainPos.x ) + 
				( pMonsterPos.z - pMainPos.z ) * ( pMonsterPos.z - pMainPos.z );
			unsigned long nSqrtDist = (unsigned long)sqrt( (float)( nDist ) );
			char szTaskWarnDesc[256];
// 			if ( nSqrtDist >= 20 )
// 			{
// 				//GameMsgPrint::getSingleton().SendUIEvent( "GE_CONVOY_DIST_LONG" );
// 			}
			sprintf( szTaskWarnDesc, "ShowConvoDistanceInfo( [===[%s%s%d%s]===] )", 
				"距离", "货车", nSqrtDist, "米" );
			// 				drawTopTitles( pcamera->GetCameraNode(), szTaskWarnDesc, color, TITLE_FOOT_LINE, -5, NULL, 0, true );
			ActorManager::getSingleton().getScriptVM()->callString(szTaskWarnDesc);
		}
		else if ( convoyInfo.m_nMonsterResID != 0 )
		{
			const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( convoyInfo.m_nMonsterResID );
			char szTaskWarnDesc[256];
			sprintf( szTaskWarnDesc, "ShowConvoDistanceInfo( [===[#L%s%s%s#n]===] )",
				"距离", "货车", "过远" );
			// 			drawTopTitles( pcamera->GetCameraNode(), szTaskWarnDesc, 0xfffc3b2c, TITLE_FOOT_LINE, -5, NULL, 0, true );
			ActorManager::getSingleton().getScriptVM()->callString(szTaskWarnDesc);
		}
	}

	UpdateFreedomPet( dtick );
}

void MainGamePlayer::updateUI( int nChangeValue )
{
	m_mustUpdateUI = true;
}

void MainGamePlayer::actualUpdateUI()
{
	GameMsgPrint::getSingleton().SendUIEvent( "UI_TARGET_ATTRIBUTE_CHG" );
	GameMsgPrint::getSingleton().SendUIEvent( "UI_ACTOR_ATTRIBUTE_CHG" );
	/*这个消息似乎不应该出现在这里*/GameMsgPrint::getSingleton().SendUIEvent( "GE_ON_ACTOR_ENTER_WORLD" );

	GameActor* pTarget = ActorManager::getSingleton().FindActor( GetSelectTarget() );
	if( pTarget != NULL && GetID() == pTarget->getHitTargetID() )
	{
		char buf[256];
		sprintf(buf,"UpdateTargetOfTargetFrame( %d )",pTarget->getHitTargetID());
		ActorManager::getSingleton().getScriptVM()->callString( buf );
	}
}

GameActor* MainGamePlayer::getDistanceActor( int dis )
{
	ACTOR_ITER iter;
	for( iter  = ActorManager::getSingleton().getActorBegin(); 
		iter != ActorManager::getSingleton().getActorEnd(); ++iter )
	{
		GameActor* pActor = iter->second;
		if( pActor == this 
			|| pActor->getType() == GAT_NPC 
			|| pActor->getType() == GAT_ITEM 
			|| pActor->getType() == GAT_PET
			|| pActor->getType() == GAT_PLAYER
			|| pActor->getActState().GetState() == GSACT_DEAD )
		{
			continue;
		}

		int RealDis = (int)pActor->GetPosition().subtract(GetPosition()).length();
		if( RealDis <= dis )
		{
			return pActor;
		}
	}
	return NULL;
}

void MainGamePlayer::setShowHead(bool bShow)
{
	if(bShow)
	{
		m_nShowFlag |= SHOW_HEAD_FLAG; 
	}else{
		m_nShowFlag ^= SHOW_HEAD_FLAG;
	}
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_SHOW_HEAD;
	msgbody.AdjustC.AdjustData.ShowHead		= (int)isShowHead();
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

void MainGamePlayer::setShowFashion(bool bShow)
{
	if( bShow )
	{
		m_nShowFlag |= SHOW_FASHION_FLAG;
	}else{
		m_nShowFlag ^= SHOW_FASHION_FLAG;
	}
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_SHOW_FASHION;
	msgbody.AdjustC.AdjustData.ShowHead		= (int)isShowFashion();
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

void MainGamePlayer::setShowBack( bool bShow )
{
	if( bShow )
	{
		m_nShowFlag |= SHOW_BACK_FLAG;
	}else{
		m_nShowFlag ^= SHOW_BACK_FLAG; 
	}
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_SHOW_BACK;
	msgbody.AdjustC.AdjustData.ShowBack		= (int)isShowBack();
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

void MainGamePlayer::setShowFace( bool bShow )
{
	if( bShow )
	{
		m_nShowFlag |= SHOW_FACE_FLAG;
	}else{
		m_nShowFlag ^= SHOW_FACE_FLAG; 
	}
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_SHOW_FACE;
	msgbody.AdjustC.AdjustData.ShowFace		= (int)isShowFace();
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

void MainGamePlayer::setShowLove( bool bShow )
{
	if( bShow )
	{
		m_nShowFlag |= SHOW_WEDDING_DSN_FLAG;
	}else{
		m_nShowFlag ^= SHOW_WEDDING_DSN_FLAG; 
	}
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_SHOW_WEDDING_DSN;
	msgbody.AdjustC.AdjustData.ShowWeddingDsn		= (int)isShowLove();
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 手动升级
void MainGamePlayer::levelUp()
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_TYPE_LEVEL;
	msgbody.AdjustC.AdjustData.Level		= 1;
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 复活
void MainGamePlayer::revivePlayer( int reviveType, int helperId, int skillId, int nFlag )
{
	tagCSPkgBody msgbody;
	msgbody.Revive.ReviveType = reviveType;
	msgbody.Revive.Flag = nFlag;
	if( reviveType == REVIVE_ACCEPT_SKILL )
	{
		msgbody.Revive.Data.AcceptSkill.HelperRoleID	= helperId;
		msgbody.Revive.Data.AcceptSkill.SkillID			= skillId;
	}
	SendGameServerMsg( REVIVE, msgbody );
}

// 结束采集
void MainGamePlayer::endCollect()
{
	tagCSPkgBody msgbody;
	msgbody.CollectClt.OpType = COLLECT_END;
	msgbody.CollectClt.CollectCltData.CollectEnd.MonID = m_nCollectMonId;
	SendGameServerMsg( COLLECT_CLT, msgbody );
}

// 确认xp技能是否能释放
int MainGamePlayer::checkXP( int skillid )
{
	if( SkillManager::getSingleton().getSkillDyAttrib( skillid ) == NULL )
	{
		return MID_XP_NOCAMPTRUMP;
	}
	if( !getBufStatus()->canUsedSpell( skillid ) || !canControl() )
	{
		return MID_SHORTCUT_CANNOTUSESPELL_BUF;
	}
	int nTrumpNum = getTrump()->getTrumpMaxNum();
	for( int i = 0; i < nTrumpNum; i++ )
	{
		tagRoleFairy_ToLua* RoleTrump = getTrump()->getRoleFairy(i);
		if( RoleTrump->FairyID != 0 && getTrump()->isFairyInState( RoleTrump->Stat, FAIRY_STAT_CAMP ) )
		{
			tagFairyDef_ToLua* FairyDef = getTrump()->getFairyDef( RoleTrump->FairyID, RoleTrump->FairyLevel );
			for( int i = 0; i < FAIRY_MAX_SKILL; i++ )
			{
				if( FairyDef != NULL && FairyDef->FairySkillList[i].SkillID != 0 )
				{
					if( FairyDef->FairySkillList[i].SkillID != skillid )
					{
						continue; 
					}

					const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef( FairyDef->FairySkillList[i].SkillID, FairyDef->FairySkillList[i].SkillLevel );
					if( getXP() < SkillManager::getSingleton().getSkillCostXp(skillid) )
					{
						return MID_XP_NOENOUGHXP;
					}

					if( ( SkillDef.WushuangFlag == 1 || SkillDef.WushuangFlag == 0 ) && getMP() >= SkillManager::getSingleton().getSkillCostMp(skillid))
					{
						return 0;
					}else{
						return MID_XP_NOENOUGHMP;
					}

					if( SkillDef.WushuangFlag == 2 && m_pCurMap->getMapId() >= 10000 )
					{
						return 0;
					}else{
						return MID_XP_NOINMAP;
					}
				}
			}
		}
	}
	return MID_XP_NOCAMPTRUMP;
}

bool MainGamePlayer::isOnMorph()
{
	return getActState().getInState( GSACT_IN_MORPH );
}

int MainGamePlayer::getPathPointNum()
{
	return (int)m_pMapMover->m_CurPath.getNumPoint();
}

Ogre::Point2D MainGamePlayer::getPathPoint( int nIndex )
{
	return m_pMapMover->m_CurPath.getPathPoint( nIndex );
}

int	MainGamePlayer::getCurPosotion( )
{
	return m_pMapMover->m_CurSeg;
}

bool MainGamePlayer::getAutoFind()
{
	return m_bAutoFind;
}

void MainGamePlayer::setAutoFind( bool bAuto )
{
	if (bAuto)
	{
		SkillManager::getSingleton().onStop();
	}
	bool bLastState = m_bAutoFind;
	m_bAutoFind = bAuto;

	// 若主角原来不处于自动寻路状态,但是现在处于自动寻路状态,则播放动画特效
	if ( !bLastState && m_bAutoFind )
	{
		getShowActor()->getCurEntity()->playMotion( "294000_13", false, 294000 ); 
	}

	// 若主角原来处于自动寻路状态,但是现在不处于自动寻路状态,则不播放动画特效
	if ( bLastState && !m_bAutoFind )
	{
		getShowActor()->getCurEntity()->stopMotion( 294000 );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_END_AUTOFIND" );
		m_pAutoLine->clearPath();
	}
}

int MainGamePlayer::continueLastAutoFind()
{
	Ogre::Point2D	lastAutoPoint = m_pAutoLine->getlastAutoDestScenePoint();

	// 若主角从未进行过自动寻路,则不进行自动寻路
	if ( lastAutoPoint.x == -1 && lastAutoPoint.y == -1 )
	{
		return MOVE_ALREADY_IN_DEST;
	}

	setAutoFind( true );
	return CmdMoveTo( lastAutoPoint.x, lastAutoPoint.y );
}

void MainGamePlayer::setOverlayRegion( const char* pName, float fSpeed )
{
// 	if( !std::string( pName ).empty() )
	{
		getCurMap()->getShowGameMap()->setOverlayRegion( pName, fSpeed );
	}
}

// 打断吟唱行为
void MainGamePlayer::breakPreAction()
{
	// 如果处于移动状态, 停下来
	if( getActState().GetState() == GSACT_MOVE )
	{
		CmdMoveStop();
	}
	breakChannel();
}

void MainGamePlayer::breakChannel()
{
	bool bBreak = false;
	GameSpell* pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
	if (pSpell != NULL)
	{
		pSpell->Break();
		bBreak = true;
	}
	BreakCurChant();
	// 打断采集状态
	if( m_ActState.GetState() == GSACT_COLLECTITEM )
	{
		endCollect();
		bBreak = true;
	}

	if (bBreak)
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_BREAK_PRE_ACTION");
	}
}

void MainGamePlayer::jump()
{
	if ( isJumping() )
	{
		return;
	}
	if (isInSkillTetanic())
	{
		return;
	}
	if (getActState().getInState(GSACT_IN_RIDE) ||
		getActState().getInState(GSACT_IN_USEDMACHINE) ||
		getActState().getInState(GSACT_IN_CARRY))
	{
		return;
	}
	//处于NOSKILL状态下则不能跳
	if (getBufStatus()->isInNoSkill())
	{
		return;
	}

	__super::jump();

	breakChannel();

	tagCSPkgBody msgbody;
	msgbody.Jump = 0;
	SendGameServerMsg( PLAYER_JUMP, msgbody );
}

void MainGamePlayer::ListenSpellPackage(int skillid, bool flag, int x, int y)
{
	if (!m_BackInfoList.empty())
	{
		std::map<int, tagBackInfo>::iterator i = m_BackInfoList.find(skillid);
		if (i != m_BackInfoList.end())
		{
			if (flag)
			{	/// 重定位
				i->second.x = x;
				i->second.z = y;
				i->second.startTime = timeGetTime();
			}
			else
			{	/// 取消移动
				m_BackInfoList.erase(i);
			}
		}
	}
}

void MainGamePlayer::ListenSpellPackage(const CSACTIONRES& msg, GameSpell *pSpell)
{
	if (msg.ID == GetID())/// 自己的包
	{
		const tagSkillDef &SkillDef = pSpell->getSkillDef();
		if (SkillDef.AtkType == SKILL_ATK_TYPE_NORMAL)
		{
			ListenSpellPackage(msg.Data.Hit.RuneTransferedID, msg.Data.Hit.SelfPosChgFlag, msg.Data.Hit.SelfPosChg.X, msg.Data.Hit.SelfPosChg.Y);
		}
	}
	else
	{
		for (int i = 0, n = msg.Data.Hit.TotalHit; i < n; ++i)
		{
			if (msg.Data.Hit.Hits[i].ID == GetID())
			{
				if (msg.Data.Hit.Hits[i].HitBackFlag)
				{	/// 被拉立即进入僵直
					tagSkillAttrib* pAttrib = (tagSkillAttrib*)(pSpell->getAttrib());
					setSkillTetanic( pAttrib->m_nMoveTime + pSpell->getHitDelay() );
					std::map<int, tagBackInfo>::iterator i = m_BackInfoList.begin();
					if (i != m_BackInfoList.end())
					{	/// 延长自己位移的时间发生点
						unsigned int newTime = timeGetTime() + pAttrib->m_nMoveTime + pSpell->getHitDelay();
						i->second.startTime = max(i->second.startTime, newTime);
					}
				}
				break;
			}
		}
	}
}

bool MainGamePlayer::canAutoTalkToNpc()
{
	return m_bAutoTalkNpc;
}

void MainGamePlayer::beginAutoTalkToNpc(int resid)
{
	//LOG_PRIVATE( "beginAutoTalkToNpc" );
	m_lastAutoTalkNpcId = resid;
	m_bAutoTalkNpc = true;
}

void MainGamePlayer::endAutoTalkToNpc()
{
	//LOG_PRIVATE( "endAutoTalkToNpc" );
	m_lastAutoTalkNpcId = 0;
	m_bAutoTalkNpc = false;
}

void MainGamePlayer::lockControl()
{
	setCanControl(false);
	if ( canAutoTalkToNpc() )
	{
		endAutoTalkToNpc();
	}
}

void MainGamePlayer::releaseControl()
{
	setCanControl(true);
}

void MainGamePlayer::setCanControl(bool b)
{
	if (b)
	{
		if (--m_nLockCount <= 0)
		{
			// 允许玩家控制
			m_nLockCount = 0;
			m_bCanControl = true;
		}
	}
	else
	{
		++m_nLockCount;
		// 禁止玩家控制
		m_bCanControl = false;
	}
}

bool MainGamePlayer::canControl()
{
	return m_bCanControl;
}

void MainGamePlayer::changeCareer( int nCareerType )
{
	tagCSPkgBody msgbody;
	CSPLAYERMISCCLT& playerMiscCltData	= msgbody.PlayerMiscClt;
	playerMiscCltData.PlayerMiscType	= PLAYER_CAREER_CHG_CLT;
	playerMiscCltData.PlayerMiscCltData.CarrerChgType = nCareerType;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::ChangeCareerToFightLevle( int nCareerType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	CSPLAYERMISCCLT& playerMiscCltData	= msgbody.PlayerMiscClt;
	playerMiscCltData.PlayerMiscType	= PLAYER_MATCH_CHG_CAREER;
	playerMiscCltData.PlayerMiscCltData.MatchChgCareer.Career = nCareerType;
	playerMiscCltData.PlayerMiscCltData.MatchChgCareer.GridIdx = nGridIdx;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::onFight(bool bForce /*= true*/)
{
	if (bForce && !isInFight())
	{
		m_bUnaware = true;
	}
	__super::onFight(bForce);
}

void MainGamePlayer::onLeaveFight()
{
	ActorManager::getSingleton().playerEnemy.clear();
	__super::onLeaveFight();
}

void MainGamePlayer::SetPosition(int x, int z, int height /*= 0 */)
{
	if (getActState().GetState() == GSACT_MOVE)
	{
		ChangeActState(GSACT_STAND);
	}
	if ( getAutoFind() )
	{
		setAutoFind( false );
	}
	m_lastCached = false;
	GameActor::SetPosition(x, z, height);
}

void MainGamePlayer::kill(int id)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (pActor)
	{
		if (pActor->getType() == GAT_MONSTER)
		{
			char szScript[256];
			sprintf( szScript, "KillMonster(%d)", pActor->getResId());
			ActorManager::getSingleton().getScriptVM()->callString(szScript);
		}
		else if (pActor->getType() == GAT_PLAYER)
		{
			char szScript[256];
			sprintf( szScript, "KillPlayer(%d)", pActor->GetID());
			ActorManager::getSingleton().getScriptVM()->callString(szScript);
		}
	}
}

void MainGamePlayer::cast(int id, int lv)
{
	char szScript[256];
	sprintf( szScript, "CastSpell(%d)", id);
	ActorManager::getSingleton().getScriptVM()->callString(szScript);
}

bool MainGamePlayer::hasFashion()
{
	//这个接口没有地方在用
	return false;
}
void MainGamePlayer::onStop(int x, int z, int state /*= 0 */)
{
	GameActor::onStop(x, z, state);
	if ( getAutoFind() )
	{
		setAutoFind( false );
	}
	m_lastCached = false;
}

bool MainGamePlayer::isInPK()
{
	return m_szPkTargetName[0] != 0;
}

void MainGamePlayer::sendLockFace(int type, int value, int skillId)
{
	static unsigned int	lastTime	= 0;
	static DelayNetSender *sender = NULL;
	const int MIN_DELAY = 150;
	if (m_AttackLock.m_nAttackLockType != type || m_AttackLock.m_nLockValue != value)
	{
		lockFace(type, value, skillId);

		tagCSPkgBody msgbody;
		msgbody.AdjustC.Type					 = ADJUST_TYPE_LOCK;
		msgbody.AdjustC.AdjustData.Lock.SkillID	 = skillId;
		msgbody.AdjustC.AdjustData.Lock.LockType = type;
		msgbody.AdjustC.AdjustData.Lock.LockVal	 = value;

		unsigned int now = timeGetTime();
		if (lastTime + MIN_DELAY <= now)
		{
			lastTime = now;
			SendGameServerMsg(ADJUST_CLT, msgbody);
		}
		else
		{
			int delay = lastTime + MIN_DELAY - now;
			if (sender == NULL || sender->complete())
			{
				lastTime += MIN_DELAY;
				delete sender;
				sender = new DelayNetSender(delay, ADJUST_CLT, msgbody);
			}
			else
			{
				sender->setMsg(ADJUST_CLT, msgbody);
			}
		}
	}
}

void MainGamePlayer::unLockFace()
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					 = ADJUST_TYPE_LOCK;
	msgbody.AdjustC.AdjustData.Lock.SkillID	 = 0;
	msgbody.AdjustC.AdjustData.Lock.LockType = ATTACK_LOCK_NONE;
	msgbody.AdjustC.AdjustData.Lock.LockVal	 = 0;
	SendGameServerMsg( ADJUST_CLT, msgbody );
	GameActor::unLockFace();
}

void MainGamePlayer::ChangeClanName( const char* pszClanName )
{
	tagCSPkgBody msgbody;
	CSCHGNAMECLT& changeClanCltData = msgbody.ChgNameClt;
	changeClanCltData.Flag	= CHG_CLAN_NAME;
	strncpy_s( changeClanCltData.NewName, NAME_LEN, pszClanName, strlen( pszClanName ) + 1 );
	SendGameServerMsg( CHGNAME_CLT, msgbody );
}

void MainGamePlayer::ChangeRoleName( const char* pszRoleName )
{
	tagCSPkgBody msgbody;
	CSCHGNAMECLT& changeClanCltData = msgbody.ChgNameClt;
	changeClanCltData.Flag	= CHG_ROLE_NAME;
	strncpy_s( changeClanCltData.NewName, NAME_LEN, pszRoleName, strlen( pszRoleName ) + 1 );
	SendGameServerMsg( CHGNAME_CLT, msgbody );
}

void MainGamePlayer::requestRideMonsterMachine( int nMonsterMemId,int nIdx )
{
	tagCSPkgBody msgbody;
	msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_RIDE;	
	CSRIDEONMACHINE& requestRideOnMachineData	= msgbody.RideMachineClt.Data.Ride;
	requestRideOnMachineData.MachineID			= nMonsterMemId;
	requestRideOnMachineData.SitIdx				= nIdx;
	SendGameServerMsg( RIDE_MACHINE_CLT, msgbody );	
}

void MainGamePlayer::requestDownMonsterMachine( int nMonsterId )
{
	if( isOnMonsterMachine() )
	{
		tagCSPkgBody msgbody;
		msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_LEAVE;
		msgbody.RideMachineClt.Data.Leave.MachineID = nMonsterId;
		SendGameServerMsg( RIDE_MACHINE_CLT, msgbody );
	}
}

void MainGamePlayer::prepareDownMonsterMachine(int nMonsterId)
{
	if( isOnMonsterMachine() )
	{
		breakPreAction();
		tagCSPkgBody msgbody;
		msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_LEAVE_PRE;
		msgbody.RideMachineClt.Data.LeavePre.MachineID = nMonsterId;
		SendGameServerMsg(RIDE_MACHINE_CLT, msgbody);
	}
}

void MainGamePlayer::breakPrepareDownMonsterMachine(int nMonsterId)
{
	if( isOnMonsterMachine() )
	{
		tagCSPkgBody msgbody;
		msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_LEAVE_PRE_BRK;
		msgbody.RideMachineClt.Data.LeavePreBrk.MachineID = nMonsterId;
		SendGameServerMsg(RIDE_MACHINE_CLT, msgbody);
	}
}

void MainGamePlayer::requestMachineKickPlayer( int nId )
{
	tagCSPkgBody msgbody;
	msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_KICK;
	msgbody.RideMachineClt.Data.Kick.KickID = nId;
	SendGameServerMsg(RIDE_MACHINE_CLT,msgbody);
}

void MainGamePlayer::requestApplyMachineInvite( int id )
{
	tagCSPkgBody msgbody;
	msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_INVITE_ACCPET;
	msgbody.RideMachineClt.Data.InviteAccpet.MachineOwnerID = id;
	SendGameServerMsg( RIDE_MACHINE_CLT,msgbody );
}

void MainGamePlayer::requestRefuseMachineInvite( int id )
{
	tagCSPkgBody msgbody;
	msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_INVITE_REFUSE;
	msgbody.RideMachineClt.Data.InviteRefuse.MachineOwnerID = id;
	msgbody.RideMachineClt.Data.InviteRefuse.RefuseType = MACHINE_REFUSE_CANCEL;
	SendGameServerMsg( RIDE_MACHINE_CLT,msgbody );
}

void MainGamePlayer::requestMachineInvite( int id )
{
	tagCSPkgBody msgbody;
	msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_INVITE;
	msgbody.RideMachineClt.Data.Invite.MachineInviteID = id;
	SendGameServerMsg( RIDE_MACHINE_CLT,msgbody );
}

void MainGamePlayer::requestMachinePassenger( int nMachineID )
{
	tagCSPkgBody msgbody;
	msgbody.RideMachineClt.CltOp = RIDE_MACHINE_CLT_PASSENGER_UI;
	msgbody.RideMachineClt.Data.Passenger.MachineID = nMachineID;
	SendGameServerMsg(RIDE_MACHINE_CLT,msgbody);
}
void MainGamePlayer::requestOffLineGift()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_BUCHANG_REQ;
	msgbody.PlayerMiscClt.PlayerMiscCltData.BuChangReq.Day = getOfflineDay();
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}
void MainGamePlayer::setMoveTask(const SimpleFunction<> &task)
{
	SkillManager::getSingleton().onStop();
	delete m_moveTask;
	m_moveTask = new SimpleFunction<>(task);
}

void MainGamePlayer::clearMoveTask()
{
	delete m_moveTask;
	m_moveTask = NULL;
}

void MainGamePlayer::requestAcceptOfflineExp()
{
	if ( m_OneOffExp  > 0 )
	{
		tagCSPkgBody msgbody;
		msgbody.PlayerMiscClt.PlayerMiscType=PLAYER_OFFLINE_EXP_CLT;
		SendGameServerMsg(PLAYER_MISC_CLT,msgbody);
	}

}

void MainGamePlayer::requestCowRangeAward()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_TAKE_COW_AWARD;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::requestOfflineExpBuff()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_EXP_BUFF_AWARD;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::RefreshCmdTime()
{
	m_lastCmdTime = timeGetTime();
	if (isAFK())
	{
		setAFK(false);
	}
}

void MainGamePlayer::onAFK(bool b)
{
	GameActor::onAFK(b);
	GameMsgPrint::getSingleton().SendUIEvent("GE_PLAYER_AFK");
}

void MainGamePlayer::setAFK(bool b)
{
	if (b)
	{
		if (getActState().GetState() == GSACT_MOVE)
		{
			CmdMoveStop();
		}
		SkillManager::getSingleton().onStop();
	}

	tagCSPkgBody msgbody;
	msgbody.LeaveClt.LeaveState = b ? LEAVE_STAT_ENTER : LEAVE_STAT_NONE;
	SendGameServerMsg(LEAVE_CLT,msgbody);
	if (isAFK() ^ b)
	{
		onAFK(b);
	}
}

void MainGamePlayer::requestSendMotionAction( int nActionId,int nTargetId )
{
	tagCSPkgBody msgbody;
	msgbody.Motion.MotionID = nActionId;
	msgbody.Motion.TargetID = nTargetId;
	SendGameServerMsg( MOTION_CLT,msgbody );
}

void MainGamePlayer::setBattleLimitData(int nBeginTime, int nMaxNum, int nNum )
{
	m_nBattleBeginTime = nBeginTime;
	m_nBattleNum = nNum;
	m_nBattleMaxNum = nMaxNum;
	if( getBattleLimitLeftTime() > 0 )
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_BUF_LIST_UPDATE");
	}
}

int MainGamePlayer::getBattleLimitLeftTime()
{
	if( m_nBattleNum != 0 && m_nBattleNum >= m_nBattleMaxNum )
	{
		int serverTime = getServerTime().sec;
		// 4个小时
		return 4*60*60 - ( serverTime - m_nBattleBeginTime );
	}
	return -1;
}

void MainGamePlayer::SelectTarget( int resid, int resType )
{
	int actorType = GAT_PLAYER;
	if( resType == USE_TARGET_MONSTER )
	{
		actorType = GAT_MONSTER;
	}else if( resType == USE_TARGET_NPC )
	{
		actorType = GAT_NPC;
	}else if( resType == USE_TARGET_PLAYER )
	{
		actorType = GAT_PLAYER;
	}

	GameActor* pTarget = ActorManager::getSingleton().FindActor( GetSelectTarget() );
	if( pTarget != NULL )
	{
		return;
	}

	if( resid != 0 )
	{
		// 选择最近 目标
		GameActor* pActor = ActorManager::getSingleton().getNearestActor( GetID(), 15000, true, resid );
		if( pActor )
		{
			SetSelectTarget( pActor->GetID() );
		}
	}else{
		GameActor* pActor = ActorManager::getSingleton().getNearestActor( GetID(), 15000, true, 0, actorType );
		if( pActor )
		{
			SetSelectTarget( pActor->GetID() );
		}		
	}
}

int MainGamePlayer::ContinueMove()
{
	if (m_lastMoveToFlag && m_lastAutoTalkNpcId != 0)
	{
		return AutoTalkNpc::getInstant()->talkByResId(m_lastAutoTalkNpcId);
	}
	if (m_mustContinueAutoMove || getAutoFind())
	{
		return continueLastAutoFind();
	}
	if (m_lastMoveToFlag)
	{
		return CmdMoveTo(m_lastMoveToX, m_lastMoveToY);
	}
	return MOVE_ALREADY_IN_DEST;
}

int MainGamePlayer::ContinueAutoFind()
{
	if (m_lastMoveToFlag && m_lastAutoTalkNpcId != 0)
	{
		return AutoTalkNpc::getInstant()->talkByResId(m_lastAutoTalkNpcId);
	}
	if (m_mustContinueAutoMove || getAutoFind())
	{
		return continueLastAutoFind();
	}
	return MOVE_ALREADY_IN_DEST;
}

void MainGamePlayer::SetAutoMove(bool autoMove)
{
	m_isAutoMove = autoMove;
}

bool MainGamePlayer::IsAutoMove()
{
	return m_isAutoMove;
}

void MainGamePlayer::setContinueAutoMove()
{
	m_mustContinueAutoMove = true;
}

int MainGamePlayer::isColorFontValid()
{
	unsigned int serverTime = getServerTime().sec;
	return m_nColorFontEndTime - serverTime;
}

tagReviveDef_ToLua* MainGamePlayer::getReviveDef()
{
	static_assert( sizeof(tagReviveDef_ToLua) == sizeof(tagReviveDef) );
	GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		return (tagReviveDef_ToLua*)DBDataManager::getSingleton().getReviveDef( pMainPlayer->getLv() );
	}
	return NULL;
}

tagLevelExpDef_ToLua* MainGamePlayer::getLevelExpDef( int nLevel )
{
	static_assert( sizeof(tagLevelExpDef_ToLua) == sizeof(tagLevelExpDef) );
	return (tagLevelExpDef_ToLua*)DBDataManager::getSingleton().getLevelExpDef( nLevel );
}

tagLevelBuffExpDef_ToLua* MainGamePlayer::getLevelBuffExpDef()
{
	static_assert( sizeof(tagLevelBuffExpDef_ToLua) == sizeof(tagLevelBuffExpDef) );
	GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		return (tagLevelBuffExpDef_ToLua*)DBDataManager::getSingleton().getLevelBuffExpDef( pMainPlayer->getLv() );
	}
	return NULL;
}

tagOfflineExpParameter_ToLua* MainGamePlayer::getOfflineExpParameter( int nType )
{
	static_assert( sizeof(tagOfflineExpParameter_ToLua) == sizeof(tagOfflineExpParameter) );
	return (tagOfflineExpParameter_ToLua*)DBDataManager::getSingleton().getOfflineExpParameter( nType );
}

void MainGamePlayer::trySendCacheMove(bool force /*= false*/)
{
	if (m_lastCached)
	{
		unsigned int now = timeGetTime();
		if (force || now > m_LastCmdMoveTick + 150)
		{
			int cur_x, cur_z;
			m_pMapMover->GetCurPos(cur_x, cur_z);
			GameMaskPath path;
			if( m_pCurMap && !m_pCurMap->FindPath(path, m_lastCmdMoveX, m_lastCmdMoveY, cur_x, cur_z, MASK_STOP | DYN_MASK_STOP, true ) )
			{
				return;
			}
			if (m_pMapMover->m_CurSeg >= 0)
			{
				for (int i = m_pMapMover->m_CurSeg + 1; i < (int)m_pMapMover->m_CurPath.getNumPoint(); ++i)
				{
					Point2D pt = m_pMapMover->m_CurPath.getPathPoint(i);
					path.addPathPoint(pt.x, pt.y);
				}
			}
			size_t npoint = path.getNumPoint();
			if( npoint - 1 > CS_MAX_MOVEPOINTS/2 )
			{
				path.limitPathPoint( CS_MAX_MOVEPOINTS/2 + 1 );
				npoint = CS_MAX_MOVEPOINTS/2 + 1;
			}
			Point2D pt;
			tagCSPkgBody msgbody;
			msgbody.MoveReq.MoveType = CS_MOVE_WALK;
			msgbody.MoveReq.MoveSeq = m_CurMoveSeq++;
			msgbody.MoveReq.x = m_lastCmdMoveX;
			msgbody.MoveReq.y = m_lastCmdMoveY;
			CSMOVEDATA &mdata = msgbody.MoveReq.Data;
			mdata.Walk.Total = int(npoint - 1);
			for(char i=1; i<= mdata.Walk.Total; i++)
			{
				pt = path.getPathPoint(i);
				mdata.Walk.Points[i-1].x = pt.x;
				mdata.Walk.Points[i-1].y = pt.y;
			}
			msgbody.MoveReq.SvrTime.BenchTimeSec = m_lastBenchTimeSec;
			msgbody.MoveReq.SvrTime.BenchTimeUSec = m_lastBenchTimeUSec;
			SendGameServerMsg(MOVE_CLT, msgbody);
			m_LastCmdMoveTick = now;
			m_moving = true;
			m_lastCached = false;
		}
// 		if( isOnMonsterMachine() && m_MonsterMachine.getMonsterEnergy() == 0 )
// 		{
// 			CmdMoveStop();			
// 		}
	}
}

void MainGamePlayer::RequestOpenBaoXiang( int nListType, int nGridIdx )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_BAOXIANG_OPEN;
	msgbody.PlayerMiscClt.PlayerMiscCltData.BaoXiangOpen.ListType = nListType;
	msgbody.PlayerMiscClt.PlayerMiscCltData.BaoXiangOpen.GridIdx = nGridIdx;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::requestHangHaiRangeAward()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_TAKE_HANG_HAI_AWARD;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::requestYangHaiBangAdd( int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_YANGHAIBANG_ADD;
	msgbody.PlayerMiscClt.PlayerMiscCltData.YangHaiBangAdd.Num = nNum;
	SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
}

void MainGamePlayer::requestYangHaiBangAward()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_YANGHAIBANG_AWARD;
	SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
}

void MainGamePlayer::requestClanWarAward()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_TAKE_CLAN_WAR_AWARD;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void MainGamePlayer::requestPaoShangOpt( int nType, int nItemId, int nPrice, int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_PAOSHANG_UI_SELECT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PaoShangUiSelect.BuySellType = nType;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PaoShangUiSelect.ItemID = nItemId;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PaoShangUiSelect.Price = nPrice;
	msgbody.PlayerMiscClt.PlayerMiscCltData.PaoShangUiSelect.Num = nNum;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

bool MainGamePlayer::hasLover()
{
	if ( m_nLoveColorType == INTIMATE_COLOR_LOVERS || m_nLoveColorType == INTIMATE_COLOR_SPOUSE )
	{
		return true;
	}
	return false;
}

void MainGamePlayer::resetLoveInfo()
{
	m_nLoveColorType = 0;
	strcpy(m_szLoverName,"");
	m_loverID = 0;
}

bool MainGamePlayer::isInPVPMap()
{
	DBDataManager& dbMgr = DBDataManager::getSingleton();

	tagMapDef mapDef = dbMgr.getMapDef(this->getMapId());
	return mapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP;
}