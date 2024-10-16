#include "GameBattle.h"
#include "GameDBData.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "GameMap.h"
#include "GameUIMsg.h"
#include "GameLoginManager.h"
#include "OgreUIRenderer.h"
#include "OgreScriptLuaVM.h"

static const int BAN_TEAM[] = {
	BATTLE_FIGHT_TYPE_CAMP_LIFT,
	BATTLE_FIGHT_TYPE_RAND_LIFT,
	BATTLE_FIGHT_TYPE_XUKONG,
};

bool operator< ( const CSBATTLEROLEINFO& lhs, const CSBATTLEROLEINFO& rhs )
{
	switch (lhs.FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return lhs.FightInfo.AoShanInfo.PolyFraction > rhs.FightInfo.AoShanInfo.PolyFraction;
	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return lhs.FightInfo.CampLiftInfo.PolyFraction > rhs.FightInfo.CampLiftInfo.PolyFraction;
	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return lhs.FightInfo.RandLiftInfo.PolyFraction > rhs.FightInfo.RandLiftInfo.PolyFraction;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return lhs.FightInfo.XuKongInfo.PolyFraction > rhs.FightInfo.XuKongInfo.PolyFraction;
	default:
		break;
	}
	return lhs.EnterTime > rhs.EnterTime;
}

GameBattle::GameBattle(void)
{
	reset();
	INetworkManager::getSingleton().RegisterMsgHandler(BATTLE_SVR, this);
	INetworkManager::getSingleton().RegisterMsgHandler(PWORLD_SVR, this);
	//GameActor_PVP::getSingleton().registerCampIdxJudger(this);

	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	fontTexture[0]					= prenderer->CreateTexture( "uires\\ui\\Za\\ShengCunDaMaoXianBiaoShi\\HongSeZhenYing.tga", 
		Ogre::BLEND_ALPHABLEND );
	fontTexture[1]					= prenderer->CreateTexture( "uires\\ui\\Za\\ShengCunDaMaoXianBiaoShi\\LanSeZhenYing.tga", 
		Ogre::BLEND_ALPHABLEND );
	fontTexture[2]					= prenderer->CreateTexture( "uires\\ui\\Za\\ShengCunDaMaoXianBiaoShi\\HuangSeZhenYing.tga", 
		Ogre::BLEND_ALPHABLEND );
	RoleText::registTitleDrawer(this);
}

GameBattle::~GameBattle(void)
{
}

void GameBattle::reset()
{
	m_BattleType = 0;
	m_RoleNum = 0;
	m_VCamp = 0;
	m_Pworld = 0;
	m_roleIdx.clear();
	ActorManager *actorMgr = ActorManager::getSingletonPtr();
	if (actorMgr != NULL)
	{
		MainGamePlayer *pActor = actorMgr->getMainPlayer();
		if (pActor != NULL)
		{
			pActor->m_nCampIdx = 0;
		}
	}
	memset( &m_BattleMatchStatus, 0, sizeof(tagCSPworldMatchStatus) );
	memset( &m_BattleCampInfo, 0, sizeof(ClanWarCampInfo) );
}

int GameBattle::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (BATTLE_SVR == msgid)
	{
		const tagCSBattleSvrData &data = msgbody.BattleSvr.SvrData;
		switch (msgbody.BattleSvr.SvrType)
		{
		case BATTLE_SVR_INFO:
			{
				if (BATTLE_OP_TYPE_SELF == data.BattleInfo.OpType)
				{
					for (size_t i = 0; i < data.BattleInfo.RoleNum; ++i)
					{
						int id = data.BattleInfo.RoleList[i].MemID;
						std::map<int, size_t>::iterator iter = m_roleIdx.find(id);
						if (iter == m_roleIdx.end())
						{
							m_roleIdx[id] = m_RoleNum++;
							iter = m_roleIdx.find(id);
						}
						m_lstRole[iter->second] = data.BattleInfo.RoleList[i];
					}
				}
				else
				{
					if ( BATTLE_OP_TYPE_UPDATE_ENERGY == data.BattleInfo.OpType )
					{
						SetEnergyPoint( data.BattleInfo.XuKongVal1, data.BattleInfo.XuKongVal2 );
						return 0;
					}
					m_RoleNum = data.BattleInfo.RoleNum;
					memcpy(m_lstRole, data.BattleInfo.RoleList, data.BattleInfo.RoleNum * sizeof(CSBATTLEROLEINFO));
					if( data.BattleInfo.OpType != BATTLE_OP_TYPE_END )
					{
						stable_sort( m_lstRole,m_lstRole+m_RoleNum );
					}
					m_roleIdx.clear();
					for (size_t i = 0; i < m_RoleNum; ++i)
					{
						m_roleIdx[m_lstRole[i].MemID] = i;
					}
				}
				switch (data.BattleInfo.OpType)
				{
				case BATTLE_OP_TYPE_SELF:
					break;
				case BATTLE_OP_TYPE_UPDATE:
					{
						MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
						if (pActor)
						{
							pActor->m_nCampIdx = m_lstRole[m_roleIdx[pActor->GetID()]].CampIdx;
						}
					}
					break;

				case BATTLE_OP_TYPE_END:
					{
						m_VCamp = data.BattleInfo.VCamp;
						MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
						if (pActor)
						{
							pActor->lockControl();
						}
						GameManager::getSingleton().SendUIEvent("GE_BATTLE_END");
					}
					break;
				case BATTLE_OP_TYPE_UPDATE_ENERGY:
					break;
				}
				//虚空要塞比分信息
				SetEnergyPoint( data.BattleInfo.XuKongVal1, data.BattleInfo.XuKongVal2 );

				//死亡之星阵营信息
				for( int i = 0; i < data.BattleInfo.CampNum; i++ )
				{
					strcpy( m_BattleCampInfo[i].CampName, data.BattleInfo.CampInfo[i].CampName );
					m_BattleCampInfo[i].CampID = data.BattleInfo.CampInfo[i].CampID;
					m_BattleCampInfo[i].CampVal = data.BattleInfo.CampInfo[i].CampVal;
				}
				
				GameManager::getSingleton().SendUIEvent("GE_BATTLE_INFO");
			}
			break;
		}
	}
	else if (PWORLD_SVR == msgid)
	{
		switch (msgbody.PworldSvr.SvrType)
		{
		case PWORLD_SVR_ENTER_QUEUE:
			{
				m_PworldQueue.insert(msgbody.PworldSvr.SvrData.EnterPworldQueue.PworldID);
				const tagPworldDef *pWorldDef = DBDataManager::getSingleton().getPworldDef(msgbody.PworldSvr.SvrData.EnterPworldQueue.PworldID);
				tagUIMsgBody uiMsg;
				if( pWorldDef != NULL )
				{
					strcpy(uiMsg.PworldInfo.PworldName, pWorldDef->Name);
				}
				uiMsg.PworldInfo.MatchType = pWorldDef->MatchType;
				uiMsg.PworldInfo.MinPlayerNum = pWorldDef->MinPlayerNum;
				uiMsg.PworldInfo.PworldID = msgbody.PworldSvr.SvrData.EnterPworldQueue.PworldID;
				GameUIMsg::getSingleton().setUIMsg(uiMsg);
				GameManager::getSingleton().SendUIEvent("GE_EXISTENCEEXP_WAIT");
			}
			break;
		case PWORLD_SVR_INVITE_ENTER_PWORLD:
			{
				m_Pworld = msgbody.PworldSvr.SvrData.InviteEnterPworld.PworldID;
				tagUIMsgBody uiMsg;
				uiMsg.PworldInfo.PworldID	= m_Pworld;
				uiMsg.PworldInfo.PworldIdx	= msgbody.PworldSvr.SvrData.InviteEnterPworld.PworldIdx;
				const tagPworldDef* pWorldDef = DBDataManager::getSingleton().getPworldDef( msgbody.PworldSvr.SvrData.InviteEnterPworld.PworldID );
				if( pWorldDef != NULL )
				{
					strcpy(uiMsg.PworldInfo.PworldName, pWorldDef->Name);
				}
				GameUIMsg::getSingleton().setUIMsg(uiMsg);
				GameManager::getSingleton().SendUIEvent("GE_EXISTENCEEXP_INVITE");
			}
			break;
		case PWORLD_SVR_EXIT_PWORLD_QUEUE:
			{
				std::set<int>::iterator i = m_PworldQueue.find(msgbody.PworldSvr.SvrData.ExitPworldQueue.PworldID);
				if (i != m_PworldQueue.end())
				{
					m_PworldQueue.erase(i);
				}
				if (m_Pworld == msgbody.PworldSvr.SvrData.ExitPworldQueue.PworldID)
				{
					m_Pworld = 0;
				}
				GameManager::getSingleton().SendUIEvent("GE_EXISTENCEEXP_NOWAIT");
			}
			break;
		case PWORLD_SVR_MATCH:
			{
				m_BattleMatchStatus = msgbody.PworldSvr.SvrData.MatchStatus;
			}
			break;
		case PWORLD_SVR_APPLY_INVITE:
			{
				char buffer[256];
				sprintf( buffer, "OpenClanWarPresentInvite( %d )", msgbody.PworldSvr.SvrData.ApplyInvite.BattleID );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;
		}
	}
	return 0;
}

unsigned int GameBattle::getBattleNum()
{
	return (unsigned int)BattleLimitInfo::getSingleton().getBattleNum();
}

const char * GameBattle::getBattleDesc(unsigned int i)
{
	return BattleLimitInfo::getSingleton().getBattleDef(i)->BattleDesc;
}

const char * GameBattle::getBattleWin(unsigned int i)
{
	return BattleLimitInfo::getSingleton().getBattleDef(i)->BattleWinDesc;
}

int	GameBattle::getBattleCamp(unsigned int i)
{
	int pworldId = getBattlePworld(i);
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
	if (pworldDef)
	{
		return pworldDef->CampNum;
	}
	return 0;
}

int GameBattle::getBattlePlayerMax(unsigned int i)
{
	int pworldId = getBattlePworld(i);
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
	if (pworldDef)
	{
		return pworldDef->PlayerNum;
	}
	return 0;
}

const char * GameBattle::getBattleName(unsigned int i)
{
	return BattleLimitInfo::getSingleton().getBattleDef(i)->Name;
}

int	GameBattle::getBattleMinLv(unsigned int i)
{
	return BattleLimitInfo::getSingleton().getBattleDef(i)->BattleQueue[0].MinLevel;
}

int GameBattle::getBattlePworld(unsigned int i)
{
	return BattleLimitInfo::getSingleton().getBattleDef(i)->BattleQueue[0].PworldID;
}

int GameBattle::getBattleTexture(unsigned int i)
{
	int pworldId = getBattlePworld(i);
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
	if (pworldDef)
	{
		const tagMapDef &mapDef = DBDataManager::getSingleton().getMapDef(pworldDef->InitMapID);
		return mapDef.ReferMapID ? mapDef.ReferMapID : mapDef.MapID;
	}
	return 0;
}

int GameBattle::getBattleID(unsigned int i)
{
	return BattleLimitInfo::getSingleton().getBattleDef(i)->BattleID;
}

void GameBattle::join(int id, int nType )
{
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_JOIN_QUEUE;
	msgbody.PworldClt.CltData.JoinQueue.BattleID = id;
	msgbody.PworldClt.CltData.JoinQueue.TeamJoin = nType;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

unsigned int GameBattle::getPlayerIdx(int id)
{
	std::map<int, size_t>::iterator i = m_roleIdx.find(id);
	if (i != m_roleIdx.end())
	{
		return i->second;
	}
	return 0;
}

unsigned int GameBattle::getPlayerNum()
{
	return m_RoleNum;
}

int GameBattle::getPlayerID(unsigned int i)
{
	return m_lstRole[i].MemID;
}

const char * GameBattle::getPlayerName(unsigned int i)
{
	return m_lstRole[i].RoleName;
}

const char * GameBattle::getPlayerClan(unsigned int i)
{
	return m_lstRole[i].ClanName;
}

int GameBattle::getPlayerCarrer(unsigned int i)
{
	return m_lstRole[i].Career;
}

int GameBattle::getPlayerCamp(unsigned int i)
{
	return m_lstRole[i].CampIdx;
}

int GameBattle::getPlayerKill(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.Kill;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.MaxKill;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.MaxKill;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.Kill;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.Kill;
	}
	return 0;
}

int GameBattle::getPlayerReward(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.BrandType;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.BrandType;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.BrandType;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.BrandType;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.BattleAwd.BrandType;
	}
	return 0;
}

int GameBattle::getPlayerRewardNum(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.TotalBrandVal;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.TotalBrandVal;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.TotalBrandVal;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.TotalBrandVal;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.BattleAwd.TotalBrandVal;
	}
	return 0;
}

int GameBattle::getPlayerRewardEx(unsigned int i)
{
	return 0;
}

int GameBattle::getPlayerRewardNumEx(unsigned int i)
{
	return 0;
}

int GameBattle::getPlayerCrystal(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.Crystal;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.Crystal;
	}
	return 0;
}

int GameBattle::getPlayerOre(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.Ore;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.Ore;
	}
	return 0;
}

int GameBattle::getPlayerAssist(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.AssistKill;
	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.AssistKill;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.AssistKill;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.AssistKill;
	}
	return 0;
}

int GameBattle::getPlayerMaxContinueKill(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.MaxContinueKill;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.MaxContinueKill;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.MaxContinueKill;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.MaxContinueKill;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.MaxContinueKill;
	}
	return 0;
}

int GameBattle::getPlayerDieNum(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.DieNum;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.DieNum;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.DieNum;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.DieNum;
	}
	return 0;
}

int GameBattle::getPlayerMaxLife(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.MaxLife;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.MaxLife;
	}
	return 0;
}

int GameBattle::getPlayerPolyFraction(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.PolyFraction;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.PolyFraction;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.PolyFraction;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.PolyFraction;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.PolyFraction;
	}
	return 0;
}

int GameBattle::getPlayerExtraExp(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.Exp + m_lstRole[i].FightInfo.AoShanInfo.FirstWinExp;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.Exp + m_lstRole[i].FightInfo.CampLiftInfo.FirstWinExp;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.Exp + m_lstRole[i].FightInfo.RandLiftInfo.FirstWinExp;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.Exp + m_lstRole[i].FightInfo.XuKongInfo.FirstWinExp;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.Exp;
	}
	return 0;
}

int GameBattle::isSameCampIdx(GameActor *pActor)
{
	MainGamePlayer *mainplayer = ActorManager::getSingleton().getMainPlayer();
	return pActor->m_nCampIdx == mainplayer->m_nCampIdx;
	//return 0;
}

bool GameBattle::isRun()
{
	return m_BattleType != 0;
}

void GameBattle::onEnterPworld(int mapid)
{
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(mapid / 100);
	if (m_BattleType != pworldDef->BattleID)
	{
		m_BattleType = pworldDef->BattleID;
		if (m_BattleType > 0)
		{
			MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			if (pMainPlayer != NULL)
			{
				pMainPlayer->RefreshCmdTime();
			}
			GameManager::getSingleton().SendUIEvent("GE_BATTLE_ENTER");
		}
	}
}

void GameBattle::onLeavePworld(int mapid)
{
	if (isRun())
	{
		GameManager::getSingleton().SendUIEvent("GE_BATTLE_LEAVE");
		reset();
		MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
		if (pActor && !pActor->canControl())
		{
			pActor->releaseControl();
		}
	}
}

void GameBattle::EnterPworld()
{
	if (m_Pworld == 0)
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_ENTER_PWORLD;
	msgbody.PworldClt.CltData.EnterPworldInst.PworldID = m_Pworld;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

void GameBattle::LeavePworld()
{
	if (!isRun())
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_EXIT_PWORLD;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

void GameBattle::ExitQueue( int teamJoinType )
{
	if (m_PworldQueue.empty())
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_EXIT_QUEUE;
	msgbody.PworldClt.CltData.ExitPworldQueue.PworldID = *(m_PworldQueue.begin());
	msgbody.PworldClt.CltData.ExitPworldQueue.TeamJoin = teamJoinType;
	SendGameServerMsg(PWORLD_CLT, msgbody);
}

int GameBattle::getCurrentBattleId()
{
	return m_BattleType;
}

void * GameBattle::GetTitleIcon(GameActor *pActor)
{
	if (!isRun())
	{
		return NULL;
	}
	int campIdx = pActor->m_nCampIdx;
	if (campIdx >= 1 && campIdx <=3)
	{
		return fontTexture[campIdx - 1];
	}
	return NULL;
}

int GameBattle::GetPriority()
{
	return 1;
}

bool GameBattle::isWaiting()
{
	return !m_PworldQueue.empty();
}

void GameBattle::clearWaitings()
{
	m_PworldQueue.clear();
}

void GameBattle::objEnterView(GameActor *actor)
{
	if( actor->getServerId() != -1 )
	{
		actor->setActorWorldName( LoginManager::getSingleton().getWorldNameById(actor->getServerId()) );
	}

	if (!isRun())
	{
		return;
	}

	tagUIMsgBody uiMsg;
	uiMsg.Who.type = actor->getType();
	uiMsg.Who.id = actor->GetID();
	GameUIMsg::getSingleton().setUIMsg(uiMsg);
	GameManager::getSingleton().SendUIEvent("GE_BATTLE_OBJ_ENTER");
}

void GameBattle::objLeaveView(GameActor *actor)
{
	if (!isRun())
	{
		return;
	}
	tagUIMsgBody uiMsg;
	uiMsg.Who.type = actor->getType();
	uiMsg.Who.id = actor->GetID();
	GameUIMsg::getSingleton().setUIMsg(uiMsg);
	GameManager::getSingleton().SendUIEvent("GE_BATTLE_OBJ_LEAVE");
}

bool GameBattle::isTeamInBattle(int id)
{
	static std::set<int> BanTeamBattles;
	if (BanTeamBattles.size()  == 0)
	{
		for (size_t i = 0; i < sizeof(BAN_TEAM) / sizeof(*BAN_TEAM); ++i)
		{
			BanTeamBattles.insert(BAN_TEAM[i]);
		}
	}
	return BanTeamBattles.end() ==
		BanTeamBattles.find(BattleLimitInfo::getSingleton().getBattleDefById(id)->FightType);
}

int GameBattle::GetBattleActiveId(int id)
{
	int eventid = BattleLimitInfo::getSingleton().getBattleDefById(id)->EventLimit;
	if (eventid == 0)
	{
		return 0;
	}
	const tagGameEventDef *pGameEventDef = DBDataManager::getSingleton().getGameEventDef(eventid);
	if (pGameEventDef == NULL)
	{
		return 0;
	}
	return pGameEventDef->ReferActiveID;
}

bool GameBattle::isWinGame()
{
	int nIdx = getPlayerIdx( ActorManager::getSingleton().getMainPlayer()->GetID() );
	return ( m_VCamp != 0 && m_VCamp == getPlayerCamp(nIdx) );
}

int GameBattle::getPlayerWinGlory(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.VGlory;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.VGlory;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.VGlory;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.VGlory;
	}
	return 0;
}

int GameBattle::getPlayerRankGlory(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.PosGlory;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.PosGlory;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.PosGlory;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.PosGlory;
	}
	return 0;
}

int GameBattle::getPlayerNumGlory(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.NumGlory;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.NumGlory;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.NumGlory;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.NumGlory;
	}
	return 0;
}


int GameBattle::getPlayerFailGlory(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.FGlory;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.FGlory;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.FGlory;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.FGlory;
	}
	return 0;
}

int GameBattle::getPlayerExp(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.Exp;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.Exp;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.Exp;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.Exp;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.Exp;
	}
	return 0;
}

int GameBattle::getPlayerFristWinExp(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.FirstWinExp;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.FirstWinExp;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.FirstWinExp;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.FirstWinExp;
	}
	return 0;
}

int GameBattle::getPlayerFiniNum(unsigned int i)
{
	switch (m_lstRole[i].FightType)
	{
	case BATTLE_FIGHT_TYPE_AOSHAN:
		return m_lstRole[i].FightInfo.AoShanInfo.BattleAwd.FiniNum;

	case BATTLE_FIGHT_TYPE_CAMP_LIFT:
		return m_lstRole[i].FightInfo.CampLiftInfo.BattleAwd.FiniNum;

	case BATTLE_FIGHT_TYPE_RAND_LIFT:
		return m_lstRole[i].FightInfo.RandLiftInfo.BattleAwd.FiniNum;
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.BattleAwd.FiniNum;
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.BattleAwd.FiniNum;
	}
	return 0;
}

int GameBattle::getTotalCanFiniNum()
{
	if( m_BattleType == 0 )
	{
		return 0;
	}
	return BattleLimitInfo::getSingleton().getBattleDefById(m_BattleType)->BrandAwd.EndNum;
}

int GameBattle::getMainPlayerCurKill()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if ( pMain != NULL )
	{
		int nIndex = getPlayerIdx( pMain->GetID() );
		switch (m_lstRole[nIndex].FightType)
		{
		case BATTLE_FIGHT_TYPE_AOSHAN:
			return m_lstRole[nIndex].FightInfo.AoShanInfo.CurrKill;

		case BATTLE_FIGHT_TYPE_CAMP_LIFT:
			return m_lstRole[nIndex].FightInfo.CampLiftInfo.CurrKill;

		case BATTLE_FIGHT_TYPE_RAND_LIFT:
			return m_lstRole[nIndex].FightInfo.RandLiftInfo.CurrKill;
		case BATTLE_FIGHT_TYPE_XUKONG:
			return m_lstRole[nIndex].FightInfo.XuKongInfo.CurrKill;
		case BATTLE_FIGHT_TYPE_CLAN_WAR:
			return m_lstRole[nIndex].FightInfo.ClanWarInfo.CurrKill;
		}
	}	
	return 0;
}

int GameBattle::getPlayerEnergy( unsigned int i )
{
	switch ( m_lstRole[i].FightType )
	{
	case BATTLE_FIGHT_TYPE_XUKONG:
		return m_lstRole[i].FightInfo.XuKongInfo.Contri;
	}
	return 0;
}

const tagXuKongPosDef_ToLua* GameBattle::getXuKongPosDef( int nCamp, int nPosIdx )
{
	static_assert( sizeof(tagXuKongPosDef_ToLua) == sizeof(tagXuKongPosDef) );
	return (const tagXuKongPosDef_ToLua*)DBDataManager::getSingleton().getXuKongPosDef( nCamp, nPosIdx );
}

void GameBattle::SetEnergyPoint( int val1, int val2 )
{
	char szbuf[256];
	sprintf(szbuf,"UpdateXuKongEnergy(%d,%d)",val1,val2);
	ActorManager::getSingleton().getScriptVM()->callString(szbuf);
}

int GameBattle::getBattlePworldById( int id )
{
	return BattleLimitInfo::getSingleton().getBattleDefById(id)->BattleQueue[0].PworldID;
}

int GameBattle::getPlayerBull( unsigned int i )
{
	switch ( m_lstRole[i].FightType )
	{
	case BATTLE_FIGHT_TYPE_CLAN_WAR:
		return m_lstRole[i].FightInfo.ClanWarInfo.Bull;
	}
	return 0;
}

bool GameBattle::isNeedClanLeaderOpenApply( int id )
{
	if ( BattleLimitInfo::getSingleton().getBattleDefById(id)->FightType == BATTLE_FIGHT_TYPE_CLAN_WAR )
	{
		return true;
	}
	return false;
}

void GameBattle::requestClanWarMatch( int id )
{
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_MATCH;
	msgbody.PworldClt.CltData.Match.PworldID = id;
	SendGameServerMsg( PWORLD_CLT, msgbody );
}

void GameBattle::requestClanWarMatchCancle( int id )
{
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_MATCH_CANCEL;
	msgbody.PworldClt.CltData.MatchCancel.PworldID = id;
	SendGameServerMsg( PWORLD_CLT, msgbody );
}
void GameBattle::requestClanWarApplyInvite( int id, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.PworldClt.CltType = PWORLD_CLT_APPLY_INVITE;
	msgbody.PworldClt.CltData.ApplyInvite.BattleID = id;
	strcpy( msgbody.PworldClt.CltData.ApplyInvite.RoleName, szName );
	SendGameServerMsg( PWORLD_CLT, msgbody );
}

int GameBattle::getClanWarMatchStatus()
{
	return m_BattleMatchStatus.MatchStatus;
}

int GameBattle::getClanWarMatchPlayerNum()
{
	return m_BattleMatchStatus.PlayerNum;
}

int GameBattle::getClanWarMatchPworldID()
{
	return m_BattleMatchStatus.PworldID;
}

int GameBattle::getClanWarCampId( int idx )
{
	return m_BattleCampInfo[idx].CampID;
}
int GameBattle::getClanWarCampVal( int idx )
{
	return m_BattleCampInfo[idx].CampVal;
}
const char* GameBattle::getClanWarCampName( int idx )
{
	return m_BattleCampInfo[idx].CampName;
}

void GameBattle::setClanBattleApplyInfo( int id, int OpType )
{
	m_ClanBattleApplyInfo.BattleID = id;
	m_ClanBattleApplyInfo.OpType = OpType;
	//当通知活动关闭的时候，清空匹配信息
	if ( OpType == 0 )
	{
		memset( &m_BattleMatchStatus, 0, sizeof(tagCSPworldMatchStatus) );
	}
}

bool GameBattle::isClanBattleOpen( int id )
{
	if ( m_ClanBattleApplyInfo.BattleID == id )
	{
		return m_ClanBattleApplyInfo.OpType == 1?true:false;
	}
	return false;
}

void GameBattle::setClanWarCampHp( int idx, int hp )
{
	m_BattleCampInfo[idx].CampHp = hp;
}

int GameBattle::getClanWarCampHp( int idx )
{
	return m_BattleCampInfo[idx].CampHp;
}
void GameBattle::setClanWarCampMaxHp( int idx, int maxHp )
{
	m_BattleCampInfo[idx].CampHpMax = maxHp;
}

int GameBattle::getClanWarCampMaxHp( int idx )
{
	return m_BattleCampInfo[idx].CampHpMax;
}