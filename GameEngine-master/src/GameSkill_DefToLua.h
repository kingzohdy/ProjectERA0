#pragma once
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1)

struct tagSkillLearn_ToLua
{
	unsigned char LevelMin;                        	/*   �ȼ�Ҫ�� */
	unsigned char Career;                          	/*   ְҵҪ�� */
	unsigned short PreDiathesisID;                  	/*   ǰ������Ҫ�� */
	unsigned int ItemID;                          	/*   ����Ҫ�� */
	unsigned short TaskID;                          	/*   �������Ҫ�� */
	unsigned short MoneyAttrID;                     	/*   ��Ҫ�Ľ�Ǯ���� */
	unsigned int MoneyVal;                        	/*   ��Ҫ�Ľ�Ǯ���� */
	unsigned char Mode;                            	/*   ѧϰ��ʽ */
	unsigned char PreDiathesisLevel;               	/*   ǰ�����ʵȼ�Ҫ�� */
	unsigned int Exp;         
};

struct tagFittingList_ToLua
{
	unsigned short ID;                             
	unsigned short Group;                          
};
struct tagResRange_ToLua
{
	unsigned int RangeType;                       	/*    Bind Macrosgroup:SKILL_RANGE_TYPE,*/
	unsigned short RangeVal1;                       	/*   �뾶��� */
	unsigned short RangeVal2;                       	/*   �ǶȻ� */
};

struct tagSkillResult_ToLua
{
	unsigned short SkillResultID;                   	/*    Bind Macrosgroup:RESULT,*/
	short SkillResultVal1;                
	int SkillResultVal2;                
	int SkillResultVal3;                
	int SkillResultVal4;                
};

struct tagSkillDef_ToLua
{
	unsigned short SkillID;                        
	unsigned short Level;                          
	char Name[RES_NAME_LEN];             
	int IconID1;                         	/*  Ver.6 ͼ��id */
	int IconID2;                         	/*  Ver.6 ͼ��id */
	int IconID3;                         	/*  Ver.6 ͼ��id */
	ARMTYPE ArmType[MAX_SKILL_ARMTYPE];      	/*   ���ܵ��������� */
	unsigned int SelfSpeCoolType;                 	/*   ������������ָ����ȴ���� */
	unsigned char SpeCoolType1;                    	/*   ����ָ��������ȴ���� */
	unsigned char SpeCoolType2;                   
	unsigned char SpeCoolType3;                   
	unsigned char SpeCoolType4;                   
	unsigned short SpeCoolVal1;                     	/*   ����ָ��������ȴֵ����λ100ms */
	unsigned short SpeCoolVal2;                     	/*   ����ָ��������ȴֵ����λ100ms */
	unsigned short SpeCoolVal3;                     	/*   ����ָ��������ȴֵ����λ100ms */
	unsigned short SpeCoolVal4;                     	/*   ����ָ��������ȴֵ����λ100ms */
	unsigned short CommCool;                        	/*   ����������ȴֵ����λ100ms */
	unsigned short SelfCool;                        	/*   ���Ҽ�����ȴֵ����λ100ms */
	unsigned short TargetType;                      	/*   ���ܶ������� */
	unsigned short HelpSkillID;                     	/*   ����ʱ��Ӧ�ļ��� */
	unsigned char HelpSkillLevel;                  	/*   ����ʱ��Ӧ�ļ��ܵȼ� */
	unsigned char MovePrepare;                     	/*   �ƶ��Ƿ����ֱ���ж��������� */
	unsigned char MoveAttack;                      	/*   �Ƿ���Ա��ƶ��߹��� */
	unsigned char MainAttr;                        	/*   ���������� Bind Macrosgroup:SKILL_ATTR,*/
	unsigned int IceHurtMax;                     
	unsigned int IceHurtMin;                     
	short IceHurtMul;                     
	short IceHurtMulMax;                   	/*  Ver.127  */
	unsigned short FireHurtMax;                    
	unsigned short FireHurtMin;                    
	short FireHurtMul;                    
	short FireHurtMulMax;                  	/*  Ver.419  */
	unsigned short ThunderHurtMax;                 
	unsigned short ThunderHurtMin;                 
	short ThunderHurtMul;                 
	short ThunderHurtMulMax;               	/*  Ver.419  */
	unsigned short LightHurtMax;                   
	unsigned short LightHurtMin;                   
	short LightHurtMul;                   
	short LightHurtMulMax;                 	/*  Ver.419  */
	unsigned short NightHurtMax;                   
	unsigned short NightHurtMin;                   
	short NightHurtMul;                   
	short NightHurtMulMax;                 	/*  Ver.419  */
	unsigned int PhsicHurtMax;                   
	unsigned int PhsicHurtMin;                   
	short PhsicHurtMul;                   
	short PhsicHurtMulMax;                 	/*  Ver.127  */
	unsigned int WaterHurtMax;                    	/*  Ver.419  */
	unsigned int WaterHurtMin;                    	/*  Ver.419  */
	short WaterHurtMul;                    	/*  Ver.419  */
	short WaterHurtMulMax;                 	/*  Ver.419  */
	unsigned int SiegeHurtMax;                    	/*  Ver.419  */
	unsigned int SiegeHurtMin;                    	/*  Ver.419  */
	short SiegeHurtMul;                    	/*  Ver.419  */
	short SiegeHurtMulMax;                 	/*  Ver.419  */
	unsigned int HolyHurtMax;                     	/*  Ver.419  */
	unsigned int HolyHurtMin;                     	/*  Ver.419  */
	short HolyHurtMul;                     	/*  Ver.419  */
	short HolyHurtMulMax;                  	/*  Ver.419  */
	unsigned int MachineHurtMax;                  	/*  Ver.419  */
	unsigned int MachineHurtMin;                  	/*  Ver.419  */
	short MachineHurtMul;                  	/*  Ver.419  */
	short MachineHurtMulMax;               	/*  Ver.419  */
	unsigned short AttackActTime;                   	/*   ��������ʱ�䣬��ʱ���ɫ�����ƶ�����λ100ms */
	unsigned char BindSkill;                       	/*  Ver.4 �������Ƿ��ǰ󶨼��ܣ���������д Bind Macrosgroup:BINDSKILL_TYPE,*/
	unsigned char FightStat;                       	/*  Ver.8 ���ܶ�ս��״̬���жϱ�־ Bind Macrosgroup:SKILL_FIGHT_STAT,*/
	tagResRange_ToLua MainRange;                       	/*   ��������Χ */
	tagResRange_ToLua SecondRange;                     	/*   ���ܸ���Χ */
	int Distance;                        	/*   �������� */
	int CollectNumMax;                   	/*   ����������,����ʱ������ */
	int CollectNumUse;                   	/*   ���Ļ�����,����ʱ������ */
	int CollectSkillUse;                 	/*  Ver.311 ��������������,0��ʾ���ǻ��������� */
	unsigned short SpdUse;                          	/*  Ver.49 �����ж��� */
	unsigned short MPUse;                           	/*   ����MP */
	unsigned short CollectNumAtkAdd;                	/*   ����ʱ���ӻ����� */
	unsigned short CollectNumDeadAdd;               	/*   ɱ��ʱ���ӻ����� */
	unsigned short MPAtkAdd;                        	/*   ����ʱ����MP */
	unsigned short MPDeadAdd;                       	/*   ɱ��ʱ����MP */
	unsigned char CollectNumAtkAddFlag;            	/*   �������Ƿ񹥻�ʱ���ӻ����� */
	unsigned char CollectNumDeadAddFlag;           	/*   �������Ƿ�ɱ��ʱ���ӻ����� */
	unsigned short JiqiMul;                         	/*  Ver.568 �����ı���Z ��ʽX��1 + z * y/t�� */
	unsigned short JiqiTime;                        	/*  Ver.568 ������ʱ��t,��λ�� */
	unsigned short JiqiChgFlag;                     	/*  Ver.568 �����Ƿ�Ҫת����Ƶ�� */
	unsigned short ContinueNum;                     	/*   Ƶ���������ǹ���� */
	unsigned char HeavyHitRate;                    	/*   ������ */
	unsigned char TargetNum;                       	/*   ��Χ����ʱͬʱ�˺��Ķ������ */
	unsigned short PrepareTime;                     	/*   ����׼��ʱ�䣬��λ100ms */
	unsigned char PrepareType;                     	/*   ����׼����˲�������������� Bind Macrosgroup:PREPARE_TYPE,*/
	unsigned char DeadTarget;                      	/*   �Ƿ���Զ�����Ŀ��ʹ�� */
	tagSkillResult_ToLua SkillResult[MAX_SKILL_RESULT];  
	unsigned int HurtLimit;                       	/*   �˺����� */
	unsigned short PilotTime;                       	/*   ���������ļ������λ100ms */
	unsigned short PilotNum;                        	/*   ���������Ĵ��� */
	unsigned char NoHurtSkill;                     	/*   �Ƿ��Ƿ��˺����ܣ������������Ѫ�� */
	unsigned char NoSkillStop;                     	/*   �Ƿ���Ҫ���Է����赲���������ס�Զ���� */
	unsigned char TotemLimit;                      	/*  Ver.36 ͼ���������� */
	unsigned char NoFightStatUse;                  	/*  Ver.40 ��ս��״̬��ʹ�� Bind Macrosgroup:NO_FIGHT_USE,*/
	short PressRate;                       	/*  Ver.10 ����ѹ��ָ�� */
	unsigned short NextSkill;                       	/*  Ver.10 �������� */
	unsigned short NextSkill2;                      	/*  Ver.10 �������� */
	unsigned short NextSkill3;                      	/*  Ver.10 �������� */
	char SkillDesc[DESC_LEN];            
	SKILLARMCHG SkillArmChg[MAX_SKILL_ARM_CHG];  	/*  Ver.37  */
	short AmmUse;                          	/*  Ver.12 �������ĵ�ҩ */
	unsigned char NotBootyFirstAtk;                	/*  Ver.23 0 �ᴥ��ս��Ʒ�������жϣ�1 ���ᴥ��ս��Ʒ�������ж� */
	unsigned char TargetRange;                     	/*  Ver.23 1 ��ʾĿ�귶Χ�������Ƿ�Χ��������Ҫѡ��һ��Ŀ����в��� Bind Macrosgroup:TARGET_RANGE,*/
	SKILLSTATUSCHG StatusChg;                       	/*  Ver.40 ״̬Ӱ�켼�ܣ����羫ȷ��� */
	tagSkillLearn_ToLua Learn;                           	/*  Ver.45 ����ѧϰ��� */
	unsigned short FittingsDefault[MAX_SKILL_DEFAULT_FITTINGS]; 	/*  Ver.40 Ĭ���������ü���ʱ�Զ����� */
	unsigned short TmpEnergeUse;                    	/*  Ver.26 �������ĵ���ʱ���� */
	unsigned short PetAddAge;                       	/*  Ver.55 ʹ�ô˼���,�������ӵ����� */
	unsigned char SkillSwitch;                     	/*  Ver.50 �Ƿ��ǿɿ��ؼ��� */
	unsigned char CollectNumAddNum;                	/*  Ver.52 ��ʾ����ÿСʱ���ӻ�����������Ŀǰֻ�Գ��＼����Ч */
	unsigned char FightCollectNumAddNum;           	/*  Ver.52 ��ʾս��״̬ÿ3��ظ��Ļ�������Ŀǰֻ�Գ��＼����Ч */
	unsigned char SkillType;                       	/*  Ver.63 �������� Bind Macrosgroup:SKILL_TYPE,*/
	short MonMiniMul;                      	/*  Ver.60 ΢�ּ����˺����� */
	short MonWeakMul;                      	/*  Ver.60 ���ּ����˺����� */
	short MonSoftMul;                      	/*  Ver.60 ��ּ����˺����� */
	short MonNormalMul;                    	/*  Ver.60 �չּ����˺����� */
	short MonHardMul;                      	/*  Ver.60 Ӳ�ּ����˺����� */
	short MonXiMul;                        	/*  Ver.60 ϡ�ּ����˺����� */
	short MonJinMul;                       	/*  Ver.60 ���ּ����˺����� */
	short MonBossMul;                      	/*  Ver.60 BOSS �ּ����˺����� */
	short MonJiMul;                        	/*  Ver.60 �� �ּ����˺����� */
	unsigned char LearnLevelLimit;                 	/*  Ver.70 ��������ѧ�ȼ����� */
	unsigned char BeatBack;                        	/*  Ver.86 ��������|1 ����״̬���Ӽ��ܻ�״̬������������ */
	unsigned char WushuangFlag;                    	/*  Ver.92 ��˫��־,0--������˫�� 1--��������˫�� 2-������˫�� */
	short UseXp;                           	/*  Ver.99 ����xpֵ */
	unsigned char IgnoreNoSkill;                   	/*  Ver.119 �����޷�ʹ�ü��ܵ�״̬Ч�� */
	unsigned char LightningLinkNum;                	/*  Ver.576 �������Ĺ�������,�����ֵ,�ͻ���Ծ������ */
	TARGETSTATUSONE TargetStatus[MAX_TARGET_STATUS]; 	/*  Ver.121 Ŀ��״̬Ӱ�� */
	short NoHurtPress;                     	/*  Ver.130 ���˺��༼�ܲ�����ѹ��ֵ  */
	unsigned char FairySkill;                      	/*  Ver.170 �Ƿ��鼼��0-�� 1-�� */
	int AmmID;                           	/*  Ver.439 �������ĵĵ�ҩID */
	int RedRuneTransferedID;             	/*  Ver.551 �ɺ���Ķ�Ӧ�ļ���ID */
	int BlueRuneTransferedID;            	/*  Ver.551 ������Ķ�Ӧ�ļ���ID */
	int YellowRuneTransferedID;          	/*  Ver.551 �����Ķ�Ӧ�ļ���ID */
	char RuneDesc[SHORT_DESC_LEN];        	/*  Ver.582  */
	char NextLevelDesc[SHORT_DESC_LEN];   	/*  Ver.622  */
	int EnergyUse;                       	/*  Ver.626 �����е����:ʹ�ü������Ķ������� */
	int AtkType;                         	/*  Ver.1105  Bind Macrosgroup:SKILL_ATK_TYPE,*/
	int UseItemID;                       	/*  Ver.1491 �����е����:ʹ�ü�������һ����Ʒ */
	int Auto;                            	/*  Ver.1597 �Ƿ��ʺ��Զ�ʩ�� */
	int CtrlFlag;                        	/*  Ver.2529 λ����|SKILL_CTRL_FLAG */
};

/* ���＼����������|����� */
struct tagSkillUpRate_ToLua
{
	unsigned short Up;                             
	unsigned short Down;                           
	unsigned short NoChg;                          
};

struct tagDiathesisDef_ToLua
{
	unsigned short DiathesisID;                     	/*   ����ID��1��255 */
	unsigned short Level;                           	/*   ���ʵȼ� */
	char Name[RES_NAME_LEN];             
	int IconID;                          	/*  Ver.6 ͼ��id */
	SKILLRESULT DiathesisResult[MAX_DIATHESIS_RESULT];
	unsigned int ArmType;                         	/*   ������Ч���������� */
	char DiathesisDesc[DESC_LEN];        
	tagSkillLearn_ToLua Learn;                           	/*  Ver.45 ����ѧϰ��� */
	unsigned char LearnLevelLimit;                 	/*  Ver.70 ��������ѧ�ȼ����� */
	unsigned short PreUsedGeniusPoint;              	/*  Ver.182 �ѽ����ĵ��츳�� */
	unsigned short UseGeniusPoint;                  	/*  Ver.182 �������츳��Ҫ���ĵ��츳�� */
	unsigned short GeniusType;                      	/*  Ver.188 �츳������ ���츳Type�������1 */
	unsigned char PetDiathesisType;                	/*  Ver.141 ���＼��(ʵ��������)���� Bind Macrosgroup:PET_SKILL_TYPE,*/
	unsigned short SelfCool;                        	/*  Ver.160 ���Ҽ�����ȴֵ����λ100ms */
	unsigned char PetDiathesisGradeType;           	/*  Ver.164 ���＼��Ʒ�ʣ��߼����ͼ� Bind Macrosgroup:PET_SKILL_GRADE_TYPE,*/
	int IconPosition;                    	/*  Ver.246 ͼ����ʾλ��|0���ޣ�1����ʾ������״̬��2����ʾ������״̬�� */
	unsigned short MutexDiathesisID;                	/*  Ver.297 ������츳id,�ͻ�����츳ֻ�ܶ���ѡ��һ */
	unsigned char PetSkillType;                    	/*  Ver.1154 0-���ǳ��＼�� 1-������2-������ѧϰ */
	unsigned short GroupSkill;                      	/*  Ver.1288 ͬһ���Ӧ����ID,���ڳ���ѧϰ,���ü��� */
	tagSkillUpRate_ToLua SkillUpRate;                     	/*  Ver.1538  */
	short ResetRate;                       	/*  Ver.1538  */
};

struct tagDiathesisData_toLua
{
	unsigned short DiathesisID;                    
	unsigned char DiathesisLevel;     
	unsigned char GeniusFlag;                      	/*  Ver.184 1��ʾ������츳 */            
	unsigned char Res;                            
};

struct tagDiathesisInfo_toLua
{
	unsigned int DiathesisNum;                   
	tagDiathesisData_toLua DiathesisList[MAX_DIATHESIS];   
	unsigned short UsedGeniusPoint;                 	/*  Ver.182 �Ѿ�ʹ�ù��ܵ��츳�� */
	unsigned short LearnGeniusPoint;                	/*  Ver.182 ��ǰѧ�����츳�� */
};

#pragma pack()

//tolua_end
