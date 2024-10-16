#ifndef LIBDIRTY_H
#define LIBDIRTY_H

#ifdef __cplusplus
extern "C"
{
#endif

void InitDirtySystem(char *pszPath);
int CheckDirty(const char* pszText , int iLevel);
int CheckCharacter ( char *pszStr );
int CheckCharacter2(char * pszStr) ;


#ifdef __cplusplus
}
#endif

#endif /* LIBDIRTY_H */
