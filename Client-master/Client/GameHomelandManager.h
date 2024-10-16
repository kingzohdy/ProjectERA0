#if ( defined( _WIN32 ) || defined( _WIN64 ) ) && defined( _MSC_VER )
#pragma once
#endif

#ifndef GAME_HOMELAND_MANAGER_H
#define GAME_HOMELAND_MANAGER_H

#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "GameUIMsg.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1)
#define HOME_MAX_LIUYAN_TOLUA				HOME_MAX_LIUYAN
#define HOME_PAGE_MAX_TOLUA					HOME_PAGE_MAX
#define HOME_MAX_LIUYAN_TEXT_TOLAU		HOME_MAX_LIUYAN_TEXT
#define HOME_CLT_MAX_TOLUA                     HOME_CLT_MAX
#define HOME_MAX_JIAJU_TOLUA                   HOME_MAX_JIAJU
/* HOME_STAT*/
enum tagHOME_STAT_CLIENT
{
	HOME_STAT_OPEN_CLIENT		= HOME_STAT_OPEN, 
	HOME_STAT_CLOSE_CLIENT	= HOME_STAT_CLOSE, 
	HOME_STAT_SALE_CLIENT		= HOME_STAT_CLOSE_CLIENT + 1,
};

/* HOME_PET_OP*/
enum tagHOME_PET_OP_CLIENT
{
	HOME_PET_OP_EAT_CLIENT				= 1, 	/* 喂食 */
	HOME_PET_OP_CLAER_CLIENT			= 2, 	/* 清洁 */
	HOME_PET_OP_PLAY_CLIENT			= 3, 	/* 玩耍 */
	HOME_PET_OP_DUANLIAN_CLIENT = 4, 	/* 锻炼 */
	HOME_PET_OP_GET_CLIENT				= 5, 	/* 获取 */
	HOME_PET_OP_REPLACE_CLIENT		= 6, 	/* 更换宠物 */
};

struct tagHomePut_ToLua
{
	int GridIdx;                         	/*   杂物包第几个格子 */
	int Dir;                             	/*   摆放朝向 */
	tagResPos_ToLua Pos;                             	/*   摆放坐标 */
};
#pragma pack()
//tolua_end

#define HOMELAND_MAP_ID	26000

typedef struct tagPos_t {
	int x;
	int z;
} pos_t;

class GameHomelandManager//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameHomelandManager>
{//tolua_export
public:
	GameHomelandManager();
	~GameHomelandManager();

	int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
	//tolua_begin
	const tagHomeLvlDef_ToLua*			getHomeLvlDef( int nLvl );
	const tagHomeMonLvlDef_ToLua*	getHomeMonLvlDef( int nLvl );
	const tagHomeFarmDef_ToLua*		getHomeFarmDef( int nLvl );
	const tagHomeMonSegDef_ToLua*	getHomeMonSegDef( int nLvl );
	//client_2_server data
	const tagHomeSvrPage_ToLua*			getHomeSvrPage();
	const tagHomeSvrHouse_ToLua*			getHomeSvrHouse();
	const tagHomeSvrFarm_ToLua*			getHomeSvrFarm();
	//home pet
	const tagHomeMon_ToLua*					getHomePet();
	int														getHomePetID();
	void														displayHomelandPet( int x, int z );
	void														removeHomelandPet();
	bool														isPetExist();
	//home furniture
	bool														beginFurnitureMode( Item* item );
	bool														endFurnitureMode();
	void														beginFurnitureModeAlready( GameActor* pActor );
	void														endFurnitureModeAlready();
	bool														isFurnitureAlready();
	bool														isFurnitureMode();
	void														cancelFurnitureMode();
	void														cancelFurnitureModeAlready();
	bool														isInHouse();
	void														stepFurnitureAngle( int deltaAngle );
	void														stepFurnitureLocation( int dx, int dy );
	int														getFurnitureID();
	void														beginMoveMode();
	void														endMoveMode();
	bool														isMovingMode();
	void														selectFurniture();
	//home relative
	bool														isInHome();
	bool														isInSelfHome();
	//farm planting
	int														getFarmSoilIndex( GameActor* pActor );
	bool														beginPlantMode( Item* item );
	bool														planting( GameActor* pTarget, int nIndex = - 1);
	bool														endPlantMode( GameActor* pTarget, int nIndex = -1 );
	bool														isInPlantMode();
	void														cancelPlantMode();
	//status
	void														resetHomelandStatus();
	//HOME_CLT_OP
	void requestHomePage( int nPage );
	void requestHomeNew();
	void requestHomeSetName( int nID, const char* szName );
	void requestHomeSetStat( int nID, int nStat );
	void requestHomeEnter( int nID );
	void requestHomeMsgBoard( int nID, const char* szMsg );
	void requestHomeDestroy( int nID );
	void requestHomeSale( int nID, int nMoney );
	void requestHomeBuy( int nID );
	void requestHomeFurniturePut( int nGridIdx, int nDir, int nX, int nY );
	void requestHomeFurniturePutAlready( const tagJiaJuAdjustInfo &oldInfo, const tagJiaJuAdjustInfo &newInfo );
	void requestHomePlant( int nIdx, int nGridIdx );
	//HOME_PET_OP
	void requestHomePet( int nType, int nGridIdx = 0 );
	void petMoveTo( int to_x, int to_z );
	void setPetBound( int w, int h );
	void playPetAction( int id );
	//tolua_end

	void openHomePetFrame();
	void closeHomePetFrame();
	void closeHomelandFrames();
	void putFurnitureConfirm();

	void Update( unsigned int dtick );
	
private:
	tagHomeSvrPage				m_HomeSvrPage;
	tagHomeSvrHouse			m_HomeSvrHouse;
	tagHomeSvrFarm				m_HomeSvrFarm;
	tagHome							m_OwnerHome;
	GameActor*						m_GardenPet;
	int									m_PetIndex;
	int									m_nPetID;
	pos_t								m_moveBound;
	pos_t								m_moveCenter;
	int									m_furnitureID;
	GameActor*						m_furnitureMoster;
	bool									m_isfurnitureAlready;
	bool									m_isMoving;
	tagJiaJuAdjustInfo			m_oldFurnitureInfo;
	Item*								m_FurnitureItem;
	Item*								m_plantItem;
	bool									m_isPlanting;
	HOMEPOS						m_houseScope;

	void UpdatePageInfo( const HOMESVRNAMECHG& nameChg );
	void UpdatePageInfo( const HOMESVRSINGLE& single );
	void UpdatePageInfo( const HOMESVRSTATCHG& stat );
	void UpdatePageInfo( const HOMESVRLIUYANCHG& message );
	void UpdatePageInfo( const HOMESVRSALECHG& sale );
	void UpdatePageInfo( const HOMESVRBUY& buy );
	void UpdatePageInfo( const HOMESVRDEL& del );
	void setOwnerHome( const tagHome& home );
};//tolua_export

#endif