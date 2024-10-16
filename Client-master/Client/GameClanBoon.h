#pragma once
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
enum tagBOON_ADD_TYPE_ToLua 
{
	BOON_ADD_TYPE_BINGDAO_DEF_SUCC_TOLUA = 1, 	/* �������سɹ� */
	BOON_ADD_TYPE_BINGDAO_DEF_FAIL_TOLUA = 2, 	/* ��������ʧ�� */
	BOON_ADD_TYPE_BINGDAO_ATK_SUCC_TOLUA = 3, 	/* ���������ɹ� */
	BOON_ADD_TYPE_XUEYUAN_DEF_SUCC_TOLUA = 4, 	/* ѩԭ���سɹ� */
	BOON_ADD_TYPE_XUEYUAN_DEF_FAIL_TOLUA = 5, 	/* ѩԭ����ʧ�� */
	BOON_ADD_TYPE_XUEYUAN_ATK_SUCC_TOLUA = 6, 	/* ѩԭ�����ɹ� */
	BOON_ADD_TYPE_SHAMO_DEF_SUCC_TOLUA = 7, 	/* ɳĮ���سɹ� */
	BOON_ADD_TYPE_SHAMO_DEF_FAIL_TOLUA = 8, 	/* ɳĮ����ʧ�� */
	BOON_ADD_TYPE_SHAMO_ATK_SUCC_TOLUA = 9, 	/* ɳĮ�����ɹ� */
	BOON_ADD_TYPE_LUORI_DEF_SUCC_TOLUA = 10, 	/* ���շ��سɹ� */
	BOON_ADD_TYPE_LUORI_DEF_FAIL_TOLUA = 11, 	/* ���շ���ʧ�� */
	BOON_ADD_TYPE_LUORI_ATK_SUCC_TOLUA = 12, 	/* ���ս����ɹ� */
	BOON_ADD_TYPE_YULIN_DEF_SUCC_TOLUA = 13, 	/* ���ַ��سɹ� */
	BOON_ADD_TYPE_YULIN_DEF_FAIL_TOLUA = 14, 	/* ���ַ���ʧ�� */
	BOON_ADD_TYPE_YULIN_ATK_SUCC_TOLUA = 15, 	/* ���ֽ����ɹ� */
	BOON_ADD_TYPE_MINGSHA_DEF_SUCC_TOLUA = 16, 	/* ��ɳ���سɹ� */
	BOON_ADD_TYPE_MINGSHA_DEF_FAIL_TOLUA = 17, 	/* ��ɳ����ʧ�� */
	BOON_ADD_TYPE_MINGSHA_ATK_SUCC_TOLUA = 18, 	/* ��ɳ�����ɹ� */
	BOON_ADD_TYPE_SHIJIAN_DEF_SUCC_TOLUA = 19, 	/* ʱ��Ƿ��سɹ� */
	BOON_ADD_TYPE_SHIJIAN_DEF_FAIL_TOLUA = 20, 	/* ʱ��Ƿ���ʧ�� */
	BOON_ADD_TYPE_SHIJIAN_ATK_SUCC_TOLUA = 21, 	/* ʱ��ǽ����ɹ� */
};

#define CLAN_BOON_LEVEL_TOLUA	CLAN_BOON_LEVEL
#define CLAN_TECH_NUM_TOLUA		CLAN_TECH_NUM
#define CLAN_ONLINE_AVE_TOLUA	CLAN_ONLINE_AVE
#define CLAN_GOLD_NUM1_TOLUA	CLAN_GOLD_NUM1
#define CLAN_GOLD_NUM2_TOLUA	CLAN_GOLD_NUM2
#define CLAN_BOON_BINDGOLD_TOLUA	CLAN_BOON_BINDGOLD
#define CLAN_BOON_GOLD_TOLUA	CLAN_BOON_GOLD
#define CLAN_CONTRI_NUM_TOLUA	CLAN_CONTRI_NUM
#define BOON_ADD_EVENT_MAX_TOLUA	BOON_ADD_EVENT_MAX
#define BOON_GET_MAX_TOLUA		BOON_GET_MAX


#pragma pack(1)
struct tagBoonAddEvent_ToLua
{
	unsigned char ID;                              	/*   ȡֵΪBOON_ADD_TYPE */
	char Per;                             	/*   �ӳɰٷֱ� */
	unsigned int Time;                            	/*   ʱ�� */
};

struct tagBoonCond_ToLua
{
	short Ave;                             	/*   ��ƽ������|12:00-24:00 */
	short AveNum;                          	/*   �ɼ��˼����� */
	unsigned int AveTime;                         	/*   �ɼ�ʱ�� */
	int Tech;                            	/*   �ܿƼ��� */
	int Contri;                          	/*   �ܹ��׶� */
	int Gold;                            	/*   �ܽ����ȡ */
	int EventNum;                       
	tagBoonAddEvent_ToLua Event[BOON_ADD_EVENT_MAX];       	/*   �����ӳ��¼� */
	unsigned int ClearTime;                      
};

struct tagBoonAssignPos_ToLua
{
	char Name[RES_NAME_LEN];             
	unsigned char Pos;                             	/*   ְλ */
	int Money;                           	/*   ���� */
	int Flag;                            	/*   0-δ��ȡ */
};

struct tagBoonAssign_ToLua
{
	int BindGlod;                        	/*   ���ܿɷ����ȯ */
	int Glod;                            	/*   ���ܽ�� */
	tagBoonAssignPos_ToLua AssignGlod;                      	/*   �᳤��� */
	int Office;                         
	tagBoonAssignPos_ToLua AssignBindGlod[BOON_GET_MAX];    	/*   ��ȯ����ϸ�� */
};

struct tagClanBoon_ToLua
{
	tagBoonCond_ToLua Cond;                           
	tagBoonAssign_ToLua Assign;                         
};
#pragma pack()
//tolua_end

class GameClanBoon//tolua_export
	:public Ogre::Singleton<GameClanBoon> , public NetMsgHandler
{//tolua_export
public:
	GameClanBoon();
	~GameClanBoon(){};
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	//tolua_begin
	//��ȡ����
	void requestClanBoonGet( int money, int flag );
	//���乤��
	void requestClanBoonAssign();
	//��ȡ���й�����Ϣ
	tagClanBoon_ToLua& getClanBoonData() { return m_ClanBoonInfo; }
	//��ȡ������Ϣ
	tagBoonAssign_ToLua& getBoonAssignData() { return m_AssignInfo; }
	//���÷�����Ϣ
	void resetAssignData();
	void setBoonAssignDataByIndex( int idx, char * szName, int pos, int money );
	//��ȡ��ȡ���
	int getBoonDataIndexByPos( int pos );
	//tolua_end
private:
	tagClanBoon_ToLua m_ClanBoonInfo;
	tagBoonAssign_ToLua m_AssignInfo;
};//tolua_export