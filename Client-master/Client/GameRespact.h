#pragma once
#include "NetMessage.h"
#include "cs_net.h"
#include <map>
#include <vector>

typedef std::map<int,std::vector<tagRespactPartyDef*>>				MAPRESPACTPARTY;
typedef std::vector<tagRespactPhaseDef*>                            VECTRESPACTPHASE;
typedef std::vector<RESPACTENTRY>                                   VECTRESPACTENTRY;

//tolua_begin 
struct tagRespactPartyDef_ToLua
{
    unsigned int ID;                        /*   ������ӪID */
    char PartyName[SHORT_DESC_LEN];       	/*   ��Ӫ���� */
    char CategoryName[SHORT_DESC_LEN];    	/*   ��Ӫ��� */
    char DescText[SHORT_DESC_LEN];        	/*   �Դ���Ӫ������ */
};

struct tagRespactPhaseDef_ToLua
{
    unsigned int Phase;                           	/*   Ŀǰ�������ﵽ�ĳ̶� */
    char PhaseText[SHORT_DESC_LEN];       	/*   ���׶ε����� */
    unsigned int NeedRespact;                     	/*   ��Ҫ�ﵽ���������� */
    int NotifyWorld;                     	/*   �ﵽ�˽׶��Ƿ���Ҫ֪ͨȫ������ */
};
//tolua_end

class GameRespact//tolua_export
    : public NetMsgHandler, public Ogre::Singleton<GameRespact>
{//tolua_export
public:
	GameRespact(void);
    ~GameRespact(void);

    //s->c
    virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

public:
    //tolua_begin 
    // ��ȡ��������������
    int getRespactCategoryNum(){ return (int)m_mapRespactParty.size(); }
    // ��ȡָ��������Ӫ����
    int getRespactPartyNum( int nId );
    // ��ȡ�����ܵ���Ӫ����
    int getRespactPartyNum();
    // ��ȡ�����ܵĽ׶�����
    int getRespactPhaseNum(){ return (int)m_vectRespactPhase.size(); }
    // ��ȡ����������ID
    int getRespactCategoryId( int nIndex );
    
	// ��ȡ������������
	const char* getRespactCategoryName( int nId );
    // ��ȡ����ֵ
    int getRespactValue( int nId );
    // ��ȡ�����׶�����
    tagRespactPhaseDef_ToLua* getRespactPhaseDef( int nIndex );
    // ��ȡ������Ӫ
    tagRespactPartyDef_ToLua* getRespactPartyDef( int nId );
    // ��ȡָ��������Ӫ
    tagRespactPartyDef_ToLua* getRespactPartyDef( int nId, int nIndex );
 
    // �Ƿ�����ӵ�е�����
    bool isHaveRespact( int nId ); 
    
    void clearRespactEntry();  
    //tolua_end 

public:
	VECTRESPACTENTRY    m_vectRespactEntry; // ������������

private:
    VECTRESPACTPHASE    m_vectRespactPhase; // �����׶�����
    MAPRESPACTPARTY     m_mapRespactParty;  // ������Ӫ����
};//tolua_export
