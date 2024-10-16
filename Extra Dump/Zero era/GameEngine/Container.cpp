#include "Container.h"
#include "OgreLog.h"
#include "ServerTimer.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "Equip.h"
#include "GameMsgPrint.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameDBData.h"
#include "GameActorManager.h"
#include "ShowActor.h"
#include "OgreScriptLuaVM.h"
#include "GameMap.h"
#include "WorldCity.h"
#include <Windows.h>

static Item nonItem;

const tagGemPagDef_ToLua*	getGemPagDef( int nId )
{
	static_assert( sizeof( tagGemPagDef_ToLua ) == sizeof( tagGemPagDef ) );
	const tagGemPagDef_ToLua* pDef = (const tagGemPagDef_ToLua*)DBDataManager::getSingleton().getGemPagDef( nId );
	return (const tagGemPagDef_ToLua*)pDef;
}

const tagArmTeshuDef_ToLua*  getArmTeshuDef( int nId )
{
	static_assert( sizeof( tagArmTeshuDef_ToLua ) == sizeof( tagArmTeshuDef ) );
	const tagArmTeshuDef_ToLua* pDef = (const tagArmTeshuDef_ToLua*)DBDataManager::getSingleton().getArmTeshuDef( nId );
	return (const tagArmTeshuDef_ToLua*)pDef;
}

Container::Container(void):m_nCurBootyId(0),m_bPre(false)
{
	INetworkManager::getSingleton().RegisterMsgHandler( PACKAGE_INFO, this );
	INetworkManager::getSingleton().RegisterMsgHandler( ITEM_COOL_INFO, this );
	INetworkManager::getSingleton().RegisterMsgHandler( SAFE_LOCK_SVR, this );

	memset(m_pContainer, 0, sizeof(m_pContainer));
	m_pContainer[CONTAINER_TYPE_AMM]		= new Container_Amm;
	m_pContainer[CONTAINER_TYPE_STUFF]		= new Container_Stuff;
	m_pContainer[CONTAINER_TYPE_TASK]		= new Container_Task;
	m_pContainer[CONTAINER_TYPE_SUNDRIES]	= new Container_Sundries;
	m_pContainer[CONTAINER_TYPE_TMP]		= new Container_Tem;
	m_pContainer[CONTAINER_TYPE_STASH]		= new Container_Stash;
	m_pContainer[CONTAINER_TYPE_MARKET]		= new Container_Market;
	m_pContainer[CONTAINER_TYPE_EXTEND]		= new Container_Extend;
	m_pContainer[CONTAINER_TYPE_EXTEND_STASH] = new Container_ExtendStash;
	m_pContainer[CONTAINER_TYPE_SPAR]		= new Container_Spar;
}

Container::~Container(void)
{	
	delete m_pContainer[CONTAINER_TYPE_AMM];
	delete m_pContainer[CONTAINER_TYPE_STUFF];		
	delete m_pContainer[CONTAINER_TYPE_TASK];		
	delete m_pContainer[CONTAINER_TYPE_SUNDRIES];	
	delete m_pContainer[CONTAINER_TYPE_TMP];		
	delete m_pContainer[CONTAINER_TYPE_STASH];	
	delete m_pContainer[CONTAINER_TYPE_MARKET];
	delete m_pContainer[CONTAINER_TYPE_EXTEND];
	delete m_pContainer[CONTAINER_TYPE_EXTEND_STASH];
	delete m_pContainer[CONTAINER_TYPE_SPAR];
	INetworkManager::getSingleton().unRegisterMsgHander( PACKAGE_INFO );
	INetworkManager::getSingleton().unRegisterMsgHander( ITEM_COOL_INFO );
	INetworkManager::getSingleton().unRegisterMsgHander( SAFE_LOCK_SVR );
}

void Container::checkAddNewItem( const ROLEPACKAGECHG& Packge_Info_Change )
{
	std::map<int,int> newItemNumMap;
	std::map<int,int> oldItemNumMap;

 	int nArmChangeNum = Packge_Info_Change.ArmChg.ChgNum;
 	for( int i = 0 ; i < nArmChangeNum ; i++ )
	{
		CSROLEARM arm = Packge_Info_Change.ArmChg.Arms[i];
		if( newItemNumMap.find( arm.Arm.DefID ) == newItemNumMap.end() )
		{
			newItemNumMap[arm.Arm.DefID] = arm.Arm.Num;
		}else{
			newItemNumMap[arm.Arm.DefID] += arm.Arm.Num;
		}
		Item& item = nonItem;
		if( arm.ListType == CONTAINER_TYPE_WEAR )
		{
			MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
			item = pMain->getEquip().getItem( arm.Arm.GridIdx );			
		}else{
			item = getItem( arm.ListType,arm.Arm.GridIdx );
			char szScript[256];
			sprintf( szScript,"AddToBetterEquip(%d,%d,%d)", arm.Arm.DefID, arm.ListType, arm.Arm.GridIdx );
			ActorManager::getSingleton().getScriptVM()->callString(szScript);
		}
		if( item.m_ItemType != ITEM_SLOTTYPE_NONE )
		{
			if( oldItemNumMap.find( item.getItemId()) == oldItemNumMap.end() )
			{
				oldItemNumMap[item.getItemId()] = item.getNum();
			}else{
				oldItemNumMap[item.getItemId()] += item.getNum();
			}
		}
	}

	int nItemChangeNum = Packge_Info_Change.ItemChg.ChgNum;
	for( int i = 0 ; i < nItemChangeNum ; i++ )
	{
		CSROLEITEM roleItem = Packge_Info_Change.ItemChg.Items[i];
		if( roleItem.ListType == CONTAINER_TYPE_SPAR )
		{
			continue;
		}
		if( newItemNumMap.find( roleItem.Item.DefID ) == newItemNumMap.end() )
		{
			newItemNumMap[roleItem.Item.DefID] = roleItem.Item.Num;
		}else{
			newItemNumMap[roleItem.Item.DefID] += roleItem.Item.Num;
		}
		Item item = getItem( roleItem.ListType,roleItem.Item.GridIdx );
		if( item.m_ItemType != ITEM_SLOTTYPE_NONE )
		{
			if( oldItemNumMap.find( item.getItemId()) == oldItemNumMap.end() )
			{
				oldItemNumMap[item.getItemId()] = item.getNum();
			}else{
				oldItemNumMap[item.getItemId()] += item.getNum();
			}
		}
	}
	for( std::map<int,int>::iterator it = newItemNumMap.begin();it!=newItemNumMap.end();it++ )
	{
		if( it->second > oldItemNumMap[it->first] )
		{
			char szScript[256];
			int nGrid = 0,nListType = 0;
			for( int i = 0 ; i < nItemChangeNum ; i++ )
			{
				if( Packge_Info_Change.ItemChg.Items[i].Item.DefID == it->first )
				{
					nListType = Packge_Info_Change.ItemChg.Items[i].ListType;
					nGrid = Packge_Info_Change.ItemChg.Items[i].Item.GridIdx;
				}
			}
			sprintf( szScript,"OnAddNewItem(%d,%d,%d)",it->first,nListType,nGrid );
			ActorManager::getSingleton().getScriptVM()->callString(szScript);
		}
	}
}

int	Container::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	// 获取背包全部信息
	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_INFO_ALL )
	{
		m_ItemCount.clear();
		const ROLEPACKAGE& Packge_Info_All = msgbody.PackageInfo.PackageInfoData.PackageAll;
		// 弹药包
		m_pContainer[CONTAINER_TYPE_AMM]->intContainer( Packge_Info_All.AmmPackage.MaxNum, 
			Packge_Info_All.AmmPackage.Num,
			CONTAINER_TYPE_AMM );
		for( int i = 0; i < Packge_Info_All.AmmPackage.Num; i++ )
		{
			m_pContainer[CONTAINER_TYPE_AMM]->setContainerData( CONTAINER_TYPE_AMM,
				Packge_Info_All.AmmPackage.RoleItems[i].GridIdx, 
				GRID_ITEM_ITEM, 
				(void*)&Packge_Info_All.AmmPackage.RoleItems[i] );
		}
		// 材料包
		m_pContainer[CONTAINER_TYPE_STUFF]->intContainer( Packge_Info_All.StuffPackage.MaxNum,
			Packge_Info_All.StuffPackage.Num,
			CONTAINER_TYPE_STUFF );
		for( int i = 0; i < Packge_Info_All.StuffPackage.Num; i++ )
		{
			m_pContainer[CONTAINER_TYPE_STUFF]->setContainerData( CONTAINER_TYPE_STUFF,
				Packge_Info_All.StuffPackage.RoleItems[i].GridIdx, 
				GRID_ITEM_ITEM, 
				(void*)&Packge_Info_All.StuffPackage.RoleItems[i] );
		}
		// 任务包
		m_pContainer[CONTAINER_TYPE_TASK]->intContainer( Packge_Info_All.TaskPackage.MaxNum, 
			Packge_Info_All.TaskPackage.Num,
			CONTAINER_TYPE_TASK );
		for( int i = 0; i < Packge_Info_All.TaskPackage.Num; i++ )
		{
			m_pContainer[CONTAINER_TYPE_TASK]->setContainerData( CONTAINER_TYPE_TASK,
				Packge_Info_All.TaskPackage.RoleItems[i].GridIdx, 
				GRID_ITEM_ITEM, 
				(void*)&Packge_Info_All.TaskPackage.RoleItems[i] );
		}

		void* p			= NULL;
		int   nGrids	= 0;
		// 杂物包
		m_pContainer[CONTAINER_TYPE_SUNDRIES]->intContainer( Packge_Info_All.SundriesPackage.MaxNum, 
			Packge_Info_All.SundriesPackage.Num,
			CONTAINER_TYPE_SUNDRIES );
		for( int i = 0; i < Packge_Info_All.SundriesPackage.Num; i++ )
		{
			if( Packge_Info_All.SundriesPackage.RoleGrids[i].Type == GRID_ITEM_ITEM )
			{
				p		= (void*)&Packge_Info_All.SundriesPackage.RoleGrids[i].GridData.RoleItem;
				nGrids	= Packge_Info_All.SundriesPackage.RoleGrids[i].GridData.RoleItem.GridIdx;
			}else{
				p		= (void*)&Packge_Info_All.SundriesPackage.RoleGrids[i].GridData.RoleArm;
				nGrids	= Packge_Info_All.SundriesPackage.RoleGrids[i].GridData.RoleArm.GridIdx;
			}
			m_pContainer[CONTAINER_TYPE_SUNDRIES]->setContainerData( CONTAINER_TYPE_SUNDRIES,
				nGrids, 
				Packge_Info_All.SundriesPackage.RoleGrids[i].Type, 
				p );
		}

		// 仓库
		m_pContainer[CONTAINER_TYPE_STASH]->intContainer( Packge_Info_All.StashPackage.MaxNum, 
			Packge_Info_All.StashPackage.Num,
			CONTAINER_TYPE_STASH );
		for( int i = 0; i < Packge_Info_All.StashPackage.Num; i++ )
		{
			if( Packge_Info_All.StashPackage.RoleGrids[i].Type == GRID_ITEM_ITEM )
			{
				p		= (void*)&Packge_Info_All.StashPackage.RoleGrids[i].GridData.RoleItem;
				nGrids	= Packge_Info_All.StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx;
			}else{
				p		= (void*)&Packge_Info_All.StashPackage.RoleGrids[i].GridData.RoleArm;
				nGrids	= Packge_Info_All.StashPackage.RoleGrids[i].GridData.RoleArm.GridIdx;
			}
			m_pContainer[CONTAINER_TYPE_STASH]->setContainerData( CONTAINER_TYPE_STASH,
				nGrids, 
				Packge_Info_All.StashPackage.RoleGrids[i].Type, 
				p );
		}
		m_pContainer[CONTAINER_TYPE_STASH]->setStashMoney( Packge_Info_All.StashPackage.Money );

		// 商城临时包
		m_pContainer[CONTAINER_TYPE_MARKET]->intContainer( Packge_Info_All.ShoppingPackage.MaxNum, 
			Packge_Info_All.ShoppingPackage.Num,
			CONTAINER_TYPE_MARKET );
		for( int i = 0; i < Packge_Info_All.ShoppingPackage.Num; i++ )
		{
			if( Packge_Info_All.ShoppingPackage.RoleGrids[i].Type == GRID_ITEM_ITEM )
			{
				p		= (void*)&Packge_Info_All.ShoppingPackage.RoleGrids[i].GridData.RoleItem;
				nGrids	= Packge_Info_All.ShoppingPackage.RoleGrids[i].GridData.RoleItem.GridIdx;
			}else{
				p		= (void*)&Packge_Info_All.ShoppingPackage.RoleGrids[i].GridData.RoleArm;
				nGrids	= Packge_Info_All.ShoppingPackage.RoleGrids[i].GridData.RoleArm.GridIdx;
			}
			m_pContainer[CONTAINER_TYPE_MARKET]->setContainerData( CONTAINER_TYPE_MARKET,
				nGrids, 
				Packge_Info_All.ShoppingPackage.RoleGrids[i].Type, 
				p );
		}

		//背包扩充栏
		m_pContainer[CONTAINER_TYPE_EXTEND]->intContainer( Packge_Info_All.EtSundPkg.MaxNum, 
			Packge_Info_All.EtSundPkg.Num,
			CONTAINER_TYPE_EXTEND );		
		for( int i = 0; i < Packge_Info_All.EtSundPkg.Num; i++ )
		{
			p		= (void*)&Packge_Info_All.EtSundPkg.RoleItems[i];
			nGrids	= Packge_Info_All.EtSundPkg.RoleItems[i].GridIdx;
			m_pContainer[CONTAINER_TYPE_EXTEND]->setContainerData( CONTAINER_TYPE_EXTEND,
				nGrids, 
				GRID_ITEM_ITEM, 
				p );
		}		
		m_pContainer[CONTAINER_TYPE_EXTEND]->setStuffNum( Packge_Info_All.EtSundPkg.StuffNum );

		//仓库扩充栏
		m_pContainer[CONTAINER_TYPE_EXTEND_STASH]->intContainer( Packge_Info_All.EtStashPkg.MaxNum ,
			Packge_Info_All.EtStashPkg.Num,
			CONTAINER_TYPE_EXTEND_STASH );
		for( int i = 0 ; i < Packge_Info_All.EtStashPkg.Num ; i ++ )
		{
			p		= (void*)&Packge_Info_All.EtStashPkg.RoleItems[i];
			nGrids	= Packge_Info_All.EtStashPkg.RoleItems[i].GridIdx;
			m_pContainer[CONTAINER_TYPE_EXTEND_STASH]->setContainerData( CONTAINER_TYPE_EXTEND_STASH,
				nGrids, 
				GRID_ITEM_ITEM, 
				p );
		}

		// 晶石背包
		m_pContainer[CONTAINER_TYPE_SPAR]->intContainer( Packge_Info_All.SparPkg.MaxNum,
			Packge_Info_All.SparPkg.Num, CONTAINER_TYPE_SPAR );
		for( int i = 0 ; i < Packge_Info_All.SparPkg.Num ; i++ )
		{			
			p = (void*)&Packge_Info_All.SparPkg.RoleItems[i];
			nGrids	= Packge_Info_All.SparPkg.RoleItems[i].GridIdx;
			m_pContainer[CONTAINER_TYPE_SPAR]->setContainerData( CONTAINER_TYPE_SPAR,
				nGrids, 
				GRID_ITEM_ITEM, 
				p );
		}

		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

		// TODO:宠物修改
		// 宠物信息
		if( pMainPlayer != NULL )
		{
			pMainPlayer->m_Pet.initPetPackage( (PETPACKAGE*)&Packge_Info_All.PetPackage );
		}

		// 初始化契约列表
		pMainPlayer->m_QiYue.setQiYueList( Packge_Info_All.QiYueInfo );

		// 仓库金钱（金币、布尔）


		// 初始化法宝信息
		if( pMainPlayer != NULL )
		{
			pMainPlayer->m_Trump.initTrumpPackage( msgbody.PackageInfo.PackageInfoData.PackageAll.FairyPackage );
		}

		CSATTRIDVAL val;
		val.StashMoney = Packge_Info_All.StashPackage.Money;
		pMainPlayer->setAttriValue( ATTR_ID_STASHMONEY, val );

		val.StashBull = Packge_Info_All.StashPackage.Bull;
		pMainPlayer->setAttriValue( ATTR_ID_STASHBULL, val );

		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
		// update UI
		GameMsgPrint::getSingleton().SendUIEvent( "GE_SKILL_LIST_UPDATE" );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_ALL_UPDATE" );
	}

	// 背包信息变化
	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_INFO_CHG )
	{
		const ROLEPACKAGECHG& Packge_Info_Change = msgbody.PackageInfo.PackageInfoData.PackageChg;
 		checkAddNewItem( Packge_Info_Change );
		int nArmChangeNum = Packge_Info_Change.ArmChg.ChgNum;
		char szScript[256];		
		for( int i = 0; i < nArmChangeNum; i++ )
		{
			const CSROLEARM& armInfo = Packge_Info_Change.ArmChg.Arms[i];
			setArmChange( armInfo.ListType, armInfo.Arm );
			sprintf( szScript, "GetNewArm( %d, %d )", armInfo.Arm.DefID, armInfo.Arm.GridIdx );
			ActorManager::getSingleton().getScriptVM()->callString( szScript );
		}

		int nItemChangeNum = Packge_Info_Change.ItemChg.ChgNum;
		
		for( int i = 0; i < nItemChangeNum; i++ )
		{
			const CSROLEITEM& itemInfo = Packge_Info_Change.ItemChg.Items[i];
			setItemChange( itemInfo.ListType, itemInfo.Item );
			sprintf( szScript, "GetNewItem( %d, %d )", itemInfo.Item.DefID, itemInfo.Item.GridIdx );
			ActorManager::getSingleton().getScriptVM()->callString( szScript );	
		}

		setItemChgCount( Packge_Info_Change );

		GameMsgPrint::getSingleton().SendUIEvent("GE_CONTAINER_CHG");
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );


		//CSROLEITEM roleItem = Packge_Info_Change.ItemChg;
	}

	// 物品拾取
	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_INFO_BOOTY )
	{
		const CSBOOTYINFO& svrBootyInfo = msgbody.PackageInfo.PackageInfoData.PackageBooty;
		tagBootyInfo BootyInfo;

		BootyInfo.id	 = svrBootyInfo.ID;
		BootyInfo.m_nNum = svrBootyInfo.Num;
		for( int i = 0; i < BootyInfo.m_nNum; i++ )
		{
			const ROLEGRID& oneBootyItem = svrBootyInfo.Items[i];

			if( oneBootyItem.Type == GRID_ITEM_ITEM )
			{
				BootyInfo.m_ItemData[i] = oneBootyItem.GridData.RoleItem;
			}
			else if( oneBootyItem.Type == GRID_ITEM_ARM ){
				BootyInfo.m_ItemData[i] = oneBootyItem.GridData.RoleArm;
			}
		}

		m_mapBootInfo[BootyInfo.id] = BootyInfo;
		m_nCurBootyId				= BootyInfo.id;
		GameMsgPrint::getSingleton().SendUIEvent( "GE_BOOTY_GETITEM" );
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_NOTIFY_ITEM_VIEW )
	{
		send_PickItem( msgbody.PackageInfo.PackageInfoData.DropBootyID );
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_SUNDRIES_NUM_CHG )
	{
		m_pContainer[CONTAINER_TYPE_SUNDRIES]->setContainerMaxNum( msgbody.PackageInfo.PackageInfoData.SundriesNum );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_STUFF_NUM_CHG )
	{
		m_pContainer[CONTAINER_TYPE_STUFF]->setContainerMaxNum( msgbody.PackageInfo.PackageInfoData.StuffNum );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_TASK_NUM_CHG )
	{
		m_pContainer[CONTAINER_TYPE_TASK]->setContainerMaxNum( msgbody.PackageInfo.PackageInfoData.TaskNum );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}
	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_STASH_NUM_CHG )
	{
		m_pContainer[CONTAINER_TYPE_STASH]->setContainerMaxNum( msgbody.PackageInfo.PackageInfoData.StashNum );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}
	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_ET_NUM )
	{
		const ETPKGNUMCHG& etNum = msgbody.PackageInfo.PackageInfoData.EtPkgNum;
		//扩充背包
		if( etNum.PkgType == 1 )
		{
			m_pContainer[CONTAINER_TYPE_EXTEND]->setContainerMaxNum( etNum.MaxNum );
			m_pContainer[CONTAINER_TYPE_EXTEND]->setStuffNum(etNum.StuffNum );
		}else if( etNum.PkgType == 2 ){
			m_pContainer[CONTAINER_TYPE_EXTEND_STASH]->setContainerMaxNum( etNum.MaxNum );
		}
		
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_CANGBAOTU_RESET )
	{
		const CANGBAOTUINFO& cangBaoTuInfo = msgbody.PackageInfo.PackageInfoData.CangBaoTuInfo;
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

		if( pMainPlayer != NULL )
		{
			pMainPlayer->m_CangBaoTuInfo.setCangBaoTuInfo( cangBaoTuInfo);
		}

		GameMsgPrint::getSingleton().SendUIEvent( "GE_CANGBAO_UPDATE" );
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_USE_PRE_BRK )
	{
		const PACKAGEUSEBRK& usePreBreakInfo = msgbody.PackageInfo.PackageInfoData.UsePreBrk;
		
		/*if ( usePreBreakInfo.IsBreak == 0 )
		{
			if ( canBenginItemPre() )
			{
				Item& preItem = getItem( usePreBreakInfo.ListType, usePreBreakInfo.GridIdx );
				beginItemPre( preItem );
			}
			
		}*/
	}

	if( msgid == PACKAGE_INFO && msgbody.PackageInfo.Type == PACKAGE_SPAR_NUM_CHG )
	{
		char szScript[256];
		int oldSparNum = m_pContainer[CONTAINER_TYPE_SPAR]->getContainerMaxNum();
		if ( oldSparNum == 0 || oldSparNum < msgbody.PackageInfo.PackageInfoData.SparNum )
		{
			sprintf(szScript, "GetNewSparNum()");
			ActorManager::getSingleton().getScriptVM()->callString( szScript );
		}
		m_pContainer[CONTAINER_TYPE_SPAR]->setContainerMaxNum( msgbody.PackageInfo.PackageInfoData.SparNum );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
	}
	
	if( msgid == ITEM_COOL_INFO )
	{
		for( int i = 0; i < msgbody.ItemCoolInfo.ItemSelfCoolNum; i++ )
		{
			int nItemCoolId = msgbody.ItemCoolInfo.ItemSelfCools[i].ItemID;
			int nItemCDLeft = msgbody.ItemCoolInfo.ItemSelfCools[i].Left;
			tagItemDef itemDef = DBDataManager::getSingleton().getItemDef( nItemCoolId );
			for( int j = 0; j < msgbody.ItemCoolInfo.ItemSpeCoolNum; j++ )
			{
				int nItemSpeCoolType = msgbody.ItemCoolInfo.ItemSpeCools[j].ItemCoolType;
				int nItemSpcCoolLeft = msgbody.ItemCoolInfo.ItemSpeCools[j].Left; 
				if( nItemSpeCoolType == 0 )
				{
					continue;
				}

				if( nItemSpeCoolType == itemDef.SelfSpeCoolType && nItemCDLeft < nItemSpcCoolLeft )
				{
					nItemCDLeft = nItemSpcCoolLeft;
				}
				setItemTypeCD( nItemSpeCoolType, ( nItemSpcCoolLeft*10 ) );
			}

			if( nItemCDLeft != 0 )
			{
				tagCDTime &cd = m_ItemCDMap[nItemCoolId];
				if (itemDef.SelfCool > nItemCDLeft * 10)
				{
					cd.startTime = timeGetTime() - (itemDef.SelfCool * 100 - nItemCDLeft * 1000);
					cd.LastTime = cd.startTime + itemDef.SelfCool * 100;
				}
				if( itemDef.SelfSpeCoolType != 0 )
				{
					setItemTypeCD( itemDef.SelfSpeCoolType, ( nItemCDLeft*10 ) );
				}
				GameMsgPrint::getSingleton().SendUIEvent( "GE_ITEMUSED_PASSCD" );
			}
		}
	}
	//背包安全锁
	if( msgid == SAFE_LOCK_SVR )
	{
		m_nLockFlag = msgbody.SafeLockSvr.LockFlag;
		//TODO HERE
		GameMsgPrint::getSingleton().SendUIEvent("GE_CONTAINER_LOCK_CHANGE");
	}
	return 0;
}

void Container::Update(unsigned int dtick)
{
	for( int i = 0 ; i < CONTAINER_TYPE_END; i ++ )
	{
		if( i >=CONTAINER_TYPE_WEAR && i < CONTAINER_TYPE_MARKET )
		{
			continue;
		}
		for( int j = 0 ; j < m_pContainer[i]->getContainerMaxNum() ; j++ )
		{
			m_pContainer[i]->getItem(j).Update(dtick);
		}
	}
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain != NULL )
	{
		Equip& equip = pMain->getEquip();
		for( int i = 0 ; i < EQUIP_POS_END ; i++ )
		{
			equip.getItem(i).Update(dtick);
		}
	}
}

void Container::setContainerGridInfo( int nListType, int nMaxNum )
{
	if( nListType == LIST_TYPE_SHI )
	{
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		pMainPlayer->m_Pet.setPetPackageGridInfo( nMaxNum );
	}else if( nListType < CONTAINER_TYPE_END )
	{
		m_pContainer[nListType]->setContainerMaxNum( nMaxNum );
	}
}

int Container::getUnusedGridNum( int nListType )
{
	int nUnusedNum = 0;
	for( int i = 0, size = m_pContainer[nListType]->getContainerMaxNum(); i < size; ++i )
	{
		ContainerData* pContainer_data = m_pContainer[nListType];
		if( pContainer_data->getItem(i).getItemId() == 0 )
		{
			++nUnusedNum;
		}
	}

	return nUnusedNum;
}

int Container::getContainerMaxNum( int nListType )
{
	if (m_pContainer[nListType] == NULL)
	{
		return 0;
	}
	return m_pContainer[nListType]->getContainerMaxNum();
}

// 从身上获取药品
Item& Container::getItem( int id )
{
	nonItem.m_ItemType = ITEM_SLOTTYPE_NONE;
	for( int i = 0; i < m_pContainer[CONTAINER_TYPE_SUNDRIES]->getContainerMaxNum(); i++ )
	{
		ContainerData* pContainer_data = m_pContainer[CONTAINER_TYPE_SUNDRIES];
		if( pContainer_data->getItem(i).getItemId() == id )
		{
			pContainer_data->getItem(i).m_ListType = CONTAINER_TYPE_SUNDRIES;
			return pContainer_data->getItem(i);
		}
	}

	for( int i = 0; i < m_pContainer[CONTAINER_TYPE_STUFF]->getContainerMaxNum(); i++ )
	{
		ContainerData* pContainer_data = m_pContainer[CONTAINER_TYPE_STUFF];
		if( pContainer_data->getItem(i).getItemId() == id )
		{
			pContainer_data->getItem(i).m_ListType = CONTAINER_TYPE_STUFF;
			return pContainer_data->getItem(i);
		}
	}

	for ( int i = 0, nMaxNum = m_pContainer[CONTAINER_TYPE_TASK]->getContainerMaxNum(); i < nMaxNum; ++i )
	{
		ContainerData* pContainer_data = m_pContainer[CONTAINER_TYPE_TASK];
		if ( pContainer_data->getItem( i ).getItemId() == id )
		{
			pContainer_data->getItem( i ).m_ListType = CONTAINER_TYPE_TASK;
			return pContainer_data->getItem(i);
		}
	}

	return nonItem;
}

int Container::getItemCount( int Id )
{
	std::map<int, int>::iterator iter = m_ItemCount.find( Id );
	if( iter != m_ItemCount.end() )
	{
		return iter->second;
	}

	return 0;
}

// 获取物品的总数量
int Container::getItemTotalNum( int nId )
{
	int nTotalNum = 0;
	nTotalNum += m_pContainer[CONTAINER_TYPE_STUFF]->getContainerItemCount( nId );
	nTotalNum += m_pContainer[CONTAINER_TYPE_TASK]->getContainerItemCount( nId );
	nTotalNum += m_pContainer[CONTAINER_TYPE_SUNDRIES]->getContainerItemCount( nId );
	return nTotalNum;
}

Item& Container::getItem( int nListType, int GridIndex )
{
	if( ( nListType < 0 || nListType >= CONTAINER_TYPE_END ) || ( GridIndex < 0 || GridIndex >= getContainerMaxNum(nListType) ) )
	{
		assert(0);
		nonItem.m_ItemType = ITEM_SLOTTYPE_NONE;
		return nonItem;
	}
	return m_pContainer[nListType]->getItem( GridIndex );
}

Item* Container::getItem(tdr_ulonglong WID)
{
	for (size_t i = 0; i < CONTAINER_TYPE_END; ++i)
	{
		for (size_t j = 0; j < getContainerMaxNum(i); ++j)
		{
			Item &item = m_pContainer[i]->getItem(j);
			if (item.getItemType() != ITEM_SLOTTYPE_NONE && item.getItemType() != ITEM_SLOTTYPE_DISABLE &&
				item.getItemWId() == WID)
			{
				return &item;
			}
		}
	}
	return NULL;
}

bool Container::getItemLink( int nListType, int GridIndex )
{
	Item& ItemLink = getItem( nListType, GridIndex );
	if( ItemLink.getItemId() != 0 && ( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) ) )
	{
		GameMsgPrint::getSingleton().setItemLink( ItemLink );
		return true;
	}
	return false;
}

int  Container::getItemResultID( const tagItemDef_ToLua* pItemDef )
{
	for ( int i = 0; i < MAX_ITEM_RESULT; ++i )
	{
		const tagResult_ToLua& oneItemResult = pItemDef->Result[i];
		if ( oneItemResult.ResultID != 0 )
		{
			return oneItemResult.ResultID;
		}
	}

	return 0;
}

bool Container::canUseItemPre( Item& useItem, int nTargetID )
{
	const tagItemDef_ToLua* pItemDef = useItem.getItemDef();
	if ( pItemDef == NULL )
	{
		return false;
	}
	
	if( pItemDef->UseTarget == USE_TARGET_MONSTER )
	{
		GameActor*	pTarget	= ActorManager::getSingleton().FindActor( nTargetID );
		if ( pTarget == NULL )
		{
			return false;
		}

		if( pTarget->getType() == GAT_MONSTER && pTarget->isDead() )
		{
			return false;
		}
		
		/*if ( pTarget->getType() != GAT_MONSTER )
		{
			return false;
		}*/

		for( int i = 0 ; i < MAX_ITEM_RESULT ; i++ )
		{
			if ( pItemDef->Result[i].ResultID == RESULT_CATCH_PET )
			{
				const tagMonsterDef&  MonsterDef = DBDataManager::getSingleton().getMonsterDef( pTarget->getResId() );
				if ( &MonsterDef == NULL )
				{
					return false;
				}

				const MONBOOTY& monsterBootyDef = MonsterDef.Booty;
				if ( monsterBootyDef.BootyPetEgg == 0 )
				{
					GameMsgPrint::getSingleton().showMidTip( MID_CANNOT_CATCH_PET );
					return false;
				}

				return true;
			}
		}
		
	}

	return true;
}

void Container::useItemPre( Item& used_Item, MainGamePlayer* pMainPlayer, int nTargetIndex )
{
	if (pMainPlayer->getActState().GetState() == GSACT_COLLECTITEM)
	{
		GameMsgPrint::getSingleton().showMidTip(MID_USEITEM_INCOLLECT_FAIL);
	}
	else
	{
		if (pMainPlayer->IsAutoMove())
		{
			GameMsgPrint::getSingleton().showMidTip(MID_USEITEMPRE_WRONG_STATE);
			return;
		}
		pMainPlayer->breakPreAction();
		if( pMainPlayer->getRider()->isOnHorse() )
		{
			pMainPlayer->getRider()->requestRideDown();
		}

		tagCSPkgBody msgbody;
		msgbody.PackageReq.Type	= PACKAGE_REQ_USE_PRE;
		CSPACKAGEUSE& itemPreData = msgbody.PackageReq.PackageReqData.PackageUsePre;
		itemPreData.SrcGridIdx	= used_Item.getGridIdx();
		itemPreData.SrcListType	= used_Item.getListType();
		itemPreData.SubIndex	= 0;

		const tagItemDef_ToLua* pItemDef  = used_Item.getItemDef(); 
		if ( pItemDef == NULL )
		{
			return;
		}

		if( pItemDef->UseTarget == 0 )
		{
			itemPreData.WID = 0;
		}
		else if( pItemDef->UseTarget == USE_TARGET_PET )
		{
			tagRolePet_ToLua* pRolePet = pMainPlayer->getPet()->getPetInfo( nTargetIndex );
			if( pRolePet == NULL )
			{
				return;
			}
			tagPetId_ToLua PetId;
			PetId.nHeight	= pRolePet->WID.nHeight;
			PetId.nLow		= pRolePet->WID.nLow;
			itemPreData.WID = *((tdr_ulonglong*)&PetId);
		}else if( pItemDef->UseTarget == USE_TARGET_MONSTER || pItemDef->UseTarget == USE_TARGET_NPC || 
					pItemDef->UseTarget == USE_TARGET_PLAYER )
		{
			if ( !canUseItemPre( used_Item, nTargetIndex ) )
			{
				return;
			}

			GameActor* pTarget	= ActorManager::getSingleton().FindActor( nTargetIndex );
			//目标指定NPC
			if( pTarget != NULL )
			{
				itemPreData.WID = pTarget->GetID();
			}else{
				return;
			}
		}
		SendGameServerMsg( PACKAGE_REQ, msgbody );
		pMainPlayer->startUseItemChant( pItemDef->ItemUsePre*1000,pItemDef->ItemID,nTargetIndex,used_Item.getListType(),used_Item.getGridIdx() );
	}
}

bool Container::canUseTarget( Item& used_Item, MainGamePlayer* pMainPlayer, int nTargetIndex )
{
	char szBuf[256];
	const tagItemDef_ToLua* pItemDef = used_Item.getItemDef();
	if( pItemDef->UseTarget == USE_TARGET_MONSTER 
		|| pItemDef->UseTarget == USE_TARGET_NPC
		|| pItemDef->UseTarget == USE_TARGET_PLAYER )
	{
		GameActor*		pTarget		= ActorManager::getSingleton().FindActor( pMainPlayer->GetSelectTarget() );
		//目标指定NPC
		if( pTarget == NULL )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_NEED_TARGET );
			return false; 
		}

		if( pItemDef->UseTargetID != 0 )
		{
			const tagMonsterDef&  MonsterDef	= DBDataManager::getSingleton().getMonsterDef( pItemDef->UseTargetID );
			if( pTarget->getResId() != pItemDef->UseTargetID )
			{
				sprintf( szBuf, "AddGameMiddleTips(\"请先选中 %s！\")", MonsterDef.Name );
				ActorManager::getSingleton().getScriptVM()->callString( szBuf );
				return false;
			}
		} 
		else
		{
			if( pItemDef->UseTarget == USE_TARGET_MONSTER  && pTarget->getType() != GAT_MONSTER )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_NEED_MONSTER_TARGET );
				return false; 
			}
			else if( pItemDef->UseTarget == USE_TARGET_NPC  && pTarget->getType() != GAT_NPC )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_NEED_NPC_TARGET );
				return false; 
			}else if( pItemDef->UseTarget == USE_TARGET_PLAYER  && pTarget->getType() != GAT_PLAYER )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_NEED_NPC_PLAYER );
				return false; 
			}else if( pItemDef->UseTarget == USE_TARGET_PLAYER && pTarget->getType() == GAT_PLAYER )
			{
				//针对摆摊玩家的变身道具，限制不能使用，需另查找SkillDef表和StatusDef表，
				if( pTarget->isInStall() )
				{
					for( int i = 0; i < MAX_ITEM_RESULT; i++ )
					{
						if( pItemDef->Result[i].ResultID == RESULT_DO_SKILL )
						{
							const tagSkillDef & SkillDef = DBDataManager::getSingleton().GetSkillDef( pItemDef->Result[i].ResultVal1, pItemDef->Result[i].ResultVal2 );
							if( SkillDef.SkillID != 0 )
							{
								for( int j = 0; j < MAX_SKILL_RESULT; j++ )
								{
									if( SkillDef.SkillResult[j].SkillResultID == RESULT_STATUS )
									{
										const tagStatusDef& StatusDef = DBDataManager::getSingleton().getStatusDef( SkillDef.SkillResult[j].SkillResultVal2, SkillDef.SkillResult[j].SkillResultVal3 );
										if( StatusDef.StatusID != 0 )
										{
											for( int k = 0; k < MAX_STATUS_RESULT; k++ )
											{
												if(StatusDef.Result[k].ResultID == STATUS_RESULT_NOSKILL)
												{
													GameMsgPrint::getSingleton().showMidTip( MID_NO_CHANGE_STALL_MODEL );
													return false; 
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		int radius = pItemDef->MaxUseDist;
		if (radius <= 0)
		{
			radius = 500;
		}
		GameXYZ selfPos = pMainPlayer->GetPosition();
		GameXYZ targetPos = pTarget->GetPosition();
		targetPos.y = selfPos.y;
		if (selfPos.subtract(targetPos).length() > radius)
		{
			std::string name = ActorManager::getSingleton().replaceSpecialCode(pTarget->getName());
			sprintf( szBuf, "AddGameMiddleTips(\"距离 %s 太远！\")", name.c_str() );
			ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			return false;
		}
	}
	else if( pItemDef->UseTarget == USE_TARGET_PET )
	{
		tagRolePet_ToLua* pRolePet = pMainPlayer->getPet()->getPetInfo( nTargetIndex );
		if( pRolePet == NULL )
		{
			sprintf( szBuf, "AddGameMiddleTips(\"请先选中一只宠物！\")" );
			ActorManager::getSingleton().getScriptVM()->callString( szBuf );
			return false;
		}
	}
	return true;
};

bool Container::isExpendableItemLimitdByScene( Item& used_Item )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return true;
	}

	if ( used_Item.getItemId() == 0 )
	{
		return true;
	}

	tagItemDef_ToLua* pItemDef	= used_Item.getItemDef();
	int nMapId					= pMainPlayer->getCurMap()->getMapId();
	if (nMapId >= GAME_PWORLD_MAPID)
	{
		const tagPworldDef* pworldDef = DBDataManager::getSingleton().getPworldDef( nMapId / 100 );
		if ( pworldDef != NULL && ( pworldDef->CtrlFlag & PWORLD_NO_RESTORE_ITEM) != 0 && pItemDef->IsReStoreItem )
		{
			return true;
		}
	}

	return false;
}

bool Container::canUseExpendableItem()
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return false;
	}

	// 处于使用物品的吟唱状态 不可使用其它物品
	if( isUseItemPre() )
	{
		return false;
	}

	if ( pMainPlayer->isInFly() )
	{
		return false;
	}
	
	if ( !pMainPlayer->canControl() && pMainPlayer->m_pCarry->getCarryState() != eCarrying )
	{
		return false;
	}
	
	if (!pMainPlayer->isInMap())
	{
		return false;
	}
	
	return true;
}

bool Container::usedItem( int nListType, int GridIndex, int wearPos, int nTargetIndex )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return false;
	}
	pMainPlayer->RefreshCmdTime();
	
	// 摆摊状体不能使用物品
	if ( pMainPlayer->isInStall() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ITEM_USE_FAIL_IN_STALL );
		return false;
	}
	// 死亡状态不能使用物品
	if (pMainPlayer->getActState().GetState() == GSACT_DEAD)
	{
		GameMsgPrint::getSingleton().showMidTip( MID_SELF_DEAD );
		return false;
	}

	Item& used_Item = getItem( nListType, GridIndex );
	if ( used_Item.getItemId() == 0 )
	{
		return false;
	}
	const tagItemDef_ToLua* pItemDef	= used_Item.getItemDef();
	// 处于战斗中不可使用指定的物品
	if ((pItemDef->CtrlFlag & ITEM_FIGHT_NO_USE) && pMainPlayer->isInFight())
	{
		GameMsgPrint::getSingleton().showMidTip(MID_INFIGHT);
		return false;
	}
	// 处于使用物品的吟唱状态 不可使用其它物品
	if( !(pItemDef->CtrlFlag & ITEM_PRE_USE) && isUseItemPre() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_ITEM_USE_FAIL_INPRE );
		return false;
	}
	if( used_Item.isValidEnd() )
	{
		char buf[256];
		sprintf( buf,"ItemValidEnd(\"%s\")",used_Item.getDisplayName() );
		ActorManager::getSingleton().getScriptVM()->callString(buf);
		return false;
	}

	if(  used_Item.getItemType() == ITEM_SLOTTYPE_ITEM && pMainPlayer->isInFly() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_USEITEM_IN_FLY_STATE );
		return false;
	}
	if (!ActorManager::getSingleton().getMainPlayer()->canControl() &&
		ActorManager::getSingleton().getMainPlayer()->m_pCarry->getCarryState() != eCarrying )
	{
		return false;
	}

	if( !isInUseArea( pItemDef, pMainPlayer ) )
	{
		int nMapId = 0;
		int range = 0;
		for( int i = 0; i < MAX_ITEM_RESULT; i++ )
		{
			const tagResult_ToLua& result = pItemDef->Result[i];
			if( result.ResultID == RESULT_IN_AREA_USE )
			{
				nMapId = result.ResultVal4;
				range = result.ResultVal1;
			}
		}
		if( nMapId > 0 )
		{
			char szBuf[256];
			sprintf( szBuf, "UseItemFailedNotInMap(%d,%d)", nMapId, range );
			ActorManager::getSingleton().getScriptVM()->callString( szBuf );
		}else{
			GameMsgPrint::getSingleton().showMidTip( MID_ITEM_USE_FAIL_NOTINAREA );
		}
		return false;
	}
	if (!pMainPlayer->isInMap())
	{
		return false;
	}
	int nMapId = pMainPlayer->getCurMap()->getMapId();
	if (nMapId >= GAME_PWORLD_MAPID)
	{
		const tagPworldDef* pworldDef = DBDataManager::getSingleton().getPworldDef( nMapId / 100 );
		if (pworldDef && (pworldDef->CtrlFlag & PWORLD_NO_RESTORE_ITEM) && pItemDef->IsReStoreItem)
		{
			GameMsgPrint::getSingleton().showMidTip( MID_PWORLD_RESTOREITEM );
			return false;
		}
	}
	// 对公会的flag 处理下
	if( used_Item.getItemDef()->WearPre.ClanLimit == 1 )
	{
		if( !WorldCityManager::getSingleton().IsWorldCityKeeper(pMainPlayer) 
			|| pMainPlayer->getClanPosition() != CLAN_POS_PRESIDENT )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_USE_ITEM_FAILED_CLAN_LIMIT_CAPTION );
			return false;
		}
	}else if( used_Item.getItemDef()->WearPre.ClanLimit == 2 )
	{
		if( !WorldCityManager::getSingleton().IsWorldCityKeeper(pMainPlayer) )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_USE_ITEM_FAILED_CLAN_LIMIT_MEMBER );
			return false;
		}		
	}
	//公会城市回城卷
	if( used_Item.getItemDef()->Result[0].ResultID == RESULT_BACK_TO_CLAN_CITY )
	{
		bool bCanUse = false;
		ActorManager::getSingleton().getScriptVM()->callFunction("CanUseBackClanCityItem",">b",&bCanUse);
		if( !bCanUse )
		{
			return false;
		}
		if( pMainPlayer->isInFight() )
		{
			GameMsgPrint::getSingleton().showMidTip(MID_USE_BACK_CLAN_ITEM_FAILED_INFIGHT);
			return false;
		}
	}
	if( used_Item.isAmm() && pMainPlayer->m_Equip.checkAmmUsed( used_Item.getItemId() ) )
	{
		pMainPlayer->m_Equip.send_EquipAmm( used_Item.getItemId() );
		return true;
	}

	for( int i = 0 ; i < MAX_ITEM_RESULT ; i++ )
	{
		//对媒体礼包和抽奖次数礼包判断
		if( pItemDef->Result[i].ResultID == RESULT_MEDIA_GIFT )
		{
			char buf[256];
			sprintf( buf,"ShowMediaGiftFrame(%d,%d,%d)",nListType,GridIndex,pItemDef->Result[i].ResultVal1 );
			ActorManager::getSingleton().getScriptVM()->callString(buf);
			return true;
		}
		if( pItemDef->Result[i].ResultID == RESULT_LTR_GIFT )
		{
			char buf[256];

			sprintf( buf,"ShowLtrGiftFrame(%d,%d,%d,%d)",nListType,GridIndex,used_Item.getItemId(),pItemDef->Result[i].ResultVal1 );
			ActorManager::getSingleton().getScriptVM()->callString(buf);
			return true;
		}
		//宠物蛋
		if( pItemDef->Result[i].ResultID == RESULT_PET_HATCH )
		{
			//对格子判断
			if( pMainPlayer->getPet()->getPetNum() >= pMainPlayer->getPet()->getPetMaxNum() )
			{
				GameMsgPrint::getSingleton().showMidTip(MID_USE_PET_EGG_PET_FULL);
				return false;
			}
		}
		// 对怪物机械的处理
		if( pItemDef->Result[i].ResultID == RESULT_MACHINE_ITEM )
		{
			if( pMainPlayer->isInMorph() )
			{
				GameMsgPrint::getSingleton().showMidTip(MID_USE_MACHINE_ITEM_IN_MORPH);
				return false;
			}
		}
		// 对变身药水的判断
		if( pItemDef->Result[i].ResultID == RESULT_SELF_STATUS )
		{
			const STATUSDEF& statusDef = DBDataManager::getSingleton().getStatusDef( pItemDef->Result[i].ResultVal2, pItemDef->Result[i].ResultVal3 );
			if( statusDef.ModeID > 0 )
			{
				if( pMainPlayer->isOnMonsterMachine() )
				{
					GameMsgPrint::getSingleton().showMidTip(MID_USE_MORPH_ITEM_IN_MACHINE);
					return false;
				}
			}
		}
		//对队员召唤卷轴进行判断
		if( pItemDef->Result[i].ResultID == RESULT_TEAM_SUMMON )
		{
			if (  !pMainPlayer->isCaptain() )
			{
				GameMsgPrint::getSingleton().showMidTip(MID_SUMMON_MUST_BE_CAPTAIN);
				return false;
			}
			else if ( pItemDef->Result[i].ResultVal1 > 0 )
			{
				//战场召唤卷使用判断
				int mapid = pMainPlayer->getMapRealId();
				if( mapid >= GAME_PWORLD_MAPID )
				{
					int nPworldId = mapid/100;
					const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
					if( pPworldDef == NULL || pPworldDef->BattleID != pItemDef->Result[i].ResultVal1 )
					{
						char buff[256];
						sprintf( buff, "ShowTeamSummonUseMid(%d)", pItemDef->Result[i].ResultVal1 );
						ActorManager::getSingleton().getScriptVM()->callString( buff );	
						return false;
					}
				}
				else
				{
					char buff[256];
					sprintf( buff, "ShowTeamSummonUseMid(%d)", pItemDef->Result[i].ResultVal1 );
					ActorManager::getSingleton().getScriptVM()->callString( buff );	
					return false;
				}
			}
		}
		//空间记忆器道具使用
		if ( pItemDef->Result[i].ResultID == RESULT_MAP_RECORDER )
		{
			char buff[256];
			sprintf( buff, "GetItemRecorderInfo( %d,%d)", used_Item.getItemId(),used_Item.getGridIdx());
			ActorManager::getSingleton().getScriptVM()->callString( buff );	
			return false;
		}
		//对公会召唤卷轴进行判断
		if( pItemDef->Result[i].ResultID == RESULT_CLAN_SUMMON )
		{
			if ( pMainPlayer->getClanPosition() == CLAN_POS_PRESIDENT ||
					pMainPlayer->getClanPosition() == CLAN_POS_VICE_PRESIDENT )
			{
				int mapid = pMainPlayer->getMapRealId();
				const tagMapDef& pMapDef = DBDataManager::getSingleton().getMapDef( mapid );
				if ( &pMapDef == NULL || pMapDef.CtrlFlag & MAP_CTRL_NO_USER_CLAN_SUMMON )//标记为2048的不能使用公会召唤卷
				{
					GameMsgPrint::getSingleton().showMidTip(MID_NO_ITEM_USE_IN_THIS_MAP);
					return false;
				}
			}
			else
			{
				GameMsgPrint::getSingleton().showMidTip(MID_SUMMON_MUST_BE_PRESIDENT);
				return false;
			}
		}
		//对宝箱道具进行判断
		if ( pItemDef->Result[i].ResultID == RESULT_BAOXIANG )
		{
			pMainPlayer->RequestOpenBaoXiang( used_Item.getListType(), used_Item.getGridIdx() );
			return false;
		}
		//对幸运罗盘进行判断
		if ( pItemDef->Result[i].ResultID == RESULT_ITEM_CARDBOX )
		{
			char buff[256];
			sprintf( buff, "UseCardBoxItem(%d,%d)", used_Item.getGridIdx(), used_Item.getListType() );
			ActorManager::getSingleton().getScriptVM()->callString( buff );
			return false;
		}

		//对飞行传送道具进行判断
		if ( pItemDef->Result[i].ResultID == RESULT_ITEM_TRANS )
		{
			char buff[256];
			sprintf( buff, "UseTransItem(%d,%d)", used_Item.getGridIdx(), used_Item.getListType() );
			ActorManager::getSingleton().getScriptVM()->callString( buff );
			return false;
		}

		//对装备属性重置道具的判断
		if ( pItemDef->Result[i].ResultID == RESULT_BASE_ATTR_RESET )
		{
			char buff[256];
			sprintf( buff, "UseBaseAttrResetItem(%d,%d)", used_Item.getListType(), used_Item.getGridIdx() );
			ActorManager::getSingleton().getScriptVM()->callString( buff );
			return false;
		}

		//对装备技能激活道具的判断
		if ( pItemDef->Result[i].ResultID == RESULT_ARM_SKILL_ACTIVE )
		{
			char buff[256];
			sprintf( buff, "UseArmSkillActiveItem(%d,%d)", used_Item.getListType(), used_Item.getGridIdx() );
			ActorManager::getSingleton().getScriptVM()->callString( buff );
			return false;
		}
	}	

	IGameWizard::getInstance()->useContainerItem( used_Item.getItemId(),used_Item.getListType(),used_Item.getGridIdx() );

	if( used_Item.isExpendable() )
	{
		pMainPlayer->requestBreakChant( eTrump_Chant );

		if( getItemCD( used_Item.getItemId() ) == 0 )
		{
			if( ( pItemDef->UseTarget != 0 && 
				!canUseTarget( used_Item, pMainPlayer, nTargetIndex ) ) )
			{
				return false;
			}
			
			if ( pItemDef->UseTarget != 0 && pItemDef->UseTarget != USE_TARGET_PET )
			{
				nTargetIndex = pMainPlayer->GetSelectTarget();
			}

			if( pItemDef->ItemUsePre > 0 )
			{
				if( !pMainPlayer->canStartChant( eUseItem_Chant ) )
				{
					return false;
				}
				useItemPre( used_Item, pMainPlayer, nTargetIndex );
				return false;
			}

			// TODO: 再加个对宠物使用，需要将脚本中取出，统一到这儿，减少入口

			// 请求使用消耗品
			char szScript[256];
			sprintf( szScript, "UseNewItem( %d, %d )", used_Item.getItemId(), GridIndex );
			ActorManager::getSingleton().getScriptVM()->callString( szScript );
			send_UsedExpendableItem( nListType, GridIndex, nTargetIndex );
			return true;
		}
		return false;
	}

	if( used_Item.getItemId() != 0 )
	{
		int nWearPos = 0;
		if( used_Item.isRing() || used_Item.isAmuleft() || used_Item.isFashionSuit() )
		{
			nWearPos = wearPos;	
		}

		pMainPlayer->m_Equip.send_Equip( nListType, used_Item, nWearPos );
		return true;
	}

	return false;
}

bool Container::usedItem( int id )
{
	Item& used_Item = getItem( id );
	if( used_Item.canUsed() )
	{
		return usedItem( used_Item.getListType(), used_Item.getGridIdx() );
	}

	return false;
}

int Container::getAmmCount( int nAmmType )
{
	return m_pContainer[CONTAINER_TYPE_AMM]->getAmmCount( nAmmType );
}

// 获取空的背包栏位
int Container::getNoneGridIndex( int nListType, int nItemId, int nIndex, bool bBind )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	int nPos = -1;
	//优先叠加
	for( int i = 0; i < nMaxNum; i++ )
	{
		if( nPos == -1 && getItem( nListType, i ).getItemId() == 0 )
		{
			nPos = i;
		}
	}

	if( nItemId > 0 && getItemDef( nItemId )->MaxPile > 0 )
	{
		for( int i = nIndex; i < nMaxNum; i++ )
		{
			Item& item = getItem( nListType, i );
			if( item.getItemId()  == nItemId 
				&& item.getNum() < item.getItemDef()->MaxPile
				&& item.isItemBind() == bBind )
			{
				if ( item.getItemDef()->Result[0].ResultID != RESULT_GEM )
				{
					return i;
				}
				else if ( item.getItemInstVal1() == 0 && item.getItemInstVal2() == 0 )
				{
					return i;
				}
			}
		}
	}

	return nPos;
}

int Container::getBootyCount( int id )
{
	return m_mapBootInfo[id].m_nNum;
}

Item& Container::getBootyItem( int id, int GridIndex )
{
	return m_mapBootInfo[id].m_ItemData[GridIndex];
}

void Container::closeBooty( int id )
{
	std::map<int,tagBootyInfo>::iterator iter = m_mapBootInfo.find( id );
	if( iter != m_mapBootInfo.end() )
	{
		m_mapBootInfo.erase( iter );
	}
	m_nCurBootyId = 0;
}

void Container::setArmChange( int nListType, const ROLEARM& arm )
{
	if( nListType == CONTAINER_TYPE_WEAR )
	{
		MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( pMainPlayer == NULL ) return;

		pMainPlayer->m_Equip.SetArmInfo( arm.GridIdx, arm );
		GameMsgPrint::getSingleton().SendUIEvent("GE_WEAR_ARM_UPDATE");
	}else
	{
		if( nListType == CONTAINER_TYPE_STASH || nListType == CONTAINER_TYPE_TMP || nListType == CONTAINER_TYPE_MARKET )
		{
			m_pContainer[nListType]->setArmChange( arm, false );
		}else{
			m_pContainer[nListType]->setArmChange( arm );
		}
	}
}

void Container::setItemChange( int nListType, const ROLEITEM& item )
{
	if( nListType == CONTAINER_TYPE_STASH || nListType == CONTAINER_TYPE_TMP || nListType == CONTAINER_TYPE_MARKET )
	{
		m_pContainer[nListType]->setItemChange( item, false );
	}else{
		m_pContainer[nListType]->setItemChange( item );
	}
}

// 登录请求所有容器信息
void Container::send_GetInfo()
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type							= PACKAGE_REQ_ALL;
	msgbody.PackageReq.PackageReqData.PackageAll	= 0; 
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 容器物品移动
void Container::send_MoveItem( int srcList, int destList, int srcGridIndex, int srcNum, int destGridIndex )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type											= PACKAGE_REQ_MOVE;
	msgbody.PackageReq.PackageReqData.PackageMove.SrcListType		= srcList;
	msgbody.PackageReq.PackageReqData.PackageMove.DstListType		= destList;
	msgbody.PackageReq.PackageReqData.PackageMove.SrcGridIdx		= srcGridIndex;
	msgbody.PackageReq.PackageReqData.PackageMove.SrcGridNum		= srcNum;
	msgbody.PackageReq.PackageReqData.PackageMove.DstGridIdx		= destGridIndex;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 容器请求整理
void Container::send_Arranger()
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type	= PACKAGE_REQ_ARRANGE;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 金钱在容器间转移
void Container::send_MoveMoney( int srcAtterId, int destAtterId, int money )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type											= PACKAGE_REQ_MONEY;
	msgbody.PackageReq.PackageReqData.PackageMoney.SrcMoneyAttrID	= srcAtterId;
	msgbody.PackageReq.PackageReqData.PackageMoney.DstMoneyAttrID	= destAtterId;
	msgbody.PackageReq.PackageReqData.PackageMoney.MoneyNum			= money;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 关闭仓库
void Container::send_CloseStash()
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type = ADJUST_TYPE_UI;
	msgbody.AdjustC.AdjustData.UI.UIType = UI_TYPE_STASH;
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 背包整理
void Container::send_PackageArrange( int ListType )
{
	assert( ListType >= 0 && ListType <= 5 );

	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type = PACKAGE_REQ_ARRANGE;
	msgbody.PackageReq.PackageReqData.PackageArrange.ListType = ListType;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 拾取物品
void Container::send_PickItem( int id )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type								= PACKAGE_REQ_PICK;
	msgbody.PackageReq.PackageReqData.PackagePick.ID	= id;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
	GameMsgPrint::getSingleton().SendUIEvent( "GE_BOOTY_GETITEM" );
}

// 拾取全部物品
void Container::send_PickAllItem( int id )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type									= PACKAGE_REQ_BOOTY_SELECT;
	msgbody.PackageReq.PackageReqData.PackagePickSelect.ID	= id;
	msgbody.PackageReq.PackageReqData.PackagePickSelect.Idx	= -1;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
	GameMsgPrint::getSingleton().SendUIEvent( "GE_BOOTY_GET_ALL_ITEM" );
}

// 拾取选中物品
void Container::send_SelectItem( int id, int selectIndex )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type									= PACKAGE_REQ_BOOTY_SELECT;
	msgbody.PackageReq.PackageReqData.PackagePickSelect.ID	= id;
	msgbody.PackageReq.PackageReqData.PackagePickSelect.Idx	= selectIndex;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
	GameMsgPrint::getSingleton().SendUIEvent( "GE_BOOTY_GET_ONE_ITEM" );
}

// 请求使用消耗品
void Container::send_UsedExpendableItem( int nListType, int GridIndex, int nTargetIndex, int nSubIndex,int UseType )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type									 = PACKAGE_REQ_USE;
	msgbody.PackageReq.PackageReqData.PackageUse.SrcListType = nListType;	
	msgbody.PackageReq.PackageReqData.PackageUse.SrcGridIdx  = GridIndex;
	msgbody.PackageReq.PackageReqData.PackageUse.WID		 = 0;
	msgbody.PackageReq.PackageReqData.PackageUse.SubIndex	=	nSubIndex;
	msgbody.PackageReq.PackageReqData.PackageUse.UseType	=	UseType;

	int nItemId = getItem( nListType, GridIndex ).getItemId();
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( nItemId != 0 && pMainPlayer != NULL )
	{
		const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( nItemId ); 
		if( itemDef.UseTarget == 0 )
		{
			msgbody.PackageReq.PackageReqData.PackageUse.WID = 0;
		}
		else if( itemDef.UseTarget == USE_TARGET_PET )
		{
			tagRolePet_ToLua* pRolePet = pMainPlayer->getPet()->getPetInfo( nTargetIndex );
			if( pRolePet == NULL )
			{
				return;
			}
			tagPetId_ToLua PetId;
			PetId.nHeight	= pRolePet->WID.nHeight;
			PetId.nLow		= pRolePet->WID.nLow;
			msgbody.PackageReq.PackageReqData.PackageUse.WID = *((tdr_ulonglong*)&PetId);
		}
		else if( itemDef.UseTarget == USE_TARGET_MONSTER || itemDef.UseTarget == USE_TARGET_NPC 
				|| itemDef.UseTarget == USE_TARGET_PLAYER )
		{
			GameActor* pTarget = ActorManager::getSingleton().FindActor( nTargetIndex );
			//目标指定NPC
			if( pTarget != NULL )
			{
				msgbody.PackageReq.PackageReqData.PackageUse.WID = pTarget->GetID();
			}else{
				return;
			}
		}
	}
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 宠物喂养
void Container::send_PetUsedItem( int nListType, int GridIndex, int nHeightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type									 = PACKAGE_REQ_USE;
	msgbody.PackageReq.PackageReqData.PackageUse.SrcListType = nListType;	
	msgbody.PackageReq.PackageReqData.PackageUse.SrcGridIdx  = GridIndex;
	msgbody.PackageReq.PackageReqData.PackageUse.WID		 = 0;
	struct LongID{
		int nHeight;
		int nLow;
	};
	LongID ID;
	ID.nHeight	= nHeightId;
	ID.nLow		= nLowId;
	msgbody.PackageReq.PackageReqData.PackageUse.WID = *(tdr_ulonglong*)&ID;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

// 物品使用cd
void Container::passItemCD( int id )
{
	const tagItemDef& itemDef = DBDataManager::getSingleton().getItemDef( id );

	tagCDTime &coolTime = m_ItemCDMap[id];
	coolTime.startTime = timeGetTime();
	coolTime.LastTime = coolTime.startTime + ( itemDef.SelfCool > 0 ? itemDef.SelfCool : 5 ) * 100;

	if( itemDef.SelfSpeCoolType != 0 )
	{
		setItemTypeCD( itemDef.SelfSpeCoolType, itemDef.SpeCool );
	}
	GameMsgPrint::getSingleton().SendUIEvent( "GE_ITEMUSED_PASSCD" );
}

// 获取物品使用cd
float Container::getItemCD( int id )
{
	std::map<int, tagCDTime>::iterator iter = m_ItemCDMap.find( id );
	if( iter != m_ItemCDMap.end() )
	{
		return iter->second.remain() / 1000.0f;
	}

	return 0;
}

// 物品是否还在CD转动
bool Container::isItemInCD( int id )
{
	std::map<int, tagCDTime>::iterator iter = m_ItemCDMap.find( id );
	if( iter != m_ItemCDMap.end() )
	{
		return iter->second.remain() != 0 ;
	}

	return false;
}

// 获取物品使用已经经过的cd
float Container::getItemCDPassTime( int id )
{
	std::map<int, tagCDTime>::iterator iter = m_ItemCDMap.find( id );
	if( iter != m_ItemCDMap.end() )
	{
		return (float)( timeGetTime() - iter->second.startTime )/1000;
	}

	return 0;
}

// 设置物品指定类型cd
void Container::setItemTypeCD( int type, int cd )
{
	for( int i = 0; i < m_pContainer[CONTAINER_TYPE_SUNDRIES]->getContainerMaxNum(); i++ )
	{
		setItemTypeCD( type, cd, m_pContainer[CONTAINER_TYPE_SUNDRIES]->getItem(i) );
	}

	for( int i = 0; i < m_pContainer[CONTAINER_TYPE_STASH]->getContainerMaxNum(); i++ )
	{
		setItemTypeCD( type, cd, m_pContainer[CONTAINER_TYPE_STASH]->getItem(i) );
	}
}

// 物品是否可放入背包
bool Container::canDragIn( int nListType, int nId )
{
	return m_pContainer[nListType]->canDragIn( nId );
}

// 设置背包变化时物品的数量
void Container::setItemChgCount( const ROLEPACKAGECHG& Packge_Info_Change )
{
	int nArmChangeNum = Packge_Info_Change.ArmChg.ChgNum;
	for( int i = 0; i < nArmChangeNum; i++ )
	{
		int nId = Packge_Info_Change.ArmChg.Arms[i].Arm.DefID;
		setItemCount( nId, getItemTotalNum( nId ) );
	}

	int nItemChangeNum = Packge_Info_Change.ItemChg.ChgNum;
	for( int i = 0; i < nItemChangeNum; i++ )
	{
		int nId = Packge_Info_Change.ItemChg.Items[i].Item.DefID;
		setItemCount( nId, getItemTotalNum( nId ) );
	}
}

void Container::setItemCount( int Id, int count )
{
	std::map<int, int>::iterator iter = m_ItemCount.find( Id );
	if( iter != m_ItemCount.end() )
	{
		iter->second = count;
	}else{
		m_ItemCount[Id] = count;
	}
}

void Container::setItemTypeCD( int type, int cd, Item& item )
{
	if( item.m_ItemType == ITEM_SLOTTYPE_NONE || item.m_ItemType == ITEM_SLOTTYPE_DISABLE )
	{
		return;
	}

	const tagItemDef &itemDef = DBDataManager::getSingleton().getItemDef( item.getItemId() );
	if( itemDef.SelfSpeCoolType == type )
	{
		tagCDTime &itemCD = m_ItemCDMap[itemDef.ItemID];
		if (itemCD.remain() < cd * 100)
		{
			itemCD.startTime = timeGetTime();
			itemCD.LastTime = itemCD.startTime + cd * 100;
		}
	}
}

// 是否在指定区域使用
bool Container::isInUseArea( const tagItemDef_ToLua* pItemDef, MainGamePlayer* pMainPlayer )
{
	GameMap* pGameMap = pMainPlayer->getCurMap();
	if ( pGameMap == NULL )
	{
		return false;
	}

	int nMapId		= pGameMap->getMapId();
	int nRealPosX	= pMainPlayer->GetPosition().x / 10;
	int nRealPosY	= pMainPlayer->GetPosition().z / 10;
	bool inArea = false;
	bool mustInArea = false;

	for( int i = 0; i < MAX_ITEM_RESULT; i++ )
	{
		const tagResult_ToLua& result = pItemDef->Result[i];
		if( result.ResultID == RESULT_IN_AREA_USE )
		{
			mustInArea = true;
			int x = (nRealPosX - result.ResultVal2);
			int y = (nRealPosY - result.ResultVal3);
			if( nMapId == result.ResultVal4 &&
				((x * x + y * y) <= result.ResultVal1 * result.ResultVal1 || (result.ResultVal2 == 0 && result.ResultVal3 == 0 && result.ResultVal1 == 0)) )
			{
				inArea = true;
				break;
			}
		}
	}
	if (mustInArea && !inArea)
	{
		return false;
	}
	return true;
}

void Container::send_ChangeCoin(int nWay,int nCount,int npcId)
{
	tagCSPkgBody msgbody;
	msgbody.ChangeCoinInto.WhichWay = nWay;
	msgbody.ChangeCoinInto.From = nCount;
	msgbody.ChangeCoinInto.NpcID = npcId;
	SendGameServerMsg(CHANGE_COIN_INTO,msgbody);
}

void Container::requestSetContainerLock( const char* szPwd )
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.SafeLockClt.Cmd = SAFE_LOCK_ACTIVE;
	strcpy(msgbody.SafeLockClt.SafeLock.LockActive,szPwd);
	SendGameServerMsg(SAFE_LOCK_CLT,msgbody);
}

void Container::requestChangeContainerPsd(const char* szOldPwd,const char* szNewPwd)
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.SafeLockClt.Cmd = SAFE_LOCK_MODIFY;
	strcpy(msgbody.SafeLockClt.SafeLock.LockModify.OldPassWD,szOldPwd);
	strcpy(msgbody.SafeLockClt.SafeLock.LockModify.NewPassWD,szNewPwd);
	SendGameServerMsg(SAFE_LOCK_CLT,msgbody);
}

void Container::requestCheckContainerPsd(const char* szPwd)
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.SafeLockClt.Cmd = SAFE_LOCK_PASS;
	strcpy(msgbody.SafeLockClt.SafeLock.LockPass,szPwd);
	SendGameServerMsg(SAFE_LOCK_CLT,msgbody);
}

void Container::requestCancelContainerLock( const char* szPwd )
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.SafeLockClt.Cmd = SAFE_LOCK_CANCEL;
	strcpy( msgbody.SafeLockClt.SafeLock.LockCancel,szPwd );
	SendGameServerMsg(SAFE_LOCK_CLT,msgbody);
}

void Container::requestLockContainer()
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.SafeLockClt.Cmd= SAFE_LOCK_REDO;
	msgbody.SafeLockClt.SafeLock.LockReDo = 0;
	SendGameServerMsg( SAFE_LOCK_CLT,msgbody );
}

int Container::getContainerLockState()
{
	if( (m_nLockFlag & SAFE_LOCK_ACTIVE_FALG) && (m_nLockFlag & SAFE_LOCK_PASS_FLAG) )
	{
		return CONTAINER_LOCK_STATE_OPEN;
	}
	if( m_nLockFlag & SAFE_LOCK_ACTIVE_FALG )
	{
		return CONTAINER_LOCK_STATE_LOCK;
	}
	return CONTAINER_LOCK_STATE_NO;
}

int Container::getStuffNum( int ListType )
{
	return m_pContainer[ListType]->getStuffNum();
}

int Container::getItemPos( int nListType,int nItemID )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	for( int i = 0; i < nMaxNum; i++ )
	{
		if( getItem( nListType, i ).getItemId() == nItemID )
		{
			return i;
		}
	}
	return -1;
}

// 物品是否处于使用前的吟唱状态
bool Container::isUseItemPre()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	return ( pMainPlayer->isInPre( eUseItem_Chant ) );
}

int Container::getUnLockItemPos( int nListType,int nItemID )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	int nIdx = -1;
	for( int i = 0; i < nMaxNum; i++ )
	{
		Item& item = getItem( nListType, i );
		if( item.getItemId() == nItemID )
		{		
			bool bLock = false;
			ActorManager::getSingleton().getScriptVM()->callFunction("IsItemLock","ii>b",
				nListType,i,&bLock);
			if( !bLock )
			{
				//优先使用绑定的物品
				if( item.isItemBind() )
				{
					return i;
				}else{
					nIdx = i;
				}				
			}
		}
	}
	return nIdx;
}

int Container::getBindItemNum( int nListType, int nItemID )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	int nCount = 0;
	for( int i = 0 ; i < nMaxNum ; i++ )
	{
		Item& item = getItem( nListType, i );
		if( item.getItemId() == nItemID )
		{
			bool bLock = false;
			ActorManager::getSingleton().getScriptVM()->callFunction("IsItemLock","ii>b",
				nListType,i,&bLock);
			if( !bLock && item.isItemBind() )
			{
				nCount += item.getNum();
			}
		}
	}
	return nCount;
}

int Container::getUnBindItemNum( int nListType, int nItemID )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	int nCount = 0;
	for( int i = 0 ; i < nMaxNum ; i++ )
	{
		Item& item = getItem( nListType, i );
		if( item.getItemId() == nItemID )
		{
			bool bLock = false;
			ActorManager::getSingleton().getScriptVM()->callFunction("IsItemLock","ii>b",
				nListType,i,&bLock);
			if( !bLock && !item.isItemBind() )
			{
				nCount += item.getNum();
			}
		}
	}
	return nCount;
}

int Container::getUnBindItemNumNotCheckLock( int nListType, int nItemID )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	int nCount = 0;
	for( int i = 0 ; i < nMaxNum ; i++ )
	{
		Item& item = getItem( nListType, i );
		if( item.getItemId() == nItemID && !item.isItemBind() )
		{
			nCount += item.getNum();
		}
	}
	return nCount;
}

int Container::getBindItemNumNotCheckLock( int nListType, int nItemID )
{
	int nMaxNum = m_pContainer[nListType]->getContainerMaxNum();
	int nCount = 0;
	for( int i = 0 ; i < nMaxNum ; i++ )
	{
		Item& item = getItem( nListType, i );
		if( item.getItemId() == nItemID && item.isItemBind() )
		{
			nCount += item.getNum();
		}
	}
	return nCount;
}

void Container::requestAddStashGrid()
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type = PACKAGE_REQ_ADD_STASH_ET;
	SendGameServerMsg(PACKAGE_REQ,msgbody);
}

bool Container::canUseTaskItem( int id )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	const tagItemDef_ToLua* pItemDef = getItemDef( id );
	int itemSubType = id/1000;
	if( !isItemCanUsed( itemSubType ) )
	{
		return false;
	}
	if( !isInUseArea( pItemDef, pMainPlayer ) )
	{
		return false;
	}
	
	if( pItemDef->UseTarget == USE_TARGET_MONSTER 
		|| pItemDef->UseTarget == USE_TARGET_NPC
		|| pItemDef->UseTarget == USE_TARGET_PLAYER )
	{
		GameActor* pTarget = ActorManager::getSingleton().FindActor( pMainPlayer->GetSelectTarget() );
		//目标指定NPC
		if( pTarget == NULL || pTarget->isDead() )
		{
			return false; 
		}
		if( pItemDef->UseTargetID != 0 )
		{
			if( pTarget->getResId() != pItemDef->UseTargetID )
			{				
				return false;
			}
		} 
		else
		{
			if( pItemDef->UseTarget == USE_TARGET_MONSTER  && pTarget->getType() != GAT_MONSTER )
			{
				return false; 
			}
			else if( pItemDef->UseTarget == USE_TARGET_NPC  && pTarget->getType() != GAT_NPC )
			{
				return false; 
			}else if( pItemDef->UseTarget == USE_TARGET_PLAYER  && pTarget->getType() != GAT_PLAYER )
			{
				return false; 
			}
		}

		int radius = pItemDef->MaxUseDist;
		if (radius <= 0)
		{
			radius = 500;
		}
		GameXYZ selfPos = pMainPlayer->GetPosition();
		GameXYZ targetPos = pTarget->GetPosition();
		targetPos.y = selfPos.y;
		if (selfPos.subtract(targetPos).length() > radius)
		{
			return false;
		}
		for( int i = 0 ; i < MAX_ITEM_RESULT ; i++ )
		{
			if( pItemDef->Result[i].ResultID == RESULT_CATCH_PET )
			{
				const tagMonsterDef&  MonsterDef = DBDataManager::getSingleton().getMonsterDef( pTarget->getResId() );
				if ( &MonsterDef == NULL )
				{
					return false;
				}

				const MONBOOTY& monsterBootyDef = MonsterDef.Booty;
				if ( monsterBootyDef.BootyPetEgg == 0 )
				{
					return false;
				}
			}
		}
	}
	return true;
}

void Container::onItemLockChange()
{
	// 背包的锁定的状态改变
	GameMsgPrint::getSingleton().SendUIEvent( "GE_ITEM_LOCK_CHG" );
}