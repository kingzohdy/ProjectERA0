#include "GameNpcDialog.h"
#include "GameUIMsg.h"
#include "GameManager.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameCamera.h"
#include "GamePlayManager.h"
#include "ServerTimer.h"
#include "GameNpc.h"
#include "GameMaxMap.h"
#include "ShowActor.h"
#include "GameMaskMap.h"
#include "GameMap.h"
#include "GameChatManager.h"
#include "OgreScriptLuaVM.h"
#include "GameHomelandManager.h"

const int TALK_DIS = OP_DIST;

GameNpcDialog::GameNpcDialog(void) : m_bCath(false), m_nNpcResId(0), m_nNpcMemId(0), m_PushCamera(false), m_lastNpcId(0)
{
	INetworkManager::getSingleton().RegisterMsgHandler( DLG_SVR, this );
	//m_nTalkState = END_DIALOG;
}

GameNpcDialog::~GameNpcDialog(void)
{
}

void GameNpcDialog::commitTaskItem(int list, int grid)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list, grid));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	lstItem.push_back(std::pair<int, int>(list5, grid5));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	lstItem.push_back(std::pair<int, int>(list5, grid5));
	lstItem.push_back(std::pair<int, int>(list6, grid6));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	lstItem.push_back(std::pair<int, int>(list5, grid5));
	lstItem.push_back(std::pair<int, int>(list6, grid6));
	lstItem.push_back(std::pair<int, int>(list7, grid7));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7, int list8, int grid8)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	lstItem.push_back(std::pair<int, int>(list5, grid5));
	lstItem.push_back(std::pair<int, int>(list6, grid6));
	lstItem.push_back(std::pair<int, int>(list7, grid7));
	lstItem.push_back(std::pair<int, int>(list8, grid8));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7, int list8, int grid8, int list9, int grid9)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	lstItem.push_back(std::pair<int, int>(list5, grid5));
	lstItem.push_back(std::pair<int, int>(list6, grid6));
	lstItem.push_back(std::pair<int, int>(list7, grid7));
	lstItem.push_back(std::pair<int, int>(list8, grid8));
	lstItem.push_back(std::pair<int, int>(list9, grid9));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::commitTaskItem(int list1, int grid1, int list2, int grid2, int list3, int grid3, int list4, int grid4, int list5, int grid5, int list6, int grid6, int list7, int grid7, int list8, int grid8, int list9, int grid9, int list10, int grid10)
{
	std::vector<std::pair<int, int>> lstItem;
	lstItem.push_back(std::pair<int, int>(list1, grid1));
	lstItem.push_back(std::pair<int, int>(list2, grid2));
	lstItem.push_back(std::pair<int, int>(list3, grid3));
	lstItem.push_back(std::pair<int, int>(list4, grid4));
	lstItem.push_back(std::pair<int, int>(list5, grid5));
	lstItem.push_back(std::pair<int, int>(list6, grid6));
	lstItem.push_back(std::pair<int, int>(list7, grid7));
	lstItem.push_back(std::pair<int, int>(list8, grid8));
	lstItem.push_back(std::pair<int, int>(list9, grid9));
	lstItem.push_back(std::pair<int, int>(list10, grid10));
	send_NpcTalk(m_nNpcMemId, lstItem);
}

void GameNpcDialog::selectOpt( int nSelectOpt, bool isLastOpt )
{
	send_NpcTalk(m_nNpcMemId, nSelectOpt);
	if( isLastOpt )
	{
		endTalk();
	}
}	

void GameNpcDialog::send_CloseTalk()
{
	tagCSPkgBody msgbody;
	msgbody.DlgC.Type		= DLG_TYPE_CLOSE;
	msgbody.DlgC.NpcID		= m_nNpcMemId;
	msgbody.DlgC.Dlg.Close	= DLG_TYPE_CLOSE;
	SendGameServerMsg( DLG_CLT, msgbody );

	GameActor* pActor	= ActorManager::getSingleton().FindActor( m_nNpcMemId );
	if( pActor == NULL ) 
	{
		return;
	}

	if (pActor->getActState().GetState() == GSACT_STAND)
	{
		pActor->ChangeActState( GSACT_STAND );
	}

	if ( pActor->getType() == GAT_NPC )
	{
		GameNPC* pNpc = ((GameNPC*)pActor);
		pNpc->reSetAngle();
	}
	
	m_nNpcMemId = 0;
	m_nNpcResId = 0;
	//m_nTalkState = END_DIALOG;
}

void GameNpcDialog::send_NextTalk()
{
	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )	
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.DlgC.Type			= DLG_TYPE_INPUT;
	msgbody.DlgC.NpcID			= m_nNpcMemId;
	msgbody.DlgC.Dlg.Input[0]	= 0;
	msgbody.DlgC.RolePos.X	= pMainPlayer->GetPosition().x/10;
	msgbody.DlgC.RolePos.Y	= pMainPlayer->GetPosition().z/10;
	ServerTimer::getSingleton().getServerTime(msgbody.DlgC.SvrTime.BenchTimeSec, msgbody.DlgC.SvrTime.BenchTimeUSec);
	SendGameServerMsg( DLG_CLT, msgbody );
}

void GameNpcDialog::endTalk()
{
	GameManager::getSingleton().SendUIEvent( "GE_NPC_ANSWER_END" );
}

void GameNpcDialog::endCurTalk()
{
	if ( m_nNpcMemId == 0 )
	{
		return;
	}

	endTalk();
}

int	GameNpcDialog::talkByMemId( int npcMemId )
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	MainGamePlayer*			pMainPlayer		= actorMgr.getMainPlayer();
	if( pMainPlayer == NULL )	
	{
		return MOVE_STATUS_FORBID;
	}

	GameActor*	pNpc = actorMgr.FindActor( npcMemId );
	if ( pNpc == NULL )
	{
		return MOVE_POINT_CANNOT_WALK;
	}

	// 结束上一次对话
	endCurTalk();
	
	m_lastNpcId = m_nNpcResId = pNpc->getResId();

	GameXYZ pos2(0,0,0);
	GameXYZ pos	= pMainPlayer->GetPosition();
	pos.y	= 0;
	pos2	= pNpc->GetPosition();
	pos2.y	= 0;

	unsigned int Dis = (unsigned int)pos2.subtract(pos).length();
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( m_nNpcResId );
	int nMaxTalkDist = getMaxNpcTalkDist( m_nNpcResId ) - 20;
	if ( pNpc->getSpeed() != 0 )
	{
		nMaxTalkDist = getMaxNpcTalkDist( m_nNpcResId ) - 50;
	}
	
	int nTalkResult = MOVE_TIME_INTERVAL_SMALL;
	if ( Dis <= nMaxTalkDist )
	{
		//// 家园宠物
		if ( npcMemId == GameHomelandManager::getSingleton().getHomePetID() ) {
			GameHomelandManager::getSingleton().openHomePetFrame();
		} else if ( GameHomelandManager::getSingleton().isInPlantMode() && GameHomelandManager::getSingleton().getFarmSoilIndex( pNpc ) >= 0 ) {
			GameHomelandManager::getSingleton().planting( pNpc );
		} else {
			const tagMonsterDef* monsterDef = NULL;
			if( pNpc->getType() == GAT_MONSTER ) {
				monsterDef = &DBDataManager::getSingleton().getMonsterDef( pNpc->getResId() );
			}
			if( monsterDef && ( monsterDef->MiscType & MON_MISC_TYPE_JIAJU ) ) {
				GameHomelandManager::getSingleton().beginFurnitureModeAlready( pNpc );
			} else {
				send_NpcTalk( npcMemId );
			}
		}
		nTalkResult = MOVE_SUCCESS;
	}
	else
	{
		int x, y;
		x = (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().x);
		y = (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().z);
		m_bCath  = true;

		const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef( pNpc->getResId() );
		int radius = 0;
		if (&monsterDef != NULL)
		{
			radius = monsterDef.Radius;
		}
		nTalkResult = pMainPlayer->CmdMoveTo( x, y, radius );
		if ( nTalkResult == MOVE_SUCCESS )
		{
			pMainPlayer->beginAutoTalkToNpc(m_lastNpcId);
		}
	}

	if ( nTalkResult == MOVE_SUCCESS )
	{
		m_nNpcMemId	= npcMemId;
	}

	return nTalkResult;
}

int GameNpcDialog::talkByResId( int npcResId )
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	MainGamePlayer*			pMainPlayer		= actorMgr.getMainPlayer();
	if( pMainPlayer == NULL )	
	{
		return MOVE_STATUS_FORBID;
	}

	GameMapShower*			pGameShower		= GameManager::getSingleton().getGameMapShower();
	int nCurMapID							= pGameShower->GetMinMapContal()->getMapID();
	const tagMapActorInfo*	pNpcInfo		= pGameShower->getNpcInfoById( nCurMapID, npcResId );
	int nRealMapID = pGameShower->getRealMapId();
	if ( pNpcInfo == NULL && nRealMapID >= GAME_PWORLD_MAPID )
	{
		pNpcInfo = pGameShower->getNpcInfoById( nRealMapID, npcResId );
	}
	GameActor*				pNpc			= actorMgr.FindActor( actorMgr.getActorIdByResId( npcResId ) );
	
	int nTalkNpcMemID = 0;
	if ( pNpc != NULL )
	{
		// 当NPC已经在视野范围，若点击的是同一个NPC，则无需重新对话
// 		if ( m_nNpcMemId != 0 && m_nNpcMemId == pNpc->GetID() )
// 		{
// 			return MOVE_ALREADY_IN_DEST;
// 		}
		
		// 若点击的视野范围的NPC是不同的NPC，则重新开始对话
		endCurTalk();
		nTalkNpcMemID = pNpc->GetID();
	}
	else
	{
		endCurTalk();
	}

	if( pNpcInfo == NULL )			
	{
		if( pNpc == NULL )
		{
			//ChatManager::getSingleton().showMidTip( MID_PATH_FINDNONPC );
			char buffer[40];
			sprintf( buffer, "ShowMoveFailReson( %d )", MID_PATH_FINDNONPC );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
			return MOVE_POINT_CANNOT_WALK;
		}
	}
	
	m_lastNpcId = m_nNpcResId = npcResId;

	GameXYZ pos2(0,0,0);
	GameXYZ pos	= pMainPlayer->GetPosition();
	pos.y		= 0;

	if( pNpc == NULL )
	{
		pos2.x = pNpcInfo->m_nRealX*10;
		pos2.z = pNpcInfo->m_nRealY*10;
	}
	else
	{
		pos2 = pNpc->GetPosition();
		pos2.y = 0;
	}

	unsigned int Dis		= (unsigned int)pos2.subtract(pos).length();
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( m_nNpcResId );
	int nMaxTalkDist = getMaxNpcTalkDist( m_nNpcResId );
	if ( pNpc != NULL && pNpc->getSpeed() != 0 )
	{
		nMaxTalkDist = getMaxNpcTalkDist( m_nNpcResId );
	}
	if (nMaxTalkDist < 0)
	{
		nMaxTalkDist = 0;
	}
	
	int nTalkResult = MOVE_TIME_INTERVAL_SMALL;
	if( Dis <= nMaxTalkDist )
	{
		if( pNpc != NULL )
		{
			if ( nTalkNpcMemID == GameHomelandManager::getSingleton().getHomePetID() ) {
				GameHomelandManager::getSingleton().openHomePetFrame();
			} else if ( GameHomelandManager::getSingleton().isInPlantMode() && GameHomelandManager::getSingleton().getFarmSoilIndex( pNpc ) >= 0 ) {
				GameHomelandManager::getSingleton().planting( pNpc );
			} else {
				send_NpcTalk( nTalkNpcMemID ); 
			}
		}
		else
		{
			pMainPlayer->beginAutoTalkToNpc(m_lastNpcId);
			m_bCath = true;
		}

		nTalkResult = MOVE_SUCCESS;
	}
	else
	{
		int x, y;
		if( pNpc == NULL )
		{
			x = (int)Ogre::WorldPos::Fix2Flt( ( pNpcInfo->m_nRealX ) * 10 );
			y = (int)Ogre::WorldPos::Fix2Flt( ( pNpcInfo->m_nRealY ) * 10 );
		}
		else
		{
			x = (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().x);
			y = (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().z);

		}
		m_bCath  = true;

		const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef( npcResId );
		int radius = 0;
		if (&monsterDef != NULL)
		{
			radius = monsterDef.Radius;
		}
		nTalkResult = pMainPlayer->CmdMoveTo( x, y, radius );
		if ( nTalkResult == MOVE_SUCCESS )
		{
			pMainPlayer->beginAutoTalkToNpc(m_lastNpcId);
		}
	}

	if ( nTalkResult == MOVE_SUCCESS )
	{
		m_nNpcMemId	= nTalkNpcMemID;
	}

	return nTalkResult;
}

unsigned int GameNpcDialog::getMaxNpcTalkDist( GameActor* pNpc )
{
	if ( pNpc == NULL )
	{
		return 0;
	}

	return getMaxNpcTalkDist( pNpc->getResId() );
}

unsigned int GameNpcDialog::getMaxNpcTalkDist( int nResId )
{
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( nResId );
	if ( &monsterDef == NULL )
	{
		return 0;
	}

	return TALK_DIS / 2 + monsterDef.Radius;
}

int	 GameNpcDialog::getTalkNpcMemID()
{
	return m_nNpcMemId;
}

void GameNpcDialog::send_NpcTalk(int npcMemId, tagCSPkgBody msgbody)
{
	GameActor* pNpc			= ActorManager::getSingleton().FindActor( npcMemId );
	MainGamePlayer* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if( pNpc != NULL )			
	{
		pMainPlayer->SetSelectTarget( pNpc->GetID() );

		bool bCanWalkTo = pMainPlayer->getCurMap()->getMaskMap()->lineOfSightTest( 
			Ogre::Point2D( pMainPlayer->GetPosition().x/10, pMainPlayer->GetPosition().z/10 ),
			Ogre::Point2D( pNpc->GetPosition().x/10, pNpc->GetPosition().z/10 ), 1 );
		if( !bCanWalkTo ) 
		{
			pMainPlayer->CmdMoveTo( (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().x),
				(int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().z) );
		}
	}

	if( pMainPlayer == NULL )	
	{
		return;
	}

	m_nNpcMemId = npcMemId;
	msgbody.DlgC.RolePos.X	= pMainPlayer->GetPosition().x/10;
	msgbody.DlgC.RolePos.Y	= pMainPlayer->GetPosition().z/10;
	ServerTimer::getSingleton().getServerTime(msgbody.DlgC.SvrTime.BenchTimeSec, msgbody.DlgC.SvrTime.BenchTimeUSec);
	SendGameServerMsg( DLG_CLT, msgbody );
	m_PushCamera = true;

	char szScript[256];
	sprintf( szScript, "springByTalk2Npc(%d)", m_nNpcResId );
	GameManager::getSingleton().getScriptVM()->callString( szScript );

	if ( pMainPlayer->getAutoFind() )
	{
		pMainPlayer->setAutoFind( false );
	}
}

void GameNpcDialog::send_NpcTalk(int npcMemId)
{
	tagCSPkgBody msgbody;
	msgbody.DlgC.Type			= DLG_TYPE_INPUT;
	msgbody.DlgC.NpcID			= npcMemId;
	msgbody.DlgC.Dlg.Input[0]	= 0;
	send_NpcTalk(npcMemId, msgbody);
}

void GameNpcDialog::send_NpcTalk(int npcMemId, int nSelectOpt)
{
	tagCSPkgBody msgbody;
	msgbody.DlgC.Type		= DLG_TYPE_INPUT;
	msgbody.DlgC.NpcID		= npcMemId;
	if (nSelectOpt == 0)
	{
		msgbody.DlgC.Dlg.Input[0] = 0;
	}
	else
	{
		sprintf( msgbody.DlgC.Dlg.Input, "Select=%d", (nSelectOpt - 1));
	}
	send_NpcTalk(npcMemId, msgbody);
}

void GameNpcDialog::send_NpcTalk(int npcMemId, const std::vector<std::pair<int, int>> &lstItem)
{
	tagCSPkgBody msgbody;
	msgbody.DlgC.Type		= DLG_TYPE_INPUT;
	msgbody.DlgC.NpcID		= npcMemId;
	size_t num = lstItem.size();
	if (num == 0)
	{
		msgbody.DlgC.Dlg.Input[0] = 0;
	}
	else if (num == 1)
	{
		sprintf(msgbody.DlgC.Dlg.Input, "ListType=%d&GridIdx=%d", lstItem[0].first, lstItem[0].second);
	}
	else
	{
		char buff[DLG_INPUT_LEN];
		sprintf(msgbody.DlgC.Dlg.Input, "Num=%u", num);
		for (size_t i = 0; i < num; ++i)
		{
			sprintf(buff, "&ListType_%u=%d&GridIdx_%u=%d", i, lstItem[i].first, i, lstItem[i].second);
			strcat(msgbody.DlgC.Dlg.Input, buff);
		}
	}
	send_NpcTalk(npcMemId, msgbody);
}

int	GameNpcDialog::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == DLG_SVR )
	{
		const CSDLGS& npcDialogSvr = msgbody.DlgS;

		//m_nTalkState = BEGIN_DIALOG;
		// 普通对话框
		if( npcDialogSvr.Type == DLG_TYPE_ANSWER )
		{
			tagUIMsgBody	UI_Msg;
			UI_Msg.NpcDialog.m_nNpcId	= npcDialogSvr.NpcID;
			UI_Msg.NpcDialog.m_bNext	= npcDialogSvr.Dlg.DlgAnswer.Next==0?false:true;
			strncpy( UI_Msg.NpcDialog.m_szAnswer, npcDialogSvr.Dlg.DlgAnswer.Answer, DLG_STR_LEN );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_NPC_ANSWER" );
		}
		// 带选项对话框
		if( npcDialogSvr.Type == DLG_TYPE_OPT )
		{
			const DLGOPTS& svrDlgOpts = npcDialogSvr.Dlg.DlgOpt;
			tagUIMsgBody UI_Msg;
			tagNpcOpt& clientNpcOpt		= UI_Msg.NpcOpt;
			clientNpcOpt.m_nNpcId		= npcDialogSvr.NpcID;
			clientNpcOpt.m_optTal		= svrDlgOpts.Total;
			clientNpcOpt.m_lastOptNum	= 0;
			strncpy( clientNpcOpt.m_szSentence, svrDlgOpts.Sentence, DLG_STR_LEN );
			for( int i = 0; i < svrDlgOpts.Total; ++i )
			{
				tagNpcDlgOneOpt& clientOneOpt	= clientNpcOpt.m_OneOpt[i];
				const DLGONEOPT& svrOneOpt		= svrDlgOpts.OneOpt[i];
				if( strstr( svrOneOpt.Opt, "#" ))
				{
					strncpy( clientOneOpt.Opt, svrOneOpt.Opt+1, DLG_OPT_LEN-1 );
					clientOneOpt.m_nType = NPC_DIALOG_LAST_OPT;
					clientNpcOpt.m_lastOpt[clientNpcOpt.m_lastOptNum] = i;
					clientNpcOpt.m_lastOptNum++;
				}
				else
				{
					strncpy( clientOneOpt.Opt, svrOneOpt.Opt, DLG_OPT_LEN );
					clientOneOpt.m_nType = NPC_DIALOG_NORMAL_OPT;
				}
			}
			
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_NPC_OPT" );
		}
		// 对话框最后一句
		if( npcDialogSvr.Type == DLG_TYPE_LAST )
		{
			tagUIMsgBody UI_Msg;
			UI_Msg.NpcLastDialog.m_nNpcId = npcDialogSvr.NpcID;
			strncpy( UI_Msg.NpcLastDialog.DlgLast, npcDialogSvr.Dlg.DlgLast, DLG_STR_LEN );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_NPC_LAST" );
		}
		
		// 带提交物品的对话框
		if( npcDialogSvr.Type == DLG_TYPE_COMMIT_OPT )
		{
			tagUIMsgBody UI_Msg;
			const CSDLGCOMMIT& commitDlg = npcDialogSvr.Dlg.DlgCommit;
			UI_Msg.NpcCommitDialog.m_nNpcId = npcDialogSvr.NpcID;
			UI_Msg.NpcCommitDialog.CommitNum = commitDlg.CommitNum;
			strncpy( UI_Msg.NpcCommitDialog.Answer, commitDlg.Answer, DLG_STR_LEN );
		
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_NPC_COMMIT" );
		}
		{
			GameActor* pNpc			= ActorManager::getSingleton().FindActor(npcDialogSvr.NpcID);
			if (pNpc)
			{
				pNpc->getShowActor()->playAction( 10180, NULL, 0, 0 );
				MainGamePlayer* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
				if (pMainPlayer)
				{
					int nRace = DBDataManager::getSingleton().GetNpcDisplayInfo( pNpc->getResId() ).race;
					if( !( nRace & MONSTER_UNSEEYOU ) )
					{
						pNpc->SetFaceTo( pNpc->GetPosition().x,
							pNpc->GetPosition().z, 
							pMainPlayer->GetPosition().x, 
							pMainPlayer->GetPosition().z );
					}
				}
			}
		}
	}
	return 0;
}

void GameNpcDialog::update(unsigned int dtick)
{
	if ( m_nNpcResId == 0 && m_nNpcMemId == 0 )
	{
		return;
	}

	MainGamePlayer*	pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL ) 
	{
		return;
	}

	if ( m_bCath && !pMainPlayer->canAutoTalkToNpc() )
	{
		m_bCath		= false;
		//LOG_PRIVATE( "GameNpcDialog::update--终止对话--380" );
		m_nNpcResId = 0;
		m_nNpcMemId = 0;
		return;
	}

	GameActor*				pNpc		= NULL;
	const tagMapActorInfo*	pNpcInfo	= NULL;
	GameMapShower*			pGameShower	= GameManager::getSingleton().getGameMapShower();
	if ( m_nNpcMemId != 0 )
	{
		pNpc = ActorManager::getSingleton().FindActor( m_nNpcMemId );
	}

	if ( pNpc == NULL )
	{
		if ( m_nNpcResId == 0 )
		{
			return;
		}

		pNpcInfo = pGameShower->getNpcInfoById( pGameShower->GetMinMapContal()->getMapID(), m_nNpcResId );
		int nRealMapID = pGameShower->getRealMapId();
		if ( pNpcInfo == NULL && nRealMapID >= GAME_PWORLD_MAPID )
		{
			pNpcInfo = pGameShower->getNpcInfoById( nRealMapID, m_nNpcResId );
		}
	}
	
	if ( pNpc == NULL && pNpcInfo == NULL )
	{
		return;
	}

	GameXYZ pos2(0,0,0);
	GameXYZ pos  = pMainPlayer->GetPosition();
	pos.y		 = 0;

	if( pNpc != NULL )
	{
		pos2	= pNpc->GetPosition();
		pos2.y	= 0;
	}
	else
	{
		pos2.x	= pNpcInfo->m_nRealX*10;
		pos2.z	= pNpcInfo->m_nRealY*10;
	}

	unsigned int nDis = (unsigned int)pos2.subtract(pos).length();

	// 超出距离关闭对话框
	if( nDis > getMaxNpcTalkDist( m_nNpcResId ) )
	{
		//// 家园NPC
		if ( m_nNpcResId == 302185 ) {
			GameHomelandManager::getSingleton().closeHomelandFrames();
		} else if ( pNpc && pNpc->GetID() == GameHomelandManager::getSingleton().getHomePetID() ) {
			GameHomelandManager::getSingleton().closeHomePetFrame();
		} else {
			endTalk();
		}
	}

	if( !m_bCath ) 
	{
		return;
	}

	if (pNpc == NULL)
	{
		pNpc = ActorManager::getSingleton().FindActor( ActorManager::getSingleton().getActorIdByResId( m_nNpcResId ) );
	}
	if ( pMainPlayer->getActState().GetState() == GSACT_STAND && pNpc != NULL && pNpc->getSpeed() > 0 )
	{	
		int nNpcX = (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().x);
		int nNpcY = (int)Ogre::WorldPos::Fix2Flt(pNpc->GetPosition().z);
		pMainPlayer->CmdMoveTo( nNpcX, nNpcY );
	}

	bool bCanWalkTo = pMainPlayer->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D( pMainPlayer->GetPosition().x/10, pMainPlayer->GetPosition().z/10 ),
		Ogre::Point2D( pos2.x/10, pos2.z/10 ), 1 );
	
	int nMaxTalkDist = getMaxNpcTalkDist( m_nNpcResId ) - 1;
	if ( pNpc != NULL && pNpc->getSpeed() != 0 )
	{
		nMaxTalkDist = getMaxNpcTalkDist( m_nNpcResId ) - 50;
	}
	if (nMaxTalkDist < 0)
	{
		nMaxTalkDist = 0;
	}
	// TODO: 判断nMaxTalkDist是否小于0，有可能m_nNpcResId没有对应的怪物DEF，导致对话一直停止的错误 

	if( nDis <= nMaxTalkDist )
	{
		if( pMainPlayer->getActState().GetState() == GSACT_MOVE )
		{
			pMainPlayer->CmdMoveStop();
		}

		if ( pNpc != NULL )
		{
			if ( pMainPlayer->canAutoTalkToNpc() )
			{
				pMainPlayer->endAutoTalkToNpc();
			}

			//// 家园宠物
			if ( pNpc->GetID() == GameHomelandManager::getSingleton().getHomePetID() ) {
				GameHomelandManager::getSingleton().openHomePetFrame();
			} else if ( GameHomelandManager::getSingleton().isInPlantMode() && GameHomelandManager::getSingleton().getFarmSoilIndex( pNpc ) >= 0 ) {
				GameHomelandManager::getSingleton().planting( pNpc );
			} else {
				const tagMonsterDef* monsterDef = NULL;
				if( pNpc->getType() == GAT_MONSTER ) {
					monsterDef = &DBDataManager::getSingleton().getMonsterDef( pNpc->getResId() );
				}
				if( monsterDef && ( monsterDef->MiscType & MON_MISC_TYPE_JIAJU ) ) {
					GameHomelandManager::getSingleton().beginFurnitureModeAlready( pNpc );
				} else {
					send_NpcTalk( pNpc->GetID() );
				}
			}
			m_bCath = false;
		}
	}
}

void GameNpcDialog::clearNpcData()
{
	m_nNpcMemId = 0;
	m_nNpcResId = 0;
}
