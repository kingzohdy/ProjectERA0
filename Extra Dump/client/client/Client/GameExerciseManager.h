#pragma once
#include "NetMessage.h"
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1)
struct tagRongYan_ToLua
{
	int ExpMul;                          	/*   ���鱶�� */
	int ItemNum;                         	/*   ���Һ��Ķ��ٸ� */
};

struct tagRongYanJuShouDef_ToLua
{
	int Level;                           	/*   ���� */
	int MonID;                           	/*   ����id */
	int MonLifeTime;                     	/*   ��������ʱ��(��λ��) */
	int Odds;                            	/*   ������boss�ļ��� */
	tagRongYan_ToLua RongYan[MAX_RONGYAN_BEISHU];    
};

struct tagRongYanMonDynAttrDef_ToLua
{
	unsigned int Level;                           	/*   ƽ������ */
	short IceDefMul;                       	/*   �����Է��������� */
	short PhsicDefMul;                     	/*   �������Է��������� */
	short HitMul;                          	/*   ������ */
	short FleeMul;                         	/*   ������ */
	short DynHPMul;                        	/*   Ѫ������ */
	short DynHurtMul;                      	/*   ����˺����� */
	short IceHurtMul;                      	/*   ���˺����� */
	short PhsicHurtMul;                    	/*   ���˺����� */
	short IceHurtDefMul;                   	/*   �����Կ������� */
	short PhsicHurtDefMul;                 	/*   �������Կ������� */
	short ElementAtkMul;                   	/*   Ԫ�ع������� */
	short ElementDefMul;                   	/*   Ԫ�ط������� */
	short DynExpMul;                       	/*   ��þ������� */
};

#pragma pack()
#define CLT_RONG_YAN_ITEM_ID			RONGYAN_HAIXIN_ITEM_ID
#define ZJ_RONGYAN_ITEM_ID_TOLUA		ZJ_RONGYAN_ITEM_ID
//tolua_end

class GameExerciseManager  //tolua_export
	: public NetMsgHandler,public Ogre::Singleton<GameExerciseManager>
{//tolua_export
public:
	GameExerciseManager(void);
	~GameExerciseManager(void);
	//tolua_begin

    /* ���Ҿ��޲���*/
	void sendBossChooseOption( bool bChosen );				// ���boss�Ƿ�ѡ��
	int  getRongYanMonsterBootyExp( int nID );				// ��û�������ֵ
	void sendBeginMultiplityNum( int nNum );				// ѡ�������� 1-4

	//�߼����Ҿ���
	void CallHighLvRongYanMonster();
	void sendHighLvBossChooseOption( bool bChosen );				// �ռ����ң����boss�Ƿ�ѡ��
	
	const tagRongYanJuShouDef_ToLua* getRongYanJuShouDef( int nIndex );
	const tagRongYanMonDynAttrDef_ToLua* getRongYanMonAttrDef( int nLv );
	//tolua_end
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
};//tolua_export	