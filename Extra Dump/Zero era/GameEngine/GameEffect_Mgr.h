#pragma once

#include "GameEffect_Base.h"
#include <list>
#include <map>
#include <string>

struct LINK_INFO
{
	int target;
	int binda;
	int bindb;
	std::string path;
};

class EntityEffect;

class EffectManage//tolua_export
{//tolua_export
	friend IGameEffect;

private:
	std::list<GameEffectPtr>							lstEffect;
	std::vector<std::pair<unsigned int, GameEffectPtr>>	destoryEffects;
	std::vector<std::pair<unsigned int, GameEffectPtr>>	displayEffects;
	std::map<int, LINK_INFO>							linkInfos;

protected:
	EffectManage();
	EffectManage(const EffectManage &other);

public:
	static EffectManage & getSingleton();

public:
	void	update(unsigned int dtick);
	void	reset();
	void	display(IGameEffect *effect, unsigned int delay);
	void	display(IGameEffect *effect);
	void	destory(IGameEffect *effect, unsigned int delay);
	void	destory(IGameEffect *effect);
	void	cleanup(IGameEffect *effect);

	void	addLink(int startId, int startBindId, int endId, int endBindId, const std::string &path);
	void	delLink(int startId, int endId);
	void	clearLink(int startId);
	//tolua_begin
	EntityEffect *	playEffect(int x, int y, int angle, const char *path, unsigned int delay = 0);
	//tolua_end
};//tolua_export