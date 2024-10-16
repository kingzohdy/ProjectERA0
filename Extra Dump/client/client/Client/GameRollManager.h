/************************************************************************/
/* Roll�����ģ��                                                       */
/************************************************************************/

#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "ItemDefine.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "Item.h"
#include <vector>

#pragma pack(1)
//tolua_begin

enum tagROLL_CLT_TYPE_TOLUA 
{
	ROLL_CLT_NEED_TOLUA = 0, 	/* ���� */
	ROLL_CLT_QUIT_TOLUA = 1, 	/* �رջ���� */
};

union tagRoleGridData_ToLua
{
	tagRoleItem_ToLua RoleItem;                        	/* GRID_ITEM_ITEM,  ��װ������ */
	tagRoleArm_ToLua RoleArm;                         	/* GRID_ITEM_ARM,  װ�� */
};

struct tagRoleGrid_ToLua
{
	char Type;                            	/*   ��Ʒ���� Bind Macrosgroup:GRID_ITEM_TYPE,*/
	char Res[3];                         
	tagRoleGridData_ToLua GridData;                       
};

struct tagCSRollUIInfo_ToLua
{
	int BootyID;                         	/*   ������MemID */
	tagRoleGrid_ToLua BootyInfo;                      
	int EndTime;                         	/*  Ver.590 ����ʱʣ������ */
};
//tolua_end
#pragma pack()

typedef std::vector<tagCSRollUIInfo> VERROLLUIINFO;

class GameRollManager//tolua_export
	:public Ogre::Singleton<GameRollManager>,public NetMsgHandler	
{//tolua_export
public:

	GameRollManager();
	~GameRollManager();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	//tolua_begin

	void sendRollMessage( int nFlag,int nBootId );

	tagCSRollUIInfo_ToLua* getRollUIInfo( int nBootId );

	Item* getRollItem( int nBootId );
	//tolua_end

	void	eraseRollUIInfo( int nBootId );
protected:
private:
	VERROLLUIINFO	m_RollUIInfo;
	Item			m_RollItem;
};//tolua_export