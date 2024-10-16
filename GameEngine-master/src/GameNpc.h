#pragma once

#include "GameActor.h"
#include "GameMsgPrint.h"

struct tagNpcQusetInfo   
{
	std::vector<int>	    m_HaveAcceptDynamicQuestId;	// 所拥有接的动态任务列表
	std::vector<int>	    m_HaveAcceptQuestId;	// 所拥有接的任务列表
	std::vector<int>	    m_HaveFiniQuestId;		// 所拥有交的任务列表
	std::vector<CYCLEGRADE>	m_HaveAcceptRecycle;	// 所拥有的接的环组任务列表
	std::vector<CYCLEGRADE>	m_HaveFiniRecycle;		// 所拥有交的环组任务列表	
	int					    m_nQuestShow;			// 头顶显示的标志
	
};

// 获取任务列表

bool isFixGetTask( int nQuestID );
extern void getQuestList( int npcId );
extern tagNpcQusetInfo& getNpcQuestInfo( int npcId ); 

//tolua_begin
void clearNpcQuestList();
enum
{
	MONSTER_ORANGE,
	MONSTER_RED,
	MONSTER_WHITE,
	MONSTER_GREEN,	
	MONSTER_YELLOW
};
// 获取头顶标志
int getNpcQuestShow( int npcId );
int getMonsterRadius( int nResId );
int getMonsterSubtype( int nResId );
int getMonsterLevel( int nResId );
int getMonsterNameColor( int nResId );
const char* getMonsterName( int nResId );
int getMonsterIcon( int nResId );
int getMonsterMaxHP( int nResId );
const char* getNpcDesc( int npcId );
const char* GetNpcName( int nResid );
int getMonMachineMaxEnergy(int resid);
//tolua_end

class GameNPC : public GameActor//tolua_export
{//tolua_export

public:
	GameNPC();
	// 怪物出生
	void onBirth( int nBirthTime ); 
	// 怪物归位
	virtual void CmdMoveBack( int to_x, int to_z );
	virtual bool UpdateMoveBack();
	// 设置子类型
	virtual void setSubType();
	// 设置攻击类型(主动攻击，被动攻击)
	virtual void setAtkType();
	virtual void updateUI( int nChangeValue );
	virtual void Update(unsigned int dtick);
public:
	// 还原朝向
	void reSetAngle();
	// 获取NPC功能描述
	const char* getNpcFunDes()
	{
		return m_strNpcFunctionDes.c_str();
	}

	bool isPatrol();

public:
	int					m_nPlayFreeActTime;		// 播放休闲动作时间
	int					m_nNpcAngle;
	std::string			m_strNpcFunctionDes;	// NPC功能描述

	int					m_nQusetShow;
	bool				m_bIsPatrol;			// 是否是巡逻npc
private:
	int					m_nMoveBackTime;
	bool				m_lastXunluo;
};//tolua_export

