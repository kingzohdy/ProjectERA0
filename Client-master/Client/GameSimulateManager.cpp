#include "GameSimulateManager.h"
#include "GameManager.h"

GameSimulateManager::GameSimulateManager()
{
	INetworkManager::getSingleton().RegisterMsgHandler(VAL_SVR,this);
	memset(&m_SimulateArmour,0,sizeof(CSVALWATCHINFO));
	memset(&m_SimulateSecond,0,sizeof(CSVALWATCHINFO));
	m_CanSend = true;
}

GameSimulateManager::~GameSimulateManager()
{


}

int GameSimulateManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == VAL_SVR )
	{
		tagCSPkgBody msg=msgbody;
		CSVALSVR	 ValSvr = msg.ValSvr;
		if ( ValSvr.Type ==  VAL_SVR_WATCH_INFO )
		{
			getPlayerinfo( ValSvr.Data.WatchInfo);
		}
		else
		{
			getResult( ValSvr.Data.FightInfo);
			m_CanSend = true;
		}
	}
	return 1;
}

void	GameSimulateManager::getResult(CSVALFIGHTSVR ResultInfo)
{
	m_Result.nAtkCounts		= ResultInfo.AtkCount;
	m_Result.nShootCounts	= ResultInfo.Hit;
	m_Result.nducks			= ResultInfo.Miss;
	m_Result.nTotalhurts	= ResultInfo.TotalHurt;
	m_Result.nAveragehurts  = ResultInfo.AvgHurt;
	m_Result.nDoubleHits	= ResultInfo.HeavyHit;
	m_Result.nNormalMaxHurts= ResultInfo.MaxHitVal;
	m_Result.nNormalMinHurts= ResultInfo.MinHitVal;
	m_Result.nNormalAvrHurts= ResultInfo.AvgHitVal;
	m_Result.nDoubleMaxHurts= ResultInfo.MaxHeavyHitVal;
	m_Result.nDoubleMinHurts= ResultInfo.MinHeavyHitVal;
	m_Result.nDoubleAvrHurts= ResultInfo.AvgHeavyHitVal;
	GameManager::getSingleton().SendUIEvent("GE_SIMULATE_SHOW");
	
}

void	GameSimulateManager::getPlayerinfo(CSVALWATCHINFO Watchinfo)
{
	if ( Watchinfo.MemID == m_nArmourId )
	{
		m_SimulateArmour = Watchinfo;
		GameManager::getSingleton().SendUIEvent("GE_SIMULATE_ARMOUR");
	}
	if ( Watchinfo.MemID == m_nSecondId)
	{
		m_SimulateSecond = Watchinfo;
		GameManager::getSingleton().SendUIEvent("GE_SIMULATE_SECOND");
	}

}

CSATTRIDVAL*	GameSimulateManager::getAttr(int SelectID,int attrId)
{
	CSVALWATCHINFO	*SimulateInfo = getSimulatePlyayer(SelectID);
	for (int i= 0; i< SimulateInfo->FightAttrVal.AttrNum; i++)
	{
		if ( attrId == SimulateInfo->FightAttrVal.AttrList[i].AttrID )
		{
			return &(SimulateInfo->FightAttrVal.AttrList[i].Value);
		}
	}
	return NULL;
}

void	GameSimulateManager::setAttr(int SelectID,int attrId,char* attrval)
{
	int nattrval = atoi(attrval);
	CSATTRIDVAL *AttrInfo=getAttr( SelectID,attrId );
	if ( AttrInfo !=NULL )
	{
		memcpy( AttrInfo, &nattrval, sizeof(int));
	}
}

CSVALWATCHINFO* GameSimulateManager::getSimulatePlyayer(int SelectID)
{
	if ( SelectID == SimArmourID )
	{
		return &m_SimulateArmour;
	}
	return &m_SimulateSecond;
}

void	GameSimulateManager::setAtkLevelMul(int SelectID, char* attrval)
{
	CSVALWATCHINFO	*SimulateInfo = getSimulatePlyayer(SelectID);
	SimulateInfo->AtkLevelMul=atoi(attrval);
}

void	GameSimulateManager::setDefLevelMul(int SelectID, char* attrval)
{
	CSVALWATCHINFO	*SimulateInfo = getSimulatePlyayer(SelectID);
	SimulateInfo->DefLevelMul = atoi(attrval);
}

void	GameSimulateManager::requestWatchInfo(char* MemId)
{
	tagCSPkgBody body;
	body.ValClt.Type = VAL_CLT_WATCH;
	int nWatchId = atoi(MemId);
	body.ValClt.Data.Watch.MemID = nWatchId;
	SendGameServerMsg(VAL_CLT,body);
}

void	Simulatecltgetinfo(CSFIGHTOBJINFO *pfightinfo, CSVALWATCHINFO actorinfo)
{
	pfightinfo->MemID			= actorinfo.MemID;
	pfightinfo->AtkLevelMul		= actorinfo.AtkLevelMul;
	pfightinfo->DefLevelMul		= actorinfo.DefLevelMul;
	pfightinfo->FightAttrVal	= actorinfo.FightAttrVal;
}

void	GameSimulateManager::requestSimulatefight()
{
	if ( m_CanSend )
	{
	tagCSPkgBody body;
	body.ValClt.Type				= VAL_CLT_FIGHT;
	body.ValClt.Data.Fight.AtkerID	= m_nAtkerId;
	body.ValClt.Data.Fight.Count		= m_nCounts;
	body.ValClt.Data.Fight.SkillID		= m_nSkillId;
	body.ValClt.Data.Fight.SkillLevel	= m_nSkillLevel;
	Simulatecltgetinfo(&(body.ValClt.Data.Fight.ObjInfo[0]),m_SimulateArmour);
	Simulatecltgetinfo(&(body.ValClt.Data.Fight.ObjInfo[1]),m_SimulateSecond);
	SendGameServerMsg(VAL_CLT,body);
	m_CanSend =false;
	}
	else
	{
		GameManager::getSingleton().SendUIEvent("GE_SIMULATE_ERROR");
	}
}

void		GameSimulateManager::setAtkerId(int selectid)
{
	if ( selectid == SimArmourID )
	{
		m_nAtkerId = m_nArmourId; 
	}
	else
	{
		m_nSecondId = m_nSecondId;
	}
}

void		GameSimulateManager::setSkillId(char* skillid)
{
	m_nSkillId = atoi(skillid);
}

void		GameSimulateManager::setSkillLevel(char* skilllevel)
{
	m_nSkillLevel = atoi(skilllevel);
}

void		GameSimulateManager::setAtkCounts(char* counts)
{
	m_nCounts = atoi(counts);
}

void		GameSimulateManager::setArmourId(char* memId)
{
	m_nArmourId = atoi(memId);
}

void		GameSimulateManager::setSecondId(char* memId)
{
	m_nSecondId = atoi(memId);
}

short	GameSimulateManager::getshortValue(int SelectID,int attrId)
{
	CSATTRIDVAL* Cstemp = getAttr(SelectID,attrId);
	if ( Cstemp == NULL ) 
	{
		return 0;
	}
	else
	{
		return *((short*)Cstemp);
	}
}

unsigned short	  GameSimulateManager::getunshortValue(int SelectID,int attrId)
{
	CSATTRIDVAL* Cstemp = getAttr(SelectID,attrId);
	if ( Cstemp == NULL ) 
	{
		return 0;
	}
	else
	{
		return *((unsigned short*)Cstemp);
	}
}

int		GameSimulateManager::getAtkLevelMul(int SelectID)
{
	CSVALWATCHINFO	*SimulateInfo = getSimulatePlyayer(SelectID);
	return SimulateInfo->AtkLevelMul;
}

int			GameSimulateManager::getDefLevelMul(int SelectID)
{
	CSVALWATCHINFO	*SimulateInfo = getSimulatePlyayer(SelectID);
	return SimulateInfo->DefLevelMul;
}

void		GameSimulateManager::Reset()
{
	m_CanSend = true;
}