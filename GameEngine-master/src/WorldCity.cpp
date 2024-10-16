#include "WorldCity.h"
#include "NetMessage.h"
#include "GameDBData.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "GameActorManager.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "AvatarEntity.h"
#include "OgreSoundSystem.h"
#include "GameActor_RoleText.h"
#include "OgreCamera.h"
#include "GameCamera.h"
#include "OgreUIRenderer.h"
#include "../UILib/ui_framemgr.h"

using namespace Ogre;

WorldCityManager::WorldCityManager() : Effigy(NULL)
{
	INetworkManager::getSingleton().RegisterMsgHandler(CITY_SVR, this);
	reset();
}

WorldCityManager::~WorldCityManager()
{

}

WorldCityManager & WorldCityManager::getSingleton()
{
	static WorldCityManager singleton;
	return singleton;
}

int WorldCityManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (msgid == CITY_SVR)
	{
		const tagCSCitySvrData &data = msgbody.CitySvr.Data;
		switch (msgbody.CitySvr.Type)
		{
		case CITY_SVR_MAP_INFO:
			{
				setFightStat(data.MapInfo.Stat);
				setCityBuildInfo(data.MapInfo.BuildNum, data.MapInfo.BuildList);
				setCaptureInfo(data.MapInfo.CaptureInfo.CaptureEndTime, data.MapInfo.CaptureInfo.ClanName);
			}
			break;

		case CITY_SVR_BUILD_UI_INFO:
			{
				setCityBuildInfo(data.UIInfo.BuildNum, data.UIInfo.BuildList);
				char script[100];
				sprintf(script, "OpenWorldCityBuildFrame(%d)", data.UIInfo.NpcID);
				ActorManager::getSingleton().getScriptVM()->callString(script);
			}
			break;

		case CITY_SVR_BUILD_WARNING:
			{
				BuildInfo[BuildInfoIdx[data.Warning.DefID]].WarnEndTime = data.Warning.WarnEndTime;
				GameMsgPrint::getSingleton().SendUIEvent("GE_CITYBUILD_WARNING");
			}
			break;

		case CITY_SVR_BUILD_CAPTURE:
			{
				setCaptureInfo(data.Capture.CaptureEndTime, data.Capture.ClanName);
			}
			break;

		case CITY_SVR_CORE_INFO:
			{
				WorldCityCoreInfo = data.CoreInfo;
				FightStat = data.CoreInfo.Stat;
				ActorManager::getSingleton().setClanFengHao();
				ActorManager::getSingleton().updateAllPlayerTitleInfo();
				UpdateWorldCityClanName();
				ClearEffigy();
				UpdateEffigy();
				GameMsgPrint::getSingleton().SendUIEvent("GE_WORLDCITY_INFO");
			}
			break;

		case CITY_SVR_FIGHT:
			{
				if (data.Fight.Stat == WORLD_CITY_STAT_FIGHT)
				{
					Ogre::SoundSystem::getSingleton().playSound2D("sounds\\sound\\haojiao1.wav", 1);
				}
				setFightStat(data.Fight.Stat);
			}
			break;
		case CITY_SVR_MAP_MEM_POS:
			{
				static_assert(sizeof(tagCSCityMapPosInfo_ToLua) == sizeof(tagCSCityMapPosInfo) );
				if( ActorManager::getSingleton().getMainPlayer()->isInClan() )
				{
					ActorManager::getSingleton().getScriptVM()->callFunction("UpdateClanCityMemberPos","u[tagCSCityMapPosInfo_ToLua]",
						&data.MapPosInfo);
				}				
			}
			break;
		case CITY_SVR_FIGHT_RANGE:
			{
				m_CityClanFightInfo = data.ClanFightRange;
				if ( data.ClanFightRange.End == 1 )
				{
					ActorManager::getSingleton().getScriptVM()->callString("WorldCityWarEnd()");
					ActorManager::getSingleton().getScriptVM()->callString("SetWorldCityWarInfoFrameWinInfo()");
				}
				else
				{
					ActorManager::getSingleton().getScriptVM()->callString("WorldCityWarBegin()");
				}
			}
			break;
		case CITY_SVR_KILL_INFO:
			{
				m_KillInfo = data.KillInfo;
				ActorManager::getSingleton().getScriptVM()->callString("WorldCityWarBegin()");
			}
			break;
		}
	}
	return 0;
}

void WorldCityManager::setCityBuildInfo(size_t num, const tagCSCityBuildInfo *info)
{
	BuildInfo.resize(num);
	BuildInfoIdx.clear();
	ActorManager::getSingleton().getScriptVM()->callString("ResetWorldCityFightCaptureBuild()");
	for (size_t i = 0; i < num; ++i)
	{
		BuildInfo[i].DefID = info[i].DefID;
		BuildInfo[i].MemID = info[i].MemID;
		BuildInfo[i].State = info[i].State;
		BuildInfo[i].Type = info[i].Type;
		BuildInfo[i].BuildIdx = info[i].BuildIdx;
		BuildInfo[i].BuildLevel = info[i].BuildLevel;
		BuildInfo[i].HP = info[i].HP;
		BuildInfo[i].MaxHP = info[i].MaxHP;
		BuildInfo[i].Energy = info[i].Energy;
		BuildInfo[i].MaxEnergy = info[i].MaxEnergy;
		BuildInfo[i].WarnEndTime = info[i].WarnEndTime;
		strcpy(BuildInfo[i].ClanName, info[i].ClanName);

		BuildInfoIdx[info[i].DefID] = i;

		if (info[i].Type == CITY_BUILD_TYPE_CONTROL || info[i].Type == CITY_BUILD_TYPE_REVIVE)
		{
			char script[100 + NAME_LEN];
			sprintf(script, "SetWorldCityFightCaptureBuild(%d, [===[%s]===],%d)", info[i].DefID, info[i].ClanName,info[i].Type == CITY_BUILD_TYPE_CONTROL);
			ActorManager::getSingleton().getScriptVM()->callString(script);
			sprintf(script, "WorldCityEnterView(%d)", info[i].MemID);
			ActorManager::getSingleton().getScriptVM()->callString(script);
		}
	}
	GameMsgPrint::getSingleton().SendUIEvent("GE_CITYBUILD_UPDATE");
	GameMsgPrint::getSingleton().SendUIEvent("GE_CITYBUILD_WARNING");
}

void WorldCityManager::setCaptureInfo(int time, const char *clan)
{
	char script[100 + NAME_LEN];
	sprintf(script, "SetWorldCityFightCaptureTime(%d, [===[%s]===])", time, clan);
	ActorManager::getSingleton().getScriptVM()->callString(script);
}

void WorldCityManager::reset()
{
	FightStat = WORLD_CITY_STAT_NORMAL;
	BuildInfo.clear();
	BuildInfoIdx.clear();
}

int WorldCityManager::getFirstCityBuildId()
{
	std::map<int, size_t>::iterator i = BuildInfoIdx.begin();
	if (i == BuildInfoIdx.end())
	{
		return 0;
	}
	return i->first;
}

int WorldCityManager::getNextCityBuildId(int id)
{
	std::map<int, size_t>::iterator i = BuildInfoIdx.find(id);
	if (i != BuildInfoIdx.end())
	{
		++i;
	}
	if (i == BuildInfoIdx.end())
	{
		return 0;
	}
	return i->first;
}

int WorldCityManager::getCityBuildPosX(int id)
{
	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(id);
	return WorldCityBuild->Pos.X;
}

int WorldCityManager::getCityBuildPosY(int id)
{
	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(id);
	return WorldCityBuild->Pos.Y;
}

const CityBuildInfo * WorldCityManager::getCityBuildInfo(int id)
{
	std::map<int, size_t>::iterator i = BuildInfoIdx.find(id);
	if (i == BuildInfoIdx.end())
		return NULL;

	return &(BuildInfo[i->second]);
}

int WorldCityManager::getCityBuildResID(const CityBuildInfo *info)
{
	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(info->DefID);
	if (info->State == CITY_BUILD_STAT_DIE)
	{
		return WorldCityBuild->DieBuildID;
	}
	return WorldCityBuild->BuildList[info->BuildIdx].BuildID[info->BuildLevel - 1];
}

bool WorldCityManager::CityBuildCanDisplay(const CityBuildInfo *info)
{
	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(info->DefID);
	return DBDataManager::getSingleton().getCityProductDef(WorldCityBuild->DieBuildID) != NULL;
}

int WorldCityManager::CityBuildUpgradeId(const CityBuildInfo *info)
{
	if (info->State == CITY_BUILD_STAT_DIE)
	{
		return 0;
	}
	if (info->BuildLevel == MAX_CITY_BUILD_LEVEL)
	{
		return 0;
	}
	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(info->DefID);
	return WorldCityBuild->BuildList[info->BuildIdx].BuildID[info->BuildLevel];
}

bool WorldCityManager::CityBuildCanRepair(const CityBuildInfo *info)
{
	return info->State != CITY_BUILD_STAT_DIE;
}

bool WorldCityManager::CityBuildCanSupply(const CityBuildInfo *info)
{
	return info->State != CITY_BUILD_STAT_DIE && info->MaxEnergy != 0;
}

bool WorldCityManager::CityBuildCanRemove(const CityBuildInfo *info)
{
	if ( info->Type == CITY_BUILD_TYPE_DOOR_BUILD || info->Type == CITY_BUILD_TYPE_DOOR2
		|| info->Type == CITY_BUILD_TYPE_DOOR3 || info->Type == CITY_BUILD_TYPE_BUFF )
	{
		return false;
	}
	return info->State != CITY_BUILD_STAT_DIE;
}

int WorldCityManager::getCityBuildFirstConstruct(const CityBuildInfo *info, bool state)
{
	if (state && info->State != CITY_BUILD_STAT_DIE)
	{
		return 0;
	}

	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(info->DefID);
	for (size_t i = 0; i < MAX_CITY_BUILD_LIST; ++i)
	{
		int id = WorldCityBuild->BuildList[i].BuildID[0];
		if (DBDataManager::getSingleton().getCityProductDef(id) != NULL)
		{
			return id;
		}
	}
	return 0;
}

int WorldCityManager::getCityBuildNextConstruct(const CityBuildInfo *info, int id, bool state)
{
	if (state && info->State != CITY_BUILD_STAT_DIE)
	{
		return 0;
	}

	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(info->DefID);
	for (size_t i = 0; i < MAX_CITY_BUILD_LIST - 1; ++i)
	{
		if (WorldCityBuild->BuildList[i].BuildID[0] == id)
		{
			if (DBDataManager::getSingleton().getCityProductDef(WorldCityBuild->BuildList[i + 1].BuildID[0]) != NULL)
			{
				return WorldCityBuild->BuildList[i + 1].BuildID[0];
			}
		}
	}
	return 0;
}

int WorldCityManager::getCityProductMapIconId(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	return CityProductInfo->MapIcon;
}

int WorldCityManager::getCityProductBuildIconId(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	return CityProductInfo->BuildIcon;
}

const char * WorldCityManager::getCityProductBuildTips(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return "";
	}
	return CityProductInfo->BuildTips;
}

const char * WorldCityManager::getCityProductBuildDesc(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return "";
	}
	return CityProductInfo->BuildDesc;
}

int WorldCityManager::getCityBuildMapIconId(int id)
{
	const CityBuildInfo *info = getCityBuildInfo(id);
	if (info == NULL)
	{
		return 0;
	}
	return getCityProductMapIconId(getCityBuildResID(info));
}

void WorldCityManager::UpgradeCityBuild(int id)
{
	tagCSPkgBody msgbody;
	msgbody.CityClt.Type = CITY_CLT_BUILD_LEVELUP;
	msgbody.CityClt.Data.LevelUP.DefID = id;
	SendGameServerMsg(CITY_CLT, msgbody);
}

void WorldCityManager::ConstructCityBuild(int id, int idx)
{
	tagCSPkgBody msgbody;
	msgbody.CityClt.Type = CITY_CLT_BUILD_CREATE;
	msgbody.CityClt.Data.Create.DefID = id;
	const tagWorldCityBuildDef *WorldCityBuild = DBDataManager::getSingleton().getWorldCityBuildDef(id);
	for (int i = 0; i < MAX_CITY_BUILD_LIST; ++i)
	{
		if (WorldCityBuild->BuildList[i].BuildID[0] == idx)
		{
			msgbody.CityClt.Data.Create.BuildListIdx = i;
			break;
		}
	}
	SendGameServerMsg(CITY_CLT, msgbody);
}

void WorldCityManager::RepairCityBuild(int id)
{
	tagCSPkgBody msgbody;
	msgbody.CityClt.Type = CITY_CLT_BUILD_REPAIR;
	msgbody.CityClt.Data.Repair.DefID = id;
	SendGameServerMsg(CITY_CLT, msgbody);
}

void WorldCityManager::SupplyCityBuild(int id)
{
	tagCSPkgBody msgbody;
	msgbody.CityClt.Type = CITY_CLT_BUILD_ADD_ENERGY;
	msgbody.CityClt.Data.Energy.DefID = id;
	SendGameServerMsg(CITY_CLT, msgbody);
}

void WorldCityManager::RemoveCityBuild(int id)
{
	tagCSPkgBody msgbody;
	msgbody.CityClt.Type = CITY_CLT_BUILD_DEL;
	msgbody.CityClt.Data.Del.DefID = id;
	SendGameServerMsg(CITY_CLT, msgbody);
}

void WorldCityManager::UpdateEffigy()
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return;
	}
	GameMap *map = player->getCurMap();
	if (map == NULL)
	{
		return;
	}
	if (map->getRealId() == WorldCityCoreInfo.Effigy.MapID)
	{
		int PoseState = POSE_NONE;
		if (WorldCityCoreInfo.Effigy.RoleName[0] != '\0')
		{
			AvatarEntity *pavatar = new AvatarEntity;
			int resid = getPlayerDisplayId(WorldCityCoreInfo.Effigy.RoleGender, WorldCityCoreInfo.Effigy.RoleCareer);
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
			pavatar->equipItem(POS_ID_HEAD, WorldCityCoreInfo.Effigy.RoleHead, 0);
			pavatar->equipItem(POS_ID_FACE, WorldCityCoreInfo.Effigy.RoleFace, 0);
			pavatar->equipItem(POS_ID_HAIR, WorldCityCoreInfo.Effigy.RoleHair, 0);
			int left = 0, right = 0;
			for (size_t i = 0; i < WorldCityCoreInfo.Effigy.RoleWear.ArmNum; ++i)
			{
				const tagRoleArm &Arm = WorldCityCoreInfo.Effigy.RoleWear.RoleArms[i];
				pavatar->equipItem(EquipPos_2_PosId[Arm.GridIdx], Arm.DefID, Arm.StarLv);
				if (Arm.GridIdx == EQUIP_POS_LEFT)
				{
					left = Arm.DefID / 10000;
				}
				else if (Arm.GridIdx == EQUIP_POS_RIGHTHAND)
				{
					right = Arm.DefID / 10000;
				}
				else if ( EquipPos_2_PosId[Arm.GridIdx] == POS_ID_FASHION_HEAD )
				{
					int nHair = DBDataManager::getSingleton().getItemDisplayInfo(Arm.DefID).hairstyle;
					nHair = nHair > 1 ? 9 : WorldCityCoreInfo.Effigy.RoleHair;
					pavatar->equipItem(POS_ID_HAIR, nHair, 0);
				}
			}
			pavatar->setAvatarAttrib(0, WorldCityCoreInfo.Effigy.RoleCareer);
			pavatar->setAvatarAttrib(1, info.gender);
			if (right == ARM_SWORD || right == ARM_KNIFE)
			{
				if (left == ARM_SWORD_LEFT)
				{
					PoseState = POSE_WEAPON_SHORTWEAPON;
				}
				else
				{
					PoseState = POSE_SHORTWEAPON_R;
				}
			}
			else if (right == ARM_BOTHSWORD)
			{
				PoseState = POSE_WEAPON_SHORTWEAPON;
			}
			else if (right == ARM_HEVY_SCATTERGUN)
			{
				PoseState = POSE_SCATTERGUN;
			}
			else if (right == ARM_BITH_GUN)
			{
				PoseState = POSE_SCATTERGUN;
			}
			else if (right == ARM_RIGHT_GUN)
			{
				if (left == ARM_GUNNER_WEAR_LEFT)
				{
					PoseState = POSE_DOUBLEGUN;
				}
				else
				{
					PoseState = POSE_GUN_R;
				}
			}
			else if (right / 10 == ARM_WIZARD)
			{
				PoseState = POSE_WAND;
			}
			else if (right / 10 == ARM_CHURCH)
			{
				PoseState = POSE_CHURCH_WAND;
			}
			pavatar->updateAttrib();

			if( WorldCityCoreInfo.Effigy.CurrStat == EFFIGY_STAT_NONE )
			{
				pavatar->stopMotion( 1277001 );
			}else if( WorldCityCoreInfo.Effigy.CurrStat == EFFIGY_STAT_FLOWER )
			{
				pavatar->playMotion( "1277001",false,1277001 );
			}else if( WorldCityCoreInfo.Effigy.CurrStat == EFFIGY_STAT_EGG )
			{
				pavatar->playMotion( "1276001",false,1277001 );
			}

			Effigy = pavatar;
		}
		else
		{
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(305100);
			Effigy = new Entity;
			Effigy->load(info.model, true);
		}
		Effigy->SetForceShadowMapCull(true);
		Effigy->attachToScene(map->getShowGameMap()->getGameScene());
		int x, y, z;
		x = WorldCityCoreInfo.Effigy.Pos.X * 10;
		z = WorldCityCoreInfo.Effigy.Pos.Y * 10;
		map->getTerrainH(x, z, y);
		Effigy->setPosition(GameXYZ(x, y + 2000, z));
		Effigy->setScale(2.2f);
		Effigy->setRotation(90 - 90.0f, 0, 0);
		Effigy->playAnim(PoseState * 1000 + ACTSTA_NORMAL * 100 + ACTSTA_NORMAL_STAND);
		//Effigy->setBoreder(m_pBackMaterial0);
		//Effigy->playMotion("1003", false, 0);
		Effigy->update(60000);
		Effigy->resetUpdate(true);
	}
}

bool WorldCityManager::IsWorldCityKeeper(const tdr_ulonglong &GID)
{
	return WorldCityCoreInfo.ClanGID != 0 && memcmp(&WorldCityCoreInfo.ClanGID, &GID, sizeof(tdr_ulonglong)) == 0;
}
int WorldCityManager::GetCityKeeperClanID()
{
	return WorldCityCoreInfo.ClanId;
}
tdr_ulonglong WorldCityManager::GetCityKeepClanWID()
{
	return WorldCityCoreInfo.ClanGID;
}
int	WorldCityManager::GerCityFightEnableStat()
{
	return WorldCityCoreInfo.FightEnableStat;
}
int	WorldCityManager::GetCityOwnerKeepNum()
{
	return WorldCityCoreInfo.KeepNum;
}
int WorldCityManager::GetCityOwnerAwardMoney()
{
	int nClanPayIndex = WorldCityCoreInfo.KeepNum + 1;
	//因为只填了第4次的占领奖励，所以如果超过就取第4次的奖励
	if ( nClanPayIndex > 4 )
	{
		nClanPayIndex = 4;
	}
	const tagClanPayDef* clanPayDef = DBDataManager::getSingleton().getClanPayDef(nClanPayIndex);
	if( clanPayDef != NULL )
	{
		return clanPayDef->Money;
	}
	return 0;
}
int	WorldCityManager::GetCityStat()
{
	return WorldCityCoreInfo.Stat;
}

bool WorldCityManager::IsWorldCityKeeper(unsigned int id)
{
	return id != 0 && WorldCityCoreInfo.ClanId == id;
}

bool WorldCityManager::IsWorldCityKeeper(GameActor *pActor)
{
	return IsWorldCityKeeper(pActor->getClanWID());
}

void WorldCityManager::setFightStat(int stat)
{
	FightStat = stat;
	UpdateMusic();
	for (std::vector<CityBuildInfo>::iterator i = BuildInfo.begin();
		i != BuildInfo.end(); ++i)
	{
		if (i->Type == CITY_BUILD_TYPE_DOOR_BUILD || i->Type == CITY_BUILD_TYPE_DOOR2 || i->Type == CITY_BUILD_TYPE_DOOR3 )
		{
			GameActor *pActor = ActorManager::getSingleton().FindActor(i->MemID);
			if (pActor)
			{
				if (IsFighting())
				{
					pActor->playAction(10601);
				}
				else
				{
					pActor->playAction(10600);
				}
			}
		}
	}
}

void WorldCityManager::UpdateWorldCityClanName()
{
	for (ACTOR_ITER i = ActorManager::getSingleton().getActorBegin(), j = ActorManager::getSingleton().getActorEnd();
		i != j; ++i)
	{
		i->second->BuildTitle();
	}
}

void WorldCityManager::update(unsigned int dtick)
{
	if (Effigy)
	{
		Effigy->update(dtick);
	}
}

void WorldCityManager::ClearEffigy()
{
	if (Effigy)
	{
		Effigy->detachFromScene();
		Effigy->release();
		Effigy = NULL;
	}
}

void WorldCityManager::draw(FollowCamera *pcamera)
{
	if (Effigy == NULL)
	{
		return;
	}
	Ogre::Camera *camera = pcamera->GetCameraNode();
	const Ogre::Vector3 &pos = Effigy->getAnchorWorldPos(106);
	if (!pcamera->GetCameraNode()->canSeePointInWorld(pos))
	{
		return;
	}
	Ogre::Vector3 scrpos;
	camera->pointWorldToWindow(scrpos, pos);
	Ogre::UIRenderer *prenderer = Ogre::UIRenderer::getSingletonPtr();
	float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ? g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
	float width, height;
	prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], "城主雕像", width, height);
	prenderer->renderText(s_hFontTexture[NAME_FONT], FONTSYTLE_BORDER, "城主雕像", scrpos.x - (width * 0.5f) * fScale,
		scrpos.y + (height - 20) * fScale, Ogre::ColorQuad(0xffd986ff), fScale, false, /*scrpos.z*/0);
	const char *name = WorldCityCoreInfo.Effigy.RoleName;
	if (name[0] == '\0')
	{
		name = "萝拉";
	}
	prenderer->GetTextExtent(s_hFontTexture[NAME_FONT], name, width, height);
	prenderer->renderText(s_hFontTexture[NAME_FONT], FONTSYTLE_BORDER, name, scrpos.x - (width * 0.5f) * fScale,
		scrpos.y + (height * 2 - 20) * fScale, Ogre::ColorQuad(0xfff6ff00), fScale, false, /*scrpos.z*/0);
}

bool WorldCityManager::IsFighting()
{
	return FightStat == WORLD_CITY_STAT_FIGHT;
}

bool WorldCityManager::IsCityBuildFight(GameActor *pActor)
{
	return IsCityBuildFight(pActor->GetID());
}

bool WorldCityManager::IsCityBuildFight(int id)
{
	std::map<int, int>::iterator i = CityBuildFight.find(id);
	return i != CityBuildFight.end();
}

void WorldCityManager::MarkCityBuildFight(GameActor *pActor, int flag)
{
	MarkCityBuildFight(pActor->GetID(), flag);
}

void WorldCityManager::MarkCityBuildFight(int id, int flag)
{
	CityBuildFight[id] = flag;
}

void WorldCityManager::UnMarkCityBuildFight(GameActor *pActor)
{
	UnMarkCityBuildFight(pActor->GetID());
}

void WorldCityManager::UnMarkCityBuildFight(int id)
{
	std::map<int, int>::iterator i = CityBuildFight.find(id);
	if (i != CityBuildFight.end())
	{
		CityBuildFight.erase(i);
	}
}

void WorldCityManager::ClearCityBuildFight()
{
	CityBuildFight.clear();
}

void WorldCityManager::objEnterView(GameActor *actor)
{
	int id = actor->GetID();
	if (IsCityBuildFight(id))
	{
		char script[100];
		sprintf(script, "WorldCityEnterView(%d)", id);
		ActorManager::getSingleton().getScriptVM()->callString(script);
	}
}

void WorldCityManager::objLeaveView(GameActor *actor)
{
	int id = actor->GetID();
	char script[100];
	sprintf(script, "WorldCityLeaveView(%d)", id);
	ActorManager::getSingleton().getScriptVM()->callString(script);
}

bool WorldCityManager::objCanAttack(GameActor *actor)
{
	int id = actor->GetID();
	for (std::vector<CityBuildInfo>::iterator i = BuildInfo.begin(); i != BuildInfo.end(); i++)
	{
		if ( i->MemID == id && i->Type == CITY_BUILD_TYPE_CONTROL )
		{
			if ( HasBuffAlive() )
			{
				return false;
			}
		}
	}
	return true;
}
bool WorldCityManager::HasBuffAlive()
{
	for (std::vector<CityBuildInfo>::iterator i = BuildInfo.begin(); i != BuildInfo.end(); i++)
	{
		if ( i->Type == CITY_BUILD_TYPE_BUFF && i->State == CITY_BUILD_STAT_ALIVE )
		{
			return true;
		}
	}
	return false;
}

void WorldCityManager::UpdateMusic()
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return;
	}
	GameMap *map = player->getCurMap();
	if (map == NULL)
	{
		return;
	}
	if (map->getRealId() == WorldCityMapID)
	{
		if (IsFighting())
		{
			map->getShowGameMap()->setOverlayMusic("sounds\\sounds_1\\b4404-2.wma", 0.6);
		}
		else
		{
			map->getShowGameMap()->setOverlayMusic(NULL, 0);
		}
	}
}

void WorldCityManager::OnEnterMap(GameMap *map)
{
	if (map->getRealId() == WorldCityMapID)
	{
		
	}
}

int WorldCityManager::GetCityBuildDefID(GameActor *pActor)
{
	return GetCityBuildDefID(pActor->GetID());
}

int WorldCityManager::GetCityBuildDefID(int id)
{
	std::map<int, int>::iterator i = CityBuildFight.find(id);
	if (i != CityBuildFight.end())
	{
		return i->second;
	}
	return 0;
}

int WorldCityManager::GetCityBuildRepairCost(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	return CityProductInfo->BuildCost.RepairClanMoney;
}

int WorldCityManager::GetCityBuildSupplyCost(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	return CityProductInfo->BuildCost.EnergyClanMoney;
}

int WorldCityManager::GetCityBuildConstructCost(int id)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	return CityProductInfo->BuildCost.MakeClanMoney;
}

int WorldCityManager::GetCityBuildConstructNextResource(int id, int resourceId/* = 0*/)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	if (resourceId == 0)
	{
		return CityProductInfo->BuildCost.Material[0].ID;
	}
	for (size_t i = 0; i < MAX_CITY_BUILD_MATERIAL - 1; ++i)
	{
		if (CityProductInfo->BuildCost.Material[i].ID == resourceId)
		{
			return CityProductInfo->BuildCost.Material[i + 1].ID;
		}
	}
	return 0;
}

int WorldCityManager::GetCityBuildConstructResourceCost(int id, int resourceId)
{
	const tagCityProductDef *CityProductInfo = DBDataManager::getSingleton().getCityProductDef(id);
	if (CityProductInfo == NULL)
	{
		return 0;
	}
	for (size_t i = 0; i < MAX_CITY_BUILD_MATERIAL; ++i)
	{
		if (CityProductInfo->BuildCost.Material[i].ID == resourceId)
		{
			return CityProductInfo->BuildCost.Material[i].Num;
		}
	}
	return 0;
}

void WorldCityManager::RequestClanMemberPos()
{
	tagCSPkgBody msgbody;
	msgbody.CityClt.Type = CITY_CLT_MAP_MEM_POS;
	msgbody.CityClt.Data.MapMemPos = 0;
	SendGameServerMsg(CITY_CLT, msgbody);
}

int WorldCityManager::GetCityFightEnableStat()
{
	return WorldCityCoreInfo.FightEnableStat;
}

int WorldCityManager::GetCityBuildType( int nMemId )
{
	for (std::vector<CityBuildInfo>::iterator i = BuildInfo.begin();
		i != BuildInfo.end(); ++i)
	{
		if( i->MemID == nMemId )
		{
			return i->Type;
		}
	}
	return 0;
}
int WorldCityManager::GetWorldCityClanNum()
{
	return m_CityClanFightInfo.Num;
}
const char* WorldCityManager::GetWorldCityClanName( int index )
{
	return m_CityClanFightInfo.ClanList[index].ClanId.Name;
}
int WorldCityManager::GetWorldCityClanGetReviveNum( int index )
{
	return m_CityClanFightInfo.ClanList[index].ReviveBuild;
}
int WorldCityManager::GetWorldCityClanGetControlNum( int index )
{
	return m_CityClanFightInfo.ClanList[index].ControlBuild;
}
int WorldCityManager::GetWorldCityClanBuffNum( int index )
{
	return m_CityClanFightInfo.ClanList[index].BuffBuild;
}
int WorldCityManager::GetWorldCityClanDoorNum( int index )
{
	return m_CityClanFightInfo.ClanList[index].DoorBuild;
}
int WorldCityManager::GetWorldCityClanDefNum( int index )
{
	return m_CityClanFightInfo.ClanList[index].DefBuild;
}
int WorldCityManager::GetWorldCityClanTotal( int index )
{
	return m_CityClanFightInfo.ClanList[index].Total;
}
int WorldCityManager::GetWorldCityClanTime( int index )
{
	return m_CityClanFightInfo.ClanList[index].Time;
}
int WorldCityManager::GetWorldCitySelClanTotal()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	for ( int i = 0; i < m_CityClanFightInfo.Num; i ++ )
	{
		if ( strcmp( m_CityClanFightInfo.ClanList[i].ClanId.Name, pMain->getClanName() ) == 0 )
		{
			return m_CityClanFightInfo.ClanList[i].Total;
		}
	}
	return 0;
}
int WorldCityManager::GetWorldCityClanAwardMoneyType( int index )
{
	const tagWorldCityFightAwardDef* worldCityFightAwardDef = DBDataManager::getSingleton().getWorldCityFightAwardDef( index );
	return worldCityFightAwardDef->MoneyType;
}
int WorldCityManager::GetWorldCityClanAwardMoney( int index )
{
	const tagWorldCityFightAwardDef* worldCityFightAwardDef = DBDataManager::getSingleton().getWorldCityFightAwardDef( index );
	return worldCityFightAwardDef->Money;
}
int WorldCityManager::GetWorldCityClanAwardItemID( int index )
{
	const tagWorldCityFightAwardDef* worldCityFightAwardDef = DBDataManager::getSingleton().getWorldCityFightAwardDef( index );
	return worldCityFightAwardDef->AwardItemID;
}
int WorldCityManager::GetWorldCityClanAwardExp( int index )
{
	const tagWorldCityFightAwardDef* worldCityFightAwardDef = DBDataManager::getSingleton().getWorldCityFightAwardDef( index );
	return worldCityFightAwardDef->Exp;
}
int WorldCityManager::GetWorldCityClanEnterTime()
{
	return m_CityClanFightInfo.EnterTime;
}
int WorldCityManager::GetWorldCitySelfClanRank()
{
	return m_CityClanFightInfo.SelfClanTop;
}
int WorldCityManager::GetWorldCitySelfClanMoneyType()
{
	return m_CityClanFightInfo.MoneyType;
}
int WorldCityManager::GetWorldCitySelfClanMoney()
{
	return m_CityClanFightInfo.Money;
}
int WorldCityManager::GetWorldCitySelfContineKill()
{
	return m_KillInfo.ContinueKill;
}
void WorldCityManager::ClearWorldCitySelfContineKill()
{
	m_KillInfo.ContinueKill = 0;
}
const char* WorldCityManager::getOwnerName()
{
	return WorldCityCoreInfo.Effigy.RoleName;
}