#pragma once
#ifndef __MAP_ORE_H__
#define __MAP_ORE_H__

#include "proto_comm.h"
#include "proto_cs.h"
#include <vector>

class MapOreInfo//tolua_export
{//tolua_export
protected:
	MapOreInfo(void);

public:
	~MapOreInfo(void);

	void SetOre(int num, const tagCSMapOreInfo infos[]);
	void SetBigOre(int num, const tagCSMapOreInfo infos[]);

	//tolua_begin
	int GetOreCount();
	int GetOreId(int idx);
	int GetOreMemId(int idx);
	int GetOreMap(int idx);
	int GetOreX(int idx);
	int GetOreY(int idx);

	int GetBigOreCount();
	int GetBigOreId(int idx);
	int GetBigOreMemId(int idx);
	int GetBigOreMap(int idx);
	int GetBigOreX(int idx);
	int GetBigOreY(int idx);

	int GetOreDetectRadius();
	bool CanCollectOre(int id);
	//tolua_end

public:
	static MapOreInfo &getSingleton();

private:
	std::vector<tagCSMapOreInfo> Ores;
	std::vector<tagCSMapOreInfo> BigOres;
};//tolua_export

#endif
