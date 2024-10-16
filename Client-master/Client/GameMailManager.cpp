#include "GameMailManager.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "Item.h"
#include "Container.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"

GameMailManager::GameMailManager(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( MAIL_SVR,  this );
	memset( &m_SendData, 0, sizeof( tagMailSendInfo ) );
}

GameMailManager::~GameMailManager(void)
{
}

// 阅读邮件
void GameMailManager::mail_Read( int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_READ;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	msgbody.MailClt.MailCltData.MailWIDForRead = *((tdr_ulonglong*)&ID);
	SendGameServerMsg( MAIL_CLT, msgbody );
}

// 删除某封邮件
void GameMailManager::mail_Delete( int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_DELETE;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;
	msgbody.MailClt.MailCltData.MailWIDForDelete = *((tdr_ulonglong*)&ID);
	SendGameServerMsg( MAIL_CLT, msgbody );
}

//删除角色全部邮件
void GameMailManager::mail_Delete_All(void)
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_ALL_DELETE;
	SendGameServerMsg( MAIL_CLT, msgbody );
}

// 发送邮件
void GameMailManager::mail_Send( const char* szToName, const char* szTitle, const char* szText, int nListType, int nGridIndex )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_SEND;
	msgbody.MailClt.MailCltData.MailCltSend.Gold		= m_SendData.m_nGold;
	
	strncpy( msgbody.MailClt.MailCltData.MailCltSend.To, szToName, strlen(szToName) + 1 );
	strncpy( msgbody.MailClt.MailCltData.MailCltSend.Title, szTitle, strlen(szTitle) + 1 );
	strncpy( msgbody.MailClt.MailCltData.MailCltSend.Text, szText, strlen(szText) + 1 );
    msgbody.MailClt.MailCltData.MailCltSend.ItemNum	 = 0;
	msgbody.MailClt.MailCltData.MailCltSend.Cost.GridIdx = 0;
	msgbody.MailClt.MailCltData.MailCltSend.Cost.ListType = 0;
	msgbody.MailClt.MailCltData.MailCltSend.Cost.WID = 0;
	if( nListType != 0 && nGridIndex >= 0 )
	{
        msgbody.MailClt.MailCltData.MailCltSend.ItemNum		= ( m_SendData.m_nItemNum > MAIL_MAX_ITEM )?MAIL_MAX_ITEM:m_SendData.m_nItemNum;
		int nNum = 0;
		for( int i = 0; i < MAIL_MAX_ITEM; i++ )
		{
			if( m_SendData.m_Items[i].getItemId() != 0 )
			{
			msgbody.MailClt.MailCltData.MailCltSend.Items[nNum].GridIdx	 = m_SendData.m_Items[i].getGridIdx();
			msgbody.MailClt.MailCltData.MailCltSend.Items[nNum].GridNum	 = m_SendData.m_Items[i].getNum();	
			msgbody.MailClt.MailCltData.MailCltSend.Items[nNum].ListType = m_SendData.m_Items[i].getListType();
			msgbody.MailClt.MailCltData.MailCltSend.Items[nNum].WID		 = m_SendData.m_Items[i].getItemWId();
			nNum++;
			}
		}
		if( m_SendData.m_nItemNum > 0 )
		{
			Item& cost_Item =	pMainPlayer->getContainer().getItem( nListType, nGridIndex );
			msgbody.MailClt.MailCltData.MailCltSend.Cost.GridIdx = nGridIndex;
			msgbody.MailClt.MailCltData.MailCltSend.Cost.ListType = nListType;
			msgbody.MailClt.MailCltData.MailCltSend.Cost.WID = cost_Item.getItemWId();
		}
	}

	SendGameServerMsg( MAIL_CLT, msgbody );
}

// 获取邮件某一格的所有物品
void GameMailManager::mail_GetGoods( int nHeight, int nLow, int nIndex )
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_GET;
	tagLong_ToLua ID;
	ID.nHeight	= nHeight;
	ID.nLow		= nLow;

	msgbody.MailClt.MailCltData.MailGettingList.WID		= *((tdr_ulonglong*)&ID);
	msgbody.MailClt.MailCltData.MailGettingList.How		= MAIL_GET_ITEM;
	
	for( std::vector<tagMailData>::iterator iter = m_MailData.begin(); iter != m_MailData.end(); ++iter )
	{
		if( *((tdr_ulonglong*)&iter->m_MailHead.m_WID) == *((tdr_ulonglong*)&ID) )
		{
			msgbody.MailClt.MailCltData.MailGettingList.ItemWID = iter->m_MailDetail.m_Items[nIndex].getItemWId();
			break;
		}
	}

	SendGameServerMsg( MAIL_CLT, msgbody );
}

//获取全部的Money
void GameMailManager::mail_GetMoney( int nHeight, int nLow )
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_GET;
	tagLong_ToLua ID;
	ID.nHeight	 = nHeight;
	ID.nLow		 = nLow;
	msgbody.MailClt.MailCltData.MailGettingList.WID = *((tdr_ulonglong*)&ID);
	msgbody.MailClt.MailCltData.MailGettingList.How = MAIL_GET_MONEY;
	SendGameServerMsg( MAIL_CLT, msgbody );
}

//获取全部的money和物品
void GameMailManager::mail_GetAll( int nHeight, int nLow,int nDel )
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_GET;
	tagLong_ToLua ID;
	ID.nHeight	 = nHeight;
	ID.nLow		 = nLow;
	msgbody.MailClt.MailCltData.MailGettingList.WID		= *((tdr_ulonglong*)&ID);
	msgbody.MailClt.MailCltData.MailGettingList.How		= MAIL_GET_ALL;
	msgbody.MailClt.MailCltData.MailGettingList.NoDel	= nDel;
	SendGameServerMsg( MAIL_CLT, msgbody );
}

//获取邮箱里的所有邮件的金钱和物品
void GameMailManager::mail_Get_All(void)
{
	tagCSPkgBody msgbody;
	msgbody.MailClt.Operation = MAIL_CLT_ALL_GET;
	SendGameServerMsg( MAIL_CLT, msgbody );
}

// 重置邮件数据
void  GameMailManager::resetMailData()
{
	m_MailData.clear();
	m_MailRead.clear();
	memset( &m_PageMail, 0, sizeof( tagPageMail ) );
	memset( &m_SendData, 0, sizeof( tagMailSendInfo ) );
}

// 设置发送邮件内容
void GameMailManager::addSendItem( const Item& item, int nIndex, int nNum )
{
	bool bInSendItemBox = false;
	Item ItemData = item;

	m_SendData.m_Items[nIndex] = item;
	m_SendData.m_Items[nIndex].setNum( nNum );
	m_SendData.m_nItemNum++;
	
}

void GameMailManager::delSendItem( int nIndex )
{
	m_SendData.m_Items[nIndex].releaseItem();
	m_SendData.m_nItemNum--;
}

void GameMailManager::addSendGold( int nGold )
{
	m_SendData.m_nGold = nGold;
}

// 获得即将发送的物品信息
const tagMailSendInfo& GameMailManager::getSendData()const
{
	return m_SendData;
}

// 清空发送物品
void GameMailManager::clearSendItem()
{
	memset( &m_SendData, 0, sizeof( tagMailSendInfo ) );
}

// 获取一页的邮件头数据
const tagPageMail* GameMailManager::getOnePageMail( int nPage )
{
	memset( &m_PageMail, 0, sizeof( tagPageMail ) );
	int nStartIndex				= MAIL_MAX_HEAD_PAGE_NUM*nPage ;
	m_PageMail.m_nPageId		= nPage;
	m_PageMail.m_nMailNum		= 0;
	m_PageMail.m_nTotalHeadNum	= (int)m_MailData.size();
	for( int i = nStartIndex; i < nStartIndex + MAIL_MAX_HEAD_PAGE_NUM; i++ )
	{
		if( i >= (int)m_MailData.size() )
		{
			break;
		}
		m_PageMail.m_nMailNum++;
		m_PageMail.m_MainData[i - nStartIndex].m_MailHead		= *( ( tagMailHead_ToLua* )&m_MailData[i].m_MailHead );
		m_PageMail.m_MainData[i - nStartIndex].m_MailDetail		= *( ( tagMailDetail_ToLua* )&m_MailData[i].m_MailDetail );
	}

	return (tagPageMail*)&m_PageMail;
}

int GameMailManager::checkMailSend(unsigned int nMoney )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL ) 
	{
		return 0;
	}
	if( pMainPlayer->getMoney() + pMainPlayer->getBull() < nMoney )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_MAIL_NO_MONEY );
		return 0;
	}
	return 1;
}

void GameMailManager::changeMailSendUserName( const char* pszOldName, const char* pszNewName )
{
	for ( std::vector<tagMailData>::iterator iter = m_MailData.begin(), end = m_MailData.end(); iter != end; ++iter )
	{
		tagMailData& oneMailData = *iter;
		if ( strncmp( oneMailData.m_MailHead.m_szSend, pszOldName, ROLE_NAME_LEN_SAVE ) == 0 )
		{
			strncpy( oneMailData.m_MailHead.m_szSend, pszNewName, ROLE_NAME_LEN_SAVE );
			GameManager::getSingleton().SendUIEvent( "GE_CHG_MAIL_SENDER_NAME" );
		}
	}
}

int GameMailManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if( msgid == MAIL_SVR )
	{
		const CSMAILSVRDATA& mailData = msgbody.MailSvr.MailSvrData;
		switch( msgbody.MailSvr.Operation )
		{
		// 客户端收到的邮件头信息
		case MAIL_SVR_HEAD_SNAPSHOT:
			{
				int mail_num			= 0;
				int mailnum				= mailData.MailHeadSnapshot.Num;
				int n_UserMailReadNum	= 0;
				int n_SysMailReadNum	= 0;
				int n_UserNoneReadNum	= 0;
				int n_SysNoneReadNum	= 0;
				int n_UserMailFlag		= 0;
				int n_SysMailFlag		= 0;
				for( int i = 0; i < mailnum; i++ )
				{	
					tagMailData m_Mail_Data;
					m_Mail_Data.m_MailHead = *( ( tagMailHead_ToLua* )&mailData.MailHeadSnapshot.Heads[i] );
					m_MailData.insert( m_MailData.begin(),m_Mail_Data );
					switch( m_Mail_Data.m_MailHead.m_nFlags & 3 )
					{
						case 0:
							n_UserNoneReadNum++;
							break;
						case 1:
							n_SysNoneReadNum++;
							break;
						case 2:
							n_UserMailReadNum++;
							break;
						default:
							n_SysMailReadNum++;
							break;
					}		
				}
				if( n_UserNoneReadNum + n_UserMailReadNum >= MAIL_MAX_NORMAL_NUM )
				{
					n_UserMailFlag = 1;
				}

				if( mailnum >= MAIL_MAX_NUM )
				{
					n_SysMailFlag = 1;
				}
				
				GameManager::getSingleton().getScriptVM()->callString( "UpdataMailPage(0)" );
				char szFuncNameBuf[256];
				if ( n_UserNoneReadNum + n_SysNoneReadNum > 0 )
				{
						sprintf( szFuncNameBuf, "NewMails_Tips(%d, %d, %d)", n_UserNoneReadNum + n_SysNoneReadNum, n_UserMailFlag, n_SysMailFlag);
				}else{

						sprintf( szFuncNameBuf, "NewMails_Tips(0, %d, %d)", n_UserMailFlag, n_SysMailFlag);
				}
				GameManager::getSingleton().getScriptVM()->callString( szFuncNameBuf );
			}
			break;
		// 邮件信息变更消息
		case MAIL_SVR_HEAD_CHG:
			{
				switch( mailData.MailChgData.What )
				{
				// 增加邮件
				case MAIL_CHG_ADD:
					{
						tagMailData m_Mail_Data;
						if( m_MailData.size() < MAIL_MAX_HEAD_NUM )
						{
							m_Mail_Data.m_MailHead = *( (tagMailHead_ToLua*)&mailData.MailChgData.ChgInfo.AddedHead );
							 std::vector<tagMailData>::iterator iter_new =  m_MailData.begin();
							 m_MailData.insert(iter_new,m_Mail_Data);
						}
						GameManager::getSingleton().SendUIEvent( "GE_MAIL_NEWMAIL" );
						if( ( m_Mail_Data.m_MailHead.m_nFlags & 1 ) == 0 )
						{
							int nTotalNum = 0;
							for( std::vector<tagMailData>::iterator iter = m_MailData.begin(); iter != m_MailData.end(); ++iter)
							{
								if( ( iter->m_MailHead.m_nFlags & 1 ) == 0)
								{
									nTotalNum++;
								}
							}
							if( nTotalNum >= MAIL_MAX_NORMAL_NUM )
							{
								GameManager::getSingleton().SendUIEvent( "GE_USER_MAIL_FULL" );
							}
						}
						if( m_MailData.size() >= MAIL_MAX_HEAD_NUM )
						{
							GameManager::getSingleton().SendUIEvent( "GE_SYS_MAIL_FULL" );
						}
					}
					break;
				//删除邮件
				case MAIL_CHG_DEL:
					{
						for( int j = 0; j < mailData.MailChgData.ChgInfo.DeletedWIDs.Num; j++ )
						{
							for( std::vector<tagMailData>::iterator iter = m_MailData.begin(); iter != m_MailData.end(); ++iter )
							{
								if( *( (tdr_ulonglong*)&iter->m_MailHead.m_WID ) == mailData.MailChgData.ChgInfo.DeletedWIDs.WIDs[j] )
								{
									char szBuf[256];
									sprintf( szBuf, "DeleteEffect(%d,%d,%d)", iter->m_MailHead.m_WID.nHeight, iter->m_MailHead.m_WID.nLow,iter->m_MailHead.m_nFlags );
									GameManager::getSingleton().getScriptVM()->callString( szBuf );
									m_MailData.erase( iter );
									break;
								}
							}
							std::vector<tdr_ulonglong>::iterator WidIt = std::find( m_MailRead.begin(),m_MailRead.end(),mailData.MailChgData.ChgInfo.DeletedWIDs.WIDs[j] );
							if( WidIt != m_MailRead.end() )
							{
								m_MailRead.erase(WidIt);
							}
						}
					}
					break;
				case MAIL_CHG_MOD:
					{
						for( std::vector<tagMailData>::iterator iter = m_MailData.begin(); iter != m_MailData.end(); ++iter )
						{
							if( *( (tdr_ulonglong*)&iter->m_MailHead.m_WID ) == mailData.MailChgData.ChgInfo.ModHead.WID )
							{
								iter->m_MailHead.m_nFlags = mailData.MailChgData.ChgInfo.ModHead.Flags;
								break;
							}
						}
						GameManager::getSingleton().SendUIEvent( "GE_MAIL_MOD_CHG" );
					}
					break;
				}
			}
			break;
		//服务器返回的详细的邮件信息
		case MAIL_SVR_CONTENT:
			{
				for( std::vector<tagMailData>::iterator iter = m_MailData.begin(); iter != m_MailData.end(); ++iter )
				{
					if( *(tdr_ulonglong*)&iter->m_MailHead.m_WID != mailData.MailContent.Head.WID )
					{
						continue;
					}
					// 邮件头
					//iter->m_MailHead = *( (tagMailHead_ToLua*)&mailData.MailContent.Head );
					iter->m_MailHead.m_nFlags = mailData.MailContent.Head.Flags;
					// 邮件主体
					iter->m_MailDetail.m_uGold = mailData.MailContent.Detail.Gold;
					int nItemNum = (*iter).m_MailDetail.m_ItemNum = mailData.MailContent.Detail.ItemNum;
					for( int i = 0; i < nItemNum; i++ )
					{
						if( mailData.MailContent.Detail.Items[i].Type == GRID_ITEM_ARM )
						{
							iter->m_MailDetail.m_Items[i] = mailData.MailContent.Detail.Items[i].GridData.RoleArm;
						}else{
							iter->m_MailDetail.m_Items[i] = mailData.MailContent.Detail.Items[i].GridData.RoleItem;
						}
					}
					strncpy( iter->m_MailDetail.m_szText, mailData.MailContent.Detail.Text, strlen( mailData.MailContent.Detail.Text ) + 1 );
					GameManager::getSingleton().SendUIEvent( "GE_MAIL_CONTENT" );
					std::vector<tdr_ulonglong>::iterator WidIt = std::find( m_MailRead.begin(),m_MailRead.end(),mailData.MailContent.Head.WID );
					if( WidIt == m_MailRead.end() )
					{
						m_MailRead.push_back( mailData.MailContent.Head.WID );
					}
				}
			}
			break;
		//邮件货物变化消息
		case MAIL_SVR_GOODS_CHG:
			{
				int Mail_Num = 0;
				for( std::vector<tagMailData>::iterator iter = m_MailData.begin(); iter != m_MailData.end(); ++iter, ++Mail_Num )
				{
					if( *( (tdr_ulonglong*)&iter->m_MailHead.m_WID ) != mailData.MailGoodsChgData.WID )
					{
						continue;
					}
					std::vector<tdr_ulonglong>::iterator WidIt = std::find( m_MailRead.begin(),m_MailRead.end(),mailData.MailGoodsChgData.WID );
					if( WidIt == m_MailRead.end() )
					{
						continue;
					}
					char szBuf[256];
					sprintf( szBuf, "GetAllGoods(%d,%d)", iter->m_MailHead.m_WID.nHeight, iter->m_MailHead.m_WID.nLow );
					GameManager::getSingleton().getScriptVM()->callString( szBuf );
					iter->m_MailDetail.m_uGold		-= mailData.MailGoodsChgData.Money;
					int item_num					= (*iter).m_MailDetail.m_ItemNum;
					int dispear_item_num			= mailData.MailGoodsChgData.Num;
					for( int j = 0; j< dispear_item_num; j++ )
					{
						for( int i = 0;i < (*iter).m_MailDetail.m_ItemNum; i++ )
						{
							tdr_ulonglong itemID = ( *iter ).m_MailDetail.m_Items[i].getItemWId();
							if( itemID != mailData.MailGoodsChgData.ItemWIDs[j] ) 
							{
								continue;
							}
							iter->m_MailDetail.m_Items[i] = iter->m_MailDetail.m_Items[(*iter).m_MailDetail.m_ItemNum - 1] ;
							memset( &( iter->m_MailDetail.m_Items[(*iter).m_MailDetail.m_ItemNum - 1] ), 0, sizeof( Item ) );
							(*iter).m_MailDetail.m_ItemNum--;
						}
					}		
				}

				GameManager::getSingleton().SendUIEvent( "GE_MAIL_CONTENT" );
			}
			break;
		case MAIL_SVR_SEND_RES:
			{
				GameManager::getSingleton().getScriptVM()->callString( "MailSendReply()" );
				if ( mailData.MailSendRes.Succeeded != 0 )
				{
					char szTmp[256];
					sprintf( szTmp, "ClearSendData( %d )", mailData.MailSendRes.Succeeded );
					GameManager::getSingleton().getScriptVM()->callString( szTmp );
					if( mailData.MailSendRes.Succeeded == 2 )
                    {
						clearSendItem();
					}
				}	
			}
			break;
		}
	}
	return 0;
}