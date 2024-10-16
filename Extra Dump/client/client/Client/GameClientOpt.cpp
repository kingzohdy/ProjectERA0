#include "GameClientOpt.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "GameShortCut.h"
const short OPTION_VERSION = 1002;

GameClientOpt::GameClientOpt(void)
{
	m_bNeedSend = false;
	m_nLastSend = 0;
}

GameClientOpt::~GameClientOpt(void)
{
}

// ���ñ������ݵ��Ⱥ�˳��
void GameClientOpt::setLevelSize( int nLevel, int nSize )
{
	m_mapLevel[nLevel] = nSize;
}

// ���ñ��浽�������ϵ���Ϣ
void GameClientOpt::setClientOpt( int nLevel, char* szData, bool bDelaySave /*= true*/ )
{
	m_OptLen = MAX_OPT_LEN;
	memcpy( m_OptContent, &OPTION_VERSION, sizeof( OPTION_VERSION ) );
	int nSize = 0;
	for( std::map<int,int>::iterator iter = m_mapLevel.begin(); iter != m_mapLevel.end(); ++iter )
	{
		if( iter->first == nLevel )
		{
			assert( ( iter->second + nSize + sizeof( OPTION_VERSION ) ) <= MAX_OPT_LEN );
			memcpy( m_OptContent + sizeof( OPTION_VERSION ) + nSize, szData, iter->second );
		}
		nSize = iter->second + nSize;
	}
	if(bDelaySave )
	{
		m_bNeedSend = true;
	}else{
		tagCSPkgBody msgbody;
		msgbody.ClientOpt.OptLen = m_OptLen;
		memcpy( msgbody.ClientOpt.OptContent, &m_OptContent, MAX_OPT_LEN );
		SendGameServerMsg( CLT_OPT, msgbody );
		m_bNeedSend = false;
		m_nLastSend = timeGetTime();
	}
}

// ��ȡ���浽�������ϵ�������Ϣ
bool GameClientOpt::loadClientOpt( int nLevel, int nVersion, char* szOutData )
{
	if( nVersion == OPTION_VERSION )
	{
		int nSize = 0;
		for( std::map<int,int>::iterator iter = m_mapLevel.begin(); iter != m_mapLevel.end(); ++iter )
		{	
			if( iter->first == nLevel )
			{
				memcpy( szOutData, m_OptContent + sizeof( OPTION_VERSION ) + nSize, iter->second );
			}
			nSize = iter->second + nSize;
		}
		return true;
	}
	return false;
}

// ����������Ϣ
void GameClientOpt::setOptContent( const char* szData )
{
	memcpy( m_OptContent, szData, MAX_OPT_LEN );
}

void GameClientOpt::update( unsigned int tick )
{
	unsigned int curTick = timeGetTime();
	if( m_bNeedSend && curTick - m_nLastSend >= 5*1000 )
	{
		tagCSPkgBody msgbody;
		msgbody.ClientOpt.OptLen = m_OptLen;
		memcpy( msgbody.ClientOpt.OptContent, &m_OptContent, MAX_OPT_LEN );
		SendGameServerMsg( CLT_OPT, msgbody );
		m_bNeedSend = false;
		m_nLastSend = curTick;
	}
}