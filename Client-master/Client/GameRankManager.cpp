#include "GameRankManager.h"
#include "NetMessage.h"
#include "GameManager.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameClanManager.h"

GameRankManager::GameRankManager()
{
	INetworkManager::getSingleton().RegisterMsgHandler(RANGE_SVR,this);
	m_myRankTip.CurrRange = -1;
	m_myBestRankTip.MainType = -1;
	m_myBestRankTip.SubType = -1;
}
GameRankManager::~GameRankManager()
{
	clearInfo();
}

void GameRankManager::clearInfo()
{
	m_myRankTip.CurrRange = -1;
	m_myRankTip.LastRange = -1;
	m_MyRankInfoMap.clear();
	m_pageInfoMap.clear();
}

int GameRankManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == RANGE_SVR )
	{
		msgbody.RangeSvr.RangeSvrData.RangeSvrPage.EntryNum;
		switch( msgbody.RangeSvr.RangeSvrOp )
		{
		case RANGE_SVR_TIP:
			{
				const CSRANGESVRTIP& tip = msgbody.RangeSvr.RangeSvrData.RangeSvrTip;
				
				m_myRankTip = tip;
				GameManager::getSingleton().SendUIEvent("RANK_TIP_SHOW");

				//修改主角的排名
				pageKey key(0,tip.MainType,tip.SubType);
				m_MyRankInfoMap[key] = tip;
				GameManager::getSingleton().SendUIEvent("MY_RANK_INFO_CHANGE");
			}
			break;
		case RANGE_SVR_PAGE:
			{
				const CSRANGESVRPAGE& pageData = msgbody.RangeSvr.RangeSvrData.RangeSvrPage;
				pageKey key( pageData.PageIdx,pageData.MainType,pageData.SubType );
				PAGEINFOMAPIT it = m_pageInfoMap.find(key);
				if( it != m_pageInfoMap.end() && it->second.PageVersion == pageData.PageVersion )
				{
					it->second.TotalNum = pageData.TotalNum;
				}else{
					m_pageInfoMap[key] = pageData;
				}
				
				GameManager::getSingleton().SendUIEvent("RANK_NEW_PAGE_DATA");
			}
			break;
		case RANGE_SVR_FIND:
			{
				const CSRANGESVRFIND& findData = msgbody.RangeSvr.RangeSvrData.RangeSvrFind;
				//查询
				if( findData.CltFind.Flag == 0 )
				{
					if( findData.EntryNum == 0 )
					{
						GameManager::getSingleton().SendUIEvent("RANK_SEARCH_NO_DATA");
					}else{
						int pageNum = (findData.EntryNum - 1)/RANGE_MAX_PAGE_ENTRY + 1;
						for( int i = 1 ; i < pageNum + 1; i ++ )
						{
							pageKey key( i,findData.CltFind.MainType,findData.CltFind.SubType,findData.CltFind.Name );
							CSRANGESVRPAGE pageInfo;
							pageInfo.PageVersion = 0;
							if( i < pageNum )
							{
								pageInfo.EntryNum = RANGE_MAX_PAGE_ENTRY ;
							}else{
								pageInfo.EntryNum = findData.EntryNum - RANGE_MAX_PAGE_ENTRY*(pageNum - 1);
							}
							pageInfo.TotalNum = findData.EntryNum;
							pageInfo.MainType = findData.CltFind.MainType;
							pageInfo.SubType = findData.CltFind.SubType ;
							int nIndex = (i-1)*RANGE_MAX_PAGE_ENTRY;
// 							if( nIndex > 0 )
// 							{
// 								nIndex = nIndex - 1;
// 							}
							memcpy( pageInfo.Entrys,findData.Entrys + nIndex,pageInfo.EntryNum*sizeof(TOPRANGE) );
							m_pageInfoMap[key] = pageInfo ;
						}
						GameManager::getSingleton().SendUIEvent("RANK_NEW_PAGE_DATA");
					}
				}else{
					//同步主角数据
					pageKey key( 0,findData.CltFind.MainType,findData.CltFind.SubType );
					CSRANGESVRTIP info;
					info.CurrRange = -1;
					if( findData.EntryNum > 0 )
					{
						for( int i = 0 ; i < findData.EntryNum ; i ++ )
						{
							if( info.CurrRange < 0 || findData.Entrys[i].TopRange < info.CurrRange )
							{
								info.CurrRange = findData.Entrys[i].TopRange;
								info.LastRange = findData.Entrys[i].RangeData.LastRange;
								info.MainType = findData.CltFind.MainType;
								info.SubType = findData.CltFind.SubType;
							}
						}
					}else{
						info.CurrRange = -1;
						info.LastRange = -1;
						info.MainType = findData.CltFind.MainType;
						info.SubType = findData.CltFind.SubType;
					}
					m_MyRankInfoMap[key] = info;
					GameManager::getSingleton().SendUIEvent("MY_RANK_INFO_CHANGE");
				}
				
			}
		default:
			break;
		}
	}
	return 0;
}

void GameRankManager::requestRankPageInfo(int mainType,int subType,int pageIdx,int pageVer)
{
	tagCSPkgBody msgbody;
	msgbody.RangeClt.RangeCltOp = RANGE_CLT_PAGE;
	msgbody.RangeClt.RangeCltData.RangeCltPage.MainType = mainType;
	msgbody.RangeClt.RangeCltData.RangeCltPage.SubType = subType;
	msgbody.RangeClt.RangeCltData.RangeCltPage.PageIdx = pageIdx;
	msgbody.RangeClt.RangeCltData.RangeCltPage.PageVersion = pageVer;
	SendGameServerMsg(RANGE_CLT,msgbody);
}

void GameRankManager::requestWatchPlayer( const char* pszRoleName )
{
	//TODO HERE
	printf( "on rank watch player: %s\n",pszRoleName );
	tagCSPkgBody msgbody;
	msgbody.RangeClt.RangeCltOp = RANGE_CLT_WATCH_PLAYER;
	strncpy_s( msgbody.RangeClt.RangeCltData.RangeCltWatchPlayer.RoleName, pszRoleName, strlen( pszRoleName ) + 1 );
	SendGameServerMsg( RANGE_CLT,msgbody );
}

void GameRankManager::requestWatchPet(int nLow,int nHeigh,const char* szOwnerName,int nLength)
{
	//TODO HERE
	tagLong_ToLua wid;
	wid.nHeight = nHeigh;
	wid.nLow = nLow;
	tagCSPkgBody msgbody;
	msgbody.RangeClt.RangeCltOp = RANGE_CLT_WATCH_PET;
	memcpy(&msgbody.RangeClt.RangeCltData.RangeCltWatchPet.WID,&wid,sizeof(tagLong_ToLua)) ;
	strncpy_s(msgbody.RangeClt.RangeCltData.RangeCltWatchPet.OwnerName,szOwnerName,nLength);
	SendGameServerMsg(RANGE_CLT,msgbody);
}

void GameRankManager::requestJoinClan(int nLow,int nHeight)
{
	tagLong_ToLua clanWid;
	clanWid.nHeight = nHeight;
	clanWid.nLow = nLow;
	tagCSPkgBody msgbody;
	msgbody.RangeClt.RangeCltOp = RANGE_CLT_JOIN_CLAN;
	memcpy(&msgbody.RangeClt.RangeCltData.RangeCltJoinClan.ClanWID,&clanWid,sizeof(tagLong_ToLua));
	SendGameServerMsg(RANGE_CLT,msgbody);
}

tagCSRangeSvrTip_toLua* GameRankManager::getMyRankTip()
{
	static_assert( sizeof(tagCSRangeSvrTip_toLua) == sizeof(CSRANGESVRTIP) );
	return (tagCSRangeSvrTip_toLua*)(&m_myRankTip);
}
tagCSRangeSvrTip_toLua* GameRankManager::getMyBestRankTip()
{
	static_assert( sizeof(tagCSRangeSvrTip_toLua) == sizeof(CSRANGESVRTIP) );
	return (tagCSRangeSvrTip_toLua*)(&m_myBestRankTip);
}

tagCSRangeSvrPage_toLua* GameRankManager::getPageData(int pageIdx,int mainType,int subType,bool bRequestServer)
{
	static_assert( sizeof(tagCSRangeSvrPage_toLua) == sizeof( CSRANGESVRPAGE ) );
	pageKey key(pageIdx,mainType,subType);
	PAGEINFOMAPIT it = m_pageInfoMap.find(key);
	if( it == m_pageInfoMap.end() )
	{
		CSRANGESVRPAGE pageInfo;
		pageInfo.PageVersion = 0;
		pageInfo.EntryNum = 0;
		pageInfo.TotalNum = 0;
		pageInfo.MainType = mainType;
		pageInfo.SubType = subType;
		m_pageInfoMap[key] = pageInfo ;
	}
	it = m_pageInfoMap.find(key);
	if( bRequestServer )
	{
		requestRankPageInfo( mainType,subType,pageIdx,it->second.PageVersion );
	}
	return (tagCSRangeSvrPage_toLua*)(&it->second);
}

void GameRankManager::requestSearch(const char* name,int mainType,int subType,int Flag)
{
	if( name == NULL )
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.RangeClt.RangeCltOp = RANGE_CLT_FIND;
	strncpy( msgbody.RangeClt.RangeCltData.RangeCltFind.Name,name,NAME_LEN );
	msgbody.RangeClt.RangeCltData.RangeCltFind.MainType = mainType;
	msgbody.RangeClt.RangeCltData.RangeCltFind.SubType = subType;
	msgbody.RangeClt.RangeCltData.RangeCltFind.Flag = Flag;
	SendGameServerMsg(RANGE_CLT,msgbody);
}

tagCSRangeSvrPage_toLua* GameRankManager::getSearchData(const char* searchName,int pageIdx,int mainType,int subType,bool bRequestServer)
{
	static_assert( sizeof(tagCSRangeSvrPage_toLua) == sizeof( CSRANGESVRPAGE ) );
	pageKey key(pageIdx,mainType,subType,searchName);
	PAGEINFOMAPIT it = m_pageInfoMap.find(key);
	if( it == m_pageInfoMap.end() )
	{
		CSRANGESVRPAGE pageInfo;
		pageInfo.PageVersion = 0;
		pageInfo.EntryNum = 0;
		pageInfo.TotalNum = 0;
		pageInfo.MainType = mainType;
		pageInfo.SubType = subType;
		m_pageInfoMap[key] = pageInfo ;
	}
	it = m_pageInfoMap.find(key);
	if( bRequestServer )
	{
		requestSearch( searchName,mainType,subType,0 );
	}
	return (tagCSRangeSvrPage_toLua*)(&it->second);
}

tagCSRangeSvrTip_toLua* GameRankManager::getCurMyRank(int mainType,int subType)
{
	static_assert( sizeof(tagCSRangeSvrTip_toLua) == sizeof( CSRANGESVRTIP ) );
	pageKey key(0,mainType,subType);
	RANKINFOMAPIT it = m_MyRankInfoMap.find(key);
	if( it == m_MyRankInfoMap.end() )
	{
		CSRANGESVRTIP info;
		info.CurrRange = -1;
		info.LastRange = -1;
		info.MainType = mainType;
		info.SubType = subType;
		m_MyRankInfoMap[key] = info;
		if( mainType == RANGE_MAIN_CLAN )
		{
			 const tagClanViewer& viewer = GameClanManager::getSingleton().getClanViewer();
			 if( strlen(viewer.m_clanInfo.m_szClanName) > 0 )
			 {
				 requestSearch(viewer.m_clanInfo.m_szClanName,mainType,subType,1);
			 }
		}else{
			requestSearch( ActorManager::getSingleton().getMainPlayer()->getName(),mainType,subType,1 );
		}
	}
	it = m_MyRankInfoMap.find(key);
	return (tagCSRangeSvrTip_toLua*)(&it->second);
}

void GameRankManager::changeRankPlayerName(const char* szOldName,const char* szNewName)
{
	bool bChangeName = false;
	PAGEINFOMAPIT it = m_pageInfoMap.begin();
	while( it != m_pageInfoMap.end() )
	{
		bool bNeedDelete = false;
		CSRANGESVRPAGE& pageData = it->second;
		for( int i = 0 ; i < pageData.EntryNum ; i++ )
		{
			if( pageData.Entrys[i].MainType == RANGE_MAIN_SINGLE 
				|| pageData.Entrys[i].MainType == RANGE_MAIN_CAREER
				|| pageData.Entrys[i].MainType == RANGE_MAIN_COMBAT
				|| pageData.Entrys[i].MainType == RANGE_MAIN_BATTLE )
			{
				if( strcmp( pageData.Entrys[i].RangeData.FirstName,szOldName ) == 0 )
				{
					bChangeName = true;
					if( it->first.strName != "" )
					{
						bNeedDelete = true;
						break;
					}else{
						strcpy( pageData.Entrys[i].RangeData.FirstName,szNewName );
					}
				}
			}else if( pageData.Entrys[i].MainType == RANGE_MAIN_PET 
				|| pageData.Entrys[i].MainType == RANGE_MAIN_CLAN )
			{
				if( strcmp( pageData.Entrys[i].RangeData.SecondName,szOldName ) == 0 )
				{
					bChangeName = true;
					if( it->first.strName != "" )
					{
						bNeedDelete = true;
						break;
					}else{
						strcpy( pageData.Entrys[i].RangeData.SecondName,szNewName );
					}
				}				
			}
		}
		if( bNeedDelete )
		{
			it = m_pageInfoMap.erase(it);
		}else{
			++ it;
		}		
	}
	for( ; it != m_pageInfoMap.end() ; it ++)
	{
		CSRANGESVRPAGE& pageData = it->second;

		
	}
	if( bChangeName )
	{
		GameManager::getSingleton().SendUIEvent( "RANK_NEW_PAGE_DATA" );
	}
}

void GameRankManager::changeRankClanName(const char* szOldName,const char* szNeName)
{
	bool bChangeName = false;
	PAGEINFOMAPIT it = m_pageInfoMap.begin();
	while( it != m_pageInfoMap.end() )
	{
		CSRANGESVRPAGE& pageData = it->second;
		bool bNeedDelete = false;
		for( int i = 0 ; i < pageData.EntryNum ; i++ )
		{
			if( pageData.Entrys[i].MainType == RANGE_MAIN_SINGLE
				|| pageData.Entrys[i].MainType == RANGE_MAIN_CAREER)
			{				
				if( strcmp(pageData.Entrys[i].RangeData.SecondName,szOldName) == 0 )
				{
					bChangeName = true;
					if( it->first.strName != "" )
					{
						bNeedDelete = true;
						break;
					}else{
						strcpy( pageData.Entrys[i].RangeData.SecondName,szNeName );
					}					
				}
			}else if( pageData.Entrys[i].MainType == RANGE_MAIN_CLAN )
			{
				if( strcmp(pageData.Entrys[i].RangeData.FirstName,szOldName) == 0 )
				{
					bChangeName = true;
					if( it->first.strName != "" )
					{
						bNeedDelete = true;
						break;
					}else{
						strcpy( pageData.Entrys[i].RangeData.FirstName,szNeName );
					}
				}
			}
		}
		if( bNeedDelete )
		{
			it = m_pageInfoMap.erase(it);
		}else{
			++ it;
		}
	}

	if( bChangeName )
	{
		GameManager::getSingleton().SendUIEvent("RANK_NEW_PAGE_DATA");
	}
}