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
	int Num;                             	/*   ��øõ��ߵĸ��� */
	int Bind;                            	/*   ��øõ���ʱ���󶨲���0������1����� */
	char Desc[RES_NAME_LEN];              	/*   ui��ʾ���� */
};
struct tagGemPagDef_ToLua
{
	int Id;                              	/*   ��ʯ��id */
	tagGemInfo_ToLua GemInfo[MAX_GEM_PAG_GEM_NUM];    	/*   ��ʯ��� */
};
struct tagArmTeshuDef_ToLua
{
	unsigned int TeshuID;                        
	short Rate;                            	/*   �����0-��ʾ�����Լӳ� */
	tagResult_ToLua Result[MAX_TESHU_RESULT];        	/*   ����Ч�� */
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
	
	// �����ϻ�ȡҩƷ
	Item& getItem( int id );
	int	  getItemCount( int Id );
	// ��ȡ��Ʒ��������
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
	// ��ȡ�յı�����λ
	int	  getNoneGridIndex( int nListType, int nItemId = 0, int nIndex = 0, bool bBind = true );
	
	int	  getCurBootyId() { return m_nCurBootyId; }
	int   getBootyCount( int id );
	Item& getBootyItem( int id, int GridIndex );
	void  closeBooty( int id );

	// ��¼��������������Ϣ
	void send_GetInfo();
	// ������Ʒ�ƶ�
	void send_MoveItem( int srcList, int destList, int srcGridIndex, int srcNum, int destGridIndex );
	// ������������
	void send_Arranger();
	// ��Ǯ��������ת��
	void send_MoveMoney( int srcAtterId, int destAtterId, int money );
	// �رղֿ�
	void send_CloseStash();
	// ��������
	void send_PackageArrange( int ListType );
	// ʰȡ��Ʒ
	void send_PickItem( int id );
	// ʰȡȫ����Ʒ
	void send_PickAllItem( int id );
	// ʰȡѡ����Ʒ
	void send_SelectItem( int id, int selectIndex ); 
	// ����ʹ������Ʒ
	void send_UsedExpendableItem( int nListType, int GridIndex, int nTargetIndex = 0, int nSubIndex = 1,int UseType = 0 );
	// ����ι��
	void send_PetUsedItem( int nListType, int GridIndex, int nHeightId, int nLowId );

	// ��Ʒʹ��cdת��
	void  passItemCD( int id );
	// ��ȡ��Ʒʹ��cd
	float getItemCD( int id );
	// ��Ʒ�Ƿ���CDת��
	bool isItemInCD( int id );
	// ��ȡ��Ʒʹ���Ѿ�������cd
	float getItemCDPassTime( int id );
	// ������Ʒָ������cd
	void  setItemTypeCD( int type, int cd );
	// ��Ʒ�Ƿ�ɷ��뱳��
	bool  canDragIn( int nListType, int nId );
	//�һ����
	void  send_ChangeCoin(int nWay,int nCount,int npcId);

	// ���ñ�����
	void requestSetContainerLock( const char* szPwd );
	//�޸�����
	void requestChangeContainerPsd(const char* szOldPwd,const char* szNewPwd);
	//��֤����
	void requestCheckContainerPsd(const char* szPwd);
	//ȡ���������� ��Ҫ�ڽ�����״̬�²���
	void requestCancelContainerLock( const char* szPwd );
	// ���ϱ���
	void requestLockContainer();
	//��ñ�������״̬
	int getContainerLockState();

	int getStuffNum( int ListType );
	int getItemPos( int nListType,int nItemID );

	// ��Ʒ�Ƿ���ʹ��ǰ������״̬
	bool isUseItemPre();
	
	//���û����������Ʒλ
	int getUnLockItemPos( int nListType,int nItemID );
	
	//��������
	void requestAddStashGrid();

	int getBindItemNum( int nListType, int nItemID );

	int getUnBindItemNum( int nListType, int nItemID );

	int getUnBindItemNumNotCheckLock( int nListType, int nItemID );

	int getBindItemNumNotCheckLock( int nListType, int nItemID );

	// ����׷�ٴ��ĵ����Ƿ����ʹ��
	bool canUseTaskItem( int id );

	void onItemLockChange();
	//tolua_end

	// ���ñ����仯ʱ��Ʒ������
	void setItemChgCount( const ROLEPACKAGECHG& Packge_Info_Change );
	void setItemCount( int Id, int count );
	void setItemTypeCD( int type, int cd, Item& item );

	void  useItemPre( Item& used_Item, MainGamePlayer* pMainPlayer, int nTargetIndex );

private:
	// �Ƿ���ָ������ʹ��
	bool isInUseArea( const tagItemDef_ToLua* pItemDef, MainGamePlayer* pMainPlayer );
	bool canUseTarget( Item& used_Item, MainGamePlayer* pMainPlayer, int nTargetIndex );

	void checkAddNewItem( const ROLEPACKAGECHG& Packge_Info_Change );

private:
	ContainerData*				m_pContainer[CONTAINER_TYPE_END];	// ��Ʒ��������
	std::map<int, tagBootyInfo> m_mapBootInfo;						// ��Ʒʰȡ
	int							m_nCurBootyId;						// ��ǰʰȡ����Ʒ	
	std::map<int, int>			m_ItemCount;						// ��������Ʒ���ϵ�ǰ����
	std::map<int, tagCDTime>	m_ItemCDMap;						// ��Ʒ��ȴʱ��
	std::map<int, tagCDTime>	m_ItemTypeCD;						// ��Ʒָ��������ȴ
	bool						m_bPre;								// ��Ʒʹ��ǰ������״̬
	int							m_nLockFlag;						// �����Ƿ�����ǣ�m_nLockFlag&SAFE_LOCK_ACTIVE_FLAG!=0��ʾ��ȫ������ m_nLockFlag&SAFE_LOCK_PASS_FLAG!=0��ʾ��֤ͨ��
};//tolua_export
