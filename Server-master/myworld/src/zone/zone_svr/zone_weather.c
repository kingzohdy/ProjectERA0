#include "zone_weather.h"
#include "zone_clt.h"
#include "zone_city.h"
#include "zone_arm.h"
#include "zone_package.h"
#include "zone_err.h"
#include "zone_oplog.h"

typedef struct WeatherChg
{
	char cHour;				// 几点钟
	char cType;				// 变化类型
}WEATHERCHG;



// 天气变化时间表
static WEATHERCHG WeatherDef[]=
{
	{0,2},
	{1,3},
	{2,1},
	{3,4},
	{4,2},
	{5,3},
	{6,1},
	{7,4},
	{8,2},
	{9,3},
	{10,1},
	{11,4},
	{12,2},
	{13,3},
	{14,1},
	{15,4},
	{16,2},
	{17,3},
	{18,1},
	{19,4},
	{20,2},
	{21,3},
	{22,1},
	{23,4},
};

int weather_init(ZONESVRENV *pstEnv)
{
	SVRWEATHER *pstWeather = &pstEnv->pstZoneObj->stWeather;
	int i;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iSize = sizeof(WeatherDef)/sizeof(WeatherDef[0]);
	int iIdx;
//	int iHour;
	time_t t;
	struct tm stNow;

	memset(pstWeather, 0, sizeof(*pstWeather));
	localtime_r(&tCurr, &stNow);

	for (i=iSize-1; i>=0; i--)
	{
		if (stNow.tm_hour >= WeatherDef[i].cHour)
		{
			pstWeather->cCurrWeather = WeatherDef[i].cType;

			// 雨天在数组WeatherDef中表示延用前面索引的天气
			if (pstWeather->cCurrWeather == WEATHER_TYPE_RAIN)
			{
				pstWeather->cCurrWeather = WeatherDef[i-1].cType; // 这里i-1不可能小于0
			}

			iIdx = (i+1)%iSize;
			pstWeather->cNextWeather = WeatherDef[iIdx].cType;
			if (pstWeather->cNextWeather == WEATHER_TYPE_RAIN)
			{
				pstWeather->cNextWeather = WeatherDef[i].cType;
			}
	

			// 求出下一次变天时间
			/*iHour = WeatherDef[iIdx].cHour;
			if (iHour == 0)
				iHour = 24;
			
			t = tCurr + (iHour-WeatherDef[i].cHour)*3600;
			localtime_r(&t, &stNow);*/

			t = tCurr + 3600;
			localtime_r(&t, &stNow);
			//stNow.tm_hour = WeatherDef[iIdx].cHour;
			stNow.tm_min = 0;
			stNow.tm_sec = 0;
			pstWeather->iChgTime = mktime(&stNow);
			break;	
		}	
	}
	return 0;
}

int weather_notify(ZONESVRENV *pstEnv, Player *pstPlayer, int iBroadcast)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	CSWEATHER *pstWeather = &pstRes->PlayerMiscSvrData.Weather;
	SVRWEATHER *pstSvrWeather = &pstEnv->pstZoneObj->stWeather;

	pstRes->PlayerMiscType = WEATHER_CHG_NOTIFY;
	pstWeather->WeatherType = pstSvrWeather->cCurrWeather;
	pstWeather->Master = pstSvrWeather->cMaster;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);

	if (iBroadcast)
	{
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
	}
	else
	{
		if (pstPlayer)
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

// 登录通知
int weather_player_login(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	weather_notify(pstEnv, pstPlayer, 0);
	return 0;
}


int weather_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iType)
{
	SVRWEATHER *pstWeather = &pstEnv->pstZoneObj->stWeather;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iSize = sizeof(WeatherDef)/sizeof(WeatherDef[0]);
	struct tm stNext;
	int i;
	time_t t;
	int iUseID;
	USEDEF *pstUseDef;
	char szName[8];
	int iPre = pstWeather->cCurrWeather;;


	if (!is_world_city_master(pstEnv, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_WEATHER1);
		return -1;
	}

	if (iType == WEATHER_TYPE_DAY)
	{
		iUseID = USE_ID_CHG_WEATHER_1;
		strcpy(szName,  "白天");
	}
	else if (iType == WEATHER_TYPE_DUSK)
	{
		iUseID = USE_ID_CHG_WEATHER_2;
		strcpy(szName,  "傍晚");
	}
	else if (iType == WEATHER_TYPE_NIGHT)
	{
		iUseID = USE_ID_CHG_WEATHER_3;
		strcpy(szName,  "夜间");
	}
	else if (iType == WEATHER_TYPE_RAIN)
	{
		iUseID = USE_ID_CHG_WEATHER_4;
		strcpy(szName,  "雨天");
	}
	else
	{
		return -1;
	}

	pstUseDef = z_find_usedef(pstEnv, iUseID);
	if (!pstUseDef)
	{
		return -1;
	}

	if (package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_NOMONEY);
		return -1;
	}

	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_WEATHER, 
				0, 0, pstUseDef->UseMoney, "改变天气");
	
	pstWeather->cCurrWeather  = iType;
	if (iPre != pstWeather->cCurrWeather)
	{
		// 全服广播给客户端 todo:
		weather_notify(pstEnv, NULL, 1);
	}
	

	t = tCurr + 1800;
	localtime_r(&t, &stNext);

	for (i=iSize-1; i>=0; i--)
	{
		if (stNext.tm_hour >= WeatherDef[i].cHour)
		{
			pstWeather->cNextWeather = WeatherDef[i].cType;
			if (pstWeather->cNextWeather == WEATHER_TYPE_RAIN)
			{
				pstWeather->cNextWeather = WeatherDef[i-1].cType;
			}
			
			pstWeather->iChgTime = t;
			break;	
		}		
	}
	pstWeather->cMaster = 1;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer,SYS_MIDDLE, ZONEERR_WEATHER2, szName);
	z_sys_strf_broadcast(pstEnv, SYS_MIDDLE2, ZONEERR_BROAD347, 
				pstPlayer->stRoleData.RoleName, szName);
	return 0;
}

int weather_tick(ZONESVRENV *pstEnv)
{
	SVRWEATHER *pstWeather = &pstEnv->pstZoneObj->stWeather;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iSize = sizeof(WeatherDef)/sizeof(WeatherDef[0]);
	struct tm stNow;
	int i;
	int iIdx;
	time_t t;
	int iPre = pstWeather->cCurrWeather;
	int iNext;
	
	if (tCurr < pstWeather->iChgTime)
	{
		return 0;
	}

	pstWeather->cCurrWeather  = pstWeather->cNextWeather;
	if (iPre != pstWeather->cCurrWeather)
	{
		// 全服广播给客户端 todo:
		weather_notify(pstEnv, NULL, 1);
	}

	localtime_r(&tCurr, &stNow);
	for (i=iSize-1; i>=0; i--)
	{
		if (stNow.tm_hour >= WeatherDef[i].cHour)
		{
			iIdx = (i+1)%iSize;

			// 不会连续出现2次雨天
			if (RAND1(100) < 50 && iPre != WEATHER_TYPE_RAIN && !pstWeather->cMaster)
			{
				iNext = WEATHER_TYPE_RAIN;

				// 雨天30分钟之后变天
				t = tCurr + 1800;
				pstWeather->iChgTime = t;
			}
			else
			{
				iNext = WeatherDef[iIdx].cType;
				if (iNext == WEATHER_TYPE_RAIN)
				{
					iNext  = WeatherDef[i].cType;
				}

				t = tCurr + 3600;
				localtime_r(&t, &stNow);

				//stNow.tm_hour = WeatherDef[iIdx].cHour;
				stNow.tm_min = 0;
				stNow.tm_sec = 0;
				pstWeather->iChgTime = mktime(&stNow);
				
			}
			
			pstWeather->cNextWeather = iNext;
			pstWeather->cMaster = 0;

			if (pstWeather->iChgTime  <= tCurr)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "weather_tick fail 2");	
			}
			break;	
		}	
	}

	if (i<0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "weather_tick fail 1");	
	}
	return 0;
}







