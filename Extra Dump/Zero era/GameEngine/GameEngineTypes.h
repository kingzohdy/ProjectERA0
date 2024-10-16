
#ifndef __GameEngineTypes_H__
#define __GameEngineTypes_H__

#include "OgrePrerequisites.h"
#include "OgreWorldPos.h"

#pragma warning( disable:4819 )

namespace Ogre
{
	class GameMapRegion;
	class GameMapRegionSet;
	class GameSkillManager;
	class ShowGameMap;
	class ShowActor;
	class AvatarEntity;
	class MCIWrap;
}

class GameActor;
class ActorManager;
class DBDataManager;
class FrameTick;
class FollowCamera;
class GamePlayManager;
class ISpellControler;
class GameSpell_Control;
class GameMap;
class GameMaskPath;
class GameMaskMap;
class GameMapMover;
class GameUI;
class INetworkManager;
class LoginManager;
class ChatManager;
class MainGamePlayer;
class SkillManager;
class GameSpell;
class IGameMsgPrint;
class GameMsgPrint;
class GameUIMsg;
class GameShortCut;
class GameSelectManager;
class GameMapShower;
class CQuadTree;
class GameRuneManager;
class Item;
class Equip;
class Container;
class GameNpcDialog;
class GameActionManager;
class GameTeamManager;
class AutoTeamManager;
class GameSkill_Package;
struct tagSkillDyAttrib;
class SkillHit;
class GameNpcShop;
class GameQuest;
class GameNPC;
class GameWizard;
class GameCollect;
class GameSkillLearn;
class GameFriendManager;
class GameExchange;
class GameDynPWorld;
class GameActor_Tick;
class GameClanManager;
class GameTrust;
class GameMailManager;
class GameStallManager;
class GameArmProce;
class IAutoFind;
union tagCSPkgBody;
class GameExplorerWin;
class GamePromotion;
class GameWatchPlayer;
class GameRespact;
class GameDesignation;
class CMarketManager;
class GameActor_PVP;

class GameUserConfig;
class ResoucePreLoadManager;
class GameRankManager;
class SecondarySkillMgr;
class GameBattle;
class GameWarfogManager;
class GameRollManager;
class GameSimulateManager;
class GameMapSendingManager;
class DreamLandManager;
class AutoAssist;
class GamePetLand;
class GameClan_BaseWar;
class GameIntelliRaceMgr;
class AutoPworldManager;
class MagicSmelterManager;
class GameGetAccountAllRole;
class GameGeniusManager;
class GameBusyManager;
class GamePoly;
class GameVipManager;
class GameClanBoon;
class GameSparta;
class GameSchool;
class GameCardBox;
class GameStoreManager;
class GameExerciseManager;
class ExtraActorMgr;
class GameTopCareer;
class GameClan_SpanBaseWar;
class GameHomelandManager;
class GameShiKong;

class NetMsgHandler
{
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody) = 0;
};

class NetDisConnectHandler
{
public:
	virtual void DisConnectHandler( const int nReslut ) = 0;
};
#define MAX_AVATAR_FIELD 23
#define GAME_PWORLD_MAPID 10000

enum
{
	RESDB_DB = 0,
	RESDB_FILE
};

enum ShorCutType
{
	SCT_NONE = 0,
	SCT_SKILL,
	SCT_ITEM,
	SCT_RIDE,
	SCT_FAIRYSKILL,
};

struct DirData
{
	char dir_ip[128];
	int dir_port;
};

//dir server 最大的数量
#define MAX_DIR_SERV_NUM 10

struct NetConfigData
{
	char dir_server[128];
	int dir_port;
	DirData dir_data[MAX_DIR_SERV_NUM];
	int num;
};

struct GameConfig
{
	bool win_fullscreen;
	bool win_title; // 标题显示 2012.04.09  , false show all information 
	int win_width;
	int win_height;

	int resdb_mode;
	char resdb_dir[128];
	char resdb_server[128];
	char resdb_user[32];
	char resdb_passwd[32];
	char resdb_database[32];

	NetConfigData dir_iplist[MAX_DIR_SERV_NUM];
	int dir_num;
	char dir_url[255];
	int clt_type;
};

union TypePointer{
	unsigned int*		dw;
	unsigned short*		wd;
	char*				b;
	char*				c;
	void*				p;
	float*				f;
	int*				l;
	TypePointer(void* in) :p(in){}
};

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }

#define JION(x, y) XJION(x, y)
#define XJION(x, y) x ## y
#define static_assert(e) typedef char JION(assertion_failed_at_line_, __LINE__) [(e) ? 1 : -1]

typedef Ogre::WorldPos GameXYZ;

extern bool LoadGameConfigFromXML(GameConfig &config, Ogre::XMLData *pxmldata);
// 获取客户端版本号
extern unsigned int getLocalVersion();

class PairFirstGreater
{
public:
	template<class F, class S>
	bool operator () (const std::pair<F, S> &a, const std::pair<F, S> &b)
	{
		return a.first > b.first;
	}
};

enum eChantType
{
	eUseItem_Chant,		//使用物品吟唱
	eRide_Chant,		//坐骑吟唱
	eTrump_Chant,		//精灵吟唱
	ePet_Chant,			//宠物吟唱
	eResolve_Chant,		//分解吟唱
	eSecondary_Chant,	//生活技能吟唱
};

#endif
