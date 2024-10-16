#pragma once
#ifndef _PWORLDSCORE__
#define _PWORLDSCORE__

#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "Item.h"
#include <set>

class PworldScore//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<PworldScore>
{//tolua_export
public:
	//tolua_begin
	struct Card 
	{
		char name[NAME_LEN];
		Item item;
	};
	enum
	{
		D = PWORLD_AWARD_VAL_D,
		C = PWORLD_AWARD_VAL_C,
		B = PWORLD_AWARD_VAL_B,
		A = PWORLD_AWARD_VAL_A,
		S = PWORLD_AWARD_VAL_S,
	};
	struct Info
	{
		int		DiffPworldID;					/*   �Ѷȸ���ID��DiffPworldDef */
		int		PworldID;						/*   ����ID��ѡ����Ѷ� */
		int		Normal;							/*   ��ͨ����÷� */
		int		JingYing;						/*   ��Ӣ����÷� */
		int		Boss;							/*   BOSS�÷� */
		int		Time;							/*   ʱ��÷� */
		int		TimeCost;						/*   ͨ��ʱ�� */
		int		Death;							/*   �����۷� */
		int		DeathNum;						/*   �������� */
		int		Total;							/*   �ܷ� */
		int		Grade;							/*   ���ۼ��� Bind Macrosgroup:PWORLD_AWARD_VAL_TYPE,*/
		int		Exp;							/*   ���� */
		int		Money;							/*   ��Ǯ */
		int		GradeExp;						/*   ���۾��� */
		int		DiffExp;						/*   �ѶȾ��� */
		int		FriendExp;						/*   ���Ѿ��� */
		int		ClanExp;						/*   ���ᾭ�� */
		int		CardNum;						/*   �Ƶ����� */
		Item	Cards[MAX_PWORLD_AWARD_CARD]; 	/*   �ƶ�Ӧ����Ʒ */
		int		OffExp;                          	/*  Ver.911 ������������� */
		int		NormalNum;                       	/*  Ver.947 ��ͨ���� */
		int		JingNum;                         	/*  Ver.947 ��Ӣ���� */
		int		BossNum;                         	/*  Ver.947 BOSS */
		int		VipCardNum;                      	/*  Ver.1950 �Ƶ����� */
		Item	VipCards[MAX_PWORLD_VIP_AWARD_CARD]; 	/*  Ver.1950 �ƶ�Ӧ����Ʒ */

		const Info &	operator = (const tagCSDiffPworldAwardUI &info);
	};
	//tolua_end
public:
	PworldScore();
	~PworldScore();

public:
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	//tolua_begin
	void	SelectCard(int index);
	bool	IsSelected();
	int		GetSelected();
	bool	IsCardSelected(int idx);
	int		GetNextSelecteCard(int idx = -1);
	//vip
	void	SelectVipCard(int index);
	bool	IsVipSelected();
	int		GetVipSelected();
	bool	IsVipCardSelected(int idx);
	int		GetNextVipSelectCard(int idx = -1);
	//tolua_end

private:
	void	reset();

public:
	//tolua_begin
	Card			AwardCards[MAX_PWORLD_AWARD_CARD];
	Card			VipAwardCards[MAX_PWORLD_VIP_AWARD_CARD];
	Info			AwardInfo;
	//tolua_end

private:
	typedef std::set<int> IntList;
	IntList	selectList;
	bool	selected;
	int		selectedIdx;

	IntList	vipSelectList;
	bool	vipSelected;
	int		vipSelectedIdx;
	
};//tolua_export


#endif