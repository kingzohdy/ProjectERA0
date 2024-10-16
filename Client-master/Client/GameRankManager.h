#pragma once

#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include <map>
#include <string>

// ���а�
#pragma pack(1)

//tolua_begin
struct tagCSRangeSvrTip_toLua
{
	int MainType;                       
	int SubType;                        
	int LastRange;                       	/*   �ϴ�����, -1��ʾ��� */
	int CurrRange;                       	/*   ��������, -1��ʾ��� */
};

struct tagRangeData_toLua
{
	unsigned int RangeVal;                       
	char FirstName[ROLE_NAME_LEN];        	/*   ���ˡ�ְҵ��ս����ʾ��ɫ���������ʾ�������������ʾ������ */
	int Career;                          	/*   ���ˡ�ְҵ��ʾ��ɫְҵ������������ */
	int Level;                           	/*   ���ˡ�ְҵ��ʾ��ɫ�ȼ���ս����ʾ�����Ѷȣ������ʾ����ȼ� */
	tagLong_ToLua GID;                      /*   ���ˡ�ְҵ��ս����ʾ��ɫGID�������ʾ����GID�������ʾ����GID */
	char SecondName[NAME_LEN];            	/*   ���ˡ�ְҵ��ʾ�������������ʾ�᳤����ս����ʾ�������������ʾ������ */
	int LastRange;                       	/*   �ϴ����� */
	int Val1;                            	/*  Ver.2004  */
	int Val2;                            	/*  Ver.2239  */
};


struct tagRange_toLua
{
	int MainType;                        	/*   �������� */
	int SubType;                         	/*   ���������� */
	int Range;                           	/*   ���� */
	tagRangeData_toLua RangeData;                      
};

struct tagCSRangeSvrPage_toLua
{
	int MainType;                       
	int SubType;                        
	int TotalNum;                        	/*  Ver.260  */
	int PageIdx;                         	/*   ҳ�ţ���1��ʼ */
	int PageVersion;                     	/*   ҳ�汾 */
	int EntryNum;                        	/*   ��ҳ�ж��ټ�¼ */
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

	CSRANGESVRTIP			m_myRankTip;	//���ǵ�tip
	CSRANGESVRTIP			m_myBestRankTip;	//���������ǰ��tips
	PAGEINFOMAP				m_pageInfoMap;	//ҳ����

 	RANKINFOMAP				m_MyRankInfoMap;	//���ǵ�������Ϣ


};//tolua_export

