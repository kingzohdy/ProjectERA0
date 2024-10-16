#include "GameActionManager.h"
#include "GameManager.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameEffect_Mgr.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Base.h"
#include "GameTeamManager.h"
#include "GameSkill_Package.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "GameMap.h"
#include "OgreScriptLuaVM.h"
#include "GameWizard.h"
#include "GameUIMsg.h"
#include "GameQuest.h"
#include "GameActor_Rider.h"
#include "GameStallManager.h"
#include "GameActor_RoleText.h"
#include "GameClanManager.h"
#include "GameSCTDamageManager.h"
#include "GameFriendManager.h"
#include <math.h>
#include <algorithm>
#include "OgreLog.h"
#include "GameActor_PVP.h"
#include "GameMaxMap.h"
#include "GameActor_Tick.h"
#include "EffectXPPlay.h"
#include "GameDamageHealCounter.h"
#include "OgreMath.h"
#include "GameQuest.h"
#include "GamePlayManager.h"
#include "GameCamera.h"
#include "GameBCT.h"
#include "AutoTeam.h"
#include "AvatarEntity.h"
#include "GameMailManager.h"
#include "../UILib/ui_framemgr.h"
#include "OgreBorderScene.h"
#include "GameShortCut.h"
#include "BulletManager.h"
#include "GameNpcDialog.h"
#include "GamePromotion.h"
#include "GameRuneManager.h"
#include "WorldCity.h"
#include "GameChatManager.h"
#include "GameBusyManager.h"
#include "GameMarketManager.h"
#include "GameRankManager.h"
#include "GameBattle.h"
#include "GameSchool.h"
#include "GameNpc.h"


// 此为宠物操作界面集合
static int s_petUIs[] =	{	UI_TYPE_PET_WASH_GENIUS,	UI_TYPE_PET_STRENGTHEN,		UI_TYPE_PET_ADD_ATTR, 
UI_TYPE_PET_SKILL_LEARN,	UI_TYPE_PET_SKILL_UP,		UI_TYPE_PET_ADD_SKILL_GRID,	UI_TYPE_PET_SKILL_RESET,
UI_TYPE_PET_WASH_SKILL_GRID 
};
GameActionManager::GameActionManager(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( ACTION_RES, this );
	INetworkManager::getSingleton().RegisterMsgHandler( SKILL_REVIVE_INFO, this );
	INetworkManager::getSingleton().RegisterMsgHandler( RIDE_LIST_CHG, this );
	INetworkManager::getSingleton().RegisterMsgHandler( INVITE_TEST_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( DIE_INFO_SVR,this );
	INetworkManager::getSingleton().RegisterMsgHandler( CHGNAME_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( MACHINE_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( PET_CAMPAIGN_BRK_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( ROLE_CHG_MEMID, this );
}

GameActionManager::~GameActionManager(void)
{
}

int	GameActionManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( PET_CAMPAIGN_BRK_SVR == msgid )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( msgbody.PetPreBrk );
		if( pActor )
		{
			pActor->onBreakChant( ePet_Chant );
		}
	}
	else if( CHGNAME_SVR == msgid )
	{
		const CSCHGNAMESVR& changeNameSvrData = msgbody.ChgNameSvr;

		if ( changeNameSvrData.Flag == CHG_ROLE_NAME )
		{
			GameFriendManager::getSingleton().changeFriendName( changeNameSvrData.OldName, changeNameSvrData.NewName );
			GameClanManager::getSingleton().changeClanMemberName( changeNameSvrData.OldName, changeNameSvrData.NewName );
			GameMailManager::getSingleton().changeMailSendUserName( changeNameSvrData.OldName, changeNameSvrData.NewName );
			CMarketManager::getSingleton().ChangeRecordName( changeNameSvrData.OldName, changeNameSvrData.NewName );
			GameRankManager::getSingleton().changeRankPlayerName( changeNameSvrData.OldName, changeNameSvrData.NewName );
			GameSchool::getSingleton().ChangeSchoolName( changeNameSvrData.OldName, changeNameSvrData.NewName );
			GameActor* pActor = ActorManager::getSingleton().findActorByName( changeNameSvrData.OldName );
			if ( pActor == NULL )
			{
				return 0;
			}
			
			tagUIMsgBody UI_Msg;
			int nActorID = pActor->GetID();
			strncpy( UI_Msg.ChangeNameData.szOldName, pActor->getName(), NAME_LEN );
			strncpy( UI_Msg.ChangeNameData.szNewName, changeNameSvrData.NewName, NAME_LEN );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			pActor->setName( changeNameSvrData.NewName );
			MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			if ( pMainPlayer->GetID() == nActorID )
			{
				GameManager::getSingleton().SendUIEvent( "GE_MAINPLAYER_CHG_NAME" );
			}
			else
			{
				GameManager::getSingleton().SendUIEvent( "GE_OTHERLAYER_CHG_NAME" );
			}
			
			if ( pMainPlayer->GetSelectTarget() == nActorID )
			{
				GameManager::getSingleton().SendUIEvent( "GE_TARGET_CHG_NAME" );
			}
			
			RoleText& actorTitleMgr = pActor->getRoleText();
			actorTitleMgr.BuildTitle();
			
			/*if ( pMainPlayer->GetID() != nActorID && pMainPlayer->GetSelectTarget() != nActorID )
			{
				GameManager::getSingleton().SendUIEvent( "GE_ONE_ACTOR_CHG_NAME" );
			}*/
		}
		else if ( changeNameSvrData.Flag == CHG_CLAN_NAME )
		{
			ActorManager::getSingleton().UpdateAllActorClanName( changeNameSvrData.WID, changeNameSvrData.NewName );
			GameClanManager::getSingleton().changeClanName( changeNameSvrData.WID,changeNameSvrData.NewName );
			GameRankManager::getSingleton().changeRankClanName(changeNameSvrData.OldName,changeNameSvrData.NewName);
		}
	}
	else if( ACTION_RES == msgid )
	{
		const CSACTIONRES& Msg = msgbody.ActionRes;
		switch( Msg.ActionID )
		{
		case ACTION_CATCH_PET:
			{
				OnActionCatchPet( Msg );
			}
			break;

		case ACTION_USE_ITEM_PRE:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor )
				{
					if( Msg.Data.UseItemPre.ItemID == 0 )
					{
						pActor->onBreakChant( eUseItem_Chant );
					}else{
						tagItemDef_ToLua* pItemDef = getItemDef( Msg.Data.UseItemPre.ItemID );
						if( pItemDef->ItemID == Msg.Data.UseItemPre.ItemID )
						{
							if( pActor->getType() != GAT_MAINPLAYER )
							{
								pActor->startUseItemChant(pItemDef->ItemUsePre*1000,
									Msg.Data.UseItemPre.ItemID,Msg.Data.UseItemPre.TargetID,
									Msg.Data.UseItemPre.ListType,Msg.Data.UseItemPre.GridIdx);
							}							
						}						
					}
				}
			}
			break;

		case ACTION_HIDE_TASK_INFO:
			{
				const HIDETASKINFO& hideTaskInfo = Msg.Data.HideTaskInfo;
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer != NULL && Msg.ID == pMainPlayer->GetID() )
				{
					GameQuest& questMgr = GameQuest::getSingleton();
					questMgr.addDynamicGetTask( hideTaskInfo.HideTaskID, hideTaskInfo.HideTaskNum );
					questMgr.updateNpcQuestTitle();
					GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
				}
			}
			break;

		case ACTION_TMPENERGE_MAX:
			{
				const CSACTIONTMPENERGEMAX& tmpMaxEnergy = Msg.Data.TmpEnergeMax;
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				pMainPlayer->setMaxEnerge( tmpMaxEnergy.TmpEnergeMax );
				GameManager::getSingleton().SendUIEvent( "GE_TMP_MAX_ENERGY_CHG" );
			}
			break;

		case ACTION_MON_LIFE:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if (pActor != NULL)
				{
					const CSACTIONMONLIFE& monsterData = Msg.Data.MonLife;
					if ( monsterData.LifeEndTime != -1 )
					{
						pActor->registerTickEvent( new GameActor_Tick_OnWink( pActor, timeGetTime(), PET_WINK_TIME,
							monsterData.LifeEndTime * 1000 ) );
					}
				}
			}
			break;

		case ACTION_OBJ_ENTER_MAP:
			{
				const CSACTIONOBJENTERMAP& enterMapObj = Msg.Data.ObjEnterMap;

				if ( enterMapObj.Type == OBJ_NPC )
				{
					GameMapShower* pMapShower = GameManager::getSingleton().getGameMapShower();
					pMapShower->updateMapNpcInfo( enterMapObj.DefID, enterMapObj.x, enterMapObj.y );
					GameQuest::getSingleton().updateNpcQuestTitle();
				}
			}
			break;

		case ACTION_OBJ_LEAVE_MAP:
			{
				const CSACTIONOBJLEAVEMAP& leaveMapObj = Msg.Data.ObjLeaveMap;
				if ( leaveMapObj.Type == OBJ_NPC )
				{
					GameMapShower* pMapShower = GameManager::getSingleton().getGameMapShower();
					pMapShower->deleteMapNpcInfo( leaveMapObj.DefID );
				}
			}
			break;

		case ACTION_DEAD:
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				GameActor* pObj = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pObj != NULL && !pObj->m_flagDead )
				{
					Ogre::BorderGameScene::getSingleton().onDetachObject(pObj->getShowActor()->getCurEntity());

					pObj->ChangeActState( GSACT_DEAD );
					if( pObj == pMainPlayer )
					{
						//GameManager::getSingleton().SendUIEvent( "GE_MESSAGE_DEAD" );
						GameManager::getSingleton().SendUIEvent( "GE_MAINPLAYER_DEAD" );
					}
					// set face to
				}
				if( pObj == pMainPlayer )
				{
					pMainPlayer->addMoveSeq(50);
				}
			}
			break;

		case ACTION_DESTROY:
			{
				GameActor* pObj = ActorManager::getSingleton().FindActor( Msg.ID );
				if (pObj != NULL )
				{
					IGameBattle::getInstance()->objLeaveView(pObj);
					IGameWizard::getInstance()->objLeaveView(pObj);
					IWorldCityManager::getInstance()->objLeaveView(pObj);
					Ogre::BorderGameScene::getSingleton().onDetachObject(pObj->getShowActor()->getCurEntity());


					if (pObj->getType() == GAT_NPC)
					{		
						GameMsgPrint::getSingleton().npcLeaveView( Msg.ID );
					}
					else if( pObj->getType() == GAT_ITEM )
					{
						ActorManager::getSingleton().getMainPlayer()->getContainer().closeBooty( pObj->GetID() );
						GameMsgPrint::getSingleton().SendUIEvent( "GE_DESTROY_ITEM" );
					}
					else if( pObj->getType() == GAT_MONSTER )
					{
						const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef(pObj->getResId());
						if (monsterDef.DynMask)
						{
							GameMaskMap *maskMap = GamePlayManager::getSingleton().getCurMap()->getMaskMap();
							GameXYZ pos = pObj->GetPosition();
							maskMap->setDynMsk(pos.x / 10, pos.z / 10,
								monsterDef.DynMaskX == 0 ? monsterDef.Radius : monsterDef.DynMaskX,
								monsterDef.DynMaskY == 0 ? monsterDef.Radius : monsterDef.DynMaskY,
								pObj->getAngle(), -int(monsterDef.DynMask));
						}
					}

					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					if ( pMainPlayer && Msg.ID == pMainPlayer->getOwnMonster() )
					{
						pMainPlayer->delOwnMonster();
					}
					pObj->OnLeaveMap();
					ActorManager::getSingleton().DestroyActor(pObj);
				}
				else
				{
					Bullet *bullet = BulletManager::getSingleton().FindBullet(Msg.ID);
					if (bullet != NULL)
					{
						bullet->Destroy();
						BulletManager::getSingleton().DestoryBullet(Msg.ID);
					}
				}
			}
			break;

		case ACTION_CLAN_EXTRA_UI:
			{
			}
			break;

		case ACTION_REVIVE:
			{
				GameActor* pObj = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pObj != NULL )
				{
					pObj->SetPosition( Msg.Data.Revive.x, Msg.Data.Revive.y );
					if (pObj == ActorManager::getSingleton().getMainPlayer())
					{
						FollowCamera *pcamera = GamePlayManager::getSingleton().getMainCamera();
						if( pcamera != NULL)
						{
							pcamera->updateCamera(pObj->GetPosition(), 0);
						}
					}
					pObj->OnRevive();
// 					pObj->ChangeActState(GSACT_STAND);
					pObj->reSetHp( Msg.Data.Revive.HP );
					GameMsgPrint::getSingleton().updateActorAttr( pObj );
					if (pObj == ActorManager::getSingleton().getMainPlayer())
					{
						GameManager::getSingleton().SendUIEvent("GE_ACTION_REVIVE");
					}
					for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
					{
						GameActor *pet = pObj->getPet()->getPetActor(i);
						if (pet && !pet->m_isHeti)
						{
							pet->OnEnterMap(pObj->getCurMap());
						}
					}
				}
			}	
			break;
		case ACTION_BALLISTIC_ATK:
			{
				const tagCSBallisticAtk& atk = Msg.Data.BallisticAtk;
				GameActor* pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pActor != NULL)
				{
					ISpellControler *sc = SkillManager::getSingleton().getSpellControler(Msg.ID);
					GameSpell *pSpell = sc->getSpell(atk.RuneTransferedID == 0 ? atk.SkillID : atk.RuneTransferedID,
						atk.Level, atk.SkillSeq);
					pSpell->setSelfPoint(pActor->GetPosition());
					pSpell->setTargetPoint(atk.AtkPos.X*10, atk.AtkPos.Y*10);
					pSpell->setTargets(atk.TargetNum, atk.TargetIDs);
					const tagSkillDef &SkillDef = pSpell->getSkillDef();
					if (atk.SelfPosChgFlag)
					{
						pSpell->setSelfPosChange(atk.SelfPosChg.X, atk.SelfPosChg.Y);
					}
					else
					{
						pSpell->setSelfPosChange(atk.AtkPos.X, atk.AtkPos.Y);
					}
					sc->onAttack(pSpell, false);
				}
				if (pActor == ActorManager::getSingleton().getMainPlayer())
				{
					ActorManager::getSingleton().getMainPlayer()->ListenSpellPackage(atk.RuneTransferedID, atk.SelfPosChgFlag, atk.SelfPosChg.X, atk.SelfPosChg.Y);
				}
			}
			break;
		case ACTION_HIT:
			{
				MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				bool passive = false;
				const CSHITS& hits = Msg.Data.Hit;
				for (size_t i = 0; i < hits.TotalHit; i++)
				{
					if (hits.Hits[i].DeadFlag)
					{
						GameActor *pActor = ActorManager::getSingleton().FindActor(hits.Hits[i].ID);
						if (pActor)
						{
							Ogre::BorderGameScene::getSingleton().onDetachObject(pActor->getShowActor()->getCurEntity());

							pActor->m_flagDead = true;
						}
					}
				}
				GameSkill_Package* pPackage = new GameSkill_Package;
				if (pMainPlayer && pMainPlayer->m_MonsterMachine.m_playerInfo.m_nMonsterMemID == Msg.ID)
				{
					pPackage->setTargetState(pMainPlayer->GetID(), hits);
				}
				else
				{
					pPackage->setTargetState(Msg.ID, hits);
				}

				int id = Msg.ID;
				if (hits.FromID != 0 && hits.FromID != id)
				{
					id = hits.FromID;
					passive = true;
				}
				if (HIT_FLAG_BE_REFLECTED & hits.Flags)
				{
					passive = true;
				}
				GameActor* pActor;
				if (hits.RealMemID != 0)
				{
					pActor = ActorManager::getSingleton().FindActor(hits.RealMemID);
					if (pActor == NULL)
					{
						pActor = ActorManager::getSingleton().FindActor(id);
					}
				}
				else
				{
					pActor = ActorManager::getSingleton().FindActor(id);
				}
				if (pActor != NULL)
				{
					GameActor *pCaster = ActorManager::getSingleton().FindActor(id);
					if (pCaster == NULL)
					{
						pCaster = pActor;
					}
					if ( hits.SelfDead )
					{
						pCaster->ChangeActState( GSACT_DEAD );
						pCaster->playAction( pCaster->getActState().getDeadActId(), NULL, 0, 0 );
					}
					ISpellControler *sc = SkillManager::getSingleton().getSpellControler(pActor->GetID());
					GameSpell *pSpell = sc->getSpell(Msg.Data.Hit.RuneTransferedID == 0 ? Msg.Data.Hit.SkillID : Msg.Data.Hit.RuneTransferedID,
						Msg.Data.Hit.Level, Msg.Data.Hit.SkillSeq);
					if (hits.FromPos.X != 0 || hits.FromPos.Y != 0)
					{
						pSpell->setSelfPoint(hits.FromPos.X * 10, hits.FromPos.Y * 10);
					}
					pSpell->setTargetPoint(hits.AtkPos.X*10, hits.AtkPos.Y*10);
					const tagSkillDef &SkillDef = pSpell->getSkillDef();
					if (SkillDef.BeatBack)
					{
						passive = true;
					}
					pSpell->setSkillMsgPackage( pPackage );
					if (SkillDef.AtkType == SKILL_ATK_TYPE_NORMAL)
					{
						if ( hits.SelfPosChgFlag )
						{
							pSpell->setSelfPosChange( hits.SelfPosChg.X, hits.SelfPosChg.Y );
						}
						else
						{
							pSpell->setSelfPosChange(hits.AtkPos.X, hits.AtkPos.Y);
						}
						sc->onAttack(pSpell, passive);
					}
					else
					{
						pPackage->doPackage();
						pSpell->onStop();
					}
					pCaster->SetFaceTo(pCaster->getAngle() - hits.DirFace, true);

					ActorManager::getSingleton().getMainPlayer()->ListenSpellPackage(Msg, pSpell);
				}
				else
				{
					pPackage->updateTargetValue();
					pPackage->doPackage();
					GameSpell* pSpell = SkillManager::getSingleton().getSpellSample(hits.SkillID);
					pSpell->getAttrib();
					if (hits.FromPos.X != 0 || hits.FromPos.Y != 0)
					{
						pSpell->setSelfPoint(hits.FromPos.X * 10, hits.FromPos.Y * 10);
					}
					pSpell->setTargetPoint(hits.AtkPos.X*10, hits.AtkPos.Y*10);
					for (list<SKillObserver *>::iterator i = pSpell->m_lstObserver.begin(); i != pSpell->m_lstObserver.end(); ++i)
					{
						(*i)->onHited(pSpell);
					}
					delete pPackage;
				}
			}
			break;
		case ACTION_MSPD_CHG:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->setSpeed( Msg.Data.MspdChg.MoveSpdCurr );
					if( Msg.Data.MspdChg.NextNum > 0 )
					{
						pActor->m_BufStatus.setNextSpeed( Msg.Data.MspdChg.MoveSpdNext );
					}

					if( Msg.Data.MspdChg.MoveSpdCurr== 0 )
					{
						pActor->onStop( pActor->GetPosition().x/10, pActor->GetPosition().z/10, GSACT_STOP );
					}
				}
			}
			break;
		case ACTION_STATUS:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->m_BufStatus.setBufStatus( Msg.Data.Status );
				}
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if ( pMainPlayer != NULL && Msg.ID == pMainPlayer->GetID() )
				{
					GameManager::getSingleton().SendUIEvent( "GE_SKILL_LIST_UPDATE" );
				}
			}
			break;
		case ACTION_FAKE_STATUS:
			{
				OnActionFakeStatus( Msg );
			}
			break;

		case ACTION_LOCK:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->lockFace(Msg.Data.Lock.LockType, Msg.Data.Lock.LockVal, Msg.Data.Lock.SkillID);
				}
			}
			break;
		case ACTION_AVATAR:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->m_Equip.setSelectFashionBag( Msg.Data.Avatar.SelectFashion,Msg.Data.Avatar.AddFashion );

					for( int i = 1; i < EQUIP_POS_END; i++ )
					{
						bool bFind = false;
						for( int j = 0; j < Msg.Data.Avatar.Num; j++ )
						{
							if( Msg.Data.Avatar.Arms[j].GridIdx == i )
							{
								bFind = true;
							}
						}
						if( !bFind ) 
						{
							pActor->m_Equip.m_ArmEquipId[i] = 0;
							pActor->m_Equip.changEquip( i, 0 );
						}
					}
					for( int i = 0; i < Msg.Data.Avatar.Num; i++ )
					{
						pActor->m_Equip.m_ArmEquipId[Msg.Data.Avatar.Arms[i].GridIdx] = Msg.Data.Avatar.Arms[i].ArmDefID;
						pActor->m_Equip.changEquip( Msg.Data.Avatar.Arms[i].GridIdx, Msg.Data.Avatar.Arms[i].ArmDefID,Msg.Data.Avatar.Arms[i].StarLv );
						//接着处理下背饰信息
						if( Msg.Data.Avatar.BackModelID > 0 && Msg.Data.Avatar.Arms[i].GridIdx == EQUIP_POS_FASHION_BACK  )
						{
							pActor->m_Equip.changEquip( Msg.Data.Avatar.Arms[i].GridIdx, Msg.Data.Avatar.BackModelID,Msg.Data.Avatar.Arms[i].StarLv );
						}
					}		
					//如果在海滩地图的时候，显示时装
					if( pActor->getCurMap() && pActor->getCurMap()->getMapId() == BEACH_MAP_ID )
					{
						if(pActor->getAvatar())
						{
							pActor->getAvatar()->setAvatarAttrib(POS_ID_FASHION_HAIR,1);
						}						
					}
					if (pActor->getAvatar())
					{
						pActor->getAvatar()->updateAttrib();
					}
					pActor->m_nPerfectStar = Msg.Data.Avatar.ArmStar;
					if ( pActor->getActState().GetState() == GSACT_MOVE )
					{
						int actionId = pActor->getShowActor()->getCurAction();

						const char* szFootString[4];
						std::string strFoot("footprint");
						szFootString[0] = strFoot.c_str();
						pActor->getShowActor()->playAction( actionId, szFootString, 1, 0 );
					}
					pActor->setPerfectEffect();
					// 设置队伍契约
					pActor->setQiYue( Msg.Data.Avatar.QiYue );
					
					MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
					if ( pActor->GetID() == pMainplayer->GetID() )
					{
						setAvatarSelectSuit( Msg.Data.Avatar.SelectFashion );
						GameManager::getSingleton().SendUIEvent( "GE_MAIN_EQUIP_CHANGE" );
					}
					else
					{
						GameManager::getSingleton().SendUIEvent( "GE_ACTOR_CHANGE" );
					}					
				}
			}
			break;
		case ACTION_AMM:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->m_Equip.changAmm( AMM_POS_LEFT,		Msg.Data.Amm.AmmLeft  );
					pActor->m_Equip.changAmm( AMM_POS_RIGHT,	Msg.Data.Amm.AmmRight );
					pActor->m_Equip.changAmm( AMM_POS_SHOULDER, Msg.Data.Amm.AmmShoulder );
				}
			}
			break;
		case ACTION_UI:
			{
				// 转职面板
				if( Msg.Data.UI.UIType == UI_TYPE_CAREER_CHG )
				{
					GameManager::getSingleton().SendUIEvent( "GE_OPEN_CAREER_CHG_FRAME" );
				}
				
				// 改名字面板
				if( Msg.Data.UI.UIType == UI_TYPE_CHG_ROLE_NAME )
				{
					char buf[256];
					sprintf( buf,"CallScript_ShowChangeNameFrame(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				if( Msg.Data.UI.UIType == UI_TYPE_CHG_CLAN_NAME )
				{
					char buf[256];
					sprintf( buf,"CallScript_ShowChangeClanNameFrame(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}


				// 仓库面板
				if( Msg.Data.UI.UIType == UI_TYPE_STASH )
				{
					char szScript[256];
					sprintf( szScript, "OpenStashFrame(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}

				// 技能学习面板
				if( Msg.Data.UI.UIType == UI_TYPE_SKILL )
				{
					char buf[256];
					sprintf( buf, "openSkillLearn(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}

				// 宠物指令面板
				if( Msg.Data.UI.UIType == UI_TYPE_PET )
				{
					char buf[256];
					sprintf( buf, "openPetSetting(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}

				// 配件面板
				if( Msg.Data.UI.UIType == UI_TYPE_FIT )
				{
					char buf[256];
					sprintf( buf, "openFittingSetting(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 公会查询面板
				if ( Msg.Data.UI.UIType == UI_TYPE_CLAN_SEARCH )
				{
					char buf[256];
					sprintf( buf,"showClanListFrame(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 公会创建面板
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_CREATE )
				{
					char buf[256];
					sprintf( buf,"ShowCreatClanFrame(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 公会解散面板
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_DISBAND )
				{
					char buf[256];
					sprintf( buf,"DisBindClan(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 公会宣告面板
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_MANIFESTO )
				{
					char buf[256];
					sprintf( buf,"ManifestoClan(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 装备品质提升
				if( Msg.Data.UI.UIType == UI_TYPE_ARM_STAR )
				{
					char szScript[256];
					sprintf( szScript, "ArmProceArmStar(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}

				// 宝石升级
				if( Msg.Data.UI.UIType == UI_TYPE_GEM_LEVEL_UP )
				{
					char szScript[256];
					sprintf( szScript, "ArmProceArmGemLevelup(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}

				// 宝石镶嵌
				if( Msg.Data.UI.UIType == UI_TYPE_GEM_INLAY )
				{
					char szScript[256];
					sprintf( szScript, "ArmProceArmGemInLay(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}

				// 宝石摘取
				if( Msg.Data.UI.UIType == UI_TYPE_GEM_PICK )
				{
					char szScript[256];
					sprintf( szScript, "ArmProceArmGemPickup(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}

				// 配件摘取
				if ( Msg.Data.UI.UIType == UI_TYPE_SKILL_FITTING_INLAY )
				{
					char szScript[256];
					sprintf( szScript, "openFittingSetting(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}

				// 宠物操作面板
				if ( std::find( s_petUIs, s_petUIs + sizeof( s_petUIs ) / sizeof( int ), Msg.Data.UI.UIType ) !=  
					s_petUIs + sizeof( s_petUIs ) / sizeof( int ) )
				{
					char buf[256];
					sprintf( buf, "openPetOperationUI(%d, %d)", Msg.Data.UI.UIType,Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 天赋系统面板
				if ( Msg.Data.UI.UIType == UI_TYPE_GENIUS )
				{
					char buf[256];
					sprintf( buf, "openGeniusUI( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 兑换金币
				if ( Msg.Data.UI.UIType == UI_TYPE_CHANGE_COIN_INTO )
				{
					char buf[256];
					sprintf(buf,"openChangeCoin(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}

				// 打开百层塔主面板
				if (Msg.Data.UI.UIType == UI_TYPE_BCT)
				{
					char buf[256];
					sprintf(buf,"openHundredLevelTown(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				// 生活技能
				if( Msg.Data.UI.UIType >=UI_TYPE_MINING_RECIPE && Msg.Data.UI.UIType <=UI_TYPE_ENGINEERING_RECIPE )
				{
					char buf[256];
					sprintf(buf,"SecondarySkillOpenFrame(%d,%d)",Msg.Data.UI.UIType - UI_TYPE_MINING_RECIPE + 1,Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_RESEARCH )
				{
					char buf[256];
					sprintf(buf,"openClanResearchFrame(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_MAKING )
				{
					char buf[256];
					sprintf( buf,"openClanMakingMachineFrame(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_FACTORY )
				{
					char buf[256];
					sprintf(buf,"openClanMakingMachineFrame(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_CLAN_STORAGE )
				{
					char buf[256];
					sprintf( buf,"OpenClanStorageFrame(%d)",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_UPGRADE_CLAN_CITY )
				{
					char buf[20];
					sprintf( buf, "UpdateClan( %d,%d )", UI_TYPE_UPGRADE_CLAN_CITY,Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				if(					
					Msg.Data.UI.UIType == UI_TYPE_UPGRADE_TECH_CENTER ||
					Msg.Data.UI.UIType == UI_TYPE_UPGRADE_COLLECTION_CENTER ||
					Msg.Data.UI.UIType == UI_TYPE_UPGRADE_FACTORY ||
					Msg.Data.UI.UIType == UI_TYPE_UPGRADE_FARM || 
					Msg.Data.UI.UIType == UI_TYPE_UPGRADE_WALL )
				{
					int nIdx = Msg.Data.UI.UIType - UI_TYPE_UPGRADE_TECH_CENTER;
					GameClanManager::getSingleton().getClanCity().setSelectBuilding( nIdx );
					char buf[20];
					sprintf( buf, "UpdateClan( %d,%d )", UI_TYPE_UPGRADE_CLAN_CITY_BUILDING,Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_ARM_BIND )
				{
					char szScript[256];
					sprintf( szScript, "ArmProceArmBind(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_ARM_UNBIND )
				{
					char szScript[256];
					sprintf( szScript, "ArmProceArmUnbind(%d)", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_ARM_MOVE )
				{
					char szScripts[256];
					sprintf(szScripts,"ArmProceMoveStar(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				if( Msg.Data.UI.UIType == UI_TYPE_OPEN_BATTLE )
				{
					GameManager::getSingleton().getScriptVM()->callString("BattleStartFrame:Show()");
				}else if( Msg.Data.UI.UIType == UI_TYPE_SAFEPWD_MODIFY )
				{
					char szScripts[256];
					sprintf(szScripts,"ContainerModifyPwd(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if( Msg.Data.UI.UIType == UI_TYPE_SAFEPWD_CANCEL )
				{
					char szScripts[256];
					sprintf(szScripts,"ContainerCancelPwd(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if( Msg.Data.UI.UIType == UI_TYPE_ARM_UNFENGYIN )
				{
					char szScripts[256];
					sprintf(szScripts,"StartUnFengYinOp( %d )",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if( Msg.Data.UI.UIType == UI_TYPE_ARM_RECAST )
				{
					char szScripts[256];
					sprintf( szScripts,"NormalArmRecast( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts ) ;
				}else if( Msg.Data.UI.UIType == UI_TYPE_ARM_RECAST_ADVANCE )
				{
					char szScripts[256];
					sprintf( szScripts,"AdvanceArmRecast( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if (Msg.Data.UI.UIType == UI_TYPE_RUNE_LEVEL_UP)
				{
					GameManager::getSingleton().getScriptVM()->callString("OpenSkillRuneCombineFrame()");
				}else if ( Msg.Data.UI.UIType == UI_TYPE_CLAN_GIFT )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenClanGiftFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if (Msg.Data.UI.UIType == UI_TYPE_UNREAL)
				{
					char script[256];
					sprintf(script,"OpenDreamLandSelectLayerFrame(%d)",Msg.ID);
					GameManager::getSingleton().getScriptVM()->callString(script);
				}else if( Msg.Data.UI.UIType == UI_TYPE_ARM_WASH )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenArmWashFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if( Msg.Data.UI.UIType == UI_TYPE_ARM_OPERATE )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenArmProceFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_SPY_TRANS )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenChoseClanCityFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_CLAN_DONATE_LOG )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenClanDonateLogFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_SET_BANNER )
				{
					char szScripts[256];
					sprintf( szScripts,"SetBaseWarBanner( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_SET_BANNER_XUANYAN )
				{
					char szScripts[256];
					sprintf( szScripts,"SetBaseWarBannerXuanyan( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_CLAN_GET_RES_INFO )
				{
					char szScripts[256];
					sprintf( szScripts,"ShowGetResInfo( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_PET_OPERATE )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenPetProceFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_RIDE_MOVE_LEVEL )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenRideMoveLevelFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_RESET_RONGLU )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenMagicSmelterFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_ARM_LEVELUP )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenArmLevelUpFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if( Msg.Data.UI.UIType == UI_TYPE_ORANGE_ARM_LEVELUP )
				{
					char szScripts[256];
					sprintf( szScripts,"OpenArmLevelUpOrangeFrame( %d )",Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_ADDATTR_MOVE )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmAttrMoveFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_ATTR_RESET )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmResetFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_PET_MOVE_STAR )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenPetStarMoveFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_PET_EAT )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenPetEatFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_PET_STEP_UP )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenPetStepUpFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}else if ( Msg.Data.UI.UIType == UI_TYPE_CLAN_BOON )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenClanBoonFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_ADDATTR_MOVE_GAOJI )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmAdvAttrMoveFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_STUDENT_REG )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenStudentRegistMsg( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_ARM_STEP_UP )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmStepUpFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_ARM_SKILL_MOVE_LOW )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmSkillAttrMoveFrame( %d,%d )", Msg.ID,UI_TYPE_ARM_SKILL_MOVE_LOW );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_ARM_SKILL_MOVE_HIGH )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmSkillAttrMoveFrame( %d,%d )", Msg.ID,UI_TYPE_ARM_SKILL_MOVE_HIGH );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_PET_IMP_UP )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenPetImpUpFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_BACK_UNREAL )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenUnRealBack( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_PET_UNBIND )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenPetUnBindFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_SKILL_ACTIVE )
				{
					char szScripts[256];
					sprintf( szScripts, "OpenArmSkillRereateFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
				else if ( Msg.Data.UI.UIType == UI_TYPE_HOME ) {
					char szScripts[256];
					sprintf( szScripts, "OpenInquireFrame( %d )", Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString( szScripts );
				}
			}
			break;

		case ACTION_TEAM:
			{
				OnActionTeam( Msg );
			}
			break;
		case ACTION_LEVEL:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if( pActor != NULL )
				{
					// 角色升级
					if (!pActor->isOnMonsterMachine())
					{
						pActor->getShowActor()->getCurEntity()->playMotion( "9502_16", false, 9502 );
					}
					if( pActor == pMainPlayer )
					{
						GameWizard::getSingleton().springByLevelUp( Msg.Data.Level);
						GameManager::getSingleton().SendUIEvent("GE_PLAYER_LEAVEUP");
					}else{
						pActor->setLv( Msg.Data.Level );
						if ( pActor->getType() == GAT_PET && pMainPlayer->GetID() == pActor->getOwnerId() )
						{
							char buffer[50];
							sprintf( buffer, "PetActorLevelUp( %d, %d )", pActor->GetID(), Msg.Data.Level );
							ActorManager::getSingleton().getScriptVM()->callString( buffer );
						}
					}
				}
			}
			break;
		case ACTION_TMP:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if ( pActor == NULL )
				{
					break;
				}
				// 角色获取临时道具
				pActor->getShowActor()->getCurEntity()->playMotion( "9504_16", false, 9504 );
				pActor->m_Equip.setTmpArm( Msg.Data.TmpItem.TmpItemID );
				pActor->m_Equip.setTmpOpType( Msg.Data.TmpItem.OperType );

				if( Msg.Data.TmpItem.TmpItemID != 0 )
				{
					const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( Msg.Data.TmpItem.TmpItemID );
					if( itemDef.ItemTmp.Type == ITEM_TMP_SUPPLY )
					{
						pActor->getShowActor()->getCurEntity()->playMotion( "720300_4", false, 720300 );
					}
					else if (itemDef.ItemTmp.Type == ITEM_TMP_MACHINE)
					{
						GameActor *TmpItem = ActorManager::getSingleton().FindActor(Msg.Data.TmpItem.MemIndex);
						if (TmpItem != NULL)
						{
							pActor->SetFaceTo(TmpItem->getAngle(), true);
						}
					}
				}
			}
			break;
		case ACTION_USE_ITEM:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if( pActor == ActorManager::getSingleton().getMainPlayer() )
					{
						((MainGamePlayer*)pActor)->getContainer().passItemCD( Msg.Data.UseItem.ItemID );
					}

					// HP补给类（一次性）：5000000~5000999
					// MP补给类（一次性）：5001000~5001999
					// HP补给类（持续性）：5002000~5002999
					// MP补给类（持续性）：5003000~5003999
					if( Msg.Data.UseItem.ItemID / 1000 == 5000 )
					{
						pActor->getShowActor()->getCurEntity()->playMotion( "5000000_15", false, 5000000 );
					}
					if( Msg.Data.UseItem.ItemID / 1000 == 5001 )
					{
						pActor->getShowActor()->getCurEntity()->playMotion( "5001000_15", false, 5001000 );
					}
					pActor->finishChant( eUseItem_Chant );
					if( Msg.Data.UseItem.ItemID == 5251504 && pActor->canPlayChatAction() )
					{
						pActor->playChatAction( 10714, true );
					}
				}
			}
			break;
		case ACTION_EFFECT:
			{
				if( Msg.ID == 0 )
				{
					GameManager::getSingleton().getScriptVM()->callString( Msg.Data.Effect.EffectStr );
				}else{
					char buf[256];
					sprintf( buf, Msg.Data.Effect.EffectStr,  Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}
			}
			break;
		case ACTION_TASK:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->getShowActor()->getCurEntity()->playMotion( "9506_16", false, 10000 );
					if( pActor == ActorManager::getSingleton().getMainPlayer() )
					{
						const CSTASKACTION&  taskActionData = Msg.Data.Task;
						GameWizard::getSingleton().springByQuestCommit( taskActionData.TaskID );
						GameQuest::getSingleton().addQuestFini( taskActionData.TaskID );
						GameQuest::getSingleton().updateNpcQuestTitle();
						GameManager::getSingleton().SendUIEvent( "GE_QUEST_CHANGE" );
					}
				}
			}
			break;
		case ACTION_AUTOFLY:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if( Msg.Data.AutoFly.AutoPathIdx == 0 )
					{
						pActor->ChangeActState( GSACT_STAND );
						pActor->getSpecialMove()->setType(MOVETYPE_NOMAL);
					}else{
						if( pActor->getType() == GAT_MAINPLAYER )
						{
							((MainGamePlayer*)pActor)->breakPreAction();
							GameManager::getSingleton().SendUIEvent( "GE_MAIN_AUTO_FLY" );
						}
						pActor->setAutoFly( Msg.Data.AutoFly.AutoPathIdx );
					}
				}
			}
			break;
		case ACTION_COLLECT:
			{
				GameActor* pActor	= ActorManager::getSingleton().FindActor( Msg.ID );
				GameActor* pMonster	= ActorManager::getSingleton().FindActor( Msg.Data.Collect.MonID );
				if( pMonster != NULL )
				{
					pMonster->reSetHp( Msg.Data.Collect.MonCurrHP );
				}

				if( pActor != NULL )
				{
					if(  Msg.Data.Collect.Flag == 0 && pMonster != NULL )
					{
						if (pMonster->getType() == GAT_ITEM)
						{
							if( pActor == ActorManager::getSingleton().getMainPlayer() )
							{
								int collectTime = pMonster->m_CollectTime;
								const char *text = "采集中";
								char szScript[256];
								sprintf( szScript, "BeginCollect(%d,%d,[===[%s]===])", collectTime, pMonster->GetID(), text);
								GameManager::getSingleton().getScriptVM()->callString(szScript);
							}
						}
						else
						{
							const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef( pMonster->getResId() );
							if( pActor == ActorManager::getSingleton().getMainPlayer() )
							{
								int collectTime = 0;
								if (pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID > 0)
								{
									GameActor* pMonster = ActorManager::getSingleton().FindActor( pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID );
									if( pMonster != NULL )
									{
										collectTime = pMonster->m_MonsterMachine.m_monsterInfo.m_nCollcetTime;
									}
								}
								else
								{
									collectTime = MonsterDef.Collect.CollectTime;
								}
								const char *text = "采集中";
								if (MonsterDef.CollectActDesc[0] != '\0')
								{
									text = MonsterDef.CollectActDesc;
								}
								char szScript[256];
								sprintf( szScript, "BeginCollect(%d,%d,[===[%s]===])", collectTime, pMonster->GetID(), text);
								GameManager::getSingleton().getScriptVM()->callString(szScript);
							}
							if( MonsterDef.Collect.CollectSecondarySkillType > 0 )
							{
								if( pActor->isInMove() )
								{
									pActor->CmdMoveStop();
								}
								pActor->getActState().SetState(GSACT_COLLECTITEM);
								if( !pActor->m_Rider.isOnHorse() )
								{								
									if( pActor->getShowActor()->getCurAction() != 10251 )
									{
										pActor->getShowActor()->stopCurAction();
										pActor->getShowActor()->playAction( 10251, NULL, 0, 0 );										
									}
								}
								break;
							}
						}
						if( pActor->getActState().GetState() != GSACT_COLLECTITEM )
						{
							pActor->m_Equip.hideWeapon();
							pActor->ChangeActState( GSACT_COLLECTITEM );
						}
						if( pActor->isOnMonsterMachine() )
						{
							GameActor* pMonMonster = ActorManager::getSingleton().FindActor( pActor->getMonsterMachine()->m_playerInfo.m_nMonsterMemID );
							if( pMonMonster )
							{
								pMonMonster->ChangeActState( GSACT_COLLECTITEM );
							}
						}						
					}else{
						if( pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID == 0 )
						{
							if( !pActor->getRider()->isOnHorse() && pActor->m_pCarry->getCarryState() == eNo_Carry )
							{
								pActor->m_Equip.showWeapon();
							}	
						}
						if( pActor->getActState().GetState() == GSACT_COLLECTITEM )
						{
							pActor->getActState().SetState(GSACT_STAND);
							pActor->onChangeStand();	
						}
						if( pActor->isOnMonsterMachine() )
						{
							GameActor* pMonMonster = ActorManager::getSingleton().FindActor( pActor->getMonsterMachine()->m_playerInfo.m_nMonsterMemID );
							if( pMonMonster && pMonMonster->getActState().GetState() == GSACT_COLLECTITEM )
							{
								pMonMonster->getActState().SetState(GSACT_STAND);
								pMonMonster->onChangeStand();
							}
						}

						if( pActor == ActorManager::getSingleton().getMainPlayer() )
						{
							GameManager::getSingleton().getScriptVM()->callString("EndCollect()");
						}
					}
				}
			}
			break;
		case ACTION_SKILL_LEARN:
			{
				char buf[256];
				const CSACTIONSKILLLEARN& skillLearnData = Msg.Data.SkillLearn;
				if( Msg.ID ==  ActorManager::getSingleton().getMainPlayer()->GetID() && 
					( skillLearnData.Type == SKILL_LEARN || skillLearnData.Type == SKILL_LEVEL_UP ) )
				{
					sprintf( buf, "SkillLearn( %d, %d )", skillLearnData.ID, skillLearnData.Level );
					ActorManager::getSingleton().getScriptVM()->callString( buf );
				}
			}
			break;
		case ACTION_FLY:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if( Msg.Data.Fly.Type == 1 )
					{
						pActor->getActState().SetState( GSACT_STAND );
						pActor->playAction( pActor->getActState().getActionId(true) );
						pActor->playAction( 10125, NULL, 0, 0 );
					}else{
						pActor->getActState().SetState( GSACT_FLY );
						pActor->playAction( pActor->getActState().getActionId(true) );
						pActor->playAction( 10122, NULL, 0, 0 );
					}
				}
			}
			break;
		case ACTION_QIANXIN:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					//if( Msg.Data.QianXin.Flag == 1 )
					//{
					//	pActor->getShowActor()->getCurEntity()->setTransparent( 0.5f );
					//}else{
					//	pActor->getShowActor()->getCurEntity()->setTransparent( 1.0f );
					//}
				}
			}
			break;

		case ACTION_RIDE_PRE:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor && pActor->getType() != GAT_MAINPLAYER )
				{
					if( Msg.Data.RidePre.RideItemID != 0 )
					{
						pActor->startRideChant( Msg.Data.RidePre.RideItemID,Msg.Data.RidePre.RideLevel );
					}
				}
			}
			break;

		case ACTION_RIDE:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor )
				{					
					if( Msg.Data.Ride.RideFlag == CLIENT_ACTION_RIDE_ON )
					{
						pActor->finishChant( eRide_Chant );
						pActor->m_Rider.rideOn( Msg.Data.Ride.RideItemID, Msg.Data.Ride.RideLevel );

						if ( pActor->getActState().GetState() == GSACT_MOVE )
						{
							int actionId = pActor->getActState().getActionId( true );
							if ( pActor->m_Rider.m_pShowActor )
							{
								const char* szFootString[4];
								std::string strFoot("footprint");
								szFootString[0] = strFoot.c_str();
								pActor->m_Rider.horsePlayAct( actionId, szFootString, 1, 0 );
							}
						}
					}else if( Msg.Data.Ride.RideFlag == CLIENT_ACTION_RIDE_DOWN )
					{
						pActor->m_Rider.rideDown();

						if ( pActor->getActState().GetState() == GSACT_MOVE )
						{
							int actionId = pActor->getShowActor()->getCurAction();
							
							const char* szFootString[4];
							std::string strFoot("footprint");
							szFootString[0] = strFoot.c_str();
							pActor->getShowActor()->playAction( actionId, szFootString, 1, 0 );
						}
					}
				}
			}
			break;

		case ACTION_HITCH_RIDE:
			{
				const tagCSRideOnInfo& rideOnInfo = Msg.Data.HitchRide;
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				assert( Msg.ID != rideOnInfo.OwnerID );
				if( pActor != NULL )
				{
					pActor->breakBTypePre();
					// 让客人上主人的坐骑
					pActor->m_Rider.rideOnSide( rideOnInfo );
				}
			}
			break;

		case ACTION_LEAVE_RIDE:
			{
				const tagCSRideOnInfo& rideOnInfo = Msg.Data.HitchRide;
				GameActor* pLeaveActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if ( pLeaveActor != NULL )
				{
					if ( pLeaveActor->m_Rider.isOnHorse() )
					{
						pLeaveActor->m_Rider.downHorseSide();
						GameActor* pRideActor = ActorManager::getSingleton().FindActor( rideOnInfo.OwnerID );
						if( pRideActor )
						{
							pRideActor->m_Rider.setPassengersInfo( rideOnInfo.PassengerNum,rideOnInfo.PassengerID );
						}
					}

					pLeaveActor->SetPosition( rideOnInfo.Pos.X, rideOnInfo.Pos.Y );
				}
			}
			break;

		case ACTION_RIDE_LEARN:
			{
				if( Msg.ID ==  ActorManager::getSingleton().getMainPlayer()->GetID() )
				{
					GameManager::getSingleton().getScriptVM()->callString( "UpdateRideFrame()" );
				}
			}
			break;

			case ACTION_RIDE_ON_MACHINE:
				{
					GameActor* pActActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if ( pActActor != NULL )
					{
						const CSACTIONRIDEONMACHINE& rideOnMachineInfo = Msg.Data.RideOnMachine;
						pActActor->m_MonsterMachine.setOwner( pActActor->GetID() );
						pActActor->m_MonsterMachine.rideOnMonsterMachine( rideOnMachineInfo );
					}
				}
				break;

			case ACTION_LEAVE_MACHINE:
				{
					GameActor* pActActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if ( pActActor != NULL )
					{
						const CSACTIONLEAVEMACHINE& rideOnMachineInfo = Msg.Data.LeaveMachine;
						pActActor->m_MonsterMachine.downMonsterMachine( rideOnMachineInfo );
					}
				}
				break;

			case ACTION_MACHINE_ATTR_CHG:
				{
					GameActor* pActActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if ( pActActor != NULL )
					{
						const CSACTIONMACHINEATTRCHG& chgAttr = Msg.Data.MachineAttrChg;
 						pActActor->m_MonsterMachine.setAttriValule( chgAttr.AttrType, chgAttr.CurrVal );
					}
				}
				break;
			case ACTION_MACHINE_PASSENGER_INFO:
				{
					GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if( pActor != NULL )
					{
						pActor->m_MonsterMachine.ChangePassengerInfo( Msg.Data.PassengerInfo );
						if( !pActor->m_MonsterMachine.canChangeSide() && !ActorManager::getSingleton().getMainPlayer()->isOnMonsterMachine() )
						{
							int nIdx = pActor->m_MonsterMachine.getFirstEmptyIdx();
							if( nIdx >=0  )
							{
								ActorManager::getSingleton().getMainPlayer()->requestRideMonsterMachine( pActor->GetID(),nIdx );
							}
						}else{
							if( Msg.ID == ActorManager::getSingleton().m_RequestMachineId )
							{
								char buf[256];
								sprintf( buf,"OnPassengerInfo(%d)",Msg.ID );
								GameManager::getSingleton().getScriptVM()->callString( buf );
							}
						}
					}
				}
				break;
			case ACTION_INPWORD_SAFETIME_DOWN:
				{
					GameActor* pTargetActor		= ActorManager::getSingleton().FindActor( Msg.ID );
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					if ( pTargetActor != NULL && pMainPlayer == pTargetActor )
					{
						char buf[256];
						sprintf( buf, "auteamTimeDown(%d)", Msg.Data.InPworldSafeTimeDown );
						GameManager::getSingleton().getScriptVM()->callString(buf);
					}
				}
				break;
			
			case ACTION_CLOSE_DIALOG:
				{
					const CSACTIONCLOSEDIALOG& closeDialogData = Msg.Data.CloseDialog;
					GameActor* pTargetActor		= ActorManager::getSingleton().FindActor( Msg.ID );
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					if ( Msg.ID == pMainPlayer->GetID() )
					{
						if ( closeDialogData.Type == 0 )
						{
							GameMsgPrint::getSingleton().SendUIEvent( "GE_CLOSE_CLAN_DIALOG" );
							// npc 身上没有挂脚本
							GameNpcDialog::getSingleton().clearNpcData();
						}else if( closeDialogData.Type == 1 )
						{
							//
							GameNpcDialog::getSingleton().endTalk();
						}
					}
					
				}
				break;

			case ACTION_PET_CAMPAIGN_PRE:
				{
					GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if( pActor && pActor->getType() != GAT_MAINPLAYER )
					{
						pActor->startPetChant( Msg.Data.PetCampaignPre.PetID );
					}
				}
				break;

		case ACTION_PET_VIEW:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					int nNum = Msg.Data.PetView.PetNum;
					GamePet* pPetPackage = pActor->getPet();
					/*int hetiIdx = pPetPackage->getPetHeti();
					if (hetiIdx != -1)
					{
						GameActor *pPet = pPetPackage->getPetActor(hetiIdx);
						if (pPet != NULL)
						{
							pPetPackage->setPetHeti( pPet->getRoleWId(),0 , false);
						}
					}*/
					for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
					{
						std::list<int> posList;

						for( int j = 0; j < nNum; j++ )
						{
							const CSPETVIEW& petViewData = Msg.Data.PetView.PetView[j];
							if( petViewData.XieDaiPos == i )
							{
								std::string strPetName = Msg.Data.PetView.PetView[j].PetName;
								if( strPetName.empty() )
								{
									strPetName = pActor->m_Pet.getPetDef( Msg.Data.PetView.PetView[j].PetID )->Name;
								}

								GameActor* pPet = pPetPackage->getPetActor( i );
								// 若宠物已召回或者已经死亡了，则重新出征宠物
								if( pPet == NULL || pPet->isDead() )
								{
									pActor->m_Pet.setShiModle( pActor->GetID(), 
										petViewData.XieDaiPos, 
										petViewData.PetID,
										strPetName.c_str(),
										petViewData.Power,
										petViewData.MaxPower, petViewData.Level,
										petViewData.PetWID, PET_OP_FIGHT, 
										petViewData.Passive,
										petViewData.DiathesisNum,
										petViewData.Diathesis,
										petViewData.GoodStatusNum,
										petViewData.GoodStatusList,
										petViewData.Genius,
										petViewData.StrengthenLevel,
										petViewData.Zhanchong );
									GameActor* myPet = pPetPackage->getPetActor( i );
									pActor->m_Pet.SetGeniusInfo( myPet, petViewData.GeniusSkillID, petViewData.GeniusSkillLvl );
								}
								else
								{
									pPet->setName( strPetName.c_str() );
									pPet->setPetFightMode( petViewData.Passive );
									GameActor* myPet = pPetPackage->getPetActor( i );
									pActor->m_Pet.SetGeniusInfo( myPet, petViewData.GeniusSkillID, petViewData.GeniusSkillLvl );
								}

								if (pActor->isDead() || petViewData.HetiFlag != 0)
								{
									pPetPackage->getPetActor(i)->OnLeaveMap();
									if (petViewData.HetiFlag != 0)
									{
										pPetPackage->setPetHeti(petViewData.PetWID,Msg.Data.PetView.PetView[j].Genius, true, petViewData.Zhanchong);
									}
								}
								else
								{
									pPet = pPetPackage->getPetActor( i );
									if ( pPet && petViewData.Zhanchong == 1 )
									{
										pPetPackage->setBattlePetEntity( pPet, petViewData.Genius );
									}
								}
								
								posList.push_back( j );
							}
						}
						for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
						{
							bool bFind = false;
							for( std::list<int>::iterator iter = posList.begin(); iter != posList.end(); ++iter )
							{
								if( i == *iter )
								{
									bFind = true;
								}
							}
							if( !bFind )
							{
								pActor->m_Pet.setShiModle( 0, i, 0, "", 0, 0, 0, 0, PET_OP_BACK );
							}
						}
					}
// 					if (pActor->getType() == GAT_MAINPLAYER)
// 					{
// 						GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_BUF_LIST_UPDATE");
// 						GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_PET_HETI");
// 					}
// 					MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
// 					if (pMainPlayer && pActor->GetID() == pMainPlayer->GetSelectTarget())
// 					{
// 						// update TargetUI
// 						GameMsgPrint::getSingleton().SendUIEvent("GE_TARGET_BUF_LIST_UPDATE");
// 						GameMsgPrint::getSingleton().SendUIEvent("GE_TARGET_PET_HETI");
// 					}
				}
			}
			break;
		case ACTION_PET_HETI:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if (pActor != NULL)
				{
					bool result = (Msg.Data.Heti.Flag == 0);
					pActor->getPet()->setPetHeti(Msg.Data.Heti.PetID, Msg.Data.Heti.PetGenius, result, Msg.Data.Heti.Zhanchong );
					pActor->getPet()->onPetHeti(result);
				}
			}
			break;
		case ACTION_FAIRY_VIEW:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
					const CSROLEFAIRYACTION& fairyActionData = Msg.Data.FairyAct;
					GameTrump* pTrumpMgr = pActor->getTrump();
					char buffer[256];
					//	放出精灵
					if ( fairyActionData.Flag == 0 )
					{
						pTrumpMgr->m_nCurCampFairyId = fairyActionData.ID;
						pTrumpMgr->m_nCurCampFairyLv = fairyActionData.LV;
						
						sprintf( buffer, "FairyCamp(%d, %d, %d )", Msg.ID, fairyActionData.ID, fairyActionData.LV );
						GameManager::getSingleton().getScriptVM()->callString( buffer );
					}
					else if ( fairyActionData.Flag == 1 )
					{
						int nTempCampFairyId = pTrumpMgr->m_nCurCampFairyId;
						int nTempCampFairyLv = pTrumpMgr->m_nCurCampFairyLv;
						pTrumpMgr->m_nCurCampFairyId = 0;
						pTrumpMgr->m_nCurCampFairyLv = 0;
						sprintf( buffer, "FairyCallBack(%d, %d, %d)", Msg.ID, nTempCampFairyId, nTempCampFairyLv );
						GameManager::getSingleton().getScriptVM()->callString( buffer );
					}

					if ( pMainplayer->GetID() ==Msg.ID )
					{
						GameManager::getSingleton().SendUIEvent( "GE_MAIN_TRUMP_FIGHT_STATE_CHG" );
					}
					pActor->loadFairyEffect();
				}
			}
			break;
		case ACTION_QIYUE:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					char buf[256];
					sprintf( buf, "qiyue_%d", Msg.Data.QiYue.QiYueID );
					pActor->getShowActor()->getCurEntity()->playMotion( buf, false, 10000 );
				}	
			}
			break;
		case ACTION_STALL:
			{
				if ( Msg.Data.Stall.Flag == 1 )
				{
					char buf[256];
					sprintf( buf, "CallScript_ActorEndStall([===[%s]===], %d)", Msg.Data.Stall.StallName, Msg.Data.Stall.Flag );
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor == NULL )
					break;

				if( Msg.Data.Stall.Flag == 0 )
				{
					pActor->ChangeActState( GSACT_STALL );
					pActor->OnStall(true);
					pActor->setStallName( Msg.Data.Stall.StallName );
				//	pActor->setStallNameColor( SELF_TITLE_COLOR );
					if ( pActor->getType() == GAT_MAINPLAYER )
					{
						pActor->breakBTypePre();
					}
				}
				else
				{
					const char* pszStallName = pActor->getStallName();
					if ( pszStallName[0] != 0 )
					{
						string strStallBtnName( pszStallName );
						strStallBtnName += "头顶UI";
						g_pFrameMgr->hideFrame( strStallBtnName.c_str() );

					}
					pActor->setStallName( "" );
					pActor->OnStall(false);
					if (pActor->getActState().GetState() != GSACT_DEAD)
					{
						pActor->ChangeActState( GSACT_STAND );
					}
					else
					{
						int actionId = pActor->getActState().getActionId();
						if ( !pActor->getShowActor()->hasAnim( actionId ) )
						{
							pActor->getShowActor()->playAction( pActor->getActState().getActionId(true), NULL, 0, 0 );
						}
						else
						{
							pActor->getShowActor()->playAction( actionId, NULL, 0, 0 );
						}
					}
					// 检查当前我所查看的摊主名称是否为当前收摊的这个
					if ( pActor != ActorManager::getSingleton().getMainPlayer() && stricmp( pActor->getName(), 
						GameStallManager::getSingleton().getSeeStallItem().m_szRoleName ) == 0 )
					{
						GameManager::getSingleton().SendUIEvent( "GE_STALL_BUY_END" );
					}
				}

				if ( pActor == ActorManager::getSingleton().getMainPlayer() )
				{
					char buf[256];
					sprintf( buf, "CallScript_UpdateStallState(%d,%d)", Msg.Data.Stall.Flag, Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}
				else
				{
					char buf[256];
					sprintf( buf, "CallScript_ActorStall(%d,%d)", Msg.Data.Stall.Flag, Msg.ID );
					GameManager::getSingleton().getScriptVM()->callString(buf);
				}

				RoleText& actorTitleMgr = pActor->getRoleText();
				actorTitleMgr.BuildTitle();

				if (Msg.Data.Stall.Flag == 1 && Msg.Data.Stall.InView == 0 && pActor != ActorManager::getSingleton().getMainPlayer())
				{
					EffectManage::getSingleton().clearLink(pActor->GetID());
					IGameBattle::getInstance()->objLeaveView(pActor);
					IGameWizard::getInstance()->objLeaveView(pActor);
					IWorldCityManager::getInstance()->objLeaveView(pActor);
					pActor->OnLeaveMap();
					ActorManager::getSingleton().DestroyActor(pActor);
				}
			}
			break;
		case ACTION_SIT_DOWN:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if( Msg.Data.Sit.Flag == 1 || Msg.Data.Sit.Flag == 0 )
					{
						pActor->sitdown();
					}
					else
					{
						pActor->situp();
					}
				}
			}
			break;
		case ACTION_PAK_ENLARGE:
			{
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if( pMainPlayer->GetID() == Msg.ID )
				{
					pMainPlayer->getContainer().setContainerGridInfo( Msg.Data.PakEnlarge.ListType, 
						Msg.Data.PakEnlarge.Num );
				}
			}
			break;
		case ACTION_CLAN_CHG:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if( pActor != NULL )
				{
					const CSROLECLANVIEW& clanViewActionData = Msg.Data.ClanChg;
					if( clanViewActionData.Position == 0 )
					{
						pActor->setClanName("");
						pActor->m_nClanPosition	= clanViewActionData.Position;
						pActor->m_nClanGroup = clanViewActionData.Group;
						pActor->setClanWID( 0 );
					}else{
						pActor->setClanName( clanViewActionData.Name );
						pActor->m_nClanPosition	= clanViewActionData.Position;
						pActor->m_nClanGroup = clanViewActionData.Group;
						pActor->setClanWID( clanViewActionData.GID );
					}
					if ( Msg.ID == pMainPlayer->GetID() )
					{
						GameQuest::getSingleton().updateNpcQuestTitle();
						GameClanManager::getSingleton().updateAllClan();
					}
					else
					{
						GameClanManager::getSingleton().updateClan(pActor);
					}

					RoleText& actorTitleMgr = pActor->getRoleText();
					actorTitleMgr.BuildTitle();
				}
			}
			break;

		case ACTION_HP_CHG:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				const CSHPCHG& hpChg = Msg.Data.HPChg;

				if ( pActor != NULL )
				{
					unsigned int uStartTime = timeGetTime();
					if (hpChg.ChgHP > 0)
					{
						GameDamageHealCounter::getSingleton().IncHeal(hpChg.ApplyID, Ogre::Min(hpChg.ChgHP, int(hpChg.CurrMaxHP - pActor->getHP())));
					}
					else
					{
						GameDamageHealCounter::getSingleton().IncDamage(hpChg.ApplyID, -hpChg.ChgHP);
						GameDamageHealCounter::getSingleton().IncHurt(Msg.ID, -hpChg.ChgHP);
					}
					bool isMaxHPChg = ( pActor->getMaxHP() != hpChg.CurrMaxHP );
					int nChangeHP	= hpChg.ChgHP;
					int nCalHP		= pActor->getHP() + nChangeHP;
					pActor->setTmpHp( hpChg.CurrHP );
					pActor->setHP( pActor->getHP() + nChangeHP );
					pActor->setMaxHP( hpChg.CurrMaxHP );

					SCTDamageManager&	sctMgr		= SCTDamageManager::getSingleton();
					SCTDamageBase*		pSCTDamage	= NULL;
					int					nHurtType	= 0;

					// 显示治疗量
					pSCTDamage	= NULL;
					if (nChangeHP > 0)
					{
						if (hpChg.ApplyID != pActor->GetID() && !pMainPlayer->isRideMonster(pActor))
						{
							pSCTDamage = sctMgr.getPassiveSCT(hpChg.ApplyID);
							nHurtType = HURT_TYPE_HP;
						}
					}
					if ( pSCTDamage != NULL )
					{

						pSCTDamage->calculateDamage( abs( nChangeHP ), 0, 0, pActor, nHurtType, 1, 0 );
						pSCTDamage->showFllyFont( uStartTime );
					}

					/// 显示受到的伤害
					pSCTDamage	= NULL;
					if ( !isMaxHPChg && nChangeHP > 0 )
					{
						if (pMainPlayer->isRideMonster(pActor))
						{
							pSCTDamage = sctMgr.getPassiveSCT(pMainPlayer->GetID());
						}
						else
						{
							pSCTDamage = sctMgr.getPassiveSCT(pActor->GetID());
						}
						nHurtType = HURT_TYPE_HP;
					}
					else if ( !isMaxHPChg && nChangeHP < 0 )
					{
						if (pMainPlayer->isRideMonster(pActor))
						{
							pSCTDamage = sctMgr.getPassiveSCT(pMainPlayer->GetID());
						}
						else
						{
							pSCTDamage = sctMgr.getPassiveSCT( pActor->GetID() );
						}
						nHurtType = HURT_TYPE_DMG;
					}	

					if ( pSCTDamage != NULL )
					{
						pSCTDamage->calculateDamage( abs( nChangeHP ), 0, 0, pActor, nHurtType, 1, 0 );
						pSCTDamage->showFllyFont( uStartTime );
					}

					/// 伤害提供者显示伤害数值
					{
						GameActor* pSource = ActorManager::getSingleton().FindActor(hpChg.ApplyID);

						pSCTDamage = NULL;
						if( pSource != NULL )
						{
							if ( nChangeHP > 0 )
							{
								if (pMainPlayer->isRideMonster(pSource))
								{
									pSCTDamage = sctMgr.getActiveSCT(pMainPlayer->GetID());
								}
								else
								{
									pSCTDamage = sctMgr.getActiveSCT(pSource->GetID());
								}
								nHurtType = HURT_TYPE_HP;
							}
							else if ( nChangeHP < 0 )
							{
								if (pMainPlayer->isRideMonster(pSource))
								{
									pSCTDamage = sctMgr.getActiveSCT(pMainPlayer->GetID());
								}
								else
								{
									pSCTDamage = sctMgr.getActiveSCT(pSource->GetID());
								}
								nHurtType = HURT_TYPE_DMG;
							}
						}

						if ( pSCTDamage != NULL )
						{
							pSCTDamage->calculateDamage( abs( nChangeHP ), 0, 0, pActor, nHurtType, 1, 0 );
							pSCTDamage->showFllyFont( uStartTime );
						}
					}

					/// 显示魔法增减数值
					pSCTDamage = NULL;
					int nChangeMP = hpChg.ChgMP;
					pActor->setMP( hpChg.CurrMP );
					pActor->setMaxMP( hpChg.CurrMaxMP );
					if ( pActor == pMainPlayer)
					{
						pSCTDamage = sctMgr.getPassiveSCT( pActor->GetID() );
						nHurtType = HURT_TYPE_MP;
					}

					if ( pSCTDamage == NULL && hpChg.ApplyID == Msg.ID )
					{
						pSCTDamage = sctMgr.getActiveSCT( pMainPlayer->GetID() );
						nHurtType = HURT_TYPE_MP;
					}

					if ( pSCTDamage != NULL )
					{
						pSCTDamage->calculateDamage( nChangeMP, 0, 0, pActor, nHurtType, 1, 0 );
						pSCTDamage->showFllyFont( uStartTime );
					}
					GameMsgPrint::getSingleton().updateActorAttr( pActor );
				}
			}
			break;
		case ACTION_JUMP:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->jump();
				}
			}
			break;
		case ACTION_FIGHT:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if( Msg.Data.Fight.FightStatus == 1 )
					{
						pActor->onFight();
					}else{
						pActor->onLeaveFight();
					}
				}
			}
			break;
		case ACTION_DESIGNATION_CHG:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				int nDesignationId = Msg.Data.DesignationChg.DesignationID;
				if( pActor != NULL )
				{
					if( nDesignationId != 0 )
					{
						const char* szDesignationName = DBDataManager::getSingleton().getDesignationDef( nDesignationId ).Name;
						pActor->setDesignation( szDesignationName );
						pActor->setDesignationID( nDesignationId );
					}else{
						pActor->setDesignation( "" );
						pActor->setDesignationID( 0 );
					}
				}

			}
			break;
		case ACTION_PK_MODE_CHG:
			{
				GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
				if ( pActor != NULL && pActor->GetID() == Msg.ID)
				{
					GameActor_PVP::getSingleton().onPkModeChanged(Msg.Data.PkModeChg.Mode);
					pActor->m_nPkMode = Msg.Data.PkModeChg.Mode;
				}
				else
				{
					pActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if( pActor != NULL )
					{
						pActor->m_nPkMode = Msg.Data.PkModeChg.Mode;
					}
				}
			}
			break;
		case ACTION_PK_VAL_CHG:
			{
				GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
				if (pActor->GetID() == Msg.ID)
				{
					GameActor_PVP::getSingleton().onPkValueChanged(Msg.Data.PkValChg.PKVal);
					pActor->setPkValue( Msg.Data.PkValChg.PKVal );
					GameManager::getSingleton().SendUIEvent("GE_MAIN_PK_VAL_CHG");
				}
				else
				{
					pActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if( pActor != NULL )
					{
						pActor->setPkValue( Msg.Data.PkValChg.PKVal );
					}
				}
			}
			break;
		case ACTION_PK_TIME:
			{
				GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pActor != NULL)
				{
					if (Msg.Data.PKTime.PkTime > 0)
					{
						pActor->m_leastNoCrimeTime = timeGetTime() + Msg.Data.PKTime.PkTime * 1000;
						//若pActor为怪物机械则把时间记在所有者身上
						if ( pActor->getSubType() == MON_SUB_TYPE_MACHINE )
						{
							GameActor *pOwner = ActorManager::getSingleton().FindActor( pActor->getOwnerId() );
							if ( pOwner != NULL )
							{
								pOwner->m_leastNoCrimeTime = timeGetTime() + Msg.Data.PKTime.PkTime * 1000;
							}
						}
					}
					else
					{
						pActor->m_leastNoCrimeTime = 0;
					}
				}
			}
			break;
		case ACTION_GET_XP:
			{
				
				GameActor *pOwner = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pOwner)
				{
					GameActor *source = ActorManager::getSingleton().FindActor(Msg.Data.GetXp.ID);
					if (source)
					{
						source->setDeathAction(new EffectXPPlay(source, pOwner, Msg.Data.GetXp.Xp));
					}
					else
					{
						// 如果起始点不存在
						int i = pOwner->getXP() + Msg.Data.GetXp.Xp;
						int j = pOwner->getMaxXP();
						pOwner->setXP(min(i, j));
					}
				}
			}
			break;
		case ACTION_MON_PK_MODE_CHG:
			{
				const CSMONPKMODECHG &monPK = Msg.Data.MonPkModeChg;
				GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pActor)
				{
					pActor->m_nPkMode = monPK.OwnerPKMode;
					pActor->setClanWID( monPK.OwnerClanID );
					pActor->m_PKInfoType = monPK.PKInfoType;
					switch (monPK.PKInfoType)
					{
					case MON_PK_TYPE_PLAYER:
						{
							pActor->setOwnerId(int(monPK.PkID));
						}
						break;

					case MON_PK_TYPE_TEAM:
						{
							pActor->setTeamWID(monPK.PkID);
						}
						break;

					case MON_PK_TYPE_CLAN:
						{
							pActor->setClanWID(monPK.PkID);
						}
						break;

					case MON_PK_TYPE_CAMP:
						{
							pActor->m_nCampIdx = int(monPK.PkID);
						}
						break;
					}
				}
			}
			break;
		case ACTION_CAREER_CHG:
			{
				const CSACTIONCAREERCHG &careerChg = Msg.Data.CareerChg;
				GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pActor)
				{
					if (pActor == ActorManager::getSingleton().getMainPlayer())
					{
						pActor->CollectSkill_SlotMax = careerChg.CollectSkillInfo.CollectSkill_SlotMax;
						pActor->CollectSkill_SlotCurr = careerChg.CollectSkillInfo.CollectSkill_SlotCurr;
						if (pActor->CollectSkill_AddType != careerChg.CollectSkillInfo.CollectSkill_AddType)
						{
							pActor->CollectSkill_AddType = careerChg.CollectSkillInfo.CollectSkill_AddType;
						}
						GameManager::getSingleton().SendUIEvent("GE_CHANGE_COLLECTTYPE");
					}
					if (pActor->m_Race != careerChg.Career)
					{
						pActor->m_Race = careerChg.Career;//职业改变
						Ogre::AvatarEntity *pAvatar = pActor->getAvatar();
						if ( pAvatar != NULL )
						{
							pAvatar->setAvatarAttrib(0, pActor->m_Race);
							pAvatar->updateAttrib();
						}
						
						tagUIMsgBody uiMsg;
						uiMsg.Who.type = pActor->getType();
						uiMsg.Who.id = pActor->GetID();
						GameUIMsg::getSingleton().setUIMsg(uiMsg);
						GameManager::getSingleton().SendUIEvent("GE_CHANGE_RACE");
					}
				}
			}
			break;
		case ACTION_BLOCKMSK_POINT:
			{
				const CSACTIONBLOCKMSKPOINT &blockMskPos = Msg.Data.BlockMskPoint;
				MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if (pMainPlayer)
				{
					pMainPlayer->CmdMoveTo(blockMskPos.Point.X, blockMskPos.Point.Y);
				}
			}
			break;
		case ACTION_MON_BOOTY_FIRST:
			{
				GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pActor != NULL)
				{
					pActor->m_BootyInfo = Msg.Data.MonBootyFirst;
					tagUIMsgBody uiMsg;
					uiMsg.Who.type = pActor->getType();
					uiMsg.Who.id = Msg.ID;
					GameUIMsg::getSingleton().setUIMsg(uiMsg);
					GameManager::getSingleton().SendUIEvent("GE_CHANGE_MON_BOOTY");
				}
			}
			break;
		case ACTION_LOGIC_AREA:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor(Msg.ID);	
				if( pActor != NULL )
				{
					pActor->m_nSafeOverlayFlag = Msg.Data.LogicArea.Type;
					if( pActor->getType() == GAT_MAINPLAYER )
					{
						char szScripts[256];
						sprintf( szScripts,"ChangeSafeArea(%d)",Msg.Data.LogicArea.Type );
						ActorManager::getSingleton().getScriptVM()->callString( szScripts );
					}
					if( pActor->getType() == GAT_MAINPLAYER )
					{
						GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_BUF_LIST_UPDATE");
					}
				}
			}
			break;
		case ACTION_TEST:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( pActor )
				{
					if( Msg.Data.RoleTest.Flag == 0 )
					{
					}
					if( Msg.Data.RoleTest.Flag == 1 )
					{
						if( pActor->getType() == GAT_MAINPLAYER )
						{
							SkillManager::getSingleton().onStop();
							pActor->getShowActor()->getCurEntity()->stopMotion(9596);
							GameActor* pTargetActor = ActorManager::getSingleton().findActorByName( pMain->m_szPkTargetName );
							if( pTargetActor )
							{
								pTargetActor->getShowActor()->getCurEntity()->stopMotion(9596);
							}
							strcpy(pMain->m_szPkTargetName,"");
						}
						pActor->m_pkEffect.setVictory( Msg.Data.RoleTest.Victory );
						pActor->m_pkEffect.setState( EFFECT_STATE_END );
					}
					if( pActor->getType() == GAT_MAINPLAYER )
					{
						GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_BUF_LIST_UPDATE");
					}
				}
			}
			break;
		case ACTION_BEGIN_LEAVE_MAP:/* 即将离开当前地图，和ENTER_MAP配对 */
			{
				GamePlayManager::getSingleton().leveMapSoon();
			}
			break;
		case ACTION_DROP_ITEM_UPDATE:
			{				
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL && pActor->getType() == GAT_ITEM )
				{
					pActor->setRoleWId( Msg.Data.DropItemUpdate.OwnerRoleID );
					pActor->setRollType( Msg.Data.DropItemUpdate.Roll );
					ActorManager::getSingleton().setItemChangeTime(Msg.Data.DropItemUpdate.PickLimitTime,Msg.ID);
					char szBuf[256];
					sprintf(szBuf,"OnItemChangeOwner(%d)",Msg.ID);
					ActorManager::getSingleton().getScriptVM()->callString(szBuf); 
				}
			}
			break;
		case ACTION_OFFLINE_EXP:
			{
				if( ActorManager::getSingleton().getMainPlayer() == NULL )
					break;
				const CSOFFLINEEXPINFO& info = Msg.Data.OfflineExpInfo;
				if( info.Type == CSOFFLINE_EXP_REMAIN )
				{
					ActorManager::getSingleton().getMainPlayer()->SetOfflineExp(info.Exp);
					ActorManager::getSingleton().getMainPlayer()->setMaxOfflineExp(info.MaxExp);
					GameManager::getSingleton().SendUIEvent("GE_ONEOFFLINE_UPDATE");
				}if( info.Type == CSOFFLINE_EXP_ONE_OFF ){				
 					if ( info.Exp >0 )
 					{
 						if ( ActorManager::getSingleton().getMainPlayer()->getOneOfflineExp() <= 0 )
 						{
 							GameManager::getSingleton().SendUIEvent("GE_OFFLINEEXP_GET");
 						}
 						else
 						{
 							GameManager::getSingleton().SendUIEvent("GE_GETONEOFFLINE_LOST");
 						}
 					}
 					else{
						if ( info.Exp ==0 && ActorManager::getSingleton().getMainPlayer()->getOneOfflineExp() > 0 )
						{
							GameManager::getSingleton().SendUIEvent("GE_GETONEOFFLINE_SUCCESS");
						}
					}
 					ActorManager::getSingleton().getMainPlayer()->SetOneOfflineExp(info.Exp);
				}else if (info.Type == CSOFFLINE_EXP_PWORLD ){
					ActorManager::getSingleton().getMainPlayer()->setPworldExp(info.Exp);
					ActorManager::getSingleton().getMainPlayer()->setMaxPworldExp(info.MaxExp);
					GameManager::getSingleton().SendUIEvent("GE_ONEOFFLINE_UPDATE");
				}
			}
			break;
		case ACTION_PREPARE_ADD_BY_HIT:
			{
				ISpellControler		*sc = SkillManager::getSingleton().getSpellControler(Msg.ID);
				if (sc != NULL)
				{
					GameSpell *pSpell = sc->getCurSpell();
					if (pSpell && pSpell->getSpellId() == Msg.Data.PrepareAddbyHit.SkillID)
					{
						int s, u;
						ServerTimer::getSingleton().getServerTime(s, u);
						int now = s * 1000 + (u + 500) / 1000;
						int cur = Msg.Data.PrepareAddbyHit.SvrTime.BenchTimeSec * 1000 +
							(Msg.Data.PrepareAddbyHit.SvrTime.BenchTimeUSec + 500) / 1000;
						pSpell->HitBackPrepareTimeTo(Msg.Data.PrepareAddbyHit.PrepareTimeUse + (now - cur));
					}
				}
			}
			break;
		case ACTION_LEAVE_STAT:
			{
				GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if (pActor)
				{
					if (Msg.Data.LeaveState.LeaveState == LEAVE_STAT_NONE)
					{
						if (pActor->isAFK())
						{
							pActor->onAFK(false);
						}
					}
					else
					{
						if (!pActor->isAFK())
						{
							pActor->onAFK(true);
						}
					}
				}
			}
			break;
		case ACTION_MOTION:
			{
				GameActor *pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if( pActor != NULL && pActor->getType() == GAT_PLAYER )
				{
					char szbuf[256];
					sprintf(szbuf,"ChatActionPlayAction(%d,%d,%d)",Msg.Data.Motion.MotionID,pActor->GetID(),Msg.Data.Motion.TargetID);
					ActorManager::getSingleton().getScriptVM()->callString(szbuf);
				}
			}
			break;
		case ACTION_TARGET:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if( pActor != NULL )
				{
					pActor->setHitTargetID( Msg.Data.Target.TargetID );
				}
			}
			break;
		case ACTION_CLAN_ITEM:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor(Msg.ID);
				if ( pActor != NULL )
				{
					pActor->m_Equip.setTmpArm(Msg.Data.ClanItem.ClanItemID);
				}
			}
			break;
		case ACTION_MACHINE_ITEM_LOCK:
			{
				MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if (pMainPlayer->GetID() == Msg.ID)
				{
					char script[256];
					Item *item = NULL;
					if (Msg.Data.MachineItemLock.LockItemWid != 0)
					{
						item = pMainPlayer->getContainer().getItem(Msg.Data.MachineItemLock.LockItemWid);
					}
					if (item != NULL)
					{
						sprintf(script,"SingleLockItem{list=%d, grid=%d, id=%d}", item->getListType(), item->getGridIdx(), item->getItemId());
					}
					else
					{
						sprintf(script,"SingleLockItem()");
					}
					ActorManager::getSingleton().getScriptVM()->callString(script);
				}
			}
			break;
		case ACTION_MACHINE_LEAVE_PRE:
			{
				GameManager::getSingleton().SendUIEvent("GE_MONSTER_MACHINE_LEAVE_PREPARE");
			}
			break;
		case ACTION_MACHINE_LEAVE_PRE_BRK:
			{
				ActorManager::getSingleton().getScriptVM()->callString("util.BreakBlockPrepare('DropMonsterMachine')");
			}
			break;
		case ACTION_CLIENT_CMD:
			{
				switch (Msg.Data.ClientCmd.CmdType)
				{
				case CLIENT_CMD_AUTO_MOVE:
					{
						const tagCSAutoMoveData &data = Msg.Data.ClientCmd.CmdData.AutoMove;
						GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
						MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
						ActorManager::getSingleton().getMainPlayer()->CmdMoveStop();
						if (data.Stop)
						{
							mutilMapFinder.StopAutoMove();
						}
						else
						{
							mutilMapFinder.SetAutoMove(data.MapID, data.X, data.Y);
						}
					}
					break;
				case CLIENT_CMD_SNAPSHOOT:
					{
						GameManager::getSingleton().ScreenSnap();
					}
					break;
				}
			}
			break;
		case ACTION_BATTLE_LIMIT_TIME:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( pMain != NULL && pMain->GetID() == Msg.ID )
				{
					pMain->setBattleLimitData( Msg.Data.BattleLimitTime.BeginTime,
						Msg.Data.BattleLimitTime.MaxNum,
						Msg.Data.BattleLimitTime.Num );
				}			
			}
			break;
		case ACTION_MAP_POS:
			{
				if( Msg.ID == ActorManager::getSingleton().getMainPlayer()->GetID() )
				{
					break;
				}
				char buf[256];
				sprintf( buf,"SetBuffMapPos( %d,%d,[===[%s]===],%d,%d )",Msg.Data.MapPos.ShowType,
					Msg.ID,Msg.Data.MapPos.Name,Msg.Data.MapPos.Pos.X,Msg.Data.MapPos.Pos.Y);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ACTION_ARM_FENJIE_PRE:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor && pActor->getType() != GAT_MAINPLAYER )
				{
					if( Msg.Data.FenJiePre.PreFlag == 0 )
					{
						pActor->onBreakChant( eResolve_Chant );
					}else{
						const CSACTIONFENJIEPRE& pre = Msg.Data.FenJiePre;
						pActor->startResolveChant( pre.ArmData.ArmIdxData.ArmListType,pre.ArmData.ArmIdxData.ArmGridIdx,pre.ArmData.WId,pre.Type );
					}
				}
			}
			break;
		case ACTION_ARM_FENJIE:
			{
				if ( Msg.Data.FenJie.Type != 2 )
				{
					GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
					if( pActor )
					{
						pActor->finishChant( eResolve_Chant );
						if( pActor == ActorManager::getSingleton().getMainPlayer() )
						{
							char szBuf[256];
							sprintf(szBuf,"OnFinishFenjie(%d)",Msg.Data.FenJie.Succ);
							GameManager::getSingleton().getScriptVM()->callString( szBuf );
						}
					}
				}
				else
				{
					char szBuf[256];
					sprintf( szBuf,"SetEpurateResult( %d, %d )", Msg.Data.FenJie.Succ, Msg.Data.FenJie.GridIdx );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
				}
			}
			break;
		case ACTION_CLAN_CITY_UPDATE:
			{
				GameClanManager::getSingleton().SetClanCityInfo(Msg.Data.ClanCityUpdate.ClanID, Msg.Data.ClanCityUpdate.ClanCity);
				GameClanManager::getSingleton().checkClanBuildChange();
			}
			break;
		case ACTION_DAILY_GOAL_INFO:
			{
				GamePromotion::getSingleton().SetDailyInfo( Msg.Data.DailyGoalInfo );
			}
			break;
		case ACTION_DYN_ACTIVE_INFO:
			{
				GamePromotion::getSingleton().SetDynActiveInfo( Msg.Data.DynActiveInfo );
			}
			break;
		case ACTION_ARM_RECAST:
			{
				static_assert( sizeof(tagRecastInfo) == sizeof(tagRecastInfo_ToLua) );
				if( Msg.Data.Recast.Succ == 1 )
				{
					tagUIMsgBody ui_msg;
					memcpy( &ui_msg.RecastInfo,&Msg.Data.Recast.RecastInfo,sizeof(tagRecastInfo) );
					GameUIMsg::getSingleton().setUIMsg( ui_msg );
					GameManager::getSingleton().SendUIEvent("GE_ARM_RECAST");
				}				
			}
			break;
		case ACTION_RESET_RUNE_EX_RES:
			{
				tagUIMsgBody uiMsg;
				uiMsg.RuneResetData.RuneType	= Msg.Data.ResetRuneExRes.RuneType;
				uiMsg.RuneResetData.RuneExID	= Msg.Data.ResetRuneExRes.RuneExID;
				GameUIMsg::getSingleton().setUIMsg(uiMsg);
				GameManager::getSingleton().SendUIEvent("GE_SKILL_RUNE_EX_INFO_RESET");
			}
			break;
		case ACTION_RUNE_EX_ENABLE_INFO:
			{
				const tagCSActionRuneExEnableInfo &info = Msg.Data.RuneExEnableInfo;
				for (size_t i = 0; i < MAX_RUNE_EX; ++i)
				{
					GameRuneManager::getSingleton().SetExRuneInfo(info.RuneExEnableInfo[i].RuneType,
						info.RuneExEnableInfo[i].RuneExID, info.RuneExEnableInfo[i].RuneNum, RUNE_EX_ENABLE_NUM);
				}
			}
			break;
		case ACTION_DO_RUNE_EX:
			{
				char buffer[100];
				sprintf(buffer, "SkillRuneDoEx(%d)", Msg.Data.DoRuneEx.RuneExID);
				ActorManager::getSingleton().getScriptVM()->callString(buffer);
			}
			break;
		case ACTION_ARM_STAR:
			{
				char buffer[256];
				sprintf( buffer, "ArmProceStarResult( %d, %d, %d )", Msg.Data.ArmStar.Succ, Msg.Data.ArmStar.Star, Msg.Data.ArmStar.MulIdx );
				ActorManager::getSingleton().getScriptVM()->callString(buffer);
				GameManager::getSingleton().SendUIEvent("GE_ARM_STAR_UP_RESULT");
			}			
			break;
		case ACTION_ACC_DE_QUICK:
			{
				char buf[256];
				if( Msg.Data.AccDeQuick.Result == 0 )
				{
					MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
					if( pMain && pMain->GetID() == Msg.ID )
					{
						pMain->setGMPermFlag( pMain->getGMPermFlag() ^ UIN_FLAG_QUICK );
					}
				}
				sprintf( buf,"OnQuickRegisterResult(%d)",Msg.Data.AccDeQuick.Result );
				ActorManager::getSingleton().getScriptVM()->callString( buf );				
			}
			break;
		case ACTION_ACHIEVEMENT_NEW:
			{
				char buf[256];
				sprintf( buf, "AddNewAchievementCategoryID(%d)", Msg.Data.AchievementNew.AchievementID );
				ActorManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		case ACTION_ACHIEVEMENT_LIST:
			{
				ActorManager::getSingleton().getScriptVM()->callString("ClearAchievementCategoryList()");
				for ( int i = 0; i < Msg.Data.AchievementList.Num; i++ )
				{
					char buf[256];
					sprintf( buf, "LoadAchievementCategoryID(%d)", Msg.Data.AchievementList.AchievementID[i] );
					ActorManager::getSingleton().getScriptVM()->callString( buf );
				}
			}
			break;
		case ACTION_ARM_WASH:
			{
				if( Msg.Data.ArmWash.Succ )
				{
					ActorManager::getSingleton().getScriptVM()->callFunction( "OnArmWashSucc","u[tagCSActionArmWash_ToLua]",&Msg.Data.ArmWash );
				}
			}
			break;
		case ACTION_VIEW_FLAG_CHG:
			{
				GameActor* player = ActorManager::getSingleton().FindActor( Msg.ID );
				if( player )
				{
					player->setViewFlg( Msg.Data.ViewFlagChg.ViewFlag );
				}
			}
			break;
		case ACTION_YESTERDAY_INFO:
			{
				if( ActorManager::getSingleton().getMainPlayer() )
				{
					ActorManager::getSingleton().getMainPlayer()->setOldDayPworldScore( Msg.Data.YesterdayInfo.ClanEveryDayPworldScore );
				}
			}
			break;
		case ACTION_COUNT_DOWN:
			{
				char szBuf[256];
				sprintf( szBuf, "SetCountDownInfo( %d,[===[%s]===] )", Msg.Data.CountDown.Sec, Msg.Data.CountDown.Title );
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case ACTION_DAILY_BULL:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( pMain && pMain->GetID() == Msg.ID )
				{
					pMain->setDailyUsedBull( Msg.Data.DailyBull.Bull );
				}
			}
			break;
		case ACTION_RUNNE_SKILL_CHG_RET:
			{
				char szBuf[256];
				sprintf( szBuf, "ShowRuneSkillChgUV( %d,%d )", Msg.Data.RunneSkillChgRet.SkillID, Msg.Data.RunneSkillChgRet.RuneType );
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case ACTION_CLAN_MACHINE_PWORLD:
			{
				char szBuf[256];
				sprintf( szBuf, "UpdatePworldWaveInfo( %d,%d,%d,%d )", Msg.Data.ClanMachinePworld.WaveNum, Msg.Data.ClanMachinePworld.WaveTimer, 
						Msg.Data.ClanMachinePworld.RaiderTimer, Msg.Data.ClanMachinePworld.PworldDefId );
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case ACTION_SHAM_DEAD:
			{
				;
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if( Msg.Data.ShamDead.Dead == 1 )
					{
						pActor->ChangeActState( GSACT_SHAMDEAD );
					}else if( pActor->getActState().GetState() == GSACT_STAND )
					{
						pActor->ChangeActState( GSACT_STAND );
					}
				}
			}
			break;
		case ACTION_COLOR_FONT_LIMIT:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( pMain && pMain->GetID() == Msg.ID )
				{
					pMain->setColorFontEndTime( Msg.Data.ColorFontLimit.ColorFontEnd );
				}
			}
			break;
		case ACTION_COW_FINAL_RANGE:
			{
				static_assert( sizeof(tagCSActionCowFinalRange) == sizeof(tagCSActionCowFinalRange_ToLua) );
				tagUIMsgBody uiMsg;
				memcpy( &uiMsg.CowFinalRangeData, &Msg.Data.CowFinalRange, sizeof(tagCSActionCowFinalRange));
				GameUIMsg::getSingleton().setUIMsg(uiMsg);
				GameManager::getSingleton().SendUIEvent("GE_COW_FINAL_RANGE_INFO");
			}
			break;
		case ACTION_CLAN_DONATE_LOG:
			{
				//GameClanManager::getSingleton().updateClanDonateInfo( Msg.Data.ClanDonateLog.ClanDonateLogEntry );
			}
			break;
		case ACTION_CLAN_SKILL_INFO:
			{
				GameClanManager::getSingleton().updateClanSkillInfo( Msg.Data.ClanSkillInfo.ClanSkillInfo );
			}
			break;
		case ACTION_PLAYER_CLAN_SKILL_INFO:
			{
				GameClanManager::getSingleton().updatePlayerClanSkillInfo( Msg.Data.PlayerClanSkillInfo.ClanSkillInfo );
			}
			break;
		case ACTION_LEVEL_FENG:
			{
				GameManager::getSingleton().SetLevelFeng( Msg.Data.LevelFeng.LevelFeng.LevelFeng );
				char szBuf[128];
				sprintf( szBuf, "OnGetLevelFeng( %d,%d,%d )",Msg.Data.LevelFeng.LevelFeng.LevelFeng, Msg.Data.LevelFeng.LevelFeng.LevelFengEnd, Msg.Data.LevelFeng.LevelFengChg);
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		case ACTION_ARM_LEVELUP:
			{
				MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
				if( pMain != NULL && pMain->GetID() == Msg.ID )
				{
					Item* item = pMain->getContainer().getItem( Msg.Data.ArmLevelUP.NextWID );
					if( item != NULL && item->getItemId() > 0 )
					{
						GameManager::getSingleton().getScriptVM()->callFunction( "OnArmLevelSuccess","u[Item]",item );
					}
				}
			}
			break;
		case ACTION_WORSHIP_EFFIGY:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor == NULL || WorldCityManager::getSingleton().GetCityKeepClanWID() == 0 )
				{
					break;
				}
				char buf[256];
				if( Msg.Data.WorshipEggify.Flower == 1 )
				{
					sprintf( buf, "[%s]双手合十，虔诚的向[%s]的城主雕像表达了自己的敬意", pActor->getName(), WorldCityManager::getSingleton().getOwnerName() );
				}else{
					sprintf( buf, "[%s]愤怒的向[%s]的城主雕像扔了一枚发臭的鸡蛋。", pActor->getName(), WorldCityManager::getSingleton().getOwnerName() );
				}
				ChatManager::getSingleton().addMakeSucessMsg(0,buf);
				
			}
			break;
		case ACTION_BEACH_INFO:
			{
				GameBusyManager::getSingleton().SetBeachTime( Msg.Data.BeachInfo.Sec );
				/*char szBuf[256];				
				sprintf(szBuf,"BusyPromotionBeachActive()");
				ActorManager::getSingleton().getScriptVM()->callString(szBuf);*/
				GameManager::getSingleton().SendUIEvent("GE_BEACH_TIME_UPDATE");
			}
			break;
		case ACTION_BEACH_BUFF:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor->getType() == GAT_MAINPLAYER )
				{	
					char szBuf[256];				
					sprintf(szBuf,"SetOnBeachBuff(%d,%d)", Msg.Data.BeachBuff.In, 3600 - Msg.Data.BeachBuff.BuffSec );
					ActorManager::getSingleton().getScriptVM()->callString(szBuf);
				}	
			}
			break;
		case ACTION_DAOBA_INFO:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if ( pActor == NULL )
				{
					break;
				}
				pActor->setDaoBaClanWid(Msg.Data.DaoBa.ClanWid);
				pActor->setDaoBaEnd(Msg.Data.DaoBa.End);
				pActor->setDaoBaName(Msg.Data.DaoBa.Name);
				ActorManager::getSingleton().updateAllPlayerTitleInfo();
			}
			break;
		case ACTION_VIP_INFO:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->setVipFlag( Msg.Data.VipInfo.VipFlag );
				}
			}
			break;
		case ACTION_XUKONG_ENERGY:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					pActor->setXuKongFlag( Msg.Data.XuKongEnergy.EnergyStat );
					pActor->setXuKongEffect();
				}
			}
			break;
		case ACTION_HANG_HAI_RANGE:
			{
				static_assert( sizeof(tagCSHangHaiRange_ToLua) == sizeof(tagCSHangHaiRange) );
				tagUIMsgBody uiMsg;
				memcpy( &uiMsg.HangHaiRangData, &Msg.Data.HangHaiRange, sizeof(tagCSHangHaiRange));
				GameUIMsg::getSingleton().setUIMsg(uiMsg);
				GameManager::getSingleton().SendUIEvent("GE_HANG_HAI_RANGE_INFO");
			}
			break;
		case ACTION_CLAN_WAR_AWARD_INFO:
			{
				if ( Msg.Data.ClanWarAwardInfo.AwardInfo.Time != 0 )
				{
					ActorManager::getSingleton().getScriptVM()->callString("SetClanWarAwardGift()");
				}
			}
			break;
		case ACTION_MAP_SHOW:
			{
				char szBuf[256];	
				if ( Msg.Data.MapShow.OpType == MAP_SHOW_OP_ENTER )
				{
					GameBattle::getSingleton().setClanWarCampMaxHp( Msg.Data.MapShow.Data.MapShowMonCamp.CampID-1, getMonsterMaxHP(Msg.Data.MapShow.Data.MapShowMonCamp.MonsterID) );
					GameBattle::getSingleton().setClanWarCampHp( Msg.Data.MapShow.Data.MapShowMonCamp.CampID-1, Msg.Data.MapShow.Data.MapShowMonCamp.HP );
					sprintf(szBuf,"SetClanWarCampMonMapInfo(%d,%d,%d,%d,%d,%d)", Msg.Data.MapShow.Data.MapShowMonCamp.MonsterID, Msg.Data.MapShow.Data.MapShowMonCamp.HP,
						Msg.Data.MapShow.MapID, Msg.Data.MapShow.Pos.X, Msg.Data.MapShow.Pos.Y, Msg.Data.MapShow.Data.MapShowMonCamp.CampID);
				}
				else if ( Msg.Data.MapShow.OpType == MAP_SHOW_OP_UPDATE )
				{
					GameBattle::getSingleton().setClanWarCampMaxHp( Msg.Data.MapShow.Data.MapShowMonCamp.CampID-1, getMonsterMaxHP(Msg.Data.MapShow.Data.MapShowMonCamp.MonsterID) );
					GameBattle::getSingleton().setClanWarCampHp( Msg.Data.MapShow.Data.MapShowMonCamp.CampID-1, Msg.Data.MapShow.Data.MapShowMonCamp.HP );
					sprintf(szBuf,"SetClanWarCampMonMapInfo(%d,%d,%d,%d,%d,%d)", Msg.Data.MapShow.Data.MapShowMonCamp.MonsterID, Msg.Data.MapShow.Data.MapShowMonCamp.HP,
						Msg.Data.MapShow.MapID, Msg.Data.MapShow.Pos.X, Msg.Data.MapShow.Pos.Y, Msg.Data.MapShow.Data.MapShowMonCamp.CampID);
				}
				else if ( Msg.Data.MapShow.OpType == MAP_SHOW_OP_LEAVE )
				{
					for ( int i = 0; i < MAX_CAMP_NUM; i++ )
					{
						GameBattle::getSingleton().setClanWarCampMaxHp( i, 0 );
						GameBattle::getSingleton().setClanWarCampHp( i, 0 );
					}
					strcpy( szBuf, "ClearClanWarCampMonMapInfo()" );
				}
				ActorManager::getSingleton().getScriptVM()->callString(szBuf);
			}
			break;
		case ACTION_CLAN_BATTLE_APPLY_INFO:
			{
 				GameBattle::getSingleton().setClanBattleApplyInfo( Msg.Data.ClanBattleApplyInfo.BattleID, Msg.Data.ClanBattleApplyInfo.OpType );
			}
			break;
		case ACTION_PRECREATE_GIFT_VERIFY:
			{
				if ( Msg.Data.PrecreateGiftVerify.OpType == ACTION_PRECREATE_GIFT_VERIFY_OP_OPEN )
				{
					char buf[256];
					sprintf( buf, "UpdatePrebuildMobileCheckFrame( %d )", 0 );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				else if ( Msg.Data.PrecreateGiftVerify.OpType == ACTION_PRECREATE_GIFT_VERIFY_OP_SN )
				{
					char buf[256];
					sprintf( buf, "UpdatePrebuildMobileCheckFrame( %d )", Msg.Data.PrecreateGiftVerify.Data.NewSN );
					GameManager::getSingleton().getScriptVM()->callString( buf );					
				}
				else if ( Msg.Data.PrecreateGiftVerify.OpType == ACTION_PRECREATE_GIFT_VERIFY_OP_RESULT )
				{
					char buf[256];
					sprintf( buf, "UpdateMobileCheckResult( %d )", Msg.Data.PrecreateGiftVerify.Data.Result );
					GameManager::getSingleton().getScriptVM()->callString( buf );
				}
				
			}
			break;
		case ACTION_WEDDING_DSN:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
				if( pActor != NULL )
				{
					if ( pActor->getType() != GAT_MAINPLAYER )
					{
						if( Msg.Data.WeddingDsn.ShowFlag == 1 )
						{
							pActor->m_nShowFlag |= SHOW_WEDDING_DSN_FLAG;
						}else
						{
							pActor->m_nShowFlag ^= SHOW_WEDDING_DSN_FLAG;
						}
						pActor->setStallName(Msg.Data.WeddingDsn.MiscName);
					}
					else{
						MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
						strcpy(pMain->m_szLoverName,Msg.Data.WeddingDsn.MiscName);
					}
					RoleText& actorTitleMgr = pActor->getRoleText();
					actorTitleMgr.BuildTitle();
				}
			}
			break;
		case ACTION_SCULPT_CHG:
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );

				if( pActor != NULL )
				{
					pActor->m_Head		= Msg.Data.SculptChg.Head;
					pActor->m_Hair		= Msg.Data.SculptChg.Hair;
					pActor->m_Face		= Msg.Data.SculptChg.Face;

					pActor->getAvatar()->setAvatarAttrib(POS_ID_HAIR, Msg.Data.SculptChg.Hair + 1);
					pActor->getAvatar()->setAvatarAttrib(POS_ID_HEAD, Msg.Data.SculptChg.Head);
					pActor->getAvatar()->setAvatarAttrib(POS_ID_FACE, Msg.Data.SculptChg.Face);
					pActor->getAvatar()->updateAttrib();
				}
			}
			break;
		}
	}else if( SKILL_REVIVE_INFO == msgid ){
		tagUIMsgBody uiMsg;
		uiMsg.ReviveInfo.m_nPlayerId	= msgbody.SkillReviveInfo.PlayerID;
		uiMsg.ReviveInfo.m_nSkillId		= msgbody.SkillReviveInfo.SkillID;
		uiMsg.ReviveInfo.DropExp		= msgbody.SkillReviveInfo.DropExp;
		uiMsg.ReviveInfo.DropMoney		= msgbody.SkillReviveInfo.DropMoney;
		uiMsg.ReviveInfo.ReviveExp		= msgbody.SkillReviveInfo.ReviveExp;
		uiMsg.ReviveInfo.ReviveMoney	= msgbody.SkillReviveInfo.ReviveMoney;
		strncpy( uiMsg.ReviveInfo.m_szRoleName, msgbody.SkillReviveInfo.RoleName, sizeof(msgbody.SkillReviveInfo.RoleName) );
		GameUIMsg::getSingleton().setUIMsg( uiMsg );
		GameManager::getSingleton().SendUIEvent( "GE_SKILL_REVIVE" );
	}else if( RIDE_LIST_CHG == msgid ){
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( pMainPlayer != NULL )
		{
			tagRideInfo RideGrids[MAX_RIDE_GRID]; 
			for( int i = 0; i < msgbody.RideListChg.RideList.Num; i++ )
			{
				RideGrids[i].RideID		= msgbody.RideListChg.RideList.RideGrids[i].ItemID;
				RideGrids[i].Endtime	= msgbody.RideListChg.RideList.RideGrids[i].Endtime;
				RideGrids[i].nLevel		= msgbody.RideListChg.RideList.RideGrids[i].Level;
			}
			pMainPlayer->m_Rider.setRideInfo( msgbody.RideListChg.RideList.Num, msgbody.RideListChg.RideList.MaxNum, RideGrids );
			GameManager::getSingleton().SendUIEvent( "GE_RIDELIST_CHG" );
		}

		ActorManager::getSingleton().getScriptVM()->callString( "UpdateRideFrame()" );
	}else if( msgid == INVITE_TEST_SVR )
	{
		MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
		switch( msgbody.InviteSvr.Cmd )
		{
		case TEST_SVR_INFO:
			{
				char szBuf[256];				
				sprintf(szBuf,"InviteSafePK(\"%s\")",msgbody.InviteSvr.InviteTestSvr.TestInvite.RoleName);
				ActorManager::getSingleton().getScriptVM()->callString(szBuf);
			}
			break;

		case TEST_SVR_RES:
			{
				if( msgbody.InviteSvr.InviteTestSvr.TestSvrRes.TestRes.Flag == 0 )
				{
					pMain->m_pkEffect.setPos( msgbody.InviteSvr.InviteTestSvr.TestSvrRes.x*10,msgbody.InviteSvr.InviteTestSvr.TestSvrRes.y*10 );
					pMain->m_pkEffect.setState( EFFECT_STATE_START );
					pMain->getShowActor()->getCurEntity()->playMotion("9596_16",false,9596);
					GameActor* pActor = ActorManager::getSingleton().findActorByName(msgbody.InviteSvr.InviteTestSvr.TestSvrRes.TestRes.RoleName);
					if( pActor != NULL )
					{
						pActor->getShowActor()->getCurEntity()->playMotion("9596_16",false,9596);
					}
					strcpy(pMain->m_szPkTargetName,msgbody.InviteSvr.InviteTestSvr.TestSvrRes.TestRes.RoleName);
					GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_START_PK");	
				}else{
					strcpy(pMain->m_szPkTargetName,"");
				}				
				GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_BUF_LIST_UPDATE");				
			}
			break;
		case TEST_SVR_CANCEL:
			{
				char szBuf[256];
				sprintf(szBuf,"CancelInvitePK()");
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		}
	}else if( msgid == DIE_INFO_SVR )
	{
		static_assert( sizeof(tagDieSvr_ToLua) == sizeof(tagDieSvr) );
		tagUIMsgBody UI_Msg;
		memcpy( &UI_Msg.DropData,&msgbody.DieSvr,sizeof(tagDieSvr_ToLua) );
		GameUIMsg::getSingleton().setUIMsg( UI_Msg );
		GameManager::getSingleton().SendUIEvent( "GE_MESSAGE_DEAD" );
	}else if( msgid == MACHINE_SVR )
	{
		switch( msgbody.MachineSvr.SvrOp )
		{
		case MACHINE_SVR_INVITE_INFO:
			{
				char buf[256];
				sprintf( buf,"ReciveMachineInvite( [===[%s]===],%d )",msgbody.MachineSvr.Data.Invite.RoleName,msgbody.MachineSvr.Data.Invite.MachineInviteID );
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		}
	}else if( msgid == ROLE_CHG_MEMID )
	{
		ActorManager::getSingleton().changeMainPlayerID( msgbody.RoleChgMem.MemID );
	}
	return 0;
}


const int FAIL_CATCH_PET	= 0;
const int SUCCESS_CATCH_PET	= 1;

void GameActionManager::OnActionSuccessCatchPet( const CSACTIONRES& Msg )
{
	ActorManager&	actorMgr	= ActorManager::getSingleton();
	GameActor*		pActor		= actorMgr.FindActor( Msg.ID );
	if ( pActor == NULL )
	{
		return;
	}
	
	const CSACTIONCATCHPET&	catchPetActionData = Msg.Data.CatchPet;

	if ( catchPetActionData.TargetID == 0 )
	{
		return;
	}

	GameActor* pPetActor = actorMgr.FindActor( catchPetActionData.TargetID );
	if ( pPetActor == NULL )
	{
		return;
	}
	pPetActor->playMotion( "10254_5_1", true );
}

void GameActionManager::OnActionFailCatchPet( const CSACTIONRES& Msg )
{
	ActorManager&	actorMgr	= ActorManager::getSingleton();
	GameActor*		pActor		= actorMgr.FindActor( Msg.ID );
	if ( pActor == NULL )
	{
		return;
	}

	const CSACTIONCATCHPET&	catchPetActionData = Msg.Data.CatchPet;

	if ( catchPetActionData.TargetID == 0 )
	{
		return;
	}

	GameActor* pPetActor = actorMgr.FindActor( catchPetActionData.TargetID );
	if ( pPetActor == NULL )
	{
		return;
	}
	
	pPetActor->playMotion( "10254_5_2", true );
}

// 捕获宠物是否成功
void GameActionManager::OnActionCatchPet( const CSACTIONRES& Msg )
{
 	const CSACTIONCATCHPET&	catchPetActionData = Msg.Data.CatchPet;
 	if ( catchPetActionData.Succ == FAIL_CATCH_PET )
 	{
 		OnActionFailCatchPet( Msg );
 	}
 	else if ( catchPetActionData.Succ == SUCCESS_CATCH_PET )
 	{
 		OnActionSuccessCatchPet( Msg );
 	}
 }

void GameActionManager::OnActionTeam( const CSACTIONRES& Msg )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
	if ( pActor == NULL )
	{
		return;
	}

	MainGamePlayer* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}
	
	const CSTEAMACTION& teamActionData	= Msg.Data.Team;
	// 若此人进入主角所在队伍
	if( teamActionData.Type == CLIENT_JOIN_TEAM )
	{
		OnActionJoinTeam( Msg );
	}
	else // 若此人退出主角所在队伍
	{
		OnActionLeaveTeam( Msg );
	}

	if ( pActor->GetID() == pMainPlayer->GetSelectTarget() )
	{
		GameManager::getSingleton().SendUIEvent( "UI_TARGET_ATTRIBUTE_CHG" );
	}

	RoleText& actorTitleMgr = pActor->getRoleText();
	actorTitleMgr.BuildTitle();
}

void GameActionManager::OnActionOtherPlayerLeaveTeam( const CSACTIONRES& Msg )
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	if ( actorMgr.getShowOtherPlayerModelOption() == HIDE_MODEL_NONE )
	{
		return;
	}

	GameActor* pOneActor = ActorManager::getSingleton().FindActor( Msg.ID );
	if ( pOneActor == NULL )
	{
		return;
	}

	GameMap* pMapMgr = pOneActor->getCurMap();
	if ( pMapMgr == NULL )
	{
		return;
	}

	Ogre::ShowActor* pOneShowActor = pOneActor->getShowActor();
	if ( !pOneShowActor->isInMap() )
	{
		return;
	}

	pOneShowActor->onLeaveMap( pMapMgr->getShowGameMap() );

	Ogre::ShowActor* pOneShowRiderActor = pOneActor->m_Rider.m_pShowActor;
	if ( pOneShowRiderActor != NULL )
	{
		pOneShowRiderActor->onLeaveMap( pMapMgr->getShowGameMap() );
	}

	GamePet* pPetpackage = pOneActor->getPet();
	for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
	{
		GameActor* pOneFightPet = pPetpackage->getFightPet( i );
		if ( pOneFightPet == NULL )
		{
			continue;
		}

		Ogre::ShowActor* pOneShowPetActor = pOneFightPet->getShowActor();
		if ( !pOneShowPetActor->isInMap() )
		{
			continue;
		}

		pOneShowPetActor->onLeaveMap( pMapMgr->getShowGameMap() );
	}
}

void GameActionManager::OnActionMainPlayerLeaveTeam( const CSACTIONRES& Msg )
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	if ( actorMgr.getShowOtherPlayerModelOption() == HIDE_MODEL_NONE )
	{
		return;
	}
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	if ( pMainplayer == NULL )
	{
		return;
	}
	
	for( ACTOR_ITER iter = actorMgr.getActorBegin(), end = actorMgr.getActorEnd(); 
		iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( actorMgr.isActorIgnoreF12( pOneActor ) )
		{
			continue;
		}

		GameMap* pMapMgr = pOneActor->getCurMap();
		if ( pMapMgr == NULL )
		{
			continue;
		}


		Ogre::ShowActor* pOneShowActor = pOneActor->getShowActor();
		if ( !pOneShowActor->isInMap() )
		{
			continue;
		}

		pOneShowActor->onLeaveMap( pMapMgr->getShowGameMap() );
		Ogre::ShowActor* pOneShowRiderActor = pOneActor->m_Rider.m_pShowActor;
		if ( pOneShowRiderActor != NULL )
		{
			pOneShowRiderActor->onLeaveMap( pMapMgr->getShowGameMap() );
		}
	}
}

void GameActionManager::OnActionLeaveTeam( const CSACTIONRES& Msg )
{
	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor* pActor		= actorMgr.FindActor( Msg.ID );
	if ( pActor == NULL )
	{
		return;
	}

	GameTeamManager& teamMgr	= GameTeamManager::getSingleton();
	tdr_ulonglong uOldTeamID	= pActor->getTeamWID();
	pActor->setTeamWID( 0 );
	pActor->setTeamName( "" );
	pActor->m_ViewFlag &= ~VIEW_FLAG_CAPTAIN;
	pActor->setViewFlg( pActor->m_ViewFlag );
	pActor->setCaptain( false );
	pActor->m_nTeamNum	= 0;

	if ( pActor->getType() == GAT_MAINPLAYER )
	{
		teamMgr.leaveTeam();		
		AutoTeamManager::getSingleton().releaseAutoTeamInfo();
		OnActionMainPlayerLeaveTeam( Msg );
		GameManager::getSingleton().SendUIEvent( "GE_TEAM_LEAVETEAM" );
		return;
	}
	
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	tdr_ulonglong mainplayerTeamWID = pMainplayer->getTeamWID();
	if ( uOldTeamID ==  mainplayerTeamWID )
	{
		OnActionOtherPlayerLeaveTeam( Msg );
		GameManager::getSingleton().SendUIEvent( "GE_TEAM_ONE_PLAYER_LEAVE" );
	}
}

void GameActionManager::OnActionOtherPlayerJoinTeam( const CSACTIONRES& Msg )
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	if ( actorMgr.getShowOtherPlayerModelOption() == HIDE_MODEL_NONE )
	{
		return;
	}

	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	tdr_ulonglong mainplayerTeamWID = pMainplayer->getTeamWID();
	if ( mainplayerTeamWID == 0 )
	{
		return;
	}

	GameActor* pOneActor = ActorManager::getSingleton().FindActor( Msg.ID );
	if ( pOneActor == NULL )
	{
		return;
	}

	if ( pOneActor->isInStall() )
	{
		return;
	}

	if ( pOneActor->getTeamWID() != mainplayerTeamWID )
	{
		return;
	}

	GameMap* pMapMgr = pOneActor->getCurMap();
	if ( pMapMgr == NULL )
	{
		return;
	}
	
	Ogre::ShowActor* pOneShowActor = pOneActor->getShowActor();
	if ( pOneShowActor->isInMap() )
	{
		return;
	}

	pOneShowActor->update(60000);
	pOneShowActor->onEnterMap( pMapMgr->getShowGameMap() );
	Ogre::ShowActor* pOneShowRiderActor = pOneActor->m_Rider.m_pShowActor;
	if( pOneShowRiderActor != NULL )
	{
		pOneShowRiderActor->onEnterMap( pMapMgr->getShowGameMap() );
	}

	/*GamePet* pPetpackage = pOneActor->getPet();
	for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
	{
		GameActor* pOneFightPet = pPetpackage->getFightPet( i );
		if ( pOneFightPet == NULL )
		{
			continue;
		}

		Ogre::ShowActor* pOneShowPetActor = pOneFightPet->getShowActor();
		if ( pOneShowPetActor->isInMap() )
		{
			continue;
		}

		pOneShowPetActor->onEnterMap( pMapMgr->getShowGameMap() );
	}*/
}

void GameActionManager::OnActionMainPlayerJoinTeam( const CSACTIONRES& Msg )
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	if ( actorMgr.getShowOtherPlayerModelOption() == HIDE_MODEL_NONE )
	{
		return;
	}
	MainGamePlayer* pMainplayer		= actorMgr.getMainPlayer();
	tdr_ulonglong mainplayerTeamWID = pMainplayer->getTeamWID();
	for( ACTOR_ITER iter = actorMgr.getActorBegin(), end = actorMgr.getActorEnd(); 
		iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		
		if ( !actorMgr.isActorIgnoreF12( pOneActor ) )
		{
			continue;
		}

		GameMap* pMapMgr = pOneActor->getCurMap();
		if ( pMapMgr == NULL )
		{
			continue;
		}

		Ogre::ShowActor* pOneShowActor = pOneActor->getShowActor();
		if ( pOneShowActor->isInMap() )
		{
			continue;
		}

		pOneShowActor->update(60000);
		pOneShowActor->onEnterMap( pMapMgr->getShowGameMap() );
		Ogre::ShowActor* pOneShowRiderActor = pOneActor->m_Rider.m_pShowActor;
		if( pOneShowRiderActor != NULL )
		{
			pOneShowRiderActor->onEnterMap( pMapMgr->getShowGameMap() );
		}
	}
}

void GameActionManager::OnActionJoinTeam( const CSACTIONRES& Msg )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( Msg.ID );
	if ( pActor == NULL )
	{
		return;
	}

	GameTeamManager& teamMgr			= GameTeamManager::getSingleton();
	MainGamePlayer* pMainPlayer			= ActorManager::getSingleton().getMainPlayer();
	const CSTEAMACTION& teamActionData	= Msg.Data.Team;
	
	pActor->setTeamWID( teamActionData.TeamID );
	pActor->setTeamName( teamActionData.TeamName );
	pActor->setCaptain( teamActionData.Captain == 0?false:true );
	//pActor->setViewFlg( VIEW_FLAG_CAPTAIN );
	pActor->m_nTeamNum = teamActionData.TeamNum;

	if ( pMainPlayer->GetID() == Msg.ID )
	{
		OnActionMainPlayerJoinTeam( Msg );
		GameManager::getSingleton().SendUIEvent( "GE_TEAM_APPLYSUCC" );
		GameManager::getSingleton().SendUIEvent( "GE_MAIN_JOIN_TEAM_SUCC" );
	}
	else
	{
		OnActionOtherPlayerJoinTeam( Msg );
		GameManager::getSingleton().SendUIEvent( "GE_TEAM_NEW_PLAYER_JOIN" );
	}
}

void GameActionManager::OnActionFakeStatus( const CSACTIONRES& Msg  )
{
	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor* pActor		= actorMgr.FindActor( Msg.ID );
	if ( pActor == NULL )
	{
		return;
	}
	
	MainGamePlayer* pMainPlayer	= actorMgr.getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}
	
	GameBufStatus& statusMgr = pActor->m_BufStatus;
	const CSACTIONFAKESTATUS& FakeStatus = Msg.Data.FakeStatus;
	if ( FakeStatus.OpType == FAKE_STATUS_OP_DEL )
	{
		statusMgr.delJiaStatus( FakeStatus.ID, FakeStatus.Level );
		//如果是主角，地图加载已经完成的话，给提示框
	}
	else if ( FakeStatus.OpType == FAKE_STATUS_OP_ADD )
	{
		statusMgr.addJiaStatus( FakeStatus.ID, FakeStatus.Level );
	}

	//vip 网吧
	if( FakeStatus.OpType == FAKE_STATUS_OP_DEL && FakeStatus.ID == VIP_BAR_STATUS_ID
		&& FakeStatus.Level == VIP_BAR_STATUS_LEVEL )
	{
		memset( &(pActor->m_NetBarInfo),0,sizeof(tagNetBarInfo_ToLua) );
	}else if( FakeStatus.ID == VIP_BAR_STATUS_ID && FakeStatus.Level == VIP_BAR_STATUS_LEVEL ){
		memset( &(pActor->m_NetBarInfo),0,sizeof(tagNetBarInfo_ToLua) );
		pActor->m_NetBarInfo.DynValueNum = FakeStatus.DynValueNum;
		for( int i = 0 ; i < FakeStatus.DynValueNum ; i++ )
		{
			pActor->m_NetBarInfo.DynValue[i] = FakeStatus.DynValue[i];
		}
	}
}