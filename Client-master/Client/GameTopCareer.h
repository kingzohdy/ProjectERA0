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
	BAOMING_XIANSHI_TOLUA = BAOMING_XIANSHI, 	/* ����ʱ����δ���������ʾ�������������� */
	BAOMING_YI_BAO_TOLUA = BAOMING_YI_BAO, 	/* ����ʱ�����ѱ��� */
	BAOMING_NO_IN_TIME_TOLUA = BAOMING_NO_IN_TIME, 	/* δ�ڱ���ʱ���� */
};

#define DIANFENG_PWORLD_ID_TOLUA DIANFENG_PWORLD_ID

#define SHZ_READY_MAP_ID_TOLUA	SHZ_READY_MAP_ID 	/* ĩ���۷��ػ���׼����ͼID */
#define ZHZ_READY_MAP_ID_TOLUA	ZHZ_READY_MAP_ID 	/* ĩ���۷�������׼����ͼID */
#define LSZ_READY_MAP_ID_TOLUA	LSZ_READY_MAP_ID 	/* ĩ���۷���ɱ��׼����ͼID */
#define HMZ_READY_MAP_ID_TOLUA	HMZ_READY_MAP_ID 	/* ĩ���۷������׼����ͼID */
#define JNZ_READY_MAP_ID_TOLUA	JNZ_READY_MAP_ID 	/* ĩ���۷������׼����ͼID */
#define LHS_READY_MAP_ID_TOLUA	LHS_READY_MAP_ID 	/* ĩ���۷�����ʦ׼����ͼID */
#define SYZ_READY_MAP_ID_TOLUA	SYZ_READY_MAP_ID 	/* ĩ���۷�ʥ����׼����ͼID */
#define YLS_READY_MAP_ID_TOLUA	YLS_READY_MAP_ID 	/* ĩ���۷�Ԧ��ʦ׼����ͼID */

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

	int									m_ActiveTime;								//����ĩ���۷���ʼ��ʱ��

	Ogre::Entity						*Effigy_Career[MAX_SUPPORT_MEM_NUM];		//��λְҵ�ھ��ĵ���,����DIANFENG_JOB_TYPE��˳����
	tagDFFirstInfo						m_EffigyCareerInfo[MAX_SUPPORT_MEM_NUM];	//��λְҵ�ھ��ĵ�����Ϣ,����DIANFENG_JOB_TYPE��˳����

	tagDFFirstInfo						m_CareerChampionInfo[MAX_SUPPORT_MEM_NUM];	//��λְҵ�ھ�����Ϣ������DIANFENG_JOB_TYPE��˳����
	
	tagCSDianFengGuessMingDan			m_GuessMingDan;								//�˽ṹ��¼���Լ�֧�ֵ������Ϣ
	tagDFGroupInfo						m_CurCareerGroupInfo;						//���ڼ�¼��ǰѡ���ְҵ��������Ϣ

	tagSupportMem						m_SupportMem[MAX_SUPPORT_MEM_NUM];								//û�о���ʱ�����ڼ�¼�Լ�ѡ��֧�ֵ������Ϣ
};//tolua_export