/*
**  @file $RCSfile: centerdapi.h,v $
**  general description of this module
**  $Id: centerdapi.h,v 1.1.1.1 2009/07/23 04:22:48 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/23 04:22:48 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef _CENTERDAPI_H_
#define _CENTERDAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

struct tagPkgHead;
struct tagFile
{
	char	szMD5[16];
	char	szFile[4096];
	int     iResult;
};

/**
  Assign an initial value to mailapi.

  @param a_pszGCIMKey tbusmgr key
  @param a_iBusinessid 
  @return 0 if successful, or -1 if an error occurred
 */
int centerdapi_initialize(const char *a_pszGCIMKey, int a_iBusinessid);

/**
  Create an centerd api handle.

  @param iProcID
  @param piHandle 
  @param iAppid		application id 5 default
  @return 0 if successful, or -1 if an error occurred
 */
int centerdapi_create(int iProcID, int* piHandle, int iAppid);

/**
  Connect to centerd

  @param iHandle
  @param iDst 
  @return 0 if successful, or -1 if an error occurred
 */
int centerdapi_connect(int iHandle, int iDst);

/**
  finish to centerdapi.
  
  @param void 
 */
void centerdapi_fini(void);

/**
  free to centerd handle.

  @param piHandle 
  @return 0 if successful, or -1 if an error occurred
 */
void centerdapi_free(int* piHandle);

/**
  recv msg from centerd.
  
  @param piHandle 
  @param piSrc 
  @param pszBuff	agent body buff
  @param iBuff	agent body len
  @param pstHead	agent head
  @return 0 if successful, or -1 if an error occurred
 */
int centerdapi_recv(int iHandle, int* piSrc, char *pszBuff, int *piBuff, struct tagPkgHead* pstHead);

/**
  send msg to centerd.

  @param piHandle 
  @param piSrc 
  @param pszBuff	agent body buff
  @param iBuff	agent body len
  @param pstHead	agent head
  @return 0 if successful, or -1 if an error occurred
 */

int centerdapi_send(int iHandle, int iDst, char *pszBuff, int iBuff, struct tagPkgHead* pstHead);

/**
  register to centerd.
  
  @param piHandle 
  @param iDst 
  @return 0 if successful, or -1 if an error occurred
 */
 
int centerdapi_register(int iHandle, int iDst);

/**
  register to centerd for muti appid.
  
  @param piHandle 
  @param iDst 
  @param iAppid
  @return 0 if successful, or -1 if an error occurred
 */
 
int centerdapi_register_muti(int iHandle, int iDst, int iAppid);

/**
  broad msg to all agents who's busid equal param 5.
  
  @param piHandle 
  @param iDst 
  @param pszBuff	agent body buff
  @param iBuff	agent body len
  @param iBusid
  @return 0 if successful, or -1 if an error occurred
 */
 
int centerdapi_broadcast(int iHandle, int iDst, char *pszBuff, int iBuff, int iBusid); 

/**
  broad msg to single agent who's busid equal param 5.
  
  @param piHandle 
  @param iDst 
  @param pszBuff	agent body buff
  @param iBuff	agent body len
  @param iBusid
  @param uiDestIP dest ip is set in TAGENTHEAD
  @return 0 if successful, or -1 if an error occurred
 */
 
int centerdapi_unicast(int iHandle, int iDst, char *pszBuff, int iBuff, int iBusid, unsigned int uiDestIP);

/**
  notify updating file to agent
  
  @param piHandle 
  @param iDst 
  @param filename which will send to agent
  @param uiBusid
  @param uiDestIP	-1 broadcast
  @return 0 if successful, or -1 if an error occurred
 */

int centerdapi_file_notify(int iHandle, int iDst, const char *pszFilename, unsigned int uiBusid, unsigned int uiDestIP);

/**
  transfer file to agent
  
  @param piHandle 
  @param iDst 
  @param pszBody
  @param iBuff
  @param uiBusid
  @return 0 if successful, or -1 if an error occurred
 */

int centerdapi_file_transfer(int iHandle, int iDst, char *pszBody, int iBuff, unsigned int uiBusid, unsigned int uiDestIP);

/**
  transfer file commit and report to master
  
  @param piHandle 
  @param iDst 
  @param pszBuff
  @param iBuff
  @param  struct tagFile result when file trans succ
  @return 0 if successful, or -1 if an error occurred
 */
int centerdapi_file_commit(int iHandle, int iDst, char *pszBuff, int iBuff, struct tagFile *pstFile);

#ifdef __cplusplus
}
#endif

#endif /* _CENTERDAPI_H_ */

