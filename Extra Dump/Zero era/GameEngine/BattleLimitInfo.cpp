#include "BattleLimitInfo.h"
#include "GameDBData.h"
#include "NetMessage.h"
#include "GameMsgPrint.h"

BattleLimitInfo::BattleLimitInfo()
{
	DBDataManager::getSingleton().loadBattleDef(m_arrBattle);
	INetworkManager::getSingleton().RegisterMsgHandler(BATTLE_LIMIT_INFO, this);
}

BattleLimitInfo::~BattleLimitInfo()
{

}

BattleLimitInfo & BattleLimitInfo::getSingleton()
{
	static BattleLimitInfo singleton;
	return singleton;
}

int BattleLimitInfo::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (BATTLE_LIMIT_INFO == msgid)
	{
		const CSBATTLELIMITDATA &info =  msgbody.BattleLimit.BattleLimitData;
		switch (msgbody.BattleLimit.Type)
		{
		case BATTLE_LIMIT_TYPE_ALL:
			{
				m_battleData.clear();
				for (int i = 0; i < info.All.LimitBattleNum; ++i)
				{
					BattleLimitData &data = getBattleData(info.All.LimitBattle[i].BattleID);
					data.Use = info.All.LimitBattle[i].Num;
				}
			}
			break;
		case BATTLE_LIMIT_TYPE_CHG:
			{
				BattleLimitData &data = getBattleData(info.Chg.BattleID);
				data.Use = info.Chg.Num;
			}
			break;
		}
		GameMsgPrint::getSingleton().SendUIEvent("BATTLE_LIMIT_INFO_CHG");
	}
	return 0;
}

BattleLimitData & BattleLimitInfo::getBattleData(int id)
{
	std::map<int, BattleLimitData>::iterator i = m_battleData.find(id);
	if (i == m_battleData.end())
	{
		BattleLimitData &data = m_battleData[id];
		for (size_t n = 0; n < getBattleNum(); ++n)
		{
			const tagBattleDef *battleDef = getBattleDef(n);
			if (battleDef->BattleID == id)
			{
				data.index = n;
				data.Total = battleDef->NumEveryDay;
				break;
			}
		}

		i = m_battleData.find(id);
	}

	return i->second;
}

const tagBattleDef * BattleLimitInfo::getBattleDef(size_t i)
{
	return m_arrBattle[i];
}

size_t BattleLimitInfo::getBattleNum()
{
	return m_arrBattle.size();
}

const tagBattleDef * BattleLimitInfo::getBattleDefById(int id)
{
	return m_arrBattle[getBattleData(id).index];
}
