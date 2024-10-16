#include "GameRuneManager.h"
#include "GameSkill_Mgr.h"
#include "Item.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"

GameRuneManager::GameRuneManager()
{
	DBDataManager::getSingleton().loadRuneAddAttrDef(RuneAddAttrs);
	INetworkManager::getSingleton().RegisterMsgHandler(RUNE_SVR, this);
	for (size_t i = 0; i < RuneAddAttrs.size(); ++i)
	{
		RuneAddAttrIdxs[RuneAddAttrs[i].ID] = i;
	}
}

GameRuneManager::~GameRuneManager()
{

}

bool GameRuneManager::IsRuneItem(int itemId)
{
	const tagItemDef &itemdef = DBDataManager::getSingleton().getItemDef(itemId);
	for (int i = 0; i < MAX_ITEM_RESULT; ++i)
	{
		if (itemdef.Result[i].ResultID == RESULT_RUNE)
		{
			return true;
		}
	}

	return false;
}

int GameRuneManager::RuneItemType(int itemid)
{
	const tagItemDef &itemdef = DBDataManager::getSingleton().getItemDef(itemid);
	for (int i = 0; i < MAX_ITEM_RESULT; ++i)
	{
		if (itemdef.Result[i].ResultID == RESULT_RUNE)
		{
			return itemdef.Result[i].ResultVal2;
		}
	}

	return 0;
}

bool GameRuneManager::IsRuneEnable(int skillId)
{
	return SkillManager::getSingleton().getSpellRuneEnable(skillId);
}

int GameRuneManager::RuneItemId(int skillId)
{
	return SkillManager::getSingleton().getSpellRuneItemId(skillId);
}

int GameRuneManager::RuneItemId(int skillId, int type)
{
	return SkillManager::getSingleton().getSpellRuneItemId(skillId, type);
}

int GameRuneManager::Unlock(int skillId)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return UNLOCK_UNKNOWN;
	}
	if (pMainPlayer->isDead())
	{
		return UNLOCK_DEAD;
	}
	if (pMainPlayer->getLv() < MIN_RUNE_LEVEL)
	{
		return UNLOCK_LEVELLIMIT;
	}
	
	tagCSPkgBody msgbody;
	msgbody.RuneClt.Operation = RUNE_CLT_ENABLE;
	msgbody.RuneClt.Extension.RuneEnable.SkillID = skillId;
	SendGameServerMsg(RUNE_CLT, msgbody);

	return UNLOCK_SUCCEED;
}

void GameRuneManager::Insert(int skillId, Item *item)
{
	tagCSPkgBody msgbody;
	msgbody.RuneClt.Operation = RUNE_CLT_SET;
	msgbody.RuneClt.Extension.RuneSet.SkillID = skillId;
	msgbody.RuneClt.Extension.RuneSet.RuneItem.ListType = item->getListType();
	msgbody.RuneClt.Extension.RuneSet.RuneItem.GridIdx = item->getGridIdx();
	msgbody.RuneClt.Extension.RuneSet.RuneItem.WID = item->getItemWId();
	SendGameServerMsg(RUNE_CLT, msgbody);
}

void GameRuneManager::Erase(int skillId, Item *item)
{
	tagCSPkgBody msgbody;
	msgbody.RuneClt.Operation = RUNE_CLT_DEL;
	msgbody.RuneClt.Extension.RuneDel.SkillID = skillId;
	msgbody.RuneClt.Extension.RuneDel.RuneItem.ListType = item->getListType();
	msgbody.RuneClt.Extension.RuneDel.RuneItem.GridIdx = item->getGridIdx();
	msgbody.RuneClt.Extension.RuneDel.RuneItem.WID = item->getItemWId();
	SendGameServerMsg(RUNE_CLT, msgbody);
}

int GameRuneManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (msgid == RUNE_SVR)
	{
		const tagCSRuneSvrExtension &data = msgbody.RuneSvr.Extension;
		switch (msgbody.RuneSvr.Operation)
		{
		case RUNE_SVR_ENABLE:
			{
				char script[100];
				sprintf(script, "RuneResult('unlock', %d, %s)", data.RuneEnable.SkillID, data.RuneEnable.Succeeded ? "true" : "false");
				GameManager::getSingleton().getScriptVM()->callString(script);
			}
			break;

		case RUNE_SVR_SET:
			{
				char script[100];
				sprintf(script, "RuneResult('insert', %d, %s)", data.RuneSet.SkillID, data.RuneSet.Succeeded ? "true" : "false");
				GameManager::getSingleton().getScriptVM()->callString(script);
			}
			break;

		case RUNE_SVR_DEL:
			{
				char script[100];
				sprintf(script, "RuneResult('erase', %d, %s)", data.RuneDel.SkillID, data.RuneDel.Succeeded ? "true" : "false");
				GameManager::getSingleton().getScriptVM()->callString(script);
			}
			break;
		}
	}
	return 0;
}

bool GameRuneManager::CanLock(int skillId)
{
	const tagSkillDef &skillDef = DBDataManager::getSingleton().GetSkillDef(skillId, SkillManager::getSingleton().getSpellLv(skillId));
	return skillDef.RedRuneTransferedID != 0 ||
		skillDef.BlueRuneTransferedID != 0 ||
		skillDef.YellowRuneTransferedID != 0;
}

void GameRuneManager::SetExRuneInfo(int type, int attr, int num, int total)
{
	RuneExInfo info;
	info.attr = attr;
	info.num = num;
	info.total = total;
	ExRuneInfos[type] = info;
	GameManager::getSingleton().SendUIEvent("GE_RUNE_EX_INFO_UPDATE");
}

const RuneExInfo * GameRuneManager::GetExRuneInfo(int type)
{
	std::map<int, RuneExInfo>::iterator i = ExRuneInfos.find(type);
	if (i != ExRuneInfos.end())
	{
		return &(i->second);
	}
	return NULL;
}

const char * GameRuneManager::GetRuneAddAttrName(int attr)
{
	const tagRuneAddAttrDef *RuneAddAttrDef = DBDataManager::getSingleton().getRuneAddAttrDef(attr);
	if (RuneAddAttrDef == NULL)
	{
		return "";
	}
	return RuneAddAttrDef->Name;
}

const char * GameRuneManager::GetRuneAddAttrDesc(int attr)
{
	const tagRuneAddAttrDef *RuneAddAttrDef = DBDataManager::getSingleton().getRuneAddAttrDef(attr);
	if (RuneAddAttrDef == NULL)
	{
		return "";
	}
	return RuneAddAttrDef->AddAttrDesc;
}

int GameRuneManager::GetRuneAddAttrRecommend(int attr)
{
	const tagRuneAddAttrDef *RuneAddAttrDef = DBDataManager::getSingleton().getRuneAddAttrDef(attr);
	if (RuneAddAttrDef == NULL)
	{
		return 0;
	}
	return RuneAddAttrDef->Recommend;
}

void GameRuneManager::ResetExInfoNormal(int type)
{
	tagCSPkgBody msgbody;
	msgbody.ActionReq.ActionID = ACTION_RESET_RUNE_EX;
	msgbody.ActionReq.Data.ResetRuneEx.RuneType = type;
	msgbody.ActionReq.Data.ResetRuneEx.OpType = RUNE_REST_NORMAL;
	SendGameServerMsg(ACTION_REQ, msgbody);
}

void GameRuneManager::ResetExInfoSave(int type)
{
	tagCSPkgBody msgbody;
	msgbody.ActionReq.ActionID = ACTION_RESET_RUNE_EX;
	msgbody.ActionReq.Data.ResetRuneEx.RuneType = type;
	msgbody.ActionReq.Data.ResetRuneEx.OpType = RUNE_REST_SAVE;
	SendGameServerMsg(ACTION_REQ, msgbody);
}

void GameRuneManager::SelectRuneSkill(int id, int type)
{
	tagCSPkgBody msgbody;
	msgbody.ActionReq.ActionID = ACTION_RUNNE_SKILL_CHG;
	msgbody.ActionReq.Data.RunneSkillChg.SkillID = id;
	msgbody.ActionReq.Data.RunneSkillChg.RuneType = type;
	SendGameServerMsg(ACTION_REQ, msgbody);
}

void GameRuneManager::CombineRuneItem( Item *heChengItem, Item *item1, Item *item2, Item *item3, Item *item4, Item *item5 )
{
	tagCSPkgBody msgbody;
	msgbody.ActionReq.ActionID = ACTION_RUNE_LEVEL_UP;
	msgbody.ActionReq.Data.RuneLevelUp.LevelUpItem.ListType = heChengItem->getListType();
	msgbody.ActionReq.Data.RuneLevelUp.LevelUpItem.GridIdx = heChengItem->getGridIdx();
	msgbody.ActionReq.Data.RuneLevelUp.LevelUpItem.WID = heChengItem->getItemWId();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[0].ListType = item1->getListType();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[0].GridIdx = item1->getGridIdx();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[0].WID = item1->getItemWId();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[1].ListType = item2->getListType();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[1].GridIdx = item2->getGridIdx();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[1].WID = item2->getItemWId();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[2].ListType = item3->getListType();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[2].GridIdx = item3->getGridIdx();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[2].WID = item3->getItemWId();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[3].ListType = item4->getListType();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[3].GridIdx = item4->getGridIdx();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[3].WID = item4->getItemWId();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[4].ListType = item5->getListType();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[4].GridIdx = item5->getGridIdx();
	msgbody.ActionReq.Data.RuneLevelUp.RuneItem[4].WID = item5->getItemWId();
	SendGameServerMsg(ACTION_REQ, msgbody);
}

int GameRuneManager::GetNextRuneAddAttr(int attr /*= 0*/)
{
	std::map<int, size_t>::iterator i;
	if (attr == 0)
	{
		i = RuneAddAttrIdxs.begin();
	}
	else
	{
		i = RuneAddAttrIdxs.find(attr);
		if (i != RuneAddAttrIdxs.end())
		{
			++i;
		}
	}
	if (i != RuneAddAttrIdxs.end())
	{
		return i->first;
	}
	return 0;
}
