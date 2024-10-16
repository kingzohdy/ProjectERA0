#pragma once 
#include "GamePet.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "OgreRect.h"
#include "GameMaskMap.h"
#include "GameMap.h"
#include "OgreScriptLuaVM.h"
#include "OgreVector2.h"

using namespace Ogre;

GamePet::GamePet(void): m_pOwner(NULL)
{
	memset( m_RoleXianPets, 0, sizeof( ROLEPET ) * MAX_PET_XIAN_PACKAGE );
	memset( m_RoleShiPets, 0, sizeof( ROLEPET ) * MAX_PET_SHI_PACKAGE );
	for( int i = 0; i < PET_MAX_FIGHT_SHIS; i++ )
	{
		m_pShiActor[i] = NULL;
	}
}

GamePet::~GamePet(void)
{
	for( int i = 0; i < PET_MAX_FIGHT_SHIS; i++ )
	{
		if( m_pShiActor[i] != NULL )
		{
			ActorManager::getSingleton().DestroyActor( m_pShiActor[i], true );
			m_pShiActor[i] = NULL;
		}
	}
}

// 涅槃 
void GamePet::petNirvana( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_NIRVANA;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 下蛋
void GamePet::petEgg( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_LAY_EGG;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 准备出征
void GamePet::petPre( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_CAMPAIGN_PRE;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 

	ActorManager::getSingleton().getScriptVM()->callString("PetCampaign()");
}

// 出征
void GamePet::petCampaign( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_CAMPAIGN;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 见习
void GamePet::petNoviciate( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_NOVICIATE;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 后备
void GamePet::petInSupport( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_BACKUP;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 转生
void GamePet::petRelive( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_ZHUANSHENG;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 技能学习
void GamePet::petSkillUp( int nType, int nId, int nLv, int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType						= PET_CLT_SKILL_UP;
	msgbody.PetClt.PetCltData.SkillUp.SkillType		= nType;
	msgbody.PetClt.PetCltData.SkillUp.SkillID		= nId;
	msgbody.PetClt.PetCltData.SkillUp.SkillLevel	= nLv;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid	= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 修改宠物名
void GamePet::petChangeName( int nHightId, int nLowId, const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType	= PET_CLT_CHG_NAME;
	memset( msgbody.PetClt.PetCltData.ChgName.PetName, 0, ROLE_NAME_LEN_CLT * sizeof( char ) );
	strncpy( msgbody.PetClt.PetCltData.ChgName.PetName, szName, strlen( szName ) );
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.ChgName.PetWid = *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
}	

// 放生侍卫
void GamePet::petFreedomShi( int nIndex )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		pMainPlayer->getContainer().send_MoveItem( LIST_TYPE_SHI, -1, nIndex, 1, 0 );
	}	
}

// 放生仙子
void GamePet::petFreedomXian( int nIndex )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		pMainPlayer->getContainer().send_MoveItem( LIST_TYPE_XIAN, -1, nIndex, 1, 0 );
	}	
}

// 宠物穿装备
void GamePet::petWearArm( int nArmId, int nListType, int nGridInex, int nHightId, int nLowId )
{	
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type						= ADJUST_TYPE_WEAR;
	msgbody.AdjustC.AdjustData.Wear.ArmDefID	= nArmId;
	msgbody.AdjustC.AdjustData.Wear.GridIdx		= nGridInex;
	msgbody.AdjustC.AdjustData.Wear.ListType	= nListType;
	msgbody.AdjustC.AdjustData.Wear.WearPos		= 0;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.AdjustC.AdjustData.Wear.PetWID		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 宠物脱装备
void GamePet::petUnWearArm( int nArmId, int nListType, int nGridInex, int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.AdjustC.Type						= ADJUST_TYPE_UNWEAR;
	msgbody.AdjustC.AdjustData.UnWear.ArmDefID	= nArmId;
	msgbody.AdjustC.AdjustData.UnWear.GridIdx	= nGridInex;
	msgbody.AdjustC.AdjustData.UnWear.ListType	= nListType;
	msgbody.AdjustC.AdjustData.UnWear.WearPos	= 0;
	tagPetId petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.AdjustC.AdjustData.UnWear.PetWID	= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( ADJUST_CLT, msgbody );
}

// 注册网络消息
void GamePet::initPetNetMessage()
{
	INetworkManager::getSingleton().RegisterMsgHandler( PET_SVR, this );
}

// 宠物包裹变化
void GamePet::petPackageChange( PETPACKAGECHG* pPackageChange )
{
	int nPackageNum = pPackageChange->ChgNum;
	for( int i = 0; i < nPackageNum; i++ )
	{
		if( pPackageChange->Pets[i].ListType == LIST_TYPE_XIAN )
		{
			int nIndex = pPackageChange->Pets[i].Pet.GridIdx;
			if( nIndex == -1 )
			{
				for( int j = 0; j < m_nRoleXianMaxNum; j++ )
				{
					if( pPackageChange->Pets[i].Pet.WID == m_RoleXianPets[j].WID && pPackageChange->Pets[i].Pet.WID != 0 )
					{
						m_RoleXianPets[j].WID = 0;
						m_nRoleXianNum--;
					}
				}
			}else{
				if( m_RoleXianPets[nIndex].WID == 0 )
				{
					m_nRoleXianNum++;
				}
				m_RoleXianPets[nIndex] = pPackageChange->Pets[i].Pet;
			}
		}

		if( pPackageChange->Pets[i].ListType == LIST_TYPE_SHI )
		{
			int nIndex = pPackageChange->Pets[i].Pet.GridIdx;
			if( nIndex == -1 ) 
			{
				for( int j = 0; j < m_nRoleShiMaxNum; j++ )
				{
					if( pPackageChange->Pets[i].Pet.WID == m_RoleShiPets[j].WID && pPackageChange->Pets[i].Pet.WID != 0 )
					{
						m_RoleShiPets[j].WID = 0;
						m_nRoleShiNum--;
					}
				}
			}else{
				if( m_RoleShiPets[nIndex].WID == 0 )
				{
					m_nRoleShiNum++;
				}
				m_RoleShiPets[nIndex] = pPackageChange->Pets[i].Pet;
			}
		}
	}
	ActorManager::getSingleton().getScriptVM()->callString("PetPackageUpdate()");
}

int GamePet::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == PET_SVR )
	{
		int nPetNum = msgbody.PetSvr.Num;
		for( int i = 0; i < nPetNum; i++ )
		{
			int				nAttrNum	= msgbody.PetSvr.PetChgDataList[i].AttrNum;
			tdr_ulonglong	ulPetId		= msgbody.PetSvr.PetChgDataList[i].Wid;			

			for( int j = 0; j < m_nRoleXianNum; j++ )
			{
				if( ulPetId == m_RoleXianPets[j].WID )
				{
					setPetAttr( m_RoleXianPets[j], (CSPETATTRCHG*)&msgbody.PetSvr.PetChgDataList[i].AttrChgList[j] );						
				}
			}

			for( int j = 0; j < m_nRoleShiNum; j++ )
			{
				if( ulPetId == m_RoleShiPets[j].WID )
				{
					setPetAttr( m_RoleShiPets[j], (CSPETATTRCHG*)&msgbody.PetSvr.PetChgDataList[i].AttrChgList[j] );				
					char szScript[256];
					sprintf( szScript, "UpdateTargetPetUI(%d)", j );
					ActorManager::getSingleton().getScriptVM()->callString( szScript );
				}
			}
		}
		ActorManager::getSingleton().getScriptVM()->callString("PetPackageUpdate()");
	}
	return 0;
}

void GamePet::setOwner( GameActor* pOwner )
{
	m_pOwner = pOwner;
}

void GamePet::initPetPackage( tagPetPackage* petInfo )
{
	m_nRoleXianNum		= petInfo->XianNum;
	m_nRoleXianMaxNum	= petInfo->MaxXianNum;
	m_nRoleShiNum		= petInfo->ShiNum;
	m_nRoleShiMaxNum	= petInfo->MaxShiNum;
	memset( m_RoleXianPets, 0, sizeof( ROLEPET ) * MAX_PET_XIAN_PACKAGE );
	memset( m_RoleShiPets, 0, sizeof( ROLEPET ) * MAX_PET_SHI_PACKAGE );

	for( int i = 0; i < m_nRoleXianNum; i++ )
	{
		int nIndex = petInfo->RoleXianPets[i].GridIdx;
		m_RoleXianPets[nIndex] = petInfo->RoleXianPets[i];
	}
	for( int i = 0; i < m_nRoleShiNum; i++ )
	{
		int nIndex = petInfo->RoleShiPets[i].GridIdx;
		m_RoleShiPets[nIndex] = petInfo->RoleShiPets[i];
	}
	ActorManager::getSingleton().getScriptVM()->callString("PetPackageUpdate()");
}

// 获取仙子数量
int GamePet::getXianNum()
{
	return m_nRoleXianNum;
}

// 获取侍卫数量
int GamePet::getShiNum()
{
	return m_nRoleShiNum;
}

// 获取仙子数量
int GamePet::getXianMaxNum()
{
	return m_nRoleXianMaxNum;
}

// 获取侍卫数量
int GamePet::getShiMaxNum()
{
	return m_nRoleShiMaxNum;
}

// 获取宠物当前等级最大经验( type 0 仙子 1宠物 )
int GamePet::getPetMaxExp( int nType, int nlv )
{
	tagPetLevelDef PetLevelDef = DBDataManager::getSingleton().getPetLvDef(nlv);
	if( nType == 0 )
	{
		return PetLevelDef.XianExp;
	}

	return PetLevelDef.ShiExp;
}

// 获取宠物战斗力
int GamePet::getPetFightVal( int id )
{
	tagPetPinZhiDef PetPinZhiDef = DBDataManager::getSingleton().getPetPinzhiDef( id );
	return PetPinZhiDef.FightValAdd;
}

// 获取宠物关键字信息
tagAddAttrDef_ToLua* GamePet::getAddAttrDef( int id )
{
	assert( sizeof(tagAddAttrDef_ToLua) == sizeof(tagAddAttrDef) );
	return (tagAddAttrDef_ToLua*)&(DBDataManager::getSingleton().getAddAttrDef(id));
}

// 获取宠物信息( type 0 仙子 1宠物 )
tagRolePet_ToLua* GamePet::getPetInfo( int nType, int nIndex )
{
	assert( sizeof(tagRolePet_ToLua) == sizeof(tagRolePet) );
	if( nType == 0 )
	{
		return (tagRolePet_ToLua* )&m_RoleXianPets[nIndex];
	}

	return (tagRolePet_ToLua* )&m_RoleShiPets[nIndex];
}

// 获取宠物静态信息
tagPetDef_ToLua* GamePet::getPetDefInfo( int id )
{
	assert( sizeof(tagPetDef_ToLua) == sizeof(tagPetDef) );
	return (tagPetDef_ToLua*)&(DBDataManager::getSingleton().getPetDef(id));
}

// 设置侍卫模型出征
void GamePet::setShiModle( int nOwnerId, int nXieDaiPos, int nPetId, const char* szPetName, int nPower, int nMaxPower )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return;
	}

	if( m_pShiActor[nXieDaiPos] == NULL )
	{
		if( nOwnerId == 0 )
		{
			return;
		}

		const tagPetDef& petDef	= DBDataManager::getSingleton().getPetDef( nPetId );
		int nPetDyId			= nOwnerId + ( nXieDaiPos + 1 ) * PET_ID_BASE;	
		m_pShiActor[nXieDaiPos]	= ActorManager::getSingleton().CreateActor( nPetDyId, GAT_PET, petDef.MouldID );
		m_pShiActor[nXieDaiPos]->setName( szPetName );
		m_pShiActor[nXieDaiPos]->OnEnterMap( pMainPlayer->getCurMap() );

		m_pShiActor[nXieDaiPos]->setType( GAT_PET );
		m_pShiActor[nXieDaiPos]->setSubType();
		m_pShiActor[nXieDaiPos]->setAtkType();
		m_pShiActor[nXieDaiPos]->ChangeActState( GSACT_STAND );
		m_pShiActor[nXieDaiPos]->setSpeed( 300 );
		// 设置位置
		setPetPostion( nXieDaiPos );
		// 设置血量
		m_pShiActor[nXieDaiPos]->setHealth( (float)nPower*100/nMaxPower );
		
		if( m_pOwner->m_ObjCamp == SKILL_TARGET_FRIEND_PLAYER )
		{ 
			m_pShiActor[nXieDaiPos]->m_ObjCamp |= SKILL_TARGET_FRIENDPET;
		}else if( m_pOwner->m_ObjCamp == SKILL_TARGET_MID_PLAYER )
		{
			m_pShiActor[nXieDaiPos]->m_ObjCamp |= SKILL_TARGET_MIDPET;
		}else if( m_pOwner->m_ObjCamp == SKILL_TARGET_SELF )
		{
			m_pShiActor[nXieDaiPos]->m_ObjCamp |= SKILL_TARGET_SELF;
		}
		
		if( m_pOwner == ActorManager::getSingleton().getMainPlayer() )
		{
			char szScript[256];
			sprintf( szScript, "UpdateTargetPetUI(%d)", nXieDaiPos );
			ActorManager::getSingleton().getScriptVM()->callString( szScript );
		}
		
		return;
	}else{
		if( nOwnerId == 0 )
		{
			m_pShiActor[nXieDaiPos]->OnLeaveMap();
			ActorManager::getSingleton().DestroyActor( m_pShiActor[nXieDaiPos], true );
			m_pShiActor[nXieDaiPos] = NULL;
			return;
		}
	}
	assert(0);
}

// 重新显示侍卫模型
void GamePet::reShowModle()
{
	for( int i = 0; i < PET_MAX_FIGHT_SHIS; i++ )
	{
		if( m_pShiActor[i] != NULL )
		{
			setPetPostion( i );
		}
	}
}

// 获得侍卫实体
GameActor* GamePet::getPetActor( int nXieDaiPos )
{
	return m_pShiActor[nXieDaiPos];
}

// 设置宠物属性
void GamePet::setPetAttr( ROLEPET& petAttr, CSPETATTRCHG* pdata )
{
	switch( pdata->Type )
	{
	case PET_CHG_NAME:
		{
			memcpy( petAttr.Name, pdata->AttrValue.ChgName, strlen(pdata->AttrValue.ChgName)+1 );
		}
		break;
	case PET_CHG_EXP:
		{
			petAttr.Exp = pdata->AttrValue.ChgExp;
		}
		break;
	case PET_CHG_SPOINT:
		{
			petAttr.SkillPoint = pdata->AttrValue.ChgSkillPoint;
		}
		break;
	case PET_CHG_MAXPOWER:
		{
			petAttr.MaxPower = pdata->AttrValue.ChgMaxPower.MaxPower;
			if( pdata->AttrValue.ChgPower.XieDaiPos >= 0 )
			{
				int nXieDaiPos = pdata->AttrValue.ChgPower.XieDaiPos;
				if( m_pShiActor[nXieDaiPos] != NULL )
				{
					m_pShiActor[nXieDaiPos]->setMaxHP( petAttr.MaxPower );
					m_pShiActor[nXieDaiPos]->setHealth( (float)petAttr.Power*100/petAttr.MaxPower );
				}
			}
		}
		break;
	case PET_CHG_POWER:
		{
			petAttr.Power = pdata->AttrValue.ChgPower.Power;
			if( pdata->AttrValue.ChgPower.XieDaiPos >= 0 )
			{
				int nXieDaiPos = pdata->AttrValue.ChgPower.XieDaiPos;
				if( m_pShiActor[nXieDaiPos] != NULL )
				{
					m_pShiActor[nXieDaiPos]->setHP( petAttr.Power );
					m_pShiActor[nXieDaiPos]->setHealth( (float)petAttr.Power*100/petAttr.MaxPower );
				}
			}
		}
		break;
	case PET_CHG_LEVEL:
		{
			petAttr.Level = pdata->AttrValue.ChgLevel;
		}
		break;
	case PET_CHG_EGGNUM:
		{
			petAttr.GenEggNum = pdata->AttrValue.ChgGenEggNum;
		}
		break;
	case PET_CHG_ZHUANSHENG:
		{
			petAttr.ZhuanShen = pdata->AttrValue.ChgZhuanShen;
		}
		break;
	case PET_CHG_AGE:
		{
			petAttr.Age	= pdata->AttrValue.ChgAge;
		}
		break;
	case PET_CHG_ARM:
		{
			petAttr.PetArms.Num		= pdata->AttrValue.ChgPetArms.Num;
			petAttr.PetArms.MaxNum	= pdata->AttrValue.ChgPetArms.MaxNum;
			for( int i = 0; i < petAttr.PetArms.Num; i++ )
			{
				int nIndex = pdata->AttrValue.ChgPetArms.Arms[i].GridIdx;
				if( nIndex == -1 )
				{
					for( int j = 0; j < MAX_PET_ARM; j++ )
					{
						if( petAttr.PetArms.Arms[j].WID == pdata->AttrValue.ChgPetArms.Arms[i].WID )
						{
							petAttr.PetArms.Arms[j].WID = 0;
						}
					}
				}else{
					petAttr.PetArms.Arms[nIndex] = pdata->AttrValue.ChgPetArms.Arms[i];
				}
			}
		}
		break;
	case PET_CHG_SKILL:
		{
			petAttr.PetSkills.SkillNum		= pdata->AttrValue.ChgPetSkills.SkillNum;
			petAttr.PetSkills.MaxNum		= pdata->AttrValue.ChgPetSkills.MaxNum;
			petAttr.PetSkills.DiathesisNum	= pdata->AttrValue.ChgPetSkills.DiathesisNum;
			memset( petAttr.PetSkills.Skills, 0, sizeof( PETSKILLONE ) * MAX_PET_SKILL );
			memset( petAttr.PetSkills.Diathesis, 0, sizeof( PETDIATHESISONE ) * MAX_PET_DIATHESIS );
			for( int i = 0; i < petAttr.PetSkills.SkillNum; i++ )
			{
				petAttr.PetSkills.Skills[i] = pdata->AttrValue.ChgPetSkills.Skills[i];
			}
			for( int i = 0; i < petAttr.PetSkills.DiathesisNum; i++ )
			{
				petAttr.PetSkills.Diathesis[i] = pdata->AttrValue.ChgPetSkills.Diathesis[i];
			}
		}
		break;
	case PET_CHG_ATTRS:
		{
			petAttr.PetAttrs.ForgetNum	= pdata->AttrValue.CHgPetAttrs.ForgetNum;
			petAttr.PetAttrs.Num		= pdata->AttrValue.CHgPetAttrs.Num;
			memset( petAttr.PetAttrs.AddAttrID, 0, sizeof( unsigned short ) * MAX_PET_ATTR );
			memset( petAttr.PetAttrs.AddAttrIDForget, 0, sizeof( unsigned short ) * MAX_PET_ATTR_FORGET_SAVE );
			for( int i = 0; i < petAttr.PetAttrs.Num; i++ )
			{
				petAttr.PetAttrs.AddAttrID[i] = pdata->AttrValue.CHgPetAttrs.AddAttrID[i];
			}
			for( int i = 0; i < petAttr.PetAttrs.ForgetNum; i++ )
			{
				petAttr.PetAttrs.AddAttrIDForget[i] = pdata->AttrValue.CHgPetAttrs.AddAttrIDForget[i];
			}
		}
		break;
	case PET_CHG_STAT:
		{
			petAttr.StatFlag = pdata->AttrValue.ChgStatFlag;
		}
		break;
	}
}

// 设置宠物位置
void GamePet::setPetPostion( int nXieDaiPos )
{
	if( m_pOwner != NULL )
	{
		if( m_pShiActor[nXieDaiPos]->getCurMap() != m_pOwner->getCurMap() )
		{
			m_pShiActor[nXieDaiPos]->OnEnterMap( m_pOwner->getCurMap() );
		}
		int x = m_pOwner->GetPosition().x/10;
		int y = m_pOwner->GetPosition().z/10;
	
		switch( nXieDaiPos )
		{
		case 0:
			{
				m_pShiActor[nXieDaiPos]->SetPosition( x, y );
			}
			break;
		case 1:
			{
				m_pShiActor[nXieDaiPos]->SetPosition( x, y );
			}
			break;
		case 2:
			{
				m_pShiActor[nXieDaiPos]->SetPosition( x, y );
			}
			break;
		}
	}
}

// 宠物移动
void GamePet::moveTo( int nXiDaiPos, int to_x, int to_z, int dis )
{
	int nOff_X = rand()%400 - 200;
	int nOff_Y = rand()%400 - 200;
	to_x = to_x+nOff_X;
	to_z = to_z+nOff_Y;

	GameActor* pActor = m_pShiActor[nXiDaiPos];
	if( pActor->getSpeed() == 0 )
	{
		return;
	}

	GameXYZ pos2	= pActor->GetPosition();
	GameXYZ pos		= m_pOwner->GetPosition();
	int nDis		= pos2.subtract(pos).length();
	if( nDis >= 500 )
	{
		pActor->setSpeed( m_pOwner->getSpeed() + 100 );
	}else{
		if( ActorManager::getSingleton().getMainPlayer()->getSpeed() - pActor->getSpeed() > 100 )
		{
			pActor->setSpeed( ActorManager::getSingleton().getMainPlayer()->getSpeed() );
		}else{
			pActor->setSpeed( 350 );
		}
	}

	Point2D end(to_x, to_z);
	Point2D scale(pActor->getCurMap()->getMaskMap()->m_lMaskPixelWidth, pActor->getCurMap()->getMaskMap()->m_lMaskPixelHeight);
	Point2D s_EndGrid = end / scale;
	if( !pActor->getCurMap()->getMaskMap()->canWalk(s_EndGrid.x, s_EndGrid.y, 1 ) )
	{
		if( !pActor->getCurMap()->getMaskMap()->findPointWalkable( end, 1 ) )
		{
			return;
		}else{
			to_x	= end.x;
			to_z	= end.y; 
		}
	}
	
	int cur_x, cur_z;
	pActor->m_pMapMover->GetCurPos(cur_x, cur_z);

	if( cur_x == to_x && cur_z == to_z )
	{
		return;
	}

	if( dis != 0 )
	{
		Vector2 end( to_x, to_z );
		Vector2 begin( cur_x, cur_z );
		float length = (end-begin).length();
		if( length < dis && pActor->getActState().GetState() == GSACT_MOVE )
		{
			int cur_x, cur_z;
			pActor->m_pMapMover->GetCurPos(cur_x, cur_z);
			pActor->m_pMapMover->StopOnPoint( cur_x, cur_z );
			pActor->ChangeActState( GSACT_STAND );
			return;
		}
		to_z = to_z + float(cur_z - to_z )/length*dis;
		to_x = to_x + float(cur_x - to_x )/length*dis;	
		if( to_x == cur_x && to_z == cur_z )
		{
			return;
		}
	}

	GameMaskPath path;
	if( !pActor->getCurMap()->FindPath(path, cur_x, cur_z, to_x, to_z, 1, true ) )
	{
		return;
	}

	size_t npoint = path.getNumPoint();
	assert(npoint > 0);
	pActor->m_pMapMover->StartOnPath(&path);

	pActor->ChangeActState(GSACT_MOVE);
}

// 刷新宠物移动
void GamePet::updatePetMoveSpe()
{
	for( int i = 0; i < PET_MAX_FIGHT_SHIS; i++ )
	{
		if( m_pShiActor[i] != NULL )
		{
			GameXYZ pos2	= m_pShiActor[i]->GetPosition();
			GameXYZ pos		= m_pOwner->GetPosition();
			int nDis		= pos2.subtract(pos).length();
			if( nDis >= 500 && nDis < 800 || nDis >= 1000 && m_pShiActor[i]->getSpeed() < m_pOwner->getSpeed() )
			{
				m_pShiActor[i]->setSpeed( m_pOwner->getSpeed() + 100 );
			}
			if( nDis >= 800 && nDis < 1000 || nDis < 500 && m_pShiActor[i]->getSpeed() > m_pOwner->getSpeed() )
			{
				m_pShiActor[i]->setSpeed( m_pOwner->getSpeed() - 100 );
			}
		}
	}
}
