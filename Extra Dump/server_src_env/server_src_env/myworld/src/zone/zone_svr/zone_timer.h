#ifndef ZONE_TIMER_H
#define ZONE_TIMER_H

#include "zone_svr.h"
#include "comm_timer.h"

enum tagTimerType 
{
    TIMER_TYPE_UNKNOW = 0, 
    TIMER_TYPE_STATUS = 1,
};

typedef struct
{
    unsigned short StatusID;                        	/*   ×´Ì¬ID£¬1000µ½3000 */
    unsigned short Level;                           	/*   ×´Ì¬µÈ¼¶ */
}TIMERSTATUSDATA;

typedef union 
{
	TIMERSTATUSDATA stStatusData;
}TIMERTYPEDATA;

struct tagTimerUserData
{
	ZONESVRENV *pstEnv;
	int iAniType;
	ZoneAniData  stAniObj;
	int iTimerType;
	TIMERTYPEDATA stTimerData;
};
struct tagTimerUserData;
typedef struct  tagTimerUserData                        	TIMERUSERDATA;
typedef struct  tagTimerUserData                        	*LPTIMERUSERDATA;
char szWatchTimerUserData[TIMER_USER_DATA - sizeof(TIMERUSERDATA) + 1];


int zone_timer_init(ZONESVRENV *pstEnv);	
int zone_timer_tick();

#endif /* ZONE_TIMER_H */

