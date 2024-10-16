#include "PworldLimitInfo.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDBData.h"
#include "GameActorManager.h"
#include "OgreScriptLuaVM.h"
#include <Windows.h>

PworldLimitInfo::PworldLimitInfo() : pworldId(0)
{
	INetworkManager::getSingleton().RegisterMsgHandler( PWORLD_LIMIT_INFO, this );
}

PworldLimitInfo::~PworldLimitInfo()
{

}

PworldLimitInfo & PworldLimitInfo::getSingleton()
{
	static PworldLimitInfo singleton;
	return singleton;
}

int PworldLimitInfo::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (msgid == PWORLD_LIMIT_INFO)
	{
		const CSPWORLDLIMITINFODATA &info =  msgbody.PworldLimitInfo.PworldLimitInfoData;
		switch (msgbody.PworldLimitInfo.Type)
		{
		case PWORLD_LIMIT_TYPE_ALL:
			{
				m_pworldData.clear();
				ActorManager::getSingleton().getScriptVM()->callString("promotionClearLimit()");
				for (int i = 0; i < info.All.LimitPworldNum; ++i)
				{
					PworldLimitData &data = getPworldData(info.All.LimitPworld[i].PworldID);
					data.freeTimeUse = info.All.LimitPworld[i].Num;
					data.itemTimeUse = info.All.LimitPworld[i].ItemNum;
					char buf[256] = {'\0'};
					sprintf(buf,"promotionSetLimit(%d,%d)",info.All.LimitPworld[i].PworldID,info.All.LimitPworld[i].Num);
					ActorManager::getSingleton().getScriptVM()->callString(buf);
				}
			}
			break;

		case PWORLD_LIMIT_TYPE_CHG:
			{
				PworldLimitData &data = getPworldData(info.Chg.PworldID);
				data.freeTimeUse = info.Chg.Num;
				data.itemTimeUse = info.Chg.ItemNum;
				char buf[256] = {'\0'};
				sprintf(buf,"promotionSetLimit(%d,%d)",info.Chg.PworldID,info.Chg.Num);
				ActorManager::getSingleton().getScriptVM()->callString(buf);
			}
			break;
		}
	}

	return 0;
}

PworldLimitData & PworldLimitInfo::getPworldData(int id)
{
	std::map<int, PworldLimitData>::iterator i = m_pworldData.find(id);
	if (i == m_pworldData.end())
	{
		PworldLimitData &data = m_pworldData[id];
		const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(id);
		if (pworldDef)
		{
			data.itemId = pworldDef->ItemEnterID;
			data.itemTimeTotal = pworldDef->ItemEnterNum;
			data.freeTimeTotal = pworldDef->NumEveryDay;
		}

		i = m_pworldData.find(id);
	}

	return i->second;
}

bool PworldLimitInfo::canEnterPworld( int nId )
{
	PworldLimitData& onePworldLimitData = getPworldData( nId );
	if ( onePworldLimitData.freeTimeTotal == 0 )
	{
		return true;
	}

	return onePworldLimitData.freeTimeUse <= onePworldLimitData.freeTimeTotal;
}

void PworldLimitInfo::onEnterMap(int id)
{
	if (id >= 10000)
	{
		pworldId = id / 100;
	}
	else
	{
		pworldId = 0;
	}
}

int PworldLimitInfo::getCurPworldId()
{
	return pworldId;
}
