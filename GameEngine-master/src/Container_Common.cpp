#include "Container_Common.h"
#include "Container.h"
#include "GameDBData.h"
#include "resdb_meta.h"

IMPLEMENT_CONTAINER( Container_Amm, tagAmmContainer )
IMPLEMENT_CONTAINER( Container_Stuff, tagStuffContainer )
IMPLEMENT_CONTAINER( Container_Task, tagTaskContainer )
IMPLEMENT_CONTAINER( Container_Sundries, tagSundriesContainer )
IMPLEMENT_CONTAINER( Container_Stash, tagStashContainer )
IMPLEMENT_CONTAINER( Container_Tem, tagTemContainer )
IMPLEMENT_CONTAINER (Container_Market,tagMarketInfo)
IMPLEMENT_CONTAINER( Container_Extend,tagExtendContainer )
IMPLEMENT_CONTAINER( Container_ExtendStash,tagExtendContainer )
IMPLEMENT_CONTAINER( Container_Spar,tagSparContainer )

#define CANDRAGIN_CONTAINER( nItemId, nMinId, nMaxId ) ( ( nItemId >= nMinId && nItemId <= nMaxId )? true : false )

int Container_Amm::getAmmCount( int nAmmType )
{
	int nCount = 0;
	for( int i = 0; i < m_Container.m_nMaxNum; i++ )
	{
		if( m_Container.m_ItemData[i].isAmm() )
		{
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( m_Container.m_ItemData[i].getItemId() );
			if( nAmmType == ammDef.AmmType )
			{
				nCount += m_Container.m_ItemData[i].getNum();
			}
		}
	}
	return nCount;
}

bool Container_Amm::canDragIn( int nItemId )
{
	return false;	
}

void Container_Stash::setStashMoney( unsigned int uMoney ) 
{
	m_Container.m_nMoney = uMoney;
};

unsigned int Container_Stash::getStashMoney() 
{
	return m_Container.m_nMoney; 
}

bool Container_Stash::canDragIn( int nItemId )
{
	return true;	
}

void Container_Tem::setTmpContainerEndTime( int nTimeEnd )
{
	m_Container.m_nTimeEnd = nTimeEnd;
};

bool Container_Tem::canDragIn( int nItemId )
{
	return true;	
}

bool Container_Stuff::canDragIn( int nItemId )
{
	return CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_STUFF_MIN, ITEM_NUM_STUFF_MAX );	
}

bool Container_Task::canDragIn( int nItemId )
{
	return CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_TASK_MIN, ITEM_NUM_TASK_MAX );	
}

bool Container_Sundries::canDragIn( int nItemId )
{
	if( CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_ITEM_MIN, ITEM_NUM_STUFF_MIN-1 )
		|| CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_STUFF_MIN, ITEM_NUM_STUFF_MAX )
		|| CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_TASK_MAX+1, ITEM_NUM_ITEM_MAX )
		|| CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_ARM_MIN, ITEM_NUM_ARM_MAX ) 
		|| CANDRAGIN_CONTAINER( nItemId, ITEM_NUM_STUFF_MAX+1, ITEM_NUM_TASK_MIN-1 ))
	{
		return true;
	}
	return false;	
}
bool Container_Market::canDragIn(int nItemId)
{
	return false;
}

void Container_Extend::setStuffNum( int nNum )
{
	m_Container.m_nStuffNum = nNum;
}

int Container_Extend::getStuffNum()
{
	return m_Container.m_nStuffNum;
}

bool Container_Extend::canDragIn( int nItemId )
{
	//TODO HERE
	return true;
}

bool Container_ExtendStash::canDragIn( int nItemId )
{
	//TODO HERE
	return true;
}

bool Container_Spar::canDragIn( int nItemId )
{
	return false;
}