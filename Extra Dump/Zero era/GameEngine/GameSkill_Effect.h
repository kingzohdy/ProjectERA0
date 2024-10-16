#pragma once
#include "GameEngineTypes.h"
#include "OgreWorldPos.h"

class GameSpellEffect
{
public:
	// 创建场景特效
	GameSpellEffect( int type, const char *respath, GameActor* pOwner );
	// 创建链状特效
	GameSpellEffect( const char* szMontionName, int nMontionClass );

	virtual ~GameSpellEffect();

	virtual void	start();
	virtual void	update(int dtick);
	bool			isDead();
	void			EnterMap( GameMap *pmap );

	void setStartPos( const Ogre::WorldPos& pos );
	void setEndPos( const Ogre::WorldPos& pos );
	void setLifeTick( int delaytick, int intick, int outtick, int livetick, int speed = 0 );
	void setLifeTick( int livetick);

	void bindingObj( int ObjId, int pos = 0 );
	void unBingdingObj();
	void setFaceTo( int from_x, int from_y, int to_x, int to_y );
	void setRotation( int nAngle );
public:
	enum
	{
		STATE_DEAD = 0,
		STATE_DELAY,
		STATE_IN,
		STATE_LIVE,
		STATE_OUT,
	};
public:
	int m_Type;					// 0:固定位置,  1:跟随角色,  2:追踪飞行,  3:自由落体飞行,  4:抛物线飞行
	int m_DelayTick;
	int m_InTick;
	int m_OutTick;
	int m_LiveTick;
	
	Ogre::WorldPos	 m_startPos;
	Ogre::WorldPos	 m_endPos;	

	Ogre::Entity*	 m_pEntity;
	GameMap*		 m_pCurMap;
	int				 m_State;
	int				 m_Life;
	float			 m_nSpeed;
	
	int				m_BindingObjId;
	bool			m_bBindingObj;
	unsigned int				m_uLinkTime;
protected:
	int				m_nPerLift;
};
