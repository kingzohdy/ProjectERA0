#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "GameMsgPrint.h"
#include "NetMessage.h"
#include "cs_net.h"
#include "GameDefineTolua.h"
#include <map>

// ��Լ״̬
//tolua_begin
enum 
{
	CLIENT_QIYUE_STATE_INVALID  = 0, 	/* ��Ч */
	CLIENT_QIYUE_STATE_VALID	= 1, 	/* ��Чδѡ�� */
	CLIENT_QIYUE_STATE_SELECTED = 2, 	/* ��Ч��ѡ�� */
};

enum
{
	CLIENT_NO_TEAM	= 0,
	CLIENT_TEAM_MEMBER,
	CLIENT_TEAM_CAPTAIN,
};

enum
{
	CLIENT_JOIN_TEAM	= 0,	// �������
	CLIENT_LEAVE_TEAM	= 1,	// �뿪����
};

enum
{
	CLIENT_MEMBER_ONLINE = 0,		// ����
};

enum tagTEAM_BOOTY_MODE_TOLUA 
{
	TEAM_BOOTY_ROLL_TOLUA = TEAM_BOOTY_ROLL, 	/* ��ɫƷ�ʼ�����ROLL�� */
	TEAM_BOOTY_RAND_TOLUA = TEAM_BOOTY_RAND, 	/* ���ѡ��һ�� */
	TEAM_BOOTY_ALL_TOLUA = TEAM_BOOTY_ALL, 	/* ������Ʒ�������Ա������ʰȡ */
};

enum 
{
	TEAM_FOLLOW_CAPTAIN = 0, 	/* �ӳ�������Ӹ��� */
	TEAM_FOLLOW_MEMBER = 1, 	/* ��Ա������Ӹ��� */
	TEAM_FOLLOW_CANCEL = 2, 	/* ȡ����Ӹ��� */
};

struct tagQiYueLimit_ToLua
{
	unsigned int QiYueLimitID;                   
	unsigned int Val;                            
};
struct tagQiYueResultTarget_ToLua
{
	unsigned int QiYueResultTargetID;            
	int ResultTargetVal1;               
	int ResultTargetVal2;               
	int ResultTargetVal3;               
	int ResultTargetVal4;               
};

struct tagQiYueResult_ToLua
{
	unsigned int QiYueResultID;                  
	int ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;                     
	tagQiYueResultTarget_ToLua Target;                         
};

struct tagQiYueDef_ToLua
{
	unsigned int QiYueID;                        
	char Name[NAME_LEN];                 
	char TypeName[NAME_LEN];              	/*  Ver.140 ��Լ���͵����� */
	tagQiYueLimit_ToLua QiYueLimit[MAX_QIYUE_LIMIT];     	/*   ��Լ�������� */
	tagQiYueResult_ToLua QiYueResults[MAX_QIYUE_RESULT];  	/*   ��Լ���� */
	char QiYueDesc[SHORT_DESC_LEN];      
};

const tagQiYueDef_ToLua* getQiYueDef( int id );
const char* getQiYueDes( int id );
const char* getQiYueDescName( int id );
const char* getQiYueType( int id );
//tolua_end
struct tagPetDiathesis_ToLua;

class GameTeamManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameTeamManager>, public IGameMsgPrint
{//tolua_export
public:
	GameTeamManager(void);
	~GameTeamManager(void);

public:

	//tolua_begin 
	// �Ƿ��ܹ��������
	bool canApplyTeam( const char* pszName );
	
	// ���������������
	void Team_OP_Apply( const char* szName );
	// �Ƿ��ܹ����������������
	bool canAcceptApplyTeam( int nHeightId, int nLowId );
	// ���������������
	void Team_OP_ApplyAccept( int height, int low );
	// �ܾ������������
	void Team_OP_ApplyRefuseAll( );
	// �ܾ��������
	void Team_OP_ApplyRefuse( int height, int low );
	// ����������
	void Team_OP_ApplyClear();
	// �Ƿ��ܹ��������
	bool canInviteTeam( const char* pszName );
	// �Ƿ��ܹ��������
	bool canInviteTeam( int nHeightId, int nLowId );
	// �����������
	void Team_OP_Invite( const char* szName );
	// �Ƿ��ܹ����������������
	bool canAcceptInviteTeam( const char* pszName );
	// �����������
	void Team_OP_InviteAccept( const char* szName );
	// �ܾ��������
	void Team_OP_InviteRefuse( const char* szName, bool bTeamOut = false );
	// ����������
	void Team_OP_InviteClear();
	// �ӳ�ת��
	void Team_OP_CaptainChange( const char* szName );
	// �˳�����
	void Team_OP_Exit();
	// ����������
	void Team_OP_Kick( const char* szName );
	// ��ɢ����
	void Team_OP_Destroy();
	// �������
	void Team_OP_ChangeName( const char* szName );
	// ������Ӹ���
	void Team_OP_AppealTeamFollow();
	// ȡ����Ӹ���
	void Team_OP_CancelTeamFollow();
	//���ܶӳ�����
	void Team_OP_SummonRequest(bool bAgree);
	// �Ƿ��ڽ�����Ӹ���
	bool isInTeamFollow();
	// ��ӣ�������������������
	void Team_OP_Add( const char* szName );
	
	// ˢ����Χ���
	void updateNearPlayer();
	// ��Χ�������
	int  getNearPlayerCount();
	// ͨ��index��ȡ��Χ���
	GameActor* getNearPlayer( int index );

	// ��ȡ������Ϣ
	const tagTeamInfo_ToLua* getTeamInfo();
	// �����б�����
	int getTeamApplyInfoSize();
	// ��ȡ�����б������Ϣ
	tagTeamApplyInfo_ToLua& getTeamApplyInfoByIndex( int nIndex );
	// ��ȡ�����б������Ϣ
	tagTeamApplyInfo_ToLua* getTeamApplyInfo( const char* pszName );
	// ��֯ģ��װ����ʾ����
	void setAvatarAttrib( int i );
	
	// ��ȡ��ӵĶ���ID
	int getMapDefID( int nRealMapID );
	// ��ȡ���鵱ǰ��Լ
	int getTeamQiYue();
	// ��ȡ��ǰ��Ա��װ����ԼID
	int getTeamMemberQiYue( int nIndex );
	// ��ȡ��Ա��Լ״̬
	int getTeamQiYueState( int nIndex );
	// ��Լѡ��( ��������ɫ���� )
	void switchQiYue( const char* szTeamPlayerName );
	
	// ��Ұ��Χ������Ƿ��ڶ�����
	bool isInTeamById( int id );
	// ��ѯĳλ����Ƿ��ڶ�����
	bool isInTeam( const char* pszName );
	void updateActorStatus( GameActor* pActor );
	void updateActorRideStatus( GameActor* pActor );
	// ʹ����Լ
	void useQiYueItem( unsigned int nQiYueID );
	// ɾ����Լ
	void delQiYueItem( unsigned int nQiYueID );
	// ��ȡ��Ա��Ϣ
	TEAMMEMBER* getTeamMemberInfo( GameActor* pActor );
	// ��ȡ�����Ա��Ϣ
	tagTeamMember_ToLua* getTeamMemberInfo( const char* pszName );
	tagTeamMember_ToLua* getTeamMemberInfoById( int id );
	tagCSRoleStatus_ToLua* getOneBuf( const char* pszName, int nId, int nLv ); 

	void changeBootyMode( int nMode );

	// ͶƱT ��
	void requestVoteKick( bool bAgree );

	// ��ȡ��ͶƱ���˵����� 
	const char* getVotePlayerName();
	
	//tolua_end
	
	// ������Ұ��Χ����ҵ���Ϣ��ͬʱ���¶����Ա������
	void updateActorAttr( GameActor* pActor );
	// �����սģʽ���ĶԶ�Ա��״̬Ӱ��
	void updateActorPetStatus( GameActor* pActor, bool bPetFight = false );
	static bool isPetDiathesisEqual( const tagPetDiathesis_ToLua& lhs, const TEAMPETVIEW& rhs );
	
	void getTeamMemberPostion( float *postionlist, float *outeye );
	void showTeamer();
public:
	
	virtual int  HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	
	void releaseTeamInfo();
	void leaveTeam();
	void addTeamTmpFriend( const TEAMINFO& TeamInfo );
	bool isInHaveAddTeamTmpFriend( const char* pszName );
private:
	void delTeamApplyInfo( int height, int low );
	
private:
	std::vector<int>							m_NearPlayerId;			// ��Χ���ID�б�
	TEAMINFO									m_TeamInfo;				// ������Ϣ
	std::vector<tagTeamApplyInfo_ToLua>			m_vectTeamApplyInfo;	// ���������б�
	std::vector<TEAMINFO>						m_vectTeamInviteInfo;	// ���������б�
	std::vector<std::string>					m_vecHaveAddTmpFriends;
	int											m_TeamSummonMemId;		//�ٻ���ID
};//tolua_export
