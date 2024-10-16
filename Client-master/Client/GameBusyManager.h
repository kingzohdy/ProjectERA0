#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDBData.h"
#include "resdb_meta.h"
#include "ItemDefine.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"

//tolua_begin
#pragma pack(1) 

struct tagBusyMoney_ToLua
{
	int MoneyID;                        
	int MoneyNum;                       
};
//struct tagRespactEntry_ToLua
//{
//	unsigned int ID;                              	/*   ����id */
//	unsigned int Value;                           	/*   ������ֵ|ע��������ֵ���ۼ����������׶ε���ֵ */
//};

struct tagBusyItem_ToLua
{
	int ItemID;                         
	int ItemNum;                        
	int Bind;                           
};
struct tagBusyDef_ToLua
{
	int ID;                              	/*   �Һ�æID,��1��ʼ */
	char Name[RES_NAME_LEN];              	/*   ���� */
	int Type;                            	/*   ���� Bind Macrosgroup:BUSY_TYPE,*/
	int PworldID;                       
	int TaskID;                         
	int TaskRecycleID;                  
	int FiniTaskID[MAX_BUSY_FINI_TASK]; 
	tagBusyMoney_ToLua CostMoney;                      
	int CostItemNum;                    
	int CostMoney15;                    
	int CostMoney20;                    
	int ExpMul15;                        	/*   ��̬����ϵ�� */
	int ExpMul20;                        	/*   ��̬����ϵ�� */
	int Exp;                            
	int ExpMul;                          	/*   ��̬����ϵ�� */
	tagBusyMoney_ToLua AwardMoney;                     
	tagRespactEntry_ToLua Respact;          /*   �������� */
	tagBusyItem_ToLua AwardItem;
	int AwardMoneyMul;                   	/*  Ver.1872  */
	int ClanMoney;                       	/*  Ver.1872  */
	int ClanOwnedContribute;             	/*  Ver.1872  */
	int ClanAnDing;                      	/*  Ver.1891  */
	tagBusyItem_ToLua EXAwardItem[MAX_BUSY_AWARD_ITEM]; 	/*  Ver.2503 ���˱�����һ���������ߣ��ټ�n�� */
};
struct tagBusyUseDef_ToLua
{
	int Num;                             	/*   �ڼ��� */
	int MoneyVal;                       
	int ItemNum;                        
};
#pragma pack()
//tolua_end
class GameBusyManager //tolua_export
	: public NetMsgHandler,public Ogre::Singleton<GameBusyManager>
{//tolua_export
public:
	GameBusyManager();
	~GameBusyManager();

	//tolua_begin
	void						IsBusyTaskCommit( int nId ,int nItem,int nFiniType );
	int							GetBeachTime();
	void						SetBeachTime( int nTime );
	int							GetUsedNum();
	const tagBusyDef_ToLua*		getBusyDef( int nId );
	const tagBusyUseDef_ToLua*	getBusyUsedDef( int nNum );

	//tolua_end
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	
public:
	int			m_nUsedNum;

private:
	int			m_nBeachTime;
};//tolua_export