#pragma once
#ifndef __DREAMLAND_H__
#define __DREAMLAND_H__

#include <map>
#include <vector>
#include "proto_comm.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreSingleton.h"
#include "GameMsgPrint.h"

//tolua_begin
struct UnrealTeamInfo
{
	int Index;
	char Name[ROLE_NAME_LEN];
	unsigned char Level;
	unsigned char Count;
	unsigned char Layer;
};
//tolua_end

class DreamLandManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<DreamLandManager>
{//tolua_export
public:
	DreamLandManager();
	~DreamLandManager();

public:
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	void SelectLayer(int layer);
	bool IsLayerSaved(int layer);
	bool IsBossLayerExist(int layer);
	const char * GetLayerName(int layer);
	int GetCurrentLayer();
	const char * GetCurrentLayerName();
	const UnrealTeamInfo * GetTeamNextInfo(const UnrealTeamInfo *info);
	const UnrealTeamInfo * GetTeamInfo( int nIndex );
	const char* GetCurrentLayerEffectName();
	int GetTeamNum();

	int	GetBufferLayer(){ return m_nBufferLayer; }
	//tolua_end

	void ExitLayer();

	void SetBufferLayer( int nLayer ){ m_nBufferLayer = nLayer; }

private:
	int		savedLayer;
	int		currentLayer;
	std::map<int, tagBossInfo>		bossInfo;
	std::vector<UnrealTeamInfo>		teamInfo;

	int		m_nBufferLayer;								//高倍层数，可以获取更多的经验
};//tolua_export

#endif
