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
		int		DiffPworldID;					/*   难度副本ID，DiffPworldDef */
		int		PworldID;						/*   副本ID，选择的难度 */
		int		Normal;							/*   普通怪物得分 */
		int		JingYing;						/*   精英怪物得分 */
		int		Boss;							/*   BOSS得分 */
		int		Time;							/*   时间得分 */
		int		TimeCost;						/*   通关时间 */
		int		Death;							/*   死亡扣分 */
		int		DeathNum;						/*   死亡次数 */
		int		Total;							/*   总分 */
		int		Grade;							/*   评价级别 Bind Macrosgroup:PWORLD_AWARD_VAL_TYPE,*/
		int		Exp;							/*   经验 */
		int		Money;							/*   金钱 */
		int		GradeExp;						/*   评价经验 */
		int		DiffExp;						/*   难度经验 */
		int		FriendExp;						/*   好友经验 */
		int		ClanExp;						/*   公会经验 */
		int		CardNum;						/*   牌的数量 */
		Item	Cards[MAX_PWORLD_AWARD_CARD]; 	/*   牌对应的物品 */
		int		OffExp;                          	/*  Ver.911 活动副本补偿经验 */
		int		NormalNum;                       	/*  Ver.947 普通怪物 */
		int		JingNum;                         	/*  Ver.947 精英怪物 */
		int		BossNum;                         	/*  Ver.947 BOSS */
		int		VipCardNum;                      	/*  Ver.1950 牌的数量 */
		Item	VipCards[MAX_PWORLD_VIP_AWARD_CARD]; 	/*  Ver.1950 牌对应的物品 */

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