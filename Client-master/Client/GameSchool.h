#pragma once

#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreSingleton.h"
#include "GameDefineToLua.h"

//tolua_begin
struct tagRoleTeacherInfo_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[ROLE_NAME_LEN_SAVE];   
	unsigned char Level;                          
	unsigned char Head;                           
	unsigned char Face;                           
	unsigned char Hair;                           
	char Gender;                         
	char Career;                         
	int JoinLevel;                      
	int JoinTime;                       
	int MeiLi;                          
	int Online;                         
	int Map;                            
	int TeamNum;              
	char ClanName[NAME_LEN];              	/*  Ver.2091 �������� */              
};
struct tagRoleStudent_ToLua
{
	tagRoleTeacherInfo_ToLua Teacher;                        
	int Exp;                             	/*   ��ǰδ��ȡ�ľ���ֵ */
	int Val;                             	/*   ��ȡ�����ʦ���ӵ����� */
	int AwardCount;                     
	int TotalCount;                     
};

struct tagRoleStudentInfo_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[ROLE_NAME_LEN_SAVE];   
	unsigned char Level;                          
	unsigned char Head;                           
	unsigned char Face;                           
	unsigned char Hair;                           
	char Gender;                         
	char Career;                         
	int JoinLevel;                      
	int DayTime;                        
	int JoinTime;                       
	int MeiLi;                          
	int Online;                         
	int Map;                            
	int TeamNum;                    
	char ClanName[NAME_LEN];              	/*  Ver.2091 �������� */
};
struct tagSchoolXinDeDef_ToLua
{
    int Level;                          
    int AddExp;                         
    int SchoolVal;                      
};
struct tagSchoolAddStudent_ToLua
{
	tagLong_ToLua RoleID;                         
	char RoleName[ROLE_NAME_LEN_SAVE];   
	tagLong_ToLua TeacherRoleID;                  
	char TeacherRoleName[ROLE_NAME_LEN_SAVE];
};
/* ROLE_SCHOOL_TYPE_TOLUA*/
enum tagROLE_SCHOOL_TYPE_TOLUA 
{
	ROLE_SCHOOL_TYPE_NONE_TOLUA = 0, 
	ROLE_SCHOOL_TYPE_TEACHER_TOLUA = 1, 
	ROLE_SCHOOL_TYPE_STUDENT_TOLUA = 2, 
};
#define MAX_ROLE_STUDENT_TOLUA MAX_ROLE_STUDENT
#define SCHOOL_DEL_MONEY_NUM_TOLUA SCHOOL_DEL_MONEY_NUM 	/* ���ʦͽ��ϵ��Ҫ�۳��İ����� */
//tolua_end

class GameSchool //tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameSchool>
{ //tolua_export
public:
	GameSchool();
	~GameSchool();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	void SetRegInfo( tagCSSvrRegInfo regInfo );
	//tolua_begin
	void InviteToSchool( const char* szName );				//�������ʦ��
	void AcceptToSchool( const char* szName );				//���ܼ���ʦ������
	void RejectToSchool( int nFlag, const char* szName );	//�ܾ�����ʦ��
	void RequestMemInfo( const char* szName );				//����ѧ�������ĳ��ѧ������Ϣ
	void DelSchool( int nHeight, int nLow, const char* szName );	//���ʦͽ��ϵ
	void SchoolXinDe( int nHeight, int nLow, const char* szName );	//�����ĵ�
	void AwardSchoolXinDe( int nHeight, int nLow );			//��ȡ�ĵ�

	void CharmRequest( const char charmFlag, const char charmType, const char* szAcceptName, int nNum );			//�ͻ�

	void RegSchool( const char* szMsg );					//�Ǽ�ʱ������

	int GetRegStudentsNum();					//��ȡ��ǰ�Ǽ����ߵ������Ŀ
	const char* GetRegStudentName( int nIdx );			//����������ȡ��ǰ�Ǽ����ߵ��������
	int GetRegStudentLevel( int nIdx );					//����������ȡ��ǰ�Ǽ����ߵ���ҵȼ�
	int GetRegStudentRace( int nIdx );					//����������ȡ��ǰ�Ǽ����ߵ����ְҵ
	const char* GetRegStudentClanName( int nIdx );		//����������ȡ��ǰ�Ǽ����ߵ���ҹ�������
	const char* GetRegStudentMsg( int nIdx );			//����������ȡ��ǰ�Ǽ����ߵ��������

	int GetSelfSchoolJobType(){ return m_SelfSchoolJobType;}
	int GetSelfStudentNum(){ return m_SelfStudentNum;}
	tagRoleStudentInfo_ToLua GetSelfStudentByIndex( int index );
	tagRoleStudentInfo_ToLua GetSelfStudent( const char* szName );
	tagRoleTeacherInfo_ToLua GetSelfTeacher();
	int GetSelfTeacherGainedCount();					//��ȡ�Լ���ʦΪ�Լ�������ĵô���
	int GetSelfAwardTeacherGainedCount();				//��ȡ�Լ���ȡ��ʦ�ĵô���
	int GetSelfCanAwardTeacherGainedExp();				//��ȡ��ǰ������ȡ�ľ���ֵ
	const tagSchoolXinDeDef_ToLua* GetSchoolXinDeDef( int nLevel );

	bool IsSelfSchool( const char* szName );			//����Ƿ����Լ���ʦ����ϵ

	void ChangeSchoolName( const char* szOldName, const char* szNewName );
	//tolua_end
private:
	tagCSSvrRegInfo m_RegInfo;					//ע��ѧ����Ϣ
	int	m_SelfSchoolJobType;					//��ǰʦͽ��ϵ�е�ְ��ROLE_SCHOOL_TYPE_TOLUA
	int	m_SelfStudentNum;						//�Լ�ѧ������
	tagRoleStudentInfo_ToLua		m_SelfStudents[MAX_ROLE_STUDENT];		//�Լ���ѧ����Ϣ
	tagRoleStudent_ToLua			m_SelfTeacher;							//�Լ�����ʦ��Ϣ

	tagSchoolAddStudent_ToLua		m_SchoolAddStudent;						//�ɹ�����ʦͽ
}; //tolua_export