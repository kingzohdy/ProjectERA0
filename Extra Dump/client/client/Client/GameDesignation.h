#pragma once
#include "NetMessage.h"
#include "cs_net.h"
#include <map>
#include <vector>

//tolua_begin 
struct tagDesignationDef_ToLua
{
	unsigned int ID;						/*   �ƺ�ID */
	char Name[SHORT_DESC_LEN];            	/*   �ƺ��ı� */
	char CategoryName[SHORT_DESC_LEN];    	/*   �ƺ���� */
	unsigned int StatusID;					/*   ״̬ID */
	unsigned int StatusLevel;				/*   ״̬�ȼ� */
	int Time;                            	/*   ״̬����ʱ�� */
    char DescText[SHORT_DESC_LEN];        	/*   �Դ˳ƺŵ����� */
    int VimCost;                         	/*  Ver.224 ���ĵľ���ֵ */
	int DesTime;                         	/*  Ver.1523 �ƺŴ��ʱ��0-����,��λ:�� */
	char Level;                           	/*  Ver.1524 �ƺ�Ʒ�� */
	char NotShow;                         	/*  Ver.1525 1-û�л�ȡ�˳ƺ�ʱ������в���ʾ */
	unsigned int RGB;                       /*  Ver.1527  */
	char EffectPath[DESIG_EFFECT_LEN];    	/*  Ver.1527  */
};
//tolua_end 

typedef std::map<std::string,std::vector<tagDesignationDef*>>	MAPDESIGNATIONDEF;
typedef std::vector<tagDesignationDef*>							VECTDESIGNATIONDEF;
typedef std::vector<DESIGNATIONENTRY>							VECTHAVEDESIGNATION;

class GameDesignation//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameDesignation>
{//tolua_export
public:
	GameDesignation(void);
	~GameDesignation(void);
	
	//tolua_begin 
	// ��������������óƺ�����
	void sendSetDesignationReq( int nId );
	//tolua_end
	
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

public:
	//tolua_begin 
	// ��ȡȫ���ĳƺ�����
	int getDesignationNum();
	// �����ͻ�ȡ�ƺŵ���������
	int getDesignationNum( std::string strCategoryName );
	// ��ȡ��ǰʹ�õĳƺ�ID
	int getCurDesignationId(){ return m_nCurDesignationId; }
	// ��ȡ�ƺ�
	tagDesignationDef_ToLua* getDesignation( int nIndex );
	// ��ȡ�ƺ�
    tagDesignationDef_ToLua* getDesignation( std::string strCategory, int nIndex );
    // ���ݳƺ�ID��ȡ�ƺ���Ϣ
    tagDesignationDef_ToLua* getDesignationById( int nId );
	// ��ȡ�ƺ�״̬����
	const char* getDesignationBufName( int nBufId, int nBufLv );

	// �Ƿ����״����óƺ�
	bool isFirstSetDesignation( int nId );
	// �Ƿ���ӵ�еĳƺ�
	bool isHaveDesignation( int nId  );
	// ��ȡ�ƺŵ���ʱ��
	int getDesignationEndTime( int nId );

	int GetDesignationMapSize();

	const char* GetDesignationCateName( int idx );
	//tolua_end 

public:
	void setActorDesignation( int nDesignationId, GameActor* pActor );
	void sortDesignation( VECTDESIGNATIONDEF& vectDesignationDef );
	void sortAllDesignation();

	// �Ƿ���ʱ��
	bool isToday( int nTime );

private:
	static bool compareDesignations( const tagDesignationDef* lhs, const tagDesignationDef* rhs );

private:
	MAPDESIGNATIONDEF				m_mapDesignationDef;	// �����ŵĳƺ�����
	VECTDESIGNATIONDEF				m_vectDesignationDef;	// ȫ���ĳƺ�����
	VECTHAVEDESIGNATION				m_vectHaveDesignation;  // ��ӵ�еĳƺ�
	int                             m_nCurDesignationId;    // ��ҵ�ǰ�ƺ�ID
};//tolua_export
