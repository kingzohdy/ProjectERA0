#ifndef __CLIENTINTERFACE_H__
#define __CLIENTINTERFACE_H__

class GameActor;

template <class T>
class SinglePtr
{
private:
	static T *instance;

protected:
	SinglePtr();
	virtual ~SinglePtr();

public:
	static T *	getInstance();
};

template <class T> T *SinglePtr<T>::instance = 0;

template <class T>
SinglePtr<T>::SinglePtr()
{
	if (instance == 0)
	{
		instance = (T *)this;
	}
}

template <class T>
SinglePtr<T>::~SinglePtr()
{
	if (instance == this)
	{
		instance = 0;
	}
}

template <class T>
T * SinglePtr<T>::getInstance()
{
	return instance;
}

class IGameBattle : public SinglePtr<IGameBattle>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
	virtual void objLeaveView(GameActor *actor) = 0;
};

class IGameClanManager : public SinglePtr<IGameClanManager>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
};

class IGameFriendManager : public SinglePtr<IGameFriendManager>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
};

class IGameWizard : public SinglePtr<IGameWizard>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
	virtual void objLeaveView(GameActor *actor) = 0;
	virtual void useContainerItem( int itemId, int listType, int GridIdx ) = 0;
};

class IWorldCityManager : public SinglePtr<IWorldCityManager>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
	virtual void objLeaveView(GameActor *actor) = 0;
	virtual bool objCanAttack(GameActor *actor) = 0;
};
class IGameSparta : public SinglePtr<IGameSparta>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
};
class IGameSpanStrong : public SinglePtr<IGameSpanStrong>
{
public:
	virtual void objEnterView(GameActor *actor) = 0;
};

#endif
