#include "Equip_Common.h"
#include "GameDBData.h"
#include "Equip.h"

int g_checkSuit( int* EquipList )
{
	for( int i = EQUIP_POS_RIGHTHAND; i <= EQUIP_POS_SHOULDER; i++ )
	{
		if( EquipList[i] == 0 ) continue;

		tagItemDef itemDef = DBDataManager::getSingleton().getItemDef( EquipList[i] );
		if( itemDef.SuitSetID != 0 )
		{
			int nCollate = itemDef.WearPosNum;
			for( int j = EQUIP_POS_RIGHTHAND; j <= EQUIP_POS_SHOULDER; j++ )
			{
				if ( j == i || EquipList[j] == 0 ) continue;

				tagItemDef itemDef2 = DBDataManager::getSingleton().getItemDef( EquipList[j] );
				if( itemDef.SuitSetID == itemDef2.SuitSetID )
				{
					nCollate += itemDef2.WearPosNum;
				}
				if( nCollate >= MAX_AVATAR - 1 )
				{
					return  itemDef.SuitSetID;
				}
			}
		}
	}
	return 0;
}

void g_AvatarChange()
{
	memset( g_AvatarAttrib, 0, sizeof(g_AvatarAttrib) );
	memset( g_AvatarStar, 0, sizeof(g_AvatarStar) );
	int nSelectNum = EQUIP_POS_END ;
	if( g_AvatarSelect == 0 )
	{
		nSelectNum = EQUIP_POS_FASHION_LEG + 1;
	}
	else if( g_AvatarSelect == 1 )
	{
		nSelectNum = EQUIP_POS_FASHION_LEG2 + 1;
	}
	else if ( g_AvatarSelect == 2 )
	{
		nSelectNum = EQUIP_POS_END;
	}
	for( int i = 0; i < nSelectNum; i++ )
	{
		g_AvatarAttrib[EquipPos_2_PosId[i]] = g_ArmEquipId_List[i];
	}
	if( g_ArmEquipId_List[EQUIP_POS_FASHION_HAIR] > 0 )
	{
		g_AvatarAttrib[EquipPos_2_PosId[EQUIP_POS_FASHION_HAIR]] = 1;
	}else{
		g_AvatarAttrib[EquipPos_2_PosId[EQUIP_POS_FASHION_HAIR]] = 0;
	}
	if( g_ArmEquipId_List[EQUIP_POS_RIGHTHAND] / 10000 == ARM_HEVY_SCATTERGUN && g_ArmEquipId_List[EQUIP_POS_LEFT] / 10000 != ARM_ENERGE )
	{
		g_AvatarAttrib[EQUIP_POS_LEFT] = 0;
	}
	
	for( int i = 0; i < nSelectNum; i++ )
	{
		g_AvatarStar[EquipPos_2_PosId[i]] = g_ArmStarLv_List[i];
	}
}

void g_ConfigFashionFair()
{	
	if( g_AvatarAttrib[POS_ID_FASHION_BODY] > 0 
		|| g_AvatarAttrib[POS_ID_FASHION_HEAD] > 0 
// 		|| g_AvatarAttrib[POS_ID_FASHION_BACK] > 0
		|| ( g_AvatarAttrib[POS_ID_FASHION_HAIR] & 1 )
		|| g_AvatarAttrib[POS_ID_HAT] == 0 )
	{
	}else{
		g_AvatarAttrib[POS_ID_HAIR] = 0;
	}
	//Ê±×°Ã±×Ó
	if( g_AvatarAttrib[POS_ID_FASHION_HEAD] == 0 )
	{
		if( g_ArmEquipId_List[EQUIP_POS_FASHION_HAIR] != 0 )
		{
			int nHair = DBDataManager::getSingleton().getItemDisplayInfo(g_ArmEquipId_List[EQUIP_POS_FASHION_HAIR]).hairstyle;
			g_AvatarAttrib[POS_ID_HAIR] = nHair;
		}
	}else{
		int nIndex = EQUIP_POS_FASHION_HEAD + g_AvatarSelect * 7;
		int nHair = DBDataManager::getSingleton().getItemDisplayInfo(g_ArmEquipId_List[nIndex]).hairstyle;
		nHair = nHair > 1 ? 9 : 0;
		g_AvatarAttrib[POS_ID_HAIR] = nHair;		
	}
}

int getArmEquip( int index )
{
	assert( index < EQUIP_POS_END );
	return g_ArmEquipId_List[index];
}

void setArmEquipIdList( int index, int id, int starLv )
{
	g_ArmEquipId_List[index] = id;
	g_ArmStarLv_List[index] = starLv;
}

void setAvatarSelectSuit( int nIndex )
{
	g_AvatarSelect = nIndex;
}

void clearEquipIdList()
{
	memset( g_ArmEquipId_List, 0, sizeof(g_ArmEquipId_List) );
}

int getAvatarAttrib( int index )
{
	return g_AvatarAttrib[index];
}

void setAvatarAttrib( int index, int nvalue )
{
	g_AvatarAttrib[index] = nvalue;
}

int getAvatarStarLv( int index )
{
	return g_AvatarStar[index];
}

void setAvatarStarLv( int index, int nvalue )
{
	g_AvatarStar[index] = nvalue;
}

int getSuitSetId( int id, int index )
{
	std::map<int,std::vector<int>>::iterator iter = Equip::m_mapSuitSet.find( id );
	if( iter != Equip::m_mapSuitSet.end() )
	{
		return (iter->second)[index];
	}
	return 0;
}

int getSuitEquipNum( int id )
{
	std::map<int,std::vector<int>>::iterator iter = Equip::m_mapSuitSet.find( id );
	if( iter != Equip::m_mapSuitSet.end() )
	{
		return (int)(iter->second).size();
	}
	return 0;
}

int convertEquipPos2PosId( int equipPos )
{
	if( equipPos >= EQUIP_POS_END )
	{
		return EQUIP_POS_NO;
	}
	return EquipPos_2_PosId[equipPos];
}