
#pragma once
#include "OgreMemoryDelegation.h"
#include "GameEngineTypes.h"
// #include "OgreEntity.h"

//1 玩家 背一个npc；
//2 玩家 背一个玩家

enum
{
	eNo_Carry,		//没有背
	eCarrying,		//背的状态中
	eCarryed,		//被别人背
	eEnd_Carry,		//放下中

	eCarry_Max
};


class GameActor_Carry : public Ogre::MemoryDelegation
{
public:

	GameActor_Carry(GameActor* pOwner);
	~GameActor_Carry();

	//背一个模型
	bool carry( int nStandActionID,int nMoveActionID,int nBindID,int nResID,int nTargetAction );
	//背另外一个玩家
	bool carry( int nCarryID,int nStandActionID,int nMoveActionID,int nBindID,int nTargetID,int nTargetAction );
	bool unCarry( unsigned int costTime );

	void update(unsigned int dtick);

	int getCarryState();

	int	getCarryStandAction();
	int getCarryMoveAction();

	void onOwnerDead();

	void onLeaveMap();
	
protected:
private:

	GameActor*		m_pOwner;					//拥有着
	int				m_nBindID;					//背的绑定点
	int				m_nCarryID;					//背的人的id
	int				m_nCarryStandActionID;	
	int				m_nCarryMoveActionID;
	int				m_nCarriedID;				//被背的人的id，小于0 表示 背一个模型
	Ogre::Entity*	m_pCarriedEntity;			//m_nCarriedID 小于 0的时候用

	int			m_nCarriedActionID;			//被背的人只需要一个动作就可以了。

	unsigned int m_uStartTime;				
	unsigned int m_uCostTime;				

	int			m_nCarryState;		//当前背的状态
};
