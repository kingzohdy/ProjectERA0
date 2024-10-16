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
    unsigned int ID;                        /*   声望阵营ID */
    char PartyName[SHORT_DESC_LEN];       	/*   阵营名称 */
    char CategoryName[SHORT_DESC_LEN];    	/*   阵营类别 */
    char DescText[SHORT_DESC_LEN];        	/*   对此阵营的描述 */
};

struct tagRespactPhaseDef_ToLua
{
    unsigned int Phase;                           	/*   目前声望所达到的程度 */
    char PhaseText[SHORT_DESC_LEN];       	/*   本阶段的名称 */
    unsigned int NeedRespact;                     	/*   需要达到的声望点数 */
    int NotifyWorld;                     	/*   达到此阶段是否需要通知全服务器 */
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
    // 获取声望的总类型数
    int getRespactCategoryNum(){ return (int)m_mapRespactParty.size(); }
    // 获取指定类别的阵营数量
    int getRespactPartyNum( int nId );
    // 获取声望总的阵营数量
    int getRespactPartyNum();
    // 获取声望总的阶段数量
    int getRespactPhaseNum(){ return (int)m_vectRespactPhase.size(); }
    // 获取声望的类型ID
    int getRespactCategoryId( int nIndex );
    
	// 获取声望有类型名
	const char* getRespactCategoryName( int nId );
    // 获取声望值
    int getRespactValue( int nId );
    // 获取声望阶段数据
    tagRespactPhaseDef_ToLua* getRespactPhaseDef( int nIndex );
    // 获取声望阵营
    tagRespactPartyDef_ToLua* getRespactPartyDef( int nId );
    // 获取指定类别的阵营
    tagRespactPartyDef_ToLua* getRespactPartyDef( int nId, int nIndex );
 
    // 是否是已拥有的声望
    bool isHaveRespact( int nId ); 
    
    void clearRespactEntry();  
    //tolua_end 

public:
	VECTRESPACTENTRY    m_vectRespactEntry; // 声望类型数据

private:
    VECTRESPACTPHASE    m_vectRespactPhase; // 声望阶段数据
    MAPRESPACTPARTY     m_mapRespactParty;  // 声望阵营数据
};//tolua_export
