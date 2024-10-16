#include "GameDesignation.h"
#include "GameDBData.h"
#include "OgreScriptLuaVM.h"
#include "OgreSingleton.h"
#include "ServerTimer.h"
#include "GameManager.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include <time.h>

GameDesignation::GameDesignation(void) : m_nCurDesignationId(0)
{
	INetworkManager::getSingleton().RegisterMsgHandler( DESIGNATION_SVR, this );
	DBDataManager::getSingleton().loadDesignation( m_mapDesignationDef, m_vectDesignationDef );
}

GameDesignation::~GameDesignation(void)
{
}

// 向服务器发送设置称号请求
void GameDesignation::sendSetDesignationReq( int nId )
{
	tagCSPkgBody msgbody;
	msgbody.DesignationClt.DesignationOp					= DESIGNATION_CLT_SET;
	msgbody.DesignationClt.DesignationData.DesignationSetID = nId;
	SendGameServerMsg( DESIGNATION_CLT, msgbody );
}

int GameDesignation::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid == DESIGNATION_SVR )
	{
		const CSDESIGNATIONSVR& designationServerMsg = msgbody.DesignationSvr; 
		switch(	designationServerMsg.DesignationOp )
		{
		case DESIGNATION_SVR_INFO:
			{
				m_vectHaveDesignation.clear();
				for(unsigned int i = 0; i < designationServerMsg.DesignationData.DesignationInfo.Num; i++ )
				{
					m_vectHaveDesignation.push_back( designationServerMsg.DesignationData.DesignationInfo.Designations[i] );
				}
				sortAllDesignation();
				m_nCurDesignationId = designationServerMsg.DesignationData.DesignationInfo.CurrentDesignationID;
				GameMsgPrint::getSingleton().SendUIEvent("GE_DESIGNATION_UPDATE");
			}
			break;
		case DESIGNATION_SVR_ADD:
			{
				m_vectHaveDesignation.push_back( designationServerMsg.DesignationData.DesignationAdd );
				sortAllDesignation();				
                char szScript[256];
                sprintf( szScript,"AddDesignation(%d)", designationServerMsg.DesignationData.DesignationAdd.ID );
                GameManager::getSingleton().getScriptVM()->callString( szScript );
			}
			break;
		case DESIGNATION_SVR_SET_RES:
			{
				const CSDESIGNATIONSETRES& designationRes = designationServerMsg.DesignationData.DesignationSetRes;
				int nPreDesignationId = m_nCurDesignationId;
				if( designationRes.Succeeded != 0 )
                {
                    m_nCurDesignationId = designationRes.DesignationID;
					VECTHAVEDESIGNATION::iterator iter = m_vectHaveDesignation.begin();
					for( ; iter != m_vectHaveDesignation.end(); iter++ )
					{
						if( iter->ID == m_nCurDesignationId )
						{
							iter->LastUseTime = designationRes.LastUseTime;
						}
					}
				}
				
				if ( !( designationRes.Succeeded != 0 && nPreDesignationId != 0 && nPreDesignationId == m_nCurDesignationId ) )
				{
					char szScript[256];
					sprintf( szScript,"DesignationSetResult(%d,%d)", designationRes.Succeeded, m_nCurDesignationId );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
				}
			}
			break;
		}

        // 设置玩家称号
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
        if( pMainPlayer != NULL )
        {
            if( m_nCurDesignationId != 0 )
            {
                const char* szDesignationName = DBDataManager::getSingleton().getDesignationDef( m_nCurDesignationId ).Name;
                pMainPlayer->setDesignation( szDesignationName );
				pMainPlayer->setDesignationID( m_nCurDesignationId );
            }else{
                pMainPlayer->setDesignation( "" );
				pMainPlayer->setDesignationID( 0 );
            }
			GameMsgPrint::getSingleton().SendUIEvent("GE_CUR_DESIGNATION");
        }
	}
	return 0;
}

// 获取称号到期时间
int GameDesignation::getDesignationEndTime( int nId )
{
	VECTHAVEDESIGNATION::iterator iter = m_vectHaveDesignation.begin();
	for( ; iter != m_vectHaveDesignation.end(); iter++ )
	{
		if( iter->ID == nId )
		{
			return iter->EndTime;
		}
	}
}

// 获取全部的称号总数
int GameDesignation::getDesignationNum()
{
	return (int)m_vectDesignationDef.size();
}

// 按类型获取称号的类型总数
int GameDesignation::getDesignationNum( std::string strCategoryName )
{
	MAPDESIGNATIONDEF::iterator iter = m_mapDesignationDef.find( strCategoryName );
	if( iter != m_mapDesignationDef.end() )
	{
		return (int)iter->second.size();
	}
	return 0;
}

// 获取称号
tagDesignationDef_ToLua* GameDesignation::getDesignation( int nIndex )
{
	assert( nIndex <= (int)m_vectDesignationDef.size() );
	return (tagDesignationDef_ToLua*)m_vectDesignationDef[nIndex];
}

// 获取称号
tagDesignationDef_ToLua* GameDesignation::getDesignation( std::string strCategory, int nIndex )
{
	MAPDESIGNATIONDEF::iterator iter = m_mapDesignationDef.find( strCategory );
	if( iter != m_mapDesignationDef.end() )
	{
		assert( nIndex <= (int)( iter->second ).size() );
		return (tagDesignationDef_ToLua*)( iter->second )[nIndex];
	}

	return NULL;
}

// 根据称号ID获取称号信息
tagDesignationDef_ToLua* GameDesignation::getDesignationById( int nId )
{
    VECTDESIGNATIONDEF::iterator iter = m_vectDesignationDef.begin();
    for( ; iter != m_vectDesignationDef.end(); iter++ )
    {
        if( (*iter)->ID == nId )
        {
            return (tagDesignationDef_ToLua*)(*iter);
        }
    }
    return NULL;
}

// 获取称号状态名称
const char* GameDesignation::getDesignationBufName( int nBufId, int nBufLv )
{
	if( nBufId == 0 || nBufLv == 0 )
	{
		return "";
	}
	return DBDataManager::getSingleton().getStatusDef( nBufId, nBufLv ).StatusDesc;
}

// 是否当日首次设置称号
bool GameDesignation::isFirstSetDesignation( int nId )
{
	VECTHAVEDESIGNATION::iterator iter = m_vectHaveDesignation.begin();
	for( ; iter != m_vectHaveDesignation.end(); iter++ )
	{
		if( iter->LastUseTime == 0 )
		{
			return true;
		}

		if( iter->ID == nId && isToday( iter->LastUseTime ) )
		{
			return false;
		}
	}
	return true;
}

// 是否已拥有的称号
bool GameDesignation::isHaveDesignation( int nId  )
{
	VECTHAVEDESIGNATION::iterator iter = m_vectHaveDesignation.begin();
	for( ; iter != m_vectHaveDesignation.end(); iter++ )
	{
		if( iter->ID == nId )
		{
			return true;
		}
	}
	return false;
}

void GameDesignation::setActorDesignation( int nDesignationId, GameActor* pActor )
{
	if( pActor != NULL )
	{
		if( nDesignationId != 0 )
		{
			const char* szDesignationName = DBDataManager::getSingleton().getDesignationDef( nDesignationId ).Name;
			pActor->setDesignation( szDesignationName );
			pActor->setDesignationID( nDesignationId );
		}else{
			pActor->setDesignation( "" );
			pActor->setDesignationID( 0 );
		}
	}
}

void GameDesignation::sortDesignation( VECTDESIGNATIONDEF& vectDesignationDef )
{
	for(unsigned int i = 0; i < vectDesignationDef.size()-1; i++ )
	{
		if( vectDesignationDef.size() <= 1 )
		{
			break;
		}

		std::stable_sort( vectDesignationDef.begin(), vectDesignationDef.end(), compareDesignations );
	}
}

int GameDesignation::GetDesignationMapSize()
{
	return (int) m_mapDesignationDef.size();
}

const char* GameDesignation::GetDesignationCateName( int idx )
{
	MAPDESIGNATIONDEF::iterator it = m_mapDesignationDef.begin();
	for( int i = 0; it != m_mapDesignationDef.end(); i++, it++ )
	{
		if( i == idx )
		{
			return it->first.c_str();
		}
	}
}

void GameDesignation::sortAllDesignation()
{
	MAPDESIGNATIONDEF::iterator iter = m_mapDesignationDef.begin();
	while( iter != m_mapDesignationDef.end() )
	{
		sortDesignation( iter->second );
		++iter;
	}
	sortDesignation( m_vectDesignationDef );
}

// 是否当天时间
bool GameDesignation::isToday( int nTime )
{
	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	time_t sert			= (time_t)sec;
	struct tm *serTM	=  localtime( &sert );
    int nSerTmDay = serTM->tm_mday;
    int nSerTmMon = serTM->tm_mon;
	time_t localt	= (time_t)( nTime );
	struct tm *localTM	=  localtime( &localt );

	if( nSerTmDay == localTM->tm_mday && nSerTmMon == localTM->tm_mon )
	{
		return true;
	}

	return false;
}

bool GameDesignation::compareDesignations( const tagDesignationDef* lhs, const tagDesignationDef* rhs )
{
	int nLhsHaveResult = GameDesignation::getSingleton().isHaveDesignation( lhs->ID ) ? 1 : 0;
	int nRhsHaveResult = GameDesignation::getSingleton().isHaveDesignation( rhs->ID ) ? 1 : 0;
	
	if ( nLhsHaveResult == nRhsHaveResult )
	{
		return lhs->ID < rhs->ID;
	}
	else
	{
		return nLhsHaveResult > nRhsHaveResult;
	}
}