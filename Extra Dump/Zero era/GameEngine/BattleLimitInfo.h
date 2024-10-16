#pragma once

#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>

//tolua_begin
struct BattleLimitData
{
	int	index;
	int	Total;
	int	Use;

	BattleLimitData()
	{
		memset(this, 0, sizeof(BattleLimitData));
	}
};
//tolua_end

class BattleLimitInfo//tolua_export
	: public NetMsgHandler
{//tolua_export
protected:
	BattleLimitInfo();

public:
	~BattleLimitInfo();
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	static BattleLimitInfo & getSingleton();

public:
	const tagBattleDef *getBattleDef(size_t i);
	const tagBattleDef *getBattleDefById(int id);
	size_t		getBattleNum();

	//tolua_begin
	BattleLimitData & getBattleData(int id);
	//tolua_end

private:
	std::map<int, BattleLimitData>		m_battleData;
	std::vector<tagBattleDef*>			m_arrBattle;
};//tolua_export