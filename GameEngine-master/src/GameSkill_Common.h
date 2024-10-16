#pragma once
#include "GameSkill_Base.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameSkill_IControl.h"

#define DECLARE_SKILL()\
	static GameSpell* create();\
	static bool init();\
	static void destory();\
	static bool save();\
	static bool load();\
	static int  getAttckDis(int id);\
	static std::map<int, std::vector<void *>>	m_SkillAttrib;\
	static std::string m_SkillType;\
	public:\
	virtual void* getAttrib();
	
#define IMPLEMENT_SKILL(class_name,class_targer)\
	std::string class_name::m_SkillType(#class_targer);\
	std::map<int, std::vector<void *>> class_name::m_SkillAttrib;\
	GameSpell* class_name::create()\
	{\
		return new class_name();\
	}\
	bool class_name::init()\
	{\
		return load();\
	}\
	void class_name::destory()\
	{\
		for(std::map<int, std::vector<void *>>::iterator i = m_SkillAttrib.begin(); i != m_SkillAttrib.end(); ++i )\
		{\
			for (std::vector<void *>::iterator j = i->second.begin(); j != i->second.end(); ++j)\
			{\
				delete ((class_targer*)*j);\
			}\
		}\
		m_SkillAttrib.clear();\
	}\
	void* class_name::getAttrib()\
	{\
		if (m_pSkillAttrib)\
			return 	m_pSkillAttrib;\
		m_pSkillAttrib = getBestAttrib(m_SkillAttrib[getSpellId()]);\
		return m_pSkillAttrib;\
	}

//tolua_begin
struct tagCDTime
{
	unsigned int		LastTime;
	unsigned int		startTime;
	tagCDTime()
	{
		startTime = LastTime = 0;
	}
	unsigned int past() const
	{
		if (startTime == 0)
		{
			return 0;
		}
		unsigned int now = getSystemTick();
		if (now > LastTime)
		{
			return total();
		}
		return now - startTime;
	}

	unsigned int remain() const
	{
		unsigned int now = getSystemTick();
		if (now > LastTime)
		{
			return 0;
		}
		return LastTime - now;
	}

	unsigned int total() const
	{
		return LastTime - startTime;
	}
};
//tolua_end
