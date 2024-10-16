#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameDBData.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1) 
struct tagSubJectSelf_ToLua
{
	char Succ;                            	/*   ����� */
    char Star;                            	/*   ���֮������ */
    unsigned short Top;                             	/*   �Լ����� */
	int Integ;                           	/*   �Լ����� */
	int CurrIdx;                         	/*  Ver.1542 ����0��ʾ��ǰ���Ѿ������,ֵΪ��Ҵ�����|��1��ʼ */
};

struct tagSubJectTopOne_ToLua
{
	unsigned short Top;                            
	int Integ;                           	/*   ���� */
	char RoleName[ROLE_NAME_LEN_SAVE];    	/*   ��ɫ�� */
};

struct tagSubJectTop_ToLua
{
	int Num;                            
	tagSubJectTopOne_ToLua Tops[SUBJECT_MAX_TOP];          
};

struct tagSubJectPloy_ToLua
{
	char Already;                         	/*   �������Ѿ�������,�ڼ����� */
	char Odd;                             	/*   ������ʣ������� */
	int ID;                              	/*   ���ID */
	int ReqEnd;                          	/*   �������ʱ��|����0��ʾҪ��ʾ��ʼ��������ʱ */
	int ResEnd;                          	/*   �������ʱ�� */
	int LockEnd;                         	/*   �������ʱ�� */
};

struct tagSubJectAward_ToLua
{
	int Top;                             	/*   �������� */
	int Base;                            	/*   �������� */
	int Add;                             	/*   ���⽱�� */
};

struct tagSubJectFlush_ToLua
{
	tagSubJectSelf_ToLua Self;                            	/*   �Լ�����Ϣ */
	tagSubJectTop_ToLua Top;                             	/*   ǰ20������Ϣ */
	tagSubJectPloy_ToLua Ploy;                            	/*   ������Ϣ */
	tagSubJectAward_ToLua Award;                           	/*   ������Ϣ|�˽ṹ�κ�һ�ֶ���ֵ��ʾ����� */
};
struct tagSubJectCommit_ToLua
{
	int ID;                              	/*   ���ID,������������֤ */
	int Idx;                             	/*   �ڼ�����,������������֤ */
	int KeyIdx;                          	/*   ������0-3  */
	int IsStar;                          	/*   �Ƿ�ʹ�����֮��1-�� */
};
struct tagSubJectKey_ToLua
{
	char Str[MAX_SUBJECT_KEY_LEN];       
};

struct tagSubJectDef_ToLua
{
	unsigned int ID;                             
	char SubJect[MAX_SUBJECT_LEN];        				/*   ��Ŀ */
	tagSubJectKey_ToLua SubJectKey[MAX_SUBJECT_KEY];    /*   4���� */
	int Succ;                            				/*   ��ȷ������|��1��ʼ */
	int Type;                            				/*  Ver.1534 ���� */
};
#pragma pack()
const tagSubJectDef_ToLua*  getSubjectDef(int id);
//tolua_end

class GameIntelliRaceMgr //tolua_export
	: public NetMsgHandler,public Ogre::Singleton<GameIntelliRaceMgr>
{//tolua_export
public:
	GameIntelliRaceMgr(void);
	~GameIntelliRaceMgr(void);
	//tolua_begin
	void IntelligenceRaceJoinAgree();
	void IntelliRaceAnswerCommit();
	void IntelliRaceAnswerQuit();
	tagSubJectFlush_ToLua* getSubJectFlushInfo();
	tagSubJectCommit_ToLua* getSubjectCommitInfo();
	
	//tolua_end
	
	tagSubJectCommit_ToLua	m_SubJectCommitInfo;

public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
private:
	tagSubJectFlush		m_SubjectFlush;

};//tolua_export