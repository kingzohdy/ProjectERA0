#include <algorithm>
#include "GameTrust.h"
#include "GameActor.h"
#include "GameActorManager.h"
#include "GamePlayManager.h"
#include "GameMainPlayer.h"
#include "GameQuest.h"

GameTrust::GameTrust(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( TRUST_SVR,  this );
}

GameTrust::~GameTrust(void)
{
}

// ��ί�йҵ���Ϣ���
void GameTrust::trust_OpenTrustFace()
{
	tagCSPkgBody msgbody;
	msgbody.TrustClt.TrustCltType = TRUST_OPEN_FACE;
	msgbody.TrustClt.TrustCltData.TrustOpen = 0;
	SendGameServerMsg( TRUST_CLT, msgbody );
}

// ��ȡ�ҵ���Ϣ
void GameTrust::trust_GetOrderInfo()
{
	tagCSPkgBody msgbody;
	msgbody.TrustClt.TrustCltType = TRUST_ROLE_ORDER;
	msgbody.TrustClt.TrustCltData.TrustRoleList.RoleNum = (unsigned char)m_GetTrustRoleList.size();
	int nNum = 0;
	for( std::list<tagLong_ToLua>::iterator iter = m_GetTrustRoleList.begin(); iter != m_GetTrustRoleList.end(); ++iter )
	{ 
		msgbody.TrustClt.TrustCltData.TrustRoleList.RoleID[nNum] = *((tdr_ulonglong*)&(*iter));
		++nNum;
	}
	SendGameServerMsg( TRUST_CLT, msgbody );
	//m_GetTrustRoleList.clear();
}

// ���������Ʒ
void GameTrust::addTrustItem( int nIndex, int nItemId, int nNum )
{
	assert( nIndex < MAX_TASK_ITEM );
	m_TrustItem[nIndex].m_uItemID	= nItemId;
	m_TrustItem[nIndex].m_nNum		= nNum;
}

// ɾ��ί��������Ʒ
void GameTrust::delTrustItem()
{
	memset( &m_TrustItem, 0, sizeof( tagTrustTaskItem_ToLua )*MAX_TASK_ITEM );
}
// ����ί����Ʒ
void GameTrust::trust_Contribute(  int nHeightId, int nLowId, int nTaskId )
{
	tagCSPkgBody msgbody;
	msgbody.TrustClt.TrustCltType = TRUST_CONTRIBUTE;
	tagLong_ToLua ID;
	ID.nHeight	= nHeightId;
	ID.nLow		= nLowId;
	msgbody.TrustClt.TrustCltData.TrustContribute.RoleID = *((tdr_ulonglong*)&ID);
	msgbody.TrustClt.TrustCltData.TrustContribute.TaskID = nTaskId;
	
	int nNum = 0;
	for( int i = 0; i < MAX_TASK_ITEM; i++ )
	{
		if( m_TrustItem[i].m_uItemID != 0 )
		{
			msgbody.TrustClt.TrustCltData.TrustContribute.TrustItem[nNum].ItemID = m_TrustItem[i].m_uItemID;
			msgbody.TrustClt.TrustCltData.TrustContribute.TrustItem[nNum].Num	= m_TrustItem[i].m_nNum;
			nNum++;
		}
	}
	msgbody.TrustClt.TrustCltData.TrustContribute.ItemNum = nNum;
	SendGameServerMsg( TRUST_CLT, msgbody );
}

// �����ҵ�
void GameTrust::trust_Order( int nTrustID )
{
	if ( isInMainTrust( nTrustID ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRUST_ALREADY_ORDER );
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.TrustClt.TrustCltType = TRUST_ORDER;
	msgbody.TrustClt.TrustCltData.OrderTrustID = nTrustID;
	SendGameServerMsg( TRUST_CLT, msgbody );
}

// �����ҵ�
void GameTrust::trust_CancelOrder( int nTrustID )
{
	if ( !isInMainTrust( nTrustID ) )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.TrustClt.TrustCltType = TRUST_CANCEL_ORDER;
	msgbody.TrustClt.TrustCltData.CancelTrustID = nTrustID;
	SendGameServerMsg( TRUST_CLT, msgbody );
}

// ί���Ƿ����
bool GameTrust::trust_TrustIsComplete( int nTrustID )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	int nTrustNeedItem	= GameQuest::getSingleton().getQuestNeedItemNum( nTrustID );
	for ( int i = 0; i < nTrustNeedItem; i++ )
	{
		int nTrustNeedItemId	= GameQuest::getSingleton().getQuestNeedItem(  nTrustID, i );
		int nTrustQuestNum		= GameQuest::getSingleton().getQuestNeedItemCount( nTrustID, i );
		int nItemCount			= pMainPlayer->getContainer().getItemCount( nTrustNeedItemId );
		if ( nItemCount < nTrustQuestNum || pMainPlayer->getLv() < GameQuest::getSingleton().getQuestLvLimit( nTrustID ) )
		{
			return false;
		}
	}
	return true;
}

// ��ȡ�����ҵ��ĺ�������
int GameTrust::getTrustRoleNum()
{
	return (int)m_FriendTrustMap.size();
}

// ��ȡ��ҹҵ���Ϣ�б�
const tagFriendTrust* GameTrust::getFriendTrustList( int nRow )const
{
	int nNum = 0 ;
	for( std::map<tdr_ulonglong, tagFriendTrust>::const_iterator iter = m_FriendTrustMap.begin();
			iter != m_FriendTrustMap.end(); ++iter )
	{
		if( nNum == nRow )
		{
			return &(iter->second);
		}
		nNum++;
	}
	return NULL;

}

// ��ȡ���ǹҵ���Ϣ�б�
const tagFriendTrust* GameTrust::getMainPlayerTrustList()const
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tdr_ulonglong uLongId = pMainPlayer->getRoleWId();

	std::map<tdr_ulonglong, tagFriendTrust>::const_iterator iter = m_FriendTrustMap.find( uLongId );
	if ( iter != m_FriendTrustMap.end() )
	{
		return &(iter->second);
	}
	return NULL;

}

bool GameTrust::isInMainTrust( int	nTaskID )
{
	const tagFriendTrust* pMainTrust = getMainPlayerTrustList();
	if ( pMainTrust == NULL )
	{
		return false;
	}

	for ( int i = 0; i < pMainTrust->m_nTrustNum; ++i )
	{
		if ( pMainTrust->m_TrustTaskData[i].m_uTaskID == nTaskID )
		{
			return true;
		}
	}

	return false;
}

bool GameTrust::isInTrust( tdr_ulonglong roleWID )
{
	return	m_FriendTrustMap.find( roleWID ) != m_FriendTrustMap.end();
}

// ��պ��ѹҵ���Ϣ
void GameTrust::clearFriendTrust()
{
	m_FriendTrustMap.clear();
	m_GetTrustRoleList.clear();
}

int GameTrust::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	typedef std::map<tdr_ulonglong, tagFriendTrust>::iterator	FriendTrustIter;
	typedef std::list<tagLong_ToLua>::iterator					RoleListIter;

	if( msgid == TRUST_SVR )
	{
		const CSTRUSTSVRDATA& trustData = msgbody.TrustSvr.TrustSvrData;
		switch( msgbody.TrustSvr.TrustSvrType )
		{
		case TRUST_VER_LIST:
			{
				int nRoleNum = trustData.TrustVerList.RoleNum;
				for( int i = 0; i < nRoleNum; i++ )
				{
					// ֻ������µĹҵ����ID 
					if( !isInTrust( trustData.TrustVerList.RoleVersionList[i].RoleID ) )
					{
						// ��Ϊ�¹�ί�еĺ��ѣ��������ί���б�

						m_GetTrustRoleList.push_back( *(tagLong_ToLua*)&trustData.TrustVerList.RoleVersionList[i].RoleID );
					}
				}

				// �ȶԿͻ��˵����ݺͷ�����������������,���ֶ���һ��
				for ( RoleListIter listIter = m_GetTrustRoleList.begin(); listIter != m_GetTrustRoleList.end(); )
				{
					bool bFound = false;
					tdr_ulonglong uClientRoleId = *( (tdr_ulonglong*)&(*listIter) );

					for ( int i = 0; i < nRoleNum; i++ )
					{
						tdr_ulonglong uRoleId = trustData.TrustVerList.RoleVersionList[i].RoleID;	
						if ( uRoleId == uClientRoleId )
						{
							bFound = true;
							break;
						}
					}

					if ( !bFound )
					{
						FriendTrustIter iter = m_FriendTrustMap.find( uClientRoleId );
						// ɾ��ί����Ϣ����
						if ( iter != m_FriendTrustMap.end() )
						{
							m_FriendTrustMap.erase( iter );
						}

						listIter = m_GetTrustRoleList.erase( listIter );
						continue;
					}

					++listIter;
				}

				GameManager::getSingleton().SendUIEvent( "GE_TRUST_UPDATETRUSTLIST" );
			}
			break;

		case TRUST_ORDER_LIST:
			{
				int nRoleNum = trustData.TrustOrderList.RoleNum;
				for( int i = 0; i < nRoleNum; i++ )
				{
					tagFriendTrust	trust;
					tdr_ulonglong	uRoleId	=	trustData.TrustOrderList.RoleTrust[i].RoleID;
					int nVersion			=	trustData.TrustOrderList.RoleTrust[i].RoleTrustVersion;
					if ( nVersion == -1 )
					{	
						FriendTrustIter iter = m_FriendTrustMap.find( uRoleId );
						if ( iter != m_FriendTrustMap.end() )
						{
							m_FriendTrustMap.erase( iter );
						}
						
						for ( RoleListIter roleIter = m_GetTrustRoleList.begin(), end = m_GetTrustRoleList.end();
								roleIter != end; ++roleIter )
						{
							if ( ((tagLong_ToLua*)&uRoleId)->nHeight == roleIter->nHeight && 
								((tagLong_ToLua*)&uRoleId)->nLow == roleIter->nLow )
							{
								m_GetTrustRoleList.erase( roleIter );
								break;
							}
						}

						continue;
					}

					trust.m_RoleId.nHeight		= ((tagLong_ToLua*)&uRoleId)->nHeight;
					trust.m_RoleId.nLow			= ((tagLong_ToLua*)&uRoleId)->nLow;
					trust.m_nVerson				= trustData.TrustOrderList.RoleTrust[i].RoleTrustVersion;
					trust.m_nTrustNum			= trustData.TrustOrderList.RoleTrust[i].TrustNum;
			
					for( int j = 0; j < trust.m_nTrustNum; j++ )
					{
						const TRUSTTASK& trustTask = trustData.TrustOrderList.RoleTrust[i].TrustTaskData[j];
						trust.m_TrustTaskData[j].m_uTaskID		= trustTask.TaskID;
						trust.m_TrustTaskData[j].m_uItemNum		= trustTask.ItemNum;
						for( int k = 0; k < trust.m_TrustTaskData[j].m_uItemNum; k++ )
						{
							trust.m_TrustTaskData[j].m_TrustItem[k].m_uItemID	= trustTask.TrustItem[k].ItemID;
							trust.m_TrustTaskData[j].m_TrustItem[k].m_nNum		= trustTask.TrustItem[k].Num;
						}
					}
					m_FriendTrustMap[uRoleId] = trust;
				}
				GameManager::getSingleton().SendUIEvent( "GE_TRUST_UPDATEORDERLIST" );
			}
			break;
		case TRUST_CONTRIBUTE_MSG:
			{
				char szBuf[256];
				// ����( ������ʷ��¼ )
				sprintf( szBuf, "trustContributeMsg(\"%s\")", trustData.TrustContributeMsg );
				GameManager::getSingleton().getScriptVM()->callString( szBuf );
			}
			break;
		}
	}
	return 0;
}
