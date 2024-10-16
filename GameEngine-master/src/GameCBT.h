#pragma once
#include <vector>
#include "proto_comm.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack( 1 )
struct tagResPosEngine_ToLua
{
	int X;                              
	int Y;                              
};

struct tagCangBaoTuMapInfo_ToLua
{
	unsigned int ItemID;                          	/*   ��ƷID */
	unsigned int MapID;                           	/*   ��ͼID */
	tagResPosEngine_ToLua Pos;                             	/*   ���صص� */
};

struct tagCangBaoTuInfo_ToLua
{
	unsigned int UpdateDayTime;                   	/*   ÿ��ˢ��ʱ�� */
	short DayTimes;                        	/*   ÿ��ʹ�ô���,������5 */
	tagCangBaoTuMapInfo_ToLua CangBaoTuMapInfo[CANGBAOTU_NUM];
};

struct tagCangBaoTuDef_ToLua
{
	unsigned int MapID;                           	/*   ���ڵ�ͼ */
	unsigned char Level;                           	/*   ��С�ȼ���0��ʾ������ */
	unsigned int ItemID;                          	/*   �ر�ͼid */
	char MapPic[RES_NAME_LEN];            	/*   ����ͼ */
	tagResPosEngine_ToLua Pos[CANGBAOTUPOS_NUM];           	/*   �ر�ͼ����� */
};

#pragma pack()

const tagCangBaoTuDef_ToLua* getCangBaoTuDefDef( int nMapID, int nLevel, int nItemID );

//tolua_end

struct tagCangBaoTuInfo;
struct tagCangBaoTuDef;
class GameCBT//tolua_export
{//tolua_export
public:
	enum { MAX_CANG_BAO_TIMES_ONE_DAY = 5 };
	GameCBT(void);
	~GameCBT(void);
	void	setCangBaoTuInfo( const tagCangBaoTuInfo& cangBaoTuList );

	//tolua_begin
	bool isCangBaoTuItem( int nItemID );
	int   getMaxCangBaoTuMap();
	const tagCangBaoTuMapInfo_ToLua&	getCangBaoTuMapInfo( int nIndex ) const;
	const tagCangBaoTuMapInfo_ToLua&	getCangBaoTuMapInfo( int nIndex );
	const tagCangBaoTuMapInfo_ToLua*	getCangBaoTuMapInfoByItemID( int nItemID );
	
	bool isArriveMaxCangBaoTimesOneDay();
	int getAlreadyUseTimes();
	int getCanUseTimes();

	// �������òر�ͼ
	void send_reset( int nItemId );
	// 
	bool isRequestReset();
	void setRequestReset( bool bRequestReset );
	//tolua_end

private:
	tagCangBaoTuInfo_ToLua	m_cangBaoTuList;
	std::vector<tagCangBaoTuDef*>	m_cangBaoTuDef;
	bool	m_bRequestReset;
};//tolua_export
