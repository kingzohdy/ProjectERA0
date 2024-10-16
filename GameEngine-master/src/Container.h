#pragma once
#include "Container_Common.h"
#include "OgreSingleton.h"
#include "GameSkill_Common.h"
#include <map>
//tolua_begin
#pragma pack( 1 )
struct tagGemInfo_ToLua
{
	unsigned int ItemID;                         
	int Num;                             	/*   获得该道具的个数 */
	int Bind;                            	/*   获得该道具时，绑定不，0代表不，1代表绑定 */
	char Desc[RES_NAME_LEN];              	/*   ui显示内容 */
};
struct tagGemPagDef_ToLua
{
	int Id;                              	/*   宝石包id */
	tagGemInfo_ToLua GemInfo[MAX_GEM_PAG_GEM_NUM];    	/*   宝石相关 */
};
struct tagArmTeshuDef_ToLua
{
	unsigned int TeshuID;                        
	short Rate;                            	/*   万分率0-表示是属性加成 */
	tagResult_ToLua Result[MAX_TESHU_RESULT];        	/*   道具效果 */
	char DefDesc[MAX_TESHU_DESC];        
};

#define MAX_GEM_PAG_GEM_NUM_TOLUA MAX_GEM_PAG_GEM_NUM
#pragma pack()
const tagGemPagDef_ToLua*	getGemPagDef( int nId );
const tagArmTeshuDef_ToLua* getArmTeshuDef( int nId );
//tolua_end

class Container//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<Container>
{//tolua_export
public:
	Container(void);
	~Container(void);

public:	
	virtual int	HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );	
	virtual void Update(unsigned int dtick);
	
	void setContainerGridInfo( int nListType, int nMaxNum );
	void setArmChange ( int nListType, const ROLEARM& arm   );
	void setItemChange( int nListType, const ROLEITEM& item );
	
	//tolua_begin
	int getUnusedGridNum( int nListType );
	int getContainerMaxNum(int nListType);
	
	// 从身上获取药品
	Item& getItem( int id );
	int	  getItemCount( int Id );
	// 获取物品的总数量
	int   getItemTotalNum( int nId );
	Item& getItem( int nListType, int GridIndex );
	Item* getItem(tdr_ulonglong WID);
	bool  getItemLink( int nListType, int GridIndex );
	int   getItemResultID( const tagItemDef_ToLua* pItemDef );
	bool  canUseItemPre( Item& useItem, int nTargetID );
	bool  isExpendableItemLimitdByScene( Item& used_Item );
	bool  canUseExpendableItem();
	bool  usedItem( int nListType, int GridIndex, int wearPos = 0, int nTargetIndex = 0 );
	bool  usedItem( int id );
	int   getAmmCount( int nAmmType );
	// 获取空的背包栏位
	int	  getNoneGridIndex( int nListType, int nItemId = 0, int nIndex = 0, bool bBind = true );
	
	int	  getCurBootyId() { return m_nCurBootyId; }
	int   getBootyCount( int id );
	Item& getBootyItem( int id, int GridIndex );
	void  closeBooty( int id );

	// 登录请求所有容器信息
	void send_GetInfo();
	// 容器物品移动
	void send_MoveItem( int srcList, int destList, int srcGridIndex, int srcNum, int destGridIndex );
	// 容器请求整理
	void send_Arranger();
	// 金钱在容器间转移
	void send_MoveMoney( int srcAtterId, int destAtterId, int money );
	// 关闭仓库
	void send_CloseStash();
	// 背包整理
	void send_PackageArrange( int ListType );
	// 拾取物品
	void send_PickItem( int id );
	// 拾取全部物品
	void send_PickAllItem( int id );
	// 拾取选中物品
	void send_SelectItem( int id, int selectIndex ); 
	// 请求使用消耗品
	void send_UsedExpendableItem( int nListType, int GridIndex, int nTargetIndex = 0, int nSubIndex = 1,int UseType = 0 );
	// 宠物喂养
	void send_PetUsedItem( int nListType, int GridIndex, int nHeightId, int nLowId );

	// 物品使用cd转动
	void  passItemCD( int id );
	// 获取物品使用cd
	float getItemCD( int id );
	// 物品是否还在CD转动
	bool isItemInCD( int id );
	// 获取物品使用已经经过的cd
	float getItemCDPassTime( int id );
	// 设置物品指定类型cd
	void  setItemTypeCD( int type, int cd );
	// 物品是否可放入背包
	bool  canDragIn( int nListType, int nId );
	//兑换金币
	void  send_ChangeCoin(int nWay,int nCount,int npcId);

	// 设置背包锁
	void requestSetContainerLock( const char* szPwd );
	//修改密码
	void requestChangeContainerPsd(const char* szOldPwd,const char* szNewPwd);
	//验证密码
	void requestCheckContainerPsd(const char* szPwd);
	//取消背包锁， 需要在解锁的状态下才行
	void requestCancelContainerLock( const char* szPwd );
	// 锁上背包
	void requestLockContainer();
	//获得背包的锁状态
	int getContainerLockState();

	int getStuffNum( int ListType );
	int getItemPos( int nListType,int nItemID );

	// 物品是否处于使用前的吟唱状态
	bool isUseItemPre();
	
	//获得没有锁定的物品位
	int getUnLockItemPos( int nListType,int nItemID );
	
	//扩充银行
	void requestAddStashGrid();

	int getBindItemNum( int nListType, int nItemID );

	int getUnBindItemNum( int nListType, int nItemID );

	int getUnBindItemNumNotCheckLock( int nListType, int nItemID );

	int getBindItemNumNotCheckLock( int nListType, int nItemID );

	// 任务追踪处的道具是否可以使用
	bool canUseTaskItem( int id );

	void onItemLockChange();
	//tolua_end

	// 设置背包变化时物品的数量
	void setItemChgCount( const ROLEPACKAGECHG& Packge_Info_Change );
	void setItemCount( int Id, int count );
	void setItemTypeCD( int type, int cd, Item& item );

	void  useItemPre( Item& used_Item, MainGamePlayer* pMainPlayer, int nTargetIndex );

private:
	// 是否在指定区域使用
	bool isInUseArea( const tagItemDef_ToLua* pItemDef, MainGamePlayer* pMainPlayer );
	bool canUseTarget( Item& used_Item, MainGamePlayer* pMainPlayer, int nTargetIndex );

	void checkAddNewItem( const ROLEPACKAGECHG& Packge_Info_Change );

private:
	ContainerData*				m_pContainer[CONTAINER_TYPE_END];	// 物品容器数据
	std::map<int, tagBootyInfo> m_mapBootInfo;						// 物品拾取
	int							m_nCurBootyId;						// 当前拾取的物品	
	std::map<int, int>			m_ItemCount;						// 该类型物品身上当前数量
	std::map<int, tagCDTime>	m_ItemCDMap;						// 物品冷却时间
	std::map<int, tagCDTime>	m_ItemTypeCD;						// 物品指定类型冷却
	bool						m_bPre;								// 物品使用前的吟唱状态
	int							m_nLockFlag;						// 背包是否被锁标记，m_nLockFlag&SAFE_LOCK_ACTIVE_FLAG!=0表示安全锁激活 m_nLockFlag&SAFE_LOCK_PASS_FLAG!=0表示认证通过
};//tolua_export
