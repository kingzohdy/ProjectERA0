#include "Equip.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameActor_Tick.h"
#include "GameMainPlayer.h"
#include "resdb_meta.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDBData.h"
#include "GameMsgPrint.h"
#include "GameSkill_Mgr.h"
#include "OgreScriptLuaVM.h"
#include "ServerTimer.h"
#include "GameMap.h"
#include <Windows.h>

std::map<int,std::vector<int>> 	Equip::m_mapSuitSet;

int getItemDisplayInfoHairStyle( int armDefId )
{
	const ITEMDISPLAYINFO* pItemDisplayInfo = &( DBDataManager::getSingleton().getItemDisplayInfo( armDefId ) );
	if ( pItemDisplayInfo == NULL )
	{
		return 1;
	}

	return pItemDisplayInfo->hairstyle;
}

Equip::Equip(void) : m_TmpEquip(0), m_TmpMachin(0), m_bHideWeap(false),m_nTmpOpType(0),m_nFashionSelect(0),m_nFashionNum(0)
{
	memset( m_ArmEquipId, 0, sizeof( m_ArmEquipId ) );
	memset( m_AmmMap, 0, sizeof(m_AmmMap) );
	memset( m_ArmStar, 0, sizeof(m_ArmStar));
	for( int i = 1; i < EQUIP_POS_END; i++ )
	{
		m_Arm[i].m_ItemType = ITEM_SLOTTYPE_NONE;
	}
}

Equip::~Equip(void)
{
}

// 获取人物身上属于套装的装备数量
int Equip::getPlayerSuitEquipNum( int id )
{
	std::list<int> lstSuitID;
	std::map<int,std::vector<int>>::iterator iter = m_mapSuitSet.find( id );
	if( iter != m_mapSuitSet.end() )
	{
		for( int i = 0; i <  getSuitEquipNum( id ); i++ )
		{
			for( int j = EQUIP_POS_RIGHTHAND; j < EQUIP_POS_END; j++ )
			{
				tagItemDef_ToLua* pItemDef = getItem( j ).getItemDef();
				//过了有效期的不计算
				if( pItemDef != NULL && pItemDef->ItemID == ( iter->second )[i] && !getItem(j).isValidEnd() )
				{
					lstSuitID.push_back( pItemDef->ItemID );
				}
			}
		}
		lstSuitID.unique();
	}
	return (int)lstSuitID.size();
}

// 获得对应格位物品
int Equip::getEquipItemID( int nGridx )
{
	assert( nGridx < EQUIP_POS_END );
	return m_ArmEquipId[nGridx];
}

void Equip::setOwner( GameActor *pActor )
{
	m_Owner = pActor;
}

bool IsFashionArm( const ROLEARM& arm )
{
	return ( arm.GridIdx == EQUIP_POS_FASHION_BODY || arm.GridIdx == EQUIP_POS_FASHION_HEAD 
		|| arm.GridIdx == EQUIP_POS_FASHION_HAIR || arm.GridIdx == EQUIP_POS_FASHION_BACK
		|| arm.GridIdx == EQUIP_POS_FASHION_SHOES || arm.GridIdx == EQUIP_POS_FASHION_HAND
		|| arm.GridIdx == EQUIP_POS_FASHION_LEG );
}

bool IsHeadArm(const ROLEARM& arm)
{
	return arm.GridIdx == EQUIP_POS_HEAD;
}

void Equip::changEquip( int GridIndex, const ROLEARM& arm )
{
	if( arm.Num == 0 )
	{
		m_Arm[GridIndex].m_ItemType = ITEM_SLOTTYPE_NONE;
 		m_Arm[GridIndex].m_ItemData.m_Arm.DefID = 0;
		m_ArmEquipId[GridIndex] = 0;
	}else{
		m_Arm[GridIndex]			= arm;
		m_Arm[GridIndex].m_ListType = CONTAINER_TYPE_WEAR;
		int sec, usec;
		ServerTimer::getSingleton().getServerTime(sec,usec);
		if( arm.ValidEnd > 0 && sec >= arm.ValidEnd )
		{
			m_ArmEquipId[GridIndex] = 0;
		}else{
			if( !IsHeadArm(arm) || m_Owner->isShowHead() )
			{
				m_ArmEquipId[GridIndex] = arm.DefID;
			}else{
				m_ArmEquipId[GridIndex] = 0;
			}
		}		
	}		
	// 判断弹药需求
	if( GridIndex == EQUIP_POS_LEFT || GridIndex == EQUIP_POS_RIGHTHAND || GridIndex == EQUIP_POS_SHOULDER )
	{
		checkWeapEquip( GridIndex );
	}
}

void Equip::CheckAvatarHair()
{
	Ogre::AvatarEntity* avatar = m_Owner->getAvatar();
	if( avatar != NULL )
	{
		if( avatar->getAvatarAttrib(POS_ID_FASHION_BODY) > 0 
			|| avatar->getAvatarAttrib(POS_ID_FASHION_HEAD) > 0 
// 			|| avatar->getAvatarAttrib(POS_ID_FASHION_BACK) > 0
			|| ( avatar->getAvatarAttrib(POS_ID_FASHION_HAIR) & 1 )
			|| avatar->getAvatarAttrib(POS_ID_HAT) == 0 )
		{
			avatar->equipItem( POS_ID_HAIR,m_Owner->getHair() + 1,0 );
		}else{
			//没有时装，并且显示帽子
			avatar->equipItem( POS_ID_HAIR,0,0);
		}
		
		if( avatar->getAvatarAttrib( POS_ID_FASHION_HEAD ) == 0 )
		{
			//没有时装 帽子
			if( m_ArmEquipId[EQUIP_POS_FASHION_HAIR] != 0 )
			{
				int nHair = DBDataManager::getSingleton().getItemDisplayInfo(m_ArmEquipId[EQUIP_POS_FASHION_HAIR]).hairstyle;
				avatar->equipItem( POS_ID_HAIR,nHair,0 );
			}
		}else{
			int nIndex = EQUIP_POS_FASHION_HEAD + m_nFashionSelect * 7;
			int nHair = DBDataManager::getSingleton().getItemDisplayInfo(m_ArmEquipId[nIndex]).hairstyle;
			nHair = nHair > 1 ? 9 : 0;
			avatar->equipItem(POS_ID_HAIR,nHair,0);
		}
	}
}


void Equip::changEquip( int GridIndex, int armDefId, int nStar /*= 0*/ )
{
	int nCanPlayChangeState	= m_Owner->getActState().canChangeState( GSACT_IN_CHANGEEQUIP );
	m_ArmStar[GridIndex] = nStar;

	//如果再使用临时武器的时候，不刷新
	if( GridIndex == EQUIP_POS_RIGHTHAND || GridIndex == EQUIP_POS_LEFT )
	{
		if( getTmpArm() != 0 )
		{
			return;
		}
	}

	switch( GridIndex )
	{
	case EQUIP_POS_RIGHTHAND:
		{// 右手武器
			if( armDefId == 0 )
			{
				m_Owner->getActState().setPoseState( POSE_NONE );
			}else{
				int arm_sub_type = armDefId/10000;
				switch( armDefId/100000 )
				{
				case ARM_FIGHTER:// 战士武器
					{
						// 剑,刀
						if( arm_sub_type == ARM_SWORD || arm_sub_type == ARM_KNIFE )
						{
							if (m_ArmEquipId[EQUIP_POS_LEFT] / 10000 == ARM_SWORD_LEFT)
							{
								m_Owner->getActState().setPoseState( POSE_WEAPON_SHORTWEAPON );
							}
							else
							{
								m_Owner->getActState().setPoseState( POSE_SHORTWEAPON_R );
							}
						}
						// 对剑
						if( arm_sub_type == ARM_BOTHSWORD )
						{
							m_Owner->getActState().setPoseState( POSE_WEAPON_SHORTWEAPON );
						}
					}
					break;
				case ARM_WIZARD:// 法师武器
					{
						m_Owner->getActState().setPoseState( POSE_WAND );
					}
					break;
				case ARM_CHURCH:
					{
						m_Owner->getActState().setPoseState( POSE_CHURCH_WAND );
					}
					break;
				case ARM_GUN:// 枪手-枪
					{
						// 重机枪
						if( arm_sub_type == ARM_HEVY_SCATTERGUN )
						{
							m_Owner->getActState().setPoseState( POSE_SCATTERGUN );
						}
						// 对手枪
						if( arm_sub_type == ARM_BITH_GUN )
						{
							m_Owner->getActState().setPoseState( POSE_DOUBLEGUN );
						}
						// 榴弹炮
						if( arm_sub_type == ARM_LIUDAN_GUN )
						{
							m_Owner->getActState().setPoseState( POSE_SCATTERGUN );
						}
						// 单手枪
						if( arm_sub_type == ARM_RIGHT_GUN )
						{
							if (m_ArmEquipId[EQUIP_POS_LEFT] / 10000 == ARM_GUNNER_WEAR_LEFT)
							{
								m_Owner->getActState().setPoseState( POSE_DOUBLEGUN );
							}
							else
							{
								m_Owner->getActState().setPoseState( POSE_GUN_R );
							}
						}
					}
					break;
				}
			}
			if( nCanPlayChangeState != STATE_UNCHANGE && !m_bHideWeap )
			{
				Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
				if (avatar != NULL)
				{
					if (armDefId / 10000 == ARM_HEVY_SCATTERGUN)
					{
						avatar->equipItem(EquipPos_2_PosId[EQUIP_POS_LEFT], 0 , 0);
					}
					avatar->equipItem(EquipPos_2_PosId[GridIndex], armDefId , nStar);
				}
			}
			if( nCanPlayChangeState != STATE_UNACT_CHANGE && !m_bHideWeap )
			{
				int actId = m_Owner->getActState().getActionId();
				if (actId != -1)
				{
					m_Owner->playAction( actId );
				}
			}
		}
		break;
	case EQUIP_POS_LEFT:
		{// 左手武器
			if( armDefId == 0 )
			{
				if( nCanPlayChangeState != STATE_UNCHANGE )
				{
					Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
					if (avatar != NULL)
					{
						avatar->equipItem( POS_ID_LEFTHAND, 0, nStar );
					}
					if (m_Owner->getActState().getPoseState() == POSE_DOUBLEGUN)
					{
						m_Owner->getActState().setPoseState( POSE_GUN_R );
					}
					else if (m_Owner->getActState().getPoseState() == POSE_WEAPON_SHORTWEAPON)
					{
						m_Owner->getActState().setPoseState( POSE_SHORTWEAPON_R );
					}
				}
			}else{
				int arm_sub_type = armDefId/10000;
				// 战士盾
				if( arm_sub_type == ARM_SHIELD )
				{
					m_Owner->getActState().setPoseState( POSE_SHORTWEAPON_R );
				}
				// 战士左手剑
				else if (arm_sub_type == ARM_SWORD_LEFT)
				{
					if (m_ArmEquipId[EQUIP_POS_RIGHTHAND] != 0)
					{
						m_Owner->getActState().setPoseState( POSE_WEAPON_SHORTWEAPON );
					}
					else
					{
						m_Owner->getActState().setPoseState( POSE_NONE );
					}
				}
				// 牧师法器
				else if( arm_sub_type == ARM_GOD_FAQI )
				{
					m_Owner->getActState().setPoseState( POSE_CHURCH_WAND );
				}
				// 法师法器
				else if( arm_sub_type == ARM_MAGIC_FAQI )
				{
					m_Owner->getActState().setPoseState( POSE_WAND );
				}
				// 枪手副手枪
				else if (arm_sub_type == ARM_GUNNER_WEAR_LEFT)
				{
					if (m_ArmEquipId[EQUIP_POS_RIGHTHAND] != 0)
					{
						if (m_ArmEquipId[EQUIP_POS_RIGHTHAND] / 10000 == ARM_RIGHT_GUN)
						{
							m_Owner->getActState().setPoseState( POSE_DOUBLEGUN );
						}
					}
					else
					{
						m_Owner->getActState().setPoseState( POSE_NONE );
					}
				}
			}
			if( nCanPlayChangeState != STATE_UNCHANGE && !m_bHideWeap )
			{
				Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
				if (avatar != NULL)
				{
					if (armDefId / 10000 != ARM_GUNNER_WEAR_LEFT || m_ArmEquipId[EQUIP_POS_RIGHTHAND] / 10000 != ARM_HEVY_SCATTERGUN)
					{
						avatar->equipItem(EquipPos_2_PosId[GridIndex], armDefId, nStar);
					}
				}
			}
			if( nCanPlayChangeState != STATE_UNACT_CHANGE && !m_bHideWeap )
			{
				m_Owner->playAction( m_Owner->getActState().getActionId() );
			}
		}
		break;
	case EQUIP_POS_HEAD:
	case EQUIP_POS_FACE:
		{
			MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			if( nCanPlayChangeState != STATE_UNCHANGE )
			{
				Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
				if (avatar != NULL)
				{
					avatar->equipItem( EquipPos_2_PosId[GridIndex], armDefId, nStar );
				}
				CheckAvatarHair();
			}
		}
		break;
	case EQUIP_POS_BODY:
	case EQUIP_POS_BACK:
	case EQUIP_POS_FASHION_BODY:
	case EQUIP_POS_FASHION_HAND:
	case EQUIP_POS_FASHION_LEG:
	case EQUIP_POS_FASHION_HEAD:
	case EQUIP_POS_FASHION_SHOES:
	case EQUIP_POS_FASHION_HAIR:

	case EQUIP_POS_FASHION_BACK:
	case EQUIP_POS_SHOULDER:
		{
			if( nCanPlayChangeState != STATE_UNCHANGE )
			{
				Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
				if (avatar != NULL)
				{
					avatar->equipItem( EquipPos_2_PosId[GridIndex], armDefId, nStar );
				}
			}
		}
		break;
	}
	if( EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_BODY ||
		EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_HAND || 
		EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_LEG  || 
		EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_HEAD ||
		EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_SHOES)
	{ 
		if( nCanPlayChangeState != STATE_UNCHANGE )
		{
			Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
			if (avatar != NULL)
			{
				avatar->equipItem( EquipPos_2_PosId[GridIndex], armDefId, nStar );
			}
		}
		CheckAvatarHair();
	}
	if (EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_BACK)
	{			
		if( nCanPlayChangeState != STATE_UNCHANGE )
		{
			Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
			if (avatar != NULL)
			{
				if( isOnChairTmp() )
				{
					avatar->equipItem( EquipPos_2_PosId[GridIndex], 0, 0 );
				}else{
					avatar->equipItem( EquipPos_2_PosId[GridIndex], armDefId, nStar );
				}
				avatar->updateAttrib();
			}
		}
		//CheckAvatarHair();
	}
	if( EquipPos_2_PosId[GridIndex] == POS_ID_FASHION_HAIR )
	{
		if( nCanPlayChangeState != STATE_UNCHANGE )
		{
			Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
			if (avatar != NULL)
			{
				if( armDefId > 0 )
				{
					avatar->equipItem( EquipPos_2_PosId[GridIndex], 1, nStar );
				}else{
					avatar->equipItem( EquipPos_2_PosId[GridIndex], 0, nStar );
				}
			}
		}
		CheckAvatarHair();
	}
	// 组合出套装
	int nSuitId = checkSuit();
	if( nCanPlayChangeState != STATE_UNCHANGE )
	{
		Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
		if (avatar != NULL)
		{
			if( nSuitId != 0 )
			{
				avatar->equipItem( POS_ID_SUIT, nSuitId,nStar );
			}else{
				avatar->equipItem( POS_ID_SUIT, 0, 0 );
			}
		}
	}
}

void Equip::changAmm( int GridIndex, int ammId )
{
	m_AmmMap[GridIndex] = ammId;
	if( AMM_POS_LEFT == GridIndex && m_Arm[EQUIP_POS_LEFT].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		int needAmm = m_Arm[EQUIP_POS_LEFT].needAmm();
		if( needAmm == UNUSED_AMM )
		{
			m_Arm[EQUIP_POS_LEFT].m_bCanUsed = true;
		}else{
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( ammId );	
			if( needAmm == ammDef.AmmType )
			{
				m_Arm[EQUIP_POS_LEFT].m_bCanUsed = true;
			}else{
				m_Arm[EQUIP_POS_LEFT].m_bCanUsed = false;
			}
		}
	}

	if( AMM_POS_RIGHT == GridIndex && m_Arm[AMM_POS_RIGHT].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		int needAmm = m_Arm[AMM_POS_RIGHT].needAmm();
		if( needAmm == UNUSED_AMM )
		{
			m_Arm[AMM_POS_RIGHT].m_bCanUsed = true;
		}else{
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( ammId );	
			if( needAmm == ammDef.AmmType )
			{
				m_Arm[AMM_POS_RIGHT].m_bCanUsed = true;
			}else{
				m_Arm[AMM_POS_RIGHT].m_bCanUsed = false;
			}
		}
	}

	if( AMM_POS_SHOULDER == GridIndex && m_Arm[AMM_POS_SHOULDER].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		int needAmm = m_Arm[AMM_POS_SHOULDER].needAmm();
		if( needAmm == UNUSED_AMM )
		{
			m_Arm[AMM_POS_SHOULDER].m_bCanUsed = true;
		}else{
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( ammId );	
			if( needAmm == ammDef.AmmType )
			{
				m_Arm[AMM_POS_SHOULDER].m_bCanUsed = true;
			}else{
				m_Arm[AMM_POS_SHOULDER].m_bCanUsed = false;
			}
		}
	}
}

void Equip::reLoadAllEquip()
{
	for( int i = 1; i < EQUIP_POS_END; i++ )
	{
		if( m_ArmEquipId[i] != 0 )
		{
			changEquip( i, m_ArmEquipId[i],m_ArmStar[i] );
		}
	}
	if (m_Owner->getAvatar())
	{
		m_Owner->getAvatar()->updateAttrib();
	}
}

void Equip::clearAllEquip()
{
	memset( m_ArmEquipId, 0, sizeof( m_ArmEquipId ) );
	memset( m_AmmMap, 0, sizeof(m_AmmMap) );
	for( int i = 1; i < EQUIP_POS_END; i++ )
	{
		m_Arm[i].m_ItemType = ITEM_SLOTTYPE_NONE;
	}
}

int Equip::checkSuit()
{
	return g_checkSuit( m_ArmEquipId );
}

void Equip::checkWeapEquip( int pos )
{
	// 需要弹药
	int needAmm = m_Arm[pos].needAmm();
	if( needAmm != UNUSED_AMM )
	{
		if( pos == EQUIP_POS_LEFT )
		{
			if( m_AmmMap[AMM_POS_LEFT] == 0 ) 
			{
				// 没装备弹药
				m_Arm[pos].m_bCanUsed = false;
				return;
			}
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( m_AmmMap[AMM_POS_LEFT] );	
			if( ammDef.AmmType != needAmm )
			{
				// 弹药类型不匹配
				m_Arm[pos].m_bCanUsed = false;
				return;
			}
		}

		if( pos == EQUIP_POS_RIGHTHAND )
		{
			if( m_AmmMap[AMM_POS_RIGHT] == 0 ) 
			{
				// 没装备弹药
				m_Arm[pos].m_bCanUsed = false;
				return;
			}
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( m_AmmMap[AMM_POS_RIGHT] );	
			if( ammDef.AmmType != needAmm )
			{
				// 弹药类型不匹配
				m_Arm[pos].m_bCanUsed = false;
				return;
			}
		}

		if( pos == EQUIP_POS_SHOULDER )
		{
			if( m_AmmMap[AMM_POS_SHOULDER] == 0 ) 
			{
				// 没装备弹药
				m_Arm[pos].m_bCanUsed = false;
				return;
			}
			tagItemDef ammDef = DBDataManager::getSingleton().getItemDef( m_AmmMap[AMM_POS_SHOULDER] );	
			if( ammDef.AmmType != needAmm )
			{
				// 弹药类型不匹配
				m_Arm[pos].m_bCanUsed = false;
				return;
			}
		}
	}
}

bool Equip::checkAmmUsed( int ammId )
{
	if( m_Arm[EQUIP_POS_RIGHTHAND].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( m_Arm[EQUIP_POS_RIGHTHAND].getItemId() );
		tagItemDef ammDef	= DBDataManager::getSingleton().getItemDef( ammId );
		if( itemDef.WeaponAmmType == ammDef.AmmType )
		{
			return true;
		}
	}

	if( m_Arm[EQUIP_POS_LEFT].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( m_Arm[EQUIP_POS_LEFT].getItemId() );
		tagItemDef ammDef	= DBDataManager::getSingleton().getItemDef( ammId );
		if( itemDef.WeaponAmmType == ammDef.AmmType )
		{
			return true;
		}
	}

	if( m_Arm[EQUIP_POS_SHOULDER].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( m_Arm[EQUIP_POS_SHOULDER].getItemId() );
		tagItemDef ammDef	= DBDataManager::getSingleton().getItemDef( ammId );
		if( itemDef.WeaponAmmType == ammDef.AmmType )
		{
			return true;
		}
	}

	return false;
}

int Equip::getEquipPos( int armDefId )
{
	return 0;
}

// 获得右手武器类型
int Equip::getRightHandEquipType()
{
	if (m_ArmEquipId[EQUIP_POS_LEFT] / 10000 == ARM_GUNNER_WEAR_LEFT)
	{
		if (m_ArmEquipId[EQUIP_POS_RIGHTHAND] / 10000 == ARM_RIGHT_GUN)
		{
			return ARM_BITH_GUN;
		}
	}
	else if (m_ArmEquipId[EQUIP_POS_LEFT] / 10000 == ARM_SWORD_LEFT)
	{
		return ARM_BOTHSWORD;
	}
	return m_ArmEquipId[EQUIP_POS_RIGHTHAND]/10000;
}

Item& Equip::getItem( int GridIndex )
{
	return m_Arm[GridIndex];
}

bool Equip::getItemLink( int GridIndex )
{
	Item& ItemLink = m_Arm[GridIndex];
	if( ItemLink.getItemId() != 0 && ( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) ) )
	{
		GameMsgPrint::getSingleton().setItemLink( ItemLink );
		return true;
	}
	return false;
}

int	Equip::getArmType( int pos )
{
	if( m_Arm[pos].m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		return m_Arm[pos].getItemId()/10000;
	}
	return ITEM_SLOTTYPE_NONE;
}

// 设置临时装备
void Equip::setTmpArm( int ArmId )
{
	if( ArmId != 0 )
	{
		bindTmpArm( m_Owner, ArmId );
	}else{
		unBindTmpArm( m_Owner, ArmId );
	}	
}

// 绑定临时武器	 
void Equip::bindTmpArm( GameActor* pActor, int nArmId )
{
	const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( nArmId );
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pActor == pMainPlayer )
	{	
		if( itemDef.ItemTmp.SkillID_1 != 0 )
		{
			SkillManager::getSingleton().setTmpSkill( 0, itemDef.ItemTmp.SkillID_1, itemDef.ItemTmp.SkillLevel_1 );
			SkillManager::getSingleton().addTmpSkill(itemDef.ItemTmp.SkillID_1);
		}
		if( itemDef.ItemTmp.SkillID_2 != 0 )
		{
			SkillManager::getSingleton().setTmpSkill( 1, itemDef.ItemTmp.SkillID_2, itemDef.ItemTmp.SkillLevel_2 );
			SkillManager::getSingleton().addTmpSkill(itemDef.ItemTmp.SkillID_2);
		}
	}
	
	switch( itemDef.ItemTmp.Type )
	{
	case ITEM_TMP_CLAN:
	case ITEM_TMP_ARM:
	case ITEM_TMP_ITEM:
		{
			usedTmpArm( nArmId );
		}
	case ITEM_TMP_MACHINE_ENERGE:
	case ITEM_TMP_ARM_AMM:
		{
			m_TmpEquip = nArmId;
			if( pActor == pMainPlayer )
			{
				pMainPlayer->m_bChangeSkillShortCut = true;
				GameMsgPrint::getSingleton().SendUIEvent( "GE_TMPITME_GETTMPITEM" );
			}
		}
		break;
	case ITEM_TMP_MACHINE:
		{
			// 临时器械变身
			m_TmpMachin = nArmId;
			m_TmpEquip  = 0;
			if( !pActor->isInFly() )
			{
				const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( nArmId );
				if( npcDisplayInfo.race == MONSTER_RIDE )
				{
					pActor->m_Rider.setRideOwner( pActor->GetID() );
					pActor->m_Rider.onHorse( nArmId, 200, false );					
					pActor->getActState().setInState( GSACT_IN_MORPH, false );
					pActor->getActState().setInState( GSACT_IN_USEDMACHINE, true );
					pActor->getActState().setInState( GSACT_IN_RIDE, true );
					pActor->ChangeActState( GSACT_STAND );
				}else{
					pActor->onMorph( nArmId );
					pActor->getActState().setInState( GSACT_IN_MORPH, true );
				}

			}

			pActor->setMaxEnerge( itemDef.ItemTmp.MachineEnerge );
			//在有技能的时候才会切换面板	
			if( pActor == pMainPlayer )
			{
				pMainPlayer->m_bChangeSkillShortCut = true;
				if( itemDef.ItemTmp.SkillID_1 != 0 || itemDef.ItemTmp.SkillID_2 != 0 )
				{
					GameMsgPrint::getSingleton().SendUIEvent( "GE_TMPITME_TMPMACHIN" );
				}
				else if ( itemDef.ItemTmp.SkillID_1 == 0 && itemDef.ItemTmp.SkillID_2 == 0 ) 
				{
					GameMsgPrint::getSingleton().SendUIEvent( "GE_TMPITME_TMPMACHIN_NOSKILL" );
				}
			}
			int nBackGrid =  EQUIP_POS_FASHION_BACK + pActor->m_Equip.m_nFashionSelect * 7;
			changEquip( nBackGrid,m_ArmEquipId[nBackGrid],m_ArmStar[nBackGrid] );
		}
		break;
	}
}

// 取消绑定临时武器
void Equip::unBindTmpArm( GameActor* pActor, int nArmId )
{
	if( m_TmpMachin != 0 )
	{
		const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( m_TmpMachin );
		if( npcDisplayInfo.race == MONSTER_RIDE )
		{
			pActor->m_Rider.downHorse();
			pActor->getActState().setInState( GSACT_IN_USEDMACHINE, false );
			pActor->getActState().setInState( GSACT_IN_RIDE, false );
		}else{
			pActor->onRestore(m_TmpMachin);
		}
		pActor->setMaxEnerge( 0 );
		pActor->ChangeActState( GSACT_STAND );
	}
	
	if( pActor == ActorManager::getSingleton().getMainPlayer() )
	{
		ActorManager::getSingleton().getMainPlayer()->m_bChangeSkillShortCut = false;
		if( SkillManager::getSingleton().getTmpSkillId(0) || SkillManager::getSingleton().getTmpSkillId(1) )
		{
			SkillManager::getSingleton().delTmpSkill(SkillManager::getSingleton().getTmpSkillId(0));
			SkillManager::getSingleton().delTmpSkill(SkillManager::getSingleton().getTmpSkillId(1));
			SkillManager::getSingleton().setTmpSkill( 0, 0, 0 );
			SkillManager::getSingleton().setTmpSkill( 1, 0, 0 );
			if( m_TmpMachin != 0 )
			{
				GameMsgPrint::getSingleton().SendUIEvent( "GE_TMPITME_LOOTTMPMACHIN" );
			}else if( m_TmpEquip != 0 ){
				GameMsgPrint::getSingleton().SendUIEvent( "GE_TMPITME_LOOTTMPITEM" );
			}
		}
	}

	if( m_TmpEquip != 0 )
	{
		m_TmpEquip  = 0;
		unUsedTmpArm();
	}
	m_TmpMachin	= 0;
	m_nTmpOpType = 0;
	
	int nFashionBackIndex = EQUIP_POS_FASHION_BACK + m_nFashionSelect * 7;
	changEquip( nFashionBackIndex,m_ArmEquipId[nFashionBackIndex],m_ArmStar[nFashionBackIndex] );
}

// 查询是否使用临时道具
bool Equip::isUsedTmp()
{
	return getTmpArm() != 0 || getTmpMachin() != 0;
}

// 获取临时装备
int Equip::getTmpArm()
{
	return m_TmpEquip;
}

// 获取临时机械
int Equip::getTmpMachin()
{
	return m_TmpMachin;
}

// 使用临时道具
void Equip::usedTmpArm( int armId )
{
	const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( armId );
	if( itemDef.ItemTmp.Type == ITEM_TMP_ARM || itemDef.ItemTmp.Type == ITEM_TMP_ITEM || itemDef.ItemTmp.Type == ITEM_TMP_CLAN )
	{
		int old = m_TmpEquip;
		m_TmpEquip = 0;
		changEquip( EQUIP_POS_RIGHTHAND, armId, 3 );
		changEquip( EQUIP_POS_LEFT,	0, 0 );
		m_TmpEquip = old;
		if (itemDef.PoseID == 0)
		{
			m_Owner->getActState().setPoseState(POSE_NONE);
		}
		else
		{
			m_Owner->getActState().setPoseState(itemDef.PoseID);
		}
		if (m_Owner->getActState().GetState() == GSACT_MOVE)
		{
			m_Owner->onChangeMove();
		}
		else
		{
			m_Owner->onChangeStand();
		}
		Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
		if (avatar != NULL)
		{
			avatar->updateAttrib();
		}
	}
}

// 退下临时道具
void Equip::unUsedTmpArm()
{
	if (m_Owner->isInSkillTetanic())
	{
		m_Owner->registerTickEvent( new GameActor_Tick_UnfixTmpArm( m_Owner , timeGetTime(), TMPAMR_UNFIXDELAY ) );
	}
	else
	{
		changEquip( EQUIP_POS_RIGHTHAND, m_ArmEquipId[EQUIP_POS_RIGHTHAND], m_ArmStar[EQUIP_POS_RIGHTHAND] );
		changEquip( EQUIP_POS_LEFT, m_ArmEquipId[EQUIP_POS_LEFT],m_ArmStar[EQUIP_POS_LEFT] );
		if (m_Owner->getAvatar())
		{
			m_Owner->getAvatar()->updateAttrib();
		}
	}
}

// 丢弃临时道具
void Equip::looseTmpArm()
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL && pMainPlayer->getActState().GetState() == GSACT_AUTOFLY )
	{
		return;
	}
	if (m_TmpEquip == 0 && m_TmpMachin == 0)
	{
		return;
	}

	do 
	{
		if (m_TmpEquip != 0)
		{
			const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef(m_TmpEquip);
			if (itemDef.ItemTmp.Type == ITEM_TMP_CLAN)
			{
				tagCSPkgBody msgbody;
				msgbody.PlayerMiscClt.PlayerMiscCltData.ClanItemID = m_TmpEquip;
				msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_DROP_CLAN_ITEM;
				SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
				break;
			}
		}
		tagCSPkgBody msgbody;
		msgbody.PackageReq.Type	= PACKAGE_REQ_TMPDROP;
		SendGameServerMsg( PACKAGE_REQ, msgbody );
	} while (false);
}

// 屏蔽武器
void Equip::hideWeapon()
{
	if( m_Owner->getActState().getInState( GSACT_IN_MORPH ) )
	{
		return;
	}
	if (!m_Owner->getShowActor()->canEquipItem())
	{
		return;
	}

	Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
	if (avatar != NULL)
	{
		avatar->equipItem( POS_ID_RIGHTHAND, 0, 0 );
		avatar->equipItem( POS_ID_LEFTHAND, 0, 0 );
		avatar->updateAttrib();
	}
	m_bHideWeap = true;
}

// 显示武器
void Equip::showWeapon()
{
	GameActor* pPlayer = m_Owner;
	if( pPlayer == NULL || pPlayer->getActState().getInState( GSACT_IN_MORPH ) )
	{
		return;
	}
	if (!pPlayer->getShowActor()->canEquipItem())
	{
		return;
	}

	m_bHideWeap = false;

	if( m_TmpEquip != 0 )
	{
		usedTmpArm( m_TmpEquip );
	}
	else
	{
		if (pPlayer->getType() == GAT_PLAYER || pPlayer->getType() == GAT_MAINPLAYER)
		{
			Ogre::AvatarEntity *avatar = pPlayer->getAvatar();
			if (avatar != NULL)
			{
				if( m_ArmEquipId[EQUIP_POS_RIGHTHAND] != 0 )
				{
					avatar->equipItem( POS_ID_RIGHTHAND, m_ArmEquipId[EQUIP_POS_RIGHTHAND],m_ArmStar[EQUIP_POS_RIGHTHAND] );
				}
				if( m_ArmEquipId[EQUIP_POS_LEFT] != 0 && m_ArmEquipId[EQUIP_POS_RIGHTHAND] / 10000 != ARM_HEVY_SCATTERGUN )
				{
					avatar->equipItem( POS_ID_LEFTHAND, m_ArmEquipId[EQUIP_POS_LEFT],m_ArmStar[EQUIP_POS_LEFT] );
				}
				avatar->updateAttrib();
			}
		}
		else
		{
			int id;
			id = Equip::getMonsterEquip(pPlayer->getResId(), EQUIP_POS_RIGHTHAND);
			Ogre::AvatarEntity *avatar = pPlayer->getAvatar();
			if (id != 0)
			{
				if (avatar != NULL)
				{
					avatar->equipItem( POS_ID_RIGHTHAND, id, 0 );
					avatar->updateAttrib();
				}
			}
			id = Equip::getMonsterEquip(pPlayer->getResId(), EQUIP_POS_LEFT);
			if (id != 0)
			{
				if (avatar != NULL)
				{
					avatar->equipItem( POS_ID_LEFTHAND, id, 0 );
					avatar->updateAttrib();
				}
			}
		}
	}
}

// 请求装装备
void Equip::send_Equip( int nListType, const Item& item, int wearPos )
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type						= ADJUST_TYPE_WEAR;
	msgbody.AdjustC.AdjustData.Wear.ArmDefID	= item.m_ItemData.m_Arm.DefID;
	msgbody.AdjustC.AdjustData.Wear.GridIdx		= item.m_ItemData.m_Arm.GridIdx;
	msgbody.AdjustC.AdjustData.Wear.ListType	= nListType;
	msgbody.AdjustC.AdjustData.Wear.WearPos		= wearPos;
	msgbody.AdjustC.AdjustData.Wear.PetWID		= 0;
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 请求脱装备
void Equip::send_unEquip( int Id, int desListType, int gridIndex, int wearPos )
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type						= ADJUST_TYPE_UNWEAR;
	msgbody.AdjustC.AdjustData.UnWear.ArmDefID	= Id;
	msgbody.AdjustC.AdjustData.UnWear.ListType	= desListType;
	msgbody.AdjustC.AdjustData.UnWear.GridIdx	= gridIndex;
	msgbody.AdjustC.AdjustData.UnWear.WearPos	= wearPos;
	msgbody.AdjustC.AdjustData.UnWear.PetWID	= 0;
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 请求装备弹药
void Equip::send_EquipAmm( int ammId )
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type					= ADJUST_TYPE_AMM;
	msgbody.AdjustC.AdjustData.Amm.AmmID	= ammId;
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

void Equip::setAvatarAttrib( int nRace, int nGender )
{
	clearEquipIdList();
	for( int i = 0; i < EQUIP_POS_END; i++ )
	{
		if ( m_ArmEquipId[i] != 0 )
		{
			setArmEquipIdList( i, m_ArmEquipId[i],m_ArmStar[i] );
		}
	}
	g_AvatarChange(); 
	::setAvatarAttrib(0, nRace );
	::setAvatarAttrib(1, nGender-1 );
	if( m_Owner->getAvatar() )
	{
		::setAvatarAttrib(POS_ID_HAIR,m_Owner->getAvatar()->getAvatarAttrib(POS_ID_HAIR));
	}
	if( m_Owner->getCurMap() && m_Owner->getCurMap()->getMapId() == BEACH_MAP_ID )
	{
		::setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
	}
}

// 获取场景模型的装备模型
int Equip::getAvatarId( int id )
{
	return getAvatarAttrib( id );
}

int Equip::getAvatarStarLv( int id )
{
	return ::getAvatarStarLv( id );
}

int Equip::getMonsterEquip(int resid, int slot)
{
	static const int Idx[][2] = {
		{EQUIP_POS_BODY, 3},
		{EQUIP_POS_RIGHTHAND, 4},
		{EQUIP_POS_LEFT, 5},
		{EQUIP_POS_HEAD, 6},
		{EQUIP_POS_SHOULDER, 7},
		{EQUIP_POS_BACK, 8},
		{EQUIP_POS_FASHION_BODY, 10},
		{EQUIP_POS_FACE, 11},
	};
	for( int i = 0; i < sizeof(Idx) / 2 / sizeof(int); i++ )
	{
		if (slot == Idx[i][0])
		{
			const tagNpcDisplayInfo &npcDisplayerInfo = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
			return npcDisplayerInfo.userdata[Idx[i][1] - 1];
		}
	}
	return 0;
}

bool Equip::isTmpPatronid()
{
	return (m_nTmpOpType & TMPITEM_ON_PATRONID);
}

bool Equip::isOnChairTmp()
{
	if( m_TmpMachin/100 == 14610 )
	{
		return true;
	}
	return false;
}

void Equip::SetArmInfo( int GridIndex, const ROLEARM& arm )
{
	if( arm.Num == 0 )
	{
		m_Arm[GridIndex].m_ItemType = ITEM_SLOTTYPE_NONE;
		m_Arm[GridIndex].m_ItemData.m_Arm.DefID = 0;
		m_ArmEquipId[GridIndex] = 0;
	}else{
		m_Arm[GridIndex]			= arm;
		m_Arm[GridIndex].m_ListType = CONTAINER_TYPE_WEAR;		
	}
}

void Equip::ShowShoulder( bool bShow )
{
	Ogre::AvatarEntity *avatar = m_Owner->getAvatar();
	if (avatar != NULL)
	{
		int id = avatar->getAvatarAttrib(EquipPos_2_PosId[EQUIP_POS_FASHION_HAIR]);
		if( bShow )
		{
			avatar->equipItem( EquipPos_2_PosId[EQUIP_POS_FASHION_HAIR], id | 2, 0 );
			avatar->equipItem( EquipPos_2_PosId[EQUIP_POS_SHOULDER], m_ArmEquipId[EQUIP_POS_SHOULDER], m_ArmStar[EQUIP_POS_SHOULDER] );
		}else{
			avatar->equipItem( EquipPos_2_PosId[EQUIP_POS_FASHION_HAIR], id ^ 2, 0 );
		}
		avatar->updateAttrib();
	}	
}
//   时装包相关
void Equip::send_SelectFashionSuit( int nSelectIdx )
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type = ADJUST_SELECT_FASHION_SUIT;
	msgbody.AdjustC.AdjustData.SelectFashionSuit.SelectIdx = nSelectIdx;
	SendGameServerMsg( ADJUST_CLT,msgbody );
}

void Equip::setSelectFashionBag( int nSelect, int nNum )
{
	m_nFashionSelect	= nSelect;
	m_nFashionNum		= nNum;
}

int Equip::getSelectFashionIndex()
{
	return m_nFashionSelect;
}
int Equip::getSelectFashionNum()
{
	return m_nFashionNum;
}