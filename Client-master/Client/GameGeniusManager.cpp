#include "GameDBData.h"
#include "GameManager.h"
#include "GameGeniusManager.h"

GameGeniusManager::GameGeniusManager()
{
	Reset();
	INetworkManager::getSingleton().RegisterMsgHandler( GUNIUS_SVR, this );
}

const tagGeniusAttrDef_ToLua* GameGeniusManager::getGeniusAttrDef( int nCareer, int nAttrId, int nLv )
{
	static_assert( sizeof(tagGeniusAttrDef_ToLua) == sizeof(tagGeniusAttrDef) );
	return (const tagGeniusAttrDef_ToLua*)DBDataManager::getSingleton().getGeniusAttrDef(nCareer,nAttrId,nLv);
}

const tagGeniusCareerDef_ToLua*	GameGeniusManager::getGeniusCareerDef( int nCareer )
{
	static_assert( sizeof(tagGeniusCareerDef_ToLua) == sizeof(tagGeniusCareerDef) );
	return (const tagGeniusCareerDef_ToLua*)DBDataManager::getSingleton().getGeniusCareerDef(nCareer);
}

const tagGeniusAddDef_ToLua* GameGeniusManager::getGeniusAddDef( int nType, int nCareer )
{
	static_assert( sizeof(tagGeniusAddDef_ToLua) == sizeof(tagGeniusAddDef) );
	return (const tagGeniusAddDef_ToLua*)DBDataManager::getSingleton().getGeniusAddDef(nType,nCareer);
}

const tagGeniusMdfDef_ToLua* GameGeniusManager::getGeniusMdfDef( int nType )
{
	static_assert( sizeof(tagGeniusMdfDef_ToLua) == sizeof(tagGeniusMdfDef) );
	return (const tagGeniusMdfDef_ToLua*)DBDataManager::getSingleton().getGeniusMdfDef(nType);
}

void GameGeniusManager::Reset()
{
	memset( &m_GeniusInfo, 0, sizeof(tagGeniusInfo_ToLua) );
}

int GameGeniusManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == GUNIUS_SVR )
	{
		switch( msgbody.GuniusSvr.Cmd )
		{
		case GUNIUS_SVR_OP_ALL:
			{
				static_assert( sizeof(tagGeniusInfo_ToLua) == sizeof(tagGeniusInfo) );
				memcpy( &m_GeniusInfo, &msgbody.GuniusSvr.Data.All.All,sizeof(tagGeniusInfo) );
				GameManager::getSingleton().SendUIEvent("GE_GENIUS_UPDATE");
			}
			break;
		case GUNIUS_SVR_OP_ONE:
			{
				for( int i = 0 ; i < m_GeniusInfo.Num ; ++i )
				{
					if( m_GeniusInfo.One[i].Type == msgbody.GuniusSvr.Data.One.Type )
					{
						for( int j = 0 ; j < m_GeniusInfo.One[i].Num; ++j )
						{
							if( m_GeniusInfo.One[i].SubOne[j].AttrID == msgbody.GuniusSvr.Data.One.One.AttrID )
							{
								m_GeniusInfo.One[i].SubOne[j].Lvl = msgbody.GuniusSvr.Data.One.One.Lvl;
								GameManager::getSingleton().SendUIEvent("GE_GENIUS_UPDATE");
								break;
							}
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

int GameGeniusManager::getAttrLv( int nIdx, int nAttrId )
{
	if( nIdx < m_GeniusInfo.Num )
	{
		for( int i = 0 ; i < m_GeniusInfo.One[nIdx].Num; ++i )
		{
			if( m_GeniusInfo.One[nIdx].SubOne[i].AttrID == nAttrId )
			{
				return m_GeniusInfo.One[nIdx].SubOne[i].Lvl;
			}
		}
		return m_GeniusInfo.One[nIdx].Type;
	}
	return 0;
}

int GameGeniusManager::getGeniusType( int nIdx )
{
	if( nIdx < m_GeniusInfo.Num )
	{
		return m_GeniusInfo.One[nIdx].Type;
	}
	return 0;
}

int GameGeniusManager::getAllGeniusValue()
{
	int nVal = 0;
	for( int i = 0 ; i < m_GeniusInfo.Num ; ++i )
	{
		for( int j = 0 ; j < m_GeniusInfo.One[i].Num; ++j )
		{
			nVal += m_GeniusInfo.One[i].SubOne[j].Lvl;
		}
	}
	return nVal;
}

int GameGeniusManager::getGeniusTypeValue( int nType )
{
	assert( nType < MAX_GENIUS_TYPE );
	int nVal = 0;
	for ( int i = 0; i < m_GeniusInfo.One[nType].Num; ++i )
	{
		nVal += m_GeniusInfo.One[nType].SubOne[i].Lvl;
	}
	return nVal;
}

void GameGeniusManager::LevelUp( int nType, int nAttrId, int nLv, int nFlag )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.GuniusClt.Cmd = GUNIUS_CLT_OP_UP;
	msgbody.GuniusClt.Data.Up.AttrID = nAttrId;
	msgbody.GuniusClt.Data.Up.Type = nType;
	msgbody.GuniusClt.Data.Up.Lvl = nLv;
	msgbody.GuniusClt.Data.Up.Flag = nFlag;
	SendGameServerMsg( GUNIUS_CLT, msgbody );
}