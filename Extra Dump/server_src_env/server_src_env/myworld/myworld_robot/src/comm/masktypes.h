
#ifndef __MASKTYPES_H__
#define __MASKTYPES_H__

#ifndef BOOL
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

#ifndef WIN32
#define max(a, b)	((a) > (b) ? (a) : (b))
#define min(a, b)	((a) < (b) ? (a) : (b))
#endif

typedef unsigned char MASK_BIT_TYPE;

#ifndef MAKEFOURCC
#define MAKEFOURCC(c1, c2, c3, c4)	( (c4)<<24 | (c3)<<16 | (c2)<<8 | (c1) )
#endif


#endif