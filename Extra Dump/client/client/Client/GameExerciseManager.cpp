#include "GameExerciseManager.h"
#include "GameDBData.h"

GameExerciseManager::GameExerciseManager(void)
{

}
GameExerciseManager::~GameExerciseManager(void)
{

}

int GameExerciseManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	return 0;
}

void GameExerciseManager::sendBossChooseOption( bool bChosen )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_RONGYAN_BOSS_SELECT;
	if( bChosen )
	{
		msgbody.PlayerMiscClt.PlayerMiscCltData.RongYanBossSelect.iSelect = 1;
	}
	else
	{
		msgbody.PlayerMiscClt.PlayerMiscCltData.RongYanBossSelect.iSelect = 0;
	}
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameExerciseManager::sendBeginMultiplityNum( int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_RONGYAN_BEGIN_SELECT;
	msgbody.PlayerMiscClt.PlayerMiscCltData.RongYanBeginSelect.iBeginSelectMul = nNum;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameExerciseManager::CallHighLvRongYanMonster()
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_ZJ_RONGYAN_CALL;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameExerciseManager::sendHighLvBossChooseOption( bool bChosen )
{
	tagCSPkgBody msgbody;
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_ZJ_RONGYAN_BOSS_SELECT;
	if( bChosen )
	{
		msgbody.PlayerMiscClt.PlayerMiscCltData.ZJRongYanBossSelect.iSelect = 1;
	}
	else
	{
		msgbody.PlayerMiscClt.PlayerMiscCltData.ZJRongYanBossSelect.iSelect = 0;
	}
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

int GameExerciseManager::getRongYanMonsterBootyExp( int nID )
{
	const tagMonsterDef* pMonsteDef = DBDataManager::getSingleton().getMonsterDefPtr(nID);
	if( pMonsteDef == NULL )
	{
		return 0;
	}
	return pMonsteDef->Booty.Exp;
}


const tagRongYanJuShouDef_ToLua* GameExerciseManager::getRongYanJuShouDef( int nIndex )
{
	static_assert( sizeof(tagRongYanJuShouDef_ToLua) == sizeof(tagRongYanJuShouDef) );
	return (const tagRongYanJuShouDef_ToLua*) DBDataManager::getSingleton().getRongYanJuShouDef( nIndex );
}

const tagRongYanMonDynAttrDef_ToLua* GameExerciseManager::getRongYanMonAttrDef( int nLv )
{
	static_assert( sizeof(tagRongYanMonDynAttrDef_ToLua) == sizeof(tagRongYanMonDynAttrDef) );
	return (const tagRongYanMonDynAttrDef_ToLua*) DBDataManager::getSingleton().getRongYanMonAttrDef( nLv );
}