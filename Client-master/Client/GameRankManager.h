#pragma once

#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include <map>
#include <string>

// 排行榜
#pragma pack(1)

//tolua_begin
struct tagCSRangeSvrTip_toLua
{
	int MainType;                       
	int SubType;                        
	int LastRange;                       	/*   上次排名, -1表示落榜 */
	int CurrRange;                       	/*   现在排名, -1表示落榜 */
};

struct tagRangeData_toLua
{
	unsigned int RangeVal;                       
	char FirstName[ROLE_NAME_LEN];        	/*   个人、职业、战斗表示角色名，公会表示公会名，宠物表示宠物名 */
	int Career;                          	/*   个人、职业表示角色职业，其他无意义 */
	int Level;                           	/*   个人、职业表示角色等级，战斗表示副本难度，宠物表示宠物等级 */
	tagLong_ToLua GID;                      /*   个人、职业、战斗表示角色GID，公会表示公会GID，宠物表示宠物GID */
	char SecondName[NAME_LEN];            	/*   个人、职业表示公会名，公会表示会长名，战斗表示副本名，宠物表示主人名 */
	int LastRange;                       	/*   上次排名 */
	int Val1;                            	/*  Ver.2004  */
	int Val2;                            	/*  Ver.2239  */
};


struct tagRange_toLua
{
	int MainType;                        	/*   排名类型 */
	int SubType;                         	/*   排名子类型 */
	int Range;                           	/*   名次 */
	tagRangeData_toLua RangeData;                      
};

struct tagCSRangeSvrPage_toLua
{
	int MainType;                       
	int SubType;                        
	int TotalNum;                        	/*  Ver.260  */
	int PageIdx;                         	/*   页号，从1开始 */
	int PageVersion;                     	/*   页版本 */
	int EntryNum;                        	/*   本页有多少记录 */
	tagRange_toLua Entrys[RANGE_MAX_PAGE_ENTRY];
};

//tolua_end
#pragma pack()


struct pageKey 
{
	pageKey(int idx,int main,int sub,std::string name = "")
	{
		strName = name;
		pageIdx = idx;
		mainType = main;
		subType = sub;
	}
	std::string strName;
	int pageIdx;
	int mainType;
	int subType;
};

// static bool operator== (const pageKey& key1, const pageKey& key2)
// {
// 	return ((key1.mainType == key2.mainType) && (key1.subType == key2.subType) &&(key1.pageIdx == key2.pageIdx) );
// }

static bool operator< ( const pageKey& key1, const pageKey& key2 )
{
	if( key1.strName == key2.strName )
	{
		if( key1.mainType < key2.mainType ) return true;
		else if(key1.mainType > key2.mainType ) return false;
		else{
			if (key1.subType < key2.subType) return true;
			else if (key1.subType > key2.subType ) return false;
			else{
				return key1.pageIdx < key2.pageIdx;
			}
		}
	}else{
		//TODO HERE
		return (key1.strName.compare(key2.strName) <0 ? true:false) ;
	}
}


class GameRankManager:	//tolua_export
	public Ogre::Singleton<GameRankManager>,public NetMsgHandler
{//tolua_export
public:
	GameRankManager();
	~GameRankManager();

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	void	requestRankPageInfo(int mainType,int subType,int pageIdx,int pageVer);
	void	requestWatchPlayer( const char* pszRoleName );
	void	requestWatchPet(int nLow,int nHeight,const char* szOwnerName,int nLength);
	void	requestJoinClan(int nLow,int nHeight);
	void	requestSearch(const char* name,int mainType,int subType,int Flag);

	tagCSRangeSvrTip_toLua*		getMyRankTip();
	tagCSRangeSvrTip_toLua*		getMyBestRankTip();
	tagCSRangeSvrPage_toLua*	getPageData(int pageIdx,int mainType,int subType,bool bRequestServer = false);

	tagCSRangeSvrPage_toLua*	getSearchData(const char* searchName,int pageIdx,int mainType,int subType,bool bRequestServer = false);
	tagCSRangeSvrTip_toLua*		getCurMyRank(int mainType,int subType);

	void clearInfo();

	//tolua_end
	void	changeRankPlayerName(const char* szOldName,const char* szNewName);
	void	changeRankClanName(const char* szOldName,const char* szNeName);

private:

	typedef std::map<pageKey,CSRANGESVRPAGE> PAGEINFOMAP;
	typedef std::map<pageKey,CSRANGESVRPAGE>::iterator PAGEINFOMAPIT;

	typedef std::map<pageKey,CSRANGESVRTIP> RANKINFOMAP;
	typedef std::map<pageKey,CSRANGESVRTIP>::iterator RANKINFOMAPIT;

	CSRANGESVRTIP			m_myRankTip;	//主角的tip
	CSRANGESVRTIP			m_myBestRankTip;	//主角排名最靠前的tips
	PAGEINFOMAP				m_pageInfoMap;	//页数据

 	RANKINFOMAP				m_MyRankInfoMap;	//主角的排行信息


};//tolua_export

