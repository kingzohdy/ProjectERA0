#include "ClanInfo.h"

ClanInfo::ClanInfo()
{

}

ClanInfo::~ClanInfo()
{

}

ClanInfo & ClanInfo::getSingleton()
{
	static ClanInfo singleton;
	return singleton;
}

void ClanInfo::setClanName(tdr_ulonglong gid, int id, const char *name)
{
	IdMap[gid] = id;
	GIdMap[id] = gid;
	NameMap[gid] = name;
}

const char * ClanInfo::getClanName(tdr_ulonglong gid)
{
	std::map<tdr_ulonglong, std::string>::iterator i = NameMap.find(gid);
	if (i == NameMap.end())
	{
		return "";
	}
	return i->second.c_str();
}

const char * ClanInfo::getClanName(int id)
{
	std::map<int, tdr_ulonglong>::iterator i = GIdMap.find(id);
	if (i == GIdMap.end())
	{
		return "";
	}
	return getClanName(i->second);
}

void ClanInfo::resetClanInfo()
{
	IdMap.clear();
	GIdMap.clear();
	NameMap.clear();
}
