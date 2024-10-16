#pragma once
#include <vector>
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
//const int NONE_AUTO_TEAM = -1;
bool isRoleInKuaFu( const char* pszName );
enum 
{
	CLT_AUTO_TEAM_LIMIT_NONE		= 0,						/* û���κ����� */
	CLT_AUTO_TEAM_LIMIT_EASY		= AUTO_TEAM_LIMIT_EASY,		/* �����Ѷȼ� */
	CLT_AUTO_TEAM_LIMIT_NORM		= AUTO_TEAM_LIMIT_NORM,		/* �����Ѷ���ͨ */
	CLT_AUTO_TEAM_LIMIT_DIFF		= AUTO_TEAM_LIMIT_DIFF,		/* �����Ѷ����� */
	CLIENT_AUTO_TEAM_LIMIT_NIGHTMARE= AUTO_TEAM_LIMIT_NIGHTMARE,/* �����Ѷ�ר�� */
	CLT_AUTO_TEAM_LIMIT_WARRIOR		= AUTO_TEAM_LIMIT_WARRIOR, 	/* ��Ҫսʿְҵ */
	CLT_AUTO_TEAM_LIMIT_MAGICOR		= AUTO_TEAM_LIMIT_MAGICOR, 	/* ��Ҫ��ʦְҵ */
	CLT_AUTO_TEAM_LIMIT_GUNER		= AUTO_TEAM_LIMIT_GUNER, 	/* ��Ҫǹ��ְҵ */
	CLT_AUTO_TEAM_LIMIT_CHUNCH		= AUTO_TEAM_LIMIT_CHUNCH, 	/* ��Ҫ��˾ְҵ */
	CLT_AUTO_TEAM_LIMIT_LEADER		= AUTO_TEAM_LIMIT_LEADER, 	/* ��Ҫ���ӳ� */
};

enum 
{
	CLT_PRE_MEMBER_NOTDECIDE= 0,	// û��̬
	CLT_PRE_MEMBER_AGREE	= 1,	// ͬ��
	CLT_PRE_MEMBER_REJECT,			// �ܾ�

	CLT_AUTO_TEAM_STATE_IN,			// �Ѿ������Զ����
	CLT_AUTO_TEAM_STATE_PREPARE,	// �����Զ���ӵ�׼��״̬
	CLT_AUTO_TEAM_STATE_NONE,		// �������Զ���ӵ����״̬
};

enum
{
	CLT_AUTO_TEAM_TASK = 0,		//  ����
	CLT_AUTO_TEAM_ACTIVE = 1,	//  �
	CLT_AUTO_TEAM_CLAN = 2, 	/* ���� */
};

enum
{
	CLT_CAREER_WARRIOR	= 1,	// սʿ
	CLT_CAREER_MAGICOR	= 2,	// ��ʦ
	CLT_CAREER_GUNER	= 3,	// ǹ��
	CLT_CAREER_CHURCH	= 4,	// ��ʦ
};

enum
{
	CLT_PWORLD_SIMPLE		= DIFF_PWORLD_GRADE_D,
	CLT_PWORLD_NORMAL		= DIFF_PWORLD_GRADE_C,
	CLT_PWORLD_HARD			= DIFF_PWORLD_GRADE_B,
	CLT_PWORLD_PROFESSIONAL	=DIFF_PWORLD_GRADE_A,
	CLT_PWORLD_END			= DIFF_PWORLD_GRADE_A + 1,
};

#pragma pack( 1 )
struct tagAutoTeamApplyIDInfo_ToLua
{
	unsigned short LimitFlag;                       	/*   �����Ѷ��������� */
	unsigned short AutoTeamID;                     
};

struct tagAutoTeamApplyInfo_ToLua
{
	short LimitFlag;                       	/*   ְҵ�Ͷӳ��������� */
	short AutoTeamIDNum;                  
	tagAutoTeamApplyIDInfo_ToLua AutoTeamIDInfos[AUTO_TEAM_ID_MAX];
};

struct tagCSAutoTeamPreMemInfo_ToLua
{
	char RoleName[NAME_LEN];             
	unsigned char Gender;                         
	unsigned char Career;                         
	unsigned char Head;                           
	unsigned char Face;                           
	unsigned char Hair;                           
	unsigned char Level;                          
	unsigned char CreatedTeamMem;                  	/*  Ver.307 0 ���ѽ������Ա��1 �ѽ������Ա */
	unsigned char AgreeStat;                       	/*   0 δͬ�⣬1 ��ͬ�� */
	tagAutoTeamApplyInfo_ToLua AutoTeamApplyInfo;  
};
#pragma pack()

struct tagAutoTeamPreMem
{
	//bool	m_isAnyInTeam;
	std::vector<tagCSAutoTeamPreMemInfo_ToLua>	m_preMemInfo;
};

enum
{
	CLIENT_ENTER_PWORLD_SAFE_STATUSID = ENTER_PWORLD_SAFE_STATUSID,
};

//tolua_end

class AutoTeamManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<AutoTeamManager>
{//tolua_export
public:
	AutoTeamManager();
	void releaseAutoTeamInfo();
	void ValidateMemberName( char* szRoleName, int nMaxNameLen );
	const char* getValidateMemberName( const char* pszRoleName );
	//tolua_begin
	
	// ȡ������ԭ��ѡ��ĸ���
	void cancelAllSelectPWorld();
	// �����ѡ��ĸ���
	void addSelectPWorld( int nAutoTeamID, int nSimpleFlg, int nNormalFlg, int nHardFlg, int nProfessionalFlag );
	// ��Ҫ��Ķ����������磺ְҵҪ���Ƿ��뵱�ӳ���
	
	// ���������������
	short getAllCareerAndCapRequest();
	// ȡ������ְҵ�Ͷӳ�Ҫ��
	void cancelAllCareerAndCapRequest();
	// ���ְҵ�Ͷӳ�Ҫ��
	void addCareerAndCapRequest( int nCarrer, bool bAdd );

	// ����ϵͳƥ���Զ���ӣ���ͨ����� Ѱ����� ��ť����Ĳ���
	void requestAutoTeam();
	// �ܾ���ɴ���
	void disagreeCreate();
	// ͬ����ɴ���
	void agreeCreate();

	// �ܾ��Զ����
	void disagreeAutoTeam();
	// ͬ���Զ����
	int getMainPlayerAgreeState();
	bool canAgreeAutoTeam();
	void agreeAutoTeam();

	// ������ɴ�������
	void requestFinishCreate();
	
	bool canTranToPWorld();
	// ���ͽ��Զ������ѡ��ĸ���
	void tranToPWorld();
	// ȡ�����ͽ�����
	void cancelTranToPWorld();
	
	// ��ȡ�Զ�����е�����
	int getAutoTeamMemberNum();
	// ��ѯ�Զ�����еĳ�Ա
	tagCSAutoTeamPreMemInfo_ToLua* getAutoTeamMember( int nIndex );
	// ��ȡԤ��������ĳ�Ա����
	int getPreMemberNum();
	// ��ѯԤ��������ĳ�Ա
	tagCSAutoTeamPreMemInfo_ToLua* getPreMemberByIndex( int nIndex );
	// ��ѯԤ��������ĳ�Ա
	tagCSAutoTeamPreMemInfo_ToLua* getPreMember( const char* pszName );
	// ����Զ����ID 
	int getAutoTeamID();
	// �Ƿ��Լ��൱�ӳ�
	bool isSelfWantToBeCaptain();
	// Ԥ�����������Ƿ����˼������
	//bool isPreMemberInTeam();
	// ��ȡ�����Ѷ�
	int getPworldDiffulty()
	{
		return m_nPworldDiffulty;
	}

	// ���������ȫ״̬
	void cleanSafeStatus( int nStatusID = ENTER_PWORLD_SAFE_STATUSID );
	//tolua_end
	
	// ��������Ԥ��������ĳ�Ա
	void incrUpdatePreMember( const CSAUTOTEAMPREADD& addMemberInfo );
	// ȫ������Ԥ��������ĳ�Ա
	void totalUpdatePreMember( const CSAUTOTEAMPREADD& addMemberInfo );
	// ����¼���Ԥ��������ĳ�Ա
	void addPreMember( const CSAUTOTEAMPREMEMINFO& newPreMemberInfo );
	// ���ټ���Ԥ��������ĳ�Ա
	void delPreMember( const char* pszName );
	// �ı�Ԥ���������Ա��׼��״̬
	void changePreMemberState( const char* pszName, unsigned char nState );
	// �����Զ���Ӷ����еĳ�Ա��Ϣ
	void updateAutoTeamMember( const CSAUTOTEAMSUCC& allMemberInfo );

	virtual int  HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

private:
	void	OnAutoTeamSucc( const CSAUTOTEAMSUCC& autoTeamSuccData );

private:
	int						m_nAutoTeamState;
	int						m_nAutoTeamID;
	AUTOTEAMAPPLYINFO		m_selfAutoTeamApplyInfo;	// �Լ��������������߸�����Ϣ
	tagAutoTeamPreMem		m_preMemberInfo;
	int						m_nPworldDiffulty;			// �Ѷȼ���
};//tolua_export