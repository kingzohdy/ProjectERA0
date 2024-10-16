#pragma once
#include "GameActor.h"
#include "OgreWorldPos.h"
#include "GameEngineTypes.h"

enum
{
	MOVETYPE_NOMAL,
	MOVETYPE_BACK,	
	MOVETYPE_FLY,
};

class SpecialMove//tolua_export
{//tolua_export
public:
	SpecialMove(void);
	~SpecialMove(void);

public:
	void			setType(int type);
	int				getType();
	
	void			setAutoFly( GameActor* pActor, int indexId, bool bFristFly = true );
	//tolua_begin
	void			addFlyPath( int x, int y, int z );
	//tolua_end

	void			doBack( GameActor* pActor, int x, int z, int BackTime, int type, int nHeight, int speed );
	virtual void	update( GameActor* pActor, unsigned int dtick );

	// 寻找飞行点高度坐标
	int				getPointHeight( GameActor* pActor, GameXYZ& pos );		
private:
	// 寻找中间起点
	int				findPathIndex( int x, int y );
	// 点是否在路径上
	bool			IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 );
private:
	Ogre::WorldPos				m_BackPos;
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
};//tolua_export
