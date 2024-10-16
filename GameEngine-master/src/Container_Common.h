#pragma once
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "Item.h"

//tolua_begin
enum ECONTAINER_TYPE 
{
	CONTAINER_TYPE_AMM		= 0, 	/* 弹药包 */
	CONTAINER_TYPE_STUFF	= 1, 	/* 材料包 */
	CONTAINER_TYPE_TASK		= 2, 	/* 任务包 */
	CONTAINER_TYPE_SUNDRIES = 3, 	/* 杂物包 */
	CONTAINER_TYPE_TMP		= 4, 	/* 临时包 */
	CONTAINER_TYPE_STASH	= 5, 	/* 仓库 */
	CONTAINER_TYPE_WEAR		= 6, 	/* 身上 */
	CONTAINER_TYPE_MARKET	= 9,	/* 商城 */
	CONTAINER_TYPE_EXTEND_STASH	= 10,	/*扩充仓库*/
	CONTAINER_TYPE_EXTEND	= 11,		/*扩充背包栏*/
	CONTAINER_TYPE_SPAR		= 12,		/*晶石背包*/
	CONTAINER_TYPE_DROP		= -1, 	/* 丢弃 */

	CONTAINER_TYPE_END		= 13,
};

#define MAX_SPAR_PKG_TOLUA MAX_SPAR_PKG

struct tagAmmContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	Item	m_ItemData[MAX_AMM_PACKAGE];
	tagAmmContainer()
	{
		m_nMaxNum = 0;
	}
};

struct tagStuffContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	Item	m_ItemData[MAX_STUFF_PACKAGE];
	tagStuffContainer()
	{
		m_nMaxNum	= 0;
	}
};

struct tagTaskContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	Item	m_ItemData[MAX_TASK_PACKAGE];
	tagTaskContainer()
	{
		m_nMaxNum	= 0;
	}
};

struct tagSundriesContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	Item	m_ItemData[MAX_SUNDRIES_PACKAGE];
	tagSundriesContainer()
	{
		m_nMaxNum	= 0;
	}
};

struct tagTemContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	int		m_nTimeEnd;
	Item	m_ItemData[MAX_TMP_PACKAGE];
	tagTemContainer()
	{
		m_nMaxNum = 0;
	}
};

struct tagStashContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	int		m_nMoney;
	Item	m_ItemData[MAX_STASH_PACKAGE];
	tagStashContainer()
	{
		m_nMaxNum	= 0;
	}
};

struct tagMarketContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
// 	int		m_nMoney;
	Item	m_ItemData[MAX_STASH_PACKAGE];
	tagMarketContainer()
	{
		m_nMaxNum	= 0;
	}
};

struct tagExtendContainer
{
	int  	m_nMaxNum;
	int		m_nNum;
	Item	m_ItemData[MAX_EXTEND_SUND_PKG];
	int		m_nStuffNum;
	tagExtendContainer()
	{
		m_nMaxNum	= 0;
	}
};

struct tagBootyInfo
{
	int		id;
	int		m_nNum;
	Item	m_ItemData[MAX_MONBOOTY_ITEM];
};

struct tagMarketInfo
{
	int		id;
	int		m_nNum;
	Item	m_ItemData[MAX_SHOPPING_PACKAGE];
};

struct tagSparContainer
{
	int		id;
	int		m_nNum;
	int		m_nMaxNum;
	Item	m_ItemData[MAX_SPAR_PKG];
	tagSparContainer()
	{
		m_nMaxNum	= 0;
	}
};

//tolua_end

#define DECLARE_CONTAINER(class_targer)\
	public:\
		virtual void	intContainer( int nMaxNum, int nNum, int nListType );\
		virtual void	setContainerData( int type, int nGridIndex, int nItemType, void* pItemDate );\
		virtual Item&	getItem( int GridIndex );\
		virtual int		getContainerMaxNum();\
		virtual int		getContainerItemCount( int nId );\
		virtual void	setContainerMaxNum( int nMaxNum );\
		virtual void	setArmChange( const ROLEARM& arm, bool bSetCount = true );\
		virtual void	setItemChange( const ROLEITEM& item, bool bSetCount = true );\
		virtual bool	canDragIn( int nItemId );\
	public:\
		class_targer	m_Container;

#define IMPLEMENT_CONTAINER(class_name,class_targer)\
	void class_name::intContainer( int nMaxNum, int nNum, int nListType )\
	{\
		m_Container.m_nMaxNum	= nMaxNum;\
		m_Container.m_nNum		= nNum;\
		for( int i = 0; i < sizeof( m_Container.m_ItemData )/sizeof(Item); i++ )\
		{\
			if( i < m_Container.m_nMaxNum )\
			{\
				m_Container.m_ItemData[i].m_ItemType = ITEM_SLOTTYPE_NONE;\
			}else{\
				m_Container.m_ItemData[i].m_ItemType = ITEM_SLOTTYPE_DISABLE;\
			}\
			m_Container.m_ItemData[i].m_ListType = nListType;\
		}\
	}\
	void class_name::setContainerData( int type, int nGridIndex, int nItemType, void* pItemDate )\
	{\
		if( nGridIndex > m_Container.m_nMaxNum || nGridIndex < 0 ) return;\
		m_Container.m_ItemData[nGridIndex].m_ListType = type;\
		m_Container.m_ItemData[nGridIndex].setItemDate( nItemType, pItemDate );\
		if( type == CONTAINER_TYPE_SUNDRIES || type == CONTAINER_TYPE_STUFF || type == CONTAINER_TYPE_TASK )\
		{\
			Container::getSingleton().setItemCount( m_Container.m_ItemData[nGridIndex].getItemId(),\
				getContainerItemCount( m_Container.m_ItemData[nGridIndex].getItemId() ) );\
		}\
	}\
	Item& class_name::getItem( int GridIndex )\
	{\
		return m_Container.m_ItemData[GridIndex];\
	}\
	int class_name::getContainerMaxNum()\
	{\
		return m_Container.m_nMaxNum;\
	}\
	int class_name::getContainerItemCount( int nId )\
	{\
		int nTotalNum = 0;\
		for( int i = 0; i < m_Container.m_nMaxNum; i++ )\
		{\
			if( getItem(i).getItemId() == nId )\
			{\
				nTotalNum += getItem(i).getNum();\
			}\
		}\
		return nTotalNum;\
	}\
	void class_name::setContainerMaxNum( int nMaxNum )\
	{\
		m_Container.m_nMaxNum = nMaxNum;\
	}\
	void class_name::setArmChange( const ROLEARM& arm, bool bSetCount )\
	{\
		m_Container.m_ItemData[arm.GridIdx] = arm;\
		if( arm.Num == 0 )\
			m_Container.m_ItemData[arm.GridIdx].m_ItemType = ITEM_SLOTTYPE_NONE;\
		else\
			m_Container.m_ItemData[arm.GridIdx].m_ItemType = ITEM_SLOTTYPE_AMM;\
	}\
	void class_name::setItemChange( const ROLEITEM& item, bool bSetCount)\
	{\
		m_Container.m_ItemData[item.GridIdx] = item;\
		if( item.Num == 0 )\
			m_Container.m_ItemData[item.GridIdx].m_ItemType = ITEM_SLOTTYPE_NONE;\
		else\
			m_Container.m_ItemData[item.GridIdx].m_ItemType = ITEM_SLOTTYPE_ITEM;\
	}

class ContainerData
{
public:
	virtual void			intContainer( int nMaxNum, int nNum, int nListType ) = 0;
	virtual void			setContainerData( int type, int nGridIndex, int nItemType, void* pItemDate ) = 0;
	virtual Item&			getItem( int GridIndex ) = 0;
	virtual int				getContainerMaxNum() = 0;
	virtual int				getContainerItemCount( int nId ) = 0;
	virtual void			setContainerMaxNum( int nMaxNum ) = 0;
	virtual void			setTmpContainerEndTime( int nTimeEnd ) {};
	virtual void			setStashMoney( unsigned int uMoney ) {};
	virtual unsigned int	getStashMoney() { return 0; }
	virtual int				getAmmCount( int nAmmType ) { return 0; }
	virtual void			setArmChange( const ROLEARM& arm, bool bSetCount = true ) = 0;
	virtual void			setItemChange( const ROLEITEM& item, bool bSetCount = true ) = 0;
	virtual bool			canDragIn( int nItemId ) = 0;
	virtual void			setStuffNum( int nNum ){};
	virtual int				getStuffNum(){return 0;};

};		

/* 弹药包 */
class Container_Amm : public ContainerData
{
public:
	Container_Amm()		{};
	~Container_Amm()	{};
	
	virtual int		getAmmCount( int nAmmType );
	DECLARE_CONTAINER( tagAmmContainer )
};

/* 材料包 */
class Container_Stuff : public ContainerData
{
public:
	Container_Stuff()	{};
	~Container_Stuff()	{};

	DECLARE_CONTAINER( tagStuffContainer )
};

/* 任务包 */
class Container_Task : public ContainerData
{
public:
	Container_Task()		{};
	~Container_Task()		{};

	DECLARE_CONTAINER( tagTaskContainer )
};

/* 杂物包 */
class Container_Sundries : public ContainerData
{
public:
	Container_Sundries()		{};
	~Container_Sundries()		{};

	DECLARE_CONTAINER( tagSundriesContainer )
};

/* 仓库 */
class Container_Stash : public ContainerData
{
public:
	Container_Stash()		{};
	~Container_Stash()		{};

	virtual void			setStashMoney( unsigned int uMoney ); 
	virtual unsigned int	getStashMoney();
	DECLARE_CONTAINER( tagStashContainer )
};

/* 临时包 */
class Container_Tem : public ContainerData
{
public:
	Container_Tem()		{};
	~Container_Tem()	{};
	
	virtual void setTmpContainerEndTime( int nTimeEnd );
	DECLARE_CONTAINER( tagTemContainer )
};

//商城背包
class Container_Market : public ContainerData
{
public:
	Container_Market()	{};
	~Container_Market()	{};

	DECLARE_CONTAINER( tagMarketContainer )
};

// 扩充背包栏
class Container_Extend : public ContainerData
{
public:
	Container_Extend() {};
	~Container_Extend() {};
	
	virtual void setStuffNum( int nNum );
	virtual int getStuffNum();
	DECLARE_CONTAINER( tagExtendContainer )
};

class Container_ExtendStash : public ContainerData
{
public:
	Container_ExtendStash() {};
	~Container_ExtendStash() {};
	DECLARE_CONTAINER( tagExtendContainer )
};

// 晶石背包
class Container_Spar : public ContainerData
{
public:
	Container_Spar() {};
	~Container_Spar() {};
	DECLARE_CONTAINER( tagSparContainer )
};