
#ifndef _OG_TERM_H
#define _OG_TERM_H

typedef void (*PFNTERMCLEAN)(void);
typedef void* (*PFNTERMEXIT)(void* pvArg);

extern int iTerminal;
void CheckTerm(const char *sDesc);
void CheckTermWithClean(const char *sDesc, PFNTERMCLEAN pfnClean);
void CheckTermWithExit(const char *sDesc, PFNTERMEXIT pfnExit, void* pvArg);

#endif
