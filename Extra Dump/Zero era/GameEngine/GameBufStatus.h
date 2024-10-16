#pragma once
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameSkill_DefToLua.h"
#include "resdb_meta.h"

enum
{
	STATUS_MISC_TYPE_ENDURE = 0,	// 霸体
	STATUS_MISC_TYPE_MOTION_BLUR,	// 运动模糊
	STATUS_MISC_TYPE_VISION_BLUR,	// 视觉模糊
	STATUS_MISC_TYPE_STOP_ACTION,	// 定身
};

#pragma pack(1)
//tolua_begin
struct tagBindSkill_ToLua
{
	unsigned short SkillID;                         	/*   技能ID */
	unsigned char Level;                           	/*   技能等级 */
	unsigned char Trig;                            	/*   触发条件，按位或 */
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
	unsigned short StatusID;                        	/*   状态ID，1000到3000 */
	unsigned short Level;                           	/*   状态等级 */
	char Name[RES_NAME_LEN];             
	int IconID;                          	/*  Ver.6 图标id */
	int ModeID;                          	/*  Ver.9 模型id */
	int EffectID;                        	/*  Ver.52 特效id */
	int BindPoint;                       	/*  Ver.26 状态特效绑点 */
	unsigned char Type;                            	/*    Bind Macrosgroup:STATUS_TYPE,*/
	unsigned char SubType;                        
	unsigned char DeadClear;                       	/*   是否死亡清除 */
	unsigned char OverlayLimit;                    	/*   叠加上限 */
	tagBindSkill_ToLua BindSkill;                      
	tagStatusResult_ToLua Result[MAX_STATUS_RESULT];      
	char StatusDesc[DESC_LEN];           
	int EffectType;                      	/*  Ver.75 效果类型 */
	unsigned char ZaRate;                          	/*  Ver.40 杂兵概率 */
	unsigned char NormRate;                        	/*  Ver.40 普通怪物概率 */
	unsigned char JinYinRate;                      	/*  Ver.40 精英怪物概率 */
	unsigned char BossRate;                        	/*  Ver.40 Boss概率 */
	unsigned char PlayerRate;                      	/*  Ver.40 玩家概率 */
	unsigned char ConsumeStatusType;               	/*  Ver.49 消耗资源类型 Bind Macrosgroup:CONSUME_STATUS_TYPE,*/
	unsigned char ConsumeStatusValue;              	/*  Ver.49 消耗资源值 */
	unsigned char IsWushuang;                      	/*  Ver.94 是否无双状态 */
	unsigned char CanCancel;                       	/*  Ver.99 这个状态是否可以被玩家自己取消 */
	unsigned short StatusFlag;                      	/*  Ver.124 按位或的规则 ,状态特殊标志 1 下线清除 2 被攻击清除 */
	unsigned short OverlayType;                     	/*  Ver.144 叠加类型 */
	char IsCltShow;                       	/*  Ver.175 客户端是否显示 */
	int OutFightKeep;                    	/*  Ver.188 怪物战斗结束是否保持不清除 */
	int AddModelID;                      	/*  Ver.283 额外的模型id */
	unsigned char MachineRate;                     	/*  Ver.523 机械概率 */
	unsigned int CltMiscFlag;                     	/*  Ver.604 客户端相关用的位 */
	unsigned char StatusInterVal;                  	/*  Ver.616 状态间隔，单位秒，0则取默认值3 */
	unsigned char ByAtkClear;                      	/*  Ver.2333 被攻击多少次状态消失0-不会消失 */
};

struct tagStatusOverlayDef_ToLua
{
	unsigned short StatusID;                        	/*   状态ID，1000到3000 */
	unsigned char Level;                           	/*   状态等级 */
	unsigned char Overlay;                         	/*   叠加数 */
	short Mul;                             	/*   修正指数 */
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
bool isStatusShowAsDebuff( int id, int lv );   // 显示到debuff列
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

	// 是否中断技能
	bool	isSkillStop( int skillId );
	bool	isSkillStop();
	//// 是否能行走
	bool	isMoveStop();
	// 是否具有某个result
	bool	isHasResult(unsigned short result);
	// 是否霸体
	bool	isEndure();

	//tolua_end
public:
	GameBufStatus*	 m_pBufStatus;

	unsigned int	m_nStartTime;
	CSROLESTATUS	m_StateInfo;	
	bool			m_bDead;
	bool			m_bShow;		// 是否显示
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
	// 取消主角的某个buffer状态
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

	// 是否可以使用技能
	bool canUsedSpell( int nSkillId );
	bool canUsedSpell();
	// 是否处于霸体状态
	bool isInEndure();
	// 是否处于不可攻击状态
	bool isInNoSkill();
	// 是否可以移动
	bool canMove();
	// 是否存在Buf
	bool checkBuf( int nBufId );
	// 查询某个buf的状态
	GameBuf* getBufStatus( int nBufId, int nLv );
	// 查询具有某个result的buff
	GameBuf* getBufResult(unsigned short result);
	// 查询某个状态的效果
	static const tagStatusDef& getStatusDef(int id, int lv);
	// 判断某个状态是否禁止施放技能
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
	GameBuf					m_GoodStatusList[MAX_GOOD_STATUS];		// 此为良性状态
	GameBuf					m_BadStatusList[MAX_BAD_STATUS];		// 此为不良状态
	GameBuf					m_NolimitStatusList[MAX_NOLIMIT_STATUS];//	
	int						m_ownerId;
	unsigned int			m_lastSetIdx;
	std::vector<GameBuf>	m_JiaStatusList;
};//tolua_export
