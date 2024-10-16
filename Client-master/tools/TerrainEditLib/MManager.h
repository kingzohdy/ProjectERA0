#pragma once

class MManager
{
public:
	MManager(void);
	~MManager(void);
};


#include <string>
#include <map>

#include <vector>

class ManagedItem {
	int refcount;
public:
	std::string name;
	ManagedItem():refcount(0){};
	ManagedItem(std::string n): name(n), refcount(0) {}
	virtual ~ManagedItem() {}

	void SetName(const char* _name){name = _name;}
	void addref()
	{
		++refcount;
	}

	bool delref()
	{
		return --refcount==0;
	}
	
};



template <class IDTYPE>
class Manager {
public:
	std::map<std::string, IDTYPE> names;
	std::map<IDTYPE, ManagedItem*> items;

	//Ôö¼Ó
	std::vector <ManagedItem*>   itemset;

	Manager()
	{
	}

	virtual IDTYPE add(std::string name) = 0;

	virtual void del(IDTYPE id)
	{
		if (items.find(id) == items.end()) return;
		if (items[id]->delref()) {
			ManagedItem *i = items[id];
			doDelete(id);
			names.erase(names.find(i->name));
			items.erase(items.find(id));
			delete i;
		}
	}

	void delall()
	{
		names.clear();
		items.clear();
	}

	void delbyname(std::string name)
	{
		if (has(name)) del(get(name));
	}

	virtual void doDelete(IDTYPE id) {}

	bool has(std::string name)
	{
		return (names.find(name) != names.end());
	}

	IDTYPE get(std::string name)
	{
		return names[name];
	}

protected:
	void do_add(std::string name, IDTYPE id, ManagedItem* item)
	{
		names[name] = id;
		item->addref();
		items[id] = item;
	}
};

class SimpleManager : public Manager<int> {
	int baseid;
public:
	SimpleManager() : baseid(0)
	{
	}

protected:
	int nextID()
	{
		return baseid++;
	}
	void clearID()
	{
		baseid = 0;
	}
};