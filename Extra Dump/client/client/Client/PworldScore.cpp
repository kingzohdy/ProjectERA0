#include "PworldScore.h"
#include "NetMessage.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"

const PworldScore::Info & PworldScore::Info::operator = (const tagCSDiffPworldAwardUI &info)
{
	DiffPworldID = info.DiffPworldID;
	PworldID = info.PworldID;
	Normal = info.NormalVal;
	JingYing = info.JingVal;
	Boss = info.BossVal;
	Time = info.TimeVal;
	TimeCost = info.TimeSec;
	Death = info.DieVal;
	DeathNum = info.DieNum;
	Total = info.TotalVal;
	Grade = info.ValGrade;
	Exp = info.Exp;
	Money = info.Money;
	GradeExp = info.GradeExp;
	DiffExp = info.DiffExp;
	FriendExp = info.FriendExp;
	ClanExp = info.ClanExp;
	CardNum = info.CardNum;
	NormalNum = info.NormalNum;
	JingNum = info.JingNum;
	BossNum = info.BossNum;
	OffExp = info.OffExp;
	VipCardNum = info.VipCardNum;
	for (int i = 0; i < CardNum; ++i)
	{
		if (info.CardsInfo[i].Grid.Type == GRID_ITEM_ITEM)
		{
			Cards[i] = info.CardsInfo[i].Grid.GridData.RoleItem;
		}
		else
		{
			Cards[i] = info.CardsInfo[i].Grid.GridData.RoleArm;
		}
	}
	for (int i = 0; i < VipCardNum; ++i)
	{
		if (info.VipCardsInfo[i].Grid.Type == GRID_ITEM_ITEM)
		{
			VipCards[i] = info.VipCardsInfo[i].Grid.GridData.RoleItem;
		}
		else
		{
			VipCards[i] = info.VipCardsInfo[i].Grid.GridData.RoleArm;
		}
	}

	return *this;
}

PworldScore::PworldScore()
{
}

PworldScore::~PworldScore()
{
}

int PworldScore::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	const tagCSDiffPworldSvrData &data = msgbody.DiffPworldSvr.Data;
	switch (msgbody.DiffPworldSvr.Type)
	{
	case DIFF_PWORLD_SVR_AWARD_UI:
		{
			reset();
			AwardInfo = data.AwardUI;
			GameManager::getSingleton().getScriptVM()->callString("OpenPworldScoreFrame()");
		}
		break;

	case DIFF_PWORLD_SVR_UPDATE_CARD:
		{
			MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
			for (int i = 0; i < data.Cards.CardNum; ++i)
			{
				int idx = data.Cards.CardsInfo[i].GridIdx;
				selectList.insert(idx);
				strcpy(AwardCards[idx].name, data.Cards.CardsInfo[i].RoleName);
				if (data.Cards.CardsInfo[i].Grid.Type == GRID_ITEM_ITEM)
				{
					AwardCards[idx].item = data.Cards.CardsInfo[i].Grid.GridData.RoleItem;
				}
				else
				{
					AwardCards[idx].item = data.Cards.CardsInfo[i].Grid.GridData.RoleArm;
				}
			}
			char script[100];
			sprintf(script, "UpdatePworldScoreCards(%s)", data.Cards.CardNum == MAX_PWORLD_AWARD_CARD ? "true" : "");
			GameManager::getSingleton().getScriptVM()->callString(script);
		}
		break;

	case DIFF_PWORLD_SVR_CARD_MSG:
		{
			char script[MAX_DIFF_PWORLD_TIP_LEN + 100];
			sprintf(script, "UpdatePworldScoreCardMsg([===[%s]===])", data.CardMsg.Msg);
			GameManager::getSingleton().getScriptVM()->callString(script);
		}
		break;

	case DIFF_PWORLD_SVR_SELECT:
		{
			if (data.Select.Succ)
			{
				selected = true;
			}
			char script[100];
			sprintf(script, "SelectPworldScoreCardResult(%s)", data.Select.Succ ? "true" : "");
			GameManager::getSingleton().getScriptVM()->callString(script);
		}
		break;
	case DIFF_PWORLD_SVR_UPDATE_VIP_CARD:
		{
			MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
			for (int i = 0; i < data.VipCards.CardNum; ++i)
			{
				int idx = data.VipCards.CardsInfo[i].GridIdx;
				vipSelectList.insert(idx);
				strcpy(VipAwardCards[idx].name, data.VipCards.CardsInfo[i].RoleName);
				if (data.VipCards.CardsInfo[i].Grid.Type == GRID_ITEM_ITEM)
				{
					VipAwardCards[idx].item = data.VipCards.CardsInfo[i].Grid.GridData.RoleItem;
				}
				else
				{
					VipAwardCards[idx].item = data.VipCards.CardsInfo[i].Grid.GridData.RoleArm;
				}
			}
			char script[100];
			sprintf(script, "UpdatePworldScoreVipCards(%s)", data.VipCards.CardNum == MAX_PWORLD_VIP_AWARD_CARD ? "true" : "");
			GameManager::getSingleton().getScriptVM()->callString(script);
		}
		break;
	case DIFF_PWORLD_SVR_SELECT_VIP:
		{
			if (data.Select.Succ)
			{
				vipSelected = true;
			}
			char script[100];
			sprintf(script, "SelectPworldScoreVipCardResult(%s)", data.Select.Succ ? "true" : "");
			GameManager::getSingleton().getScriptVM()->callString(script);
		}
		break;
	}
	return 0;
}

void PworldScore::SelectCard(int index)
{
	selectedIdx = index;
	tagCSPkgBody msgbody;
	msgbody.DiffPworldClt.Type = DIFF_PWORLD_CLT_SELECT_CARD;
	msgbody.DiffPworldClt.Data.Select.GridIdx = index;
	SendGameServerMsg(DIFF_PWORLD_CLT, msgbody);
}

void PworldScore::SelectVipCard(int index)
{
	vipSelectedIdx = index;
	tagCSPkgBody msgbody;
	msgbody.DiffPworldClt.Type = DIFF_PWORLD_CLT_SELECT_VIP_CARD;
	msgbody.DiffPworldClt.Data.VipSelect.GridIdx = index;
	SendGameServerMsg(DIFF_PWORLD_CLT,msgbody);
}

bool PworldScore::IsSelected()
{
	return selected;
}

bool PworldScore::IsCardSelected(int idx)
{
	return selectList.find(idx) != selectList.end();
}

int PworldScore::GetNextSelecteCard(int idx /*= -1*/)
{
	IntList::iterator i;
	if (idx == -1)
	{
		i = selectList.begin();
	}
	else
	{
		i = selectList.find(idx);
		if (i != selectList.end())
		{
			++i;
		}
	}
	if (i != selectList.end())
	{
		return *i;
	}
	return -1;
}

void PworldScore::reset()
{
	selectList.clear();
	selected = false;
	vipSelectList.clear();
	vipSelected = false;
}

int PworldScore::GetSelected()
{
	if (!selected)
	{
		return -1;
	}
	return selectedIdx;
}

bool PworldScore::IsVipSelected()
{
	return vipSelected;
}

bool PworldScore::IsVipCardSelected(int idx)
{
	return vipSelectList.find(idx) != vipSelectList.end();
}

int PworldScore::GetNextVipSelectCard(int idx /*= -1*/)
{
	IntList::iterator i;
	if (idx == -1)
	{
		i = vipSelectList.begin();
	}
	else
	{
		i = vipSelectList.find(idx);
		if (i != vipSelectList.end())
		{
			++i;
		}
	}
	if (i != vipSelectList.end())
	{
		return *i;
	}
	return -1;
}

int PworldScore::GetVipSelected()
{
	if (!vipSelected)
	{
		return -1;
	}
	return vipSelectedIdx;
}