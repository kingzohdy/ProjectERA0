#ifndef _TLIB_CFG_H_
#define _TLIB_CFG_H_

#define CFG_STRING	(int)1
#define CFG_INT		(int)2
#define CFG_LONG		(int)3
#define CFG_DOUBLE	(int)4
#define CFG_LINE		(int)5
#define CFG_IP		(int)6
#define CFG_ADDR		(int)7

#define CFG_SECTION		(int)50

#define CFG_INT_ARRAY		(int)102
#define CFG_IP_ARRAY		(int)106
#define CFG_ADDR_ARRAY		(int)107
#define CFG_SECTION_ARRAY		(int)150

#define CFG_STRING_ARRAY	(int)108
#define LENGTH_CFG_STRING	(int)80

void TLib_Cfg_GetConfig(char *sConfigFilePath, ...);

#endif
