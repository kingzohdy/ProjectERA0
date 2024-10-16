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
	// 根据动画路线飞行 
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

	// 寻找飞行点高度坐标
	int				getPointHeight( GameActor* pActor, GameXYZ& pos );		
private:
	// 寻找中间起点
	int				findPathIndex( int x, int y );
	// 点是否在路径上
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

	Ogre::Model*				m_pFlyAnimMod;		// 飞行动画
	unsigned int				m_nStartTick;		// 开始时间
	unsigned int				m_nAnimPassTime;	// 动画时间
	GameXYZ						m_EndPos;			// 降落点

	int							m_nNpcId;
	bool						m_bInitCircleInfo;
	int							m_nDelayBindTick;	//延后bind时间
	unsigned int				m_nStartJumpTime;	//起跳时间
	unsigned int				m_uLastNotifyTime;	//最后次通知服务器时间
	struct
	{
		int				x;						/// 圆心坐标
		int				y;
		int				time;					/// 总花费时间
		float			startAngle;				/// 起始角度
		float			angle;					/// 总转动角度
		float			startRadius;			/// 起始半径
		float			endRadius;				/// 终止半径
	}							m_CircleInfo;	/// 离心运动的运动参数
	std::vector<int>			m_vectorAnimPath;	//多条飞行动画
	unsigned int							m_nCurAnimIndex;
	bool					m_bHideUI;
};//tolua_export
