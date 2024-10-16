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
	VIP_ASSIST_OPTION_INTERVAL = 0,				// ���ʱ��ѡ��
	VIP_ASSIST_OPTION_HPFOOD,					// HP����ѡ��
	VIP_ASSIST_OPTION_MPFOOD,					// MP����ѡ��
	VIP_ASSIST_OPTION_REPAIR,					// ����װ��ѡ��
	VIP_ASSIST_OPTION_SELL_NORMAL,				// ���۰�װѡ��
	VIP_ASSIST_OPTION_SELL_EXCELLENCE,			// ������װѡ��
	VIP_ASSIST_OPTION_SELL_BEST,				// ������װѡ��
	//	VIP_ASSIST_OPTION_DECOMPOSE,				// �ֽ�װ��ѡ��
	VIP_ASSIST_OPTION_DECOPOSE_EXCELLENCE,		// �ֽ���װѡ��
	VIP_ASSIST_OPTION_DECOPOSE_BEST,			// �ֽ���װѡ��
	VIP_ASSIST_OPTION_NO_PERFECT_STAR,			// ���ֽ�������ѡ��
	VIP_ASSIST_OPTION_RETURN_POS,				// ����ظ����ѡ��
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
	bool	m_bVipChkBtn[VIP_ASSIST_OPTION_END];    // VIP�߼�����
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
	// �����ͨ����ѡ��
	tagAutoAssistData*		getAutoAssistOpt();
	void					setAutoAssistBegin(bool isbegin);
	bool					getAutoAssistBegin();
	// ������ͨ����ѡ������
	void					saveAutoAssistOpt();
	bool					targetCanAttack( int targetId );
	// �̶���Χ�һ�
	bool					isInfixedRangeAssist();
	// ����һ������ꡢ��ͼid
	void					saveNativePos();
	// �Զ�Ѱ·����
	void					HandleAutoFind();
	// ��ÿɲ�׽����
	void					GetNearestCaptureMonster();
	void					ResumePetSubType();
	
	// ���VIP����ѡ��
	tagVipAutoAssistData*	getVIPAutoAssistOpt();
	// ����VIPѡ������
	void					saveVIPAssistOpt();
	// �Զ����عһ���
	bool					ReturnAssistPosition();
	// ���ø߼�������ʼֵ
	void					resetVipAssist();
	// ���ùһ����ͼID
	void					setAssistMapID( int nMapID );
	//tolua_end
	
	void					resetAssistInfo();

private:

	bool					m_asBeginFlag;			// �һ���ʼ���
	std::vector<int>		m_CapturePet;
	

public:

	tagAutoAssistData		m_assistData;			// ��ͨ����ѡ��
	GameXYZ					m_nPos;					// �һ�������
	int						m_nMapID;				// �һ����ͼid
	tagVipAutoAssistData	m_VipAssist;			// VIP����ѡ��

};//tolua_export