#pragma once
#include <string>
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#define  MAX_AUTO_ASSIST_SKILL_NUM	10
#define	 MAX_CHKBTN_NUM				10

//tolua_begin

#define  MAX_VIP_CHKBTN_NUM			12
enum tagVIP_ASSIST_OPTION_TOLUA
{
	VIP_ASSIST_OPTION_INTERVAL = 0,				// 间隔时间选项
	VIP_ASSIST_OPTION_HPFOOD,					// HP购买选项
	VIP_ASSIST_OPTION_MPFOOD,					// MP购买选项
	VIP_ASSIST_OPTION_REPAIR,					// 修理装备选项
	VIP_ASSIST_OPTION_SELL_NORMAL,				// 出售白装选项
	VIP_ASSIST_OPTION_SELL_EXCELLENCE,			// 出售绿装选项
	VIP_ASSIST_OPTION_SELL_BEST,				// 出售蓝装选项
	//	VIP_ASSIST_OPTION_DECOMPOSE,				// 分解装备选项
	VIP_ASSIST_OPTION_DECOPOSE_EXCELLENCE,		// 分解绿装选项
	VIP_ASSIST_OPTION_DECOPOSE_BEST,			// 分解蓝装选项
	VIP_ASSIST_OPTION_NO_PERFECT_STAR,			// 不分解完美星选项
	VIP_ASSIST_OPTION_RETURN_POS,				// 复活回复活点选项
	VIP_ASSIST_OPTION_END,
};
struct tagAutoAssistData 
{
	int		m_nSkillId[MAX_AUTO_ASSIST_SKILL_NUM];
	int		m_hpId;
	int		m_mpId;
	int		m_hpSliderVal;
	int		m_mpSliderVal;
	bool	m_bChkBtn[MAX_CHKBTN_NUM];	
} ;
struct tagVipAutoAssistData
{
	bool	m_bVipChkBtn[VIP_ASSIST_OPTION_END];    // VIP高级辅助
	char	m_nHpIndex;
	char	m_nMpIndex;
	int		m_nHPNum;
	int		m_nMPNum;
	int		m_nInterval;
};

//tolua_end
class AutoAssist//tolua_export
	:public Ogre::Singleton<AutoAssist>
{//tolua_export
public:
	AutoAssist(void);
	~AutoAssist(void);

	//tolua_begin
	
	void					setAutoAssistPickUpOpt(int index,bool chkBtnVal );
	// 获得普通辅助选项
	tagAutoAssistData*		getAutoAssistOpt();
	void					setAutoAssistBegin(bool isbegin);
	bool					getAutoAssistBegin();
	// 保存普通辅助选项设置
	void					saveAutoAssistOpt();
	bool					targetCanAttack( int targetId );
	// 固定范围挂机
	bool					isInfixedRangeAssist();
	// 保存挂机点坐标、地图id
	void					saveNativePos();
	// 自动寻路处理
	void					HandleAutoFind();
	// 获得可捕捉宠物
	void					GetNearestCaptureMonster();
	void					ResumePetSubType();
	
	// 获得VIP辅助选项
	tagVipAutoAssistData*	getVIPAutoAssistOpt();
	// 保存VIP选项设置
	void					saveVIPAssistOpt();
	// 自动返回挂机点
	bool					ReturnAssistPosition();
	// 设置高级辅助初始值
	void					resetVipAssist();
	// 设置挂机点地图ID
	void					setAssistMapID( int nMapID );
	//tolua_end
	
	void					resetAssistInfo();

private:

	bool					m_asBeginFlag;			// 挂机开始标记
	std::vector<int>		m_CapturePet;
	

public:

	tagAutoAssistData		m_assistData;			// 普通辅助选项
	GameXYZ					m_nPos;					// 挂机点坐标
	int						m_nMapID;				// 挂机点地图id
	tagVipAutoAssistData	m_VipAssist;			// VIP辅助选项

};//tolua_export