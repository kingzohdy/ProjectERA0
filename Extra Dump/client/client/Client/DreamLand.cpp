#include "DreamLand.h"
#include "GameManager.h"
#include "GameDBData.h"
#include "OgreScriptLuaVM.h"

DreamLandManager::DreamLandManager()
{
	ExitLayer();
	INetworkManager::getSingleton().RegisterMsgHandler(UNREAL_SVR, this);
}

DreamLandManager::~DreamLandManager()
{

}

int DreamLandManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (UNREAL_SVR == msgid)
	{
		const tagCSUnrealSvr &data = msgbody.UnrealSvr.Data;
		switch (msgbody.UnrealSvr.Cmd)
		{
		case UNREAL_SVR_OP_SAVE_INFO:
			{
				savedLayer = data.Save.Save;
				GameManager::getSingleton().SendUIEvent("GE_DREAMLAND_LAYER_SAVE");
			}
			break;

		case UNREAL_SVR_OP_MON_INFO:
			{
				currentLayer = data.Mon.Layer;
				GameManager::getSingleton().SendUIEvent("GE_DREAMLAND_ENTER");
			}
			break;

		case UNREAL_SVR_OP_TEAM_INFO:
			{
				
				teamInfo.resize(data.Team.TeamNum);
				for (int i = 0; i < data.Team.TeamNum; ++i)
				{
					const tagUnrealTeamOne &info = data.Team.TeamInfos[i];
					strcpy(teamInfo[i].Name, info.CaptainName);
					teamInfo[i].Level = info.Level;
					teamInfo[i].Count = info.Num;
					teamInfo[i].Layer = info.Layer;

					teamInfo[i].Index = i;
				}
				{
					char script[100];
					sprintf(script, "OpenDreamLandTeamFrame(%d)", data.Team.NpcID);
					GameManager::getSingleton().getScriptVM()->callString(script);
				}
			}
			break;

		case UNREAL_SVR_OP_BOSS_INFO:
			{
				bossInfo.clear();
				for (int i = 0; i < data.Boss.Num; ++i)
				{
					const tagBossInfo &info = data.Boss.BossInfo[i];
					if (info.ID != 0)
					{
						const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef(info.ID);
						bossInfo[monsterDef.BossType] = info;
					}
				}
				GameManager::getSingleton().SendUIEvent("GE_DREAMLAND_BOSSINFO");
				{
					char script[100];
					sprintf(script, "OpenDreamLandBossFrame(%d)", data.Boss.NpcID);
					GameManager::getSingleton().getScriptVM()->callString(script);
				}
			}
			break;
		}
	}
	return 0;
}

void DreamLandManager::SelectLayer(int layer)
{
	tagCSPkgBody msgbody;
	msgbody.UnrealClt.Cmd = UNREAL_CLT_OP_ENTER;
	msgbody.UnrealClt.Data.Enter.Layer = layer;
	SendGameServerMsg(UNREAL_CLT, msgbody);
}

bool DreamLandManager::IsLayerSaved(int layer)
{
	if (layer <= 10)
	{
		return true;
	}
	return (savedLayer & (1 << ((layer - 1) / 10 - 1))) != 0;
}

bool DreamLandManager::IsBossLayerExist(int layer)
{
	return bossInfo.find(layer) != bossInfo.end();
}

const UnrealTeamInfo * DreamLandManager::GetTeamNextInfo(const UnrealTeamInfo *info)
{
	int index = 0;
	if (info != NULL)
	{
		index = info->Index + 1;
	}
	if (teamInfo.size() <= index)
	{
		return NULL;
	}
	return &teamInfo[index];
}

const UnrealTeamInfo * DreamLandManager::GetTeamInfo( int nIndex )
{
	assert( nIndex <= (int)teamInfo.size() );
	return &teamInfo[nIndex];
}

const char * DreamLandManager::GetLayerName(int layer)
{
	const tagUnrealDef *unreadDef = DBDataManager::getSingleton().getUnrealDef(layer);
	if (unreadDef == NULL)
	{
		return NULL;
	}
	return unreadDef->Name;
}

int DreamLandManager::GetCurrentLayer()
{
	return currentLayer;
}

void DreamLandManager::ExitLayer()
{
	currentLayer = 0;
}

const char * DreamLandManager::GetCurrentLayerName()
{
	int layer = GetCurrentLayer();
	if (layer == 0)
	{
		return NULL;
	}
	return GetLayerName(layer);
}

const char* DreamLandManager::GetCurrentLayerEffectName()
{
	int layer = GetCurrentLayer();
	if( layer == 0 )
	{
		return NULL;
	}
	const tagUnrealDef *unreadDef = DBDataManager::getSingleton().getUnrealDef(layer);
	if (unreadDef == NULL)
	{
		return NULL;
	}
	return unreadDef->EffectName;
}

int DreamLandManager::GetTeamNum()
{
	return teamInfo.size();	
}