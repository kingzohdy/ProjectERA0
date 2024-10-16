#ifndef LIBDIRTY_H
#define LIBDIRTY_H

#ifdef __cplusplus
extern "C"
{
#endif

void InitDirtySystem(char *pszPath);
void InitDirtySystem2(char *pszPath,char *pszMd5Path);
int CheckDirty(const char* pszText , int iLevel);
int CheckCharacter ( char *pszStr );
int CheckCharacter2(char * pszStr) ;
int CheckCharacter1(char * pszStr) ;
int DirtyReplace( char * pszText );


#ifdef __cplusplus
}
#endif

#endif /* LIBDIRTY_H */
