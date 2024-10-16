#ifndef _GAMEACTOR_PVP_H
#define _GAMEACTOR_PVP_H
#include "GameActor.h"
#include "proto_comm.h"
#include "OgreSingleton.h"
#include <list>

class IJudgeCampIdx
{
public:
	virtual int		isSameCampIdx(GameActor *pActor) = 0;// 插入式判断是否同一阵营，返回=0表示无关，继续进行下一个判断，返回>0表示同一阵营，返回<0表示不同阵营
};

class GameActor_PVP//tolua_export
{//tolua_export
private:
	int						m_forcePkMode;
	int						m_idTeamMemberName[MAX_TEAM_MEMBER];
	char					m_strTeamMemberName[MAX_TEAM_MEMBER][NAME_LEN];
	tdr_ulonglong			m_widTeamMemberName[MAX_TEAM_MEMBER];
	unsigned int			m_nTeamMemberNum;
	CLANID					m_lstEnemyClan[CLAN_MAX_ENEMY];
	size_t					m_nEnemyClanNum;

	void *					fontTexture[2];

	std::list<IJudgeCampIdx *>	m_judgerList;

	GameActor_PVP();

public:
	void	judgeCamp(GameActor *pActor, bool IsRookieProtect);
	bool	hasCrime(GameActor *pActor);
	void	forcePkMode(int mode);
	void	onPkValueChanged(int value);
	void	onPkModeChanged(int mode);

	bool	isSameTeam(GameActor *pActor);
	bool	isSameTeam(int id);
	bool	isSameTeam( tdr_ulonglong wid);
	bool	isSameClan(GameActor *pActor);
	int		isSameCampIdx( GameActor *pActor );
	bool	isSameGroup(GameActor *pActor);
	bool	isEnemyClan(GameActor *pActor);
	bool	isCrime(GameActor *pActor);
	void	setTeamMember(int id, const char *name, tdr_ulonglong wid);
	void	resetTeamMember();
	void	setEnemyClan(CLANID enemy);
	void	resetEnemyClan();

	void	registerCampIdxJudger(IJudgeCampIdx *handle);

//tolua_begin
	int		getPkMode();
	bool	isForcePkMode();
	int		getAmok(GameActor *pActor);
	int		getAmok(int amok);
	int		getPkValue();
	void	changePKMode(int mode);
	void	changeNextPkMode();
	void	requestSafePK( const char* szRoleName );
	void	requestSafePKRes( int nFlag,const char* szRoleName );
//tolua_end

	static GameActor_PVP& getSingleton()
	{
		static GameActor_PVP singleton;
		return singleton;
	}
};//tolua_export

#endif
