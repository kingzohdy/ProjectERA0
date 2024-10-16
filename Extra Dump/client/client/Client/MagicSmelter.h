#pragma once
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>
#include <vector>

//tolua_begin
#pragma pack(1)
struct tagRongluGrade_ToLua
{
	int MapID;                           	/*   �����ͼ */
	int KillNum;                         	/*   ɱ���� */
	int MonIDs[RONGLU_KILL_MON_MAX];     	/*   ��Щ�� */
	int IconIDs[RONGLU_ICONID_MAX];      	/*   ������Ʒ */
	int MapID1;                          	/*  Ver.1793 �����ͼ2 */
};

struct tagRongluDef_ToLua
{
	int ItemID;                         
	int Rate;                            	/*   ����� */
	char Pinzhi;                          	/*   0-��ɫ 1-�� 2-�� 3-�� 4-�� */
	tagRongluGrade_ToLua Grade[RONGLU_GRADE_MAX];        
};
#pragma pack()
//tolua_end

class MagicSmelterManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<MagicSmelterManager>
{//tolua_export
public:
	MagicSmelterManager(void);
	~MagicSmelterManager(void);
	//tolua_begin
	//��ʼ��¯����
	void requestRecastSmelter( int nGridIdx );
	//��������
	void endRecastSmelter( int nGridIdx, int id );
	void breakRecastSmelter();
	//��ȡ��¯�����
	tagRongluDef_ToLua* getRongluDef( int id );
	//����Ʒ�ʻ����¯id
	int getRongluIdByPinZhi( int nIndex );
	//���ջ����Ʒ��id
	int getRongluRecastEndID() { return m_nResultID; }
	//�����¯��Ӧ�ȼ���
	int getRongluLv() { return m_nGradeLv; }
	//������¯������Ч
	void playRongluRecastSound();
	//��������
	void initRongluInfo();
	//tolua_end
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
private:
	int m_nSmelterID;
	int m_nResultID;
	int m_nGradeLv;
	std::vector<tagRongluDef*> m_vecRongluData;
};//tolua_export