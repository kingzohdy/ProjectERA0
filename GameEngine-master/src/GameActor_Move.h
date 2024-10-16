#pragma once
#include "OgreMemoryDelegation.h"
#include "GameActor.h"
#include "OgreWorldPos.h"
#include "GameEngineTypes.h"

enum
{
	MOVETYPE_NOMAL,
	MOVETYPE_BACK,	
	MOVETYPE_FLY,
	MOVETYPE_ANIMFLY,
};

class SpecialMove//tolua_export
	: public Ogre::MemoryDelegation
{//tolua_export
public:
	SpecialMove(void);
	~SpecialMove(void);

public:
	void			setType(int type);
	int				getType();
	
	void			setAutoFly( GameActor* pActor, int indexId, bool bFristFly = true );
	// ���ݶ���·�߷��� 
	void			setAnimFly( GameActor* pActor, int indexId );
	
	//tolua_begin
	void			addFlyPath( int x, int y, int z );
	void			playAnimFly( int nOwnerId, const char* szAnimOmod, int modelId, int x, int y, int npcId, 
						int delayTick = 0, bool bCanShowUI = true );
	void			continueAnimFly( int nOwnerId, const char* szAnimOmod,int modelId, int x, int y,int npcId );
	void			InsertAnimPath(int animId);
	bool			checkCancelFirstAnimFly();
	bool			canShowUI();
	//tolua_end

	void			doBack( GameActor* pActor, int mapid, int x, int z, int BackTime, int type, int nHeight, int speed );
	void			setCircleInfo(GameActor* pActor, int x, int y, bool inverted = true);
	virtual void	update( GameActor* pActor, unsigned int dtick );

	// Ѱ�ҷ��е�߶�����
	int				getPointHeight( GameActor* pActor, GameXYZ& pos );		
private:
	// Ѱ���м����
	int				findPathIndex( int x, int y );
	// ���Ƿ���·����
	bool			IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 );
private:
	Ogre::WorldPos				m_InitPos;
	Ogre::WorldPos				m_BackPos;
	int							m_mapid;
	int							m_BackAngle;
	int							m_nMaxHeight;
	int							m_nBackSpeed;
	int							m_BackTime;
	int							m_BackType;
	unsigned int				m_uPreBackTime;
	unsigned int				m_uBackStartTime;

	int							m_nMoveType;
	std::vector<GameXYZ>		m_vectFlyPos;
	int							m_nFlyPointIndex;
	int							m_curSeg;

	double						m_paramA;
	double						m_paramB;

	Ogre::Model*				m_pFlyAnimMod;		// ���ж���
	unsigned int				m_nStartTick;		// ��ʼʱ��
	unsigned int				m_nAnimPassTime;	// ����ʱ��
	GameXYZ						m_EndPos;			// �����

	int							m_nNpcId;
	bool						m_bInitCircleInfo;
	int							m_nDelayBindTick;	//�Ӻ�bindʱ��
	unsigned int				m_nStartJumpTime;	//����ʱ��
	unsigned int				m_uLastNotifyTime;	//����֪ͨ������ʱ��
	struct
	{
		int				x;						/// Բ������
		int				y;
		int				time;					/// �ܻ���ʱ��
		float			startAngle;				/// ��ʼ�Ƕ�
		float			angle;					/// ��ת���Ƕ�
		float			startRadius;			/// ��ʼ�뾶
		float			endRadius;				/// ��ֹ�뾶
	}							m_CircleInfo;	/// �����˶����˶�����
	std::vector<int>			m_vectorAnimPath;	//�������ж���
	unsigned int							m_nCurAnimIndex;
	bool					m_bHideUI;
};//tolua_export
