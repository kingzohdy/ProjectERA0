#include "MapOre.h"
#include <assert.h>
#include "GameManager.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameCollect.h"

MapOreInfo::MapOreInfo(void)
{

}

MapOreInfo::~MapOreInfo(void)
{

}

MapOreInfo & MapOreInfo::getSingleton()
{
	static MapOreInfo singleton;
	return singleton;
}

void MapOreInfo::SetOre(int num, const tagCSMapOreInfo infos[])
{
	Ores.resize(num);
	if (num > 0)
	{
		memcpy(&(Ores[0]), infos, sizeof(infos[0]) * num);
	}
	GameManager::getSingleton().SendUIEvent("GE_MAP_ORE_UPDATE");
}

void MapOreInfo::SetBigOre(int num, const tagCSMapOreInfo infos[])
{
	BigOres.resize(num);
	if (num > 0)
	{
		memcpy(&(BigOres[0]), infos, sizeof(infos[0]) * num);
	}
	GameManager::getSingleton().SendUIEvent("GE_MAP_BIG_ORE_UPDATE");
}

int MapOreInfo::GetOreCount()
{
	return Ores.size();
}

int MapOreInfo::GetOreId(int idx)
{
	assert(idx >= 0 && idx < Ores.size());
	return Ores[idx].ID;
}

int MapOreInfo::GetOreMemId(int idx)
{
	assert(idx >= 0 && idx < Ores.size());
	return Ores[idx].MemID;
}

int MapOreInfo::GetOreMap(int idx)
{
	assert(idx >= 0 && idx < Ores.size());
	return Ores[idx].Map;
}

int MapOreInfo::GetOreX(int idx)
{
	assert(idx >= 0 && idx < Ores.size());
	return Ores[idx].Pos.X;
}

int MapOreInfo::GetOreY(int idx)
{
	assert(idx >= 0 && idx < Ores.size());
	return Ores[idx].Pos.Y;
}

int MapOreInfo::GetBigOreCount()
{
	return BigOres.size();
}

int MapOreInfo::GetBigOreId(int idx)
{
	assert(idx >= 0 && idx < BigOres.size());
	return BigOres[idx].ID;
}

int MapOreInfo::GetBigOreMemId(int idx)
{
	assert(idx >= 0 && idx < BigOres.size());
	return BigOres[idx].MemID;
}

int MapOreInfo::GetBigOreMap(int idx)
{
	assert(idx >= 0 && idx < BigOres.size());
	return BigOres[idx].Map;
}

int MapOreInfo::GetBigOreX(int idx)
{
	assert(idx >= 0 && idx < BigOres.size());
	return BigOres[idx].Pos.X;
}

int MapOreInfo::GetBigOreY(int idx)
{
	assert(idx >= 0 && idx < BigOres.size());
	return BigOres[idx].Pos.Y;
}

int MapOreInfo::GetOreDetectRadius()
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return 0;
	}
	if (!player->isOnMonsterMachine())
	{
		return 0;
	}
	MonsterMachine* pMonsterMachine = player->getMonsterMachine();
	tagMonMachineDef_ToLua* monMachineDef = pMonsterMachine->getMonMachineDef();
	if (monMachineDef == NULL)
	{
		return 0;
	}
	return monMachineDef->OreViewDist;
}

bool MapOreInfo::CanCollectOre(int id)
{
	return GameCollect::getSingleton().limitMachineCollect(id);
}
