#include "OgreXMLData.h"
#include "OgreStringUtil.h"
#include "GameSkill_Base.h"
#include "GameSkill_IControl.h"
#include "SkillBlow.h"
#include "SkillAttackLeaveEff.h"
#include "SkillExtraHitEff.h"
#include "SkillCastEff.h"
#include "SkillHitLeaveEff.h"
#include "Skilllink.h"
#include "SkillAttackPointEff.h"
#include "SkillHitPointEff.h"
#include "SkillPrepareAttackPointEff.h"
#include "SkillPreLockEff.h"
#include "GameSkill_Mgr.h"
#include <stdio.h>
#include <Windows.h>

using namespace Ogre;

void GameSpell::parserSkillAttrib( XMLNode& child, tagSkillAttrib* p, ATTRIB_MAP& attribMap )
{
	wchar_t buff[1024];
	char buff2[1024];
	if (child.hasAttrib("id") && child.attribToInt("id", p->m_nSkillId))
	{
		if (SkillManager::getSingleton().getSpellDes(p->m_nSkillId))
		{
			char buff[100];
			_snprintf(buff, 100, "技能%d重复定义", p->m_nSkillId);
			::MessageBox(NULL, buff, "错误", MB_OK);
		}
	}
	if (!child.hasAttrib("behitdelay") || !child.attribToInt("behitdelay", p->m_nBehitDelay))
	{
		
	}

	if (!child.hasAttrib("behitId") || !child.attribToInt("behitId", p->m_BehitId))
	{
		
	}

	if (!child.hasAttrib("preId") || !child.attribToInt("preId", p->m_PreActId))
	{
		
	}

	if (!child.hasAttrib("collectId") || !child.attribToInt("collectId", p->m_CollectActId))
	{
		
	}

	if (!child.hasAttrib("actId1") || !child.attribToInt("actId1", p->m_ActId[0]))
	{
		
	}

	if (!child.hasAttrib("actId2") || !child.attribToInt("actId2", p->m_ActId[1]))
	{
		
	}

	if (!child.hasAttrib("actId3") || !child.attribToInt("actId3", p->m_ActId[2]))
	{
		
	}

	if( child.attribToString("preeffectname") )
	{
		p->m_strPreEffName = child.attribToString("preeffectname");
	}

	if (!child.hasAttrib("hitbacktype") || !child.attribToInt("hitbacktype", p->m_nHitBackType))
	{
		
	}

	if (!child.hasAttrib("hitbackspeed") || !child.attribToInt("hitbackspeed", p->m_nHitBackSpeed))
	{
		
	}

	if (!child.hasAttrib("hitbackhight") || !child.attribToInt("hitbackhight", p->m_nHitBackHight))
	{
		
	}

	if (!child.hasAttrib("backtime") || !child.attribToInt("backtime", p->m_nHitBackTime))
	{
		
	}

	if ( child.hasAttrib("backinfo") )
	{
		p->m_strHitBackExtraInfo = child.attribToString("backinfo");
	}

	if (!child.hasAttrib("movetime") || !child.attribToInt("movetime", p->m_nMoveTime))
	{
		
	}

	if (!child.hasAttrib("hideweapon") || !child.attribToInt("hideweapon", p->m_nHideWeapon))
	{
	}

	if (child.hasAttrib("prehideweapon"))
	{
		p->m_isPreHideWeapon = child.attribToBool("prehideweapon");
	}

	if (child.hasAttrib("nobreak"))
	{
		p->m_isNoBreak = child.attribToBool("nobreak");
	}

	if( child.attribToString("attackeffname") )
	{
		p->m_ActEffName	= child.attribToString("attackeffname");
	}

	if( child.attribToString("hiteffectname") )
	{
		p->m_HitEffName	= child.attribToString("hiteffectname");
	}

	if( child.attribToString("collecteffname") )
	{
		p->m_CollectEffectName = child.attribToString("collecteffname");
	}

	if( child.attribToString("collectblasteffname") )
	{
		p->m_CollectBlastEffectName	= child.attribToString("collectblasteffname");
	}

	if( child.attribToString("collect1") )
	{
		p->m_CollectPower[0] = child.attribToString("collect1");
	}

	if( child.attribToString("collect2") )
	{
		p->m_CollectPower[1] = child.attribToString("collect2");
	}

	if( child.attribToString("collect3") )
	{
		p->m_CollectPower[2] = child.attribToString("collect3");
	}

	if( child.hasAttrib("traceeff") )
	{
		p->m_strTraceEff = child.attribToString("traceeff");
	}

	if (!child.hasAttrib("tracefade") || !child.attribToInt("tracefade", p->m_nTraceFadeTime))
	{
		
	}

	if (!child.hasAttrib("tracetime") || !child.attribToInt("tracetime", p->m_nTraceTotalTime))
	{
		
	}

	if (!child.hasAttrib("tracespeed") || !child.attribToInt("tracespeed", p->m_nTraceSpeed))
	{
		
	}

	if (child.attribToString("script"))
	{
		p->m_strScript = child.attribToString("script");
	}

	if( child.hasAttrib("motionectname") )
	{
		p->m_lstObserver.push_back(SkillExtraHitEff::create(child));
	}

	if( child.hasAttrib("casteff") )
	{
		p->m_lstObserver.push_back(SkillCastEff::create(child));
	}

	if ( child.hasAttrib("linkeffect"))
	{
		p->m_lstObserver.push_back(SkillLink::create(child));
	}

	if ( child.hasAttrib("bloweff"))
	{
		p->m_lstObserver.push_back(SkillBlow::create(child));
	}

	if( child.hasAttrib("attackleaveeff") )
	{
		p->m_lstObserver.push_back(SkillAttackLeaveEff::create(child));
	}

	if( child.hasAttrib("hitleaveeff") )
	{
		p->m_lstObserver.push_back(SkillHitLeaveEff::create(child));
	}

	if( child.hasAttrib("attackpointeff") )
	{
		p->m_lstObserver.push_back(SkillAttackPointEff::create(child));
	}

	if( child.hasAttrib("hitpointeff") )
	{
		p->m_lstObserver.push_back(SkillHitPointEff::create(child));
	}

	if( child.hasAttrib("preattackeff") )
	{
		p->m_lstObserver.push_back(SkillPrepareAttackPointEff::create(child));
	}
	if( child.hasAttrib("prelockname") )
	{
		p->m_lstObserver.push_back(SkillPreLockEff::create(child));
	}
	if( child.attribToString("name") )
	{
		StringUtil::UTF8ToUnicode( buff , 1024 , child.attribToString("name") );
		StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
		p->m_Name = buff2;	
	}

	if( child.attribToString("title") )
	{
		StringUtil::UTF8ToUnicode( buff , 1024 , child.attribToString("title") );
		StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
		p->m_Title = buff2;
	}
	//////////////////////////////////////////////////////////////////////////
	if( child.hasAttrib("flyspeed") )
	{
		p->m_flySpeed	= child.attribToFloat("flyspeed");
	}
	else
	{
		p->m_flySpeed = 0;
	}

	if( child.hasAttrib("throwtype") )
	{
		p->m_strThrowtype	= child.attribToString("throwtype");
	}

	if( child.hasAttrib("flyactorname") )
	{
		p->m_strFlyActorName = child.attribToString("flyactorname");	
	}

	if (!child.hasAttrib("flydesanchorid") || !child.attribToInt("flydesanchorid", p->m_nFlyDesAnchorId))
	{
		p->m_nFlyDesAnchorId = 0;
	}

	if (!child.hasAttrib("flysrcanchorid") || !child.attribToInt("flysrcanchorid", p->m_nFlySrcAnchorId))
	{
		p->m_nFlySrcAnchorId = 0;
	}

	if (!child.hasAttrib("flyheight") || !child.attribToFloat("flyheight", p->m_nFlyHeightRadio))
	{
		p->m_nFlyHeightRadio = 0;
	}

	if (!child.hasAttrib("attacktime") || !child.attribToInt("attacktime", p->m_nFlyActorNum))
	{
		p->m_nFlyActorNum = 1;
	}

	if (!child.hasAttrib("attackdelay") || !child.attribToInt("attackdelay", p->m_nFlyActorDelay))
	{
		p->m_nFlyActorDelay = 0;
	}

	if (child.hasAttrib("flymulti"))
	{
		p->m_bFlyMulti = child.attribToBool("flymulti");
	}
	else
	{
		p->m_bFlyMulti = false;
	}

	attribMap["id"]						= &p->m_nSkillId;		
	attribMap["behitdelay"]				= &p->m_nBehitDelay;
	attribMap["behitId"]				= &p->m_BehitId;
	attribMap["preId"]					= &p->m_PreActId;
	attribMap["collectId"]				= &p->m_CollectActId;
	attribMap["actId1"]					= &p->m_ActId[0];	
	attribMap["actId2"]					= &p->m_ActId[1];
	attribMap["actId3"]					= &p->m_ActId[2];
	attribMap["preeffectname"]			= &p->m_strPreEffName;
	attribMap["attackeffname"]			= &p->m_ActEffName;
	attribMap["hiteffectname"]			= &p->m_HitEffName;
	attribMap["collecteffname"]			= &p->m_CollectEffectName;
	attribMap["collectblasteffname"]	= &p->m_CollectBlastEffectName;
	attribMap["collect1"]				= &p->m_CollectPower[0];
	attribMap["collect2"]				= &p->m_CollectPower[1];
	attribMap["collect3"]				= &p->m_CollectPower[2];
	attribMap["type"]					= &p->m_strType;
	attribMap["name"]					= &p->m_Name;
}