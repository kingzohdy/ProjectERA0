#pragma once
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1)
struct tagCardBoxItem_ToLua
{
	int ItemID;                         
	int Num;                            
	int SelectRate;
	int AwardRate[MAX_GROUP_ITEM_AWARD_RATE_NUM];
	int Bind;                            	/*   0非绑定，1绑定 */
	int Broadcast;                       	/*   为1则抽到的时候广播 */
};

struct tagCardBoxGroupDef_ToLua
{
	int GroupID;                        
	tagCardBoxItem_ToLua Cards[MAX_CARD_BOX_GROUP_ITEM]; 
};

struct tagGroupItemRange_ToLua
{
	int Num;                            
	int Rate;                           
};

struct tagBoxGroupSelect_ToLua
{
	int GroupID;                        
	tagGroupItemRange_ToLua Range[MAX_CARD_BOX_ITEM_RANGE]; 
};

struct tagCardBoxDef_ToLua
{
	int ID;                             
	int GodGoin[MAX_CARD_BOX_AWARD_NUM];
	int GroupID;                        
	int AddGroupID;
	int LimitID;                        
	int LimitVal;                       
};
#pragma pack()
//tolua_end

class GameCardBox//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameCardBox>
{//tolua_export
public:
	GameCardBox(void);
	~GameCardBox(void);
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
	//tolua_begin
	void requestOpenCardBox( int nGridIdx, int nListType );
	void requestCloseCardBox();
	void requestWashCard();
	void requestSelectCard( int nGridIdx );
	void requestNextCard();
	const tagCardBoxDef_ToLua* getCardBoxDef( int id );
	//tolua_end
};//tolua_export