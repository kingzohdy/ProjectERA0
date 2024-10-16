#pragma once
#include "OgreMemoryDelegation.h"
#include "GameEngineTypes.h"
#include "SmartPtr.h"

class IGameMover : public AutoRelease<IGameMover>, public Ogre::MemoryDelegation
{
private:
	bool		isActive;
	GameXYZ		curpos;

protected:
	void	reach();
	void	setx(int i);
	void	sety(int i);
	void	setz(int i);
	void	setpos(int x, int y, int z);
	void	setpos(const GameXYZ &pos);

public:
	IGameMover(const GameXYZ &pos) : isActive(true), curpos(pos) {}
	virtual ~IGameMover() {}
	const GameXYZ &	getpos();
	void	getpos(int &x, int &y, int &z);
	int		getx();
	int		gety();
	int		getz();
	bool	actived();
	virtual void update() = 0;
};

typedef SmartPtr<IGameMover>	GameMoverPtr;

inline void IGameMover::reach()
{
	isActive = false;
}

inline const GameXYZ & IGameMover::getpos()
{
	return curpos;
}

inline bool IGameMover::actived()
{
	return isActive;
}

inline void IGameMover::setx(int i)
{
	curpos.x = i;
}

inline void IGameMover::sety(int i)
{
	curpos.y = i;
}

inline void IGameMover::setz(int i)
{
	curpos.z = i;
}

inline void IGameMover::setpos(int x, int y, int z)
{
	curpos.x = x;
	curpos.y = y;
	curpos.z = z;
}

inline void IGameMover::setpos(const GameXYZ &pos)
{
	curpos = pos;
}

inline void IGameMover::getpos(int &x, int &y, int &z)
{
	x = curpos.x;
	y = curpos.y;
	z = curpos.z;
}

inline int IGameMover::getx()
{
	return curpos.x;
}

inline int IGameMover::gety()
{
	return curpos.y;
}

inline int IGameMover::getz()
{
	return curpos.z;
}
