#ifndef _MONSTER_TITLE_DRAWER_H_
#define _MONSTER_TITLE_DRAWER_H_

#include "GameActor_TitleBaseDrawer.h"
class MonsterTitleDrawer : public IActorTitleDrawer
{
private:
	virtual bool IsShowName();
	virtual void BuildNameTtile();

	virtual bool IsShowClanName();
	virtual void BuildClanNameTtile();

	virtual bool IsShowTemaOrChengHaoName();
	virtual void BuildTemaOrChengHaoNameTtile();

	virtual bool IsShowOwnerName();
	virtual void BuildOwnerNameTitle();

	virtual bool IsShowStallName();
	virtual void BuildStallNameTitle();

	virtual bool IsShowFunctionDesc();
	virtual void BuildFunctionDescTitle();
};

#endif