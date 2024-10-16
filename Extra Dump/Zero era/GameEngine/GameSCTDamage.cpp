#include "GameSCTDamage.h"
#include "GameActor.h"
#include "GameDBData.h"
#include <Windows.h>
#include <math.h>
#include <string>
#include "GameActorManager.h"
using namespace std;

extern bool isNormalSkill( int nSkillID, int nSkillLevel );
typedef void ( SCTDamageBase::*SCTProcessor )();

SCTDamageBase::SCTDamageBase() : m_nTotalHurts( 1 ), m_pActor( NULL )
{
	m_SCTProcessorMap[HURT_TYPE_DMG]		= &SCTDamageBase::calNormalDamageSCT;
	m_SCTProcessorMap[HURT_TYPE_HEAVY_DMG]	= &SCTDamageBase::calHeavyDamageSCT;
	m_SCTProcessorMap[HURT_TYPE_HITDEAD]	= &SCTDamageBase::calHeavyDamageSCT;
	m_SCTProcessorMap[HURT_TYPE_HP]			= &SCTDamageBase::calAddHPSCT;
	m_SCTProcessorMap[HURT_TYPE_MP]			= &SCTDamageBase::calAddMPSCT;
	m_SCTProcessorMap[HURT_TYPE_MISS]		= &SCTDamageBase::calMissSCT;
	m_SCTProcessorMap[HURT_TYPE_NOEFFECT]	= &SCTDamageBase::calImmuneSCT;
	m_SCTProcessorMap[HURT_TYPE_SBSORB]		= &SCTDamageBase::calSorbSCT;
	m_SCTProcessorMap[HURT_TYPE_EXP]		= &SCTDamageBase::calExperienceSCT;
	m_SCTProcessorMap[HURT_TYPE_MONEY]		= &SCTDamageBase::calAddMoneySCT;
	m_SCTProcessorMap[HURT_TYPE_BINDGOLD]	= &SCTDamageBase::calAddBindGoldSCT;
	m_SCTProcessorMap[HURT_TYPE_ZHENGYI]	= &SCTDamageBase::calAddZhengYiSCT;
	m_SCTProcessorMap[HURT_TYPE_LIFTBRAND]	= &SCTDamageBase::calAddLiftBrand;
}

void SCTDamageBase::preCalculateSCT( int nHitValue, int nSkillID, int nSkillLevel, GameActor* pActor, 
										int nHurtType, int nTotalHurts, int nHurtIndex )
{
	m_FlyText.reset();
	m_Font					= RoleText::tagFlyFont();
	m_Font.m_nAttackTime	= nHurtIndex;
	m_nHitValue				= nHitValue;
	m_bNormalSkill			= ( nSkillID == 0 ) ? false : isNormalSkill( nSkillID, nSkillLevel );
	m_pActor				= pActor;
	m_nHurtType				= nHurtType;	
	m_nTotalHurts			= nTotalHurts;

	if ( nSkillID != 0 && RoleText::hasFontType( nSkillID ) )
	{
		m_Font.m_pFontType = RoleText::getFontType( nSkillID );
	} 
}

void SCTDamageBase::showFllyFont( unsigned int uStartTime )
{
	ActorManager& actorManager = ActorManager::getSingleton();
	// 若不显示冒雪伤害
	if( !actorManager.getGameOpt( GAME_OPT_SHOW_MAINPLAYER_DAMAGE_SHOW ) )
	{
		return;
	}

	if ( m_FlyText.m_nFontType == -1 )
	{
		return;
	}

	int subType = 0;
	if ( m_pActor->getType() == GAT_MONSTER )
	{
		const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( m_pActor->getResId() );
		subType = monsterDef.SubType;
	}

	// 火药桶这类临时道具不冒伤害
	if( subType == MON_SUB_TYPE_TMP )
	{
		return;
	}

	m_Font.m_vectorText.push_back( m_FlyText );
	m_Font.m_uStartTime		= uStartTime;
	if ( m_Font.m_pFontType != NULL )
	{
		m_Font.m_uStartTime += m_Font.m_pFontType->m_nDelayTime;
	}
	//m_Font.m_nAttackTime	= -( m_Font.m_nAttackTime + ( -1 ) * ( m_nTotalHurts / 2 ) );
	m_pActor->getRoleText().AddFlyFont( m_Font );
}

void SCTDamageBase::calculateDamage( int nHitValue, int nSkillID, int nSkillLeve, GameActor* pActor, 
										int nHurtType, int nTotalHurts, int nHurtIndex )
{
	preCalculateSCT( nHitValue, nSkillID, nSkillLeve, pActor, nHurtType, nTotalHurts, nHurtIndex );
	SCTProcessor sct = getSCTCalculator( m_nHurtType );
	assert( sct != NULL && "多了一个未加定义的显示" );

	(this->*sct)();
}

void SCTDamageBase::calNormalDamageSCT()	
{
	if ( m_nHitValue == 0 )
	{
		return;
		//return attackMiss();
	}
}

void SCTDamageBase::calHeavyDamageSCT()	
{
	if ( m_nHitValue == 0 )
	{
		return;
		//return attackMiss();
	}
}

void SCTDamageBase::calMissSCT()
{
	attackMiss();
}

SCTProcessor SCTDamageBase::getSCTCalculator( int nHurtType )
{
	if ( m_SCTProcessorMap.find( nHurtType ) == m_SCTProcessorMap.end() )
	{
		return NULL;
	}

	return m_SCTProcessorMap[nHurtType];
}

void MainplayerAttack::calImmuneSCT()
{
	m_Font.m_pFontType		= RoleText::getFontType( 14 );
	m_FlyText.m_Text		= "5";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}

void MainplayerAttack::attackMiss()
{
	if ( m_bNormalSkill )
	{
		// 对方所受伤害为0,则显示闪避
		m_Font.m_pFontType		= RoleText::getFontType( 2 );
		m_FlyText.m_Text		= "2";
		m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
	}
	else
	{
		// 对方所受伤害为0,则显示闪避
		m_Font.m_pFontType		= RoleText::getFontType( 6 );
		m_FlyText.m_Text		= "1";
		m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
	}
}

void MainplayerAttack::calNormalDamageSCT()
{
	SCTDamageBase::calNormalDamageSCT();
	if ( m_nHitValue == 0 )
	{
		return;
	}

	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	if ( m_bNormalSkill )
	{
		m_Font.m_pFontType		= m_Font.m_pFontType == NULL ? RoleText::getFontType( 0 ) : m_Font.m_pFontType;
		m_FlyText.m_Text		= buffer;
		m_FlyText.m_nFontType	= NORMAL_MAIN_ATTACK_FONT;
	}
	else
	{	
		m_Font.m_pFontType		= m_Font.m_pFontType == NULL ? RoleText::getFontType( 4 ) : m_Font.m_pFontType;
		m_FlyText.m_Text		= buffer;
		m_FlyText.m_nFontType	= SKILL_MAIN_NORMAL_ATTACK_FONT;
	}
}

void MainplayerAttack::calHeavyDamageSCT()
{
	SCTDamageBase::calHeavyDamageSCT();
	
	if ( m_nHitValue == 0 )
	{
		return;
	}

	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	if ( m_bNormalSkill )
	{
		m_Font.m_pFontType		= m_Font.m_pFontType == NULL ? RoleText::getFontType( 1, true ) : m_Font.m_pFontType;
		m_FlyText.m_Text		= buffer;
		m_FlyText.m_nFontType	= NORMAL_MAIN_HEAVY_ATTACK_FONT;
	}
	else
	{	
		m_Font.m_pFontType		= m_Font.m_pFontType == NULL ? RoleText::getFontType( 5, true ) : m_Font.m_pFontType;
		m_FlyText.m_Text		= buffer;
		m_FlyText.m_nFontType	= SKILL_MAIN_HEAVY_ATTACK_FONT;
	}
}

void MainplayerAttack::calSorbSCT()
{
	m_Font.m_pFontType		= RoleText::getFontType( 14 );
	m_FlyText.m_Text		= "7";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}

void MainPetAttack::calNormalDamageSCT()
{
	SCTDamageBase::calNormalDamageSCT();
	if ( m_nHitValue == 0 )
	{
		return;
	}

	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 7 );
	m_FlyText.m_Text		= buffer;
	m_FlyText.m_nFontType	= NORMAL_MAIN_PET_ATTACK_FONT;
}

void MainPetAttack::calHeavyDamageSCT()
{
	SCTDamageBase::calHeavyDamageSCT();
	if ( m_nHitValue == 0 )
	{
		return;
	}

	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 8, true );
	m_FlyText.m_Text		= buffer;
	m_FlyText.m_nFontType	= HEAVY_MAINPET_ATTACK_FONT;
}

void MainPetAttack::calImmuneSCT()
{
	// TODO: 策划未定
	/*m_FlyText.m_Text		= "5";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
	m_Font.m_pFontType		= RoleText::getFontType( 14 );*/
}

void MainPetAttack::attackMiss()
{
	m_Font.m_pFontType		= RoleText::getFontType( 9 );
	m_FlyText.m_Text		= "0";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}

void MainPetAttack::calSorbSCT()
{
	m_Font.m_pFontType		= RoleText::getFontType( 14 );
	m_FlyText.m_Text		= "7";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}

void MainplayerHurted::calNormalDamageSCT()
{
	SCTDamageBase::calNormalDamageSCT();
	if ( m_nHitValue == 0 )
	{
		return;
	}

	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 10 );
	m_FlyText.m_Text		= buffer;
	m_FlyText.m_nFontType	= NORMAL_MAIN_DAMAGED_FONT;
}

void MainplayerHurted::calHeavyDamageSCT()
{
	SCTDamageBase::calHeavyDamageSCT();
	if ( m_nHitValue == 0 )
	{
		return;
	}

	char buffer[80];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 11, true );
	m_FlyText.m_Text		= buffer;
	m_FlyText.m_nFontType	= HEAVY_MAIN_DAMAGED_FONT;
}

void MainplayerHurted::calAddHPSCT()
{
	char buffer[40];
	sprintf( buffer, "%d", abs( m_nHitValue ) );

	m_Font.m_pFontType		= RoleText::getFontType( 15 );
	m_FlyText.m_Text		= m_nHitValue < 0 ? buffer : ( std::string( "A" ) + std::string( buffer ) );
	m_FlyText.m_nFontType	= HP_ADD_FONT;
}

void MainplayerHurted::calAddMPSCT()
{
	char buffer[40];
	sprintf( buffer, "%d", abs( m_nHitValue ) );

	m_Font.m_pFontType		= RoleText::getFontType( 16 );
	if (m_nHitValue > 0)
	{
		m_FlyText.m_Text		= std::string("A") + buffer;
	}
	else if (m_nHitValue < 0)
	{
		m_FlyText.m_Text		= std::string("B") + buffer;
	}
	else
	{
		m_FlyText.m_Text		= "";
	}
	m_FlyText.m_nFontType	= MP_ADD_FONT;
}

void MainplayerHurted::calImmuneSCT()
{
	m_Font.m_pFontType		= RoleText::getFontType( 13 );
	m_FlyText.m_Text		= "4";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}

void MainplayerHurted::calExperienceSCT()
{
	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 17 );
	m_FlyText.m_Text		= std::string( "A" ) + std::string( buffer );
	m_FlyText.m_nFontType	= EXP_ADD_FONT;
}

void MainplayerHurted::calAddMoneySCT()
{
	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 23 );
	m_FlyText.m_Text		= std::string( "A" ) + std::string( buffer );
	m_FlyText.m_nFontType	= MONEY_ADD_FONT;
}

void MainplayerHurted::calAddBindGoldSCT()
{
	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 24 );
	m_FlyText.m_Text		= std::string( "A" ) + std::string( buffer );
	m_FlyText.m_nFontType	= BINDGOLD_ADD_FONT;
}

void MainplayerHurted::calAddZhengYiSCT()
{
	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 17 );
	m_FlyText.m_Text		= std::string( "A" ) + std::string( buffer );
	m_FlyText.m_nFontType	= ZHENGYI_ADD_FONT;
}

void MainplayerHurted::calAddLiftBrand()
{
	char buffer[40];
	sprintf( buffer, "%d", m_nHitValue );

	m_Font.m_pFontType		= RoleText::getFontType( 17 );
	m_FlyText.m_Text		= std::string( "A" ) + std::string( buffer );
	m_FlyText.m_nFontType	= LIFTBRAND_ADD_FONT;
}

void MainplayerHurted::attackMiss()
{
	m_Font.m_pFontType		= RoleText::getFontType( 12 );
	m_FlyText.m_Text		= "3";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}

void MainplayerHurted::calSorbSCT()
{
	m_Font.m_pFontType		= RoleText::getFontType( 14 );
	m_FlyText.m_Text		= "6";
	m_FlyText.m_nFontType	= MISS_IMMUNE_FONT;
}
