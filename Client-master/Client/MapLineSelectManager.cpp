#include "MapLineSelectManager.h"
#include "GameManager.h"
#include "GameMaxMap.h"
#include "MapOre.h"

MapLineSelectManager::MapLineSelectManager(void) : curMapId(0)
{
	INetworkManager::getSingleton().RegisterMsgHandler(MAP_SVR, this);
}

MapLineSelectManager::~MapLineSelectManager(void)
{
}

MapLineSelectManager & MapLineSelectManager::getSingleton()
{
	static MapLineSelectManager singleton;

	return singleton;
}

int MapLineSelectManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (msgid == MAP_SVR)
	{
		const CSMAPSVRDATA &data = msgbody.MapSvr.Data;
		switch (msgbody.MapSvr.SvrOp)
		{
		case MAP_SVR_LINE_INFO:
			{
				mapLines.resize(data.MapLineInfo.LineNum);
				memcpy(&mapLines[0], data.MapLineInfo.MapInfo, sizeof(CSMAPLINEDATA) * data.MapLineInfo.LineNum);
				GameManager::getSingleton().SendUIEvent("GE_MAP_LINE_INFO");
			}
			break;

		case MAP_SVR_ORE_INFO:
			{
				MapOreInfo::getSingleton().SetOre(data.OreList.OreNum, data.OreList.OreInfo);
			}
			break;

		case MAP_SVR_BIG_ORE_INFO:
			{
				MapOreInfo::getSingleton().SetBigOre(data.BigOreList.OreNum, data.BigOreList.OreInfo);
			}
			break;
		}
	}
	return 0;
}

int MapLineSelectManager::getMapLineNum()
{
	return int(mapLines.size());
}

int MapLineSelectManager::getMapLineId(int i)
{
	if (i >= getMapLineNum())
	{
		return 0;
	}
	return mapLines[i].MapID;
}

int MapLineSelectManager::getMapLineState(int i)
{
	if (i < getMapLineNum())
	{
		switch (mapLines[i].Busy)
		{
		case MAP_BUSY_TYPE_NORMAL:
			return 1;

		case MAP_BUSY_TYPE_BUSY:
			return 2;

		case MAP_BUSY_TYPE_FULL:
			return 3;
		}
	}

	return 0;
}

void MapLineSelectManager::requestChangeMapLine(int id)
{
	tagCSPkgBody msgbody;
	msgbody.MapClt.CltOp = MAP_CLT_CHANGE_LINE;
	msgbody.MapClt.Data.ChgLine.MapID = id;
	SendGameServerMsg(MAP_CLT, msgbody);
}

void MapLineSelectManager::requestMapLineInfo()
{
	int mapId = getCurMapId();
	const tagMapDef &mapDef = DBDataManager::getSingleton().getMapDef(mapId);
	if (mapDef.MapLine > 0)
	{
		tagCSPkgBody msgbody;
		msgbody.MapClt.CltOp = MAP_CLT_UPDATE_LINE_INFO;
		msgbody.MapClt.Data.UpdateLineInfo.MapID = mapId;
		SendGameServerMsg(MAP_CLT, msgbody);
	}
	else
	{
		mapLines.resize(0);
		GameManager::getSingleton().SendUIEvent("GE_MAP_LINE_INFO");
	}
}

int MapLineSelectManager::getCurMapId()
{
	return GameManager::getSingleton().getGameMapShower()->getRealMapId();
}
