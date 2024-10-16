#pragma once
#ifndef __CLANINFO_H
#define __CLANINFO_H

#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>

class ClanInfo
{
protected:
	ClanInfo();

public:
	~ClanInfo();
	void setClanName(tdr_ulonglong gid, int id, const char *name);
	const char * getClanName(tdr_ulonglong gid);
	const char * getClanName(int id);
	void resetClanInfo();

public:
	static ClanInfo & getSingleton();

private:
	std::map<tdr_ulonglong, int>	IdMap;
	std::map<int, tdr_ulonglong>	GIdMap;
	std::map<tdr_ulonglong, std::string>	NameMap;
};

#endif