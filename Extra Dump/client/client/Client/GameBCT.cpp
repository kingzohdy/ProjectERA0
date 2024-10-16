#include "GameBCT.h"
#include <Windows.h>
#include "GameUIMsg.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "NetMessage.h"
#include "GameDBData.h"
#include "GameWheelLottery.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMap.h"

using namespace std;

GameBCT::GameBCT()
{
	INetworkManager::getSingleton().RegisterMsgHandler( BCT_SVR, this );
	m_nPworldID = 0;
	memset( &m_LayerAward, 0, sizeof(m_LayerAward) );
}

GameBCT::~GameBCT()
{

}

GameBCT & GameBCT::getSingleton()
{
	static GameBCT singleton;
	return singleton;
}

int GameBCT::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if (msgid == BCT_SVR)
	{
		const CSBCTSVR &data = msgbody.BctRes.SvrData;
		switch (msgbody.BctRes.Cmd)
		{
		case BCT_CMD_SVR_HISTORY: 	/* 每层历史记录 */
			{
				int layer = data.BctLayerHistory.Layer;
				setHistory(layer, data.BctLayerHistory, timeGetTime());
				if (layer < 100)
				{
					setHistory(layer + 1, data.BctLayerHistory );
				}

				char buf[256];
				sprintf(buf,"UpdateHundredLevelTownIntroduceTopInfo(%d)", layer);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;

		case BCT_CMD_SVR_TOP: 	/* 自己最高层数记录 */
			{
				memcpy( m_LayerAward, data.BctSelfTop.LayerAward, sizeof(m_LayerAward) );
				char buf[256];
				sprintf(buf,"SetHundredLevelTownHistoryFloor(%d)", data.BctSelfTop.Layer);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;

		case BCT_CMD_SVR_EXIT: 	/* 退出白层塔发总结数据 */
			{
				const BCTSVREXIT &info = data.BctSvrExit;
				GameWheelLottery::getSingleton().reset();
				GameWheelLottery::getSingleton().add(info.RaffleNum);
				m_boxStatistic.clear();
				for (int i = 0; i < info.PassItemNum; ++i)
				{
					const PASSITEM &item = info.PassItems[i];
					m_boxStatistic[item.Layer].id = item.ItemID;
					m_boxStatistic[item.Layer].num = item.Num;
				}

				char buf[256];
				sprintf(buf,"SetHundredLevelTownFinishFrame(%d, %d, %d)", info.CurrPassLayer, info.CurrPassNum, info.TotalExp);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;

		case BCT_CMD_SVR_FAIL: 	/* 白层塔失败面板 */
			{
				char buf[256];
				sprintf(buf,"SetHundredLevelTownFail(%d, %d)", int(float(data.BctFail.CurrMonHp) / data.BctFail.LayerMonHp * 100), 100 - data.BctFail.HistoryMonHp);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;

		case BCT_CMD_SVR_PASS: 	/* 弹出白层塔过关面板 */
			{
				char buf[256];
				sprintf(buf,"SetHundredLevelTownFinishDialog(%d)", data.BctPass.Layer);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;

		case BCT_CMD_SVR_RAFFLE_START: /* 抽奖 */
			{
				int result = data.BctRaffleStart.Index;
				GameWheelLottery::getSingleton().setResult(result);
				char buf[256];
				sprintf(buf,"SetGameWheelLotteryResult(%d)", result);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;

		case BCT_CMD_SVR_PASS_STOP_TIME: 	/* 停止计时 */
			{
				GameManager::getSingleton().SendUIEvent("GE_STOP_PWORLD_TIME");
			}
			break;

		case BCT_CMD_SVR_INFO:
			{
				char buf[256];
				sprintf(buf,"ReEnterHundredLevelTown(%d, %d, %d)", data.BctInfo.CurrLayer, data.BctInfo.StartLayer, data.BctInfo.PassLayer);
				GameManager::getSingleton().getScriptVM()->callString( buf );
			}
			break;
		}
	}

	return 0;
}

void GameBCT::setHistory(int layer, const BCTLAYERHISTORY& info, unsigned int updateTime/* = 0*/)
{
	BCTHistory &data = m_history[layer];
	if( updateTime == 0 )
	{
		data.TopTime = info.NextTopUseTime;
		data.TopTimeMs = info.NextTopUseTimeMs;
		strncpy( data.TopName, info.NextTopName, NAME_LEN -1 );
	}else{
		data.SelfTime = info.SelfTopUseTime;
		data.SelfTimeMs = info.SelfTopUseTimeMs;
		data.SelfWhen = info.SelfTopRecordTime;
		data.TopTime = info.TopUseTime;
		data.TopTimeMs = info.TopUseTimeMs;
		data.UpdateTime = updateTime;
		strncpy( data.TopName, info.TopName, NAME_LEN -1 );
	}
}

const BCTHistory * GameBCT::getHistory(int layer)
{
	map<int, BCTHistory>::iterator i = m_history.find(layer);
	if (i == m_history.end())
	{
		return NULL;
	}

	return &(i->second);
}

const char * GameBCT::getDesciption(int layer)
{
	const tagBCTDef* BCTDef = DBDataManager::getSingleton().getBCTDef(layer);
	if (BCTDef == NULL)
	{
		assert(false);
		return NULL;
	}

	return BCTDef->BCTDesc;
}

int GameBCT::getAwardExp(int layer)
{
	const tagBCTDef* BCTDef = DBDataManager::getSingleton().getBCTDef(layer);
	if (BCTDef == NULL)
	{
		assert(false);
		return 0;
	}

	return BCTDef->AwardExp;
}

BCTBoxItem GameBCT::getBoxItem(int layer)
{
	BCTBoxItem ret;
	const tagBCTDef* BCTDef = DBDataManager::getSingleton().getBCTDef(layer);
	if (BCTDef == NULL)
	{
		assert(false);
	}
	else
	{
		ret.Num = BCTDef->Num;
		for (int i = 0; i < ret.Num; ++i)
		{
			ret.ItemID[i] = BCTDef->BCTItems[i].ItemID;
			ret.ItemNum[i] = BCTDef->BCTItems[i].Num;
		}
	}

	return ret;
}

void GameBCT::onEnterPworld(int nMapId)
{
	const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef( nMapId / 100 );
	if (pworldDef == NULL)
	{
		assert(false);
		return;
	}
	if (pworldDef->Type == PWORLD_TYPE_SINGLE && pworldDef->PolyType == PWORLD_POLY_BCT)
	{
		GameManager::getSingleton().SendUIEvent("GE_BCT_ENTER");
	}
	m_nPworldID = nMapId;
}

void GameBCT::onLeavePworld(int nMapId)
{
	GameManager::getSingleton().SendUIEvent("GE_BCT_LEAVE");
	m_nPworldID = 0;
}

bool GameBCT::hasBox(int layer)
{
	const tagBCTDef* BCTDef = DBDataManager::getSingleton().getBCTDef(layer);
	if (BCTDef != NULL)
	{
		return BCTDef->Num > 0;
	}

	return false;
}

void GameBCT::EnterPworld(int layer, bool useItem)
{
	tagCSPkgBody msgbody;
	msgbody.BctReq.Cmd = BCT_CMD_CLT_ENTER;
	msgbody.BctReq.CltData.BctEnter.StartLayer = layer;
	msgbody.BctReq.CltData.BctEnter.UseItem = useItem ? 1 : 0;
	msgbody.BctReq.CltData.BctEnter.PworldID = m_nPworldID;
	SendGameServerMsg(BCT_CLT, msgbody);
}

void GameBCT::ContinuePworld(int layer)
{
	tagCSPkgBody msgbody;
	msgbody.BctReq.Cmd = BCT_CMD_CLT_CONTINUE;
	msgbody.BctReq.CltData.BctContinue.NextLayer = layer;
	SendGameServerMsg(BCT_CLT, msgbody);
}

void GameBCT::RepeatPworld(int layer)
{
	tagCSPkgBody msgbody;
	msgbody.BctReq.Cmd = BCT_CMD_CLT_AGAIN;
	msgbody.BctReq.CltData.BctAgain.Layer = layer;
	SendGameServerMsg(BCT_CLT, msgbody);
}

void GameBCT::ExitPworld()
{
	tagCSPkgBody msgbody;
	msgbody.BctReq.Cmd = BCT_CMD_CLT_EXIT;
	msgbody.BctReq.CltData.BctExit.PworldID = m_nPworldID / 100;
	SendGameServerMsg(BCT_CLT, msgbody);
}

int GameBCT::getBoxIcon(int layer)
{
	const tagBCTDef* BCTDef = DBDataManager::getSingleton().getBCTDef(layer);
	if (BCTDef == NULL)
	{
		return -1;
	}

	return BCTDef->ChestID;
}

BCTMonsterType GameBCT::getMonsterType(int layer)
{
	BCTMonsterType ret;
	ret.Num = 0;
	const tagBCTDef* BCTDef = DBDataManager::getSingleton().getBCTDef(layer);
	if (BCTDef == NULL)
	{
		assert(false);
	}
	else
	{
		for (int i = 0; i < BCT_MON_TYPE_NUM; ++i)
		{
			if (BCTDef->BCTMon[i].MonID != 0)
			{
				ret.MonsterID[ret.Num] = BCTDef->BCTMon[i].MonID;
				ret.Level[ret.Num] = BCTDef->BCTMon[i].Level;
				++ret.Num;
			}
		}
	}

	return ret;
}

const char * GameBCT::getMonsterName(int id)
{
	const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef(id);
	return monsterDef.Name;
}

int GameBCT::getMonsterIcon(int id)
{
	const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef(id);
	return monsterDef.IconID;
}

int GameBCT::getNextBoxLayer(int layer)
{
	std::map<int, BCTBoxStatistic>::iterator i;
	if (layer == 0)
	{
		i = m_boxStatistic.begin();
	}
	else
	{
		i = m_boxStatistic.find(layer);
		if (i != m_boxStatistic.end())
		{
			++i;
		}
	}
	if (i == m_boxStatistic.end())
	{
		return 0;
	}
	{
		return i->first;
	}
}

BCTBoxStatistic * GameBCT::getLayerItem(int layer)
{
	std::map<int, BCTBoxStatistic>::iterator i = m_boxStatistic.find(layer);
	if (i == m_boxStatistic.end())
	{
		return NULL;
	}

	return &(i->second);
}

void GameBCT::updateHistory(int layer)
{
	const unsigned int loseTime = 10000;
	map<int, BCTHistory>::iterator i = m_history.find(layer);
	if (i != m_history.end() && i->second.UpdateTime + loseTime > timeGetTime())
	{
		char buf[256];
		sprintf(buf,"UpdateHundredLevelTownIntroduceTopInfo(%d)", layer);
		GameManager::getSingleton().getScriptVM()->callString( buf );
	}

	tagCSPkgBody msgbody;
	msgbody.BctReq.Cmd = BCT_CMD_CLT_LAYER_TOP;
	msgbody.BctReq.CltData.BctTopInfo.Layer = layer;
	SendGameServerMsg(BCT_CLT, msgbody);
}

bool GameBCT::isIn()
{
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	if (pActor && pActor->getCurMap())
	{
		return pActor->getCurMap()->getRealId() / 100 == 300;
	}
	return false;
}

bool GameBCT::IsAwardReceive(int layer)
{
	int idx = layer>>5;
	unsigned long layerVal = 1<<(layer%32);
	unsigned long result = m_LayerAward[idx] & layerVal;
	return result > 0 ?true:false;
}

void GameBCT::GetAward(int layer)
{
	tagCSPkgBody msgbody;
	msgbody.BctReq.Cmd = BCT_CMD_CLT_AWARD;
	msgbody.BctReq.CltData.BctAward.Layer = layer;
	SendGameServerMsg(BCT_CLT, msgbody);
}

BCTAwardItem GameBCT::GetAwardItem( int layer, int idx )
{
	BCTAwardItem item;
	memset( &item, 0, sizeof(BCTAwardItem) );
	const tagBCTDef* def = DBDataManager::getSingleton().getBCTDef(layer);
	if ( def != NULL )
	{
		item.itemID = def->GiftItem[idx].ItemID;
		item.itemNum = def->GiftItem[idx].ItemNum;
	}
	return item;
}