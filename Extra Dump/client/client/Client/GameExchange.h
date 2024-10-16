#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "Item.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameActor_Pet.h"

//tolua_begin
// 己方交易宠物信息
struct tagSrcExchangePet
{
	int m_nListType;	// 0仙子, 1侍卫, -1改栏没有宠物                       
	int m_nGridIdx;		// 身上的格子位
	int m_nHeightID;
	int m_nLowID;
	
	bool isValid()
	{
		if( m_nListType == -1 || m_nGridIdx == -1 )
		{
			return false;
		}
		return true;
	}

	tagSrcExchangePet()
	{
		reset();
	}

	void reset()
	{
		m_nListType = -1;
		m_nGridIdx	= -1;
		m_nHeightID	= 0;
		m_nLowID	= 0;
	}
};

enum
{
	CLIENT_MAX_TRADE_MONEY	= INT_MAX,
	CLIENT_MAX_TRADE_DIST	= 10,
};
//tolua_end

class GameExchange//tolua_export
		: public NetMsgHandler, public Ogre::Singleton<GameExchange>
{//tolua_export
public:
	GameExchange(void);
	~GameExchange(void);

public:
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void update(unsigned int dtick);
	
	//tolua_begin
	
	// c->s
	void test( const char* pszContent );
	void helpTest( const char* pszContent );

	// 请求交易
	void inviteExchange( const char* szName );
	// 接受邀请
	void acceptExchange( const char* pszName );
	// 拒绝邀请
	void rejectExchange( const char* pszName, int nRejectType );
	// 取消交易(客户端自己关闭对话框)
	void cancelExchange( int nRejectType );
	// 交易锁定
	void lockExchange();
	// 交易确认
	void dealExchange();
	// 取消锁定交易
	void cancelLock();
public:
	
	// 设置交易的金钱数目
	void addExchangeMoney( unsigned int nMoney );
	// 添加一件交易物品
	// 参数( 交易栏的位置，目标容器，格子 )
	void addExchangeItem( int nIndex, int nListType, int nGrid, int nNum );
	// 清除交易物品
	void delExchangeItem( int nIndex );
	// 添加一个交易宠物
	// 参数( 交易栏的位置, 目标容器(tagLIST_TYPE 0仙子, 1侍卫), 所在宠物栏格子 ) 
	void addExchangePet( int nIndex, int nListType, int nGrid, int nHeightID, int nLowID );
	// 清除一个交易宠物
	void delExchangePet( int nIndex );

	// 获取交易物品( nType 0为自身1为交易方, nIndex为交易栏上的index )
	Item& getExchangItem( int nType, int nIndex );
	
	void synchronizeSelfPet();
    //TODO::宠物修改
	// 获取交易宠物信息( nType 0为自身1为交易方, nIndex为交易栏上的index )
	// 注意：如果取到的PetID为0,表示这个栏位没有宠物就不用弹开宠物资质查看面板
    tagRolePet_ToLua* getExchangePetInfo( int nType, int nIndex );
	// 获取交易宠物静态信息( nType 0为自身1为交易方, nIndex为交易栏上的index )
    
    // 是否是正在交易中的物品
	bool isInExchangeItem( int nListType, int nGrid );
	// 是否是正在交易中的宠物
	bool isInExchangePet( int nListType, int nGrid );
	bool isInExchangePetByWID( int nHeightID, int nLowID );
	
	// 检查双方是否可以展开交易
	bool canExchange( const char* pszName );

	// 获取交易对象名字
	const char* getExchangeName()
	{
		return m_strExchangeName.c_str();
	}
	// 获得己方交易的金钱
	unsigned int getSrcMoney()
	{
		return m_nSrcMoney;
	}
	// 获取对方交易的金钱
	unsigned int getDesMoney()
	{
		return m_nDesMoney;
	}
	// 己方交易是否锁定
	bool isSrcLockExchange() 
	{
		return m_bSrcLockExchange;
	}
	// 对方是否交易锁定
	bool isDesLockExchange()
	{
		return m_bDesLockExchange;
	}

	bool isSrcItemlistFull();
	bool isSrcPetlistFull();

	bool isInExchange();
	bool isConfirmExchage();
	
	//tolua_end
private:
	// 结束交易
	void endExchange();
private:
	Item				m_srcItemList[EXCHG_MAX_ITEM_NUM];	// 自己交易物品列表
	Item				m_desItemList[EXCHG_MAX_ITEM_NUM];	// 目标交易物品列表
	tagSrcExchangePet	m_srcPetList[EXCHG_MAX_PET_NUM];	// 自身交易的宠物
	tagRolePet_ToLua	m_desPetList[EXCHG_MAX_PET_NUM];	// 目标交易的宠物

	unsigned int		m_nSrcMoney;						// 自己的金钱
	unsigned int		m_nDesMoney;						// 目标方金钱
	
	bool				m_bSrcLockExchange;					// 己方交易锁定
	bool				m_bDesLockExchange;					// 对方交易锁定
	std::string			m_strExchangeName;					// 交易对象名
	bool				m_bConfirmExchange;					// 是否确认交易
};//tolua_export
