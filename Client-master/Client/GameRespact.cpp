#include "GameRespact.h"
#include "GameDBData.h"
#include "OgreSingleton.h"
#include "GameManager.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"

GameRespact::GameRespact(void)
{
    INetworkManager::getSingleton().RegisterMsgHandler( RESPACT_SVR, this );
    DBDataManager::getSingleton().loadRespactphaseDef( m_vectRespactPhase );
    DBDataManager::getSingleton().loadRespactPartyDef( m_mapRespactParty );
}

GameRespact::~GameRespact(void)
{
}

int GameRespact::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
    if( msgid == RESPACT_SVR )
    {
        switch(	msgbody.RespactSvr.Operation )
        {
        case RESPACT_SVR_INFO:
            {
               m_vectRespactEntry.clear();
               int nRespactNum = msgbody.RespactSvr.RespactSvrData.RespactInfo.RespactPartyNum;
               m_vectRespactEntry.resize( nRespactNum );
               for( int i = 0; i < nRespactNum; i++ )
               {
                    m_vectRespactEntry[i] = msgbody.RespactSvr.RespactSvrData.RespactInfo.Respact[i];
               }
            }
            break;
        case RESPACT_SVR_CHG:
            {
                bool bFound = false;
                VECTRESPACTENTRY::iterator iter = m_vectRespactEntry.begin();
                for(; iter != m_vectRespactEntry.end(); iter++ )
                {
                    if( iter->ID == msgbody.RespactSvr.RespactSvrData.RespactChg.ID )
                    {
                        iter->Value = msgbody.RespactSvr.RespactSvrData.RespactChg.Value;
                        bFound = true;
                    }
                }

                if( !bFound )
                {
                    m_vectRespactEntry.push_back( msgbody.RespactSvr.RespactSvrData.RespactChg );
                }

                GameManager::getSingleton().SendUIEvent("GE_PESPACT_UPDATE");
            }
            break;
        }
    }
    return 0;
}

// 获取声望的类型名
int GameRespact::getRespactCategoryId( int nIndex )
{
    int nCount = 0;
    MAPRESPACTPARTY::iterator iter = m_mapRespactParty.begin();
    while( iter != m_mapRespactParty.end() )
    {
        if( nCount == nIndex )
        {
            return iter->first;
        }
        nCount++;
        iter++;
    }
	return nCount;
}

// 获取声望有类型名
const char* GameRespact::getRespactCategoryName( int nId )
{
    MAPRESPACTPARTY::iterator iter = m_mapRespactParty.begin();
    while( iter != m_mapRespactParty.end() )
    {
        for( size_t i = 0; i < iter->second.size(); i++ )
        {
            if( nId == (iter->second[i])->ID )
            {
                return (iter->second[i])->CategoryName;
            }
        }
        iter++;
    }
    return "";
}

// 获取声望值
int GameRespact::getRespactValue( int nId )
{
    VECTRESPACTENTRY::iterator iter = m_vectRespactEntry.begin();
    for(; iter != m_vectRespactEntry.end(); iter++ )
    {
        if( iter->ID == nId )
        {
            return iter->Value;
        }
    }

    return 0;
}

// 获取指定类别的阵营数量
int GameRespact::getRespactPartyNum(  int nId  )
{
    MAPRESPACTPARTY::iterator iter = m_mapRespactParty.find( nId );
    if( iter != m_mapRespactParty.end() )
    {
        return (int)iter->second.size();
    }
	return 0;
}

// 获取声望总的阵营数量
int GameRespact::getRespactPartyNum()
{
    int nCount = 0;
    MAPRESPACTPARTY::iterator iter = m_mapRespactParty.begin();
    while( iter != m_mapRespactParty.end() )
    {
        nCount += (int)iter->second.size();
        iter++;
    }  
    return nCount;
}

// 获取声望阶段数据
tagRespactPhaseDef_ToLua* GameRespact::getRespactPhaseDef( int nIndex )
{
    return (tagRespactPhaseDef_ToLua*)m_vectRespactPhase[nIndex];
}

// 获取声望信息
tagRespactPartyDef_ToLua* GameRespact::getRespactPartyDef( int nId )
{
    return ( tagRespactPartyDef_ToLua* )&DBDataManager::getSingleton().getRespactPartyDef( nId );
}

// 获取指定类别的阵营
tagRespactPartyDef_ToLua* GameRespact::getRespactPartyDef( int nId, int nIndex )
{
    MAPRESPACTPARTY::iterator iter = m_mapRespactParty.find( nId );
    if( iter != m_mapRespactParty.end() )
    {
        return (tagRespactPartyDef_ToLua*)iter->second[nIndex];
    }
	
    return NULL;
}

// 是否是已拥有的声望
bool GameRespact::isHaveRespact( int nId )
{
    VECTRESPACTENTRY::iterator iter = m_vectRespactEntry.begin();
    while( iter != m_vectRespactEntry.end() )
    {
        if( iter->ID == nId )
        {
            return true;
        }
        iter++;
    }
    return false;
}

void GameRespact::clearRespactEntry()
{
    m_vectRespactEntry.clear();
}