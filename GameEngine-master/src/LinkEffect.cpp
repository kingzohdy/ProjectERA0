#include "LinkEffect.h"
#include "ShowActor.h"

LinkEffect::LinkEffect(const char *path, int actorId, int bindId, const GameXYZ &endPos, int motionClass)
	: effect_path(path), actor_id(actorId), bind_id(bindId), motion_class(motionClass), target_pos(endPos), living(true)
{
}

LinkEffect::~LinkEffect()
{
	destory();
}

void LinkEffect::destory()
{
	if (!living)
	{
		return;
	}
	living = false;
	GameActor *pActor = ActorManager::getSingleton().FindActor(actor_id);
	if (pActor != NULL)
	{
		pActor->getShowActor()->getCurEntity()->stopMotion(motion_class);
	}
}

bool LinkEffect::update(unsigned int dtick)
{
	if (!living)
	{
		return false;
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(actor_id);
	if (pActor == NULL)
	{
		return false;
	}
	pActor->getShowActor()->getCurEntity()->playFlashChain(effect_path.c_str(), target_pos.toVector3(), motion_class, bind_id);
	return true;
}

void LinkEffect::setTargetPos(const GameXYZ &endPos)
{
	target_pos = endPos;
}

LinkFollowEffect::LinkFollowEffect(const char *path, int actorId, int bindId, int targetId, int targetBindId, int motionClass) :
	LinkEffect(path, actorId, bindId, GameXYZ(0, 0, 0), motionClass), target_id(targetId), target_bindId(targetBindId)
{
}

bool LinkFollowEffect::update(unsigned int dtick)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(target_id);
	if (pActor == NULL)
	{
		return false;
	}
	setTargetPos(pActor->getShowActor()->getCurEntity()->getAnchorWorldPos(target_bindId));
	return LinkEffect::update(dtick);
}
