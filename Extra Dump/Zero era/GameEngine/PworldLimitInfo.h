#pragma once

#include "GameEngineTypes.h"
#include <map>

//tolua_begin
struct PworldLimitData
{
	int freeTimeTotal;
	int freeTimeUse;
	int itemId;
	int itemTimeTotal;
	int itemTimeUse;

	PworldLimitData()
	{
		memset(this, 0, sizeof(PworldLimitData));
	}
};
//tolua_end

class PworldLimitInfo//tolua_export
	: public NetMsgHandler
{//tolua_export
protected:
	PworldLimitInfo();

public:
	~PworldLimitInfo();
	int		getCurPworldId();
	void	onEnterMap(int id);
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	static PworldLimitInfo & getSingleton();

public:
	//tolua_begin
	PworldLimitData & getPworldData(int id);
	bool canEnterPworld( int nId );
	//tolua_end

private:
	std::map<int, PworldLimitData>		m_pworldData;
	int					pworldId;
};//tolua_export