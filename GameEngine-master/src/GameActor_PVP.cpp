#include "GameActor_PVP.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "OgreScriptLuaVM.h"
#include "OgreLog.h"
#include "OgreUIRenderer.h"
#include "GameMap.h"
#include "WorldCity.h"
#include <Windows.h>

GameActor_PVP::GameActor_PVP()
{
	resetTeamMember();
	resetEnemyClan();

	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	fontTexture[0]					= prenderer->CreateTexture( "uires\\ui\\Za\\pk1.tga", 
		Ogre::BLEND_ALPHABLEND );
	fontTexture[1]					= prenderer->CreateTexture( "uires\\ui\\Za\\pk2.tga", 
		Ogre::BLEND_ALPHABLEND );
}

void GameActor_PVP::judgeCamp(GameActor *pActor, bool IsRookieProtect)
{
	if (!pActor)
	{
		return;
	}


	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if (pActor == pMain || pMain == NULL)
	{
		return;
	}
	if (pActor->getType() == GAT_MONSTER &&
		( pActor->getOwnerId() == pMain->GetID() || pActor->isPolanCar()) )
	{
		pActor->m_ObjCamp = SKILL_TARGET_FRIEND_MON;
		return;
	}

	int pkMode = getPkMode();
	if (!isForcePkMode())
	{
		if (pMain->m_MonsterMachine.m_playerInfo.m_nMonsterMemID != 0)
		{
			if (!WorldCityManager::getSingleton().IsFighting() && WorldCityManager::getSingleton().IsCityBuildFight(pMain->m_MonsterMachine.m_playerInfo.m_nMonsterMemID))
			{
				pkMode = PK_MODE_NORMAL;
			}
			else
			{
				GameActor *monsterMachine = ActorManager::getSingleton().FindActor(pMain->m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
				if (monsterMachine != NULL)
				{
					switch (monsterMachine->m_PKInfoType)
					{
					case MON_PK_TYPE_PLAYER:
						pkMode = monsterMachine->m_nPkMode;
						break;

					case MON_PK_TYPE_TEAM:
						pkMode = PK_MODE_TEAM;
						break;

					case MON_PK_TYPE_CLAN:
						pkMode = PK_MODE_CLAN;
						break;

					case MON_PK_TYPE_CAMP:
						pkMode = PK_MODE_CAMP;
						break;
					}
				}
			}
		}
	}

	int nResult = pActor->getObjCamp();
	int oldResult = nResult;
	bool sameTeam = isSameTeam(pActor);//是否同队伍
	bool sameClan = isSameClan(pActor);//是否同公会
	bool sameGroup = isSameGroup(pActor);//是否同阵营
	if (pActor->getType() == GAT_PLAYER)
	{
		switch (pkMode)
		{
		case PK_MODE_NORMAL:
			nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_MID_PLAYER;
			break;

		case PK_MODE_POLICE:
			if (isCrime(pActor))
			{
				nResult = SKILL_TARGET_ENEMY_PLAYER;
			}
			else
			{
				nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_MID_PLAYER;
			}
			break;

		case PK_MODE_ALL:
			nResult = SKILL_TARGET_ENEMY_PLAYER;
			break;

		case PK_MODE_TEAM:
			nResult = sameTeam ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_ENEMY_PLAYER;
			break;

		case PK_MODE_CLAN:
			nResult = sameClan ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_ENEMY_PLAYER;
			break;

		case PK_MODE_CAMP:
			nResult = sameGroup ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_ENEMY_PLAYER;
			break;

		case PK_MODE_ENEMY_CLAN:
			nResult = SKILL_TARGET_MID_PLAYER;
			if ( isEnemyClan(pActor) )
			{
				nResult = SKILL_TARGET_ENEMY_PLAYER;
			}
			if ( isCrime(pActor) )
			{
				nResult = SKILL_TARGET_ENEMY_PLAYER;
			}
			if (  sameClan )
			{
				nResult = SKILL_TARGET_FRIEND_PLAYER;
			}
			break;

		default:
			assert(false);
		}
		if (nResult == SKILL_TARGET_ENEMY_PLAYER)
		{
			if (IsRookieProtect && pActor->getLv() < ROOKIE_PROTECT_LEVEL)
			{
				nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_MID_PLAYER;
			}
			else if( pMain->isInSafeOverlay() || pActor->isInSafeOverlay()  )
			{
				nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_PLAYER : SKILL_TARGET_MID_PLAYER;
			}
		}
		if (nResult != SKILL_TARGET_ENEMY_PLAYER)
		{
			if (strcmp(pMain->m_szPkTargetName, pActor->getName()) == 0)
			{
				nResult = SKILL_TARGET_ENEMY_PLAYER;
			}
		}
		if (sameTeam)
		{
			nResult |= SKILL_TARGET_TEAM_PLAYER;
		}
	}
	//临时处理
	else if (pActor->getType() == GAT_MONSTER)
	{
		if (pActor->m_PKInfoType == MON_PK_TYPE_NONE)
		{
			nResult = SKILL_TARGET_ENEMY_MON;
		}
		else if (pActor->m_PKInfoType == MON_PK_TYPE_PROTECT)
		{
			nResult = SKILL_TARGET_FRIEND_MON;
		}
		else if (pActor->m_PKInfoType == MON_PK_TYPE_PANISH)
		{
			MainGamePlayer* pActor = ActorManager::getSingleton().getMainPlayer();
			nResult = (pActor->m_nPkMode == PK_MODE_ALL) ? SKILL_TARGET_ENEMY_MON : SKILL_TARGET_FRIEND_MON;
		}
		else if( pActor->m_PKInfoType == MON_PK_TYPE_CLAN && sameClan && WorldCityManager::getSingleton().IsCityBuildFight(pActor) )
		{
			nResult = SKILL_TARGET_FRIEND_MON;
		}
		else if (!WorldCityManager::getSingleton().IsFighting() && WorldCityManager::getSingleton().IsCityBuildFight(pActor))
		{
			nResult = SKILL_TARGET_MID_MON;
		}
		else if ( pActor->getSubType() == MON_SUB_TYPE_RONGLU && pMain->isInSafeOverlay() )
		{
			nResult = SKILL_TARGET_MID_MON;
		}
		else if ( pActor->getSubType() == MON_SUB_TYPE_LOST_TOTEM && pMain->isInSafeOverlay() )
		{
			nResult = SKILL_TARGET_MID_MON;
		}
		else
		{
			switch (pkMode)
			{
			case PK_MODE_NORMAL:
				nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_MID_MON;
				break;

			case PK_MODE_POLICE:
				if (isCrime(pActor))
				{
					nResult = SKILL_TARGET_ENEMY_MON;
				}
				else
				{
					nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_MID_MON;
				}
				break;

			case PK_MODE_ALL:
				nResult = SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_TEAM:
				nResult = sameTeam ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_CLAN:
				nResult = sameClan ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_CAMP:
				nResult = sameGroup ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_ENEMY_CLAN:
				nResult = SKILL_TARGET_MID_MON;
				if ( isEnemyClan(pActor) )
				{
					nResult = SKILL_TARGET_ENEMY_MON;
				}
				if ( isCrime(pActor) )
				{
					nResult = SKILL_TARGET_ENEMY_MON;
				}
				if ( sameClan )
				{
					nResult = SKILL_TARGET_FRIEND_MON;
				}
				break;

			default:
				assert(false);
			}
		}
	}
	else if (pActor->getType() == GAT_PET)
	{
		if (pActor->getOwnerId() == pMain->GetID())
		{
			nResult = SKILL_TARGET_SELFPET;
		}
		else
		{
			switch (pkMode)
			{
			case PK_MODE_NORMAL:
				nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIENDPET : SKILL_TARGET_MIDPET;
				break;

			case PK_MODE_POLICE:
				if (isCrime(pActor))
				{
					nResult = SKILL_TARGET_ENEMYPET;
				}
				else
				{
					nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIENDPET : SKILL_TARGET_MIDPET;
				}
				break;

			case PK_MODE_ALL:
				nResult = SKILL_TARGET_ENEMYPET;
				break;

			case PK_MODE_TEAM:
				nResult = sameTeam ? SKILL_TARGET_FRIENDPET : SKILL_TARGET_ENEMYPET;
				break;

			case PK_MODE_CLAN:
				nResult = sameClan ? SKILL_TARGET_FRIENDPET : SKILL_TARGET_ENEMYPET;
				break;

			case PK_MODE_CAMP:
				nResult = sameGroup ? SKILL_TARGET_FRIENDPET : SKILL_TARGET_ENEMYPET;
				break;

			case PK_MODE_ENEMY_CLAN:
				nResult = SKILL_TARGET_MIDPET;
				if ( isEnemyClan(pActor) )
				{
					nResult = SKILL_TARGET_ENEMYPET;
				}
				if ( isCrime(pActor) )
				{
					nResult = SKILL_TARGET_ENEMYPET;
				}
				if ( sameClan )
				{
					nResult = SKILL_TARGET_FRIENDPET;
				}
				break;

			default:
				assert(false);
			}
			if (nResult != SKILL_TARGET_ENEMYPET)
			{
				GameActor *pOwner = ActorManager::getSingleton().FindActor(pActor->getOwnerId());
				if (pOwner && strcmp(pMain->m_szPkTargetName, pOwner->getName()) == 0)
				{
					nResult = SKILL_TARGET_ENEMYPET;
				}
			}
		}
	}
	else if (pActor->getType() == GAT_NPC)
	{
		if (pActor->m_PKInfoType == MON_PK_TYPE_NONE)
		{
			nResult = SKILL_TARGET_FRIEND_MON;
		}
		else if (pActor->m_PKInfoType == MON_PK_TYPE_PROTECT)
		{
			nResult = SKILL_TARGET_FRIEND_MON;
		}
		else if (pActor->m_PKInfoType == MON_PK_TYPE_PANISH)
		{
			MainGamePlayer* pActor = ActorManager::getSingleton().getMainPlayer();
			nResult = (pActor->m_nPkMode == PK_MODE_ALL) ? SKILL_TARGET_ENEMY_MON : SKILL_TARGET_FRIEND_MON;
		}
		else if (!WorldCityManager::getSingleton().IsFighting() && WorldCityManager::getSingleton().IsCityBuildFight(pActor))
		{
			nResult = SKILL_TARGET_MID_MON;
		}
		else
		{
			switch (pkMode)
			{
			case PK_MODE_NORMAL:
				nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_MID_MON;
				break;

			case PK_MODE_POLICE:
				if (isCrime(pActor))
				{
					nResult = SKILL_TARGET_ENEMY_MON;
				}
				else
				{
					nResult = (sameTeam || sameClan) ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_MID_MON;
				}
				break;

			case PK_MODE_ALL:
				nResult = SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_TEAM:
				nResult = sameTeam ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_CLAN:
				nResult = sameClan ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_CAMP:
				nResult = sameGroup ? SKILL_TARGET_FRIEND_MON : SKILL_TARGET_ENEMY_MON;
				break;

			case PK_MODE_ENEMY_CLAN:
				nResult = SKILL_TARGET_MID_MON;
				if ( isEnemyClan(pActor) )
				{
					nResult = SKILL_TARGET_ENEMY_MON;
				}
				if ( isCrime(pActor) )
				{
					nResult = SKILL_TARGET_ENEMY_MON;
				}
				if ( sameClan )
				{
					nResult = SKILL_TARGET_FRIEND_MON;
				}
				break;

			default:
				assert(false);
			}
		}
	}

	pActor->setObjCamp( nResult );
	if (oldResult != nResult)
	{
		pActor->getRoleText().BuildTitle();
	}
	if (pActor->getType() == GAT_PLAYER)
	{
		for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
		{
			GameActor *pet = pActor->getPet()->getPetActor(i);
			if( pet != NULL )
			{
				if (nResult & SKILL_TARGET_FRIEND_PLAYER)
				{
					pet->m_ObjCamp = SKILL_TARGET_FRIENDPET;
				}
				else if (nResult & SKILL_TARGET_ENEMY_PLAYER)
				{
					pet->m_ObjCamp = SKILL_TARGET_ENEMYPET;
				}
				else
				{
					pet->m_ObjCamp = SKILL_TARGET_MIDPET;
				}
			}
		}
	}
}

int GameActor_PVP::getPkMode()
{
	MainGamePlayer* pActor = ActorManager::getSingleton().getMainPlayer();
	if (pActor == NULL)
	{
		return PK_MODE_NORMAL;
	}
	DBDataManager& db = DBDataManager::getSingleton();
	tagMapDef const& mapDef = db.getMapDef(pActor->getMapId());
	if(mapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP)
	{
		return pActor->m_nPkMode;
	}
	if( pActor->m_nSafeOverlayFlag&SCENEOVERLAY_PK )
	{
		return pActor->m_nPkMode;
	}
	if (m_forcePkMode != -1)
	{
		return m_forcePkMode;
	}
	if (pActor->getLv() < ROOKIE_PROTECT_LEVEL)
	{
		GameMap *map = pActor->getCurMap();
		if (map)
		{
			const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(map->getRealId());
			if (mapdef.CtrlFlag & MAP_CTRL_ROOKIE_PROTECT)
			{
				return PK_MODE_NORMAL;
			}
		}
	}
	if (pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID != 0)
	{
	}
	return pActor->m_nPkMode;
}

void GameActor_PVP::forcePkMode(int mode)
{
	bool lastForcePeaceMode	= m_forcePkMode != -1 && m_forcePkMode != PK_MODE_NORMAL;
	m_forcePkMode = mode;
	bool curForcePeaceMode		= m_forcePkMode != -1 && m_forcePkMode != PK_MODE_NORMAL;
	ActorManager& actorMgr		= ActorManager::getSingleton();
	if ( lastForcePeaceMode != curForcePeaceMode )
	{
		actorMgr.updateAllPlayerTitleColor();
	}

	if (mode == -1)
	{
		actorMgr.getScriptVM()->callString("ForcePkMode()");
	}
	else
	{
		char szBuf[256];
		sprintf(szBuf, "ForcePkMode(%d)", mode);
		actorMgr.getScriptVM()->callString(szBuf);
	}
}

bool GameActor_PVP::isSameTeam(GameActor *pActor)
{
	if (pActor->getType() == GAT_MONSTER || pActor->getType() == GAT_PET)
	{
		int id = pActor->getOwnerId();
		if (id != 0)
		{
			return isSameTeam(id);
		}
	}
	MainGamePlayer* player = ActorManager::getSingleton().getMainPlayer();
	return player && (player == pActor || player->isInSameTeam(pActor->getTeamWID()));
}

bool GameActor_PVP::isSameTeam(int id)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (pActor != NULL)
	{
		return isSameTeam(pActor);
	}
	for (size_t i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if (m_idTeamMemberName[i] == id)
		{
			return true;
		}
	}
	return false;
}

bool GameActor_PVP::isSameTeam( tdr_ulonglong wid)
{
	GameActor* pActor = ActorManager::getSingleton().findActorByWId( wid );
	if( pActor != NULL )
	{
		return isSameTeam(pActor);
	}
	for (size_t i = 0; i < MAX_TEAM_MEMBER; ++i)
	{
		if( m_widTeamMemberName[i] == wid )
		{
			return true;
		}
	}
	return false;
}

bool GameActor_PVP::isSameClan(GameActor *pActor)
{
	if (pActor->getType() == GAT_MONSTER || pActor->getType() == GAT_PET)
	{
		int id = pActor->getOwnerId();
		GameActor *pOwner = ActorManager::getSingleton().FindActor(id);
		if (pOwner != NULL)
		{
			return isSameClan(pOwner);
		}
	}
	return pActor->isClanSame() || pActor->isClanAlly();
}

int GameActor_PVP::isSameCampIdx(GameActor *pActor)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (!pMainPlayer)
	{
		return 0;
	}
	int mapid = pMainPlayer->getMapRealId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef->CampNum < 1 )
		{
			return 0;
		}
	}/*
	else
	{
		return 0;
	}*/
	if (pActor->getType() == GAT_PET)
	{
		pActor = ActorManager::getSingleton().findActorByName(pActor->getOwnerName());
		if (pActor == NULL)
		{
			return 0;
		}
	}
	if ( pMainPlayer->isInPVPMap() && pActor->getType() == GAT_NPC ) {
		if ( pActor->m_nCampIdx > 0 && pMainPlayer->m_nCampIdx != pActor->m_nCampIdx ) {
			const tagMonsterDef &mondef = DBDataManager::getSingleton().getMonsterDef( pActor->getResId() );
			if ( mondef.AINormalSkills[0].SkillID != 0 ) {
				return -1;
			} else {
				return 1;
			}
		} else {
			return 1;
		}
	}

	return pMainPlayer->m_nCampIdx == pActor->m_nCampIdx ? 1 : -1;
}

bool GameActor_PVP::isSameGroup(GameActor *pActor)
{
	if (pActor->getType() == GAT_MONSTER || pActor->getType() == GAT_PET)
	{
		int id = pActor->getOwnerId();
		GameActor *pOwner = ActorManager::getSingleton().FindActor(id);
		if (pOwner != NULL)
		{
			return isSameGroup(pOwner);
		}
	}
	int ret = 0;
	for (std::list<IJudgeCampIdx *>::iterator i = m_judgerList.begin(); i != m_judgerList.end(); ++i)
	{
		ret = (*i)->isSameCampIdx(pActor);
		if (ret != 0)
		{
			break;
		}
	}
	if (ret == 0)
	{
		ret = isSameCampIdx( pActor );
	}
	return ret > 0;
}

bool GameActor_PVP::isEnemyClan(GameActor *pActor)
{
	if (pActor->getType() == GAT_MONSTER || pActor->getType() == GAT_PET)
	{
		int id = pActor->getOwnerId();
		GameActor *pOwner = ActorManager::getSingleton().FindActor(id);
		if (pOwner != NULL)
		{
			return isEnemyClan(pOwner);
		}
	}
	tdr_ulonglong clanId = pActor->getClanWID();
	for (size_t i = 0; i < m_nEnemyClanNum; ++i)
	{
		if (m_lstEnemyClan[i].GID == clanId)
		{
			return true;
		}
	}
	return false;
}

bool GameActor_PVP::isCrime(GameActor *pActor)
{
	if (pActor->getType() == GAT_MONSTER || pActor->getType() == GAT_PET)
	{
		int id = pActor->getOwnerId();
		GameActor *pOwner = ActorManager::getSingleton().FindActor(id);
		if (pOwner != NULL)
		{
			return isCrime(pOwner);
		}
	}
	return hasCrime(pActor) || getAmok(pActor) != 0;
}

void GameActor_PVP::setTeamMember(int id, const char *name, tdr_ulonglong wid)
{
	if (m_nTeamMemberNum < MAX_TEAM_MEMBER && name)
	{
		m_idTeamMemberName[m_nTeamMemberNum] = id;
		strncpy(m_strTeamMemberName[m_nTeamMemberNum], name, NAME_LEN);
		m_widTeamMemberName[m_nTeamMemberNum] = wid;
		++m_nTeamMemberNum;
	}
	else
	{
		assert(false && "m_nTeamMemberNum 太大");
	}
}

void GameActor_PVP::resetTeamMember()
{
	m_nTeamMemberNum = 0;
	memset(m_idTeamMemberName, 0, sizeof(m_idTeamMemberName));
	memset(m_strTeamMemberName, 0, sizeof(m_strTeamMemberName));
	memset(m_widTeamMemberName, 0, sizeof(m_widTeamMemberName));
}

void GameActor_PVP::setEnemyClan(CLANID enemy)
{
	if (m_nEnemyClanNum == CLAN_MAX_ENEMY)
	{
		m_lstEnemyClan[CLAN_MAX_ENEMY - 1] = enemy;
	}
	else
	{
		m_lstEnemyClan[m_nEnemyClanNum++] = enemy;
	}
}

void GameActor_PVP::resetEnemyClan()
{
	m_nEnemyClanNum = 0;
}

void GameActor_PVP::changePKMode(int mode)
{
	tagCSPkgBody msgbody;
	msgbody.PKClt.PKCltType = PK_MODE_CHG;
	msgbody.PKClt.PKCltData.PKMode = mode;
	SendGameServerMsg( PK_CLT, msgbody );
	char szScripts[256];
	sprintf( szScripts,"ChangePKModeTips(%d)",mode );
	ActorManager::getSingleton().getScriptVM()->callString( szScripts );
}

void GameActor_PVP::changeNextPkMode()
{
	static const int modes[] = {
		PK_MODE_NORMAL,
		PK_MODE_POLICE,
		PK_MODE_ALL,
		PK_MODE_TEAM,
		PK_MODE_CLAN,
		PK_MODE_CAMP,
		PK_MODE_ENEMY_CLAN,
	};
	MainGamePlayer* pActor = ActorManager::getSingleton().getMainPlayer();
	for (int i = 0; i < sizeof(modes) / sizeof(modes[0]); ++i)
	{
		if (modes[i] == pActor->m_nPkMode)
		{
			//因为pk为5的阵营模式未开放，这里特殊处理下
			if ( i == 4 )
			{
				i = i + 1;
			}
			int mode = modes[(i + 1) % (sizeof(modes) / sizeof(modes[0]))];
			changePKMode(mode);
			break;
		}
	}
}

void GameActor_PVP::onPkValueChanged(int value)
{
	char szBuf[256];
	sprintf(szBuf, "PkValueChanged(%d)", value);
	ActorManager::getSingleton().getScriptVM()->callString(szBuf);
}

void GameActor_PVP::onPkModeChanged(int mode)
{
	char szBuf[256];
	sprintf(szBuf, "PkModeChanged(%d)", mode);
	ActorManager::getSingleton().getScriptVM()->callString(szBuf);
}

void GameActor_PVP::registerCampIdxJudger(IJudgeCampIdx *handle)
{
	m_judgerList.push_back(handle);
}

void GameActor_PVP::requestSafePK( const char* szRoleName )
{
	tagCSPkgBody msgbody;
	msgbody.InviteClt.Cmd = TEST_CLT_INVITE;
	strcpy(msgbody.InviteClt.InviteTestClt.TestInvite.RoleName,szRoleName);
	SendGameServerMsg(INVITE_TEST_CLT,msgbody);
}

void GameActor_PVP::requestSafePKRes( int nFlag,const char* szRoleName )
{
	tagCSPkgBody msgbody;
	msgbody.InviteClt.Cmd = TEST_CLT_RES;
	msgbody.InviteClt.InviteTestClt.TestCltRes.Flag = nFlag;
	strcpy(msgbody.InviteClt.InviteTestClt.TestCltRes.RoleName,szRoleName);
	SendGameServerMsg(INVITE_TEST_CLT,msgbody);
}

bool GameActor_PVP::hasCrime(GameActor *pActor)
{
	return pActor->m_leastNoCrimeTime > timeGetTime();
}

int GameActor_PVP::getAmok(GameActor *pActor)
{
	return getAmok(pActor->m_nPkValue);
}

int GameActor_PVP::getAmok(int amok)
{
	if (amok >= PLAYER_PK_PUBLIC_ENEMY)
	{
		return 2;
	}
	else if (amok >= PLAYER_PK_FRENZY)
	{
		return 1;
	}

	return 0;
}

bool GameActor_PVP::isForcePkMode()
{
	return m_forcePkMode != -1;
}

int GameActor_PVP::getPkValue()
{
	MainGamePlayer* pActor = ActorManager::getSingleton().getMainPlayer();
	return pActor->m_nPkValue;
}
