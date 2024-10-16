#include "ItemPicker.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "GameCollect.h"
#include "GameMsgPrint.h"
#include "GameEffect_Mgr.h"
#include "EntityEffect.h"
#include "Item.h"
#include "GameAutoAssist.h"
#include <Windows.h>
#include <iostream>

using namespace std;


const int MOVE_PICK_ITEM_DISTANCE = 150;
ItemPicker::Hold::Hold(int id) : itemid(id)
{

}

void ItemPicker::Hold::operator() ()
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	pMainPlayer->getContainer().send_PickItem(itemid);
}

ItemPicker::Collect::Collect(int id) : itemid(id)
{

}

void ItemPicker::Collect::operator() ()
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	GameCollect::getSingleton().purgeStartCollect(itemid);
}

ItemPicker::ItemPicker(void)
{
}

ItemPicker::~ItemPicker(void)
{
}

ItemPicker & ItemPicker::getSingleton()
{
	static ItemPicker singleton;
	return singleton;
}

void ItemPicker::pickItem(int itemid)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	GameActor *pTarget = ActorManager::getSingleton().FindActor(itemid);
	if (pTarget == NULL)
	{
		return;
	}
	GameXYZ pos = pMainPlayer->GetPosition();
	GameXYZ targetPos = pTarget->GetPosition();
	targetPos.y = pos.y;
	float length = pos.subtract(targetPos).length();
	if (pTarget->m_CollectTime > 0)
	{
		if (length <= MAX_COLLECT_ITEM_DIST)
		{
			GameCollect::getSingleton().purgeStartCollect( pTarget->GetID() );
		}
		else
		{
			if (MOVE_SUCCESS == pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(pTarget->GetPosition().x),
				(int)Ogre::WorldPos::Fix2Flt(pTarget->GetPosition().z), MAX_COLLECT_ITEM_DIST))
			{
				pMainPlayer->setMoveTask(Collect(itemid));
			}
		}
	}
	else
	{
		if (length <= MAX_PICK_DIST)
		{
			pMainPlayer->getContainer().send_PickItem(itemid);
		}
		else
		{
			if (MOVE_SUCCESS == pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(pTarget->GetPosition().x),
				(int)Ogre::WorldPos::Fix2Flt(pTarget->GetPosition().z), MOVE_PICK_ITEM_DISTANCE))
			{
				pMainPlayer->setMoveTask(Hold(itemid));
			}
		}
	}
}

bool ItemPicker::tryPickItem(int itemid)
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return false;
	}
	GameActor *pTarget = ActorManager::getSingleton().FindActor(itemid);
	if (pTarget == NULL)
	{
		return false;
	}
	GameXYZ pos = pMainPlayer->GetPosition();
	GameXYZ targetPos = pTarget->GetPosition();
	targetPos.y = pos.y;
	float length = pos.subtract(targetPos).length();
	if (pTarget->m_CollectTime > 0)
	{
		if (length > MAX_COLLECT_ITEM_DIST)
		{
			GameMsgPrint::getSingleton().showMidTip(EMID_MESSAGE(MID_PICK_ITEM_TOO_FAR));
			return false;
		}
		GameCollect::getSingleton().purgeStartCollect(itemid);
	}
	else
	{
		if (length > MAX_PICK_DIST)
		{
			GameMsgPrint::getSingleton().showMidTip(EMID_MESSAGE(MID_PICK_ITEM_TOO_FAR));
			return false;
		}
		pMainPlayer->getContainer().send_PickItem(itemid);
	}
	return true;
}

void ItemPicker::pickAllItems()
{
	ActorManager &actorMgr = ActorManager::getSingleton();
	MainGamePlayer *pMainPlayer = actorMgr.getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}

	static unsigned int lastPickTime = 0;
	static const unsigned int PICK_INTERVAL = 500;

	unsigned int now = timeGetTime();
	if (lastPickTime + PICK_INTERVAL > now)
	{
		return;
	}
	lastPickTime = now;

	static const int MAX_ITEM_COUNT = 5;
	std::list<int>	items;
	GameXYZ pos = pMainPlayer->GetPosition();
	for (ACTOR_ITER iter = actorMgr.getActorBegin(), iterEnd = actorMgr.getActorEnd(); iter != iterEnd && items.size() < MAX_ITEM_COUNT; ++iter)
	{
		GameActor *item = iter->second;
		if (item->getType() == GAT_ITEM)
		{
			const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef(item->getResId());
			if ((itemDef.ItemTmp.Type == ITEM_TMP_NO || item->m_IsPickItem) && item->canPickUp() && item->m_CollectTime == 0)
			{
				GameXYZ targetPos = item->GetPosition();
				targetPos.y = pos.y;
				if (pos.subtract(item->GetPosition()).length() <= MAX_PICK_DIST)
				{ 
					items.push_back(item->GetID());
				}
			}
		}
	}
	for (std::list<int>::iterator i = items.begin(); i != items.end(); ++i)
	{
		pMainPlayer->getContainer().send_PickItem(*i);
	}
	EntityEffect *effect = EffectManage::getSingleton().playEffect(pos.x / 10, pos.z / 10, 0, "effect\\entity\\9630_14.ent", 1500);
	if (effect)
	{
		effect->setScale(MAX_PICK_DIST / 430.0);
	}
}
bool ItemPicker::PickItemsForAutoAssist( int m_nId)
{
	
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameXYZ pos = pMainPlayer->GetPosition();
	if (pMainPlayer == NULL)
	{
		return true;
	}
	if ( !pMainPlayer->canMove() )
	{
		return false;
	}
	if ( m_nId	==	-1 )
	{
		GameXYZ targetPos	=	AutoAssist::getSingleton().m_nPos;
		pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(targetPos.x),
			(int)Ogre::WorldPos::Fix2Flt(targetPos.z), 0 );
		return true;
	}
	else
	{
		GameActor *pTarget = ActorManager::getSingleton().FindActor(m_nId);

		if (pTarget == NULL || !pTarget->canPickUp())
		{
			pMainPlayer->getActState().SetState( GSACT_STAND );
			return true;
		}
	
		GameXYZ targetPos = pTarget->GetPosition();
		targetPos.y = pos.y;
		float length = pos.subtract(targetPos).length();
		if (pTarget->m_CollectTime == 0)
		{
			if (length <= MAX_PICK_DIST)
			{
				pMainPlayer->getContainer().send_PickItem(m_nId);
				return true;
			}
			else 
			{	
				if(targetPos.x != 0 || targetPos.z != 0)
				{
					pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(pTarget->GetPosition().x),
						(int)Ogre::WorldPos::Fix2Flt(pTarget->GetPosition().z), MOVE_PICK_ITEM_DISTANCE );
				}
				return false;
			}
		}		
		return true;
	}
	
}