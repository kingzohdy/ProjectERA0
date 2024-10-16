#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include "GameMsgPrint.h"
#include "ClientInterface.h"
#include <map>
#include <vector>

//tolua_begin
enum  
{
	CLIENT_RELATION_TYPE_NONE = 0, 
	CLIENT_RELATION_TYPE_FRIEND = 1,		// ����
	CLIENT_RELATION_TYPE_MASTER = 2,		// ʦ��
	CLIENT_RELATION_TYPE_PRENTICE = 4,		// ͽ��
	CLIENT_RELATION_TYPE_SPOUSE = 8,		// ����
	CLIENT_RELATION_TYPE_BROTHER = 16,		// ���
	CLIENT_RELATION_TYPE_SU_DI = 32,	// ׷ɱ
	CLIENT_RELATION_TYPE_BLACK = 64,		// ������
	CLIENT_RELATION_TYPE_TMPFRIEND = 128,	// ��ʱ
	CLIENT_RELATION_TYPE_XIONG_SHOU = 256,		// ����	
	CLIENT_RELATION_TYPE_NEWCOMER = 512	//����
};

enum  
{
	CLIENT_OFFLINE	= 0,		// ����
	CLIENT_ONLINE	= 1,		// ����
	MUD_ONLINE		= 2,
};

enum tagINTIMATE_COLOR_TYPE_TOLUA
{
	INTIMATE_COLOR_FRIEND_TOLUA = INTIMATE_COLOR_FRIEND, 	/* ��ͨ���ѹ�ϵ */
	INTIMATE_COLOR_LOVERS_TOLUA = INTIMATE_COLOR_LOVERS, 	/* ���¹�ϵ���ۺ�ɫ */
	INTIMATE_COLOR_SPOUSE_TOLUA = INTIMATE_COLOR_SPOUSE, 	/* ���޹�ϵ���Ϻ�ɫ */
};

// ������Ϣ
struct tagFriendInfo 
{
	tagLong_ToLua	m_RoleID;
	int				m_nRelationType;
	char			m_szName[NAME_LEN];			/* RELATION_ATTR_NAME,   */
	int				m_nGender;					/* RELATION_ATTR_GENDER,   */
	int				m_nLevel;					/* RELATION_ATTR_LEVEL,   */
	int				m_nBuddyVal;				/* RELATION_ATTR_BUDDYVAL,   */		
	int				m_nHead;					/* RELATION_ATTR_HEAD,   */
	int				m_nHair;
	int				m_nFace;
	int				m_nCareer;					/* RELATION_ATTR_CAREER,   */
	int				m_nIsOnline;				/* RELATION_ATTR_ONLINE,  0������ 1���� */
	char			m_szEmotion[EMOTION_LEN];	/* RELATION_ATTR_EMOTION,   */
	char			m_szMemName[NAME_LEN];		/* RELATION_ATTR_MEMNAME,   */
	char			m_szClanName[NAME_LEN];
	unsigned int	m_nClanID;
	bool			m_bOutSide;				//�Ƿ��Ǳ�ķ�������
	int				m_nGprsEnd;				// RELATION_ATTR_GPRSEND
	int				m_nGroupID;
	int				m_nIntimateColorType;		//���ܶȹ�ϵ��0��ͨ���ѣ�1���¹�ϵ��2���޹�ϵ
};

struct tagCSBuddyInfoRes_ToLua
{
	int Level;                          
	int Career;                         
	int Head;                           
	int Hair;                           
	int Face;                           
	int Gender;                         
	char RoleName[NAME_LEN];             
	char ClanName[NAME_LEN];
	tdr_ulonglong ClanWid;
	RESPOS Pos;
	int Map;
	int FriendVal;
	unsigned char TeamRoleNum;
	char Emotion[EMOTION_LEN];
	int Span;                            	/*  Ver.1776  */
	unsigned int Charm;                     /*  Ver.2055  */
};

int getServeSecondTime();
//�Ժ����б��������
bool otherFriendCompare(const tagFriendInfo& lhs,const tagFriendInfo& rhs );
//tolua_end

bool operator< ( const tagFriendInfo& lhs, const tagFriendInfo& rhs );
bool operator==( const tagFriendInfo& lhs, const tagFriendInfo& rhs );
bool operator== ( const tagFriendInfo& friendInfo, const char* pszName );
bool operator== ( const tagFriendInfo& friendInfo, const tagLong_ToLua& roleID );
bool operator== ( const tagFriendInfo& friendInfo, const tdr_ulonglong roleWID );


typedef std::vector<tagFriendInfo>				FRI_VECTOR;
typedef std::vector<tagFriendInfo>::iterator	FRI_VECTOR_ITER;
typedef std::map<int, FRI_VECTOR>				FRI_MAP;
typedef std::map<int, FRI_VECTOR>::iterator		FRI_MAP_ITER;

class GameFriendManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameFriendManager>, public IGameFriendManager
{//tolua_export
public:
	GameFriendManager(void);
	~GameFriendManager(void);
	virtual void objEnterView(GameActor *actor);
	
	// c->s
public:
	//tolua_begin
	
	void reSetFriendList( const char* pszCallStack );
	
	// ׷��ĳ��
	void track( int nFlag, const char* pszName );
	// ���ú����б�
	bool canBeFriend( const char* pszRoleName );
	// �Ӻ���(ע�⣺�� ����Ҫ�޶����ֺ͸�����Ϣ��С)
	void addFriend( const char* szRoleName, const char* szReqInfo );
	// ����ĳ����Ϣ
	void requirePlayerInfo( const char* szRoleName, int nType );
	// �����ʱ����
	void addTmpFriend( const char* szName );
	// �����ʱ����
	void addTmpEnemy( const char* szName );
	// �Ӻ�����
	void addBlack( const char* szRoleName );
	// ��׷ɱ����
	void addEnem( const char* szRoleName, int nRoleHightId, int nRoleLowId );
	// ���Խ��ܺ�������
	bool canAcceptBeFriend( const char* pszRoleName );
	// ���������Ϣ�ظ�
	// ˵����Accept=0 ��ʾ�ܾ�, AddOther=0 ��ʾ����ӶԷ�Ϊ����,�ͻ����յ�RELATION_OP_TYPE_CHG��Ϣ��,��Ӻ���	AddOther=1��ʾ���Ա��˼Ӻ��ѵ�����
	void addFriendRes( const char* szRoleName, int nAccept, int nAddOther, bool bTimeOut = false );
	// �޸�����ǩ��
	void changeEmotion( const char* szEmotion );
	// ���ú��ѷ���
	void setFriendGroup( const char* szRoleName, int nGroupID );
	// ��ȡ��������()
	int getMaxFriendNum();
	// �޸ı�ע����
	// ע��:����˲��ظ����ͻ���ֱ�Ӹ�
	void changeMemName( const char* szRoleName, const char* szMemName );
	// ɾ����ϵ����
	// nType��ӦCLIENT_RELATION_TYPE_FRIEND...
	void delRelation( int nType, int nRoleHightId, int nRoleLowId );
	// �鿴���װ��
	void seeAvatarReq( const char* szName );
	void sortFriendInfo( int nRelationType );

	void RequestWatchFriend( const char* szName );

	//����ұ�׵Ļ�Ӧ
	void ActToPursue( const char* szPursueName, int nFlag );
	//�Խ�����ԵĻ�Ӧ
	void ActToMarryOath( const char* szName, int nFlag );
	//�Խ����ϵ���ѵĻ�Ӧ
	void ActToRemoveRelationCost( const char* szName);
	//tolua_end

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	void	resetFriendList();
	//tolua_begin
	// ɾ�����
	void	deleteRole( int nRelationType, tdr_ulonglong roleWID );
	void	deleteRole( tdr_ulonglong roleWID );
	void	deleteRole( int nRelationType, const char* pszName );
	
	// ����������ȡ������Ϣ
	tagFriendInfo* getFriendInfoByName( int nRelationType, const char* pszName );
	tagFriendInfo* getFriendInfoByName( const char* pszName );
	// ���ݺ���Id��ȡ������Ϣ
	tagFriendInfo* getFriendInfoByRoleId( int nRoleHightId, int nRoleLowId );
	tagFriendInfo* getFriendInfoByRoleId( tdr_ulonglong roleWID );
	// ��ȡ��ϵ�������
	int getRelationPlayerCount( int nType );
	// ����index��ȡ������Ϣ
	tagFriendInfo* getFriendInfoByIndex( int nType, int nIndex );
	// ��ȡ���ǩ��
	const char* getEmotion() const;
	
	// ��ѯ��ҹ�ϵ
	bool	isPlayerRelation( int nRelationType, const char* pszName );
	// ��ѯ��ҹ�ϵ
	bool	isPlayerRelation( int nRelationType, int nHeightId, int nLowId );
	// ��ѯ������Ƿ���ָ����ϵ
	int		getPlayerRelation( const char* pszName );
	// ��ѯ��ǰ��������
	tagCSBuddyInfoRes_ToLua&	getCurRequireRoleInfo();
	//tolua_end

	void	onLeavePWorld( int nMapid );
	
	//�ж��Ƿ���ս����������
	bool	isInBattleBlack(const char* szName);
	void changeFriendName( const char* pszOldName, const char* pszNewName );
	// ���¹�����Ϣ
	void	updateClanInfo( tdr_ulonglong roleWID, const RELATIONCLAN& clanInfo );
	//��ѯ��ҵ�����״̬
	int		getPlayerOnlineTypeByName( std::string szName);
	void	setOnlinePlayer(std::string szName,int OnlineType);
	std::map<std::string,int>		m_OnlinePlayer;	
private:
	// ���ö�������
	void setRoleAttr( tagFriendInfo& info, int nType, const CSRELATIONATTRDATA* pdata );
	// ����������֪ͨ
	void roleOnLineInfo( int nOnline, const char* pszName );

	void OnRelationTypeChange( const CSRELATIONCHG& relationChgInfo );
	void OnRelationTypeListChange( const CSRELATIONCHGLIST& relationChgList );
	void UpdateScenePlayer( const char* pszName );

	//�����Ϣ����
	void HandleMarryInfo( const CSINVOLVEMARRYSVR& InvolveMarrySvr );
		
private:
	FRI_MAP		m_FriendMap;
	char		m_szEmotion[EMOTION_LEN];
	tagLong_ToLua m_increaseWid;
	tagCSBuddyInfoRes_ToLua	m_CurRequireRoleInfo;	
	int			m_nMaxFriendNum;
};//tolua_export
