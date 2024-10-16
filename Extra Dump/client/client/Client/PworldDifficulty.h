#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

class PworldDifficulty//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<PworldDifficulty>
{//tolua_export
public:
	//tolua_begin
	enum
	{
		EASY = DIFF_PWORLD_GRADE_D,
		GENERAL = DIFF_PWORLD_GRADE_C,
		HARD = DIFF_PWORLD_GRADE_B,
		EXPERT = DIFF_PWORLD_GRADE_A,

		MAXNUM = MAX_DIFF_PWORLD_LIST,
	};
	struct Tip
	{
		char RoleName[NAME_LEN];             
		char Msg[MAX_DIFF_PWORLD_TIP_LEN];    	/*   队员不符合条件提示信息 */
	};
	//tolua_end
public:
	PworldDifficulty();
	~PworldDifficulty();

public:
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	//tolua_begin
	int				getDifficulty(int diffPworldId, int pworldId);
	int				getDifficulty(int pworldId);
	const char *	getDiffPworldName(int diffPworldId);
	const char *	getDiffPworldDesc(int diffPworldId);
	int				getDifficultyPworldId(int diffPworldId, int difficulty);
	const char *	getDifficultyDesc(int diffPworldId, int difficulty);
	int				getDifficultyReward(int diffPworldId, int difficulty, int index);
	void			enterPworld(int npcId, int diffPworldId, int pworldId);
	void			forceEnterPworld(int npcId, int diffPworldId, int pworldId);
	int				getPworldReferMapId(int pworldId);
	bool			isPworldLimit(int diffPworldId, int difficulty);
	//tolua_end

private:
	void		realEnterPworld(int npcId, int diffPworldId, int pworldId, bool confirm);

public:
	//tolua_begin
	int		tipNum;
	Tip		tips[MAX_TEAM_MEMBER];
	//tolua_end
	
};//tolua_export