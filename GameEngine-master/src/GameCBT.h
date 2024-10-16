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
	unsigned int ItemID;                          	/*   物品ID */
	unsigned int MapID;                           	/*   地图ID */
	tagResPosEngine_ToLua Pos;                             	/*   宝藏地点 */
};

struct tagCangBaoTuInfo_ToLua
{
	unsigned int UpdateDayTime;                   	/*   每日刷新时间 */
	short DayTimes;                        	/*   每日使用次数,上限是5 */
	tagCangBaoTuMapInfo_ToLua CangBaoTuMapInfo[CANGBAOTU_NUM];
};

struct tagCangBaoTuDef_ToLua
{
	unsigned int MapID;                           	/*   所在地图 */
	unsigned char Level;                           	/*   最小等级，0表示不限制 */
	unsigned int ItemID;                          	/*   藏宝图id */
	char MapPic[RES_NAME_LEN];            	/*   轮廓图 */
	tagResPosEngine_ToLua Pos[CANGBAOTUPOS_NUM];           	/*   藏宝图坐标点 */
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

	// 请求重置藏宝图
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
