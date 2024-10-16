#pragma once

//////////////////////////////////////////////////////////////////////////
// 符文操作接口
//////////////////////////////////////////////////////////////////////////
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>
#include <vector>

class Item;

//tolua_begin

#define RESET_RUNE_COST_MONEY RUNE_REST_COST_MONEY
struct RuneExInfo
{
	int attr;
	int num;
	int total;
};
//tolua_end

class GameRuneManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameRuneManager>
{//tolua_export
public:
	//tolua_begin
	enum
	{
		UNLOCK_SUCCEED,
		UNLOCK_DEAD,
		UNLOCK_LEVELLIMIT,
		UNLOCK_NOITEM,

		UNLOCK_UNKNOWN,
	};
	//tolua_end

public:
	GameRuneManager();
	~GameRuneManager();
	//tolua_begin
	bool	IsRuneItem(int itemId);
	int		RuneItemType(int itemid);
	bool	IsRuneEnable(int skillId);
	int		RuneItemId(int skillId);
	int		RuneItemId(int skillId, int type);

	bool	CanLock(int skillId);
	int		Unlock(int skillId);
	void	Insert(int skillId, Item *item);
	void	Erase(int skillId, Item *item);

	const RuneExInfo *GetExRuneInfo(int type);
	const char *	GetRuneAddAttrName(int attr);
	const char *	GetRuneAddAttrDesc(int attr);
	int				GetRuneAddAttrRecommend(int attr);
	int				GetNextRuneAddAttr(int attr = 0);

	void	SelectRuneSkill(int id, int type);

	void	CombineRuneItem( Item *heChengItem, Item *item1, Item *item2, Item *item3, Item *item4, Item *item5 );
	void	ResetExInfoNormal(int type);
	void	ResetExInfoSave(int type);
	//tolua_end

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void	SetExRuneInfo(int type, int attr, int num, int total);

private:
	std::map<int, RuneExInfo>	ExRuneInfos;
	std::vector<tagRuneAddAttrDef>	RuneAddAttrs;
	std::map<int, size_t>		RuneAddAttrIdxs;
};//tolua_export