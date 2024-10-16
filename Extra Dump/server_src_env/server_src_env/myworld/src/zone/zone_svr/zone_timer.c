/*
**  @file $RCSfile: zone_timer.c,v $
**  general description of this module
**  $Id: zone_timer.c,v 1.3 2012/07/14 03:14:57 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/14 03:14:57 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_timer.h"


/**
* ��ʱ�Ļص�����
*
* @param pstEnv   ��������
* @return         =0�ɹ� ��0ʧ��
*/
static void timer_callback(struct timer_heap_t *ht, struct timer_entry *entry)
{
	TIMERUSERDATA *pstTimerUserData;
	
	pstTimerUserData = (TIMERUSERDATA *)entry->user_data;
	switch(pstTimerUserData->iTimerType)
	{
		case TIMER_TYPE_STATUS:
			// TODO your self same thing
			break;
		default:
			break;
	}
	
	//������timer_entery;
	timer_entry_push_back(ht, entry);
}

/**
* ����<=1��Ķ����������ʱ��������.�����Ҫ��������,
* ��Ҫ��rebuild�н�����timer
*
* @param pstEnv   ��������
* @return         =0�ɹ� ��0ʧ��
*/
int zone_timer_init(ZONESVRENV *pstEnv)
{
	int iRet = 0;
	//��ʼ�����ٶ�ʱ���ڵ㣬�����Ļ������Լ���������
	iRet = timer_heap_create(ht,10240);
	if(iRet<0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone_timer_init fail %d",iRet);
	}
	
	
	return iRet;
}

int zone_timer_tick()
{
	timer_heap_poll(ht, NULL);
	return 0;
}
/**
* ����timer�����ݲ������ڹ����ڴ���
* ����������Ҫ���½���timer
* @param pstEnv   ��������
* @return         =0�ɹ� ��0ʧ��
*/
int zone_timer_rebuild(ZONESVRENV *pstEnv)
{
	return 0;
	UNUSED(pstEnv);
}

//������ص�schedule,Ms����,
int zone_timer_schedule_status(ZONESVRENV *pstEnv,ZoneAni *pstZoneAni,unsigned short StatusID,
					unsigned short Level,int Ms)
{
	TIMERUSERDATA stTimerUserData;
	struct timer_entry *entry=NULL;
	struct timeval delay;

	timer_entry_pop_back(ht,entry);
	if(!entry)
	{
		return -1;
	}
	memset(&stTimerUserData,0,sizeof(stTimerUserData));
	stTimerUserData.pstEnv = pstEnv;
	stTimerUserData.iAniType = pstZoneAni->iType;
	stTimerUserData.stAniObj = pstZoneAni->stObj;
	stTimerUserData.iTimerType = TIMER_TYPE_STATUS;
	stTimerUserData.stTimerData.stStatusData.StatusID = StatusID;
	stTimerUserData.stTimerData.stStatusData.Level = Level;

	memset(&delay,0,sizeof(delay));
	delay.tv_sec = Ms/1000;
	delay.tv_usec = Ms%1000;
	timer_entry_init(entry,0,&stTimerUserData,sizeof(stTimerUserData),timer_callback);
	timer_heap_schedule(ht,entry,&delay);
	return 0;
}