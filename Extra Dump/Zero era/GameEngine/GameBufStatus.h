#pragma once
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameSkill_DefToLua.h"
#include "resdb_meta.h"

enum
{
	STATUS_MISC_TYPE_ENDURE = 0,	// ����
	STATUS_MISC_TYPE_MOTION_BLUR,	// �˶�ģ��
	STATUS_MISC_TYPE_VISION_BLUR,	// �Ӿ�ģ��
	STATUS_MISC_TYPE_STOP_ACTION,	// ����
};

#pragma pack(1)
//tolua_begin
struct tagBindSkill_ToLua
{
	unsigned short SkillID;                         	/*   ����ID */
	unsigned char Level;                           	/*   ���ܵȼ� */
	unsigned char Trig;                            	/*   ������������λ�� */
};

struct tagStatusResult_ToLua
{
	unsigned short ResultID;                        	/*    Bind Macrosgroup:STATUS_RESULT,*/
	int ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;                     
};

struct tagStatusDef_ToLua
{
	unsigned short StatusID;                        	/*   ״̬ID��1000��3000 */
	unsigned short Level;                           	/*   ״̬�ȼ� */
	char Name[RES_NAME_LEN];             
	int IconID;                          	/*  Ver.6 ͼ��id */
	int ModeID;                          	/*  Ver.9 ģ��id */
	int EffectID;                        	/*  Ver.52 ��Чid */
	int BindPoint;                       	/*  Ver.26 ״̬��Ч��� */
	unsigned char Type;                            	/*    Bind Macrosgroup:STATUS_TYPE,*/
	unsigned char SubType;                        
	unsigned char DeadClear;                       	/*   �Ƿ�������� */
	unsigned char OverlayLimit;                    	/*   �������� */
	tagBindSkill_ToLua BindSkill;                      
	tagStatusResult_ToLua Result[MAX_STATUS_RESULT];      
	char StatusDesc[DESC_LEN];           
	int EffectType;                      	/*  Ver.75 Ч������ */
	unsigned char ZaRate;                          	/*  Ver.40 �ӱ����� */
	unsigned char NormRate;                        	/*  Ver.40 ��ͨ������� */
	unsigned char JinYinRate;                      	/*  Ver.40 ��Ӣ������� */
	unsigned char BossRate;                        	/*  Ver.40 Boss���� */
	unsigned char PlayerRate;                      	/*  Ver.40 ��Ҹ��� */
	unsigned char ConsumeStatusType;               	/*  Ver.49 ������Դ���� Bind Macrosgroup:CONSUME_STATUS_TYPE,*/
	unsigned char ConsumeStatusValue;              	/*  Ver.49 ������Դֵ */
	unsigned char IsWushuang;                      	/*  Ver.94 �Ƿ���˫״̬ */
	unsigned char CanCancel;                       	/*  Ver.99 ���״̬�Ƿ���Ա�����Լ�ȡ�� */
	unsigned short StatusFlag;                      	/*  Ver.124 ��λ��Ĺ��� ,״̬�����־ 1 ������� 2 ��������� */
	unsigned short OverlayType;                     	/*  Ver.144 �������� */
	char IsCltShow;                       	/*  Ver.175 �ͻ����Ƿ���ʾ */
	int OutFightKeep;                    	/*  Ver.188 ����ս�������Ƿ񱣳ֲ���� */
	int AddModelID;                      	/*  Ver.283 �����ģ��id */
	unsigned char MachineRate;                     	/*  Ver.523 ��е���� */
	unsigned int CltMiscFlag;                     	/*  Ver.604 �ͻ�������õ�λ */
	unsigned char StatusInterVal;                  	/*  Ver.616 ״̬�������λ�룬0��ȡĬ��ֵ3 */
	unsigned char ByAtkClear;                      	/*  Ver.2333 ���������ٴ�״̬��ʧ0-������ʧ */
};

struct tagStatusOverlayDef_ToLua
{
	unsigned short StatusID;                        	/*   ״̬ID��1000��3000 */
	unsigned char Level;                           	/*   ״̬�ȼ� */
	unsigned char Overlay;                         	/*   ������ */
	short Mul;                             	/*   ����ָ�� */
};

enum
{
	CLIENT_DIE_REMIT_BUFFID = DIE_REMIT_BUFFID,
	CLIENT_VIP_BAR_STATUS_ID = VIP_BAR_STATUS_ID,
	CLIENT_VIP_BAR_STATUS_LEVEL = VIP_BAR_STATUS_LEVEL,
};

extern const tagStatusDef_ToLua* getStatusDef(int id, int lv);
extern const tagStatusOverlayDef_ToLua* getStatusOverlayDef(int id, int lv, int overlay );
bool isBuffCanCancel( int id, int lv );
bool canShowBuff( int id, int lv );
bool isStatusShowAsDebuff( int id, int lv );   // ��ʾ��debuff��
bool isCltShowBuff( int id, int lv );
//tolua_end
#pragma pack()

//tolua_begin
enum {
	FOR_EVERY = MAX_STATUS_LAST
};
//tolua_end

class GameBufStatus;
class GameBuf//tolua_export
{//tolua_export
public:
	GameBuf(void);
	~GameBuf(void);

	void begin( const CSROLESTATUS& StateInfo, unsigned int startTime, int ownerId, int index );
	void reset(const CSROLESTATUS& StateInfo, unsigned int startTime, int index);
	void reset(const CSROLESTATUS& StateInfo, int index);
	void end();
	void clear();
	
	int  update( unsigned int dtick );

	void buf_begin_actorStop();
	void buf_end_actorStop();
	void buf_begin_eff( int id );
	void buf_end_eff();
	void buf_reload_eff(int id);
	void buf_clear_eff();
	void buf_eff( int id, int state = 0 );

	//tolua_begin
	int		getIndex()
	{
		return m_index;
	}
	int		getIconId();
	unsigned int getBufId() 
	{
		return m_StateInfo.StatusID;
	}
	int		getBufLv() 
	{ 
		return m_StateInfo.Level; 
	}
	double	getLife();
	bool	isShow();

	int		getOverlay()  
	{ 
		return m_StateInfo.Overlay; 
	}
	unsigned int getTotalLife()  
	{ 
		return m_nTotalLife; 
	}

	// �Ƿ��жϼ���
	bool	isSkillStop( int skillId );
	bool	isSkillStop();
	//// �Ƿ�������
	bool	isMoveStop();
	// �Ƿ����ĳ��result
	bool	isHasResult(unsigned short result);
	// �Ƿ����
	bool	isEndure();

	//tolua_end
public:
	GameBufStatus*	 m_pBufStatus;

	unsigned int	m_nStartTime;
	CSROLESTATUS	m_StateInfo;	
	bool			m_bDead;
	bool			m_bShow;		// �Ƿ���ʾ
	int				m_ownerId;
	unsigned int	m_nTotalLife;

	unsigned int	m_index;
};//tolua_export

class GameBufStatus//tolua_export
{//tolua_export
public:
	GameBufStatus(void);
	~GameBufStatus(void);
	
	void setOwnerId( int Id ); 

	void setGoodBufStatus( int nNum, const CSROLESTATUS* pGoodStatusList );
	void setBufStatus(const CSSTATUSINFO& status, unsigned int idx = getBufStatusIdx());
	void setBufStatus(const CSSTATUSINFO& status, unsigned int startTime, unsigned int idx);
	void setNextSpeed( const CSONEMSPD& nextspeed );
	
	void update( unsigned int dtick );
	void updateSpeed( unsigned int dtick );

	void updateUI();
	
	void reLoadAllEff();
	void clearAllEff();
	void petClearBuff();

	static unsigned int getBufStatusIdx();

	//tolua_begin
	// ȡ�����ǵ�ĳ��buffer״̬
	void cancelBuffer( int nBufferID, int nBuffLevel );
	int	getGoodStatusCount()		
	{ 
		return m_nGoodStatusNum; 
	}
	int	getBadStatusCount()		
	{ 
		return m_nBadStatusNum; 
	}
	int	getNolimitStatusCount()	
	{ 
		return m_nNoLimitStatusNum; 
	}

	int getJiaStatusCount()	
	{
		return (int)m_JiaStatusList.size();
	}

	GameBuf* getGoodStatus( int index );
	GameBuf* getBadStatus( int index );
	GameBuf* getNolimitStatus( int index );
	GameBuf* getJiaStatus( int index );

	// �Ƿ����ʹ�ü���
	bool canUsedSpell( int nSkillId );
	bool canUsedSpell();
	// �Ƿ��ڰ���״̬
	bool isInEndure();
	// �Ƿ��ڲ��ɹ���״̬
	bool isInNoSkill();
	// �Ƿ�����ƶ�
	bool canMove();
	// �Ƿ����Buf
	bool checkBuf( int nBufId );
	// ��ѯĳ��buf��״̬
	GameBuf* getBufStatus( int nBufId, int nLv );
	// ��ѯ����ĳ��result��buff
	GameBuf* getBufResult(unsigned short result);
	// ��ѯĳ��״̬��Ч��
	static const tagStatusDef& getStatusDef(int id, int lv);
	// �ж�ĳ��״̬�Ƿ��ֹʩ�ż���
	static bool StatusNoSkill(int id, int lv);
	void addJiaStatus( int nID, int nLv );
	void delJiaStatus( int nID, int nLv );
	//tolua_end
public:
	std::list<int>	m_effidlist;
private:
	bool					m_bNextSpeed;
	CSONEMSPD				m_nextSpeed;	
	int						m_nGoodStatusNum;                  
	int						m_nBadStatusNum;                   
	int						m_nNoLimitStatusNum;               
	GameBuf					m_GoodStatusList[MAX_GOOD_STATUS];		// ��Ϊ����״̬
	GameBuf					m_BadStatusList[MAX_BAD_STATUS];		// ��Ϊ����״̬
	GameBuf					m_NolimitStatusList[MAX_NOLIMIT_STATUS];//	
	int						m_ownerId;
	unsigned int			m_lastSetIdx;
	std::vector<GameBuf>	m_JiaStatusList;
};//tolua_export
