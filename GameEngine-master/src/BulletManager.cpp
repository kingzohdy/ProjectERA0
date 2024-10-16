#include "BulletManager.h"
#include "GameMap.h"
#include "OgreRect.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "GameSkill_Mgr.h"

using namespace Ogre;

Bullet::Bullet(int id, int resId, int owner) :
	m_Map(NULL), m_ID(id), m_ResID(resId),
	m_OwnerID(owner), m_CurSeg(-1), m_SegOffset(0), m_Angle(0)
{
	m_ShowActor = new ShowActor(resId, false);
}

Bullet::~Bullet()
{
	OGRE_RELEASE(m_ShowActor);
	if( SkillManager::getSingletonPtr() != NULL )
	{
		SkillManager::getSingleton().deleteActorSpell(GetID());
	}
}

void Bullet::OnPathMove(float speed, const GameMaskPath &path)
{
	m_Speed = speed;
	m_Path = path;
	m_CurSeg = 0;
	m_SegOffset = 0;
	if (m_Path.getNumPoint() > 0)
	{
		Point2D pt = m_Path.getPathPoint(0);
		SetPosition(pt.x, pt.y);
	}

	if (m_Path.getNumPoint() > 1)
	{
		Point2D pt = m_Path.getPathPoint(1);
		SetFaceTo(pt.x, pt.y);
	}
}

void Bullet::Update(unsigned int dtick)
{
	m_ShowActor->update(dtick);
	if (m_CurSeg < 0)
		return;

	m_SegOffset += m_Speed * Ogre::TickToTime(dtick);

	float seglen = 0;
	while(1)
	{
		seglen = ((float)m_Path.getSegmentLen(m_CurSeg));
		if (m_SegOffset < seglen)
			break;

		m_SegOffset -= seglen;
		++m_CurSeg;
		if((size_t)m_CurSeg >= m_Path.getNumSegment())
		{
			break;
		}
	}

	if ((size_t)m_CurSeg >= m_Path.getNumSegment())
	{
		m_CurSeg = -1;
		Point2D pt = m_Path.getPathPoint(m_Path.getNumSegment());
		SetPosition(pt.x, pt.y);
		MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
		if (player && player->GetID() == m_OwnerID)
		{
			BulletManager::getSingleton().BulletAttack(this, NULL);
		}
	}
	else
	{
		Point2D pt1, pt2;
		pt1 = m_Path.getPathPoint(m_CurSeg);
		pt2 = m_Path.getPathPoint(m_CurSeg + 1);

		float t = m_SegOffset / seglen;
		SetPosition(int(pt1.x + t * (pt2.x - pt1.x)), int(pt1.y + t * (pt2.y - pt1.y)));
		SetFaceTo(FaceAngleBetween(pt1.x, pt1.y, pt2.x, pt2.y));
	}
}

const GameXYZ & Bullet::GetPosition()
{
	return m_Pos;
}

void Bullet::SetPosition(const GameXYZ &pos)
{
	if (m_Map == NULL)
	{
		MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
		if (player == NULL)
		{
			return;
		}
		m_Map = player->getCurMap();
		if (m_Map == NULL)
		{
			return;
		}
		m_ShowActor->onEnterMap(m_Map->getShowGameMap());
	}
	m_Pos = pos;
	m_ShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);
}

void Bullet::SetPosition(int x, int y)
{
	if (m_Map == NULL)
	{
		MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
		if (player == NULL)
		{
			return;
		}
		m_Map = player->getCurMap();
		if (m_Map == NULL)
		{
			return;
		}
		m_ShowActor->onEnterMap(m_Map->getShowGameMap());
	}
	if (m_Map != NULL)
	{
		int h;
		x *= 10;
		y *= 10;
		m_Map->getTerrainH(x, y, h);
		h += m_ShowActor->getActorOffsetZ();
		SetPosition(GameXYZ(x, h, y));
	}
}

void Bullet::SetFaceTo(int x, int y)
{
	if ((x != m_Pos.x) || (y != m_Pos.z))
	{
		SetFaceTo(FaceAngleBetween(m_Pos.x, m_Pos.z, x, y));
	}
}

void Bullet::SetFaceTo(int angle)
{
	m_Angle = angle;
	m_ShowActor->setFaceTo(angle);
}

void Bullet::GetAttackSkill(int &id, int &level)
{
	const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(m_ResID);
	id = MonsterDef.AINormalSkills[0].SkillID;
	level = MonsterDef.AINormalSkills[0].SkillLevel;
}

int Bullet::GetAttackSkillId()
{
	const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(m_ResID);
	return MonsterDef.AINormalSkills[0].SkillID;
}

int Bullet::GetAttackSkillLevel()
{
	const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(m_ResID);
	return MonsterDef.AINormalSkills[0].SkillLevel;
}

int Bullet::GetOwnerId()
{
	return m_OwnerID;
}

void Bullet::Destroy()
{
	if (m_Map != NULL)
	{
		m_ShowActor->onLeaveMap(m_Map->getShowGameMap());
	}
}

int Bullet::GetAngle()
{
	return m_Angle;
}

int Bullet::GetSubType()
{
	const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(m_ResID);
	return MonsterDef.SubType;
}

unsigned int Bullet::GetRadius()
{
	const tagMonsterDef &MonsterDef = DBDataManager::getSingleton().getMonsterDef(m_ResID);
	return MonsterDef.Radius;
}

int Bullet::GetID()
{
	return m_ID;
}

BulletManager::BulletManager()
{

}

BulletManager::~BulletManager()
{
	for (std::map<int, Bullet *>::iterator i = Bullets.begin(); i != Bullets.end(); ++i)
	{
		delete i->second;
	}
	Bullets.clear();
}

Bullet * BulletManager::CreateBullet(int id, int resId, int owner, int x, int y)
{
	std::map<int, Bullet *>::iterator i = Bullets.find(id);
	if (i == Bullets.end())
	{
		Bullets[id] = new Bullet(id, resId, owner);
		i = Bullets.find(id);
	}
	i->second->SetPosition(x, y);
	return i->second;
}

void BulletManager::DestoryBullet(int id)
{
	DestoryBullet(FindBullet(id));
}

void BulletManager::DestoryBullet(Bullet *bullet)
{
	if (bullet)
	{
		bullet->Destroy();
		DestroyBullets.push_back(bullet->GetID());
	}
}

void BulletManager::ClearBullet(bool detach)
{
	for (std::map<int, Bullet *>::iterator i = Bullets.begin(); i != Bullets.end();)
	{
		if (detach)
		{
			i->second->Destroy();
		}
		delete i->second;
		i = Bullets.erase(i);
	}
	BulletsTarget.clear();
}

Bullet * BulletManager::FindBullet(int id)
{
	std::map<int, Bullet *>::iterator i = Bullets.find(id);
	if (i != Bullets.end())
	{
		return i->second;
	}
	return NULL;
}

void BulletManager::Update(unsigned int dtick)
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player == NULL)
	{
		return;
	}
	int id = player->GetID();
	GameMap *map = player->getCurMap();
	if (map == NULL)
	{
		return;
	}
	for (std::list<int>::iterator i = DestroyBullets.begin(); i != DestroyBullets.end(); ++i)
	{
		std::map<int, Bullet *>::iterator j = Bullets.find(*i);
		if (j != Bullets.end())
		{
			delete j->second;
			Bullets.erase(j);
		}
		std::map<int, std::set<int>>::iterator k = BulletsTarget.find(*i);
		if (k != BulletsTarget.end())
		{
			BulletsTarget.erase(k);
		}
	}
	for (std::map<int, Bullet *>::iterator i = Bullets.begin(); i != Bullets.end(); ++i)
	{
		Bullet *bullet = i->second;
		bullet->Update(dtick);
		if (bullet->GetOwnerId() == id)
		{
			unsigned int range = bullet->GetRadius();
			const GameXYZ &selfPos = bullet->GetPosition();
			if (!map->getMaskMap()->canWalkPixelCoord(selfPos.x / 10, selfPos.z / 10))
			{
				BulletAttack(bullet, NULL);
			}
			else
			{
				for( ACTOR_ITER iter  = ActorManager::getSingleton().getActorBegin(); 
					iter != ActorManager::getSingleton().getActorEnd(); ++iter)
				{
					GameActor *pActor = iter->second;
					if (pActor->isDead())
					{
						continue;
					}
					if (pActor->isOnMonsterMachine())
					{
						continue;
					}
					if (pActor->isCamp(SKILL_TARGET_ENEMY_MON) || pActor->isCamp(SKILL_TARGET_ENEMY_PLAYER) || pActor->isCamp(SKILL_TARGET_ENEMYPET))
					{
						if (pActor->getType() == GAT_MONSTER && (pActor->getSubType() >= 100 || pActor->getSubType() == MON_SUB_TYPE_HUMAN_FRIEND || pActor->getSubType() == MON_SUB_TYPE_HUMAN_BROTHER))
						{
							continue;
						}
						unsigned int radius = 30;
						if (pActor->getType() == GAT_MONSTER)
						{
							radius = DBDataManager::getSingleton().getMonsterDef(pActor->getResId()).Radius;
						}
						GameXYZ pos = pActor->GetPosition();
						pos.y = selfPos.y;
						if (pos.subtract(selfPos).length() <= range + radius)
						{
							BulletAttack(bullet, pActor);
							if (bullet->GetSubType() == MON_SUB_TYPE_SKILL_DESTORY)
							{
								break;
							}
						}
					}
				}
			}
		}
	}
}

void BulletManager::BulletAttack(Bullet *bullet, GameActor *target)
{
	std::set<int> &targets = BulletsTarget[bullet->GetID()];
	if (targets.size() >= MAX_SKILL_TARGET)
	{
		return;
	}
	int id = target ? target->GetID() : 0;
	if (targets.find(id) != targets.end())
	{
		return;
	}
	targets.insert(id);
	GameSpellPtr pSpell = SkillManager::getSingleton().createSpell(bullet->GetAttackSkillId(), SkillManager::getSingleton().getSpellControler());
	pSpell->setSpellLv(bullet->GetAttackSkillLevel());
	if (target)
	{
		pSpell->setSingleTarget(target->GetID());
		const GameXYZ &pos = target->GetPosition();
		pSpell->setTargetPoint(pos.x, pos.z);
	}
	else
	{
		const GameXYZ &pos = bullet->GetPosition();
		pSpell->setTargetPoint(pos.x, pos.z);
	}
	pSpell->calSkillTargets();
	pSpell->onAttack(true);
	++pSpell->m_nPilotNum;
	SkillManager::getSingleton().sendAttackMsg(pSpell, 0, bullet->GetID());
	if (bullet->GetSubType() == MON_SUB_TYPE_SKILL_DESTORY)
	{
		DestoryBullet(bullet);
	}
}
