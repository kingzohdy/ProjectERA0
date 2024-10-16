#pragma once
#include "GameActor_RoleText.h"
#include <map>
#include "proto_comm.h"
#include "proto_cs.h"

enum	tagHURT_TYPE_EXTEND
{
	HURT_TYPE_EXP = HURT_TYPE_SBSORB + 1,
	HURT_TYPE_MONEY,
	HURT_TYPE_BINDGOLD,
	HURT_TYPE_ZHENGYI,
	HURT_TYPE_LIFTBRAND,
};

class GameActor;
class SCTDamageBase
{
public:
	SCTDamageBase();
	void	showFllyFont( unsigned int uStartTime );
	void	calculateDamage( int nHitValue, int nSkillID, int nSkillLeve, GameActor* pActor, 
								int nHurtType, int nTotalHurts, int nHurtIndex );

	virtual void	calNormalDamageSCT();
	virtual void	calHeavyDamageSCT();
	virtual void	calAddHPSCT()			{};
	virtual void	calAddMPSCT()			{};
	virtual void	calMissSCT();
	virtual void	calSorbSCT()			{};
	virtual void	calImmuneSCT()			{};
	virtual void	calExperienceSCT()		{};
	virtual void	calAddMoneySCT()		{};
	virtual void	calAddBindGoldSCT()		{};
	virtual void	calAddZhengYiSCT()		{};
	virtual void	calAddLiftBrand()		{};
protected:
	virtual void	attackMiss()	{}

protected:
	tagFlyText				m_FlyText;
	RoleText::tagFlyFont	m_Font;

	int						m_nHitValue;
	int						m_nHurtType;
	bool					m_bNormalSkill;
	int						m_nTotalHurts;
	GameActor*				m_pActor;

private:

	typedef void ( SCTDamageBase::*SCTProcessor )();

	void			preCalculateSCT( int nHitValue, int nSkillID, int nSkillLeve, GameActor* pActor, int nHurtType, 
										int nTotalHurts, int nHurtIndex );
	SCTProcessor	getSCTCalculator( int nHurtType );

private:
	std::map<int, SCTProcessor>		m_SCTProcessorMap;
};

class MainplayerAttack: public SCTDamageBase
{
public:
	// 只实现扣血的处理计算
	virtual void	calNormalDamageSCT();
	// 只实现扣血的处理计算
	virtual void	calHeavyDamageSCT();

	virtual void	calImmuneSCT();
	virtual void	calSorbSCT();

private:
	virtual void	attackMiss();
};

class MainPetAttack: public SCTDamageBase
{
public:
	// 只实现扣血的处理计算
	virtual void	calNormalDamageSCT();
	// 只实现扣血的处理计算
	virtual void	calHeavyDamageSCT();
	virtual void	calImmuneSCT();
	virtual void	calSorbSCT();

private:
	virtual void	attackMiss();
};

class MainplayerHurted : public SCTDamageBase
{
public:
	// 只实现扣血的处理计算
	virtual void	calNormalDamageSCT();
	// 只实现扣血的处理计算
	virtual void	calHeavyDamageSCT();

	virtual void	calAddHPSCT();
	virtual void	calAddMPSCT();

	virtual void	calImmuneSCT();
	virtual void	calSorbSCT();
	virtual void	calExperienceSCT();
	virtual void	calAddMoneySCT();
	virtual void	calAddBindGoldSCT();
	virtual void	calAddZhengYiSCT();
	virtual void	calAddLiftBrand();
private:
	virtual void	attackMiss();
};