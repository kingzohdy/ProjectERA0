#pragma once
#include "GameEngineTypes.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDBData.h"

const int PET_MAX_FIGHT_SHIS = 3;

#pragma pack(1)

//tolua_begin
enum  
{
	CLIENT_PET_STAT_FIGHT = 1, 		/* ���� */
	CLIENT_PET_STAT_SEEFIGHT = 2, 	/* ��ϰ */
	CLIENT_PET_STAT_DEAD = 4, 		/* ���� */
};

struct tagPetId
{
	int nHeight;
	int nLow;
};

struct tagRoleArm_ToLua
{
	int DefID;                          
	int DefPos;                         
	unsigned short Num;                            
	short GridIdx;                        
	tdr_ulonglong WID;                             	/*   ��ƷΨһ��ʶ, �뱣֤���ֶκ�֮ǰ���ֶ���RoleItem��ȫ��ͬ */
	unsigned int Dur;                             	/*  Ver.8 װ����Ķ� */
	int ValidEnd;                        	/*  Ver.8 ��Ч����ʱ�䣬������װ */
};

struct tagPetArm_ToLua
{
	unsigned char Num;                            
	unsigned char MaxNum;                         
	unsigned char AddedCount;                      	/*   ����װ�������� */
	tagRoleArm_ToLua Arms[MAX_PET_ARM];              
};

struct tagPetSkillOne_ToLua
{
	unsigned short SkillID;                        
	unsigned char SkillLevel;                     
	unsigned char Res;                            
	unsigned int SkillCollectNum;     
};

struct tagPetDiathesisOne_ToLua
{
	unsigned short DiathesisID;                    
	unsigned char Level;                          
	unsigned char Res;                            
};

struct tagPetSkill_ToLua
{
	unsigned char SkillNum;                       
	unsigned char MaxNum;                         
	unsigned char DiathesisNum;                   
	unsigned char Res;                            
	tagPetSkillOne_ToLua Skills[MAX_PET_SKILL];          
	tagPetDiathesisOne_ToLua Diathesis[MAX_PET_DIATHESIS];   
};

struct tagPetAttr_ToLua
{
	unsigned char Num;                            
	unsigned char ForgetNum;                      
	unsigned short AddAttrID[MAX_PET_ATTR];        
	unsigned short AddAttrIDForget[MAX_PET_ATTR_FORGET_SAVE];
};

struct tagRolePet_ToLua
{
	unsigned short PetID;                          
	char Name[ROLE_NAME_LEN_CLT];        
	tagPetId WID;									/*   ����Ψһ��ʶ */
	unsigned int Exp;                             	/*   ���ﾭ��ֵ */
	unsigned int SkillPoint;                      	/*   ����ʣ�༼�ܵ� */
	unsigned int SkillPointTotal;                 	/*   �����ܹ���õļ��ܵ� */
	unsigned int MaxPower;                        	/*   ���ӵ����MP�����������HP */
	unsigned int Power;                           	/*   ���ӵ�MP��������HP */
	unsigned char Level;                          
	unsigned char Hierarchy;                       	/*   �ڼ��� */
	unsigned char GenEggNum;                       	/*   �����Ĵ��� */
	unsigned char ZhuanShen;                       	/*   ��ת�ĳ��� */
	unsigned int QianLi;                          	/*   Ǳ�� */
	unsigned int NengLi;                          	/*   ���� */
	unsigned int PingZhi;                         	/*   Ʒ�� */
	unsigned int QianZhi;                         	/*   Ǳ�� */
	unsigned int Age;                             	/*   ���� */
	tagPetArm_ToLua PetArms;                        
	tagPetSkill_ToLua PetSkills;                      
	tagPetAttr_ToLua PetAttrs;                      /*   ����ؼ������� */
	int PetDefIdx;                      
	char GridIdx;                         			/*   ��λ���ӵ����� */
	unsigned char StatFlag;                        	/*   ״̬ */
	unsigned char AddSkillGrid;                    	/*  Ver.55 ���⼼�������� */
};

struct tagPetDef_ToLua
{
	unsigned int ID;                             
	unsigned int MouldID;							  	/*  Ver.53  */
	char Name[NAME_LEN];                 
	unsigned char Type;								  	/*   ������� Bind Macrosgroup:PET_TYPE,*/
	unsigned char Race;								  	/*   ������� */
	unsigned char SubRace;							  	/*   ��������� */
	unsigned char MaxLevel;							  	/*   �ȼ����� */
	unsigned char BornLevel;						 	/*   ����ʱ�ĳ�ʼ�ȼ� */
	unsigned char MaxAge;							 	/*   ������� */
	unsigned short MaxGenEgg;                       	/*   ����¶��ٴε� */
	PETGENEGG GenEggs[MAX_PET_GENEGG_ID];				/*   �������ĵ����б� */
	unsigned short IceAtk;                          	/*   �����Թ����� */
	unsigned short IceDef;                          	/*   �����Է����� */
	unsigned short FireAtk;                         	/*   �����Թ����� */
	unsigned short FireDef;                         	/*   �����Է����� */
	unsigned short ThunderAtk;                      	/*   �����Թ����� */
	unsigned short ThunderDef;                      	/*   �����Է����� */
	unsigned short LightAtk;                        	/*   �����Թ����� */
	unsigned short LightDef;                        	/*   �����Է����� */
	unsigned short NightAtk;                        	/*   �����Թ����� */
	unsigned short NightDef;                        	/*   �����Է����� */
	unsigned short PhsicAtk;                        	/*   �������Թ����� */
	unsigned short PhsicDef;						   	/*   �������Է����� */
	ATKERHURTCHG AtkerHurtChg;						  	/*   �˺����� */
	ATKEEHURTCHG AtkeeHurtChg;							/*   �ֿ��˺����� */
	unsigned short Hit;                             	/*   ������ */
	unsigned short Flee;                            	/*   ������ */
	short TiShen;									 	/*   ��������ĸ��� */
	unsigned short SubRaceLevelStart;               	/*   �󻮲���Ҫ��д */
	unsigned short SubRaceLevelEnd;                 	/*   �󻮲���Ҫ��д */
	char PetDesc[SHORT_DESC_LEN];						/*  Ver.53 �������� */
	unsigned short BasicSkills[MAX_PET_BASIC_SKILL]; 	/*   ����������ܱ�����ѧϰ */
	unsigned short BasicDiathesis[MAX_PET_BASIC_DIATHESIS]; 	/*   ����������ʱ�����ѧϰ */
	unsigned int HideSkills[MAX_PET_HIDE_SKILL];  		/*   �������ؼ��ܱ� */
	unsigned short BornSkills;                      	/*  Ver.56 ����������� */
};

struct tagAttrResult_ToLua
{
	unsigned short ResultID;                        	/*    Bind Macrosgroup:RESULT,*/
	short ResultVal1;                     
	int ResultVal2;                     
	int ResultVal3;                     
	int ResultVal4;               
};

struct tagAddAttrDef_ToLua
{
	unsigned int AddAttrID;                       	/*   ��������ID */
	char AddAttrDesc[SHORT_DESC_LEN];     	/*  Ver.54 �ؼ������� */
	tagAttrResult_ToLua Result[MAX_ADDATTR_RESULT];     
};

#pragma pack()
//tolua_end

class GamePet//tolua_export 
	: public NetMsgHandler
{//tolua_export
public:
	GamePet(void);
	~GamePet(void);

public:
	// C->S
	
	//tolua_begin
	// ���� 
	void petNirvana( int nHightId, int nLowId  );
	// �µ�
	void petEgg( int nHightId, int nLowId );
	// ׼������
	void petPre( int nHightId, int nLowId );
	// ����
	void petCampaign( int nHightId, int nLowId );
	// ��ϰ
	void petNoviciate( int nHightId, int nLowId );
	// ��
	void petInSupport( int nHightId, int nLowId );
	// ת��
	void petRelive( int nHightId, int nLowId );
	// ����ѧϰ( type = 0 ���ܣ�1 ����)
	void petSkillUp( int nType, int nId, int nLv, int nHightId, int nLowId );
	// �޸ĳ�����
	void petChangeName( int nHightId, int nLowId, const char* szName );
	// ��������
	void petFreedomShi( int nIndex );
	// ��������
	void petFreedomXian( int nIndex );
	// ���ﴩװ��
	void petWearArm( int nArmId, int nListType, int nGridInex, int nHightId, int nLowId );
	// ������װ��
	void petUnWearArm( int nArmId, int nListType, int nGridInex, int nHightId, int nLowId );
	//tolua_end

	// S->C

	// ע��������Ϣ
	void initPetNetMessage();
	// ��������仯
	void petPackageChange( PETPACKAGECHG* pPackageChange );
	// ����Э�鴦��
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	// ����ӵ����
	void setOwner( GameActor* pOwner );

	//tolua_begin
	// ��ʼ��������λ
	void initPetPackage( tagPetPackage* petInfo );
	// ��ȡ��������
	int getXianNum();
	// ��ȡ��������
	int getShiNum();
	// ��ȡ��������
	int getXianMaxNum();
	// ��ȡ��������
	int getShiMaxNum();
	// ��ȡ���ﵱǰ�ȼ������( type 0 ���� 1���� )
	int getPetMaxExp( int nType, int nlv );
	// ��ȡ����ս����
	int getPetFightVal( int id );

	// ��ȡ����ؼ�����Ϣ
	tagAddAttrDef_ToLua* getAddAttrDef( int id );
	// ��ȡ������Ϣ( type 0 ���� 1���� )
	tagRolePet_ToLua* getPetInfo( int nType, int nIndex );	
	// ��ȡ���ﾲ̬��Ϣ
	tagPetDef_ToLua* getPetDefInfo( int id );
	// �������ʵ��
	GameActor* getPetActor( int nXieDaiPos );
	//tolua_end

	// ��������ģ�ͳ���
	void setShiModle( int nOwnerId, int nXieDaiPos, int nPetId, const char* szPetName, int nPower, int nMaxPower );
	// ������ʾ����ģ��
	void reShowModle();
	// ���ó���λ��
	void setPetPostion( int nXieDaiPos );

	// �����ƶ�
	void moveTo( int nXiDaiPos, int to_x, int to_z, int dis );
	// ˢ�³����ƶ����
	void updatePetMoveSpe();
private:
	// ���ó�������
	void setPetAttr( ROLEPET& petAttr, CSPETATTRCHG* data );
	
private:
	GameActor*	m_pOwner;
	int			m_nRoleXianNum;
	int			m_nRoleXianMaxNum;
	int			m_nRoleShiNum;
	int			m_nRoleShiMaxNum;
	ROLEPET		m_RoleXianPets[MAX_PET_XIAN_PACKAGE];
	ROLEPET		m_RoleShiPets[MAX_PET_SHI_PACKAGE];

	GameActor*	m_pShiActor[PET_MAX_FIGHT_SHIS];			// ����ģ��
};//tolua_export
