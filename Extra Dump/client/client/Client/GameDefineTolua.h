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
	unsigned int MapID;                           	/*   所在地图 */
	unsigned int ListIndex;                       	/*   地图区域的唯一索引 */
	char Name[NAME_LEN];                  	/*   功能区域的名字 */
	tagResPos_ToLua Pos;                             	/*   位置 */
	unsigned int MonsterID;                       	/*  Ver.119 怪物ID */
	unsigned char Level;                           	/*  Ver.119 等级 */
};

struct tagHomePos_ToLua
{
	tagResPos_ToLua OPos;                           
	int Xlen;                           
	int Ylen;                           
};

/* 农场土地坐标已经默认状态1-开垦的 */
struct tagFarmSoilDefault_ToLua
{
	tagResPos_ToLua Pos;                            
	int Stat;                           
};

struct tagHomeLvlDef_ToLua
{
	int Lvl;                            
	int PworldID;                        	/*   对应的副本ID */
	int NewUseMoney;                     	/*   新建时消耗银币 */
	int NewUseItem;                      	/*   新建时消耗道具 */
	tagHomePos_ToLua HousePos;                        	/*   住宅位置 */
	tagHomePos_ToLua FarmPos;                         	/*   农场位置 */
	tagHomePos_ToLua GardenPos;                       	/*   花园位置 */
	int GardenDefaultMonID;              	/*   默认的花园宠物ID */
	tagFarmSoilDefault_ToLua Soil[FARM_MAX_SOIL];             	/*   默认农场土地 */
	int SoilMonID;                       	/*   未开垦的刷哪个怪 */
};

struct tagHomeMonLvlDef_ToLua
{
	int Lvl;                            
	int Exp;                             	/*   需要多少经验升级 */
};

struct tagHomeFarmDef_ToLua
{
	int Lvl;                            
	int Exp;                             	/*   需要多少经验升级 */
	int SoilNum;                         	/*   等级对应的菜地数量 */
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
	tagGarndenMonSeg_ToLua Seg[GARDEN_MAX_LVL_SEG];         	/*   宠物蛋相关 */
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
	int BackModelID;                     	/*  Ver.2428 若有值，则代表背饰模型id为这个 */
};

struct tagCSPetDiathesisView_ToLua
{
	unsigned short ID;                             
	unsigned short Level;                         
};

struct tagTeamPetView_ToLua
{
	char Passive;                         	/*   宠物目前是否处于被动模式，0：主动模式；1：被动模式 */
	char DiathesisNum;                    	/*   宠物目前的素质总数 */
	tagCSPetDiathesisView_ToLua Diathesis[MAX_PET_SKILL_GRID];   	/*   宠物目拥有的素质 */
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
	int OffLineTime;                     	/*   下线时间，0 表示在线，非0表示什么时候下的线 */
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
	char VoteFlag;                        	/*  Ver.1054 1表示赞成 2表示不赞成 */
	int KickStartTime;                   	/*  Ver.1054 本人发起踢人的时间 */
	int BattlePworldID;                  	/*  Ver.1336 跨服战场状态就会有此ID */
	int Follow;								/*  Ver.1397 组队跟随状态 */
	unsigned char EnterPworldFlag;                 	/*  Ver.1671 副本自动传送1-确认 */
};

struct tagTeamInfo_ToLua
{
	tdr_ulonglong TeamID;                          	/*   队伍唯一标识 */
	char TeamName[NAME_LEN];             
	unsigned char MemberNum;                      
	unsigned char Res;                            
	tagTeamMember_ToLua TeamMember[MAX_TEAM_MEMBER];    
	unsigned int QiYue;                           	/*  Ver.58  */
	unsigned int TeamMonID;                       	/*  Ver.176  */
	int TeamBootyMode;                   	/*  Ver.585  Bind Macrosgroup:TEAM_BOOTY_MODE,*/
	int KickVoteStartTime;               	/*  Ver.1054 发起踢人的时间,0表示不处理相关逻辑 */
	int KickMemberUin;                   	/*  Ver.1054 被踢人的标识 */
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