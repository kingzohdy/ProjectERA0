#ifndef __GAMESIMULATEMANAGER_H__
#define __GAMESIMULATEMANAGER_H__
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "GameDefineTolua.h"
//tolua_begin
enum SimulateplayerId
{
	SimArmourID =0,
	SimSecondID =1	
};
struct SimulateResult
{
	int nAtkCounts;
	int nShootCounts,nducks;
	int nTotalhurts,nAveragehurts;
	int nDoubleHits;
	int nNormalMaxHurts,nNormalMinHurts,nNormalAvrHurts;
	int nDoubleMaxHurts,nDoubleMinHurts,nDoubleAvrHurts;
};
//tolua_end
class GameSimulateManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameSimulateManager>
{//tolua_export
public: 
	GameSimulateManager();
	~GameSimulateManager();
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void		 getPlayerinfo(CSVALWATCHINFO Watchinfo);
	void		getResult(CSVALFIGHTSVR ResultInfo);
	CSATTRIDVAL* getAttr(int SelectID,int attrId);
	CSVALWATCHINFO* getSimulatePlyayer(int SelectID);
	
	//tolua_begin
	void		 setAttr(int SelectID,int attrId,char* attrval);
	void		 setAtkLevelMul(int SelectID, char* attrval);
	void		 setDefLevelMul(int SelectID, char* attrval);
	void		 requestWatchInfo(char* MemId);
	void		 requestSimulatefight();
	void		 setAtkerId(int SelectID);
	void		 setSkillId(char* skillid);
	void		 setSkillLevel(char* skilllevel);
	void		 setAtkCounts(char* counts);
	void		 setArmourId(char* memId);
	void		 setSecondId(char* memId);
	short		  getshortValue(int SelectID,int attrId);
	unsigned short	  getunshortValue(int SelectID,int attrId);
	int			getAtkLevelMul(int SelectID);
	int			getDefLevelMul(int SelectID);
	void		Reset();
	SimulateResult	getResultshow()
	{
		return m_Result;
	}
	//tolua_end
private:
	int				m_nArmourId,m_nSecondId;
	int				m_nAtkerId;
	int				m_nCounts;
	int				m_nSkillId;
	int				m_nSkillLevel;
	CSVALWATCHINFO	m_SimulateArmour;
	CSVALWATCHINFO	m_SimulateSecond;
	SimulateResult	m_Result;
	bool			m_CanSend;

};//tolua_export
#endif