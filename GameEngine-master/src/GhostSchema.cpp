#include "GhostSchema.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "NetMessage.h"
#include "GameMainPlayer.h"
#include "OgreRect.h"
#include "GameMap.h"
#include "GameMaskMap.h"
#include <Windows.h>

bool GhostSchema::isGhost = false;
int GhostSchema::height = 0;
float GhostSchema::times = 1.0f;

void GhostSchema::enter(int i)
{
	isGhost = true;
	tagCSPkgBody msgbody;
	CSCHATREQ& chatReqData	= msgbody.ChatReq;
	chatReqData.Type		= CHAT_SAY;
	chatReqData.ToName[0]	= 0;
	chatReqData.LinkLen		= 0;
	chatReqData.Res			= 1;
	strncpy(chatReqData.Content, "//spook 1", sizeof(chatReqData.Content));
	memset(chatReqData.LinkContent, 0, sizeof(chatReqData.LinkContent));
	SendGameServerMsg(CHAT_REQ, msgbody);
	height = ActorManager::getSingleton().getMainPlayer()->GetPosition().y;
	if (i != 0)
	{
		ActorManager::getSingleton().getMainPlayer()->setSpeed(ActorManager::getSingleton().getMainPlayer()->getSpeed() / times);
		times = i / 100.0f;
		ActorManager::getSingleton().getMainPlayer()->setSpeed(ActorManager::getSingleton().getMainPlayer()->getSpeed() * times);
	}
}

void GhostSchema::exit()
{
	isGhost = false;
	tagCSPkgBody msgbody;
	CSCHATREQ& chatReqData = msgbody.ChatReq;
	chatReqData.Type		= CHAT_SAY;
	chatReqData.ToName[0]	= 0;
	chatReqData.LinkLen		= 0;
	chatReqData.Res			= 1;
	strncpy(chatReqData.Content, "//spook 0", sizeof(chatReqData.Content));
	memset(chatReqData.LinkContent, 0, sizeof(chatReqData.LinkContent));
	SendGameServerMsg(CHAT_REQ, msgbody);
	ActorManager::getSingleton().getMainPlayer()->setSpeed(ActorManager::getSingleton().getMainPlayer()->getSpeed() / times);
}

void GhostSchema::update()
{
	if (!isGhost)
	{
		return;
	}
	static unsigned int lastTime = 0;
	static int lastX = -1;
	static int lastY = -1;

	unsigned int now = timeGetTime();
	if (lastTime + 1500 > now)
	{
		return;
	}
	lastTime = now;

	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	GameXYZ pos = pActor->GetPosition();
	Ogre::Point2D end(pos.x / 10, pos.z / 10);
	if (pActor->getCurMap()->getMaskMap()->findPointWalkable(end, MASK_STOP | DYN_MASK_STOP))
	{
		if (end.x == lastX && end.y == lastY)
		{
			return;
		}
		lastX = end.x;
		lastY = end.y;
		tagCSPkgBody msgbody;
		msgbody.AutoFlyClt.AutoFlyEnd = 0;
		msgbody.AutoFlyClt.EndPos.X = end.x;
		msgbody.AutoFlyClt.EndPos.Y = end.y;
		SendGameServerMsg(AUTO_FLY_CLT, msgbody);
	}
}
