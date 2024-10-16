#pragma once
#include <vector>
#include "proto_comm.h"
#include "NetMessage.h"
#include "proto_cs.h"

class MapLineSelectManager//tolua_export
	: public NetMsgHandler
{//tolua_export
protected:
	MapLineSelectManager(void);

public:
	~MapLineSelectManager(void);
	virtual int  HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	//tolua_begin
	int		getMapLineNum();
	int		getMapLineId(int i);
	int		getMapLineState(int i);
	void	requestChangeMapLine(int id);
	void	requestMapLineInfo();
	int		getCurMapId();
	//tolua_end

public:
	static MapLineSelectManager &getSingleton();

private:
	std::vector<CSMAPLINEDATA>	mapLines;
	int							curMapId;
};//tolua_export
