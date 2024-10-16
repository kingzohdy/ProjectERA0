#include "ApexProxyLib.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include "windows.h"
#else
#include <dlfcn.h>
#endif

//#define UsingStaticLib

typedef int    SH_START_PROC();
typedef void   SH_DONE_PROC(void);

typedef int    SH_GETDATA_PROC( char* pData,int nBuffLen,char * pcDataType,unsigned int* pnSendID,int * pRetLen );
typedef void   SH_SENDDATA_PROC(char cMsgId,unsigned int nId,const char * pBuffer,int nLen);

typedef void*  SH_SETFUNC_PROC(void * pFunc,int nFuncFlag);

typedef int    SH_SET_CONFIG_PATH(const char *szConfigPath);  


static SH_DONE_PROC *		g_sh_exit		= NULL;
static SH_SETFUNC_PROC *	g_sh_setfunc	= NULL;
static SH_START_PROC *		g_sh_start		= NULL;
static SH_SENDDATA_PROC *   g_sh_recv       = NULL; 
static SH_GETDATA_PROC *	g_sh_getdata    = NULL;
static SH_SET_CONFIG_PATH * g_sh_set_config_path = NULL;  

#ifdef WIN32
static HMODULE g_hASclient			= NULL;
#else
static void  * g_hASclient			= NULL;     
#endif


static void cleanup_sh()
{
	if ( g_sh_exit )
	{
		g_sh_exit();
	}
 
	if(NULL!=g_hASclient)
#ifdef WIN32
		FreeLibrary(g_hASclient);
#else
			dlclose(g_hASclient);
#endif

	g_hASclient		= NULL;
	g_sh_exit		= NULL;
	g_sh_start		= NULL;
    g_sh_getdata	= NULL;
	g_sh_setfunc	= NULL;	
}

void		s_end()
{
	cleanup_sh();
}

int		s_getData_FromApex( char* pData, int nBuffLen, char * pcDataType, unsigned int * pnSendID, int * pRetLen )
{
	int nRet = 1;
	if ( g_sh_getdata )
	{
		nRet = g_sh_getdata( pData, nBuffLen, pcDataType, pnSendID, pRetLen );
	}
	return nRet;
}
void		s_sendData_ToApex( char cMsgId, unsigned int nUserId, const char * pBuff, int nBuffLen )
{
	if ( g_sh_recv )
	{
		g_sh_recv( cMsgId, nUserId, pBuff, nBuffLen );
	}
}
void *   s_setFunc( void * pFunc, int nFuncFlag )
{
	void * pRet = NULL;
	if( g_sh_setfunc )
		pRet = g_sh_setfunc( pFunc, nFuncFlag );
	return pRet;
}
#ifdef UsingStaticLib
typedef long (*_FUNC_PROXY_REC)(char  cMsgId, int  nId, const char  *pBuffer,int  nLen);
// apexproxy send data to gameserver by this function
typedef long (*_FUNC_PROXY_SEND)(int  nId, const char  *pBuffer,int  nLen);

// apexproxy kicks player by this function
typedef long (*_FUNC_PROXY_KILLUSER)( int  nId,char  * sChar,int  action);
int						sh_load_start_v2();
void						sh_load_exit(void);
void						sh_setfunc(void * IN pFunc,int IN iFuncFlag);
int						sh_recv(char cMsgId,unsigned int nId,const char * pBuffer,int nLen);
int						sh_getdata(char* pData,int nBuffLen,char * pcDataType,unsigned int* pnSendID,int * pRetLen);
int            sh_set_config_path(const char *szConfigPath);  
#endif
int		s_start(const char *szConfigPath)
{
#ifdef UsingStaticLib
	g_sh_exit		= (SH_DONE_PROC *)sh_load_exit;
	g_sh_start		= (SH_START_PROC *)sh_load_start_v2;
	g_sh_getdata	= (SH_GETDATA_PROC *)sh_getdata;
	g_sh_recv		= (SH_SENDDATA_PROC *)sh_recv;
	g_sh_setfunc    = (SH_SETFUNC_PROC *)sh_setfunc;
  g_sh_set_config_path = (SH_SET_CONFIG_PATH *)sh_set_config_path;  
#else
#ifdef WIN32
	if (!g_hASclient)
	{
		g_hASclient		= LoadLibraryA( "ApexProxy.dll" );
		if ( !g_hASclient ) 
			return -2;
		g_sh_exit		= (SH_DONE_PROC *) GetProcAddress( g_hASclient, "sh_load_exit" );
		g_sh_start		= (SH_START_PROC *) GetProcAddress( g_hASclient, "sh_load_start_v2" );
		g_sh_getdata	= (SH_GETDATA_PROC *) GetProcAddress( g_hASclient, "sh_getdata" );
		g_sh_recv		= (SH_SENDDATA_PROC *) GetProcAddress( g_hASclient, "sh_recv" );
		g_sh_setfunc	= (SH_SETFUNC_PROC *) GetProcAddress( g_hASclient, "sh_setfunc" );
    g_sh_set_config_path = (SH_SET_CONFIG_PATH *)GetProcAddress( g_hASclient, "sh_set_config_path" );  //add by xxc 2013S1

	}
#else
	if (!g_hASclient) 
	{
		g_hASclient = dlopen( "./ApexProxy.so",RTLD_NOW );
		const char *dlError = dlerror();
		if( dlError ) 
		{
			printf( "%s\n", dlError );
		}
		if (!g_hASclient) 
			return -2;
		g_sh_exit = (SH_DONE_PROC *) dlsym( g_hASclient, "sh_load_exit" );
		if( dlError ) 
		{
			printf( "%s\n", dlError ); 		
		}
		g_sh_start = (SH_START_PROC *) dlsym( g_hASclient, "sh_load_start_v2" );
		if( dlError ) 
		{
			printf( "%s\n", dlError ); 	
		}
		g_sh_getdata = (SH_GETDATA_PROC *) dlsym(g_hASclient, "sh_getdata");
		if( dlError ) 
		{
			printf( "%s\n", dlError ); 	
		}
		g_sh_recv  = ( SH_SENDDATA_PROC *) dlsym( g_hASclient, "sh_recv" );
		if( dlError ) 
		{
			printf( "%s\n", dlError ); 	
		}
		g_sh_setfunc  = ( SH_SETFUNC_PROC *) dlsym( g_hASclient, "sh_setfunc" );
		if( dlError ) 
		{
			printf( "%s\n", dlError ); 	
		}
       
        g_sh_set_config_path  = ( SH_SET_CONFIG_PATH *) dlsym( g_hASclient, "sh_set_config_path" );
        if( dlError ) 
        {
            printf( "%s\n", dlError ); 	
        }
        //
	}
#endif
	if ( !g_sh_exit || !g_sh_start || !g_sh_getdata || !g_sh_recv || !g_sh_setfunc || !g_sh_set_config_path)
	{
		if(g_hASclient)
#ifdef WIN32
			FreeLibrary(g_hASclient);
#else
			dlclose(g_hASclient);
#endif

		g_hASclient		=NULL;
		g_sh_exit		= NULL;
		g_sh_start		= NULL;
		g_sh_recv		= NULL;
		g_sh_getdata	= NULL;
        g_sh_set_config_path = NULL;  
		return -3;
	}
#endif
    int nRet = 0;
    nRet = g_sh_set_config_path(szConfigPath); 
    if (nRet == 0) {
        nRet = g_sh_start();
    }
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
//typedef long (*_FUNC_S_SEND) ( unsigned int nId, const char * pBuffer, int nLen );
//typedef long (*_FUNC_S_KILLUSER) ( unsigned int nId );
//extern "C" void		s_setFunc( void * pFunc, int nFuncFlag );

