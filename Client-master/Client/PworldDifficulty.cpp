#include "PworldDifficulty.h"
#include "NetMessage.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "PworldLimitInfo.h"
#include "GameDBData.h"
#include "PworldScore.h"

PworldDifficulty::PworldDifficulty()
{
	tipNum = 0;
	INetworkManager::getSingleton().RegisterMsgHandler(DIFF_PWORLD_SVR, this);
}

PworldDifficulty::~PworldDifficulty()
{

}

int PworldDifficulty::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	int result = 0;
	if (msgid == DIFF_PWORLD_SVR)
	{
		const tagCSDiffPworldSvrData &data = msgbody.DiffPworldSvr.Data;
		switch (msgbody.DiffPworldSvr.Type)
		{
		case DIFF_PWORLD_SVR_UI:
			{
				char script[256];
				if (data.UIInfo.DynLevel > 0)
				{
					sprintf(script,"OpenPworldDifficultyDynFrame(%d,%d,%d)", data.UIInfo.NpcID, data.UIInfo.DiffPworldID, data.UIInfo.DynLevel);
				}
				else
				{
					if (data.UIInfo.TeamCreateID == 0)
					{
						sprintf(script,"OpenPworldDifficultyFrame(%d,%d)", data.UIInfo.NpcID, data.UIInfo.DiffPworldID);
					}
					else
					{
						sprintf(script,"OpenPworldDifficultyFrame(%d,%d,%d,%d)", data.UIInfo.NpcID, data.UIInfo.DiffPworldID, getDifficulty(data.UIInfo.DiffPworldID, data.UIInfo.TeamCreateID),data.UIInfo.TeamCreateID);
					}
				}
				GameManager::getSingleton().getScriptVM()->callString(script);
			}
			break;

		case DIFF_PWORLD_SVR_TIPS:
			{
				tipNum = data.Tips.Num;
				for (int i = 0; i < tipNum; ++i)
				{
					strcpy(tips[i].RoleName, data.Tips.Tips[i].RoleName);
					strcpy(tips[i].Msg, data.Tips.Tips[i].Msg);
				}
				char script[256];
				sprintf(script,"OpenPworldDifficultyTipsFrame(%d,%d)", data.Tips.DiffPworldID, getDifficulty(data.Tips.DiffPworldID, data.Tips.PworldID));
				GameManager::getSingleton().getScriptVM()->callString(script);
			}
			break;

		default:
			{
				if (PworldScore::getSingletonPtr())
				{
					result = PworldScore::getSingletonPtr()->HandleNetMsg(msgid, msgbody);
				}
			}
		}
	}
	return result;
}

int PworldDifficulty::getDifficulty(int diffPworldId, int pworldId)
{
	const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef(diffPworldId);
	if (diffPworldDef != NULL)
	{
		for (int i = 0; i < MAX_DIFF_PWORLD_LIST; ++i)
		{
			if (diffPworldDef->PworldInfo[i].PworldID == pworldId)
			{
				return i;
			}
		}
	}
	return -1;
}

int PworldDifficulty::getDifficulty(int pworldId)
{
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
	if (pworldDef != NULL)
	{
		return getDifficulty(pworldDef->DiffPworldID, pworldId);
	}
	return -1;
}

int PworldDifficulty::getDifficultyPworldId(int diffPworldId, int difficulty)
{
	const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef(diffPworldId);
	if (diffPworldDef != NULL)
	{
		return diffPworldDef->PworldInfo[difficulty].PworldID;
	}
	return 0;
}

const char * PworldDifficulty::getDifficultyDesc(int diffPworldId, int difficulty)
{
	const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef(diffPworldId);
	if (diffPworldDef != NULL)
	{
		return diffPworldDef->PworldInfo[difficulty].Desc;
	}
	return "";
}

int PworldDifficulty::getDifficultyReward(int diffPworldId, int difficulty, int index)
{
	const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef(diffPworldId);
	if (diffPworldDef != NULL)
	{
		return diffPworldDef->PworldInfo[difficulty].AwardItem[index];
	}
	return 0;
}

void PworldDifficulty::enterPworld(int npcId, int diffPworldId, int pworldId)
{
	realEnterPworld(npcId, diffPworldId, pworldId, false);
}

void PworldDifficulty::forceEnterPworld(int npcId, int diffPworldId, int pworldId)
{
	realEnterPworld(npcId, diffPworldId, pworldId, true);
}

void PworldDifficulty::realEnterPworld(int npcId, int diffPworldId, int pworldId, bool confirm)
{
	tagCSPkgBody msgbody;
	msgbody.DiffPworldClt.Type = DIFF_PWORLD_CLT_ENTER;
	msgbody.DiffPworldClt.Data.Enter.NpcID = npcId;
	msgbody.DiffPworldClt.Data.Enter.DiffPworldID = diffPworldId;
	msgbody.DiffPworldClt.Data.Enter.PworldID = pworldId;
	msgbody.DiffPworldClt.Data.Enter.EnterFlag = confirm ? 1 : 0;
	SendGameServerMsg(DIFF_PWORLD_CLT, msgbody);
}

const char * PworldDifficulty::getDiffPworldName(int diffPworldId)
{
	const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef(diffPworldId);
	if (diffPworldDef != NULL)
	{
		return diffPworldDef->NameDesc;
	}
	return "";
}

const char * PworldDifficulty::getDiffPworldDesc(int diffPworldId)
{
	const tagDiffPworldDef *diffPworldDef = DBDataManager::getSingleton().getDiffPworldDef(diffPworldId);
	if (diffPworldDef != NULL)
	{
		const tagActiveHelper * activeHelper = DBDataManager::getSingleton().getActiveHelper(diffPworldDef->ActiveID);
		if (activeHelper != NULL)
		{
			return activeHelper->ActiveDesc;
		}
	}
	return "";
}

int PworldDifficulty::getPworldReferMapId(int pworldId)
{
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
	if (pworldDef != NULL)
	{
		const tagMapDef &mapDef = DBDataManager::getSingleton().getMapDef(pworldDef->InitMapID);
		if (mapDef.ReferMapID == 0)
		{
			return mapDef.MapID;
		}
		return mapDef.ReferMapID;
	}
	return 0;
}

bool PworldDifficulty::isPworldLimit(int diffPworldId, int difficulty)
{
	PworldLimitData &data = PworldLimitInfo::getSingleton().getPworldData(getDifficultyPworldId(diffPworldId, difficulty));
	if (data.freeTimeTotal != 0 && data.freeTimeUse >= data.freeTimeTotal)
	{
		return true;
	}
	return false;
}
