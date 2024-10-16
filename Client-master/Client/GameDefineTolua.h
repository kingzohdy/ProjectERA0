#pragma once
#include "resdb_meta.h"

#pragma pack(1)

//tolua_begin 
struct tagLong_ToLua
{
	int nHeight;
	int nLow;
};
 
struct tagCSRoleStatus_ToLua
{
	unsigned short StatusID;                       
	unsigned char Level;                          
	unsigned char Overlay;                         	/*  Ver.128  */
	unsigned int Time;                           
	unsigned int ApplyID;                         	/*  Ver.566  */
	unsigned char ApplyRelation;                   	/*  Ver.566  */
};

struct tagCSStatusInfo_ToLua
{
	unsigned char GoodStatusNum;                  
	unsigned char BadStatusNum;                   
	unsigned char NoLimitStatusNum;               
	unsigned char Res;                            
	tagCSRoleStatus_ToLua GoodStatusList[MAX_GOOD_STATUS];
	tagCSRoleStatus_ToLua BadStatusList[MAX_BAD_STATUS];  
	tagCSRoleStatus_ToLua NolimitStatusList[MAX_NOLIMIT_STATUS];
};

struct tagResPos_ToLua
{
	int X;                              
	int Y;                              
};

struct tagSceneAreaDef_ToLua
{
	unsigned int MapID;                           	/*   ���ڵ�ͼ */
	unsigned int ListIndex;                       	/*   ��ͼ�����Ψһ���� */
	char Name[NAME_LEN];                  	/*   ������������� */
	tagResPos_ToLua Pos;                             	/*   λ�� */
	unsigned int MonsterID;                       	/*  Ver.119 ����ID */
	unsigned char Level;                           	/*  Ver.119 �ȼ� */
};

struct tagHomePos_ToLua
{
	tagResPos_ToLua OPos;                           
	int Xlen;                           
	int Ylen;                           
};

/* ũ�����������Ѿ�Ĭ��״̬1-���ѵ� */
struct tagFarmSoilDefault_ToLua
{
	tagResPos_ToLua Pos;                            
	int Stat;                           
};

struct tagHomeLvlDef_ToLua
{
	int Lvl;                            
	int PworldID;                        	/*   ��Ӧ�ĸ���ID */
	int NewUseMoney;                     	/*   �½�ʱ�������� */
	int NewUseItem;                      	/*   �½�ʱ���ĵ��� */
	tagHomePos_ToLua HousePos;                        	/*   סլλ�� */
	tagHomePos_ToLua FarmPos;                         	/*   ũ��λ�� */
	tagHomePos_ToLua GardenPos;                       	/*   ��԰λ�� */
	int GardenDefaultMonID;              	/*   Ĭ�ϵĻ�԰����ID */
	tagFarmSoilDefault_ToLua Soil[FARM_MAX_SOIL];             	/*   Ĭ��ũ������ */
	int SoilMonID;                       	/*   δ���ѵ�ˢ�ĸ��� */
};

struct tagHomeMonLvlDef_ToLua
{
	int Lvl;                            
	int Exp;                             	/*   ��Ҫ���پ������� */
};

struct tagHomeFarmDef_ToLua
{
	int Lvl;                            
	int Exp;                             	/*   ��Ҫ���پ������� */
	int SoilNum;                         	/*   �ȼ���Ӧ�Ĳ˵����� */
};

struct tagHomePetEgg_ToLua
{
	int Rate;                           
	int EggID;                          
};

struct tagGarndenMonSeg_ToLua
{
	int Lvl;                            
	tagHomePetEgg_ToLua Egg[GARDEN_MON_MAX_EGG];        
};

struct tagHomeMonSegDef_ToLua
{
	int MonID;                          
	tagGarndenMonSeg_ToLua Seg[GARDEN_MAX_LVL_SEG];         	/*   ���ﵰ��� */
};

struct tagArmProf_ToLua
{
	unsigned char GridIdx;                        
	char Res[3];                         
	unsigned int ArmDefID;                       
	unsigned char StarLv;                          	/*  Ver.647  */
};

struct tagRoleAvatarProf_ToLua
{
	unsigned char Num;                            
	char Res[3];                         
	tagArmProf_ToLua Arms[MAX_AVATAR];               
	unsigned int QiYue;                           	/*  Ver.58  */
	unsigned int ArmStar;                         	/*  Ver.1929  */
	unsigned char SelectFashion;                   	/*  Ver.2005  */
	unsigned char AddFashion;                      	/*  Ver.2005  */
	int BackModelID;                     	/*  Ver.2428 ����ֵ���������ģ��idΪ��� */
};

struct tagCSPetDiathesisView_ToLua
{
	unsigned short ID;                             
	unsigned short Level;                         
};

struct tagTeamPetView_ToLua
{
	char Passive;                         	/*   ����Ŀǰ�Ƿ��ڱ���ģʽ��0������ģʽ��1������ģʽ */
	char DiathesisNum;                    	/*   ����Ŀǰ���������� */
	tagCSPetDiathesisView_ToLua Diathesis[MAX_PET_SKILL_GRID];   	/*   ����Ŀӵ�е����� */
};
    
struct tagTeamMemberPet_ToLua
{
	unsigned char PetNum;                         
	char Res[3];                         
	tagTeamPetView_ToLua PetView[PET_MAX_FIGHT_SHI];     
};

struct tagTeamMember_ToLua
{
	char MemberName[NAME_LEN];           
	unsigned char Level;                          
	unsigned char Position;                        	/*    Bind Macrosgroup:TEAM_POSITION,*/
	unsigned char Career;                         
	unsigned char Gender;                         
	unsigned int HP;                             
	unsigned int MaxHP;                          
	unsigned int MP;                             
	unsigned int MaxMP;                          
	unsigned int MapID;                            
	tagResPos_ToLua Pos;                            
	tagCSStatusInfo_ToLua StatusInfo;                     
	int OffLineTime;                     	/*   ����ʱ�䣬0 ��ʾ���ߣ���0��ʾʲôʱ���µ��� */
	tagRoleAvatarProf_ToLua Avatar;                         
	unsigned char Head;                            	/*  Ver.17  */
	unsigned char QiYueValid;                      	/*  Ver.58  Bind Macrosgroup:QIYUE_STATE,*/
	unsigned char Face;                            	/*  Ver.98  */
	unsigned char Hair;                            	/*  Ver.98  */
	unsigned short RideID;                          	/*  Ver.136  */
	unsigned short RideLevel;                       	/*  Ver.136  */
	unsigned short FairyID;                         	/*  Ver.136  */
	unsigned short FairyLevel;                      	/*  Ver.136  */
	tagTeamMemberPet_ToLua PetView;                         	/*  Ver.255  */
	int MemID;                           	/*  Ver.392  */
	int Uin;                             	/*  Ver.798  */
	tdr_ulonglong RoleID;                          	/*  Ver.850  */
	int ZoneBusID;                       	/*  Ver.892  */
	char VoteFlag;                        	/*  Ver.1054 1��ʾ�޳� 2��ʾ���޳� */
	int KickStartTime;                   	/*  Ver.1054 ���˷������˵�ʱ�� */
	int BattlePworldID;                  	/*  Ver.1336 ���ս��״̬�ͻ��д�ID */
	int Follow;								/*  Ver.1397 ��Ӹ���״̬ */
	unsigned char EnterPworldFlag;                 	/*  Ver.1671 �����Զ�����1-ȷ�� */
};

struct tagTeamInfo_ToLua
{
	tdr_ulonglong TeamID;                          	/*   ����Ψһ��ʶ */
	char TeamName[NAME_LEN];             
	unsigned char MemberNum;                      
	unsigned char Res;                            
	tagTeamMember_ToLua TeamMember[MAX_TEAM_MEMBER];    
	unsigned int QiYue;                           	/*  Ver.58  */
	unsigned int TeamMonID;                       	/*  Ver.176  */
	int TeamBootyMode;                   	/*  Ver.585  Bind Macrosgroup:TEAM_BOOTY_MODE,*/
	int KickVoteStartTime;               	/*  Ver.1054 �������˵�ʱ��,0��ʾ����������߼� */
	int KickMemberUin;                   	/*  Ver.1054 �����˵ı�ʶ */
};

struct tagTeamApplyInfo_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[NAME_LEN];             
	unsigned char Career;                         
	unsigned char Level;                          
	unsigned char Gender;     
	unsigned int startTime;
	char ClanName[NAME_LEN];              	/*  Ver.1086  */
};
#pragma pack()
//tolua_end