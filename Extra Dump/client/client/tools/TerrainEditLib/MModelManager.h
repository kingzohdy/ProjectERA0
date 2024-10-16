#pragma once
#include "mmanager.h"


namespace TE
{

class MModelManager :
	public SimpleManager
{
public:
	MModelManager(void);
	~MModelManager(void);
public:
	int add(std::string name);
	int addEX(std::string name);
    

	int v;

	void resetAnim();
	void updateEmitters(float dt);
	void clear();

	ManagedItem*  GetIDObject( int id )
	{
		if(id >= 0 && id < itemset.size() )
		    return itemset[id];

		return NULL;
	}

    ManagedItem*  GetObject(std::string name)
	{
		return items[get(name)];
	}

};

};
extern TE::MModelManager g_ModelManager;