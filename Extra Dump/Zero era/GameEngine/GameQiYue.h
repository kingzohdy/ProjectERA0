#pragma once
#include "proto_comm.h"
//tolua_begin
#pragma pack(1)
struct tagQiYueInfo_ToLua
{
	unsigned int CurrQiYueID;                     	/*   当前使用的契约 */
	unsigned int QiYueNum;                        	/*   已经学过的契约 */
	unsigned int QiYueMax;                        	/*   最大契约个数 */
	unsigned int QiYueList[QIYUE_MAX_NUM];        	/*   已学契约的列表 */
};

#pragma pack()
//tolua_end

struct tagQiYueInfo;
class GameQiYue//tolua_export
{//tolua_export
public:
	GameQiYue(void);
	~GameQiYue(void);
	void	setQiYueList( const tagQiYueInfo& qiYueList );

	//tolua_begin
	const tagQiYueInfo_ToLua&	getQiYueList() const;
	const tagQiYueInfo_ToLua&	getQiYueList();
	//tolua_end

private:
	tagQiYueInfo_ToLua	m_qiYueList;
};//tolua_export
