#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDefineTolua.h"
#include "GameWizard.h"

#pragma pack(1)
//tolua_begin
struct tagRStrongOwner_ToLua
{
	int SvrID;                          
	int ClanID;                         
	int OccTime;                        
	char ClanName[NAME_LEN];             
};
struct tagRSaleOne_ToLua
{
	int SvrID;                          
	int ClanID;                         
	int Sale;                           
	char ClanName[NAME_LEN];             
};
struct tagRSalePlayerOne_ToLua
{
	int SvrID;                          
	int Sale;                           
	int ClanID;                         
	char RoleName[NAME_LEN];             
};
struct tagRStrongSale_ToLua
{
	int iSale;                           	/*   ����ǰ���� */
	tagRSaleOne_ToLua RSale[MAX_RSALE_TOP];           
	int iPlayer;                         	/*   ������,Ҫ��Ǯ�� */
	tagRSalePlayerOne_ToLua RPlayer[MAX_RSALE_NUM];         
};
struct tagSpanStrongMon_ToLua
{
	int Flag;                            	/*   1-��ʾ��������Դ�� */
	int MemID;                          
	int Camp;                           /* 1-�ط� 2-����*/
	int X;                              
	int Y;                              
};
struct tagRStrongPworld_ToLua
{
	int InstID;                         
	int DefRes;                         
	int AtkRes;                         
	int MonNum;                         
	tagSpanStrongMon_ToLua Mon[MAX_SPAN_STRONG_RES_MON];   
	tdr_ulonglong PworldWid;                       	/*  Ver.2410  */
};
struct tagRStrongOne_ToLua
{
	char Stat;                           
	char IsOpen;                         
	int StrongID;                       
	tagRStrongOwner_ToLua Owner;                           	/*   �ݵ������ */
	tagRStrongSale_ToLua Sale;                           
	tagRStrongPworld_ToLua Pworld;                         
	tagRSaleOne_ToLua Atk;                             	/*  Ver.2410 ������ */
};
struct tagResMon_ToLua
{
	int MonID;                          
	int DefMonID;                        	/*  Ver.1592  */
	int AtkMonID;                        	/*  Ver.1592  */
	tagResPos_ToLua Pos;                            
};

struct tagMonPos_ToLua
{
	int MonID;                          
	tagResPos_ToLua Pos;                            
};
struct tagGiftMoney_ToLua
{
	int Type;                           
	int Num;                            
};
struct tagStrongAward_ToLua
{
	int Exp;                             	/*  Ver.1868  */
	tagGiftItem_ToLua Item[MAX_STRONG_ITEM];          
	tagGiftMoney_ToLua Money[MAX_STRONG_MONEY];        
};
struct tagWorldAward_ToLua
{
	int Type;                            	/*   1-����2-���� 3-��� */
	int Num;                            
};
struct tagKillBuff_ToLua
{
	int KillNum;                        
	int BuffID;                         
	int BuffLvl;                        
	int BuffTime;                       
};

struct tagDieBuff_ToLua
{
	int DieNum;                         
	int BuffID;                         
	int BuffLvl;                        
	int BuffTime;                       
};
/* ս���ݵ㶨��� */
struct tagSpanStrongDef_ToLua
{
	int StrongID;                        	/*   �ݵ�ID */
	char Lvl;                             	/*   �ݵ�ȼ� */
	int PworldID;                        	/*   ��Ӧ�ĸ���ID */
	int BaseUse;                         	/*   ���ĵ׼� */
	char Name[RES_NAME_LEN];             
	tagResMon_ToLua ResMon[MAX_RES_MON];             	/*   4����Դ�� */
	int ResLimit;                        	/*   ��Դ���� */
	tagMonPos_ToLua ReviveMonID[MAX_STRONG_REVIVE_MON]; 	/*   ����������0-���ط� 1-������ */
	tagStrongAward_ToLua WinAward;                       
	tagStrongAward_ToLua FailAward;            
	tagWorldAward_ToLua WorldAward[MAX_WORLD_AWARD_TYPE]; 	/*  Ver.2443  */
	tagKillBuff_ToLua KillBuff[MAX_STRONG_KILL_BUFF];  	/*  Ver.2457  */
	tagDieBuff_ToLua DieBuff;                         	/*  Ver.2457 ��Ϯbuff */  
};
struct tagCsSpanStrongWarOne_ToLua
{
	char RoleName[NAME_LEN];             
	char Career;                         
	unsigned short ResGet;                         
	unsigned short CountKill;                      
	unsigned short MaxContinueKill;                
	unsigned short AssitNum;                       
	unsigned short CurrContinueKill;               
	int MapID;                          
	int Die;                            
	unsigned int EnterTime;                      
	char ClanName[NAME_LEN];             
	int Camp;                            	/*  Ver.2426   1-�ط� 2-����*/
};
struct tagCsSpanStrongPworld_ToLua
{
	int DefRes;                         
	int AtkRes;                         
	int DefSvrID;                       
	int AtkSvrID;                       
	char DefClanName[NAME_LEN];          
	char AtkClanName[NAME_LEN];          
	int MonNum;                         
	tagSpanStrongMon_ToLua Mon[MAX_SPAN_STRONG_RES_MON];   
};
//tolua_end
#pragma pack()

class GameClan_SpanBaseWar//tolua_export
	:public NetMsgHandler, public IGameSpanStrong, public Ogre::Singleton<GameClan_SpanBaseWar>
{//tolua_export
public:
	GameClan_SpanBaseWar(void);
	~GameClan_SpanBaseWar(void);
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	virtual void	objEnterView(GameActor *actor);

	//tolua_begin
	//c->s
	void SpanStrongSale( int nID, int nMoney );
	void SpanStrongEnter( int nID );
	void ReqSpanStrongInfo();
	void ReqSelectGroup( int nStrongID, int nGroupID );
	void ReqSpanStrongAward( int nStrongID );

	tagRStrongOne_ToLua getSpanStrongInfoByIndex( int nIdx );			//��ȡ����ݵ���Ϣ
	tagSpanStrongDef_ToLua getSpanStrongDef( int ID );						//��ȡ����ݵ㶨��

	tagCsSpanStrongWarOne_ToLua	getSpanStrongPlayerInfo( int nIdx );			//��ȡ���ս���������Ϣ
	int	getSpanStrongPlayerNum();																				//��ȡ���ս���������Ŀ
	int	getSpanStrongSelfContinueKill();																	//��ȡ���ս���Լ�����ɱ��

	tagCsSpanStrongPworld_ToLua getSpanStrongWarPworldInfo();					//��ȡ��ǰ���ս����Ϣ

	int getSpanStrongOwnerExpVal();																			//��ȡռ���־���ӳ�
	int getSpanStrongOwnerBindGold();																		//��ȡռ���ð����
	int getSpanStrongOwnerBull();																					//��ȡռ���ð�����

	void loadSpanStrongPworldID();																				//load���п���ݵ��PworldID
	bool isInSpanStrongPworld();																				//�Ƿ��ڿ��ս����

	//tolua_end
private:
	int												m_nSpanPworldID[MAX_SPAN_STRONG_NUM];
	tagRStrongInfo						mAllStrongInfo;										//���п���ݵ���Ϣ
	tagCsSpanStrongWarAll			mWarAllPlayerInfo;									//����ݵ�ս���У����ս����Ϣ
	tagCsSpanStrongPworld		mSpanStrongPwolrd;								//��ǰ��������Ŀ���ݵ�Ĺ�����Ϣ
	tagCsSpanStrongWarOne		mSpanStrongSelfInfo;								//����ݵ�ս���У��Լ���Ϣ
	tagCsSpanStrongOcc				m_SpanStrongOccInfo;							//����ݵ㼰ʱ֪ͨ��ռ����Ϣ
};//tolua_export
