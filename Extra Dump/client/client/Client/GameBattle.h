#pragma once

#include <vector>
#include <set>
#include <map>
#include "proto_comm.h"
#include "NetMessage.h"
#include "GameActor_PVP.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreSingleton.h"
#include "BattleLimitInfo.h"
#include "GameMsgPrint.h"
#include "ClientInterface.h"
#include "GameDefineTolua.h"

//tolua_begin
#define XUKONG_NPC_ID_TOLUA XUKONG_NPC_ID

struct tagXuKongPosDef_ToLua
{
	short Camp;                           
	short PosIndex;                       
	tagResPos_ToLua Pos;                            
};
enum tagPWORLD_SVR_MATCH_STATUS_ToLua
{
	PWORLD_SVR_MATCH_CAN_NOT_TOLUA = 0, 	/* �������ֶ�ƥ�� */
	PWORLD_SVR_MATCH_CAN_TOLUA = 1, 	/* �����ֶ�ƥ���� */
	PWORLD_SVR_MATCH_DOING_TOLUA = 2, 	/* ����ƥ�� */
	PWORLD_SVR_MATCH_DONE_TOLUA = 3, 	/* ƥ����� */
};
struct ClanWarCampInfo
{
	int CampID;                          	/*   ��Ӫid */
	int CampVal;                         	/*   ��Ӫ���� */
	char CampName[NAME_LEN];              	/*   ��Ӫ���� */
	int CampHp;								/*   ��ŦѪ��  */
	int CampHpMax;							/*   ��ŦѪ�����  */
};
enum tagTEAM_JOIN_TYPE_TOLUA
{
	TEAM_JOIN_SINGLE_TOLUA = 0, 	/* ���� */
	TEAM_JOIN_TEAM_TOLUA = 1, 	/* ���� */
	TEAM_JOIN_CLAN_TOLUA = 2, 	/* ���� */
};
//tolua_end

class GameBattle//tolua_export
	: public NetMsgHandler, public IJudgeCampIdx, public IDrawTitleIcon, public IGameBattle, public Ogre::Singleton<GameBattle>
{//tolua_export
public:
	GameBattle(void);
	~GameBattle(void);

public:
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual int		isSameCampIdx(GameActor *pActor);
	virtual void *	GetTitleIcon(GameActor *pActor);
	virtual int		GetPriority();
	virtual void	objEnterView(GameActor *actor);
	virtual void	objLeaveView(GameActor *actor);

	void	onEnterPworld(int mapid);
	void	onLeavePworld(int mapid);

	//tolua_begin
	bool			isTeamInBattle(int id);
	int				GetBattleActiveId(int id);
	int				getCurrentBattleId();
	unsigned int	getBattleNum();
	const char *	getBattleDesc(unsigned int i);
	const char *	getBattleWin(unsigned int i);
	int				getBattleID(unsigned int i);
	int				getBattleCamp(unsigned int i);
	int				getBattlePlayerMax(unsigned int i);
	const char *	getBattleName(unsigned int i);
	int				getBattleMinLv(unsigned int i);
	int				getBattlePworld(unsigned int i);
	int				getBattleTexture(unsigned int i);

	unsigned int	getPlayerIdx(int id);
	unsigned int	getPlayerNum();
	int				getPlayerID(unsigned int i);
	const char *	getPlayerName(unsigned int i);
	const char *	getPlayerClan(unsigned int i);
	int				getPlayerCarrer(unsigned int i);
	int				getPlayerCamp(unsigned int i);
	int				getPlayerKill(unsigned int i);
	int				getPlayerReward(unsigned int i);
	int				getPlayerRewardNum(unsigned int i);
	int				getPlayerRewardEx(unsigned int i);
	int				getPlayerRewardNumEx(unsigned int i);

	int				getPlayerCrystal(unsigned int i);
	int				getPlayerOre(unsigned int i);
	int				getPlayerAssist(unsigned int i);
	int				getPlayerMaxContinueKill(unsigned int i);
	int				getPlayerDieNum(unsigned int i);
	int				getPlayerMaxLife(unsigned int i);
	int				getPlayerPolyFraction(unsigned int i);
	int				getPlayerExtraExp(unsigned int i);
	int				getPlayerWinGlory(unsigned int i);
	int				getPlayerFailGlory(unsigned int i);
	int				getPlayerRankGlory(unsigned int i);
	int				getPlayerNumGlory(unsigned int i);
	int				getPlayerExp(unsigned int i);
	int				getPlayerFristWinExp(unsigned int i);
	int				getPlayerFiniNum(unsigned int i);
	int				getTotalCanFiniNum();
	int				getMainPlayerCurKill();
	int				getBattlePworldById(int id);

	void			EnterPworld();
	void			LeavePworld();
	void			ExitQueue( int teamJoinType = TEAM_JOIN_SINGLE_TOLUA );
	bool			isRun();
	bool			isWaiting();
	void			join(int id, int nType);
	void			reset();
	void			clearWaitings();
	bool			isWinGame();
	int				getPlayerEnergy(unsigned int i);

	const tagXuKongPosDef_ToLua* getXuKongPosDef( int nCamp, int nPosIdx );

	int				getPlayerBull( unsigned int i );
	//�Ƿ�Ҫ�᳤/���᳤����������
	bool			isNeedClanLeaderOpenApply( int id );
	//ս���ֶ�ƥ��
	void			requestClanWarMatch( int id );
	//ս��ȡ��ƥ��
	void			requestClanWarMatchCancle( int id );
	//���𹫻��Ա�������ս����
	void			requestClanWarApplyInvite( int id, const char* szName );

	//��ȡ��ǰƥ��״̬
	int				getClanWarMatchStatus();
	//��ȡ��ǰƥ������
	int				getClanWarMatchPlayerNum();
	//��ȡ��ǰƥ��ս��id
	int				getClanWarMatchPworldID();

	//��ȡ��Ӫid
	int				getClanWarCampId( int idx );
	//��ȡ��Ӫ����
	int				getClanWarCampVal( int idx );
	//��ȡ��Ӫ����
	const char* 	getClanWarCampName( int idx );
	//������Ӫ��ŦѪ��
	void			setClanWarCampHp( int idx, int hp );
	//��ȡ������Ӫ��ŦѪ��
	int				getClanWarCampHp( int idx );
	//������Ӫ��Ŧ���Ѫ��
	void			setClanWarCampMaxHp( int idx, int maxHp );
	//��ȡ������Ӫ��Ŧ���Ѫ��
	int				getClanWarCampMaxHp( int idx );
	//��ȡ����ս������״̬
	bool			isClanBattleOpen( int id );

	void			setClanBattleApplyInfo( int id, int OpType );
	//tolua_end
	void			SetEnergyPoint( int val1, int val2 );

private:
	std::map<int, size_t>		m_roleIdx;
	int							m_Pworld;
	std::set<int>				m_PworldQueue;
	CSBATTLEROLEINFO			m_lstRole[MAX_PWORLD_MEMBER];
	void *						fontTexture[3];
	unsigned int				m_RoleNum;
	int							m_VCamp;
	int							m_BattleType;

	tagCSPworldMatchStatus		m_BattleMatchStatus;
	ClanWarCampInfo					m_BattleCampInfo[MAX_CAMP_NUM];
	tagCSActionClanBattleApplyInfo			m_ClanBattleApplyInfo;		//����֮�ǹ���ս������״̬,������һ����¼������һ��������һ���Ͳ��ܿ���
};//tolua_export
