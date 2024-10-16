#pragma once
#include "GameActor.h"
#include "OgreRect.h"
#include "GameDiathesis.h"
#include "GameMaskMap.h"
#include "proto_cs.h"
#include "SimpleBind.h"

class IAutoFind;

//tolua_begin
enum
{
	MOVE_TIME_INTERVAL_SMALL,	// 2��Ѱ·ʱ����̫��
	MOVE_SPEED_ZERO,			// �ƶ��ٶ�Ϊ0
	MOVE_STATUS_FORBID,			// ��ǰ״̬�������ƶ�
	MOVE_TYPE_NOT_NORMAL,		// ���ܽ�����ͨ�ƶ�
	MOVE_STATE_CANNOT_MOVE,		// ���ܴӵ�ǰ״̬��Ϊ�ƶ�״̬
	MOVE_IN_TETANIC,			// ���ڽ�ֱ״̬
	MOVE_POINT_CANNOT_WALK,		// Ŀ��㲻�ɴ�
	MOVE_ALREADY_IN_DEST,		// Ŀ��㼴Ϊ���ǵ�ǰ���ڵ�
	MOVE_DIST_INTERVAL_SMALL,	// Ŀ�������ǵ�ǰλ�þ���̫��
	MOVE_SUCCESS,				// Ѱ·�ɹ�
};

enum
{
	CLIENT_CAREER_NON					= CAREER_NON,					/* 8λ�洢,��4λ��ʾתְְҵ,��4λ��ʾ����ְҵ */
	CLIENT_CAREER_WARRIOR				= CAREER_WARRIOR,				/* תְǰսʿ */
	CLIENT_CAREER_MAGICOR				= CAREER_MAGICOR,				/* תְǰ��ʦ */
	CLIENT_CAREER_GUNER					= CAREER_GUNER,					/* תְǰǹ�� */
	CLIENT_CAREER_CHURCH				= CAREER_CHURCH, 				/* תְǰ��ʦ */

	CLIENT_CAREER_WARRIOR_SINGLEHAND	= CAREER_WARRIOR_SINGLEHAND, 	/* �ػ��� */
	CLIENT_CAREER_WARRIOR_DOUBLEHAND	= CAREER_WARRIOR_DOUBLEHAND, 	/* ������ */

	CLIENT_CAREER_MAGICOR_SINGLEHAND	= CAREER_MAGICOR_SINGLEHAND, 	/* ������ */
	CLIENT_CAREER_MAGICOR_DOUBLEHAND	= CAREER_MAGICOR_DOUBLEHAND, 	/* ����ʦ */

	CLIENT_CAREER_GUNER_SINGLEHAND		= CAREER_GUNER_SINGLEHAND, 		/* ��ɱ�� */
	CLIENT_CAREER_GUNER_DOUBLEHAND		= CAREER_GUNER_DOUBLEHAND,	 	/* ������ */

	CLIENT_CAREER_CHURCH_SINGLEHAND		= CAREER_CHURCH_SINGLEHAND, 	/* ʥ���� */
	CLIENT_CAREER_CHURCH_DOUBLEHAND		= CAREER_CHURCH_DOUBLEHAND, 	/* Ԧ��ʦ */
};

enum
{
	CLIENT_CAREERCHG_LEVEL	= CAREERCHG_LEVEL,
	CLIENT_CAREERCHG_ITEMID = CAREERCHG_ITEMID,
};

struct tagConvoyTask
{
	tagConvoyTask()
	{
		m_bHaveConvoy	= false;
		m_nMonsterID	= 0;
		m_nMonsterResID	= 0;
		m_nMonsterPosX	= 0;
		m_nMosnterPoxY	= 0;
		m_nQuestId		= 0;
	}

	bool	m_bHaveConvoy;
	int		m_nMonsterID;
	int		m_nMonsterResID;
	int		m_nMonsterMapId;
	int		m_nMonsterPosX;
	int		m_nMosnterPoxY;
	int		m_nQuestId;
};

/* �������Ķ���� */
struct tagReviveMoney_ToLua
{
	int MoneyType;                      
	int MoneyVal;                       
};

struct tagReviveDef_ToLua
{
	int Level;                          
	tagReviveMoney_ToLua StandRevive;       /*   ԭ�ظ��� */
	tagReviveMoney_ToLua PerfectRevive;     /*   �������� */
	int ReviveTimes;                     	/*   ԭ�ظ��ʼ�������� */
	int PerfectReviveTimes;              	/*   �������ʼ�������� */
	int IncreaseVal;                     	/*   �����������ֵ */
	int HPMPRate;                        	/*   ԭ�ظ���ָ����ʣ��ٷֱ� */
	int StandIncreaseVal;                	/*  Ver.1635 ԭ�ظ������ֵ */
	int StandIncreaseVal2;               	/*  Ver.1639 ԭ�ظ�������ȱ�ϵ�� */
	int StandIncreaseNum;                	/*  Ver.1639 ԭ�ظ�����ٴ�֮��ʼ�Ŵ���� */
	int PerfectIncreaseVal2;             	/*  Ver.1639 ������������ȱ�ϵ�� */
	int PerfectIncreaseNum;              	/*  Ver.1639 ����������ٴ�֮��ʼ�Ŵ���� */
	int DayReviveTimes;                  	/*  Ver.1677 ÿ�����ԭ�ظ������ */
	int DayPerfectReviveTimes;           	/*  Ver.1677 ÿ���������������� */
	int PerfectFixedNum;                 	/*  Ver.1963  */
	int PerfectFixedMul;                 	/*  Ver.1963  */
};

struct tagLevelExpDef_ToLua
{
	int Level;                          
	int ExpMul[MAX_LEVEL_EXP_LIST];     
};

struct tagLevelBuffExpDef_ToLua
{
	int Level;                          
	int Time;                            	/*   BUFF����ʱ�䣬��λ�� */
	int Exp;                             	/*   ÿ10����һ���ǵľ��� */
	int DayLimit;                        	/*  Ver.1772 ����������֮����Ч */
	int LevelAwardLimit;                 	/*  Ver.1772 ÿ���ȼ�����ȡ��BUFF�Ĵ��� */
};

struct tagOfflineExpParameter_ToLua
{
	int OfflineExpType;                  	/*   ���߾��鲹������ Bind Macrosgroup:OFFLINE_EXP_TYPE,*/
	int Parameter1;                      	/*   ��ʽ��Ӧ����1 */
	float Parameter2;                      	/*   ��ʽ��Ӧ����2 */
};

#define MAX_AOSHAN_BRAND_TOLUA				MAX_AOSHAN_BRAND 	/* ����������� */
#define MAX_LIFT_BRAND_TOLUA							MAX_LIFT_BRAND 	/* ����ɳ���� */

//tolua_end

class MainGamePlayer : public GamePlayer//tolua_export
{//tolua_export
public:
	MainGamePlayer();
	~MainGamePlayer();
	virtual void	OnReachPathTarget();
	// @param: bAutoFind ��ʾ�Ƿ����Զ�Ѱ·
	// �´�Ѱ·���ϴ�Ѱ·ʱ�����Ƿ�̫��
	bool isTimeShort();

	void	setMoveTask(const SimpleFunction<> &task);
	void	clearMoveTask();

	void	SetAutoMove(bool autoMove);
	bool	IsAutoMove();

	void	trySendCacheMove(bool force = false);
	
	virtual int		CmdMoveTo(int x, int z, int dis = 0 );
	virtual int		ContinueMove();
	//tolua_begin
	bool			canWalk( int to_x, int to_y );
	bool			canDelayMove();
	bool			canMove();
	virtual void	CmdMovePause();
	virtual void	CmdMoveStop();
	void			setAFK(bool b);
	//�Զ�����
	bool			isInPilState();			

	void setFace(int f)
	{
		m_Face = f;
		if (this->getAvatar())
		{
			this->getAvatar()->setAvatarAttrib( POS_ID_FACE, m_Face );	
			this->getAvatar()->updateAttrib();
		}
	}
	void setHair(int h)
	{
		m_Hair = h;
		if (this->getAvatar())
		{
			this->getAvatar()->setAvatarAttrib( POS_ID_HAIR, m_Hair + 1);	
			this->getAvatar()->updateAttrib();
		}
	}
	void setHead(int h)
	{
		m_Head = h;
		if (this->getAvatar())
		{
			this->getAvatar()->setAvatarAttrib( POS_ID_HEAD, m_Head );	
			this->getAvatar()->updateAttrib();
		}
	}
	bool isInPVPMap();
	//tolua_end
	virtual void	SetPosition(int x, int z, int height = 0 );
	void			setMoveSeq(int seq)
	{
		m_CurMoveSeq = seq; 
	}
	void			addMoveSeq(int add)
	{
		m_CurMoveSeq += add;
	}
	virtual void	Update(unsigned int dtick);
	virtual void	updateUI( int nChangeValue );
	virtual void	actualUpdateUI();
	GameActor*		getDistanceActor( int dis );
	void			ListenSpellPackage(int skillid, bool flag, int x, int y);// �������ܰ�����
	void			ListenSpellPackage(const CSACTIONRES& msg, GameSpell *pSpell);
	virtual bool	canAutoTalkToNpc();
	void			beginAutoTalkToNpc(int resid);
	virtual void	endAutoTalkToNpc();
	/// ��ֹ��ҿ��ƣ�����ͽ�ֹ�������ʹ�ã�
	void			lockControl();
	/// ������ҿ���
	void			releaseControl();
	/// ��ֹ/����
	void			setCanControl(bool b);
	// ˢ���ϴβ�����ʱ��
	void			RefreshCmdTime();
	virtual void	onAFK(bool b);

	virtual void	onStop(int x, int z, int state = 0 );
	virtual void	onFight(bool bForce = true);
	virtual void	onLeaveFight();

	void SetConvoy( bool bHave, int nQuestId )
	{
		m_convoyInfo.m_bHaveConvoy = bHave;
		m_convoyInfo.m_nQuestId = nQuestId;
		if ( !bHave )
		{
			m_convoyInfo.m_nMonsterResID = 0;
		}
	}

	void SetWorldID( int nWorldID )
	{
		m_nWorldID	= nWorldID;
	}

	void SetOneOfflineExp(int nOneOfflineExp)
	{
		m_OneOffExp = nOneOfflineExp;
	}

	void SetOfflineExp(int OfflineExp)
	{
		m_OfflineExp = OfflineExp;
	}
	
	bool canUsedSpell()
	{
		return m_BufStatus.canUsedSpell();
	}

	bool GetConvoy()
	{
		return m_convoyInfo.m_bHaveConvoy;
	}

	void delOwnMonster( )
	{
		m_convoyInfo.m_nMonsterID = 0;
	}

	void addOwnMonster( int nId, int nResId )
	{
		m_convoyInfo.m_nMonsterID		= nId;
		m_convoyInfo.m_nMonsterResID	= nResId;
	}
	
	//tolua_begin
	tagConvoyTask& getConvoyInfo()
	{
		return m_convoyInfo;
	}

	int GetWorldID()
	{
		return m_nWorldID;
	}

	int	getOwnMonster( )
	{
		return m_convoyInfo.m_nMonsterID;
	}
	
	Container&	getContainer() 
	{
		return m_Container; 
	}
	Equip&		getEquip() 
	{
		return m_Equip; 
	}
	int			getPworldLeaveTime()			
	{ 
		return m_nPworldLeaveTime; 
	}
	int			getPworldStartTime()
	{
		return m_nPworldStartTime;
	}

	int				getOldDayPworldScore()
	{
		return m_nYestodayInfo;
	}
	void			setOldDayPworldScore( int num)
	{
		m_nYestodayInfo = num;
	}

	// ����ͷ����ʾ
	void		setShowHead(bool bShow);
	// ����ʱװ��ʾ
	void		setShowFashion(bool bShow);
	void		setShowBack( bool bShow );
	void		setShowFace( bool bShow );
	// �������·��޹�ϵ��ʾ
	void		setShowLove( bool bShow );
	
	// �ֶ�����
	void			levelUp();
	// ����
	void			revivePlayer( int reviveType, int helperId, int skillId, int nFlag = 0 );
	// �����ɼ�
	virtual void	endCollect();

	virtual void	kill(int id);

	virtual void	cast(int id, int lv);

	virtual bool	hasFashion();
	
	// ȷ��xp�����Ƿ����ͷ�
	int				checkXP( int skillid );
	// �Ƿ��ڱ���״̬
	bool			isOnMorph();
	int				getPathPointNum();
	Ogre::Point2D	getPathPoint( int nIndex );
	int				getCurPosotion( );
	bool			getAutoFind();
	void			setAutoFind( bool bAuto );
	void			setContinueAutoMove();
	
	int getOneOfflineExp()
	{
		return m_OneOffExp;
	}

	int getOfflineExp()
	{
		return m_OfflineExp;
	}
	int getMaxOfflineExp()
	{
		return m_MaxOfflineExp;
	}
	void setMaxOfflineExp( int val )
	{
		m_MaxOfflineExp = val;
	}
	int getPworldExp()
	{
		return m_PworldExp;
	}
	void setPworldExp( int val )
	{
		m_PworldExp = val;
	}
	int getMaxPworldExp()
	{
		return m_MaxPworldExp;
	}
	void setMaxPworldExp( int val )
	{
		m_MaxPworldExp = val;
	}

	inline IAutoFind* getAutoLine()
	{
		return m_pAutoLine;
	}

	int continueLastAutoFind();
	int	ContinueAutoFind();

	DiathesisManager& getDiathesisManager()
	{
		return m_Diathesis;
	}

	void setOverlayRegion( const char* pName, float fSpeed = 0.5f );
	virtual void breakBTypePre();
	// ���������Ϊ
	void breakPreAction();
	// �������������Ϊ
	void breakChannel();
	// ��
	virtual void jump();
	/// �ж�����ܷ����
	bool			canControl();
	void changeCareer( int nCareerType );
	void ChangeCareerToFightLevle( int nCareerType, int nGridIdx );		//���ڱ�������תְ

	const char*		getPkTargetName(){return m_szPkTargetName;}
	bool isInPK();
	void sendLockFace(int type, int value, int skillId);
	virtual void unLockFace();

	void ChangeClanName( const char* pszClanName );
	void ChangeRoleName( const char* pszRoleName );

	void requestRideMonsterMachine( int nMonsterMemId,int nIdx=0 );
	void requestDownMonsterMachine( int nMonsterId );
	void prepareDownMonsterMachine( int nMonsterId );
	void breakPrepareDownMonsterMachine( int nMonsterId );
	void requestMachineKickPlayer( int nId );
	void requestMachinePassenger( int nMachineID );
	void requestAcceptOfflineExp();
	void requestCowRangeAward();
	void requestOfflineExpBuff();
	void RequestOpenBaoXiang( int nListType, int nGridIdx );
	void requestHangHaiRangeAward();							// ��ȡ������ཱ��
	void requestYangHaiBangAdd( int nNum );						// ι������
	void requestYangHaiBangAward();								// ����ȡ��

	void requestClanWarAward();									//��ȡ������ս���˽���

	//���������������
	void requestPaoShangOpt( int nType, int nItemId, int nPrice, int nNum );

	tagReviveDef_ToLua* getReviveDef();
	tagLevelExpDef_ToLua* getLevelExpDef( int nLevel );
	tagLevelBuffExpDef_ToLua* getLevelBuffExpDef();
	tagOfflineExpParameter_ToLua* getOfflineExpParameter( int nType );

	//���鶯��
	void requestSendMotionAction( int nActionId, int nTargetId );
	
	//�Ƿ���GM Ȩ��
	bool isHaveGMPerm(){ return m_nGmPermFlag > 0; }

	//�Ƿ���
	bool isOutSide(){ return m_bIsOutSide; }

	void requestApplyMachineInvite( int id );
	void requestRefuseMachineInvite( int id );
	void requestMachineInvite( int id );
	int  getBattleLimitLeftTime();

	void SelectTarget( int resid, int resType );
	int	 getDailyUsedBull(){ return m_nDailyUsedBull; }
	int isColorFontValid();

	//ĩ����ҫ����
	int getSpartaVictory(){ return m_SpartaVictory; }
	int getSpartaRunAway(){ return m_SpartaRunAway; }
	int getSpartaDefeat(){ return m_SpartaDefeat; }

	int getWeekCharm(){ return m_WeekCharm; }

	int getOfflineDay(){ return m_OffLineDay; }
	void requestOffLineGift();

	int	getNormalMachinePworldClanID(){ return m_nNormalMachinePworldClanID; }	//��ȡ�����ػ���Ŧ-��ͨģʽ�Ĺ���ID
	int	getUnionMachinePworldClanID(){ return m_nUnionMachinePworldClanID; }	//��������ģʽ�����������ID

	int getLoveColorType(){ return m_nLoveColorType; }		//��ȡ��ϵ���ͣ����»��߷��ޣ�
	bool hasLover();										//�Ƿ��з��޻������¹�ϵ
	const char* getLoverName(){ return m_szLoverName; }		//��ȡ���޻����¹�ϵ�У���һ�������

	//tolua_end
	void setGMPermFlag( int nFlag ){ m_nGmPermFlag = nFlag; }
	int getGMPermFlag() { return m_nGmPermFlag; }
	//����ս����ȴ����
	void setBattleLimitData(int nBeginTime, int nMaxNum, int nNum );
	
	void setDailyUsedBull(int bull){ m_nDailyUsedBull = bull; }

	void setColorFontEndTime( int t ){ m_nColorFontEndTime = t; };

	//ĩ����ҫ����
	void setSpartaVictory( int nVictory ){ m_SpartaVictory = nVictory; }
	void setSpartaRunAway( int nRun ){ m_SpartaRunAway = nRun; }
	void setSpartaDefeat( int nDefeat ){ m_SpartaDefeat = nDefeat; }

	void setOfflineDay( int nDay ){ m_OffLineDay = nDay; }

	void resetLoveInfo();

private:
	void UpdateFreedomPet( unsigned int dtick );
public:
	FollowCamera*		m_pCamera;
	int					m_CurMoveSeq;
	unsigned int		m_LastCmdMoveTick;
	int					m_lastCmdMoveX;
	int					m_lastCmdMoveY;
	int					m_lastBenchTimeSec;
	int					m_lastBenchTimeUSec;
	bool				m_lastCached;
	unsigned int		m_moving;
	Container			m_Container;				// ��������

	bool				m_bPworldInfoFlag;			// �Ƿ��и�����Ϣ
	int 				m_nPworldLeaveTime;			// �����뿪ʣ��ʱ��
	int					m_nPworldStartTime;
	int					m_nPworldInstID;			// ����ʵ��ID
	int					m_nCollectMonId;			// ���ڲɼ��Ķ���
	bool				m_bAutoFind;				// �Զ�Ѱ·״̬
	IAutoFind*			m_pAutoLine;
	int					m_nMemID;
	bool				m_bChangeSkillShortCut;		// �Ƿ��л����ܶ�����
	bool				m_bUnaware;					// ���ֲ�������ʾ����������������ս��
	unsigned int		m_lastCmdTime;
	int					m_DriverId;

	char				m_szPkTargetName[ROLE_NAME_LEN];	//�д����

	bool				m_bIsOutSide;				//�Ƿ���
	int					m_nYestodayInfo;		//�����������Ϣ

	int					m_WeekCharm;				//��������ֵ

	int					m_nNormalMachinePworldClanID;		//�����ػ���Ŧ-��ͨģʽ�Ĺ���ID����ʵֻ�����Լ��Ĺ����ID��
	int					m_nUnionMachinePworldClanID;		//�����ػ���Ŧ-����ģʽ�Ĺ���ID

	int					m_nLoveColorType;		//���ܶȹ�ϵ���ͣ�1���£�2����
	char				m_szLoverName[ROLE_NAME_LEN];		//���»���޹�ϵ����һ�������
	tdr_ulonglong		m_loverID;							//��һ���roleid
	
private:
	DiathesisManager	m_Diathesis;
	bool				m_bCanControl;
	bool				m_bAutoTalkNpc;
	int					m_lastAutoTalkNpcId;
	int					m_nLockCount;		
	tagConvoyTask		m_convoyInfo;				// ��ӵ�е��������
	SimpleFunction<>	*m_moveTask;
	bool				m_isAutoMove;
	int					m_nWorldID;
	int					m_nGmPermFlag;				// GM Ȩ��
	//���߾���
	int					m_OneOffExp;				//һ�������߽���
	int					m_OfflineExp;				// ����
	int					m_MaxOfflineExp;			// ���ֵ����
	int					m_PworldExp;				// �����
	int					m_MaxPworldExp;				// �������

	int					m_nBattleBeginTime;         /*   ս�����ڿ�ʼʱ�� */
	int					m_nBattleNum;               /*   ս�������ڽ������ */
	int					m_nBattleMaxNum;            /*   ս����������������� */
	int					m_lastMoveToX;
	int					m_lastMoveToY;
	bool				m_lastMoveToFlag;
	bool				m_mustContinueAutoMove;
	int					m_nDailyUsedBull;			//�������ĵ�bull
	int					m_nColorFontEndTime;

	int					m_SpartaVictory;			//ĩ����ҫ����ʤ������
	int					m_SpartaRunAway;			//ĩ����ҫ�������ܴ���
	int					m_SpartaDefeat;				//ĩ����ҫ����ʧ�ܴ���

	int					m_OffLineDay;				//�����ʧ����
};//tolua_export
