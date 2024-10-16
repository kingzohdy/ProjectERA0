
#ifndef _OG_COMM_H
#define _OG_COMM_H

#define SERVER_KEY	"1234567890abcdef"

#define ZONE_LISTEN_PORT 6299
#define ZONE_CONNECT_TIMEOUT 100

#define OBJECT_NAME_LEN 50

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned char cWorld;
	unsigned char cZone;
	unsigned char cModule;
	unsigned char cInstance;
} ProcIDChar;

typedef struct {
	int iRandCount;
	long aiRandSet[50];
	time_t tTune;
	char cSuccFlag;		//标志位，通过GM设置效果成功率、命中率等概率为100
} TestEnv;

typedef struct {
	char cStatus;
	int iDest;
	long tStartWait;
} BusEmptyTest;

#define BUS_EMPTY_WAIT_TIMEOUT		600
#define BUS_EMPTY_RETRY_INTERVEL		60
#define BUS_EMPTY_TEST_NOTSTART		0
#define BUS_EMPTY_TEST_WAIT			1
#define BUS_EMPTY_TEST_FINISH			2

#define BUS_EMPTY_STATUS( stTest )		( stTest.cStatus )

#define INIT_BUS_EMPTY_TEST_STAT( stTest, iBusDest )  \
do {  \
	stTest.cStatus = BUS_EMPTY_TEST_NOTSTART;  \
	stTest.iDest = iBusDest;  \
} while(0)

#define SET_BUS_EMPTY_TEST_WAIT( stTest, tTmNow )  \
do {  \
	stTest.cStatus = BUS_EMPTY_TEST_WAIT;  \
	stTest.tStartWait = tTmNow;  \
} while(0)

#define SET_BUS_EMPTY_TEST_FINISH( stTest )  \
do {  \
	stTest.cStatus = BUS_EMPTY_TEST_FINISH;  \
} while(0)


int IntCompare(const void *p1, const void *p2);
int KillPre(int iProcID);
int GetPidByEntity( int iEntityId) ;
int SendSig2Entity( int iEntityId , int sig) ;

#ifdef __cplusplus
}
#endif


#endif
