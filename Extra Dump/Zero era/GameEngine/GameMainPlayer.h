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
	MOVE_TIME_INTERVAL_SMALL,	// 2次寻路时间间隔太短
	MOVE_SPEED_ZERO,			// 移动速度为0
	MOVE_STATUS_FORBID,			// 当前状态不允许移动
	MOVE_TYPE_NOT_NORMAL,		// 不能进行普通移动
	MOVE_STATE_CANNOT_MOVE,		// 不能从当前状态变为移动状态
	MOVE_IN_TETANIC,			// 处于僵直状态
	MOVE_POINT_CANNOT_WALK,		// 目标点不可达
	MOVE_ALREADY_IN_DEST,		// 目标点即为主角当前所在点
	MOVE_DIST_INTERVAL_SMALL,	// 目标点和主角当前位置距离太近
	MOVE_SUCCESS,				// 寻路成功
};

enum
{
	CLIENT_CAREER_NON					= CAREER_NON,					/* 8位存储,高4位表示转职职业,低4位表示现有职业 */
	CLIENT_CAREER_WARRIOR				= CAREER_WARRIOR,				/* 转职前战士 */
	CLIENT_CAREER_MAGICOR				= CAREER_MAGICOR,				/* 转职前法师 */
	CLIENT_CAREER_GUNER					= CAREER_GUNER,					/* 转职前枪手 */
	CLIENT_CAREER_CHURCH				= CAREER_CHURCH, 				/* 转职前牧师 */

	CLIENT_CAREER_WARRIOR_SINGLEHAND	= CAREER_WARRIOR_SINGLEHAND, 	/* 守护者 */
	CLIENT_CAREER_WARRIOR_DOUBLEHAND	= CAREER_WARRIOR_DOUBLEHAND, 	/* 征服者 */

	CLIENT_CAREER_MAGICOR_SINGLEHAND	= CAREER_MAGICOR_SINGLEHAND, 	/* 聚能者 */
	CLIENT_CAREER_MAGICOR_DOUBLEHAND	= CAREER_MAGICOR_DOUBLEHAND, 	/* 炼魂师 */

	CLIENT_CAREER_GUNER_SINGLEHAND		= CAREER_GUNER_SINGLEHAND, 		/* 猎杀者 */
	CLIENT_CAREER_GUNER_DOUBLEHAND		= CAREER_GUNER_DOUBLEHAND,	 	/* 毁灭者 */

	CLIENT_CAREER_CHURCH_SINGLEHAND		= CAREER_CHURCH_SINGLEHAND, 	/* 圣佑者 */
	CLIENT_CAREER_CHURCH_DOUBLEHAND		= CAREER_CHURCH_DOUBLEHAND, 	/* 驭雷师 */
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

/* 复活消耗定义表 */
struct tagReviveMoney_ToLua
{
	int MoneyType;                      
	int MoneyVal;                       
};

struct tagReviveDef_ToLua
{
	int Level;                          
	tagReviveMoney_ToLua StandRevive;       /*   原地复活 */
	tagReviveMoney_ToLua PerfectRevive;     /*   完美复活 */
	int ReviveTimes;                     	/*   原地复活开始递增次数 */
	int PerfectReviveTimes;              	/*   完美复活开始递增次数 */
	int IncreaseVal;                     	/*   完美复活递增值 */
	int HPMPRate;                        	/*   原地复活恢复比率，百分比 */
	int StandIncreaseVal;                	/*  Ver.1635 原地复活递增值 */
	int StandIncreaseVal2;               	/*  Ver.1639 原地复活递增等比系数 */
	int StandIncreaseNum;                	/*  Ver.1639 原地复活多少次之后开始放大递增 */
	int PerfectIncreaseVal2;             	/*  Ver.1639 完美复活递增等比系数 */
	int PerfectIncreaseNum;              	/*  Ver.1639 完美复活多少次之后开始放大递增 */
	int DayReviveTimes;                  	/*  Ver.1677 每日免费原地复活次数 */
	int DayPerfectReviveTimes;           	/*  Ver.1677 每日免费完美复活次数 */
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
	int Time;                            	/*   BUFF持续时间，单位秒 */
	int Exp;                             	/*   每10秒跳一次涨的经验 */
	int DayLimit;                        	/*  Ver.1772 开服多少天之后生效 */
	int LevelAwardLimit;                 	/*  Ver.1772 每个等级可领取次BUFF的次数 */
};

struct tagOfflineExpParameter_ToLua
{
	int OfflineExpType;                  	/*   离线经验补偿类型 Bind Macrosgroup:OFFLINE_EXP_TYPE,*/
	int Parameter1;                      	/*   公式对应参数1 */
	float Parameter2;                      	/*   公式对应参数2 */
};

#define MAX_AOSHAN_BRAND_TOLUA				MAX_AOSHAN_BRAND 	/* 最大绿洲牌子 */
#define MAX_LIFT_BRAND_TOLUA							MAX_LIFT_BRAND 	/* 最大狂沙牌子 */

//tolua_end

class MainGamePlayer : public GamePlayer//tolua_export
{//tolua_export
public:
	MainGamePlayer();
	~MainGamePlayer();
	virtual void	OnReachPathTarget();
	// @param: bAutoFind 表示是否发起自动寻路
	// 下次寻路和上次寻路时间间隔是否太短
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
	//自动辅助
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
	void			ListenSpellPackage(int skillid, bool flag, int x, int y);// 监听技能包数据
	void			ListenSpellPackage(const CSACTIONRES& msg, GameSpell *pSpell);
	virtual bool	canAutoTalkToNpc();
	void			beginAutoTalkToNpc(int resid);
	virtual void	endAutoTalkToNpc();
	/// 禁止玩家控制（允许和禁止必须配对使用）
	void			lockControl();
	/// 允许玩家控制
	void			releaseControl();
	/// 禁止/允许
	void			setCanControl(bool b);
	// 刷新上次操作的时间
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

	// 设置头盔显示
	void		setShowHead(bool bShow);
	// 设置时装显示
	void		setShowFashion(bool bShow);
	void		setShowBack( bool bShow );
	void		setShowFace( bool bShow );
	// 设置情侣夫妻关系显示
	void		setShowLove( bool bShow );
	
	// 手动升级
	void			levelUp();
	// 复活
	void			revivePlayer( int reviveType, int helperId, int skillId, int nFlag = 0 );
	// 结束采集
	virtual void	endCollect();

	virtual void	kill(int id);

	virtual void	cast(int id, int lv);

	virtual bool	hasFashion();
	
	// 确认xp技能是否能释放
	int				checkXP( int skillid );
	// 是否处于变身状态
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
	// 打断所有行为
	void breakPreAction();
	// 打断所有吟唱行为
	void breakChannel();
	// 跳
	virtual void jump();
	/// 判断玩家能否控制
	bool			canControl();
	void changeCareer( int nCareerType );
	void ChangeCareerToFightLevle( int nCareerType, int nGridIdx );		//用于比赛服的转职

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
	void requestHangHaiRangeAward();							// 获取航海大亨奖励
	void requestYangHaiBangAdd( int nNum );						// 喂养海蚌
	void requestYangHaiBangAward();								// 海蚌取珠

	void requestClanWarAward();									//领取公会跨服战个人奖励

	//跑商相关买卖操作
	void requestPaoShangOpt( int nType, int nItemId, int nPrice, int nNum );

	tagReviveDef_ToLua* getReviveDef();
	tagLevelExpDef_ToLua* getLevelExpDef( int nLevel );
	tagLevelBuffExpDef_ToLua* getLevelBuffExpDef();
	tagOfflineExpParameter_ToLua* getOfflineExpParameter( int nType );

	//表情动作
	void requestSendMotionAction( int nActionId, int nTargetId );
	
	//是否有GM 权限
	bool isHaveGMPerm(){ return m_nGmPermFlag > 0; }

	//是否跨服
	bool isOutSide(){ return m_bIsOutSide; }

	void requestApplyMachineInvite( int id );
	void requestRefuseMachineInvite( int id );
	void requestMachineInvite( int id );
	int  getBattleLimitLeftTime();

	void SelectTarget( int resid, int resType );
	int	 getDailyUsedBull(){ return m_nDailyUsedBull; }
	int isColorFontValid();

	//末世荣耀殿堂
	int getSpartaVictory(){ return m_SpartaVictory; }
	int getSpartaRunAway(){ return m_SpartaRunAway; }
	int getSpartaDefeat(){ return m_SpartaDefeat; }

	int getWeekCharm(){ return m_WeekCharm; }

	int getOfflineDay(){ return m_OffLineDay; }
	void requestOffLineGift();

	int	getNormalMachinePworldClanID(){ return m_nNormalMachinePworldClanID; }	//获取开启守护枢纽-普通模式的公会ID
	int	getUnionMachinePworldClanID(){ return m_nUnionMachinePworldClanID; }	//开启联合模式的其它公会的ID

	int getLoveColorType(){ return m_nLoveColorType; }		//获取关系类型（情侣或者夫妻）
	bool hasLover();										//是否有夫妻或者情侣关系
	const char* getLoverName(){ return m_szLoverName; }		//获取夫妻或情侣关系中，另一半的名字

	//tolua_end
	void setGMPermFlag( int nFlag ){ m_nGmPermFlag = nFlag; }
	int getGMPermFlag() { return m_nGmPermFlag; }
	//设置战场冷却数据
	void setBattleLimitData(int nBeginTime, int nMaxNum, int nNum );
	
	void setDailyUsedBull(int bull){ m_nDailyUsedBull = bull; }

	void setColorFontEndTime( int t ){ m_nColorFontEndTime = t; };

	//末世荣耀殿堂
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
	Container			m_Container;				// 背包容器

	bool				m_bPworldInfoFlag;			// 是否有副本信息
	int 				m_nPworldLeaveTime;			// 副本离开剩余时间
	int					m_nPworldStartTime;
	int					m_nPworldInstID;			// 副本实例ID
	int					m_nCollectMonId;			// 正在采集的对象
	bool				m_bAutoFind;				// 自动寻路状态
	IAutoFind*			m_pAutoLine;
	int					m_nMemID;
	bool				m_bChangeSkillShortCut;		// 是否切换技能动作条
	bool				m_bUnaware;					// 措手不及，表示被怪主动攻击进入战斗
	unsigned int		m_lastCmdTime;
	int					m_DriverId;

	char				m_szPkTargetName[ROLE_NAME_LEN];	//切磋对象

	bool				m_bIsOutSide;				//是否跨服
	int					m_nYestodayInfo;		//昨天的驱逐活动信息

	int					m_WeekCharm;				//本周魅力值

	int					m_nNormalMachinePworldClanID;		//开启守护枢纽-普通模式的公会ID（其实只能是自己的公会的ID）
	int					m_nUnionMachinePworldClanID;		//开启守护枢纽-联合模式的公会ID

	int					m_nLoveColorType;		//亲密度关系类型，1情侣，2夫妻
	char				m_szLoverName[ROLE_NAME_LEN];		//情侣或夫妻关系，另一半的名字
	tdr_ulonglong		m_loverID;							//另一半的roleid
	
private:
	DiathesisManager	m_Diathesis;
	bool				m_bCanControl;
	bool				m_bAutoTalkNpc;
	int					m_lastAutoTalkNpcId;
	int					m_nLockCount;		
	tagConvoyTask		m_convoyInfo;				// 所拥有的任务怪物
	SimpleFunction<>	*m_moveTask;
	bool				m_isAutoMove;
	int					m_nWorldID;
	int					m_nGmPermFlag;				// GM 权限
	//离线经验
	int					m_OneOffExp;				//一次性离线奖励
	int					m_OfflineExp;				// 补偿
	int					m_MaxOfflineExp;			// 最大值补偿
	int					m_PworldExp;				// 活动补偿
	int					m_MaxPworldExp;				// 最大活动补偿

	int					m_nBattleBeginTime;         /*   战场周期开始时间 */
	int					m_nBattleNum;               /*   战场周期内进入次数 */
	int					m_nBattleMaxNum;            /*   战场周期内最多进入次数 */
	int					m_lastMoveToX;
	int					m_lastMoveToY;
	bool				m_lastMoveToFlag;
	bool				m_mustContinueAutoMove;
	int					m_nDailyUsedBull;			//当日消耗的bull
	int					m_nColorFontEndTime;

	int					m_SpartaVictory;			//末世荣耀殿堂胜利次数
	int					m_SpartaRunAway;			//末世荣耀殿堂逃跑次数
	int					m_SpartaDefeat;				//末世荣耀殿堂失败次数

	int					m_OffLineDay;				//玩家流失天数
};//tolua_export
