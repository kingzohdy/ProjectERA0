#include "../UILib/ui_gameui.h"
#include "GameSelectManager.h"
#include "OgreRay.h"
#include "OgreCamera.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Common.h"
#include "GameShortCut.h"
#include "GamePlayManager.h"
#include "GameCamera.h"
#include "GameActor.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "OgreVector3.h"
#include "OgreRect.h"
#include "OgreEntity.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "OgreGameScene.h" 
#include "GameManager.h"
#include "GameNpcDialog.h"
#include "../UILib/ui_framemgr.h"
#include "../UILib/ui_scriptfunc.h"
#include "../UILib/ui_frame.h"
#include "GameChatManager.h"
#include "GameCollect.h"
#include "GameMainPlayer.h"
#include "GameStallManager.h"
#include "OgreInputManager.h"
#include "GameQuest.h"
#include "SecondarySkillMgr.h"
#include "ItemPicker.h"
#include "GameExchange.h"
#include "GameMaxMap.h"
#include "MultiMapAutoFind.h"
#include "GameExActorMgr.h"
#include "GameHomelandManager.h"

using namespace Ogre;

const int MAX_KEEP_LEFT_DOWN_TIME = 1000;
GameSelectManager::GameSelectManager(void) : m_pSelEntity(NULL), 
											 m_pClickEntity(NULL), 
											 m_PreSelId(0),
											 m_pSelectEff(NULL),
											 m_bCancelSelect(false),
											 m_bAllowNow(false),
											 m_bLastCmd(false),
											 m_pDirDecal(NULL),
											 m_pRectDecal(NULL),
											 m_selectMode(NONE),
											 m_nRadio(0)
{
	m_AutoLDownInfo.first	= false;
	m_AutoLDownInfo.second	= 0;
	m_nLastHighlightItemID	= 0;
	m_nLastStallActorID		= 0;
	GameMsgPrint::getSingleton().registGameMsgHandler( this );
	GameManager::getSingleton().addRightBtnCallBack( GameSelectManager::onRightButton );
	GameManager::getSingleton().addLeftBtnCallBack( GameSelectManager::onLeftButton );
}

GameSelectManager::~GameSelectManager(void)
{
	GameMsgPrint::getSingleton().unRegistGameMsgHandler( this );
	GameManager::getSingleton().unRightBtnCallBack( GameSelectManager::onRightButton  );
	GameManager::getSingleton().unLeftBtnCallBack( GameSelectManager::onLeftButton  );

	if( m_pSelEntity != NULL )
	{
		OGRE_RELEASE(m_pSelEntity);
	}
	if (m_pDirDecal != NULL)
	{
		OGRE_RELEASE(m_pDirDecal);
	}
	if (m_pRectDecal != NULL)
	{
		OGRE_RELEASE(m_pRectDecal);
	}

	OGRE_RELEASE(m_pClickEntity);
}

void GameSelectManager::onLButtonDown( const Ogre::InputEvent &event )
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	FollowCamera*	pMainCamera = GamePlayManager::getSingleton().getMainCamera();
	GameMap*		pCurMap		= GamePlayManager::getSingleton().getCurMap();		
	bool bHomelandPlanting = GameHomelandManager::getSingleton().isInSelfHome() && GameHomelandManager::getSingleton().isInPlantMode();
	if( pMainPlayer == NULL || pMainCamera == NULL || pCurMap == NULL || ( m_bCancelSelect && !bHomelandPlanting ) )
	{
		m_bCancelSelect = false;
		return;
	}

	Ogre::WorldRay ray;
	pMainCamera->GetCameraNode()->getViewRayByScreenPt(&ray, event.mouse.x, event.mouse.y);
	float dist;
	if( !pCurMap->getShowGameMap()->pickGround( ray, &dist ) )
	{
		return;
	}
	Ogre::WorldPos pickpoint = ray.m_Origin + ray.m_Dir*dist;
	int x = (int)Ogre::WorldPos::Fix2Flt(pickpoint.x);
	int z = (int)Ogre::WorldPos::Fix2Flt(pickpoint.z);
	
	// 处理处于select状态下的法术
	GameSpell *pSelectedSpell = SkillManager::getSingleton().getIntentSpell();
	if( pSelectedSpell != NULL )
	{
		GameXYZ pos = m_pSelectPos;

		if (m_selectMode == ANGLE)
		{
			pMainPlayer->SetFaceTo(m_nLastAngle + 180, true);
			GameXYZ selfPos = pMainPlayer->GetPosition();
			pos = selfPos;
			pos.x += (int)(15000 * Cos((float)pMainPlayer->getAngle()));
			pos.z -= (int)(15000 * Sin((float)pMainPlayer->getAngle()));
		}
		SkillManager::getSingleton().testResult(SkillManager::getSingleton().useSpellOnce(pSelectedSpell->getSpellId(), pos.x, pos.z));
		return;
	}
	//处理怪物机械集气
	if( pMainPlayer->isOnMonsterMachine() && pMainPlayer->getMonsterMachine()->isInCollectSkill() )
	{
		return;
	}

	// 处理shitf+LButton
	if( onShiftLButtonDown( pMainPlayer, x, z ) )
	{
		return;
	}
	GameActor* pActor = ExtraActorMgr::getSingleton().mounseOnActor( ray );
	ExtraActorMgr::getSingleton().setNpcTalkID(0);
	ExtraActorMgr::getSingleton().setLastNpcTalkID(0);
	if ( pActor == NULL )
	{
		pActor = ActorManager::getSingleton().pickActor( ray, PICK_TYPE_RBUTTON_DOWN );
	}
	else if( pActor->getType() == GAT_EXTRA_NPC )
	{
		ExtraActorMgr::getSingleton().setNpcTalkID( pActor->GetID() );
		pMainPlayer->setAutoFind( false );
		Ogre::Point2D start(x, z);
		pMainPlayer->CmdMoveTo( x, z );
		return;
	}
//	GameActor* pActor   = ActorManager::getSingleton().pickActor( ray, PICK_TYPE_LBUTTON_DOWN );
	for (std::set<IMouseEventAction *, PtrLess>::iterator i = m_lstMouseEventAction.begin(); i != m_lstMouseEventAction.end(); ++i)
	{
		if ((*i)->onLButtonDown(pActor, x, z))
		{
			return;
		}
	}
	// 处理ctrl+LButton
	if( onCtrlLButtonDown( pMainPlayer, pActor, x, z ) )
	{
		return;
	}
	// 处理alt+LButton
	if( onAltLButtonDown( pMainPlayer, pActor, x, z ) )
	{
		return;
	}

	if (!pMainPlayer->canControl() && pMainPlayer->m_pCarry->getCarryState() != eCarrying )
	{
		return;
	}

	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
	if (mutilMapFinder.IsAutoMoving())
	{
		return;
	}

	const char *cursor = GameManager::getSingleton().GetGameUI()->getCurrentCursor();
	if (stricmp(cursor, "exchange") == 0 || stricmp(cursor, "disableExchange") == 0)
	{
		OnExchangeLButtonDown( pActor );
		return;
	}
	
	if ( pMainPlayer->canAutoTalkToNpc() )
	{
		pMainPlayer->endAutoTalkToNpc();
	}

	if( pActor != NULL )
	{
		if( selectTarget( pMainPlayer, pActor ) )
		{
			// TODO: 这要打断自动与NPC对话
			return;
		}
		if (isMouseSwap() && pActor->GetID() == pMainPlayer->GetSelectTarget())
		{
			// TODO: 这要打断自动与NPC对话
			int nLeftSkillId = GameShortCut::getSingleton().getNormalAttackSkill();
			if( nLeftSkillId == 0)
			{
				// 若主角处于自动寻路的状态,则终止主角的自动寻路状态
				pMainPlayer->setAutoFind( false );
				Ogre::Point2D start(x, z);
				pMainPlayer->CmdMoveTo( x, z );
				showSelCursor( x, z );
				m_AutoLDownInfo.first	= false;
				m_AutoLDownInfo.second	= 0;
				return;
			}
			GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
			if (pSpell == NULL)
			{
				if( (!pMainPlayer->m_Rider.isOnHorse() || pMainPlayer->m_bChangeSkillShortCut) && 
						SkillManager::getSingleton().normalAttack(nLeftSkillId) )
				{
					return;
				}
				pMainPlayer->setAutoFind( false );
				SkillManager::getSingleton().onStop();
				if ( needContinueMove( pMainPlayer->CmdMoveTo( x, z ) ) )
				{
					m_pLastCmdX = x;
					m_pLastCmdY = z;
					m_bLastCmd = true;
				}
				m_AutoLDownInfo.first	= false;
				m_AutoLDownInfo.second	= 0;
			}
			else if (SkillManager::getSingleton().canMoveAttack(pSpell->getSpellId()))
			{
				pMainPlayer->setAutoFind( false );
				if ( needContinueMove( pMainPlayer->CmdMoveTo( x, z ) ) )
				{
					m_pLastCmdX = x;
					m_pLastCmdY = z;
					m_bLastCmd = true;
				}
			}
		}
		
		// 若鼠标处于不支持点击行走状态
		if (GetCurrentCursorLevel() >= MAX_LEVEL_CAN_MOVE)
		{
			return;
		}

		GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
		if (pSpell != NULL)
		{
			if (pSpell->getState() == GameSpell::STATE_COLLECT)
			{
				return;
			}
			tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
			if (attrib && attrib->m_isNoBreak)
			{
				return;
			}
		}
	}

	// 若鼠标处于不支持点击行走状态
	if (GetCurrentCursorLevel() >= MAX_LEVEL_CAN_MOVE)
	{
		return;
	}

	GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
	if (pSpell != NULL)
	{
		if (pSpell->getState() == GameSpell::STATE_COLLECT)
		{
			return;
		}
		tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
		if (attrib && attrib->m_isNoBreak)
		{
			return;
		}
	}
	
	// 若主角处于自动寻路的状态,则终止主角的自动寻路状态
	pMainPlayer->setAutoFind( false );
	Ogre::Point2D start(x, z);
	if (pActor == NULL || pActor->getSubType() == MON_SUB_TYPE_TRAP 
		|| pActor->getSubType() == MON_SUB_TYPE_BUILD || pActor->getType() == GAT_ITEM 
		|| pActor->getSubType() == MON_SUB_TYPE_MACHINE )
	{
		if( pActor && pActor->getType() == GAT_ITEM )
		{
			const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( pActor->getResId() );
			if( itemDef.ItemTmp.Type == ITEM_TMP_NO )
			{
				return;
			}
		}
		SkillManager::getSingleton().onStop();
		if ( needContinueMove( pMainPlayer->CmdMoveTo( x, z ) ) )
		{
			m_pLastCmdX = x;
			m_pLastCmdY = z;
			m_bLastCmd = true;
		}
		showSelCursor( x, z );
		m_AutoLDownInfo.first	= false;
		m_AutoLDownInfo.second	= 0;
	}
}

void GameSelectManager::onRButtonDown(const Ogre::InputEvent &event)
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	FollowCamera*	pMainCamera = GamePlayManager::getSingleton().getMainCamera();
	GameMap*		pCurMap		= GamePlayManager::getSingleton().getCurMap();		
	int				nCheck		= 0;
	SkillManager::getSingleton().clearIntentSpell();
	if( pMainPlayer == NULL || pMainCamera == NULL || pCurMap == NULL )
	{
		return;
	}
	//if( pMainPlayer->m_pCarry->getCarryState() == eCarrying )
	//{
		//现在先不需要 右键放下
		//pMainPlayer->m_pCarry->unCarry(0);
	//}
	Ogre::WorldRay ray;
	pMainCamera->GetCameraNode()->getViewRayByScreenPt(&ray, event.mouse.x, event.mouse.y);
	float dist;
	if( !pCurMap->getShowGameMap()->pickGround( ray, &dist ) )
	{
		return;
	}
	Ogre::WorldPos pickpoint = ray.m_Origin + ray.m_Dir*dist;
	int x = (int)Ogre::WorldPos::Fix2Flt(pickpoint.x);
	int z = (int)Ogre::WorldPos::Fix2Flt(pickpoint.z);

	GameActor* pActor = ExtraActorMgr::getSingleton().mounseOnActor( ray );
	ExtraActorMgr::getSingleton().setNpcTalkID( 0 );
	if ( pActor == NULL )
	{
		pActor = ActorManager::getSingleton().pickActor( ray, PICK_TYPE_RBUTTON_DOWN );
	}
	else if( pActor->getType() == GAT_EXTRA_NPC )
	{
		return;
	}
//	GameActor* pActor = ActorManager::getSingleton().pickActor( ray, PICK_TYPE_RBUTTON_DOWN );
	for (std::set<IMouseEventAction *, PtrLess>::iterator i = m_lstMouseEventAction.begin(); i != m_lstMouseEventAction.end(); ++i)
	{
		if ((*i)->onRButtonDown(pActor, x, z))
		{
			return;
		}
	}
	// 处理ctrl+RButton
	if( onCtrlRButtonDown( pMainPlayer, pActor, x, z ) )
	{
		return;
	}
	// 处理alt+RButton
	if( onAltRButtionDown( pMainPlayer, pActor, x, z ) )
	{
		return;
	}

	//家园种植
	if ( GameHomelandManager::getSingleton().isInSelfHome() && GameHomelandManager::getSingleton().isInPlantMode() ) {
			GameHomelandManager::getSingleton().cancelPlantMode();
			return;
	}
	if( pActor != NULL && !pActor->isCamp( SKILL_TARGET_SELF ) )
	{
		if( pActor->getSubType() == MON_SUB_TYPE_COLLECT
			&& (!SecondarySkillMgr::getSingleton().canCollect(pActor->getResId()) ||
			!GameQuest::getSingleton().canCollectQuest( pActor->getResId())))
		{
			return;
		}

		// 拾取物品
		if( pActor->getType() == GAT_ITEM )
		{
			const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( pActor->getResId() );
			if( (itemDef.ItemTmp.Type == ITEM_TMP_NO || pActor->m_IsPickItem) && pActor->getRoleWId() == pMainPlayer->getRoleWId() )
			{
				ItemPicker::getSingleton().tryPickItem( pActor->GetID() );
				return;
			}
		}

		do
		{
// 			if( pMainPlayer->m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE )
// 			{
// 				break;
// 			}

			if( pActor->getSubType() == MON_SUB_TYPE_COLLECT 
				&& (!SecondarySkillMgr::getSingleton().canCollect(pActor->getResId())
				|| !GameQuest::getSingleton().canCollectQuest( pActor->getResId())) )
			{
				break;
			}

			if (pActor->getSubType() == MON_SUB_TYPE_TRAP 
				|| pActor->getSubType() == MON_SUB_TYPE_BUILD )
			{
				/// 不允许选中图腾和地雷
				break;;
			}

			if( pActor->getType() == GAT_ITEM )
			{
				break;
			}
			pMainPlayer->SetSelectTarget( pActor->GetID() );
		} while (false);

		if (!isMouseSwap() && pActor->GetID() == pMainPlayer->GetSelectTarget())
		{
			if ( pMainPlayer->canAutoTalkToNpc() )
			{
				pMainPlayer->endAutoTalkToNpc();
			}
			
			int nLeftSkillId = GameShortCut::getSingleton().getNormalAttackSkill();
			if( nLeftSkillId == 0)
			{
				return;
			}

			GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
			if (pSpell == NULL)
			{
				if( (!pMainPlayer->m_Rider.isOnHorse() || pMainPlayer->m_bChangeSkillShortCut) && 
						SkillManager::getSingleton().normalAttack(nLeftSkillId) )
				{
					return;
				}
			}
			else if (SkillManager::getSingleton().canMoveAttack(pSpell->getSpellId()))
			{
				pMainPlayer->setAutoFind( false );
				if ( needContinueMove( pMainPlayer->CmdMoveTo( x, z ) ) )
				{
					m_pLastCmdX = x;
					m_pLastCmdY = z;
					m_bLastCmd = true;
				}
			}
			
			// TODO:
// 			pMainPlayer->setAutoFind( false );
// 			SkillManager::getSingleton().onStop();
// 			if ( needContinueMove( pMainPlayer->CmdMoveTo( x, z ) ) )
// 			{
// 				m_pLastCmdX = x;
// 				m_pLastCmdY = z;
// 				m_bLastCmd	= true;
// 			}
// 			m_AutoLDownInfo.first	= false;
// 			m_AutoLDownInfo.second	= 0;
		}
	}
	/*m_AutoLDownInfo.first	= false;
	m_AutoLDownInfo.second	= 0;*/
}

void GameSelectManager::onMouseMove( const Ogre::InputEvent &event )
{
	GameMap*		pCurMap		= GamePlayManager::getSingleton().getCurMap();	
	FollowCamera*	pMainCamera = GamePlayManager::getSingleton().getMainCamera();
	if( pMainCamera == NULL || pCurMap == NULL )
	{
		return;
	}
	int bNeedChange = 0;
	if (GetCurrentCursorLevel() == eCursorMoveChange )
	{
		if(!UIIsInDragState())
		{
			bNeedChange = 1;
		}
		if( GameHomelandManager::getSingleton().isInSelfHome() && GameHomelandManager::getSingleton().isInPlantMode())
		{
			bNeedChange = 2;
		}
	}
	
	Frame* pFrame = g_pFrameMgr->FindUIObjectOnPoint( event.mouse.x, event.mouse.y, false );
	if( pFrame != NULL )
	{
		if( !pFrame->m_bInputTransparent && pFrame->m_FrameStrata != FS_PARENT )
		{
			return;
		}
	}
	Ogre::WorldRay ray;
	float dist;
	pMainCamera->GetCameraNode()->getViewRayByScreenPt(&ray, event.mouse.x, event.mouse.y);
	
	if( pCurMap->getShowGameMap()->pickGround( ray, &dist ) )
	{
		Ogre::WorldPos pickpoint = ray.m_Origin + ray.m_Dir*dist;
		int x = (int)Ogre::WorldPos::Fix2Flt(pickpoint.x);
		int z = (int)Ogre::WorldPos::Fix2Flt(pickpoint.z);
		Ogre::Point2D start(x, z);
		Ogre::Point2D scale(pCurMap->getMaskMap()->m_lMaskPixelWidth, pCurMap->getMaskMap()->m_lMaskPixelHeight);
		Ogre::Point2D s_StartGrid = start / scale;
		
		if( GameManager::getSingleton().GetGameUI()->isCursorDrag() && bNeedChange != 2 )
		{
			return;
		}
		
		int y;
		bool b = pCurMap->getTerrainH(x*10, z*10, y);
		assert(b);
		Ogre::WorldPos pos( Ogre::Vector3( float(x*10), float(y), float(z*10) ) );
		pos = pos/10;
		m_pSelectPos = pos;

		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

		if( m_pSelectEff != NULL && m_selectMode == POINT )
		{
			GameXYZ selfPos = pMainPlayer->GetPosition();
			selfPos.y = m_pSelectPos.y;
			bool bAllow = selfPos.subtract(m_pSelectPos).length() - m_nAllowDis <= 0.001;
			if (bAllow != m_bAllowNow)
			{
				m_bAllowNow = bAllow;
				m_pSelectEff->detachFromScene();
				OGRE_RELEASE(m_pSelectEff);
				m_pSelectEff = new Ogre::Entity;
				m_pSelectEff->load( m_bAllowNow ? m_szAllowEff.c_str() : m_szUnAllowEff.c_str(), false );
				m_pSelectEff->update(0);
				BoxBound &boxBound = m_pSelectEff->getWorldBounds().getBox();
				Vector3 scale((float)m_nRadio);
				scale = scale / (boxBound.getMaxPos() - boxBound.getMinPos());
				scale.y = 1;
				m_pSelectEff->setScale(scale);
				m_pSelectEff->attachToScene( pCurMap->getShowGameMap()->getGameScene() );
			}
			m_pSelectEff->setPosition(m_pSelectPos);
			if (m_bSpin)
			{
				int angle = FaceAngleBetween(selfPos.x, selfPos.z, m_pSelectPos.x, m_pSelectPos.z);
				m_pSelectEff->setRotation(angle - 90.0f, 0, 0);
			}
		}
		GameSpell *pSpell = SkillManager::getSingleton().getAngleSpell();
		if ( pSpell && m_selectMode == ANGLE )
		{
			GameXYZ selfPos = pMainPlayer->GetPosition();
			selfPos.y = m_pSelectPos.y;
			int angle = FaceAngleBetween(selfPos.x, selfPos.z, m_pSelectPos.x, m_pSelectPos.z);
			pMainPlayer->SetFaceTo(angle, true);
	//		pMainPlayer->sendLockFace(ATTACK_LOCK_DIR, pMainPlayer->getAngle(), pSpell->getSpellId());
		}

		static int nMoveInActorId = 0;

		int nMaskBit = 1;
		if( pMainPlayer != NULL && pMainPlayer->getActState().GetState() == GSACT_FLY )
		{
			nMaskBit = 4;
		}
		GameActor* pActor = ExtraActorMgr::getSingleton().mounseOnActor( ray );
		if ( pActor == NULL )
		{
			pActor = ActorManager::getSingleton().mounseOnActor( ray );
		}
		else if( pActor->getType() == GAT_EXTRA_NPC )
		{
			// 栏位npc
			GameManager::getSingleton().GetGameUI()->SetCurrentCursor("speak");
			return;
		}
		int actorId = 0;
		if (pActor && pActor->isSameRoleWId(ActorManager::getSingleton().getMainPlayer()))
		{
			actorId = pActor->GetID();
		}
		if (m_nLastHighlightItemID != actorId)
		{
			GameActor *lastActor = ActorManager::getSingleton().FindActor(m_nLastHighlightItemID);
			if (lastActor && lastActor->getType() == GAT_ITEM )
			{
				const tagItemDef &itemDef  = DBDataManager::getSingleton().getItemDef(lastActor->getResId());
				if (itemDef.ItemTmp.Type == ITEM_TMP_NO)
				{
					char szScript[256];
					sprintf( szScript, "ItemOutFocus(%d)", m_nLastHighlightItemID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}
			}
			
			GameActor *pActor = ActorManager::getSingleton().FindActor(actorId);
			if (pActor && pActor->getType() == GAT_ITEM )
			{
				const tagItemDef &itemDef  = DBDataManager::getSingleton().getItemDef(pActor->getResId());
				if (itemDef.ItemTmp.Type == ITEM_TMP_NO)
				{
					char szScript[256];
					sprintf( szScript, "ItemInFocus(%d)", actorId );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}
			}
			m_nLastHighlightItemID = actorId;
		}
		//摊位设置
		int stallID = 0;
		if ( pActor && pActor->getType() == GAT_PLAYER && pActor->isInStall())
		{
			stallID = pActor->GetID();
		}
		if( m_nLastStallActorID != stallID )
		{
			GameActor *lastActor = ActorManager::getSingleton().FindActor( m_nLastStallActorID );
			if ( lastActor && lastActor->getType() == GAT_PLAYER )
			{
				char szScript[256];
				sprintf( szScript, "StallOutFocus(%d)", m_nLastStallActorID );
				GameManager::getSingleton().getScriptVM()->callString( szScript );
			}
			GameActor* pActor = ActorManager::getSingleton().FindActor( stallID );
			if ( pActor && pActor->getType() == GAT_PLAYER && pActor->isInStall() )
			{
				char szScript[256];
				sprintf( szScript, "StallInFocus(%d)", stallID );
				GameManager::getSingleton().getScriptVM()->callString( szScript );
			}
			m_nLastStallActorID = stallID;
		}

		if( pActor == NULL )
		{
			if (bNeedChange == 1)
			{
				if ( pCurMap->GridHasMask( s_StartGrid.x, s_StartGrid.y, nMaskBit ) )
				{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("normal");
				}
				else
				{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("point");
				}
			}		
			
			GameActor* pPreActor = ActorManager::getSingleton().FindActor( nMoveInActorId );
			if ( pPreActor != NULL )
			{
				pPreActor->getRoleText().setEnableDrawTitle( false );
			}
			nMoveInActorId = 0;
		}
		else if(bNeedChange )
		{			
			if( pActor->getType() == GAT_MONSTER )
			{
				if( GameCollect::getSingleton().canCollect( pActor ) && GameCollect::getSingleton().limitMachineCollectSimple(pActor->getResId()) )
				{
					if( SecondarySkillMgr::getSingleton().canCollect(pActor->getResId()) &&
						GameQuest::getSingleton().canCollectQuest(pActor->getResId()) )
					{
						int nRace = DBDataManager::getSingleton().GetNpcDisplayInfo( pActor->getResId()).race;
						if (nRace&MONSTER_PET)
						{
							GameManager::getSingleton().GetGameUI()->SetCurrentCursor("rope");
						}
						else
						{
							if( GameQuest::getSingleton().IsCollectNeedItem(pActor->getResId()) )
							{
								GameManager::getSingleton().GetGameUI()->SetCurrentCursor("taskCollect");
							}else{
								GameManager::getSingleton().GetGameUI()->SetCurrentCursor("mine");
							}							
						}
					}
				}else if(pActor->getSubType() != MON_SUB_TYPE_COLLECT && pActor->isCamp(SKILL_TARGET_ENEMY_MON))
				{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("attack");
				}else if( pActor->getSubType() == MON_SUB_TYPE_MACHINE )
				{
					MonsterMachine* pMonster = pActor->getMonsterMachine();
					if( pMonster->isSameCamp() && pMonster->canRideOn() )
					{
						GameManager::getSingleton().GetGameUI()->SetCurrentCursor("loot");
					}
					else if (pActor->isCamp(SKILL_TARGET_ENEMY_MON) || pActor->isCamp(SKILL_TARGET_ENEMY_PLAYER))
					{
						GameManager::getSingleton().GetGameUI()->SetCurrentCursor("attack");
					}
				}
			}
			else if( pActor->getType() == GAT_NPC )
			{
				int nRace = DBDataManager::getSingleton().GetNpcDisplayInfo( pActor->getResId()).race;
				if (nRace&MONSTER_SKILLNPC)
				{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("learn");
				}else if(nRace&MONSTER_SHOPNPC)
				{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("buy");
				}else if ( GameHomelandManager::getSingleton().isInPlantMode() && GameHomelandManager::getSingleton().getFarmSoilIndex( pActor ) > 0 ) {

				}else{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("speak");
				}
			}
			else if( pActor->getType() == GAT_ITEM )
			{
				const tagItemDef &itemDef  = DBDataManager::getSingleton().getItemDef(pActor->getResId());
				if( itemDef.ItemTmp.Type == ITEM_TMP_NO || pActor->m_IsPickItem )
				{
					if( pActor->getRoleWId() == pMainPlayer->getRoleWId() 
						|| pActor->isRollItem() )
					{
						GameManager::getSingleton().GetGameUI()->SetCurrentCursor("loot");
					}
				}else{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("loot");
				}
			}
			else if( pActor->getType() == GAT_PLAYER || pActor->getType() == GAT_PET)
			{
				//根据pvp状态设置鼠标
				if( pActor->m_ObjCamp & SKILL_TARGET_ENEMY_PLAYER ||					
					pActor->m_ObjCamp & SKILL_TARGET_ENEMYPET)
				{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("attack");
				}else{
					GameManager::getSingleton().GetGameUI()->SetCurrentCursor("normal");
				}
				// 若是他人的宠物，则允许显示其主人名字信息
			}else if( pActor->getType() == GAT_MAINPLAYER )
			{
				GameManager::getSingleton().GetGameUI()->SetCurrentCursor("normal");
			}

			if( pActor->GetID() != nMoveInActorId )
			{
				GameActor* pPreActor = ActorManager::getSingleton().FindActor( nMoveInActorId );
				if( pPreActor != NULL )
				{
					pPreActor->getRoleText().setEnableDrawTitle( false );
				}

				if( pActor->getType() == GAT_MONSTER || pActor->getType() == GAT_ITEM || 
					pActor->getType() == GAT_PLAYER || pActor->getType() == GAT_PET || 
					pActor->getType() == GAT_MAINPLAYER )
				{
					nMoveInActorId = pActor->GetID();
					pActor->getRoleText().setEnableDrawTitle( true );
					
					if( pActor->getSubType() == MON_SUB_TYPE_COLLECT
						&&(!SecondarySkillMgr::getSingleton().canCollect(pActor->getResId())
						||!GameQuest::getSingleton().canCollectQuest( pActor->getResId())) )
					{
						pActor->getRoleText().setEnableDrawTitle( false );
					}
				}
				else
				{
					nMoveInActorId = 0;
				}
			}
		}	
	}
}

void GameSelectManager::update( unsigned int dtick )
{
	MainGamePlayer *pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if( m_pClickEntity != NULL )
	{
		m_pClickEntity->update( dtick );
	}

	GameSpell *pSpell = SkillManager::getSingleton().getTurnSpell();
	if( pSpell != NULL )
	{
		GameXYZ pos = pMainplayer->GetPosition();
		int angle = FaceAngleBetween(pos.x, pos.z, m_pSelectPos.x, m_pSelectPos.z);
		pMainplayer->SetFaceTo(angle, true);
		pMainplayer->sendLockFace(ATTACK_LOCK_DIR, pMainplayer->getAngle(), pSpell->getSpellId());
		pSpell->setTargetPoint(pos.x + (int)(50 * Cos((float)pMainplayer->getAngle())),
			pos.z - (int)(50 * Sin((float)pMainplayer->getAngle())));
	}
	GameSpell *pIntentSpell = SkillManager::getSingleton().getIntentSpell();
	if( pIntentSpell != NULL )
	{
		GameManager::getSingleton().registerSelfHandler( true );
	}else{
		GameManager::getSingleton().registerSelfHandler( false );
	}

	if( m_pSelectEff != NULL )
	{
		if (m_selectMode == ANGLE)
		{
			if( pMainplayer != NULL )
			{
				GameXYZ pos = pMainplayer->GetPosition();
				m_pSelectEff->setPosition(pos);
				if (m_pSelectPos.subtract(pos).length() > MINSELECT)
				{
					m_nLastAngle = FaceAngleBetween(m_pSelectPos.x, m_pSelectPos.z, pos.x, pos.z);
				}
				GameSpell *pSpell = SkillManager::getSingleton().getReadySpell();
				if (pSpell)
				{
					pMainplayer->SetFaceTo(m_nLastAngle + 180, true);
					pMainplayer->sendLockFace(ATTACK_LOCK_DIR, pMainplayer->getAngle(), pSpell->getSpellId());
					pSpell->setTargetPoint(pos.x + (int)(15000 * Cos((float)pMainplayer->getAngle())),
						pos.z - (int)(15000 * Sin((float)pMainplayer->getAngle())));
				}
				//对怪物机械的判断
/*
				if( pMainplayer->isOnMonsterMachine() )
				{
					pMainplayer->getMonsterMachine()->setCollectSelectData( m_nLastAngle );
					pos.x = pMainplayer->getMonsterMachine()->m_AttakPos.X*10;
					pos.z = pMainplayer->getMonsterMachine()->m_AttakPos.Y*10;
					pMainplayer->getCurMap()->getTerrainH(pos.x,pos.z,pos.y);
					m_pSelectEff->setPosition( pos );
				}else*/
{
					m_pSelectEff->setRotation(m_nLastAngle + 90.0f, 0, 0);
				}
			}
		}
		m_pSelectEff->update( dtick );
	}
	if (m_pDirDecal != NULL)
	{
		m_pDirDecal->clearPoints();
		m_pDirDecal->setWidth(m_nWidth ? m_nWidth : 320.0f);
		m_pDirDecal->setWidthGridNum(4);
		GameSpell *pSpell = SkillManager::getSingleton().getIntentSpell();
		if (pSpell)
		{
			pSpell->setTargetPoint(m_pSelectPos);
			GameXYZ selfPos = pMainplayer->GetPosition();
			GameXYZ targetPos = *(pSpell->getTargetPoint());
			float length = (targetPos - selfPos).toVector3().length();
			Vector3 dir;
			if (length == 0)
			{
				dir = Vector3(0, 0, 1);
			}
			else
			{
				GameXYZ tmp = targetPos - selfPos;
				tmp.y = 0;
				dir = tmp.normalize();
			}
			length += m_nWidth / 2.0f;
			targetPos += dir * int(m_nWidth / 2);
			if (length < m_nWidth)
			{
				selfPos -= dir * (m_nWidth - length);
				length = m_nWidth;
			}
			Vector3 diff = dir * length;
			m_pDirDecal->setTextureRadio(length / m_nLimit);
			for (int i = 0; i < 21; i++)
			{
				m_pDirDecal->addPoint(selfPos.toVector3() + (i / 20.0f) * diff);
			}
			m_pDirDecal->setPosition(selfPos);
		}
		m_pDirDecal->setRebuild(true);
		m_pDirDecal->update(0);
	}
	if (m_pRectDecal != NULL)
	{
		m_pRectDecal->clearPoints();
		m_pRectDecal->setWidth(m_nWidth ? m_nWidth : 320.0f);
		m_pRectDecal->setWidthGridNum(4);
		GameXYZ selfPos = pMainplayer->GetPosition();
		Vector3 dir = (m_pSelectPos - selfPos).normalize();
		for (int i = 0; i < 21; i++)
		{
			m_pRectDecal->addPoint(selfPos.toVector3() + (i / 20.0f) * m_nLimit * dir);
		}
		m_pRectDecal->setPosition(selfPos);
		m_pRectDecal->setRebuild(true);
		m_pRectDecal->update(0);
		if (m_pSelectPos.subtract(selfPos).length() > MINSELECT)
		{
			m_nLastAngle = FaceAngleBetween(m_pSelectPos.x, m_pSelectPos.z, selfPos.x, selfPos.z);
		}
	}

	if( pMainplayer != NULL )
	{
		if ( m_bLastCmd )
		{
			GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
			if (pSpell && !SkillManager::getSingleton().canMoveAttack(pSpell->getSpellId()))
			{
				m_bLastCmd = false;
			}
			else if (GamePlayManager::getSingleton().getCurMap() == NULL)
			{
				m_bLastCmd = false;
			}
			else if ( !pMainplayer->canDelayMove() )
			{
				m_bLastCmd = false;
				SkillManager::getSingleton().onStop();
				if ( needContinueMove( pMainplayer->CmdMoveTo(m_pLastCmdX, m_pLastCmdY) ) )
				{
					m_bLastCmd = true;
				}
			}
		}
	}
	
	checkOnLButtonDown();
}

void GameSelectManager::onRightButton(bool press)
{
	if (press)
	{
		GameManager::getSingleton().getScriptVM()->callString( "GameClientRightButtonDown()" );
	}
	else
	{
		if( UIIsInDragState() )
		{
			UIEndDrag();
			GameUIMsg::getSingleton().getUIGV()->DragFrom = 0;
		}
	}
}

void GameSelectManager::onLeftButton(bool press)
{
	if (press)
	{
		GameManager::getSingleton().getScriptVM()->callString( "GameClientLeftButtonDown()" );
	}
}

void GameSelectManager::showAngleEffect( const char *szAllowPath, int nCumulateTime, unsigned int nDistance )
{
	m_selectMode = ANGLE;		/// 设置为角度选择
	m_szAllowEff = szAllowPath;
	m_nLastCumulateTime = nCumulateTime;
	m_nDistance			= nDistance;

	MainGamePlayer *pMainplayer = ActorManager::getSingleton().getMainPlayer();
	GameSpell *pSpell = SkillManager::getSingleton().getAngleSpell();
	if ( pSpell )
	{
		int length = int( m_nLastCumulateTime / 1000.0 * m_nDistance * 10 + 0.5);
		int angle = pMainplayer->getAngle();
		GameXYZ pos = pMainplayer->GetPosition();
		pos.x += length * Cos(angle);
		pos.z -= length * Sin(angle);
		pSpell->setTargetPoint(pos);
	}
}

void GameSelectManager::showSelectEffect( const char* szAllowPath, const char* szUnAllowPath, int dis, int radio, bool spin )
{
	m_selectMode = POINT;		/// 设置为地点选择
	m_szAllowEff = szAllowPath;
	m_szUnAllowEff = szUnAllowPath;
	if (m_szUnAllowEff == "")
	{
		m_szUnAllowEff = m_szAllowEff;
	}
	m_nAllowDis = dis;
	m_nRadio = radio;
	m_bSpin = spin;
	GameMap* pCurMap = GamePlayManager::getSingleton().getCurMap();		

	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}

	MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
	if( pActor != NULL )
	{
		GameXYZ pos = pActor->GetPosition();
		pos.y = m_pSelectPos.y;
		m_bAllowNow = pos.subtract(m_pSelectPos).length() - m_nAllowDis <= 0.001;

		m_pSelectEff = new Ogre::Entity;
		m_pSelectEff->load( m_bAllowNow ? m_szAllowEff.c_str() : m_szUnAllowEff.c_str(), false );
		m_pSelectEff->update(0);
		BoxBound &boxBound = m_pSelectEff->getWorldBounds().getBox();
		Vector3 scale((float)m_nRadio);
		scale = scale / (boxBound.getMaxPos() - boxBound.getMinPos());
		scale.y = 1;
		m_pSelectEff->setScale(scale);
		m_pSelectEff->attachToScene( pCurMap->getShowGameMap()->getGameScene() );
		m_pSelectEff->setPosition( m_pSelectPos );
		if (m_bSpin)
		{
			int angle = FaceAngleBetween(pos.x, pos.z, m_pSelectPos.x, m_pSelectPos.z);
			m_pSelectEff->setRotation(angle - 90.0f, 0, 0);
		}
	}
	//隐藏鼠标指针
	while(ShowCursor(false) >= 0);
}

void GameSelectManager::hideSelectEffect()
{
	m_selectMode = NONE;
	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}
	if (m_pDirDecal != NULL)
	{
		m_pDirDecal->detachFromScene();
		OGRE_RELEASE(m_pDirDecal);
	}
	if (m_pRectDecal != NULL)
	{
		m_pRectDecal->detachFromScene();
		OGRE_RELEASE(m_pRectDecal);
	}
	//显示鼠标指针
	while( ShowCursor(true) < 0);
}

// 下次选中忽略
void GameSelectManager::cancelNextSelect()
{
	m_bCancelSelect = true;
}

// 打断自动点击走路
void GameSelectManager::breakAutoClick()
{
	m_AutoLDownInfo.first	= false;
	m_AutoLDownInfo.second	= 0;
}


void GameSelectManager::onLeaveMap()
{
	if( m_pClickEntity != NULL )
	{
		m_pClickEntity->detachFromScene();
		OGRE_RELEASE(m_pClickEntity);
	}

	if ( m_pSelEntity != NULL )
	{
		m_pSelEntity->detachFromScene();
		GameActor* pSelObj = ActorManager::getSingleton().FindActor( m_PreSelId );
		if( pSelObj != NULL )
		{
			pSelObj->getShowActor()->getCurEntity()->unbindObject(m_pSelEntity);
		}
		OGRE_RELEASE( m_pSelEntity );
	}
	m_PreSelId = 0;

	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}

	if (m_pDirDecal != NULL)
	{
		m_pDirDecal->detachFromScene();
		OGRE_RELEASE(m_pDirDecal);
	}

	if (m_pRectDecal != NULL)
	{
		m_pRectDecal->detachFromScene();
		OGRE_RELEASE(m_pRectDecal);
	}

	SkillManager::getSingleton().clearIntentSpell();
}

bool GameSelectManager::onCtrlLButtonDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z )
{
	if( (GetKeyState(VK_RCONTROL) & 0xff00) || (GetKeyState(VK_LCONTROL) & 0xff00) )
	{
		int nSkillId = GameShortCut::getSingleton().getSPShortCut( SELECT_CTRL_L );

		int tmpSmallSkillId = SkillManager::getSingleton().getTmpSkillId( 0 );
		int tmpBigSkillId	= SkillManager::getSingleton().getTmpSkillId( 1 );
		int tmpArm			= pMainPlayer->m_Equip.getTmpArm();
		int tmpMachine		= pMainPlayer->m_Equip.getTmpMachin();
		
		if( tmpMachine != 0 )
		{
			// 临时武器如能使用，左键技能为临时武器技
			tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( tmpMachine );
			if( itemDef.ItemTmp.Type == ITEM_TMP_MACHINE )
			{
				tagSkillDyAttrib* pSkillDyAttrib = SkillManager::getSingleton().getSkillDyAttrib( tmpBigSkillId );
				if( pSkillDyAttrib != NULL )
				{
					nSkillId = tmpBigSkillId;
				}
			}
		}

		if (nSkillId)
		{
			if (pTarget)
			{
				SkillManager::getSingleton().useSpellOnce(nSkillId, pTarget->GetID(), x * 10, z * 10);
			}
			else
			{
				SkillManager::getSingleton().useSpellOnce(nSkillId, x * 10, z * 10);
			}
		}
		return true;
	}
	return false;
}

bool GameSelectManager::onCtrlRButtonDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z )
{
	if( (GetKeyState(VK_RCONTROL) & 0xff00) || (GetKeyState(VK_LCONTROL) & 0xff00) )
	{
		static const int nSkillList[] = {
			0,
			7000,
			7001,
			7002,
			7003
		};
		int nRace		= pMainPlayer->getRace();
		int nSkillId	= nSkillList[nRace & 0x0f];
		tagSkillDyAttrib* pSkillDyAttrib = SkillManager::getSingleton().getSkillDyAttrib( nSkillId );
		if( pSkillDyAttrib != NULL )
		{
			GameXYZ selfPos;
			if (m_selectMode == ANGLE)
			{
				pMainPlayer->SetFaceTo(m_nLastAngle + 180);
				selfPos = pMainPlayer->GetPosition();
				selfPos.x += (int)(50 * Cos((float)pMainPlayer->getAngle()));
				selfPos.z -= (int)(50 * Sin((float)pMainPlayer->getAngle()));
			}
			else
			{
				selfPos = m_pSelectPos;
			}
			if (nSkillId)
			{
				if (pTarget)
				{
					SkillManager::getSingleton().useSpellOnce(nSkillId, pTarget->GetID(), x * 10, z * 10);
				}
				else
				{
					SkillManager::getSingleton().useSpellOnce(nSkillId, x * 10, z * 10);
				}
			}
		}
		return true;
	}
	
	return false;
}	

bool GameSelectManager::onAltLButtonDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z )
{
	if( (GetKeyState(VK_LMENU) & 0xff00) || (GetKeyState(VK_RMENU) & 0xff00) )
	{
		int nSkillId = GameShortCut::getSingleton().getSPShortCut( SELECT_ALT_L );
		if (nSkillId)
		{
			if (pTarget)
			{
				SkillManager::getSingleton().useSpellOnce(nSkillId, pTarget->GetID(), x * 10, z * 10);
			}
			else
			{
				SkillManager::getSingleton().useSpellOnce(nSkillId, x * 10, z * 10);
			}
		}
		return true;
	}
	return false;
}

bool GameSelectManager::onAltRButtionDown( MainGamePlayer* pMainPlayer, GameActor* pTarget, int x, int z )
{
	if( (GetKeyState(VK_LMENU) & 0xff00) || (GetKeyState(VK_RMENU) & 0xff00) )
	{
		int nSkillId = GameShortCut::getSingleton().getSPShortCut( SELECT_ALT_R );
		if (nSkillId)
		{
			if (pTarget)
			{
				SkillManager::getSingleton().useSpellOnce(nSkillId, pTarget->GetID(), x * 10, z * 10);
			}
			else
			{
				SkillManager::getSingleton().useSpellOnce(nSkillId, x * 10, z * 10);
			}
		}
		
		return true;
	}
	return false;
}

bool GameSelectManager::onShiftLButtonDown( MainGamePlayer* pMainPlayer, int x, int y )
{
	if( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) )
	{
		if( pMainPlayer->getCurMap() != NULL )
		{
			ChatManager::getSingleton().setPositionLink( pMainPlayer->getCurMap()->getRealId(), x, y );
			return true;
		}
	}
	return false;
}

// 是否可以选取为目标
bool GameSelectManager::canSelectTarget( MainGamePlayer* pMainPlayer, GameActor* pTarget )
{
	// 锁定定状态下不能选中目标
	// 	if( pMainPlayer->m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE )
	// 	{
	// 		return false;
	// 	}

	if( pTarget->getSubType() == MON_SUB_TYPE_COLLECT 
		&& (!SecondarySkillMgr::getSingleton().canCollect(pTarget->getResId())
		|| !GameQuest::getSingleton().canCollectQuest( pTarget->getResId())) )
	{
		return false;
	}

	if (pTarget->getSubType() == MON_SUB_TYPE_TRAP 
		|| pTarget->getSubType() == MON_SUB_TYPE_BUILD )
	{
		/// 不允许选中图腾和地雷
		return false;
	}

	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pTarget->getResId());
	if (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_ATKEE)
	{
		// 不允许选中不可攻击的类型
		return false;
	}

	return true;
}

bool GameSelectManager::tryCommunicationToTarget( GameActor* pTarget )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return false;
	}

	// 摆摊对话
	if( pTarget->getType() == GAT_PLAYER && pTarget->getActState().GetState() == GSACT_STALL )
	{
		if ( GameStallManager::getSingleton().canSeeStall() )
		{
			GameStallManager::getSingleton().stall_See( pTarget->GetID() );
		}

		return true;
	}

	// 同NPC对话
	const tagMonsterDef* monsterDef = NULL;
	if( pTarget->getType() == GAT_MONSTER ) {
		monsterDef = &DBDataManager::getSingleton().getMonsterDef( pTarget->getResId() );
	}
	if( pTarget->getType() == GAT_NPC || ( monsterDef && ( monsterDef->MiscType & MON_MISC_TYPE_JIAJU ) ) )
	{
		GameNpcDialog::getSingleton().talkByMemId( pTarget->GetID() );
		return true;
	}

	// 采集物品
	if( GameCollect::getSingleton().canCollect( pTarget ) && GameCollect::getSingleton().limitMachineCollectSimple(pTarget->getResId()) )
	{
		GameCollect::getSingleton().collect( pTarget->GetID() );
		return true;
	}

	// 拾取物品
	if( pTarget->getType() == GAT_ITEM )
	{
		const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( pTarget->getResId() );
		if( (itemDef.ItemTmp.Type == ITEM_TMP_NO || pTarget->m_IsPickItem) && pTarget->getRoleWId() == pMainPlayer->getRoleWId() )
		{
			ItemPicker::getSingleton().pickItem(pTarget->GetID());
			return true;
		}
	}

	return false;
}

// 目标选择
bool GameSelectManager::selectTarget( MainGamePlayer* pMainPlayer, GameActor* pTarget )
{
	if ( !canSelectTarget( pMainPlayer, pTarget ) )
	{
		return false;
	}

	if( pMainPlayer != NULL )
	{
		if( pTarget->getType() != GAT_ITEM )
		{
			pMainPlayer->SetSelectTarget( pTarget->GetID() );
		}

		return tryCommunicationToTarget( pTarget );
	}
	return false;
}

void GameSelectManager::showSelCursor( int x, int z )
{
	GameMap* pCurMap = GamePlayManager::getSingleton().getCurMap();		
	
	if( m_pClickEntity != NULL )
	{
		m_pClickEntity->detachFromScene();
		OGRE_RELEASE(m_pClickEntity);
	}

	if( m_pClickEntity == NULL )
	{
		m_pClickEntity = new Ogre::Entity;
		m_pClickEntity->load( "effect\\entity\\7000_12.ent", false );
	}
	int y;
	bool b = pCurMap->getTerrainH(x*10, z*10, y);
	assert(b);
	Ogre::WorldPos pos( Ogre::Vector3( (float)(x*10), (float)y, (float)(z*10) ) );
	pos = pos/10;
	pos += Ogre::Vector3( 0, 20, 0 );
	m_pClickEntity->setPosition( pos );
	m_pClickEntity->update(0);
	m_pClickEntity->attachToScene( pCurMap->getShowGameMap()->getGameScene() );
}

void GameSelectManager::showSelEffect( bool bdel )
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		GameActor* pSelObj = ActorManager::getSingleton().FindActor( m_PreSelId );
		if( bdel && pSelObj != NULL && m_pSelEntity != NULL )
		{
			pSelObj->getShowActor()->getCurEntity()->unbindObject(m_pSelEntity);
			OGRE_RELEASE(m_pSelEntity);
			return;
		}

		if( pSelObj != NULL && m_pSelEntity != NULL )
		{
			pSelObj->getShowActor()->getCurEntity()->unbindObject(m_pSelEntity);
			OGRE_RELEASE(m_pSelEntity);
		}	
		
		if( m_pSelEntity == NULL)
		{
			m_pSelEntity = new Ogre::Entity;
			m_pSelEntity->load( "effect\\entity\\9500_16.ent", false );
		}

		pSelObj = ActorManager::getSingleton().FindActor( pMainPlayer->GetSelectTarget() );
		m_PreSelId = pMainPlayer->GetSelectTarget();
		if( pSelObj != NULL )
		{
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pSelObj->getResId());
			m_pSelEntity->setScale(info.radius / (info.scale * 75.0f));
			pSelObj->getShowActor()->getCurEntity()->bindObject( 0, m_pSelEntity );
		}
	}
}

// 判断是否需要继续寻路
bool GameSelectManager::needContinueMove( int nReasonCode )
{
	static int s_continueMove[] = { MOVE_TIME_INTERVAL_SMALL, MOVE_SPEED_ZERO, MOVE_IN_TETANIC };
	for ( int i = 0; i < sizeof( s_continueMove ) / sizeof( int ); ++i )
	{
		if ( nReasonCode == s_continueMove[i] )
		{
			return true;
		}
	}

	return false;
}

void GameSelectManager::checkOnLButtonDown()
{
	// 若游戏失去焦点，则无需处理鼠标左键持续点击事件
	// 若鼠标当前正在拖曳某个控件，则不作处理
	if ( !InputManager::getSingleton().isFocus() || g_pFrameMgr->m_pDragFrame != NULL )
	{
		return;
	}

	// 若鼠标右键处于按下状态，则不作操作
	if ( ( GetKeyState( VK_RBUTTON ) & 0xff00) )
	{
		return;
	}
	
	// 检测鼠标是否保持点击场景
	Frame* pFrame = g_pFrameMgr->FindUIObjectOnPoint( GetCursorPosX(), GetCursorPosY(), false );
	
	if ( pFrame == NULL )
	{
		return;
	}

	if ( stricmp( pFrame->GetName(), UI_ROOT_NAME.c_str() ) != 0  )
	{
		return;
	}

	/// 发现正在吟唱、引导、集气，则立即停止自动点击
	GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
	if (pSpell && !SkillManager::getSingleton().canMoveAttack(pSpell->getSpellId()))
	{
		if (m_AutoLDownInfo.first)
		{
			breakAutoClick();
		}
		return;
	}

	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
	if (mutilMapFinder.IsAutoMoving())
	{
		breakAutoClick();
		return;
	}

	unsigned int nCurTime = timeGetTime();

	// 若鼠标没点击在场景中，则不计时

	// 若刚开始对长按住鼠标左键进行计时,先初始化长按的计时器
	if ( ( GetKeyState( VK_LBUTTON ) & 0xff00 ) != 0 && m_AutoLDownInfo.second == 0 )
	{
		m_AutoLDownInfo.second = nCurTime;
		return;
	}

	// 若长按的时间没超过2秒，则不进行自动点击
	if ( ( GetKeyState( VK_LBUTTON ) & 0xff00 ) != 0 && ( nCurTime - m_AutoLDownInfo.second < MAX_KEEP_LEFT_DOWN_TIME ) )
	{
		return;
	}

	// 若第一次对长按操作进行初始化
	if ( ( GetKeyState( VK_LBUTTON ) & 0xff00 ) != 0 && !m_AutoLDownInfo.first )
	{
		m_AutoLDownInfo.first	= true;
		m_AutoLDownInfo.second	= nCurTime;
		return;
	}

	// 若在没有按住鼠标左键的情况下，判断是否进行自动点击操作
	// 若长按时间没超过2秒就停止
	if ( ( GetKeyState( VK_LBUTTON ) & 0xff00 ) == 0 && !m_AutoLDownInfo.first )
	{
		m_AutoLDownInfo.second	= 0;
		return;
	}

	const int MIN_TIEM_IN_TWICE = 500;
	// 若鼠标之前长按得时间超过2秒，但是两次调用的间隔小于500ms
	if ( nCurTime - m_AutoLDownInfo.second < MIN_TIEM_IN_TWICE )
	{
		return;
	}
	m_AutoLDownInfo.second = nCurTime;

	// 检测鼠标是否保持点击场景

	Ogre::InputEvent event;
	event.mouse.x = GetCursorPosX();
	event.mouse.y = GetCursorPosY();

	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	FollowCamera*	pMainCamera = GamePlayManager::getSingleton().getMainCamera();
	GameMap*		pCurMap		= GamePlayManager::getSingleton().getCurMap();	
	Ogre::WorldRay	ray;
	pMainCamera->GetCameraNode()->getViewRayByScreenPt(&ray, event.mouse.x, event.mouse.y);
	float dist;
	if( !pCurMap->getShowGameMap()->pickGround( ray, &dist ) )
	{
		return;
	}

	Ogre::WorldPos pickpoint = ray.m_Origin + ray.m_Dir*dist;
	int x = (int)Ogre::WorldPos::Fix2Flt(pickpoint.x);
	int z = (int)Ogre::WorldPos::Fix2Flt(pickpoint.z);

	SkillManager::getSingleton().onStop();
	pMainPlayer->CmdMoveTo( x, z );
	showSelCursor( x, z );
}

void GameSelectManager::OnExchangeLButtonDown( GameActor* pTarget )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pTarget == NULL || pMainPlayer == NULL )
	{
		return;
	}

	if ( !canSelectTarget( pMainPlayer, pTarget ) )
	{
		return;
	}

	if( pTarget->getType() != GAT_ITEM )
	{
		pMainPlayer->SetSelectTarget( pTarget->GetID() );
	}
	
	GameXYZ pos  = pMainPlayer->GetPosition();
	pos.y		 = 0;

	GameXYZ pos2 = pTarget->GetPosition();
	pos2.y		 = 0;

	int nDis = (int)(pos2.subtract( pos ).length() / 100);

	if ( pTarget->getType() != GAT_PLAYER )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_CANNOT_NOT_CAN_TRADE_OBJECT );
		return;
	}

	if ( nDis > CLIENT_MAX_TRADE_DIST )
	{
		ChangeCursorState( "disableExchange" );
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_CANNOT_DIST_TOO_LONG );
		return;
	}

	GameExchange::getSingleton().inviteExchange( pTarget->getName() );
	ChangeCursorState( "normal" );
}

void GameSelectManager::showAdjustEffect( const char* szAllowPath, const char* szUnAllowPath, void *pExtendData )
{
	m_selectMode = ANGLE;		/// 设置为角度选择
	m_szAllowEff = szAllowPath;
	m_szUnAllowEff = szUnAllowPath;
	if (m_szUnAllowEff == "")
	{
		m_szUnAllowEff = m_szAllowEff;
	}

	GameMap* pCurMap = GamePlayManager::getSingleton().getCurMap();		

	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}

	m_pSelectEff = new Ogre::Entity;
	m_pSelectEff->load( szAllowPath, false );
	m_pSelectEff->update(0);
	m_pSelectEff->attachToScene( pCurMap->getShowGameMap()->getGameScene() );
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	if( pActor->isOnMonsterMachine() )
	{
		unsigned short radio = (unsigned short)pExtendData;
		BoxBound &boxBound = m_pSelectEff->getWorldBounds().getBox();
		Vector3 scale((float)radio);
		scale = scale / (boxBound.getMaxPos() - boxBound.getMinPos());
		scale.y = 1;
		m_pSelectEff->setScale(scale);
	}
	if( pActor != NULL )
	{
		GameXYZ pos = pActor->GetPosition();
		m_pSelectEff->setPosition(pos);
		if (m_pSelectPos.subtract(pos).length() <= MINSELECT)
		{
			m_nLastAngle = pActor->getAngle() + 180;
		}
		else
		{
			m_nLastAngle = FaceAngleBetween(m_pSelectPos.x, m_pSelectPos.z, pos.x, pos.z);
		}
		m_pSelectEff->setRotation(m_nLastAngle + 90.0f, 0, 0);
	}
}

void GameSelectManager::showCircularEffect( const char *szPath, unsigned int nLimit, void *pExtendData )
{
	m_selectMode = POINT;		/// 设置为地点选择
	m_nLimit = nLimit;
	m_nWidth = (unsigned int)pExtendData;

	if (m_pDirDecal == NULL)
	{
		m_pDirDecal = new DirDecal;
	}
	else
	{
		m_pDirDecal->detachFromScene();
	}
	m_pDirDecal->setTextureRes(szPath);
	m_pDirDecal->setWidth(m_nWidth ? m_nWidth : 320.0f);
	m_pDirDecal->setWidthGridNum(4);
	GameSpell *pSpell = SkillManager::getSingleton().getIntentSpell();
	if (pSpell)
	{
		GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
		pSpell->setTargetPoint(m_pSelectPos);
		GameXYZ selfPos = pActor->GetPosition();
		GameXYZ targetPos = *(pSpell->getTargetPoint());
		GameXYZ dir = (targetPos - selfPos).normalize();
		targetPos += dir * int(m_nWidth / 2);
		Vector3 diff = targetPos.toVector3() - selfPos.toVector3();
		diff.y = 0;
		m_pDirDecal->setTextureRadio(diff.length() / m_nLimit);
		for (int i = 0; i < 21; i++)
		{
			m_pDirDecal->addPoint(selfPos.toVector3() + (i / 20.0f) * diff);
		}
		m_pDirDecal->setPosition(selfPos);
	}
	m_pDirDecal->setRebuild(true);
	m_pDirDecal->update(0);
	GameMap* pCurMap = GamePlayManager::getSingleton().getCurMap();
	m_pDirDecal->attachToScene(pCurMap->getShowGameMap()->getGameScene(),false);
}

void GameSelectManager::showRectEffect(const char *szPath, unsigned int width, void *pExtendData)
{
	m_selectMode = ANGLE;		/// 设置为角度选择
	RectEffectData *data = (RectEffectData *)pExtendData;
	m_nLimit = data->limit;
	m_nWidth = width;

	if (m_pRectDecal == NULL)
	{
		m_pRectDecal = new DirDecal;
	}
	else
	{
		m_pRectDecal->detachFromScene();
	}
	m_pRectDecal->setTextureRes(szPath);
	m_pRectDecal->setWidth(m_nWidth ? m_nWidth : 320.0f);
	m_pRectDecal->setWidthGridNum(4);
	GameActor *pMainplayer = ActorManager::getSingleton().getMainPlayer();
	GameXYZ selfPos = pMainplayer->GetPosition();
	Vector3 dir = (m_pSelectPos - selfPos).normalize();
	for (int i = 0; i < 21; i++)
	{
		m_pRectDecal->addPoint(selfPos.toVector3() + (i / 20.0f) * m_nLimit * dir);
	}
	m_pRectDecal->setPosition(selfPos);
	m_pRectDecal->setRebuild(true);
	m_pRectDecal->update(0);
	GameMap* pCurMap = GamePlayManager::getSingleton().getCurMap();
	m_pRectDecal->attachToScene(pCurMap->getShowGameMap()->getGameScene(),false);
}

int GameSelectManager::getLastHighlightItemID()
{
	return m_nLastHighlightItemID;
}

void GameSelectManager::HighlightNewItem( int nMemID )
{
	m_nLastHighlightItemID = nMemID;
}

void GameSelectManager::insertMouseEventHandler(IMouseEventAction *h)
{
	m_lstMouseEventAction.insert(h);
}

bool GameSelectManager::isMouseSwap()
{
	return ActorManager::getSingleton().getGameConfig().getGameOpt(GAME_OPT_SWAP_MOUSE);
}

int GameSelectManager::getLastHighStallActorID()
{
	return m_nLastStallActorID;
}

void GameSelectManager::HighlightStallActor( int nActorID )
{
	m_nLastStallActorID = nActorID;
}