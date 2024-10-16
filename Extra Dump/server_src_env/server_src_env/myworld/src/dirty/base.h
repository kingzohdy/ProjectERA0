#ifndef __BASE_H__
#define __BASE_H__

#ifdef __cplusplus
extern "C" 
{
#endif

int validFile(const char *szFilePath);
int copyFile (const char *szfrom, const char *szDest);

int getMd5String	(char *pShm, char szMd5String[]);
int getMd5FilePath	(char *pShm, char szMd5File[]);
int setMd5String	(char *pShm, char *pszMd5String);
int setMd5FilePath	(char *pShm, char *pszMd5File);

//

#ifdef __cplusplus
}
#endif

#endif

