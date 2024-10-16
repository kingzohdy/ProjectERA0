#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

#pragma pack(1)
//tolua_begin
enum tagDIANFENG_BAOMING_TOLUA
{
	BAOMING_XIANSHI_TOLUA = BAOMING_XIANSHI, 	/* 报名时间内未报名玩家显示报名参赛按键！ */
	BAOMING_YI_BAO_TOLUA = BAOMING_YI_BAO, 	/* 报名时间内已报名 */
	BAOMING_NO_IN_TIME_TOLUA = BAOMING_NO_IN_TIME, 	/* 未在报名时间内 */
};

#define DIANFENG_PWORLD_ID_TOLUA DIANFENG_PWORLD_ID

#define SHZ_READY_MAP_ID_TOLUA	SHZ_READY_MAP_ID 	/* 末世巅峰守护者准备地图ID */
#define ZHZ_READY_MAP_ID_TOLUA	ZHZ_READY_MAP_ID 	/* 末世巅峰征服者准备地图ID */
#define LSZ_READY_MAP_ID_TOLUA	LSZ_READY_MAP_ID 	/* 末世巅峰猎杀者准备地图ID */
#define HMZ_READY_MAP_ID_TOLUA	HMZ_READY_MAP_ID 	/* 末世巅峰毁灭者准备地图ID */
#define JNZ_READY_MAP_ID_TOLUA	JNZ_READY_MAP_ID 	/* 末世巅峰聚能者准备地图ID */
#define LHS_READY_MAP_ID_TOLUA	LHS_READY_MAP_ID 	/* 末世巅峰炼魂师准备地图ID */
#define SYZ_READY_MAP_ID_TOLUA	SYZ_READY_MAP_ID 	/* 末世巅峰圣佑者准备地图ID */
#define YLS_READY_MAP_ID_TOLUA	YLS_READY_MAP_ID 	/* 末世巅峰驭雷师准备地图ID */

#define MOSHI_DIANFENG_JINGCAI_MONEY_TOLUA MOSHI_DIANFENG_JINGCAI_MONEY
#define MOSHI_DIANFENG_BAOMING_MONEY_TOLUA MOSHI_DIANFENG_BAOMING_MONEY

//tolua_end
#pragma pack()

class GameTopCareer//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameTopCareer>
{//tolua_export
public:
	GameTopCareer(void);
	~GameTopCareer(void);

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	void		Update(unsigned int dtick);
	void		draw(FollowCamera *pcamera);

	void		ClearCareerEffigy();
	void		UpdateCareerEffigy();

	void		SetEffigyStarEffect( Ogre::Entity *Effigy, int nStar );

	//tolua_begin
	int			getCareerChampionRace( int careerIdx );
	int			getCareerChampionGender( int careerIdx );
	int			getCareerChampionAvatarId( int careerIdx, int posIdx );
	int			getCareerChampionAvatarStarLv( int careerIdx, int posIdx );
	const char*	getCareerChampionName( int careerIdx );
	const char*	getCareerChampionClanName( int careerIdx );

	const char* getSupportNameByIdx( int idx );
	int			getGuessGroupNum();
	const char*	getGuessGroupNameByIndex( int idx );
	const char*	getGuessGroupClanNameByIndex( int idx );
	int			getGuessGroupSupportNumByIndex( int idx );

	int			getActiveTime(){ return m_ActiveTime; }

	void		requestTopCareerSupportInfo( int idx );
	void		requestGuessTopCareer();
	void		requestJoinTopCareer();
	void		requestTopCareerTrans();
	void		requestTopCareerIfGuessed();
	void		SelectTopCareerWiner( int groupIdx, int idx );
	bool		IsTopCareerSelected( int groupIdx );
	//tolua_end

	void		SetEffigyCareerInfo( int idx, const tagDFFirstInfo info );
	void		SetCareerChampionInfo( int idx, const tagDFFirstInfo info );
	void		SetCurCareerInfo( tagDFGroupInfo info ){ m_CurCareerGroupInfo = info; }
	void		SetGuessMingDan( tagCSDianFengGuessMingDan mingdan ){ m_GuessMingDan = mingdan; }
	void		SetTopCareerActiveTime( int time ){ m_ActiveTime = time; }
private:

	int									m_ActiveTime;								//本次末世巅峰活动开始的时间

	Ogre::Entity						*Effigy_Career[MAX_SUPPORT_MEM_NUM];		//八位职业冠军的雕像,按照DIANFENG_JOB_TYPE的顺序来
	tagDFFirstInfo						m_EffigyCareerInfo[MAX_SUPPORT_MEM_NUM];	//八位职业冠军的雕像信息,按照DIANFENG_JOB_TYPE的顺序来

	tagDFFirstInfo						m_CareerChampionInfo[MAX_SUPPORT_MEM_NUM];	//八位职业冠军的信息，按照DIANFENG_JOB_TYPE的顺序来
	
	tagCSDianFengGuessMingDan			m_GuessMingDan;								//此结构记录了自己支持的玩家信息
	tagDFGroupInfo						m_CurCareerGroupInfo;						//用于记录当前选择的职业组的玩家信息

	tagSupportMem						m_SupportMem[MAX_SUPPORT_MEM_NUM];								//没有竞猜时，用于记录自己选择支持的玩家信息
};//tolua_export