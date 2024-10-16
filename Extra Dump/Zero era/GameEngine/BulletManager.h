#pragma once
#ifndef __BULLETMANAGER__
#define __BULLETMANAGER__

#include "OgreSingleton.h"
#include "OgreWorldPos.h"
#include "GameEngineTypes.h"
#include "GameMaskMap.h"
#include "ShowActor.h"
#include <map>

class GameMap;

class Bullet : public Ogre::MemoryDelegation
{
private:
	Ogre::ShowActor	*	m_ShowActor;
	int					m_ID;
	int					m_ResID;
	int					m_OwnerID;
	GameXYZ				m_Pos;
	float				m_Speed;
	GameMap *			m_Map;
	GameMaskPath		m_Path;
	int					m_CurSeg;
	float				m_SegOffset;
	int					m_Angle;

public:
	Bullet(int id, int resId, int owner);
	~Bullet();

	void OnPathMove(float speed, const GameMaskPath &path);
	void Update(unsigned int dtick);
	const GameXYZ & GetPosition();
	void SetPosition(const GameXYZ &pos);
	void SetPosition(int x, int y);
	void SetFaceTo(int x, int y);
	void SetFaceTo(int angle);
	void GetAttackSkill(int &id, int &level);
	int GetAttackSkillId();
	int GetAttackSkillLevel();
	int GetOwnerId();
	void Destroy();
	int GetAngle();
	int GetSubType();
	unsigned int GetRadius();
	int GetID();
};

class BulletManager : public Ogre::Singleton<BulletManager>
{
private:
	std::map<int, Bullet *>		Bullets;
	std::list<int>				DestroyBullets;
	std::map<int, std::set<int>>BulletsTarget;	

public:
	BulletManager();
	~BulletManager();

	Bullet *	CreateBullet(int id, int resId, int owner, int x, int y);
	void		DestoryBullet(int id);
	void		DestoryBullet(Bullet *bullet);
	void		ClearBullet(bool detach);
	Bullet *	FindBullet(int id);

	void		Update(unsigned int dtick);
	void		BulletAttack(Bullet *bullet, GameActor *target);
};

#endif
