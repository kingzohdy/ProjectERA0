#ifndef BLACK_WHITE_H
#define BLACK_WHITE_H

#include "pal/pal.h"

#define MAX_ADDR_NUM 64

typedef struct  tagAddrEntry
{
    int  iAddr ;
    int  iAddrMask ;
}AddrEntry ;

typedef struct tagAddrList
{
   int iAddrNum ;
   AddrEntry astAddr[MAX_ADDR_NUM] ;	
}ADDRLIST ;


typedef struct tagBlackWhiteHandle
{
	ADDRLIST stBlack;
	ADDRLIST stWhite;
}BlackWhiteHandle;

int black_white_init(char *pszCfgPath, BlackWhiteHandle **ppstHandle);

//0 ip ok, -1 ip forbidden
int black_white_check(BlackWhiteHandle *pstHandle, int iIP);

int black_white_fini(BlackWhiteHandle **ppstHandle);

#endif /* BLACK_WHITE_H */
