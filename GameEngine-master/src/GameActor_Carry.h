
#pragma once
#include "OgreMemoryDelegation.h"
#include "GameEngineTypes.h"
// #include "OgreEntity.h"

//1 ��� ��һ��npc��
//2 ��� ��һ�����

enum
{
	eNo_Carry,		//û�б�
	eCarrying,		//����״̬��
	eCarryed,		//�����˱�
	eEnd_Carry,		//������

	eCarry_Max
};


class GameActor_Carry : public Ogre::MemoryDelegation
{
public:

	GameActor_Carry(GameActor* pOwner);
	~GameActor_Carry();

	//��һ��ģ��
	bool carry( int nStandActionID,int nMoveActionID,int nBindID,int nResID,int nTargetAction );
	//������һ�����
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

	GameActor*		m_pOwner;					//ӵ����
	int				m_nBindID;					//���İ󶨵�
	int				m_nCarryID;					//�����˵�id
	int				m_nCarryStandActionID;	
	int				m_nCarryMoveActionID;
	int				m_nCarriedID;				//�������˵�id��С��0 ��ʾ ��һ��ģ��
	Ogre::Entity*	m_pCarriedEntity;			//m_nCarriedID С�� 0��ʱ����

	int			m_nCarriedActionID;			//��������ֻ��Ҫһ�������Ϳ����ˡ�

	unsigned int m_uStartTime;				
	unsigned int m_uCostTime;				

	int			m_nCarryState;		//��ǰ����״̬
};
