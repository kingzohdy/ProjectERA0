/*
**  @file $RCSfile: tconvert.h,v $
**  general description of this module
**  $Id: tconvert.h,v 1.3 2012/07/27 05:08:55 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:08:55 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCONVERT_H

#define TCONVERT_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

static char HEX_value[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

long tconvert_get_bytesl(const char *a_szStr, char** a_ppszEnd, int a_iRadix);
long tconvert_get_daysl(const char *a_szStr, char** a_ppszEnd, int a_iRadix);

char* Bin2Hex(unsigned char * pbin,int binlen, char* phex,int *hexlen);


char* Hex2Bin(char * phex,int hexlen, unsigned char* pbin,int* binlen);


#ifdef __cplusplus
#endif /* __cplusplus */

#endif /* TCONVERT_H */
