#include <Windows.h>
#include "GodSchema.h"
#include "GameKeyMoveOperate.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "GameDBData.h"
#include "OgreWorldPos.h"
#include "OgreMath.h"
#include "OgreEntity.h"
#include "GamePlayManager.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
using namespace Ogre;

GodUnitList godUnitList;
GodUnitList godTesterList;
GodEffectList godEffectList;

static int curMinVirId = -1;

static GodMouseEventHandle godMouseEventHandle;

static bool isGodRunning = false;

void god_begin(int resid, int num)
{
	isGodRunning = true;
	int x, y;
	WorldPos pos = ActorManager::getSingleton().getMainPlayer()->GetPosition();
	x = pos.x / 10;
	y = pos.z / 10;
	for (int i = 0; i < num; ++i)
	{
		godUnitList.push_back(GodUnit::create(resid, int(x + Cos(i * 360.0f / num) * 100 * (num - 1)), int(y + Sin(i * 360.0f / num) * 100 * (num - 1)), ActorManager::getSingleton().getMainPlayer()->getAngle()));
	}
	GamePlayManager::getSingleton().getSelectManager()->insertMouseEventHandler(&godMouseEventHandle);
	GameKeyMoveOperate::getSingleton().SetGod();
}

void god_test(int num)
{
	isGodRunning = true;
	int x, y;
	WorldPos pos = ActorManager::getSingleton().getMainPlayer()->GetPosition();
	x = pos.x / 10;
	y = pos.z / 10;
	for (int i = 0; i < num; ++i)
	{
		GodUnit *unit = GodUnit::clone(int(x + Cos(i * 37.1f) * 15 * (i + 1)), int(y + Sin(i * 37.1f) * 15 * (i + 1)), ActorManager::getSingleton().getMainPlayer()->getAngle());
		if (unit)
		{
			godTesterList.push_back(unit);
		}
	}
}

void god_test_end()
{
	for (GodUnitList::iterator i = godTesterList.begin(); i != godTesterList.end(); ++i)
	{
		ActorManager::getSingleton().DestroyActor((*i));
	}
	curMinVirId = -1;
	godTesterList.clear();
}

void god_end()
{
	isGodRunning = false;
	for (GodUnitList::iterator i = godUnitList.begin(); i != godUnitList.end(); ++i)
	{
		delete *i;
	}
	for( GodEffectList::iterator i = godEffectList.begin(); i != godEffectList.end(); ++i)
	{
		delete *i;
	}
	godUnitList.clear();
	godEffectList.clear();
	GameKeyMoveOperate::getSingleton().SetGod(false);
	GameManager::getSingleton().getScriptVM()->callString("resumeAccelKeys()");
}

void god_update(unsigned int tick)
{
	GameMap *map = GamePlayManager::getSingleton().getCurMap();
	unsigned int now = timeGetTime();
	for (GodUnitList::iterator i = godTesterList.begin(); i != godTesterList.end(); ++i)
	{
		if ((*i)->getActState().GetState() != GSACT_MOVE)
		{
			if ((*i)->deadTime == 0)
			{
				(*i)->deadTime = now + rand() % 3000 + 2000;
			}
			else if ((*i)->deadTime <= now)
			{
				(*i)->deadTime = 0;
				GameMaskPath path;
				int cur_x, cur_z;
				(*i)->m_pMapMover->GetCurPos(cur_x, cur_z);
				int to_x = cur_x + rand() % 1001 - 500;
				int to_z = cur_z + rand() % 1001 - 500;
				if (map->FindPath(path, cur_x, cur_z, to_x, to_z, 1, true ))
				{
					(*i)->OnPathMove( &path );
				}
			}
		}
	}
	for (GodUnitList::iterator i = godUnitList.begin(); i != godUnitList.end(); )
	{
		(*i)->update(tick);
		if ((*i)->Death())
		{
			(*i)->OnLeaveMap();
			delete *i;
			i = godUnitList.erase(i);
		}
		else
		{
			if (!(*i)->getShowActor()->getCurEntity()->hasAnimPlaying(10420) && !(*i)->getShowActor()->getCurEntity()->hasAnimPlaying(10180) && !(*i)->getShowActor()->getCurEntity()->hasAnimPlaying(10111))
			{
				const int id[] = {1098, 2098, 3098, 4098, 1099, 2099, 3099, 4099, 5098};
				const char *name[] = {"1098_4", "2098_4", "3098_4", "4098_4", "1099_4", "2099_4", "3099_4", "4099_4", "5098_4"};
				const char*	szEffName[1];
				szEffName[0] = "";
				const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo((*i)->getResId());
				int race = info.race;
				if (race)
				{
					for (int i = 0; i < sizeof(id) / sizeof(int); ++i)
					{
						if (id[i] == race)
						{
							szEffName[0] = name[i];
							break;
						}
					}
				}
				if (szEffName[0][0] == '\0')
				{
					(*i)->playAction( 10111, szEffName, 1, 10180 );
				}
				else
				{
					if (strcmp(szEffName[0], "5098_4") == 0)
					{
						(*i)->playAction( 10420, szEffName, 1, 10180 );
					}
					else
					{
						(*i)->playAction( 10180, szEffName, 1, 10180 );
					}
				}
			}
			++i;
		}
	}
	for( GodEffectList::iterator i = godEffectList.begin(); i != godEffectList.end(); )
	{
		(*i)->update( tick );
		if((*i)->isDead())
		{
			delete (*i);
			i = godEffectList.erase(i);
		}
		else
		{
			for (GodUnitList::iterator j = godUnitList.begin(); j != godUnitList.end(); ++j)
			{
				if ((*j)->getActState().GetState() != GSACT_DEAD && (*i)->hitTest(*j))
				{
					for (GodUnitList::iterator k = godUnitList.begin(); k != godUnitList.end(); ++k)
					{
						(*i)->onHit(*k);
					}
					break;
				}
			}
			++i;
		}
	}
	if (isGodRunning && godUnitList.empty() && godEffectList.empty())
	{
		god_end();
	}
}

int GodUnit::getRadius()
{
	const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( m_ResID );
	return npcDisplayInfo.radius;
}

bool GodUnit::Death()
{
	return deadTime && timeGetTime() > deadTime + 5000;
}

void GodUnit::hit()
{
	if (--life > 0)
	{
		int actId = getActState().getActionId(140);
		if(!hasAnim( actId ))
		{
			actId = getActId(POSE_NONE, 140);
		}
		playAction(actId);
	}
	else
	{
		deadTime = timeGetTime();
		ChangeActState(GSACT_DEAD);
	}
}

void GodUnit::update(unsigned int tick)
{
	GameActor::Update(tick);
}

GodUnit * GodUnit::create(int resid, int x, int y, int angle)
{
	GodUnit *ret = new GodUnit;
	ret->setType( GAT_MONSTER );
	ret->LoadRes(resid);
	ret->OnEnterMap( ActorManager::getSingleton().getMainPlayer()->getCurMap() );
	ret->SetPosition( x, y );
	ret->SetFaceTo( angle );
	ret->ChangeActState( GSACT_STAND );

	return ret;
}

GodUnit * GodUnit::clone(int x, int y, int angle)
{
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	GodUnit *ret = new GodUnit;
	ret->SetID(curMinVirId--);
	if (!ActorManager::getSingleton().insertVirtualActor(ret))
	{
		delete ret;
		return NULL;
	}
	ret->m_Gender	= player->m_Gender;
	ret->m_Race		= player->m_Race;
	ret->m_Head		= player->m_Head;
	ret->m_Hair		= player->m_Hair;
	ret->m_Face		= player->m_Face;
	ret->setType( GAT_PLAYER );
	ret->setName(player->getName());
	ret->LoadRes(player->getResId());
	ret->OnEnterMap( ActorManager::getSingleton().getMainPlayer()->getCurMap() );
	ret->SetPosition( x, y );
	ret->SetFaceTo( angle );
	ret->ChangeActState( GSACT_STAND );
	ret->setSpeed(player->getSpeed());
	ret->setLv(player->getLv());
	ret->reSetHp(player->getHP());
	ret->setMaxHP(player->getMaxHP());
	ret->setMP(player->getMP());
	ret->setMaxMP(player->getMaxMP());

	Equip &equip = player->m_Equip;
	Equip &otherEquip = ret->m_Equip;
	ret->m_nShowFlag = player->m_nShowFlag;
	for (int i = EQUIP_POS_NO + 1; i < EQUIP_POS_END; ++i)
	{
		if (equip.m_ArmEquipId[i] != 0)
		{
			otherEquip.m_ArmEquipId[i] = equip.m_ArmEquipId[i];
			otherEquip.changEquip(i, equip.m_ArmEquipId[i]);
		}
	}
	ret->getAvatar()->updateAttrib();

	if (player->getRider()->isOnHorse())
	{
		ret->getRider()->onHorse(player->getRider()->getHorseId());
	}

	GamePet *pet = player->getPet();
	GamePet *petOther = ret->getPet();
	for (int i = 0; i < PET_MAX_FIGHT_SHI; ++i)
	{
		GameActor *pActor = pet->getPetActor(i);
		if (pActor == NULL)
		{
			continue;
		}
		GodUnit *pActorOther = new GodUnit;
		pActorOther->SetID(curMinVirId--);
		if (!ActorManager::getSingleton().insertVirtualActor(pActorOther))
		{
			delete pActorOther;
			continue;
		}
		pActorOther->setOwnerId(ret->GetID());
		petOther->setPetActor(i, pActorOther);
		pActorOther->setType( GAT_PET );
		pActorOther->setName(pActor->getName());
		pActorOther->LoadRes(pActor->getResId());
		pActorOther->OnEnterMap( ActorManager::getSingleton().getMainPlayer()->getCurMap() );
		pActorOther->SetPosition( x, y );
		pActorOther->SetFaceTo( angle );
		pActorOther->ChangeActState( GSACT_STAND );
		pActorOther->setSpeed(pActor->getSpeed());
		pActorOther->setLv(pActor->getLv());
		pActorOther->reSetHp(pActor->getHP());
		pActorOther->setMaxHP(pActor->getMaxHP());
		pActorOther->setMP(pActor->getMP());
		pActorOther->setMaxMP(pActor->getMaxMP());
	}

	return ret;
}

GodUnit::GodUnit() : GameActor()
{
	deadTime = 0;
	life = 3;
}

GodEffect::GodEffect(const char *respath, int speed, int life)
: GameSpellEffect(0, respath, ActorManager::getSingleton().getMainPlayer())
{
	setLifeTick( 0, 0, 0, life, speed );
}

bool GodEffect::hitTest(GodUnit *unit)
{
	return m_startPos.subtract(unit->GetPosition() + WorldPos(0, 2000, 0)).length() <= unit->getRadius();
}

void GodEffect::onHit(GodUnit *unit)
{
	if (m_startPos.subtract(unit->GetPosition() + WorldPos(0, 2000, 0)).length() < 300)
	{
		unit->hit();
	}
	m_State = STATE_DEAD;
}

void GodEffect::update(int dtick)
{
	GameSpellEffect::update( dtick );
	if (m_Life > m_LiveTick)
	{
		m_State = STATE_DEAD;
	}
	else
	{
		m_startPos.x += int(Cos(m_angle) * (m_nSpeed * dtick));
		m_startPos.z += int(Sin(m_angle) * (m_nSpeed * dtick));
		Ogre::Point2D end(m_startPos.x / 10, m_startPos.z / 10);
		Ogre::Point2D scale( ActorManager::getSingleton().getMainPlayer()->getCurMap()->getMaskMap()->m_lMaskPixelWidth, ActorManager::getSingleton().getMainPlayer()->getCurMap()->getMaskMap()->m_lMaskPixelHeight );
		Ogre::Point2D s_EndGrid = end / scale;
		if (!ActorManager::getSingleton().getMainPlayer()->getCurMap()->getMaskMap()->canWalk(s_EndGrid.x, s_EndGrid.y))
		{
			m_State = STATE_DEAD;
		}
		else
		{
			setStartPos( m_startPos );
		}
	}
}

GodEffect * GodEffect::create(const char *respath, int speed, int life, float angle)
{
	GodEffect* pFlyEffect = new GodEffect(respath, speed, life);
	WorldPos pos = ActorManager::getSingleton().getMainPlayer()->GetPosition();
	pos.y += 2000;
	pFlyEffect->setStartPos( pos );
	pFlyEffect->m_angle = -angle;
	pFlyEffect->m_pEntity->setRotation(angle - 90, 0, 0);
	pFlyEffect->start();
	pFlyEffect->EnterMap( ActorManager::getSingleton().getMainPlayer()->getCurMap() );

	godEffectList.push_back(pFlyEffect);
	return pFlyEffect;
}

bool GodMouseEventHandle::onLButtonDown(GameActor* pTarget, int x, int z)
{
	if (!GameKeyMoveOperate::getSingleton().IsGod())
	{
		return false;
	}
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	pActor->SetFaceTo(x, z);
	int actId = pActor->getActState().getActionId(400);
	if(!pActor->hasAnim( actId ))
	{
		actId = getActId(POSE_NONE, 400);
	}
	pActor->playAction(actId);
	GodEffect::create("effect\\entity\\201000_2.ent", 10000, 5000, float(pActor->getAngle()));
	return true;
}

bool GodMouseEventHandle::onRButtonDown(GameActor* pTarget, int x, int z)
{
	if (!GameKeyMoveOperate::getSingleton().IsGod())
	{
		return false;
	}
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	int actId = pActor->getActState().getActionId(400);
	if(!pActor->hasAnim( actId ))
	{
		actId = getActId(POSE_NONE, 400);
	}
	pActor->playAction(actId);
	GodSlash::create("effect\\entity\\201000_2.ent", 10000, 5000, float(pActor->getAngle()));
	return true;
}

bool GodSlash::hitTest(GodUnit *unit)
{
	return true;
}

void GodSlash::onHit(GodUnit *unit)
{
	m_State = STATE_DEAD;
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	WorldPos pos = unit->GetPosition();
	if (cos(atan2((double)pos.z - m_startPos.z, pos.x - m_startPos.x) + pActor->getAngle() * 3.1415926 / 180) > 0.5 && m_startPos.subtract(pos + WorldPos(0, 2000, 0)).length() <= unit->getRadius() + 200)
	{
		unit->hit();
	}
}

GodSlash * GodSlash::create(const char *respath, int speed, int life, float angle)
{
	GodSlash* pFlyEffect = new GodSlash(respath, speed, life);
	WorldPos pos = ActorManager::getSingleton().getMainPlayer()->GetPosition();
	pos.y += 2000;
	pFlyEffect->setStartPos( pos );
	pFlyEffect->m_angle = - angle;
	pFlyEffect->m_pEntity->setRotation(angle-90, 0, 0);
	pFlyEffect->start();
	pFlyEffect->EnterMap( ActorManager::getSingleton().getMainPlayer()->getCurMap() );

	godEffectList.push_back(pFlyEffect);
	return pFlyEffect;
}

GodSlash::GodSlash(const char *respath, int speed, int life) : GodEffect(respath, speed, life)
{

}
