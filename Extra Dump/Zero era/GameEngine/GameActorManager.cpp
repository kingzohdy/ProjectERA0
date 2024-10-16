#include <algorithm>
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameActor_Pet.h"
#include "GameActor_PVP.h"
#include "GameActor.h"
#include "GameNpc.h"
#include "GameMaskMap.h"
#include "GameMsgPrint.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "ServerTimer.h"
#include "GameSkill_Mgr.h"
#include "GameActor_RoleText.h"
#include "OgreScriptLuaVM.h"
#include "OgreRay.h"
#include "AvatarManager.h"
#include "AvatarEntity.h"
#include <Windows.h>
#include "GameMap.h"
#include "OgreRoot.h"
#include "GameSCTDamageManager.h"
#include "GameActor_Tick.h"
#include "OgreSoundSystem.h"
#include "GameEffect_Mgr.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "WorldCity.h"

#include "../UILib/ui_framemgr.h"

#include "OgreBorderScene.h"
#include "BulletManager.h"
#include "ApexClient.h"

#include "OgreLog.h"
using namespace Ogre;
using namespace std;

const int MAXPLAYERDISPLAY = 30;

int getPlayerRelationType( GameActor* pActor );

_FUNC_C_REC pfSendToApexClient = NULL;

long netSendToGameServer(const char *pBuffer, int nLen) 
{
	/*tagCSPkgBody msgbody;
	msgbody.ApexClt.DataLen = nLen;
	memcpy( &msgbody.ApexClt.Data, pBuffer, nLen );
	SendGameServerMsg( APEX_CLT, msgbody );*/

	return 0;
}

bool CompareRoleName(	const char* pszLeftOriginalName,	int nLeftWorldID, 
					 const char* pszRightOriginalName,	int nRightWorldID )
{
	assert( pszLeftOriginalName != NULL && pszRightOriginalName != NULL );
	bool isOriginalNameSame = ( strcmp( pszLeftOriginalName, pszRightOriginalName ) == 0 );
	bool isWorldSame		= ( nLeftWorldID == nRightWorldID );

	if ( !isOriginalNameSame )
	{
		return false;
	}

	return nLeftWorldID == nRightWorldID;
}


bool CompareRoleName( const char* pszLeftName, const char* pszRightName )
{
	assert( pszLeftName != NULL && pszRightName != NULL );

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer == NULL )
	{
		return false;
	}

	const char* pszLeftResult	= strpbrk( pszLeftName, "$" );
	int nLeftWorldID			= 0;
	if ( pszLeftResult == NULL )
	{
		pszLeftResult	= pszLeftName;
		nLeftWorldID	= pMainplayer->GetWorldID();
	}
	else
	{
		++pszLeftResult;
		const char* pszLeftWorldText = pszLeftName + 2;
		nLeftWorldID = atoi( pszLeftWorldText );
	}

	const char* pszRightResult	= strpbrk( pszRightName, "$" );
	int nRightWorldID			= 0;
	if ( pszRightResult == NULL )
	{
		pszRightResult	= pszRightName;
		nRightWorldID	= pMainplayer->GetWorldID();
	}
	else
	{
		++pszRightResult;
		const char* pszRightWorldText = pszRightName + 2;
		nRightWorldID = atoi( pszRightWorldText );
	}

	return CompareRoleName( pszLeftResult, nLeftWorldID, pszRightResult, nRightWorldID );
}


class LooseItemShow : public IActorDeathAction
{
protected:
	tagLoosItemPosInfo		m_info;

public:
	LooseItemShow(const tagLoosItemPosInfo &info)
	{
		m_info		= info;
	}

	virtual void doAction(GameActor *pActor)
	{
		ActorManager::getSingleton().addLooseItemShow(m_info);
	}
};

class ActorDistance
{
private:
	GameActor *m_refer;
public:
	ActorDistance(GameActor *refer) : m_refer(refer) {}
	bool operator () (GameActor *a, GameActor *b)
	{
		const GameXYZ &pr = m_refer->GetPosition();
		const GameXYZ &p1 = a->GetPosition();
		const GameXYZ &p2 = b->GetPosition();
		int diffx1 = (p1.x - pr.x) / 100;
		int diffy1 = (p1.z - pr.z) / 100;
		int diffx2 = (p2.x - pr.x) / 100;
		int diffy2 = (p2.z - pr.z) / 100;
		return diffx1 * diffx1 + diffy1 * diffy1 < diffx2 * diffx2 + diffy2 * diffy2;
	}
};

bool ActorManager::HeapCmp::operator() (GameActor *_Left, GameActor *_Right) const
{
	ActorManager &actorMgr = ActorManager::getSingleton();
	bool hasLeft, hasRight;
	hasLeft = actorMgr.playerEnemy.find(_Left->GetID()) != actorMgr.playerEnemy.end();
	hasRight = actorMgr.playerEnemy.find(_Right->GetID()) != actorMgr.playerEnemy.end();
	if (hasLeft)
	{
		if (!hasRight)
		{
			return false;
		}
	}
	else
	{
		if (hasRight)
		{
			return true;
		}
	}
	hasLeft = actorMgr.lastAttacker.find(_Left->GetID()) != actorMgr.lastAttacker.end();
	hasRight = actorMgr.lastAttacker.find(_Right->GetID()) != actorMgr.lastAttacker.end();
	if (hasLeft)
	{
		if (!hasRight)
		{
			return false;
		}
	}
	else
	{
		if (hasRight)
		{
			return true;
		}
	}
	return (_Left->GetPosition().subtract(basePos).length() >
			_Right->GetPosition().subtract(basePos).length());
}

template<class T>
bool ActorManager::TimeHeapCmp<T>::operator () (const pair<unsigned, T> &_Left, const pair<unsigned, T> &_Right) const
{
	return _Left.first > _Right.first;
}

ActorManager::ActorManager() : m_pMainPlayer(NULL), m_pScriptVM(NULL), m_nShowModelOption( HIDE_MODEL_NONE ), m_bShowOtherPlayerTitle( true ),m_bShowPlayerID( false ),m_RequestMachineId(0)
{
	ShowActor::initCommon();
	AvatarEntity::m_pAvatarMgr = new AvatarManager;
	AvatarEntity::m_pAvatarMgr->init();

	INetworkManager::getSingleton().RegisterMsgHandler(MOVE_STOP,	this);
	INetworkManager::getSingleton().RegisterMsgHandler(MOVE_OTHER,	this);
	INetworkManager::getSingleton().RegisterMsgHandler(ENTER_VIEW,	this);
	INetworkManager::getSingleton().RegisterMsgHandler(LEAVE_VIEW,	this);
	INetworkManager::getSingleton().RegisterMsgHandler(ATTR_CHG,	this);
	INetworkManager::getSingleton().RegisterMsgHandler(BASE_ATTR_CHG,this);
	//INetworkManager::getSingleton().RegisterMsgHandler(APEX_SVR,	this);

	m_attrMapToHurt[ATTR_ID_EXP]			= HURT_TYPE_EXP;
	m_attrMapToHurt[ATTR_ID_MONEY]			= HURT_TYPE_MONEY;
	m_attrMapToHurt[ATTR_ID_BINDGODCOIN]	= HURT_TYPE_BINDGOLD;
	m_attrMapToHurt[ATTR_ID_ZHENGYI]		= HURT_TYPE_ZHENGYI;
	m_attrMapToHurt[ATTR_ID_LIFT_BRAND]		= HURT_TYPE_LIFTBRAND;
	//m_attrMapToHurt[ATTR_ID_MP]		= HURT_TYPE_MP;
	InitBaseWarMapOwnerList();
	DBDataManager::getSingleton().loadSpartaLevelDef(m_SpartaLevelData);
}

ActorManager::~ActorManager()
{
	DestroyAllActor( true );
	delete AvatarEntity::m_pAvatarMgr;
	ShowActor::releaseCommon();
}

void ActorManager::InitBaseWarMapOwnerList()
{
	m_BaseWarMapOwnerList[0].nBaseWarMapID = 21300,m_BaseWarMapOwnerList[0].nMapID = 2202,m_BaseWarMapOwnerList[0].nOwnerClanWID = 0;
	m_BaseWarMapOwnerList[1].nBaseWarMapID = 21400,m_BaseWarMapOwnerList[1].nMapID = 2200,m_BaseWarMapOwnerList[1].nOwnerClanWID = 0;
	m_BaseWarMapOwnerList[2].nBaseWarMapID = 21500,m_BaseWarMapOwnerList[2].nMapID = 2300,m_BaseWarMapOwnerList[2].nOwnerClanWID = 0;
	m_BaseWarMapOwnerList[3].nBaseWarMapID = 21600,m_BaseWarMapOwnerList[3].nMapID = 2400,m_BaseWarMapOwnerList[3].nOwnerClanWID = 0;
	m_BaseWarMapOwnerList[4].nBaseWarMapID = 21700,m_BaseWarMapOwnerList[4].nMapID = 2500,m_BaseWarMapOwnerList[4].nOwnerClanWID = 0;
	m_BaseWarMapOwnerList[5].nBaseWarMapID = 21800,m_BaseWarMapOwnerList[5].nMapID = 3404,m_BaseWarMapOwnerList[5].nOwnerClanWID = 0;
}

GameActor* ActorManager::FindActor( int id )
{
	std::map<int, GameActor *>::iterator iter = m_Actors.find(id);
	if(iter != m_Actors.end())
	{
		return iter->second;
	}
	return NULL;
}

int ActorManager::getActorIdByResId( int id )
{
	std::map<int, int>::iterator iter = m_ActorsByResId.find( id );
	if( iter != m_ActorsByResId.end() )
	{
		return iter->second;
	}
	return 0;
}

GameActor* ActorManager::CreateActor(int id, GameActorType type, int resid, bool background/* = false*/)
{
	assert(FindActor(id) == NULL);

	GameActor *pactor = NULL;
	switch(type)
	{
	case GAT_NPC:
		{
			pactor = new GameNPC;
			const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( resid );
			((GameNPC*)pactor)->m_strNpcFunctionDes = npcDisplayInfo.npcdesc;
			char szScript[256];
			sprintf( szScript, "AddNearNpc(%d, %d)", id, resid);
			m_pScriptVM->callString(szScript);
		}
		break;
	case GAT_MONSTER:
		{
			pactor = new GameNPC;
			const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( resid );
			((GameNPC*)pactor)->m_strNpcFunctionDes = npcDisplayInfo.npcdesc;
			m_Monsters.push_back( pactor );
			char szScript[256];
			sprintf( szScript, "AddNearMonster(%d)", id);
			m_pScriptVM->callString(szScript);
			const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( resid );
			
			if ( pMonsterDef != NULL && pMonsterDef->SubType == MON_SUB_TYPE_COLLECT && 
					( npcDisplayInfo.race & MONSTER_PET ) != 0 )
			{
				pactor->setCollectInfo( MONSTER_PET, true );
			}
			if( pMonsterDef )
			{
				pactor->setDartCarType( pMonsterDef->DartCarType );
			}
		}
		break;
	case GAT_PLAYER:
		{
			pactor = new GamePlayer;
			m_Players.insert(pactor);
			char szScript[256];
			sprintf( szScript, "AddNearPlayer(%d)", id);
			m_pScriptVM->callString(szScript);
		}
		break;
	case GAT_MAINPLAYER:
		{	
			m_pMainPlayer = new MainGamePlayer;
			pactor = m_pMainPlayer;
			m_pMainPlayer->m_ObjCamp = SKILL_TARGET_SELF;
			SCTDamageManager::getSingleton().addDamageActor( id );

			//// 启动反外挂
			//long re = CHCStart( netSendToGameServer, pfSendToApexClient );
			//LOG_SEVERE( "Apex return: %d", re );
		}
		break;
	case GAT_ITEM:
		pactor = new GameNPC;
		break;
	case GAT_PET:
	case GAT_FAIRY:
		pactor = new GameNPC;
		break;
	default:
		assert(0);
		break;
	}
	pactor->setType(type);

	pactor->LoadRes(resid);
	pactor->SetID(id);
	m_Actors[id] = pactor;
	m_ActorsByResId[resid] = id;
	
	if( type == GAT_PLAYER || type == GAT_MAINPLAYER )
	{
		if (pactor->getShowActor())
		{
			pactor->getShowActor()->setFootprint(pactor->getShowActor()->getCurEntity(), pactor, true);
		}
	}

	return pactor;
}

void ActorManager::DestroyActor(GameActor *pactor, bool bDeletePet)
{
	int id = pactor->GetID();
	std::map<int, GameActor *>::iterator iter = m_Actors.find(id);

	if ( iter != m_Actors.end() )
	{
		// 不删除宠物，由拥有者统一删除
		if( ( pactor->getType() == GAT_PET || pactor->getType() == GAT_FAIRY ) && !bDeletePet )
		{
			return;	
		}
		removeActorFade(pactor);
		m_Actors.erase(iter);

		char szPopName[256];
		if ( pactor->getType() == GAT_PLAYER || pactor->getType() == GAT_MAINPLAYER )
		{
			sprintf( szPopName, "%s",pactor->getName() );
		}
		else if( pactor->getType() == GAT_PET )
		{
			sprintf( szPopName, "%d", pactor->GetID() );
		}
		else
		{
			sprintf( szPopName, "%s%d",pactor->getName(), pactor->GetID() );
		}
		g_pFrameMgr->hidePopWin( szPopName );
		switch (pactor->getType())
		{
		case GAT_NPC:
			{
				char szScript[256];
				sprintf( szScript, "RemoveNearNpc(%d,%d)", pactor->GetID(),pactor->getResId());
				m_pScriptVM->callString(szScript);
			}
			break;

		case GAT_MONSTER:
			{
				char szScript[256];
				sprintf( szScript, "RemoveNearMonster(%d)", pactor->GetID());
				m_pScriptVM->callString(szScript);
			}
			break;

		case GAT_PLAYER:
			{
				char szScript[256];
				sprintf( szScript, "RemoveNearPlayer(%d)", pactor->GetID());
				m_pScriptVM->callString(szScript);
			}
			break;
		}
		updateMonsters( pactor );

		for (unsigned int i = 0; i < m_ItemsChangeOwner.size(); ++i)
		{
			if (m_ItemsChangeOwner[i].second == pactor->GetID())
			{
				m_ItemsChangeOwner.erase(m_ItemsChangeOwner.begin() + i);
				break;
			}
		}
		delete pactor;
	}
	else
	{
		assert(0);
	}
	
	if( m_pMainPlayer == pactor )
	{
		m_pMainPlayer = NULL;
		EffectManage::getSingleton().clearLink(id);
		BulletManager::getSingleton().ClearBullet(false);
		m_pScriptVM->callString("MainPlayerDestroy()");

		CHCEnd();
	}
}

void ActorManager::DestroyAllActor(bool bDelMainPlayer /*= true*/, bool detach /*= false */)
{
	for( std::map<int, GameActor*>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); )
	{
		if( bDelMainPlayer && m_pMainPlayer && m_pMainPlayer ==  iter->second )
		{
			EffectManage::getSingleton().clearLink(m_pMainPlayer->GetID());
			BulletManager::getSingleton().ClearBullet(detach);
			m_pScriptVM->callString("MainPlayerDestroy()");
			m_pMainPlayer = NULL;
			updateMonsters( iter->second );
			if (detach)
			{
				iter->second->OnLeaveMap();
			}
			removeActorFade(iter->second);
			delete iter->second;
			iter = m_Actors.erase( iter );
// 			m_monsterMachineWaiters.clear();

			CHCEnd();
			continue;
		}
		else
		{
			if( m_pMainPlayer ==  iter->second )
			{
				++iter;
				continue;
			}
		}
		if( iter->second->getType() == GAT_PET || iter->second->getType() == GAT_FAIRY  )
		{
			++iter;
			continue;
		}
		
		updateMonsters( iter->second );
 		if (detach)
 		{
 			iter->second->OnLeaveMap();
 		}
		removeActorFade(iter->second);
		delete iter->second;
		iter = m_Actors.erase( iter );
	}

	m_pScriptVM->callString("ClearNearNpc() ClearNearMonster() ClearNearPlayer()");
	m_ActorsByResId.clear();
	m_ItemsChangeOwner.clear();
}

void ActorManager::Update(unsigned int dtick)
{
	unsigned int now = timeGetTime();

	int IsRookieProtect = false;
	if (m_pMainPlayer != NULL)
	{
		while (!m_ItemsChangeOwner.empty())
		{
			if (m_ItemsChangeOwner.front().first > now)
			{
				break;
			}
			GameActor *pActor = ActorManager::getSingleton().FindActor(m_ItemsChangeOwner.front().second);
			std::pop_heap(m_ItemsChangeOwner.begin(), m_ItemsChangeOwner.end(), TimeHeapCmp<int>());
			m_ItemsChangeOwner.pop_back();
			if (pActor != NULL && pActor->getType() == GAT_ITEM)
			{
				pActor->setRoleWId(m_pMainPlayer->getRoleWId());
				//取消roll 状态
				pActor->setRollType( ITEM_ROLL_NONE );
				if (pActor->m_CollectTime > 0)
				{
					pActor->playMotion("9510_16");
				}
				else
				{
					pActor->playMotion("9572_16");
				}
				char szScript[256];
				sprintf( szScript, "OnItemChangeOwner(%d)", pActor->GetID() );
				getScriptVM()->callString( szScript );
			}
		}
		GameMap *map = m_pMainPlayer->getCurMap();
		if (map)
		{
			const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(map->getRealId());
			if (mapdef.CtrlFlag & MAP_CTRL_ROOKIE_PROTECT)
			{
				IsRookieProtect = true;
			}
		}
	}

	bool beHide = false;
	GameXYZ selfPos;
	if (m_Players.size() > MAXPLAYERDISPLAY && m_pMainPlayer)
	{
		beHide = true;
		selfPos = m_pMainPlayer->GetPosition();
	}

	std::set<GameActor *>	nearestPlayers;
	if (beHide)
	{
		std::vector<GameActor *> players;
		players.reserve(m_Players.size());
		for (std::set<GameActor *>::iterator i = m_Players.begin(), j = m_Players.end(); i != j; ++i)
		{
			players.push_back(*i);
		}
		std::sort(players.begin(), players.end(), ActorDistance(m_pMainPlayer));
		players.resize(MAXPLAYERDISPLAY);
		for (std::vector<GameActor *>::iterator i = players.begin(), j = players.end(); i != j; ++i)
		{
			nearestPlayers.insert(*i);
		}
	}

	std::map<int, GameActor *>::iterator iter = m_Actors.begin();
	FollowCamera *camera = FollowCamera::getSingletonPtr();
	Ogre::Camera *cutcamera = NULL;
	if (camera != NULL)
	{
		cutcamera = camera->GetCutActorCameraNode();
	}
	for(; iter!=m_Actors.end(); )
	{
		if (iter->second->getType() != GAT_ITEM)
		{
			if (cutcamera && !cutcamera->canSeePointInWorld(iter->second->GetPosition()))
			{
				iter->second->optimizeClear(false);
			}
			else
			{
				if (!canShowActorModelInScene(iter->second))
				{
					GameMap *map = iter->second->getCurMap();
					if (map != NULL)
					{
						iter->second->getShowActor()->onLeaveMap(map->getShowGameMap());
						if ( iter->second->m_Rider.m_pShowActor != NULL )
						{
							iter->second->m_Rider.m_pShowActor->onLeaveMap(map->getShowGameMap());
						}
					}
				}
				if (iter->second->getType() == GAT_PLAYER)
				{
					if (beHide)
					{
						if (nearestPlayers.find(iter->second) != nearestPlayers.end())
						{
							iter->second->optimizeDisplay(true);
						}
						else
						{
							iter->second->optimizeDisplay(false);
						}
					}
					else
					{
						iter->second->optimizeDisplay(true);
					}
				}
				iter->second->optimizeClear(true);
			}
		}

		GameActor* pActor = iter->second;
		////特殊处理，带有巡逻路径的机械怪，乘客的位置要和怪物同步
		if( pActor->getType() == GAT_MONSTER && pActor->getSubType() == MON_SUB_TYPE_MACHINE )
		{
			if( pActor->m_nPatrolID > 0 )
			{
				int passengerNum = pActor->m_MonsterMachine.m_monsterInfo.m_nMaxPassenger > MAX_MON_MACHINE_PASSENGER ? MAX_MON_MACHINE_PASSENGER : pActor->m_MonsterMachine.m_monsterInfo.m_nMaxPassenger;
				for( int m = 0; m < passengerNum; m++ )
				{
					int passengerId = pActor->m_MonsterMachine.m_monsterInfo.m_nPassengerID[m];
					GameActor* pPassenger = FindActor(passengerId);
					if( pPassenger != NULL )
					{
						pPassenger->SetPosition(  pActor->GetPosition().x / 10, pActor->GetPosition().z / 10 );
					}
				}
			}
		}
		iter->second->Update(dtick);

		if( pActor->getActState().GetState() == GSACT_MOVEBACK )
		{
			if( !pActor->UpdateMoveBack() )
			{
				if( m_pMainPlayer->GetSelectTarget() == pActor->GetID() )
				{
					GameMsgPrint::getSingleton().showSelEffect( true );
				}
				removeActorFade(pActor);
				iter = m_Actors.erase( iter );
				pActor->OnLeaveMap();
				switch (pActor->getType())
				{
				case GAT_NPC:
					{
						char szScript[256];
						sprintf( szScript, "RemoveNearNpc(%d, %d)", pActor->GetID(), pActor->getResId());
						m_pScriptVM->callString(szScript);
					}
					break;

				case GAT_MONSTER:
					{
						char szScript[256];
						sprintf( szScript, "RemoveNearMonster(%d)", pActor->GetID());
						m_pScriptVM->callString(szScript);
					}
					break;

				case GAT_PLAYER:
					{
						char szScript[256];
						sprintf( szScript, "RemoveNearPlayer(%d)", pActor->GetID());
						m_pScriptVM->callString(szScript);
					}
					break;
				}
				updateMonsters( pActor );
				delete pActor;
				continue;
			}
		}
		if (pActor->getType() == GAT_PLAYER || (pActor->getType() == GAT_MONSTER && pActor->getSubType() < 100 && pActor->getSubType() != MON_SUB_TYPE_HUMAN_FRIEND && pActor->getSubType() != MON_SUB_TYPE_HUMAN_BROTHER) ||
			pActor->getType() == GAT_NPC)
		{
			GameActor_PVP::getSingleton().judgeCamp(pActor, IsRookieProtect);
		}
		iter++;
	}

	for (std::map<GameActor *, std::pair<float, bool>>::iterator i = m_fadeActors.begin(); i != m_fadeActors.end();)
	{
		static const float alphaStep = 1;
		GameActor *pActor = i->first;
		if (i->second.second)
		{
			float alpha = i->second.first + dtick * (alphaStep / 1000);
			i->second.first = alpha;
			if (alpha > 1)
			{
				alpha = 1;
			}
			int resid = pActor->GetMorphResID();
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
			if (pActor->m_Rider.m_pShowActor != NULL)
			{
				pActor->m_Rider.m_pShowActor->getCurEntity()->setTransparent(alpha);
			}
			else
			{
				pActor->getShowActor()->getCurEntity()->setTransparent(alpha * info.alpha);
			}
			if (alpha == 1)
			{
				i = m_fadeActors.erase(i);
				continue;
			}
		}
		else
		{
			float alpha = i->second.first - dtick * (alphaStep / 1000);
			i->second.first = alpha;
			if (alpha > 0.1f)
			{
				int resid = pActor->GetMorphResID();
				const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
				if (pActor->m_Rider.m_pShowActor != NULL)
				{
					pActor->m_Rider.m_pShowActor->getCurEntity()->setTransparent(alpha);
				}
				else
				{
					pActor->getShowActor()->getCurEntity()->setTransparent(alpha * info.alpha);
				}
			}
			else
			{
				int resid = pActor->GetMorphResID();
				const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
				if (pActor->m_Rider.m_pShowActor != NULL)
				{
					pActor->m_Rider.m_pShowActor->getCurEntity()->setTransparent(1);
				}
				else
				{
					pActor->getShowActor()->getCurEntity()->setTransparent(info.alpha);
				}
				if (pActor->getCurMap() != NULL)
				{
					if (pActor->m_lastOptimizeTime == 0)
					{
						pActor->m_lastOptimizeTime = timeGetTime();
					}
					pActor->getShowActor()->onLeaveMap(pActor->getCurMap()->getShowGameMap());
					if (pActor->m_Rider.m_pShowActor != NULL)
					{
						pActor->m_Rider.m_pShowActor->onLeaveMap(pActor->getCurMap()->getShowGameMap());
					}
					pActor->m_pSmallDecal->show(false);
				}
				i = m_fadeActors.erase(i);
				continue;
			}
		}
		++i;
	}

	looseItemShow();

	for (std::map<int, int>::iterator i = lastAttacker.begin(); i != lastAttacker.end();)
	{
		if ((unsigned int)i->second <= now)
		{
			i = lastAttacker.erase(i);
			continue;
		}
		++i;
	}
	if ( !m_ClientSelfDestoryActors.empty() )
	{
		for ( int i = 0, size = (int)m_ClientSelfDestoryActors.size(); i < size; ++i )
		{
			GameActor* pActor = FindActor(m_ClientSelfDestoryActors[i]);
			if (pActor != NULL)
			{
				pActor->OnLeaveMap();
				DestroyActor( pActor, true );
			}
		}
		m_ClientSelfDestoryActors.clear();
	}
}

// 添加骑乘等待列表
void ActorManager::pushWaitRideList( int nOwnerId, int nGuestId )
{
	m_waitRideList[nOwnerId].push_back( nGuestId );
}

// 获取骑乘列表
void ActorManager::popWaitRideList( int nOwnerId, std::vector<int>& passengerList )
{
	OWNER_ITER iter = m_waitRideList.find( nOwnerId );

	if ( iter != m_waitRideList.end() )
	{
		passengerList = iter->second;
		m_waitRideList.erase( iter );
	}
}

void ActorManager::drawActorTitles(FollowCamera *pcamera)
{
	
	for( std::map<int, GameActor *>::iterator iter = m_Actors.begin(); iter!=m_Actors.end(); iter++)
	{
		GameActor* pOneActor = iter->second;
		RoleText& drawRoleTextMgr = pOneActor->getRoleText();
		if( pOneActor->getActState().GetState() != GSACT_DEAD )
		{
			drawRoleTextMgr.drawTitles(pcamera);	
		}

		drawRoleTextMgr.drawFlyTitles(pcamera);
		drawRoleTextMgr.drawMonsterPopWin( pcamera );
		drawRoleTextMgr.drawPetPopWin( pcamera );
		if( m_bShowPlayerID )
		{
			drawRoleTextMgr.drawIDTextTitle(pcamera);
		}
	}
}

int getSelectActorLevel( int nType )
{
	const int  MAX_CALACTOR = 5;
	const int  actorLevel[MAX_CALACTOR] = {
		GAT_ITEM,
		GAT_MONSTER,
		GAT_NPC,
		GAT_PLAYER,
		GAT_PET
	};
	for( int i = 0; i < MAX_CALACTOR; i++ )
	{
		if( nType == actorLevel[i] )
		{
			return i;
		}
	}

	return 9999;
}

// 获取选择优先级
GameActor* ActorManager::getSelectActor( GameActor* pPreSelect, GameActor* pCurSelect )
{
	if( pPreSelect != NULL )
	{
		if( getSelectActorLevel( pPreSelect->getType() ) < getSelectActorLevel( pCurSelect->getType() ) )
		{
			return pPreSelect;
		}else{
			return pCurSelect;
		}
	}else{
		if( pCurSelect == m_pMainPlayer )
		{
			return NULL;
		}
		return pCurSelect;
	}

	return NULL;
}

// 鼠标所在的对象
GameActor*	ActorManager::mounseOnActor( const Ogre::WorldRay& worldray )
{
	Ray ray;
	worldray.getRelativeRay(ray, WorldPos(0,0,0));
	GameActor* pSelectActor = NULL;

	if(BorderGameScene::getSingletonPtr())
	{
		BorderGameScene::getSingleton().clear();
	}

	std::map<int, GameActor*>::iterator iter = m_Actors.begin();
	for( iter = m_Actors.begin(); iter!=m_Actors.end(); iter++)
	{
		iter->second->getShowActor()->getCurEntity()->setInstanceAmbient(ColourValue(0.45f, 0.45f, 0.45f, 0));

		GameActor* pactor = iter->second;
		bool b = pactor->getShowActor()->intersectRay(ray);
		if( b ) 
		{
			if (!pactor->isShow())
			{
				continue;
			}

			if( pactor->getType() != GAT_PLAYER && pactor->getActState().GetState() == GSACT_DEAD )
			{	
				continue;
			}

			if (pactor->getType() == GAT_PET && pactor->m_isHeti)
			{
				continue;
			}

			if (pactor->getSubType() == MON_SUB_TYPE_TRAP 
				|| pactor->getSubType() == MON_SUB_TYPE_BUILD)
			{
				/// 不允许选中图腾和地雷
				continue;
			}
			if (pactor->getType() == GAT_PET && ActorManager::getSingleton().getGameOpt(GAME_OPT_FORBIT_SELECT_PET))
			{
				//系统设置（禁止选中宠物），不允许选中宠物
				continue;
			}
			if( pactor->getSubType() == MON_SUB_TYPE_MACHINE 
				&& pactor->getMonsterMachine()->isPlayerOnMachine( ActorManager::getSingleton().getMainPlayer() ))
			{
				continue;
			}
			
			if (!isShowOtherPlayerTitle() && !pactor->isInScene() )
			{
				if ( !canShowActorModelInScene( pactor ) )
				{
					continue;
				}
			}

			if ( pactor->getType() == GAT_MONSTER && pactor->getSubType() == MON_SUB_TYPE_COLLECT && 
					!pactor->canCollect() )
			{
				// 既不为宠物采集怪也不为任务允许采集怪，则也不允许选中
				continue;
			}

			/// 主人死了，不能选中宠物 或者主人在飞行状态
			if ( pactor->getType() == GAT_PET && FindActor(pactor->getOwnerId()) && 
					(FindActor(pactor->getOwnerId())->isDead() || FindActor(pactor->getOwnerId())->isInFly()) )
			{
				continue;
			}

			int resid = pactor->getResId();
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
			if (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_SELECT)
			{
				continue;
			}

			if( pSelectActor != NULL )
			{
				static int nLevel[5][2] = {
					{ GAT_MONSTER,		1 },
					{ GAT_NPC,			2 },
					{ GAT_PLAYER,		3 },
					{ GAT_PET,			4 },
					{ GAT_MAINPLAYER,	6 },
				}; 

				int nSelectLevel = -1;
				int nActorLevel	 = -1;
				if (pactor->getType() == GAT_ITEM)
				{
					const tagItemDef &itemDef  = DBDataManager::getSingleton().getItemDef(pactor->getResId());
					if (&itemDef == NULL || itemDef.ItemTmp.Type == ITEM_TMP_NO)
					{
						nActorLevel = 0;
					}
					else
					{
						nActorLevel = 5;
					}
				}
				else
				{
					for( int i = 0; i < sizeof(nLevel) / (sizeof(int) * 2); i++ )
					{
						if( pactor->getType() == nLevel[i][0] )
						{
							nActorLevel = nLevel[i][1];
							break;
						}
					}
				}

				if (pSelectActor->getType() == GAT_ITEM)
				{
					const tagItemDef &itemDef  = DBDataManager::getSingleton().getItemDef(pSelectActor->getResId());
					if (&itemDef == NULL || itemDef.ItemTmp.Type == ITEM_TMP_NO)
					{
						nSelectLevel = 0;
					}
					else
					{
						nSelectLevel = 5;
					}
				}
				else
				{
					for( int i = 0; i < sizeof(nLevel) / (sizeof(int) * 2); i++ )
					{
						if( pSelectActor->getType() == nLevel[i][0] )
						{
							nSelectLevel = nLevel[i][1];
							break;
						}
					}
				}

				if( nActorLevel < nSelectLevel )
				{
					pSelectActor = pactor;
				}
			}else{
				pSelectActor = pactor;
			}
		}
	}
	
	if( pSelectActor != NULL )
	{
		Ogre::ShowActor* pSelectShowActor = pSelectActor->getShowActor();
		if ( pSelectShowActor != NULL && !pSelectShowActor->isInMap() && !pSelectActor->isInStall() )
		{
			return pSelectActor;
		}

		Vector3 color = Vector3(0.16f,0.99f,0.99f);
		if ((pSelectActor->m_ObjCamp & SKILL_TARGET_ENEMY_MON) ||
			(pSelectActor->m_ObjCamp & SKILL_TARGET_ENEMY_PLAYER) ||
			(pSelectActor->m_ObjCamp & SKILL_TARGET_ENEMYPET))
		{
			color = Vector3(1.0f, 0.0f, 0.0f);
		}

		if (pSelectActor->getType() != GAT_MAINPLAYER && pSelectActor->getSubType() != MON_SUB_TYPE_TOTEM && pSelectActor->getSubType() != MON_SUB_TYPE_TRAP)
		{
			pSelectActor->getShowActor()->getCurEntity()->setInstanceAmbient(ColourValue(0.8f, 0.8f, 0.8f, 0));
			if (BorderGameScene::getSingletonPtr() && pSelectActor->getShowActor())
			{
				Entity *entity = 0;
				entity = pSelectActor->getShowActor()->getCurEntity();
				if (entity)
				{
					entity->setBorderColor(color);
					BorderGameScene::getSingleton().onAttachObject(entity);
				}
			}

			if ( pSelectActor->m_Rider.isOnHorse() )
			{
				Ogre::ShowActor* pRideShowActor = pSelectActor->m_Rider.m_pShowActor;
				if ( pRideShowActor != NULL )
				{
					Entity *entity = 0;
					entity = pRideShowActor->getCurEntity();
					if (entity)
					{
						entity->setBorderColor(color);
						BorderGameScene::getSingleton().onAttachObject(entity);
					}
				}
			}
		}
	}

	return pSelectActor;
}

GameActor* ActorManager::pickActor(const WorldRay &worldray, int nPickType )
{
	GameActor* pSelectActor = mounseOnActor( worldray );
	if( pSelectActor != NULL )
	{
		if( pSelectActor->getType() == GAT_MAINPLAYER )
		{
			if (  nPickType == PICK_TYPE_LBUTTON_DOWN && pSelectActor->getActState().GetState() == GSACT_STALL )
			{
				GameMsgPrint::getSingleton().SendUIEvent( "GE_SELECT_MAINPLAYER_STALL_MODEL" );
			}

			return NULL;
		}
	}
	
	return pSelectActor;
}

GameActor*	ActorManager::findActorByName(const char* pszName)
{
	for (std::map<int, GameActor*>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
	{
		if( iter->second->getType() == GAT_PET )
		{
			continue;
		}
		if ( CompareRoleName(iter->second->getName(),pszName) )
		{
			return iter->second;
		}
	}

	return NULL;
}

GameActor*  ActorManager::findActorByName( const char* pszName, int nType )
{
	for (std::map<int, GameActor*>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor->getType() == nType && CompareRoleName(pOneActor->getName(),pszName) )
		{
			return pOneActor;
		}
	}

	return NULL;
}

GameActor*	ActorManager::findActorByWId( tdr_longlong wId )
{
	for (std::map<int, GameActor*>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); ++iter)
	{
		if ( iter->second->getRoleWId() == wId && iter->second->getType() != GAT_ITEM )
		{
			return iter->second;
		}
	}

	return NULL;
}

void ActorManager::tryPickMonsterMachine()
{
	if( getMainPlayer() == NULL || getMainPlayer()->isOnMonsterMachine() )
	{
		return;
	}
	//判断是否能使用机械
	for( std::map<int, GameActor *>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); ++iter )
	{
		MonsterMachine* pMonster = iter->second->getMonsterMachine();
		if ( iter->second->getType() != GAT_MONSTER || pMonster->m_monsterInfo.m_nMonsterMemID == 0 )
		{
			continue;
		}
		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( iter->second->getResId() );
		int radius = npcDisplayInfo.radius;
		if (radius < 200)
		{
			radius = 200;
		}
		if( !IsPointInCircle( iter->second->GetPosition().x, iter->second->GetPosition().z,
			m_pMainPlayer->GetPosition().x, m_pMainPlayer->GetPosition().z, 
			radius * 10 ) )
		{
			continue;
		}
		//需要判断公会
		if( (pMonster->isSameCamp())
			&& pMonster->canRideOn() )
		{
			if( pMonster->m_monsterInfo.m_nMaxPassenger > 1 )
			{
				m_RequestMachineId = iter->second->GetID();
				getMainPlayer()->requestMachinePassenger( iter->second->GetID() );
				return;
			}else{
				getMainPlayer()->requestRideMonsterMachine( iter->second->GetID() );
				return;				
			}
		}
	}
}

void ActorManager::canPickTmpItem()
{
	for( std::map<int, GameActor *>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); ++iter )
	{
		if ( iter->second->getType() != GAT_ITEM )
		{
			continue;
		}

		const tagItemDef& itemdef = DBDataManager::getSingleton().getItemDef( iter->second->getResId() );
		if( itemdef.ItemTmp.Type == ITEM_TMP_NO )
		{
			continue;
		}

		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( iter->second->getResId() );
		int radius = npcDisplayInfo.radius;
		if (radius < 200)
		{
			radius = 200;
		}

		if( IsPointInCircle( iter->second->GetPosition().x, iter->second->GetPosition().z,
								m_pMainPlayer->GetPosition().x, m_pMainPlayer->GetPosition().z, 
								radius * 10 ) )
		{
			if( m_pMainPlayer->isInMorph() && 
				!( m_pMainPlayer->GetMorphResID() == 292006 && itemdef.ItemTmp.Type != ITEM_TMP_MACHINE ) && 
				itemdef.ItemTmp.Type != ITEM_TMP_NO && 
				itemdef.ItemTmp.Type != ITEM_TMP_SUPPLY 
				)
			{
				continue;
			}

			pickTmpItem( iter->second->GetID() );
			return;
		}
	}
}
void ActorManager::pickTmpItem( int id )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type								= PACKAGE_REQ_PICK;
	msgbody.PackageReq.PackageReqData.PackagePick.ID	= id;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

bool ActorManager::needConvertName( const char* pszName )
{
	return strstr( pszName, "$" ) != NULL;
}

std::string	ActorManager::replaceSpecialCode( const std::string& strNeedConvertString )
{
	if ( needConvertName( strNeedConvertString.c_str() ) )
	{
		// 目前只定了这种情况
		return strNeedConvertString.substr( strNeedConvertString.find( "$" ) + 1 );
	}

	return strNeedConvertString;	
}

GameActor* ActorManager::getNearestActor( int src, int radius, bool bTask, int resid, int actorType )
{
	static std::vector<GameActor *>		s_nearestActor;
	static std::vector<GameActor *>		s_nearestTargetActor;
	static std::set<int>				s_preSelectList; 
	GameActor*							pActor = NULL;
	GameActor*							pPreActor = NULL;
	GameActor* pSrcActor	= FindActor( src );
	if( pSrcActor == NULL ) 
	{
		return NULL;
	}
	s_nearestActor.clear();
	s_nearestTargetActor.clear();
	GameXYZ		srcPos = pSrcActor->GetPosition();
	m_heapCmp.basePos = srcPos;

	for( std::map<int, GameActor *>::iterator iter = m_Actors.begin(); iter != m_Actors.end(); ++iter )
	{
		if( pSrcActor == iter->second )
		{
			continue;
		}

		if (!iter->second->isShow())
		{
			continue;
		}

		if( iter->second->getType() == GAT_PET )
		{
			continue;
		}

		if( iter->second->getType() == GAT_ITEM || iter->second->getActState().GetState() == GSACT_DEAD )
		{
			continue;
		}
		
		if( iter->second->getShowActor()->getActorRadius() == 0 )
		{
			continue;
		}
		if( resid > 0 && iter->second->getResId() != resid )
		{
			continue;
		}
		if( actorType >= 0 && iter->second->getType() != actorType )
		{
			continue;
		}
		if( !bTask )
		{
			// 特殊目标，不可选取
			if (iter->second->getSubType() == MON_SUB_TYPE_COLLECT ||
				iter->second->getSubType() == MON_SUB_TYPE_TRAP ||
				iter->second->getSubType() == MON_SUB_TYPE_BUILD ||
				iter->second->getSubType() == MON_SUB_TYPE_RONGLU)
			{
				continue;
			}

			if( !iter->second->isCamp( SKILL_TARGET_ENEMY_MON ) && !iter->second->isCamp( SKILL_TARGET_ENEMY_PLAYER ) )
			{
				continue;
			}
			if ( iter->second->getSubType() == MON_SUB_TYPE_HUMAN_FRIEND )
			{
				continue;
			}
		}		

		int resid = iter->second->getResId();
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
		if (info.MiscType & NPCDISPLAYINFO_MISC_TYPE_NO_SELECT)
		{
			continue;
		}

		if( radius != -1 )
		{
			GameXYZ pos	= iter->second->GetPosition();
			int length	= (int)pos.subtract(srcPos).length();
			if (length > radius/10)
			{
				continue;
			}
			if (iter->second->GetSelectTarget() == src)
			{
				s_nearestTargetActor.push_back(iter->second);
				std::push_heap( s_nearestTargetActor.begin(), s_nearestTargetActor.end(),
					m_heapCmp );
			}
			else
			{
				s_nearestActor.push_back(iter->second);
				std::push_heap( s_nearestActor.begin(), s_nearestActor.end(),
					m_heapCmp );
			}
		}
	}

	// 失去目标从最近的开始选择
	GameActor *target = FindActor(m_pMainPlayer->GetSelectTarget());
	if (target == NULL || target->isDead())
	{
		s_preSelectList.clear();
	}
	else
	{
		s_preSelectList.insert(target->GetID());
	}

	/// 删除不在身边的已选目标
	for( std::set<int>::iterator iterList = s_preSelectList.begin(); iterList != s_preSelectList.end(); )
	{
		bool bFind = false;
		for( std::vector<GameActor *>::iterator iter = s_nearestActor.begin(); iter != s_nearestActor.end(); ++iter )
		{
			if( *iterList == (*iter)->GetID() )
			{
				bFind = true;
				break;
			}
		}
		if( !bFind )
		{
			iterList = s_preSelectList.erase( iterList );
			continue;
		}

		for( std::vector<GameActor *>::iterator iter = s_nearestTargetActor.begin(); iter != s_nearestTargetActor.end(); ++iter )
		{
			if( *iterList == (*iter)->GetID() )
			{
				bFind = true;
				break;
			}
		}
		if( !bFind )
		{
			iterList = s_preSelectList.erase( iterList );
			continue;
		}

		++iterList;
	}

	/// 如果有以自己作为目标，优先选择
	if (!s_nearestTargetActor.empty())
	{
		pPreActor = s_nearestTargetActor.front();
		while (!s_nearestTargetActor.empty())
		{
			pActor = s_nearestTargetActor.front();
			if (s_preSelectList.find(pActor->GetID()) == s_preSelectList.end())
			{
				break;
			}
			pActor = NULL;
			std::pop_heap( s_nearestTargetActor.begin(), s_nearestTargetActor.end(), m_heapCmp );
			s_nearestTargetActor.pop_back();
		}
	}
	if (pActor == NULL && !s_nearestActor.empty())
	{
		if (pPreActor == NULL)
		{
			pPreActor = s_nearestActor.front();
		}
		while (!s_nearestActor.empty())
		{
			pActor = s_nearestActor.front();
			if (s_preSelectList.find(pActor->GetID()) == s_preSelectList.end())
			{
				break;
			}
			pActor = NULL;
			std::pop_heap( s_nearestActor.begin(), s_nearestActor.end(), m_heapCmp );
			s_nearestActor.pop_back();
		}
	}

	if (pActor == NULL)
	{
		pActor = pPreActor;
		s_preSelectList.clear();
	}

	if (pActor != NULL)
	{
		s_preSelectList.insert(pActor->GetID());
	}

	return pActor;
}

void ActorManager::looseItemShow()
{
	typedef std::pair<int, int>				Range;
	typedef std::pair<Range, const char *>	PathMap;

#define DEF(x, y, z)	PathMap(Range(x, y), z)
	static const PathMap Id2Path[] = {
		DEF(5011000, 5013000, "sounds\\diaoluo\\5zawudiaoluo.wav"),

		DEF(1500000, 1510000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1010000, 1020000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1510000, 1520000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1030000, 1040000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1200000, 1210000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1250000, 1260000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1100000, 1110000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1420000, 1430000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1280000, 1290000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1180000, 1190000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1580000, 1590000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),
		DEF(1060000, 1070000, "sounds\\diaoluo\\1wuqidiaoluo.wav"),

		DEF(2140000, 2150000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2110000, 2120000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2120000, 2130000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2130000, 2140000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2040000, 2050000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2010000, 2020000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2020000, 2030000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2030000, 2040000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2300000, 2310000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2440000, 2450000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2410000, 2420000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2420000, 2430000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2430000, 2440000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(1270000, 1280000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2210000, 2220000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2220000, 2230000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2240000, 2250000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2710000, 2720000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2720000, 2730000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2730000, 2740000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2740000, 2750000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2910000, 2920000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2920000, 2930000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2930000, 2940000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2940000, 2950000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2610000, 2620000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2620000, 2630000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2630000, 2640000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2640000, 2650000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2810000, 2820000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2820000, 2830000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2830000, 2840000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),
		DEF(2840000, 2850000, "sounds\\diaoluo\\2zhuangbeidiaoluo.wav"),

		DEF(5210000, 5220000, "sounds\\diaoluo\\3baoshidiaoluo.wav"),
		DEF(5000000, 5010000, "sounds\\diaoluo\\4yaopingdiaoluo.wav"),
	};
#undef DEF

	static unsigned int lastLooseTime = 0;
	static unsigned int lastLooseCount = 0;

	unsigned int now = timeGetTime();
	while (m_ItemTimeArray.size() > 0)
	{
		if (m_ItemTimeArray.front().first > now)
		{
			break;
		}
		const tagLoosItemPosInfo &info = m_ItemTimeArray.front().second;
		m_ItemDropCount[info.m_nFromID] = m_ItemDropCount[info.m_nFromID] - 1;
		GameActor* item = FindActor(info.m_nID);
		if( item != NULL && m_pMainPlayer != NULL )
		{
			item->OnEnterMap( m_pMainPlayer->getCurMap() );
			item->SetPosition(info.m_nsX, info.m_nsY);
			item->SetFaceTo(info.m_nX, info.m_nY, info.m_nsX, info.m_nsY, true);
			item->playAction(99999);
			if (lastLooseTime != now)
			{
				lastLooseTime = now;
				lastLooseCount = 0;
			}

			if (++lastLooseCount <= 5)
			{
				for (int i = 0; i < sizeof(Id2Path) / sizeof(PathMap); ++i)
				{
					int itemid = item->getRealId();
					if (itemid >= Id2Path[i].first.first && itemid < Id2Path[i].first.second)
					{
						Ogre::SoundSystem::getSingleton().playSound2D(Id2Path[i].second, 1);
						break;
					}
				}
			}
			int x = info.m_nsX - info.m_nX;
			int y = info.m_nsY - info.m_nY;
			switch (info.m_droptype)
			{
			case VIEW_DROP_TYPE_SINGLY:
				item->doBack(0, info.m_nID, info.m_nX, info.m_nY, 1000, BACK_PARABOLA, 5000 - 20000000 / (10000 + (x * x + y * y)), 0);
				break;
			case VIEW_DROP_TYPE_ROLL:
				item->doBack(0, info.m_nID, info.m_nX, info.m_nY, 1000, BACK_LINE, 0, 0);
				break;
			default:
				item->SetPosition(info.m_nX, info.m_nY);
				item->doBack(0, info.m_nID, info.m_nX, info.m_nY, 700, BACK_CURPOST, 3000, 0);
			}
		}
		std::pop_heap(m_ItemTimeArray.begin(), m_ItemTimeArray.end(), TimeHeapCmp<tagLoosItemPosInfo>());
		m_ItemTimeArray.pop_back();
	}
}

int ActorManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	switch(msgid)
	{
	case MOVE_STOP:
		{			
			const CSMOVESTOP &stop = msgbody.MoveStop;
			if( m_pMainPlayer == NULL )
			{
				break;
			}
			m_pMainPlayer->setMoveSeq(stop.MoveSeq);
			if( m_pMainPlayer->isInFly() )
			{
				break;
			}
			bool bContinue = m_pMainPlayer->getAutoFind();
			
			m_pMainPlayer->setMoveSeq(stop.MoveSeq);
			if (m_pMainPlayer->getActState().GetState() == GSACT_STALL)
			{
				m_pMainPlayer->onStop(stop.x, stop.y, GSACT_STALL);
			}
			else
			{
				m_pMainPlayer->onStop(stop.x, stop.y);
			}

			int iAngle = stop.Dir;
			//m_pMainPlayer->SetFaceTo( iAngle * 2 + 90, true );
			ServerTimer::getSingleton().updateServerBenchTime(stop.BenchTimeMs);

			if (bContinue)
			{
				GameMsgPrint::getSingleton().SendUIEvent( "GE_CHECK_MULTIMAP_AUTOFIND" );
			}
		}
		break;
	case MOVE_OTHER:
		{
			const float TurnDelay = 0.2f;
			const float MaxDelay = 2.0f;
			const double MaxFast = 1.2;

			const CSMOVEOTHER &othermove = msgbody.MoveOther;
			GameActor *pactor = FindActor(othermove.ID);
			if( pactor == NULL )
			{
				Bullet *bullet = BulletManager::getSingleton().FindBullet(othermove.ID);
				if (bullet != NULL)
				{
					if (othermove.MoveType == CS_MOVE_WALK)
					{
						GameMaskPath oldpath, path;
						//在路径点中，添加自己的起点
						oldpath.addPathPoint(othermove.x, othermove.y);
						for( int i=0; i<othermove.Data.Walk.Total; i++ )
						{
							oldpath.addPathPoint( othermove.Data.Walk.Points[i].x, othermove.Data.Walk.Points[i].y );
						}

						double oldLength, newLength;
						oldLength = oldpath.getPathLength();

						const GameXYZ &pos = bullet->GetPosition();
						path.addPathPoint(pos.x / 10, pos.z / 10);
						for (size_t i = 1; i < oldpath.getNumPoint(); ++i)
						{
							Ogre::Point2D point = oldpath.getPathPoint(i);
							path.addPathPoint(point.x, point.y);
						}
						newLength = path.getPathLength();

						if (oldLength == 0)
						{
							bullet->OnPathMove(float(newLength / TurnDelay), path);
						}
						else
						{
							bullet->OnPathMove(float(othermove.MspdInfo.MoveSpdCurr * newLength / oldLength), path);
						}
					}
					else
					{
						const GameXYZ &pos = bullet->GetPosition();
						GameMaskPath path;
						path.addPathPoint(pos.x / 10, pos.z / 10);
						path.addPathPoint(othermove.x, othermove.y);
						bullet->OnPathMove((float)path.getPathLength() / TurnDelay, path);
					}
				}
				break;
			}
			if( pactor->isOnMonsterMachine() )
			{
				break;
			}
			pactor->setSpeed(othermove.MspdInfo.MoveSpdCurr);
			if( othermove.MspdInfo.NextNum > 0 )
			{
				pactor->m_BufStatus.setNextSpeed( othermove.MspdInfo.MoveSpdNext );
			}
			if( othermove.MoveType == CS_MOVE_WALK )
			{
				if( othermove.Data.Walk.Total == 0 )
				{
					break;
				}
				GameMaskPath path;
				//怪物
				if( pactor->getType() == GAT_MONSTER )
				{
					if( pactor->getSubType() == MON_SUB_TYPE_MACHINE 
						&& pactor->m_MonsterMachine.isMainPlayerDriver() && pactor->m_nPatrolID <= 0 )
					{
						break;
					}
					GameMaskPath oldpath;
					oldpath.reset( pactor->getCurMap()->getMaskMap() );
					//在路径点中，添加自己的起点
					oldpath.addPathPoint(othermove.x, othermove.y);
					if( pactor->GetPosition().x <= 0 || pactor->GetPosition().z <= 0 )
					{
						LOG_SEVERE("wrong actor pos,x is %d,y is %d",pactor->GetPosition().x,pactor->GetPosition().z );
					}
					for( int i=0; i<othermove.Data.Walk.Total; i++ )
					{
						oldpath.addPathPoint( othermove.Data.Walk.Points[i].x, othermove.Data.Walk.Points[i].y );
					}

					double oldLength, newLength;
					oldLength = oldpath.getPathLength();

					////检测这个路径上是否有障碍
					//if( !oldpath.isValidPath() )
					//{
					//	Point2D begin( pactor->GetPosition().x/10,pactor->GetPosition().z/10 );
					//	Point2D end( othermove.Data.Walk.Points[othermove.Data.Walk.Total-1].x
					//				,othermove.Data.Walk.Points[othermove.Data.Walk.Total-1].y );
					//	//寻路失败时，不会清除原path中的路径。
					//	pactor->getCurMap()->FindPath( oldpath, begin.x, begin.y, end.x, end.y, MASK_STOP | DYN_MASK_STOP, false );
					//}

					int cur_x, cur_z;
					pactor->m_pMapMover->GetCurPos(cur_x, cur_z);
					path.addPathPoint(cur_x, cur_z);
					for (size_t i = 1; i < oldpath.getNumPoint(); ++i)
					{
						Ogre::Point2D point = oldpath.getPathPoint(i);
						path.addPathPoint(point.x, point.y);
					}
					newLength = path.getPathLength();

					if (oldLength == 0)
					{
						pactor->m_pMapMover->SetFixSpeed(float(newLength / TurnDelay));
					}
					else
					{
						pactor->m_pMapMover->SetFixSpeed(float(othermove.MspdInfo.MoveSpdCurr * newLength / oldLength));
					}
				}
				else
				{
					GameMaskPath oldpath;
					oldpath.reset( pactor->getCurMap()->getMaskMap() );
					int cur_x, cur_z;
					pactor->m_pMapMover->GetCurPos(cur_x, cur_z);
					path.addPathPoint(cur_x, cur_z);
					oldpath.addPathPoint(othermove.x, othermove.y);
					for(int i=0; i<othermove.Data.Walk.Total; i++)
					{
						oldpath.addPathPoint(othermove.Data.Walk.Points[i].x, othermove.Data.Walk.Points[i].y);
					}
					//如果是乘坐在机械上的话，特别处理下
					if( pactor->isOnMonsterMachine() )
					{
						for (size_t i = 1; i < oldpath.getNumPoint(); ++i)
						{
							Ogre::Point2D point = oldpath.getPathPoint(i);
							path.addPathPoint(point.x, point.y);
						}
					}
					else
					{
						//其他玩家
						path.append(oldpath);
					}

					double oldLength, newLength;
					oldLength = oldpath.getPathLength();
					newLength = path.getPathLength();
					if (oldLength == 0)
					{
						pactor->m_pMapMover->SetFixSpeed(float(newLength / TurnDelay));
					}
					else
					{
						double oldTime = oldLength / othermove.MspdInfo.MoveSpdCurr;
						double newSpeed = newLength / oldTime;
						double maxSpeed = othermove.MspdInfo.MoveSpdCurr * MaxFast;
						if (newSpeed > maxSpeed)
						{
							newSpeed = maxSpeed;
						}
						double newTime = newLength / newSpeed;
						if (newTime - oldTime > MaxDelay)
						{
							path = oldpath;
							pactor->SetPosition(othermove.x, othermove.y);
							pactor->m_pMapMover->SetFixSpeed(0);
						}
						else
						{
							pactor->m_pMapMover->SetFixSpeed(float(newSpeed));
						}
					}
				}

				pactor->OnPathMove( &path );
				for( int j = 0; j < PET_MAX_FIGHT_SHI; j ++ )
				{
					GameActor* pActor = pactor->m_Pet.getPetActor( j );
					if( pActor != NULL )
					{
						pActor->m_Pet.setMoveToPoint( othermove.Data.Walk.Points[othermove.Data.Walk.Total-1].x, 
							othermove.Data.Walk.Points[othermove.Data.Walk.Total-1].y );
					}
				}
			}
			else
			{
				pactor->onTurn(othermove.x, othermove.y);
			}
		}
		break;
	case ENTER_VIEW:
		{
			int npc_res_id		= 310001;
			int player_res_id	= 2;
			const char *pactorname = "";
			int nArmStar = 0;
			char szName[256];
			int actorstate = 0;
			int monsterLv = 0;
			if( m_pMainPlayer == NULL || m_pMainPlayer->getCurMap() == NULL )
			{
				break;
			}

			const CSENTERVIEW &enterview = msgbody.EnterView;
			WorldCityManager::getSingleton().UnMarkCityBuildFight(enterview.ID);
			GameActor *pactor = FindActor(enterview.ID);
			if(pactor == NULL)
			{
				GameActorType actortype = GAT_PLAYER;
				int resid = player_res_id;

				if(enterview.Type == OBJ_NPC)
				{
					actortype = GAT_NPC;
					resid = enterview.Object.Npc.Index;
					const tagNpcDisplayInfo &npcdef = DBDataManager::getSingleton().GetNpcDisplayInfo(enterview.Object.Npc.Index);
					pactorname	= npcdef.name;
				}
				else if(enterview.Type == OBJ_MONSTER)
				{
					actortype = GAT_MONSTER;
					resid = enterview.Object.Monster.Index;
					const tagMonsterDef &mondef = DBDataManager::getSingleton().getMonsterDef(resid);
					monsterLv  = mondef.Level;
					if( enterview.Object.Monster.OwnerName[0] == 0)
					{
						if(mondef.MiscType & MON_MISC_TYPE_WORD_PWD)
							pactorname = "";
						else
							pactorname = mondef.Name;
					}

					actorstate = enterview.Object.Monster.Stat;
				 }
				else if(enterview.Type == OBJ_ITEM)
				{
					actortype = GAT_ITEM;
					const tagItemDef &itemdef = DBDataManager::getSingleton().getItemDef(enterview.Object.Item.Index);
					if( enterview.Object.Item.OwnerName[0] == 0 )
					{
						pactorname = itemdef.Name;
					}
					nArmStar = enterview.Object.Item.ArmStar;
					resid = enterview.Object.Item.Index;
					if( resid != 9900001 && resid != 9900002 && (itemdef.ItemTmp.Type == ITEM_TMP_NO || enterview.Object.Item.DropItemType == DROP_ITEM_GRID) )
					{
						resid = Item::getDisplayIndex(resid);
					}
				}
				else
				{
					assert(enterview.Type == OBJ_PLAYER);
					actortype = GAT_PLAYER;
					const CSPLAYER &obj = enterview.Object.Player;
					resid = getPlayerDisplayId( obj.Gender, obj.Career );

					pactorname = obj.Name;
					actorstate = obj.Stat;
				}

				if (enterview.Type == OBJ_MONSTER)
				{
					const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(resid);
					if (MonsterDef.SubType == MON_SUB_TYPE_SKILL_DESTORY || MonsterDef.SubType == MON_SUB_TYPE_SKILL_NODESTORY)
					{
						Bullet *bullet = BulletManager::getSingleton().CreateBullet(enterview.ID, resid, enterview.Object.Monster.OwnerID, enterview.x, enterview.y);
						bullet->SetFaceTo(enterview.Dir * 2 + 90);
						break;
					}
				}

				pactor = CreateActor( enterview.ID, actortype, resid, true );
				if (Root::getSingleton().getActorTransProcess())
				{
					addActorFade(pactor, true);
				}
				pactor->setName( pactorname/*replaceSpecialCode( pactorname ).c_str()*/ );
				pactor->setArmStar( nArmStar );
				pactor->setType( actortype );
				if( enterview.Type != OBJ_ITEM )
				{
					pactor->OnEnterMap( m_pMainPlayer->getCurMap() );
					pactor->SetPosition( enterview.x, enterview.y );
					int iAngle = enterview.Dir;
					pactor->SetFaceTo( iAngle * 2 + 90, true );
				}else{
					const CSDROPITEM& dropItemInfo = enterview.Object.Item;
					const tagItemDef& itemdef = DBDataManager::getSingleton().getItemDef( dropItemInfo.Index );
					pactor->setOwnerName( "" );
					pactor->setRollType( enterview.Object.Item.Roll );
					pactor->m_CollectTime = dropItemInfo.CollectTime;
					pactor->m_IsPickItem = (dropItemInfo.DropItemType == DROP_ITEM_GRID);
					if( enterview.Object.Item.OwnerName[0] != 0 )
					{
						sprintf( szName, "%s的%s", enterview.Object.Item.OwnerName, itemdef.Name );
						pactor->setOwnerName( enterview.Object.Item.OwnerName );
						pactor->setName( szName );
					}
					if( enterview.Object.Item.Flag == 1 &&  itemdef.ItemTmp.Type == ITEM_TMP_NO )
					{
						tagLoosItemPosInfo looseItemInfo;
						looseItemInfo.m_nID				= pactor->GetID();
						looseItemInfo.m_nFromID			= enterview.Object.Item.FromID;
						looseItemInfo.m_nX				= enterview.x;
						looseItemInfo.m_nY				= enterview.y;
						looseItemInfo.m_droptype		= enterview.Object.Item.DropType;
						GameActor *pMonster = FindActor(enterview.Object.Item.FromID);						
						if (enterview.Object.Item.ShowFlag == 0 && pMonster
							&& (pMonster->getType() != GAT_PLAYER || pMonster->getType() == GAT_MAINPLAYER)
							&& pMonster->getActState().GetState() != GSACT_DEAD)
						{
							GameXYZ pos = pMonster->GetPosition();
							looseItemInfo.m_nsX = pos.x / 10;
							looseItemInfo.m_nsY = pos.z / 10;
							pMonster->setDeathAction(new LooseItemShow(looseItemInfo));
						}
						else
						{
							if (pMonster)
							{
								GameXYZ pos = pMonster->GetPosition();
								looseItemInfo.m_nsX = pos.x / 10;
								looseItemInfo.m_nsY = pos.z / 10;
							}
							else
							{
								looseItemInfo.m_nsX = enterview.x;
								looseItemInfo.m_nsY = enterview.y;
							}
							addLooseItemShow(looseItemInfo);
						}
						if( enterview.Object.Item.RoleID == m_pMainPlayer->getRoleWId() || enterview.Object.Item.RoleID == 0 )
						{
							char buf[256];
							sprintf( buf,"AutoAssistPick( %d ,%d)",enterview.Object.Item.Index,enterview.ID);
							ActorManager::getSingleton().getScriptVM()->callString(buf);
						}
					}else{
						pactor->ChangeActState( GSACT_STAND );
						if ( itemdef.ItemTmp.Type == ITEM_TMP_MACHINE )
						{
							if ( dropItemInfo.TmpEnerge < 0 )
							{
								pactor->getShowActor()->getCurEntity()->playMotion( "9593_16", false, 9593 );
							}
// 							else if( pactor->getResId() == 1460001 )
// 							{
// 								pactor->getShowActor()->getCurEntity()->playMotion( "jiantou", false, 0 );
// 							}
						}
					}
				}

				if( enterview.Type == OBJ_MONSTER )
				{
					resid = enterview.Object.Monster.Index;
					const tagMonsterDef &mondef = DBDataManager::getSingleton().getMonsterDef(resid);
					if (enterview.Object.Monster.TopName[0] != '\0')
					{
						((GameNPC *)pactor)->m_strNpcFunctionDes = enterview.Object.Monster.TopName;
					}
					pactor->m_BufStatus.setBufStatus( enterview.Object.Monster.StatusInfo );
					const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pactor->getResId());
					if (info.PoseType == 0)
					{
						pactor->getActState().setPoseState( POSE_NONE );
					}
					else
					{
						pactor->getActState().setPoseState( info.PoseType );
					}
					pactor->setLv( enterview.Object.Monster.Level );

					const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pactor->getResId() );
					
					pactor->reSetHp( (int)enterview.Object.Monster.HP );
					pactor->setMaxHP( (int)enterview.Object.Monster.HPMax );
					pactor->setAtkType();
					pactor->getShowActor()->getCurEntity()->setPostSceneRenderer( ActorManager::getSingleton().getPostSceneRenderer() );
					pactor->setOwnerFlag( 0 );
					pactor->setOwnerName( "" );
					pactor->setOwnerId( enterview.Object.Monster.OwnerID );
					pactor->m_nPatrolID = enterview.Object.Monster.PatrolID;
					static const int idx[] = {
						EQUIP_POS_BODY,
						EQUIP_POS_RIGHTHAND,
						EQUIP_POS_LEFT,
						EQUIP_POS_HEAD,
						EQUIP_POS_SHOULDER,
						EQUIP_POS_BACK,
						EQUIP_POS_FASHION_BODY,
						EQUIP_POS_FACE,
					};
					const tagNpcDisplayInfo &npcDisplayerInfo = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
					if (npcDisplayerInfo.model[0] == 0)
					{
						for (int i = 0; i < sizeof(idx) / sizeof(int); i++)
						{
							int itemid = Equip::getMonsterEquip(resid, idx[i]);
							if (itemid)
							{
								pactor->m_Equip.m_ArmEquipId[ idx[i] ] = itemid;
								pactor->m_Equip.changEquip(idx[i], itemid,0);
							}
						}
					}
					if (pactor->getAvatar())
					{
						pactor->getAvatar()->updateAttrib();
					}
					pactor->setTeamWID(enterview.Object.Monster.OwnerTeamID);
					if( enterview.Object.Monster.OwnerName[0] != 0 && enterview.Object.Monster.Stat != CS_STAT_DEAD )
					{
						const tagMonsterDef& mondef = DBDataManager::getSingleton().getMonsterDef(resid);
						GameActor* pActor = findActorByName( enterview.Object.Monster.OwnerName );
						if ( pActor == m_pMainPlayer && enterview.Object.Monster.MachineFlag )
						{
							m_pMainPlayer->addOwnMonster( enterview.ID, resid );
						}
						if( pActor == m_pMainPlayer && mondef.MiscType & MON_MISC_TYPE_COW )
						{
							pactor->playMotion("9622_16");
						}
// 						// 判断是组队怪物还是个人怪物
// 						if ( mondef.SubType == MON_SUB_TYPE_SINGLE_CAR || 
// 								mondef.SubType == MON_SUB_TYPE_SINGLE_SAFECAR )
// 						{
// 							// 判断拥有者是否是主角
// 							GameActor* pActor = findActorByName( enterview.Object.Monster.OwnerName );
// 							if ( pActor == m_pMainPlayer )
// 							{
// 								m_pMainPlayer->addOwnMonster( enterview.ID, resid );
// 							}
// 						}
// 						else if ( mondef.SubType == MON_SUB_TYPE_TEAM_CAR ||  
// 									mondef.SubType == MON_SUB_TYPE_TEAM_SAFECAR )
// 						{
// 							// 若是主角的队伍
// 							if ( enterview.Object.Monster.OwnerTeamID == m_pMainPlayer->getTeamWID() )
// 							{
// 								m_pMainPlayer->addOwnMonster( enterview.ID, resid );
// 							}
// 							
// 							pactor->setOwnerTeamWID( enterview.Object.Monster.OwnerTeamID );
// 						}
// 						//sprintf( szName,"%s\r的%s",enterview.Object.Monster.OwnerName,mondef.Name );
						pactor->setOwnerFlag( 1 );
						pactor->setOwnerName( enterview.Object.Monster.OwnerName );
						pactor->setName( mondef.Name );
					}
					if (monsterDef.DynMask)
					{
						GameMaskMap *maskMap = m_pMainPlayer->getCurMap()->getMaskMap();
						GameXYZ pos = pactor->GetPosition();
						maskMap->setDynMsk(pos.x / 10, pos.z / 10,
							monsterDef.DynMaskX == 0 ? monsterDef.Radius : monsterDef.DynMaskX,
							monsterDef.DynMaskY == 0 ? monsterDef.Radius : monsterDef.DynMaskY,
							pactor->getAngle(), monsterDef.DynMask);
					}
					pactor->m_nCampIdx = enterview.Object.Monster.CampIdx;
				}else if( enterview.Type == OBJ_PLAYER ){
					pactor->m_Gender	= enterview.Object.Player.Gender;
					pactor->m_Race		= enterview.Object.Player.Career;
					pactor->m_Head		= enterview.Object.Player.Head;
					pactor->m_Hair		= enterview.Object.Player.Hair;
					pactor->m_Face		= enterview.Object.Player.Face;
					
					if (( enterview.Object.Player.ViewFlag & VIEW_FLAG_CAPTAIN ))
					{
						pactor->setCaptain(true);
					}
					if ( ( enterview.Object.Player.ViewFlag & VIEW_FLAG_SPY ) )
					{
						pactor->m_ViewFlag |= VIEW_FLAG_SPY;
					}
					else
					{
						pactor->m_ViewFlag &= ~VIEW_FLAG_SPY;
					}
					if ( ( enterview.Object.Player.ViewFlag & VIEW_FLAG_ANTI_SPY ) )
					{
						pactor->m_ViewFlag |= VIEW_FLAG_ANTI_SPY;
					}
					else
					{
						pactor->m_ViewFlag &= ~VIEW_FLAG_ANTI_SPY;
					}
					if( enterview.Object.Player.ViewFlag & VIEW_FLAG_SAFE )
					{
						pactor->m_nSafeOverlayFlag |= SCENEOVERLAY_SAFE;
					}else{
						pactor->m_nSafeOverlayFlag = 0;
					}
					if( enterview.Object.Player.ViewFlag & VIEW_FLAG_PK )
					{
						pactor->m_nSafeOverlayFlag |= SCENEOVERLAY_PK;
					}
					
					pactor->setTeamWID( enterview.Object.Player.TeamID );
					pactor->setTeamName( enterview.Object.Player.TeamName );
					pactor->setViewFlg( pactor->m_ViewFlag );
					pactor->setLv( (int)enterview.Object.Player.Level );
					pactor->reSetHp( (int)enterview.Object.Player.HP );
					pactor->setMaxHP( (int)enterview.Object.Player.HPMax );
					pactor->setMP( (int)enterview.Object.Player.MP );
					pactor->setMaxMP( (int)enterview.Object.Player.MPMax );
					GameMsgPrint::getSingleton().updateActorAttr( pactor );
					
					pactor->setDaoBaClanWid(enterview.Object.Player.DaoBa.ClanWid);
					pactor->setDaoBaEnd(enterview.Object.Player.DaoBa.End);
					pactor->setDaoBaName(enterview.Object.Player.DaoBa.Name);
					pactor->setSpartaFightVal( enterview.Object.Player.SpartaFightVal );
					pactor->m_Equip.setSelectFashionBag( enterview.Object.Player.Avatar.SelectFashion,enterview.Object.Player.Avatar.AddFashion );
					if (pactor->getAvatar())
					{
						pactor->getAvatar()->equipItem( POS_ID_HAIR, enterview.Object.Player.Hair + 1, 0 );	
						pactor->getAvatar()->equipItem( POS_ID_FACE, enterview.Object.Player.Face, 0 );	
						pactor->getAvatar()->equipItem( POS_ID_HEAD, enterview.Object.Player.Head, 0 );	
					}
					
					// 设置玩家头顶称号
					if( enterview.Object.Player.DesignationID != 0 )
					{
						const char* szDesignationName = DBDataManager::getSingleton().getDesignationDef( 
														enterview.Object.Player.DesignationID ).Name;
						pactor->setDesignation( szDesignationName );
						pactor->setDesignationID( enterview.Object.Player.DesignationID );
					}

					for( int i = 0; i < enterview.Object.Player.Avatar.Num; i++ )
					{
						pactor->m_Equip.m_ArmEquipId[enterview.Object.Player.Avatar.Arms[i].GridIdx] 
						= enterview.Object.Player.Avatar.Arms[i].ArmDefID;
						pactor->m_Equip.changEquip( enterview.Object.Player.Avatar.Arms[i].GridIdx,
							enterview.Object.Player.Avatar.Arms[i].ArmDefID,
							enterview.Object.Player.Avatar.Arms[i].StarLv );
					}

					pactor->m_Equip.setTmpArm( enterview.Object.Player.TmpItem );
					if (pactor->getAvatar())
					{
						pactor->getAvatar()->updateAttrib();
					}
					//接着处理下背饰信息,写在这里是不想改变属性，只想改变显示
					for( int i = 0; i < enterview.Object.Player.Avatar.Num; i++ )
					{
						if( enterview.Object.Player.Avatar.BackModelID > 0 && enterview.Object.Player.Avatar.Arms[i].GridIdx == EQUIP_POS_FASHION_BACK  )
						{
							pactor->m_Equip.changEquip( enterview.Object.Player.Avatar.Arms[i].GridIdx,
								enterview.Object.Player.Avatar.BackModelID,
								enterview.Object.Player.Avatar.Arms[i].StarLv );
						}
					}
					pactor->ChangeActState(GSACT_STAND);
					if( ( enterview.Object.Player.Stat & CS_STAT_STALL ) != 0 )
					{
						pactor->ChangeActState( GSACT_STALL );
						pactor->OnStall(true);						
					}
					if( enterview.Object.Player.ShowFlag & 16 )
					{
						pactor->m_nShowFlag |= SHOW_WEDDING_DSN_FLAG;
					}
					pactor->setStallName( enterview.Object.Player.MiscName );
				
					//如果在海滩地图的时候，显示时装
					if( pactor->getCurMap() && pactor->getCurMap()->getMapId() == BEACH_MAP_ID )
					{
						if(pactor->getAvatar())
						{
							pactor->getAvatar()->setAvatarAttrib(POS_ID_FASHION_HAIR,1);
						}						
					}

					// 副本需要变身的，把进入视野的玩家状态切换成变身状态
					if( m_pMainPlayer->getCurMap()->getMapId() >= GAME_PWORLD_MAPID )
					{
						int nPworldId = pactor->getCurMap()->getMapId()/100;
						const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
						if( pPworldDef != NULL )
						{
							if( pPworldDef->PlayerModeID != 0 )
							{
								pactor->onMorph( pPworldDef->PlayerModeID );	
							}
						}
					}
					pactor->m_BufStatus.setBufStatus( enterview.Object.Player.StatusInfo );
					pactor->m_Equip.m_TmpEquip = enterview.Object.Player.TmpItem; 
					int iAngle = enterview.Dir;
					pactor->SetFaceTo( iAngle * 2 + 90, true );
					pactor->m_nCampIdx = enterview.Object.Player.CampIdx;
				}else if( enterview.Type == OBJ_NPC )
				{
					const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( pactor->getResId() );
					pactor->setMaxHP( monsterDef.HP );
					pactor->reSetHp( monsterDef.HP );
					pactor->setLv( monsterDef.Level );
					if (pactor->getAvatar())
					{
						pactor->getAvatar()->updateAttrib();
					}
					pactor->m_nCampIdx = enterview.Object.Npc.CampIdx;
				}

				pactor->setSubType();
				pactor->setAtkType();
		
				if( actorstate & CS_STAT_DEAD )
				{
					pactor->onChangeBone();
				}else{
					if( enterview.Type != OBJ_ITEM )
					{
						// TODO::change state...
						if( enterview.Type != OBJ_PLAYER )
						{
							pactor->ChangeActState(GSACT_STAND);
						}
						if( actorstate & CS_STAT_SIT )
						{
							pactor->ChangeActState( GSACT_SIT );
							pactor->playAction( 10200, NULL, 0, 0 );
							pactor->m_Equip.hideWeapon();
						}
					}
				}
// 				if ( pactor->getType() == GAT_NPC )
// 				{
// 					char szScript[256];
// 					sprintf( szScript, "ScirptCreateActor(%d,%d)", pactor->GetID(), pactor->getType() );
// 					m_pScriptVM->callString(szScript);
// 				}
			}

			if( enterview.Type != OBJ_ITEM )
			{
				const char* pszOwnerName = enterview.Object.Monster.OwnerName;
				if ( enterview.Type == OBJ_MONSTER && getShowOtherPlayerModelOption() != HIDE_MODEL_NONE && !isActorIgnoreF12(pactor) )
				{
					const CSMONSTER& monsterInfo	= enterview.Object.Monster;
					tdr_ulonglong ownerTeamWID		= monsterInfo.OwnerTeamID;
					if (  ( ownerTeamWID == 0 && strcmp( pszOwnerName, m_pMainPlayer->getName() ) != 0 ) || 
							!m_pMainPlayer->isInSameTeam( ownerTeamWID ) )
					{
						Ogre::ShowActor* pMonsterShowActor = pactor->getShowActor();
						GameMap* pMapMgr = pactor->getCurMap();
						if ( pMapMgr != NULL )
						{
							pMonsterShowActor->onLeaveMap( pMapMgr->getShowGameMap() );
							pactor->m_pSmallDecal->show(false);
						}
					}
				}
				else
				{
					pactor->OnEnterMap( m_pMainPlayer->getCurMap() );
					pactor->SetPosition( enterview.x, enterview.y );
					int iAngle = enterview.Dir;
					pactor->SetFaceTo( iAngle * 2 + 90, true );
				}
			}
			else
			{
				pactor->setRoleWId(0);
				pactor->setItemNum(enterview.Object.Item.ItemNum);
				pactor->setRealId(enterview.Object.Item.Index);
				if (!pactor->isSameRoleWId(ActorManager::getSingleton().getMainPlayer()))
				{
					if( enterview.Object.Item.RoleID == 0 )
					{
						pactor->setTeamWID( enterview.Object.Item.TeamID );
						pactor->setClanWID( enterview.Object.Item.ClanGID );
						setItemChangeTime(enterview.Object.Item.PickLimitTime * 1000, pactor->GetID());											
					}else{
						if (m_pMainPlayer->getRoleWId() == enterview.Object.Item.RoleID)
						{
							setItemChangeTime(0, pactor->GetID());
						}
						else
						{
							setItemChangeTime(enterview.Object.Item.PickLimitTime * 1000, pactor->GetID());
						}
					}					
				}
				const tagItemDef& itemdef = DBDataManager::getSingleton().getItemDef( enterview.Object.Item.Index );
				if( enterview.Object.Item.Flag != 1 || itemdef.ItemTmp.Type != ITEM_TMP_NO )
				{
					pactor->OnEnterMap( m_pMainPlayer->getCurMap() );
					pactor->SetPosition( enterview.x, enterview.y );
					int iAngle = enterview.Dir;
					pactor->SetFaceTo( iAngle * 2 + 90, true );
				}
			}

			if ( enterview.Type == OBJ_NPC )
			{
				GameMsgPrint::getSingleton().npcEnterView( enterview.ID );
				pactor->m_ObjCamp = SKILL_TARGET_FRIEND_MON;
				((GameNPC*)pactor)->m_bIsPatrol = enterview.Object.Npc.PatrolID >0?true:false;
				pactor->m_nCampIdx = enterview.Object.Npc.CampIdx;
				pactor->m_ViewBanner.GloryEffect = enterview.Object.Npc.BannerInfo.GloryEffect;
				pactor->m_ViewBanner.Sign = enterview.Object.Npc.BannerInfo.Sign;
				pactor->m_ViewBanner.Back = enterview.Object.Npc.BannerInfo.Back;
				pactor->m_ViewBanner.Style = enterview.Object.Npc.BannerInfo.Style;
				sprintf( pactor->m_ViewBanner.XuanYan, "%s", enterview.Object.Npc.BannerInfo.XuanYan );
				pactor->setBannerInfo();
				pactor->m_PKInfoType = 0;
				pactor->setClanWID( 0 );
				pactor->m_PKInfoType = MON_PK_TYPE_NONE;
				if (enterview.Object.Npc.MonPkFlag)
				{
					pactor->m_PKInfoType = enterview.Object.Npc.MonPkModeChg.PKInfoType;
					pactor->m_nPkMode = enterview.Object.Npc.MonPkModeChg.OwnerPKMode;
					pactor->setClanWID(enterview.Object.Npc.MonPkModeChg.OwnerClanID);
					switch (enterview.Object.Npc.MonPkModeChg.PKInfoType)
					{
					case MON_PK_TYPE_PLAYER:
						{
							pactor->setOwnerId(int(enterview.Object.Npc.MonPkModeChg.PkID));
						}
						break;

					case MON_PK_TYPE_TEAM:
						{
							pactor->setTeamWID(enterview.Object.Npc.MonPkModeChg.PkID);
						}
						break;

					case MON_PK_TYPE_CLAN:
						{
							pactor->setClanWID(enterview.Object.Npc.MonPkModeChg.PkID);
						}
						break;

					case MON_PK_TYPE_CAMP:
						{
							pactor->m_nCampIdx = int(enterview.Object.Npc.MonPkModeChg.PkID);
						}
						break;
					}
				}
			}

			if( enterview.Type == OBJ_PLAYER )
			{
				if( pactor->getType() == GAT_MAINPLAYER )
				{
					break;
				}
				if( pactor != NULL )
				{
					if( actorstate& CS_STAT_FIGHT)
					{
						pactor->onFight();
					}

					if (enterview.Object.Player.LeaveState == LEAVE_STAT_ENTER)
					{
						pactor->onAFK(true);
					}

					if( actorstate & CS_STAT_FLY )
					{
						pactor->getActState().SetState( GSACT_STAND );
					}else if( actorstate & CS_STAT_QIANXIN  )
					{
						pactor->getShowActor()->getCurEntity()->setTransparent( 0.5f );
					}
					if( enterview.Object.Player.PrepareInfoFlag )
					{
						//TODO HERE 处理 PrepareInfo
					}

					if ( actorstate& CS_STAT_CAMPAIGN_PRE )
					{
						pactor->startPetChant(0);					
					}
					pactor->m_nPerfectStar	= enterview.Object.Player.Avatar.ArmStar;
					pactor->setPerfectEffect();
					//九星特效
					/*if ( pactor->getWanMeiStarNum() == PERFECT_STAR_NINE )
					{
						pactor->playMotion( "wanmeiX9",false );
					}
					else
					{
						pactor->stopMotion("wanmeiX9");
					}*/
					pactor->m_nPkMode		= enterview.Object.Player.PkInfo.PkMode;
					pactor->m_nPkValue		= enterview.Object.Player.PkInfo.PkVal;
					if (enterview.Object.Player.PkInfo.PkTime > 0)
					{
						pactor->m_leastNoCrimeTime = timeGetTime() + enterview.Object.Player.PkInfo.PkTime * 1000;
					}
					pactor->m_nCampIdx = enterview.Object.Player.CampIdx;
					pactor->m_ObjCamp = SKILL_TARGET_MID_PLAYER;

					const CSROLERIDEVIEW& rideView = enterview.Object.Player.RideView;
					const CSPLAYER& player = enterview.Object.Player;
					// 若进入视野范围的实体处于骑乘状态
					if ( player.RideItemID != 0 )
					{
						// 判断进入视野的实体是骑乘的主人还是客人
						// 若是主人，则直接开始骑坐骑
						if ( pactor->GetID() == rideView.OwnerID )
						{
							pactor->m_Rider.rideOn(player.RideItemID,player.RideLevel, true);
						}
						else
						{
							// 若进入视野的实体是骑乘的客人，判断主人是否已经显示，若主人没显示，则一直等待，直到主人显示后，再开始显示客人
							tagCSRideOnInfo rideInfo;
							rideInfo.OwnerID		= rideView.OwnerID;
							memcpy( rideInfo.PassengerID, rideView.PassengerID, MAX_RIDE_PASSENGER * sizeof( unsigned int ) );
							rideInfo.PassengerNum	= rideView.PassengerNum;
							rideInfo.RideID			= player.RideItemID;
							rideInfo.RideLevel		= player.RideLevel;
							pactor->m_Rider.rideOnSide( rideInfo );
						}
					}

					if( actorstate & CS_STAT_COLLECT )
					{
						pactor->ChangeActState( GSACT_COLLECTITEM );
					}

					int nPetNum = enterview.Object.Player.PetView.PetNum;
					//char petNameBuf[256];
					for (int i = 0; i < PET_MAX_FIGHT_SHI; ++i)
					{
						if (pactor->m_Pet.getPetActor(i) != NULL)
						{
							pactor->m_Pet.setShiModle( 0, i, 0, "", 0, 0, 0, 0, PET_OP_BACK );
						}
					}
					for( int i = 0; i < nPetNum; i++ )
					{
						const CSPETVIEW& petViewData = enterview.Object.Player.PetView.PetView[i];
						std::string strPetName = petViewData.PetName;
						if( strPetName.empty() )
						{
							int id		= petViewData.PetID;
							strPetName	= DBDataManager::getSingleton().getPetDef(id).Name;
						}
						
						pactor->m_Pet.setShiModle( pactor->GetID(), 
							petViewData.XieDaiPos,
							petViewData.PetID,
							strPetName.c_str(),
							petViewData.Power,
							petViewData.MaxPower,
							petViewData.Level,
							petViewData.PetWID,
							PET_OP_FIGHT,
							petViewData.Passive,
							petViewData.DiathesisNum,
							petViewData.Diathesis,
							petViewData.GoodStatusNum,
							petViewData.GoodStatusList,
							petViewData.Genius,
							petViewData.StrengthenLevel,
							petViewData.Zhanchong );
						if (pactor->isDead() || petViewData.HetiFlag != 0)
						{
							pactor->getPet()->getPetActor(enterview.Object.Player.PetView.PetView[i].XieDaiPos)->OnLeaveMap();
							if (petViewData.HetiFlag != 0)
							{
								pactor->getPet()->setPetHeti(petViewData.PetWID, petViewData.Genius, true, petViewData.Zhanchong);
							}
						}
						else
						{
							GameActor* pPet = pactor->getPet()->getPetActor( petViewData.XieDaiPos );
							if ( pPet && petViewData.Zhanchong == 1 )
							{
								pactor->getPet()->setBattlePetEntity( pPet, petViewData.Genius );
							}
						}
					}

					// 放出精灵
					pactor->getTrump()->m_nCurCampFairyId = enterview.Object.Player.FairyView.FairyID;
					pactor->getTrump()->m_nCurCampFairyLv = enterview.Object.Player.FairyView.FairyLV;
					pactor->loadFairyEffect();

					if( ( enterview.Object.Player.Stat & CS_STAT_RIDE_MON_MACHINE ) != 0 )
					{
						const CSROLEMACHINEINFO& machineInfo = enterview.Object.Player.MachineInfo;
						pactor->m_MonsterMachine.setOwner( pactor->GetID() );
						pactor->m_MonsterMachine.rideOnMonsterMachine( machineInfo );			
					}
// 					if( enterview.Object.Player.Stat & CS_STAT_ARM_FENJIE_PRE )
// 					{
// 						pactor->startResolveChant( 0,0,0 );
// 					}

					pactor->m_nClanPosition = enterview.Object.Player.ClanView.Position;
					pactor->m_nClanGroup = enterview.Object.Player.ClanView.Group;
					pactor->setClanName( enterview.Object.Player.ClanView.Name );
					pactor->m_uClanWID		= enterview.Object.Player.ClanView.GID;
					pactor->m_TeamName		= enterview.Object.Player.TeamName;
					pactor->m_nTeamNum		= enterview.Object.Player.TeamNum;
					pactor->setTeamWID( enterview.Object.Player.TeamID );
					pactor->setVipFlag( enterview.Object.Player.VipFlag );
					pactor->setXuKongFlag( enterview.Object.Player.XuKongEnergy );
					pactor->setXuKongEffect();

					if( getShowOtherPlayerModelOption() != HIDE_MODEL_NONE && !m_pMainPlayer->isInSameTeam( pactor->getTeamWID() ) )
					{
						if ( !( getShowOtherPlayerModelOption() == HIDE_OTHER_PLAYER_AND_PET && ( ( enterview.Object.Player.Stat & CS_STAT_STALL ) != 0 ) ) )
						{
							pactor->getShowActor()->onLeaveMap( pactor->getCurMap()->getShowGameMap() );
							pactor->m_pSmallDecal->show(false);
						}
					}

					//切磋标记
					if( enterview.Object.Player.RoleTest )
					{
						if( strcmp( pactorname,m_pMainPlayer->m_szPkTargetName )==0 )
						{
							pactor->getShowActor()->getCurEntity()->playMotion("9596_16",false,9596);
						}
						pactor->m_pkEffect.setPos( enterview.Object.Player.QiecuoX*10,enterview.Object.Player.QiecuoY*10 );
						pactor->m_pkEffect.setState( EFFECT_STATE_LIVE );
					}
					
					const CSPLAYER& playerEnterViewData			= enterview.Object.Player;
					const CSROLEITEMVIEW& itemPreEnterViewData	= playerEnterViewData.ItemView;

					if (itemPreEnterViewData.PreItemID != 0)
					{
						tagItemDef_ToLua* pItemDef = getItemDef( itemPreEnterViewData.PreItemID );
						if( pItemDef->ItemID == itemPreEnterViewData.PreItemID )
						{
							pactor->startUseItemChant(pItemDef->ItemUsePre*1000,
								itemPreEnterViewData.PreItemID,itemPreEnterViewData.PreTargetID);
						}
					}
					
					const tagMotionInfo& motion = enterview.Object.Player.MotionInfo;
					if( motion.MotionID > 0 )
					{						
						char szbuf[256];
						sprintf(szbuf,"ChatActionPlayAction(%d,%d,%d)",motion.MotionID,pactor->GetID(),motion.TargetID);
						ActorManager::getSingleton().getScriptVM()->callString(szbuf);
					}
					pactor->setHitTargetID( enterview.Object.Player.SelectTargetID );
					if( enterview.Object.Player.ShamDead == 1 )
					{
						pactor->ChangeActState( GSACT_SHAMDEAD );
					}
				}
			}

			if( enterview.Type == OBJ_MONSTER )
			{
				if( pactor != NULL )
				{
					const CSMONSTER& monsterData = enterview.Object.Monster;
					((GameNPC*)pactor)->onBirth( monsterData.BirthTimeLeft );
					pactor->m_nCampIdx = enterview.Object.Monster.CampIdx;
					pactor->m_BootyInfo.MemID = enterview.Object.Monster.BootyOwner;
					pactor->m_BootyInfo.BootyOwnerType = enterview.Object.Monster.BootyOwnerType;
					pactor->m_BootyInfo.BootyOwnerWid = enterview.Object.Monster.BootyOwnerWid;
					pactor->m_nPatrolID = enterview.Object.Monster.PatrolID;
					if( monsterData.MonPkFlag )
					{
						pactor->m_nPkMode = monsterData.MonPkModeChg.OwnerPKMode;
						pactor->setClanWID(monsterData.MonPkModeChg.OwnerClanID);
						pactor->m_PKInfoType = monsterData.MonPkModeChg.PKInfoType;
						switch (monsterData.MonPkModeChg.PKInfoType)
						{
						case MON_PK_TYPE_PLAYER:
							{
								pactor->setOwnerId(int(monsterData.MonPkModeChg.PkID));
							}
							break;

						case MON_PK_TYPE_TEAM:
							{
								pactor->setTeamWID(monsterData.MonPkModeChg.PkID);
							}
							break;

						case MON_PK_TYPE_CLAN:
							{
								pactor->setClanWID(monsterData.MonPkModeChg.PkID);
							}
							break;

						case MON_PK_TYPE_CAMP:
							{
								pactor->m_nCampIdx = int(monsterData.MonPkModeChg.PkID);
							}
							break;
						}
					}
					
					MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					if( ActorManager::getSingleton().FindActor( pactor->getOwnerId() ) == pMainPlayer )
					{
						pactor->m_ObjCamp = SKILL_TARGET_SELFPET;
					}
					else if (pactor->getSubType() == MON_SUB_TYPE_HUMAN_FRIEND)
					{
						pactor->m_ObjCamp = SKILL_TARGET_FRIEND_MON;
					}
					else if (pactor->getSubType() == MON_SUB_TYPE_HUMAN_BROTHER)
					{
						pactor->m_ObjCamp = SKILL_TARGET_FRIEND_PLAYER;
					}
					else if (pactor->getSubType() == MON_SUB_TYPE_SINGLE_SAFECAR ||
						pactor->getSubType() == MON_SUB_TYPE_TEAM_SAFECAR || (pMainPlayer && pMainPlayer->getOwnMonster() == pactor->GetID() && (pactor->getSubType() == MON_SUB_TYPE_SINGLE_CAR ||
						pactor->getSubType() == MON_SUB_TYPE_TEAM_CAR)))
					{
						pactor->m_ObjCamp = SKILL_TARGET_FRIEND_MON;
					}
					else
					{
						pactor->m_ObjCamp |= SKILL_TARGET_MID_MON;
						pactor->m_ObjCamp |= SKILL_TARGET_ENEMY_MON;
					}
					if ( monsterData.LifeEndTime != -1 )
					{
						pactor->registerTickEvent( new GameActor_Tick_OnWink( pactor, timeGetTime(), PET_WINK_TIME,
													monsterData.LifeEndTime * 1000 ) );
					}
					if (enterview.Object.Monster.PrepareAtkFlag)
					{
						int skillId = enterview.Object.Monster.MonPrepareAtkInfo.SkillID;
						int skillLv = enterview.Object.Monster.MonPrepareAtkInfo.Level;
						const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(skillId, skillLv);
						int during = SkillDef.PrepareTime * 100 -
							(enterview.Object.Monster.MonPrepareAtkInfo.sec * 1000 + int(enterview.Object.Monster.MonPrepareAtkInfo.usec * 0.001 + 0.5));
						pactor->setPrepareSkill(skillId, skillLv, 0, timeGetTime() - during);
						ISpellControler *sc = SkillManager::getSingleton().getSpellControler(pactor->GetID());
						GameSpell *pSpell = sc->getSpell(skillId, skillLv, 0);
						if( pSpell != NULL )
						{
							const RESPOS &pos = enterview.Object.Monster.MonPrepareAtkInfo.AtkPos;
							pSpell->setSelfPoint(pactor->GetPosition());
							pSpell->setTargetPoint(pos.X * 10, pos.Y * 10);
							sc->onPrepare(pSpell);
						}
					}
					if( enterview.Object.Monster.MachineFlag )
					{
						const MONMACHINEINFO& machineInfo = monsterData.MachineInfo;
						pactor->m_MonsterMachine.setOwner( pactor->GetID() );
						pactor->m_MonsterMachine.setMachineRideData( machineInfo );
						pactor->m_nPkValue = machineInfo.PKVal;
						if (machineInfo.PkTime > 0)
						{
							int sec, usec;
							ServerTimer::getSingleton().getServerTime(sec, usec);
							pactor->m_leastNoCrimeTime = timeGetTime() + (machineInfo.PkTime - sec) * 1000;
						}
						pactor->m_MonsterMachine.m_FromItemWID = machineInfo.FromItemWid;
					}
					pactor->setHitTargetID( enterview.Object.Monster.HitTargetID );

					if ( pactor->getSubType() == MON_SUB_TYPE_RONGLU ) 
					{
						pactor->m_nKillNum = enterview.Object.Monster.Rongluinfo.Num;
						pactor->m_nKillMax = enterview.Object.Monster.Rongluinfo.Max;
					}
					if ( pactor->getSubType() == MON_SUB_TYPE_LOST_TOTEM )
					{
						pactor->m_nLostTotemExpNum = enterview.Object.Monster.LostToteminfo.Num;
						pactor->m_nLostTotemExpMax = enterview.Object.Monster.LostToteminfo.Max;
						pactor->setLostTotemEffect();
					}
				}
				if (enterview.Object.Monster.FactoryFlag != 0)
				{
					pactor->setBuildTime(enterview.Object.Monster.FactoryInfo.BuildTime * 1000);
				}
				if (enterview.Object.Monster.CityBuildFlag != 0)
				{
					const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(enterview.Object.Monster.CityBuildFlag);
					if (WorldCityBuild && 
						( WorldCityBuild->Type == CITY_BUILD_TYPE_DOOR_BUILD || WorldCityBuild->Type == CITY_BUILD_TYPE_DOOR2 || WorldCityBuild->Type == CITY_BUILD_TYPE_DOOR3))
					{
						if (WorldCityManager::getSingleton().IsFighting())
						{
							pactor->playAction(10101);
						}
						else
						{
							pactor->playAction(10100);
						}
					}
					WorldCityManager::getSingleton().MarkCityBuildFight(pactor, enterview.Object.Monster.CityBuildFlag);
				}
			}

			GameActor* pSelectActor = FindActor( m_pMainPlayer->GetSelectTarget() );
			if( pSelectActor != NULL && pactor->GetID() == pSelectActor->getHitTargetID() )
			{
				char buf[256];
				sprintf( buf,"OnChangeHitTargetID( %d )",pactor->GetID() );
				getScriptVM()->callString( buf );

			}

			IGameBattle::getInstance()->objEnterView(pactor);
			IGameClanManager::getInstance()->objEnterView(pactor);
			IGameFriendManager::getInstance()->objEnterView(pactor);
			IGameWizard::getInstance()->objEnterView(pactor);
			IWorldCityManager::getInstance()->objEnterView(pactor);
			IGameSparta::getInstance()->objEnterView(pactor);
			IGameSpanStrong::getInstance()->objEnterView(pactor);
			RoleText& actorTitleMgr = pactor->getRoleText();
			actorTitleMgr.BuildTitle();
		}
		break;
	case LEAVE_VIEW:
		{
			const CSLEAVEVIEW &leaveview = msgbody.LeaveView;
			EffectManage::getSingleton().clearLink(leaveview.ID);
			GameActor *pactor = FindActor(leaveview.ID);
			if( pactor )
			{
				if (pactor->getType() == GAT_MONSTER)
				{
					const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef(pactor->getResId());
					if (monsterDef.DynMask)
					{
						GameMaskMap *maskMap = m_pMainPlayer->getCurMap()->getMaskMap();
						GameXYZ pos = pactor->GetPosition();
						maskMap->setDynMsk(pos.x / 10, pos.z / 10,
							monsterDef.DynMaskX == 0 ? monsterDef.Radius : monsterDef.DynMaskX,
							monsterDef.DynMaskY == 0 ? monsterDef.Radius : monsterDef.DynMaskY,
							pactor->getAngle(), -int(monsterDef.DynMask));
					}
					
					if ( m_pMainPlayer->GetCatchPet() == leaveview.ID )
					{
						m_pMainPlayer->onBreakChant( eUseItem_Chant );
 						GameMsgPrint::getSingleton().showMidTip( MID_CATCH_PET_DEAD );
					}
				}
				IGameBattle::getInstance()->objLeaveView(pactor);
				IGameWizard::getInstance()->objLeaveView(pactor);
				IWorldCityManager::getInstance()->objLeaveView(pactor);
				if (pactor->getType() == GAT_NPC)
				{		
					int resid = pactor->getResId();
					GameMsgPrint::getSingleton().npcLeaveView( leaveview.ID);
				}
				if( pactor->getType() == GAT_ITEM )
				{
					ActorManager::getSingleton().getMainPlayer()->getContainer().closeBooty( pactor->GetID() );
					GameMsgPrint::getSingleton().SendUIEvent( "GE_DESTROY_ITEM" );
				}
				if ( leaveview.ID == m_pMainPlayer->getOwnMonster() )
				{
					m_pMainPlayer->delOwnMonster();
				}
				if( pactor->getType() == GAT_MONSTER && pactor->getSubType() == MON_SUB_TYPE_MACHINE )
				{
					GameActor* pDrive = ActorManager::getSingleton().FindActor( pactor->m_MonsterMachine.m_monsterInfo.m_nPassengerID[0] );
					if( pDrive != NULL )
					{						
						CSACTIONLEAVEMACHINE rideOnMachineInfo ;
						rideOnMachineInfo.RolePos.X = pDrive->GetPosition().x / 10;
						rideOnMachineInfo.RolePos.Y = pDrive->GetPosition().z / 10;
						pDrive->m_MonsterMachine.downMonsterMachine(rideOnMachineInfo);
					}
				}
				
				if ( leaveview.HomeFlag == 0 )
				{
// 					if( pactor->getType() == GAT_PET )
// 					{
// 						for( std::map<int, GameActor *>::iterator iter = m_Actors.begin(); 
// 								iter != m_Actors.end(); ++iter )
// 						{
// 							for( int j = 0; j < PET_MAX_FIGHT_SHI; j++ )
// 							{
// 								GameActor* pPet = pactor->m_Pet.getPetActor( j ); 
// 								if( pPet != NULL && pPet == pactor )
// 								{
// 									pPet = NULL;
// 								}
// 							}
// 						}			
// 					}
					pactor->OnLeaveMap();
					DestroyActor(pactor);
				}else{
					pactor->setSpeed( leaveview.HomeData.Speed );
					pactor->CmdMoveBack( leaveview.HomeData.Pos.X, leaveview.HomeData.Pos.Y );
				}
			}
			else
			{
				BulletManager::getSingleton().DestoryBullet(leaveview.ID);
			}
		}
		break;
	case ATTR_CHG:
		{
			const CSPLAYERATTRVAL& allChgAttrVal = msgbody.AttrChg;
		
			for( int i = 0; i < allChgAttrVal.AttrNum; i++ )
			{
				if ( m_pMainPlayer == NULL )
				{
					break;
				}
				
				const CSATTRVAL& oneChgVal = allChgAttrVal.AttrList[i];
				if( oneChgVal.AttrID == ATTR_ID_COLLECTNUM )
				{
 					SkillManager::getSingleton().setSpellCollectNumCur( 
						oneChgVal.Value.CollectNum.SkillID, oneChgVal.Value.CollectNum.CollectNum );
				}
				
				const CSATTRIDVAL* preValue	= m_pMainPlayer->getAttriValule( oneChgVal.AttrID );
				
				int nChgValue = 0;
				
				if ( oneChgVal.AttrID == ATTR_ID_EXP )
				{
					nChgValue = oneChgVal.Value.Exp - preValue->Exp;
				}
				else if( oneChgVal.AttrID == ATTR_ID_LEVEL )
				{
					nChgValue = oneChgVal.Value.Level - preValue->Level;
				}
				else if (oneChgVal.AttrID == ATTR_ID_COLLECTSKILLUSE)
				{
					m_pMainPlayer->CollectSkill_SlotCurr = oneChgVal.Value.CollectSkillUse;
				}
				else if (oneChgVal.AttrID == ATTR_ID_MONEY)
				{
					nChgValue = oneChgVal.Value.Money - preValue->Money;
				}
				else if (oneChgVal.AttrID == ATTR_ID_BINDGODCOIN)
				{
					nChgValue = oneChgVal.Value.BindGodCoin - preValue->BindGodCoin;
				}
				else if ( oneChgVal.AttrID == ATTR_ID_VIGOR )
				{
					nChgValue = oneChgVal.Value.Vigor - preValue->Vigor;
				}
				else if ( oneChgVal.AttrID == ATTR_ID_VIGORMAX )
				{
					nChgValue = oneChgVal.Value.VigorMax - preValue->VigorMax;
				}
				else if ( oneChgVal.AttrID == ATTR_ID_ZHENGYI )
				{
					nChgValue = oneChgVal.Value.Zhengyi - preValue->Zhengyi;
				}
				else if ( oneChgVal.AttrID == ATTR_ID_LIFT_BRAND )
				{
					nChgValue = oneChgVal.Value.LiftBrand -preValue->LiftBrand;
				}
				else if ( oneChgVal.AttrID == ATTR_ID_CHARM )
				{
					m_pMainPlayer->m_WeekCharm += oneChgVal.Value.Charm - preValue->Charm;
				}
				
				m_pMainPlayer->setAttriValue( oneChgVal.AttrID, oneChgVal.Value);

				if ( oneChgVal.AttrID == ATTR_ID_EXP && nChgValue != 0  )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "UI_ACTOR_EXP_CHG" );
				}
				else if ( oneChgVal.AttrID == ATTR_ID_LEVEL && nChgValue > 0  )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "UI_ACTOR_LEVEL_CHG" );
				}
				else if( oneChgVal.AttrID == ATTR_ID_XP || oneChgVal.AttrID == ATTR_ID_XPMAX )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "UI_ACTOR_XP_CHG" );
				}
				else if ( oneChgVal.AttrID == ATTR_ID_VIGOR )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "UI_ACTOR_VIG_CHG" );
				}
				else if ( oneChgVal.AttrID == ATTR_ID_VIGORMAX )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "UI_ACTOR_VIGMAX_CHG" );
				}
				else if ( oneChgVal.AttrID == ATTR_ID_COLLECTSKILLUSE)
				{
					GameMsgPrint::getSingleton().SendUIEvent("GE_CHANGE_COLLECTTYPE");
				}
				else if ( oneChgVal.AttrID == ATTR_ID_BULL )
				{
					GameMsgPrint::getSingleton().SendUIEvent("GE_CHANGE_BULL");
				}
				else if ( oneChgVal.AttrID == ATTR_ID_MONEY )
				{
					GameMsgPrint::getSingleton().SendUIEvent("GE_CHANGE_MONEY");
				}
				else if ( oneChgVal.AttrID == ATTR_ID_ZHENGYI )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "GE_CHANGE_ZHENGYI" );
				}
				else if ( oneChgVal.AttrID == ATTR_ID_SCHOOL_VAL )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "GE_CHANGE_CHUANCHENG" );
				}
				// 检查是否是主角的经验值，MP增加，若是，则显示主角增加的经验值，MP
				if ( nChgValue <=0 || m_attrMapToHurt.find( oneChgVal.AttrID ) == m_attrMapToHurt.end() )
				{
					continue;
				}

				SCTDamageManager&	sctMgr		= SCTDamageManager::getSingleton();
				SCTDamageBase*		pSCTDamage	= sctMgr.getPassiveSCT( m_pMainPlayer->GetID()  );
				sctMgr.setEntityAttr( m_attrMapToHurt[oneChgVal.AttrID], nChgValue, m_pMainPlayer->GetID() );
				
				if ( pSCTDamage == NULL )
				{
					continue;
				}

				pSCTDamage->calculateDamage( nChgValue, 0, 0, m_pMainPlayer, 
												m_attrMapToHurt[oneChgVal.AttrID], 1, 0 );
				pSCTDamage->showFllyFont( timeGetTime() );
			}
		}
		break;

	case BASE_ATTR_CHG:
		{
			const CSPLAYERATTRVAL& allChgAttrVal = msgbody.BaseAttrChg;
			for( int i = 0; i < allChgAttrVal.AttrNum; i++ )
			{
				if ( m_pMainPlayer == NULL )
				{
					break;
				}
				const CSATTRVAL& oneChgVal = allChgAttrVal.AttrList[i];
				m_pMainPlayer->setBaseAttriValue( oneChgVal.AttrID, oneChgVal.Value);
			}
		}
		break;
	
	case APEX_SVR:
		{
			//pfSendToApexClient( msgbody.ApexSvr.Data, msgbody.ApexSvr.DataLen );
		}
		break;
	default:
		break;
	}

	return 0;
}

void ActorManager::addClientSelfDestroy( GameActor* pActor )
{
	m_ClientSelfDestoryActors.push_back( pActor->GetID() );
}

// 获取玩家数量
int ActorManager::getPlayerCount()
{
	int nCount = 0;
	for( ACTOR_ITER iter = m_Actors.begin(); iter != m_Actors.end(); ++iter )
	{
		if( iter->second->getType() == GAT_PLAYER )
		{
			nCount++;
		}
	}
	return nCount;
}

// 获取怪物数量
int ActorManager::getMonsterCount()
{
	return (int)m_Monsters.size();
}

// 根据索引找到monster
GameActor* ActorManager::getMonster( int nIndex )
{
	assert( nIndex < (int)m_Monsters.size() );
	return m_Monsters[nIndex];
}

// actors总数
int ActorManager::getActorNums()
{
	return (int)m_Actors.size();
}
// 根据索引找到Actor
GameActor* ActorManager::getActor( int nIndex )
{
	int incrindex = 0;
	for( std::map<int, GameActor *>::iterator iter = m_Actors.begin(); iter!=m_Actors.end(); iter++)
	{
		if (incrindex == nIndex)
		{
			return iter->second;
		}
		incrindex++;
	}
	return NULL;
}
void ActorManager::updateMonsters( GameActor* pActor )
{
	switch (pActor->getType())
	{
	case GAT_PLAYER:
		{
			std::set<GameActor *>::iterator it = m_Players.find(pActor);
			if (it != m_Players.end())
			{
				m_Players.erase(it);
			}
		}
		break;
	case GAT_MONSTER:
		{
			for ( std::vector<GameActor*>::iterator iter = m_Monsters.begin(), end = m_Monsters.end(); 
				iter != end; ++iter )
			{
				if ( pActor->GetID() == ( *iter )->GetID() )
				{
					m_Monsters.erase( iter );
					return;
				}
			}
		}
		break;
	}
}	

// 获取NPC数量
int ActorManager::getNpcCount()
{
	int nCount = 0;
	for( ACTOR_ITER iter = m_Actors.begin(); iter != m_Actors.end(); ++iter )
	{
		if( iter->second->getType() == GAT_NPC )
		{
			nCount++;
		}
	}
	return nCount;
}

void ActorManager::resetShowModelAndTitleOption()
{
	m_nShowModelOption = HIDE_MODEL_NONE;
	m_bShowOtherPlayerTitle	= true;
}

void ActorManager::changeShowOtherPlayerModel()
{
	++m_nShowModelOption;
	if ( m_nShowModelOption > HIDE_MODEL_OPTION_MAX )
	{
		m_nShowModelOption = HIDE_OTHER_PLAYER_AND_PET;
	}

	GameMsgPrint::getSingleton().SendUIEvent( "GE_CHANGE_SHOW_PLAYER_MODEL" );
}

bool ActorManager::canShowActorModelInScene( GameActor* pActor )
{
	if ( pActor == NULL )
	{
		return false;
	}

	//在怪物机械上面不用显示玩家模型
	int nHideOtherPlayerOption	= getShowOtherPlayerModelOption();
	int nActorType				= pActor->getType();
	if ( nActorType == GAT_PLAYER )
	{
		if ( pActor->isOnMonsterMachine() )
		{
			return false;
		}
		
		if ( nHideOtherPlayerOption == HIDE_MODEL_NONE )
		{
			return true;
		}

		if ( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET && pActor->isInStall() )
		{
			return true;
		}

		if ( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET_AND_STALL && pActor->isInStall() )
		{
			return false;
		}

		if ( m_pMainPlayer->isInSameTeam( pActor->getTeamWID() ) )
		{
			return true;
		}

		return false;
	}
	
	
	if ( nHideOtherPlayerOption == HIDE_MODEL_NONE )
	{
		return true;
	}

	if ( nActorType == GAT_PET )
	{
		if ( pActor->isCamp( SKILL_TARGET_SELFPET ) )
		{
			return true;
		}

		return false;
	}

	if ( nActorType == GAT_MONSTER )
	{
		switch (pActor->m_PKInfoType)
		{
		case MON_PK_TYPE_PLAYER:
			{
				if (!GameActor_PVP::getSingleton().isSameTeam(pActor))
				{
					return false;
				}
			}
			break;

		case MON_PK_TYPE_TEAM:
			{
				if (!m_pMainPlayer->isInSameTeam(pActor->getTeamWID()))
				{
					return false;
				}
			}
			break;
		}

		const char* pszOwnerName = pActor->getOwnerName();
		if ( pszOwnerName[0] == 0 )
		{
			return true;
		}

		if ( m_pMainPlayer->isInSameTeam( pActor->getOwnerTeamWID() ) )
		{
			return true;
		}

		if ( strcmp( pszOwnerName, m_pMainPlayer->getName() ) == 0 )
		{
			return true;
		}

		return false;
	}

	return true;
}

// 获取是否显示玩家
int ActorManager::getShowOtherPlayerModelOption()
{
	return m_nShowModelOption;
}

void ActorManager::setShowItherPlayerTitle()
{
	m_bShowOtherPlayerTitle = !m_bShowOtherPlayerTitle;
	updateAllPlayerTitleInfo();
	GameMsgPrint::getSingleton().SendUIEvent( "GE_CHANGE_SHOW_PLAYER_TITLE" );
}

void ActorManager::updateAllTeamPlayerTitleInfo()
{
	for ( ACTOR_ITER iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor == NULL )
		{
			continue;
		}

		if ( pOneActor->getType() != GAT_PLAYER || !m_pMainPlayer->isInSameTeam( pOneActor->getTeamWID() ) )
		{
			continue;
		}

		RoleText& actorTitleMgr = pOneActor->getRoleText();
		actorTitleMgr.BuildTitle();
	}
}

// 更新所有玩家的头顶信息
void ActorManager::updateAllPlayerTitleInfo()
{
	for ( ACTOR_ITER iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor == NULL )
		{
			continue;
		}

		if ( pOneActor->getType() != GAT_PLAYER )
		{
			continue;
		}

		RoleText& actorTitleMgr = pOneActor->getRoleText();
		actorTitleMgr.BuildTitle();
	}
}

void ActorManager::updateAllPlayerTitleColor()
{
	for ( ACTOR_ITER iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor == NULL )
		{
			continue;
		}

		if ( pOneActor->getType() != GAT_PLAYER )
		{
			continue;
		}

		RoleText& actorTitleMgr = pOneActor->getRoleText();
		actorTitleMgr.UpdateTextColor();
	}
}

// 是否显示公会名字
bool ActorManager::isShowOtherPlayerTitle()
{
	return m_bShowOtherPlayerTitle;
}

// 设置哪些信息要显示
void ActorManager::setGameOpt( int nIndex, bool bShow )
{
	assert( nIndex < GAME_OPT_REFUSE_ADD_FRIEND_REQ );
	m_GameConfig.setGameOpt( nIndex, bShow );
	if ( nIndex == GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME || nIndex == GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME || 
			nIndex == GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME || nIndex == GAME_OPT_SHOW_MAIN_CLAN_NAME )
	{
		OnChangeAllPlayerNameOpt();
	}
}

void ActorManager::setGameOptValue( int nIndex, char value )
{
	assert( nIndex < GAME_OPT_REFUSE_ADD_FRIEND_REQ );
	m_GameConfig.setGameOpt( nIndex, value );
	if ( nIndex == GAME_OPT_SHOW_ALLPLAYER_AND_PET_NAME || nIndex == GAME_OPT_SHOW_ALLPLAYER_TEAM_DESIGNATION_NAME || 
		nIndex == GAME_OPT_SHOW_OTHERPLAYER_CLAN_NAME || nIndex == GAME_OPT_SHOW_MAIN_CLAN_NAME )
	{
		OnChangeAllPlayerNameOpt();
	}
}

// 获取对应显示信息列表
bool ActorManager::getGameOpt( int nIndex )
{
	/*assert( nIndex < GAME_OPT_REFUSE_ADD_FRIEND_REQ );*/
	return m_GameConfig.getGameOpt( nIndex );
}

GameActor * ActorManager::getNextActorById(int id)
{
	std::map<int, GameActor*>::iterator i	= m_Actors.find(id);
	std::map<int, GameActor*>::iterator end	= m_Actors.end();
	if (i == end || ++i == end)
	{
		return NULL;
	}
	return i->second;
}

GameActor * ActorManager::getNextActor(GameActor *pre)
{
	if (pre == NULL)
	{
		if (m_Actors.empty())
		{
			return NULL;
		}
		return m_Actors.begin()->second;
	}
	return getNextActorById(pre->GetID());
}

void ActorManager::addLooseItemShow(const tagLoosItemPosInfo &info)
{
	int total = 1;
	std::map<int, int>::iterator i = m_ItemDropCount.find(info.m_nFromID);
	if (i != m_ItemDropCount.end())
	{
		total = i->second + 1;
	}
	m_ItemDropCount[info.m_nFromID] = total;
	unsigned int t = timeGetTime();
	if (info.m_droptype != VIEW_DROP_TYPE_ALL)
	{
		t += LOOSEITEM_DELAYTIME * total;
	}
	else
	{
		t += LOOSEITEM_DEATHTIME;
	}
	m_ItemTimeArray.push_back(pair<unsigned int, tagLoosItemPosInfo>(t, info));
	std::push_heap(m_ItemTimeArray.begin(), m_ItemTimeArray.end(), TimeHeapCmp<tagLoosItemPosInfo>());
}

void ActorManager::changeMainPlayerID(int newId)
{
	if( m_pMainPlayer != NULL )
	{
		int oldId = m_pMainPlayer->GetID();
		ACTOR_ITER it = m_Actors.find(m_pMainPlayer->GetID());
		if( it != m_Actors.end() )
		{
			m_Actors.erase(it);
		}
		m_Actors[newId] = m_pMainPlayer;
		m_pMainPlayer->SetID(newId);
		if ( m_pMainPlayer->getAutoFind() )
		{
			m_pMainPlayer->setAutoFind( false );
		}
		//pet,buff
		SCTDamageManager::getSingleton().clearDamageActor();
		SCTDamageManager::getSingleton().addDamageActor(newId);
		m_pMainPlayer->getBufStatus()->setOwnerId(newId);
		m_pMainPlayer->getBufStatus()->reLoadAllEff();
		for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
		{
			GameActor* pPet = m_pMainPlayer->getPet()->getFightPet(i);
			if( pPet != NULL )
			{
				int newPetyID = newId + ( i + 1 ) * PET_ID_BASE;
				it = m_Actors.find( pPet->GetID() );
				if( it != m_Actors.end() )
				{
					m_Actors.erase(it);
				}
				m_Actors[newPetyID] = pPet;
				pPet->SetID(newPetyID);
				pPet->setOwnerId( newId );
			}
		}

		for( it = m_Actors.begin(); it != m_Actors.end() ; ++it )
		{
			if( it->second->getMonsterMachine()->m_nOwnerID == oldId )
			{
				it->second->getMonsterMachine()->m_nOwnerID = newId;
			}
		}

	}
}

void ActorManager::setItemChangeTime( unsigned int delay, int id )
{
	m_ItemsChangeOwner.push_back(pair<unsigned int, int>(timeGetTime() + delay, id));
	push_heap(m_ItemsChangeOwner.begin(), m_ItemsChangeOwner.end(), TimeHeapCmp<int>());
}

void ActorManager::changeShowPlayerID()
{
	m_bShowPlayerID = !m_bShowPlayerID;
}

float ActorManager::getActorDistance(int id)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (pActor == NULL)
	{
		return -1;
	}
	GameXYZ selfPos = getMainPlayer()->GetPosition();
	GameXYZ targetPos = pActor->GetPosition();
	selfPos.y = targetPos.y;
	unsigned int nRadius = 0;
	if (pActor->getType() == GAT_MONSTER)
	{
		nRadius = DBDataManager::getSingleton().getMonsterDef(pActor->getResId()).Radius;
	}
	return selfPos.subtract(targetPos).length()///减去目标的触摸半径
		- nRadius;
}

bool ActorManager::insertVirtualActor(GameActor *pActor)
{
	if (pActor == NULL)
	{
		return false;
	}
	std::map<int, GameActor*>::iterator i = m_Actors.find(pActor->GetID());
	if (i == m_Actors.end())
	{
		m_Actors[pActor->GetID()] = pActor;
		return true;
	}

	return false;
}

void ActorManager::UpdateAllActorClanName( tdr_ulonglong clanWID, const char* pszClanName )
{
	for ( std::map<int, GameActor*>::iterator iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor->isInClan() && pOneActor->getClanWID() == clanWID )
		{
			pOneActor->setClanName( pszClanName );
			RoleText& actorTitleMgr = pOneActor->getRoleText();
			actorTitleMgr.BuildTitle();
		}
	}
}

void ActorManager::updateActorstate(GameActor* pOneActor, bool bshow)
{
	Ogre::ShowActor *temp=pOneActor->getShowActor();
	Ogre::Entity    *etemp=temp->getCurEntity();
	etemp->show(bshow);
	pOneActor->SetShow(bshow);
}

void  ActorManager::hideAllActors()
{
	for ( std::map<int, GameActor*>::iterator iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if (pOneActor)
		{
			updateActorstate(pOneActor, false);
		}
	}
}

void ActorManager::OnChangeAllPlayerNameOpt()
{
	updateAllPlayerTitleInfo();
}

void ActorManager::WarFogUpdate(int wardistance, float x, float z)
{
	for ( std::map<int, GameActor*>::iterator iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor && !pOneActor->isShow() )
		{
			if ( (x-pOneActor->GetPosition().x/10)*(x-pOneActor->GetPosition().x/10) + (z-pOneActor->GetPosition().z/10)*(z-pOneActor->GetPosition().z/10) < wardistance )
			{
				updateActorstate(pOneActor, true);
			}
		}
	}
}

bool ActorManager::isActorIgnoreF12(GameActor* pOneActor)
{
	if ( pOneActor == NULL )
	{
		return true;
	}

	ActorManager& actorMgr			= ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer		= actorMgr.getMainPlayer();
	tdr_ulonglong mainplayerTeamWID = pMainPlayer->getTeamWID();
	int nActorType					= pOneActor->getType();
	if ( nActorType == GAT_MAINPLAYER )
	{
		return true;
	}

	if ( nActorType == GAT_PET )
	{
		if ( pOneActor->isCamp( SKILL_TARGET_SELFPET ) )
		{
			return true;
		}

		/*int nPetOwnerID			= pOneActor->getOwnerId();
		GameActor* pPetOwner	= actorMgr.FindActor( nPetOwnerID );
		if ( pPetOwner == NULL )
		{
			return false;
		}

		if ( pMainPlayer->isInSameTeam( pPetOwner->getTeamWID() ) )
		{
			return true;
		}*/

		return false;
	}

	if ( nActorType == GAT_MONSTER )
	{
		switch (pOneActor->m_PKInfoType)
		{
		case MON_PK_TYPE_PLAYER:
			{
				if (!GameActor_PVP::getSingleton().isSameTeam(pOneActor))
				{
					return false;
				}
			}
			break;

		case MON_PK_TYPE_TEAM:
			{
				if (!pMainPlayer->isInSameTeam(pOneActor->getTeamWID()))
				{
					return false;
				}
			}
			break;
		}

		const char* pszOwnerName = pOneActor->getOwnerName();
		if ( pszOwnerName[0] == 0 )
		{
			return true;
		}

		tdr_ulonglong uOwnerTeamWID = pOneActor->getOwnerTeamWID();
		if ( pMainPlayer->isInSameTeam( uOwnerTeamWID ) )
		{
			return true;
		}

		if ( strcmp( pszOwnerName, pMainPlayer->getName() ) == 0 )
		{
			return true;
		}

		return false;
	}

	if ( nActorType == GAT_PLAYER )
	{
		bool isInSame = pMainPlayer->isInSameTeam( pOneActor->getTeamWID() );

		if ( pOneActor->isInStall() )
		{
			return false;
		}

		if ( isInSame )
		{
			return true;
		}

		return false;
	}

	GameMap* pMapMgr = pOneActor->getCurMap();
	if ( pMapMgr == NULL )
	{
		return true;
	}

	return true;
}

int ActorManager::getNextPlayerEnemy(int id /*= 0*/)
{
	std::set<int>::iterator i;
	if (id == 0)
	{
		i = playerEnemy.begin();
	}
	else
	{
		i = playerEnemy.find(id);
		if (i != playerEnemy.end())
		{
			++i;
		}
	}
	if (i != playerEnemy.end())
	{
		return *i;
	}
	return 0;
}

void ActorManager::addActorFade(GameActor *who, bool isIn)
{
	std::map<GameActor *, std::pair<float, bool>>::iterator i = m_fadeActors.find(who);
	if (i == m_fadeActors.end())
	{
		float alpha = isIn ? 0.1f : 1.0f;
		m_fadeActors[who].first = alpha;
		int resid = who->GetMorphResID();
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
		if (who->m_Rider.m_pShowActor != NULL)
		{
			who->m_Rider.m_pShowActor->getCurEntity()->setTransparent(alpha);
		}
		else
		{
			who->getShowActor()->getCurEntity()->setTransparent(alpha * info.alpha);
		}
	}
	m_fadeActors[who].second = isIn;
}

void ActorManager::removeActorFade(GameActor *who)
{
	std::map<GameActor *, std::pair<float, bool>>::iterator i = m_fadeActors.find(who);
	if (i != m_fadeActors.end())
	{
		GameActor *pActor = i->first;
		int resid = who->GetMorphResID();
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
		if (pActor->m_Rider.m_pShowActor != NULL)
		{
			pActor->m_Rider.m_pShowActor->getCurEntity()->setTransparent(1);
		}
		else
		{
			pActor->getShowActor()->getCurEntity()->setTransparent(info.alpha);
		}
		if (!i->second.second)
		{
			if (pActor->getCurMap() != NULL)
			{
				if (pActor->m_lastOptimizeTime == 0)
				{
					pActor->m_lastOptimizeTime = timeGetTime();
				}
				pActor->getShowActor()->onLeaveMap(pActor->getCurMap()->getShowGameMap());
				if (pActor->m_Rider.m_pShowActor != NULL)
				{
					pActor->m_Rider.m_pShowActor->onLeaveMap(pActor->getCurMap()->getShowGameMap());
				}
				pActor->m_pSmallDecal->show(false);
			}
		}
		m_fadeActors.erase(i);
	}
}

bool ActorManager::isInMainPlayerTeam(GameActor *who)
{
	if (who == NULL)
	{
		return false;
	}
	if (who == getMainPlayer())
	{
		return true;
	}
	switch (who->getType())
	{
	case GAT_PET:
		{
			if (who->isCamp(SKILL_TARGET_SELFPET))
			{
				return true;
			}
			return GameActor_PVP::getSingleton().isSameTeam(who);
		}

	case GAT_MONSTER:
		{
			GameActor *pActor = FindActor(who->getOwnerId());
			if (pActor != NULL)
			{
				return isInMainPlayerTeam(pActor);
			}
			return false;
		}

	case GAT_PLAYER:
		{
			return getMainPlayer()->isInSameTeam(who->getTeamWID());
		}

	default:
		return false;
	}
}

int ActorManager::getServerId(const char* szName )
{
	const char* pszID = strpbrk( szName, "$" );
	if( pszID != NULL )
	{
		pszID = szName + 2;		
		return atoi( pszID );
	}
	return -1;
}
void ActorManager::setMapOwnerWID( int mapID, tdr_ulonglong clanWID )
{
	for ( int i = 0; i < MAX_BASE_WAR_MAP_CLAN_INFO ; i++ )
	{
		if ( m_BaseWarMapOwnerList[i].nMapID == mapID )
		{
			m_BaseWarMapOwnerList[i].nOwnerClanWID = clanWID;
		}
	}
}

const char* ActorManager::getClanFengHao( const tdr_ulonglong ClanWid )
{
	std::map<tdr_ulonglong,  string>::iterator i = m_ClanFengHao.find(ClanWid);
	if ( i != m_ClanFengHao.end() )
	{
		return i->second.c_str();
	}
	return "";
}
void ActorManager::setClanFengHao()
{
	tdr_ulonglong ClanWid = 0;
	m_ClanFengHao.clear();
	for ( int i = 0; i < MAX_BASE_WAR_MAP_CLAN_INFO ; i++ )
	{
		if ( m_BaseWarMapOwnerList[i].nOwnerClanWID != 0 )
		{
			ClanWid = m_BaseWarMapOwnerList[i].nOwnerClanWID;
			int ownStrongNum = 0;
			for ( int i = 0; i < MAX_BASE_WAR_MAP_CLAN_INFO ; i++ )
			{
				if ( m_BaseWarMapOwnerList[i].nOwnerClanWID == ClanWid )
				{
					ownStrongNum++;
				}
			}
			if ( WorldCityManager::getSingleton().IsWorldCityKeeper(ClanWid) )
			{
				switch( ownStrongNum ){
			case 1:
				m_ClanFengHao[ClanWid] = "[主境·城主]";
				break;
			case 2:
				m_ClanFengHao[ClanWid] = "[双境·城主]";
				break;
			case 3:
				m_ClanFengHao[ClanWid] = "[三境·城主]";
				break;
			case 4:
				m_ClanFengHao[ClanWid] = "[四境·城主]";
				break;
			case 5:
				m_ClanFengHao[ClanWid] = "[五境·城主]";
				break;
			case 6:
				m_ClanFengHao[ClanWid] = "[世界·霸主]";
				break;
				}
			}
			else
			{
				switch( ownStrongNum ){
			case 1:
				{
					const char* szName = "";
					for ( int i = 0; i < MAX_BASE_WAR_MAP_CLAN_INFO ; i++ )
					{
						if ( m_BaseWarMapOwnerList[i].nOwnerClanWID == ClanWid )
						{
							szName = DBDataManager::getSingleton().getMapDef(m_BaseWarMapOwnerList[i].nMapID).Name;
						}
					}
					if ( strcmp( szName, "冰岛") == 0 )
					{
						m_ClanFengHao[ClanWid] = "[冰岛·军阀]";
					}
					else if ( strcmp( szName, "流金沙漠") == 0 )
					{
						m_ClanFengHao[ClanWid] = "[沙漠·军阀]";
					}
					else if ( strcmp( szName, "落日荒野") == 0 )
					{
						m_ClanFengHao[ClanWid] = "[荒野·军阀]";
					}
					else if ( strcmp( szName, "越雾雨林") == 0 )
					{
						m_ClanFengHao[ClanWid] = "[雨林·军阀]";
					}
					else if ( strcmp( szName, "银风雪原") == 0 )
					{
						m_ClanFengHao[ClanWid] = "[雪原·军阀]";
					}
					else if ( strcmp( szName, "鸣沙陷地") == 0 )
					{
						m_ClanFengHao[ClanWid] = "[鸣沙·军阀]";
					}
				}
				break;
			case 2:
				m_ClanFengHao[ClanWid] = "[双域·领主]";
				break;
			case 3:
				m_ClanFengHao[ClanWid] = "[三域·领主]";
				break;
			case 4:
				m_ClanFengHao[ClanWid] = "[四域·领主]";
				break;
			case 5:
				m_ClanFengHao[ClanWid] = "[五域·领主]";
				break;
			case 6:
				m_ClanFengHao[ClanWid] = "[全域·领主]";
				break;
				}
			}
		}
	}
	tdr_ulonglong worldCityKeeper = WorldCityManager::getSingleton().GetCityKeepClanWID();
	std::map<tdr_ulonglong,  string>::iterator i = m_ClanFengHao.find(worldCityKeeper);
	//如果列表里没有占领时间城的公会，添加进去（即只占领了时间城，但是却没有据点的这种情况）
	if ( i == m_ClanFengHao.end() )
	{
		m_ClanFengHao[worldCityKeeper] = "[时间·城主]";
	}
}

void ActorManager::SetPerfectStarNineActor()
{
	for ( std::map<int, GameActor*>::iterator iter = m_Actors.begin(), end = m_Actors.end(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor->getType() != GAT_PLAYER && pOneActor->getType() != GAT_MAINPLAYER )
		{
			continue;
		}
		
		if ( ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_PERFECT_STAR_EFFECT ) )
		{
			pOneActor->getShowActor()->getCurEntity()->stopMotion( 30006 );
		//	pOneActor->stopMotion("wanmeiX9");
		}
		else
		{
			//pOneActor->playMotion( "wanmeiX9",true );
			pOneActor->setPerfectEffect();
		}

	}	
}
const char* ActorManager::GetSpartaLevelName( int nFightVal )
{
	std::vector<tagSpartaLevelDef*>::iterator iter = m_SpartaLevelData.begin();
	for ( ; iter != m_SpartaLevelData.end(); ++iter)
	{
		if ( (*iter)->FightValMin <= nFightVal && (*iter)->FightValMax >= nFightVal )
		{
			return (*iter)->Name;
		}
	}
	return "";
}
