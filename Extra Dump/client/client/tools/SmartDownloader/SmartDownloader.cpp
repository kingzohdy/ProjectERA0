// SmartDownloader.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "SmartDownloader.h"
#include "Downloader.h"
#include "XMLData.h"
#include "des.h"
#include "md5.h"
#include <memory.h>
#include <iostream>
#include <vector>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib")
#define MAX_LOADSTRING 100

struct PackageHeader
{
	unsigned int id;
	int version;
	int numfiles;
	int keyoffset;
	int keylen;
	int dataoffset;
	int datalen;
};

struct PkgFileInfo
{
	unsigned int key1;
	unsigned int key2;
	unsigned int offset;
	unsigned int originsize;
	unsigned int packsize; //未压缩文件packsize == originsize
	unsigned int flags; //bit0: compress
};

struct MapData 
{
	bool bStop;
	unsigned int processId;
};

struct PkgFileMd5
{
	char md5[16];
};

#define SC_EXIST	4
// Global Variables:
HINSTANCE			hInst;								// current instance
std::vector<PkgFileMd5> gSmartClientFileMd5;
std::vector<char>	gSmartClientFileInfo;
std::string			gStrDownUrl;
PackageHeader		gCommonPackageHeader;
size_t				gDownloadIdx;
volatile bool		gbQuit;
std::vector<PkgFileInfo> gCommonHeaderInfo;
HANDLE				gFileMapping;
DWORD				threadId;
unsigned int		gLastFlushTime;
Downloader			downloader;

const char GuidName[] = "{E7F8A607-89C0-4689-8C76-B7D28EA56E08}";
const char MutexName[] = "{EF6C4BE0-ADCF-445e-8425-E883C5AA8B9C}";

// Forward declarations of functions included in this code module:
BOOL				InitInstance(HINSTANCE, int);
void				DownloadNextFile();
DWORD WINAPI		ThreadDownloadPatch( LPVOID param );
int APIENTRY _tWinMain(HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR lpCmdLine,
						int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	if(AllocConsole())
	{
		SetConsoleTitle("Debug Window");
		freopen("CONOUT$","w", stdout);
		freopen("CONIN$", "r", stdin);
	}
#endif
	if( strcmp( lpCmdLine,"StartDownload" ) != 0 )
	{
		return 0;
	}

	if (!InitInstance (hInstance, nCmdShow))
	{
		return 0;
	}

	DWORD dwThreadId;
	gLastFlushTime = timeGetTime();
	HANDLE hThread = ::CreateThread(
		NULL,
		0, 
		ThreadDownloadPatch,
		NULL, 
		0, 
		&dwThreadId);

	while(true)
	{
		MSG kMsg;
		if (PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
		{
			if (kMsg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&kMsg);
			DispatchMessage(&kMsg);
		}
		else
		{
			if( !gbQuit )
			{
				::Sleep( 10 );
				MapData* pBuf = (MapData*)MapViewOfFile(gFileMapping, FILE_MAP_READ, 0, 0, sizeof(MapData));
				if( pBuf )
				{
					gbQuit = pBuf->bStop;
					UnmapViewOfFile( pBuf );
				}
				if( gbQuit )
				{
					downloader.AbortDownload();
					break;
				}
			}
		}
	}
	if(gFileMapping!=NULL)
	{
		CloseHandle(gFileMapping);
	}
	gFileMapping = NULL;
	::WaitForSingleObject(hThread, INFINITE);
	::CloseHandle(hThread);
	return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	gbQuit = false;
	hInst = hInstance; // Store instance handle in our global variable
	threadId = GetCurrentThreadId();
	XMLData data;
	bool bSmartClient = false;
	if(data.loadFile("client.cfg"))
	{
		XMLNode	root = data.getRootNode();
		if (root.hasChild("SmartClient"))
		{
			XMLNode node	= root.getChild("SmartClient");
			bSmartClient	= node.attribToBool("flag");
		}
	}
	if( !bSmartClient )
	{
		return FALSE;
	}

	// 判断是否有微端在运行
	HANDLE hMutex = CreateMutex( NULL, FALSE, MutexName );
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		CloseHandle(hMutex);
		return FALSE;
	}
	CloseHandle(hMutex);
	hMutex = NULL;

	//读取url
	char* pszDecryptBuffer	= getDecryptFileBuffer( "serverlist.data", CLIENT_JIEMI );
	if ( pszDecryptBuffer == NULL )
	{
		return FALSE;
	}
	XMLData xmldata;
	if( !xmldata.loadBuffer( pszDecryptBuffer, strlen( pszDecryptBuffer )) )
	{
		delete[] pszDecryptBuffer;
		return FALSE;
	}
	delete[] pszDecryptBuffer;
	XMLNode pRootNode = xmldata.getRootNode();
	XMLNode pChildNode = pRootNode.getChild( "SmartClient" );
	if( pChildNode.isNull() )
	{
		return FALSE;
	}
	gStrDownUrl = pChildNode.attribToString("server") + std::string("/common.pkg");

	// 是否有common.pkg文件
	FILE *comFile = fopen("data\\common.pkg", "rb");
	gCommonHeaderInfo.clear();
	gSmartClientFileInfo.clear();
	if (comFile)
	{
		if( fread(&gCommonPackageHeader,sizeof(PackageHeader),1,comFile) != 1 )
		{
			fclose(comFile);
			return FALSE;
		}
		gSmartClientFileInfo.resize( gCommonPackageHeader.numfiles );
		memset(&gSmartClientFileInfo[0], 0, gCommonPackageHeader.numfiles*sizeof(char));
		// 读取头信息
		gCommonHeaderInfo.resize( gCommonPackageHeader.numfiles );
		fseek( comFile, gCommonPackageHeader.keyoffset, SEEK_SET );
		if( fread(&gCommonHeaderInfo[0], gCommonPackageHeader.numfiles * sizeof(PkgFileInfo), 1, comFile) != 1 )
		{
			fclose(comFile);
			return FALSE;
		}
	}else{
		//没有common.pkg的话，不下载
		// 看看是否有attach，或者attach的下载部分文件
		return FALSE;
	}
	
	FILE *scfiFile = fopen("data\\SCFI.info", "rb");
	if (scfiFile)
	{
		size_t size = 0; 
		fread(&size, sizeof(size), 1, scfiFile);
		if( size != gCommonPackageHeader.numfiles )
		{
			return FALSE;
		}
		fread(&gSmartClientFileInfo[0], size * sizeof(char), 1, scfiFile);
		fclose(scfiFile);
	}
	FILE *md5File = fopen("data\\common.md5", "rb");
	if (md5File)
	{
		fseek(md5File, 0, SEEK_END);
		size_t len = ftell(md5File);
		size_t size = len / sizeof(PkgFileMd5);
		if (gCommonPackageHeader.numfiles == size)
		{
			gSmartClientFileMd5.resize(size);
			fseek(md5File, 0, SEEK_SET);
			fread(&gSmartClientFileMd5[0], size * sizeof(PkgFileMd5), 1, md5File);
		}
		fclose(md5File);
	}
	gDownloadIdx = 0;
	gFileMapping = OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE, FALSE, GuidName);
	if( gFileMapping != NULL )
	{
		return FALSE;
	}
	gFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, sizeof(MapData), GuidName);
	if( gFileMapping == NULL )
	{
		return FALSE;
	}
	MapData* pBuf = (MapData*)MapViewOfFile(gFileMapping, FILE_MAP_WRITE, 0, 0, sizeof(MapData));
	if( pBuf )
	{
		pBuf->bStop = false;
		pBuf->processId = GetCurrentProcessId();
	}
	UnmapViewOfFile( pBuf );
	return TRUE;
}

DWORD WINAPI ThreadDownloadPatch( LPVOID param )
{
	try{
		DownloadNextFile();
	}	
	catch( ... ){
	}
	
	return 0;
}


void DownloadNextFile()
{
	if( gDownloadIdx >= gSmartClientFileInfo.size() )
	{
		PostThreadMessage(threadId,WM_QUIT,0,0);
		return;
	}
	downloader.AbortDownload();
	
	while( gDownloadIdx < gSmartClientFileInfo.size() && !gbQuit )
	{
		if( gSmartClientFileInfo[gDownloadIdx ] != SC_EXIST )
		{
			const PkgFileInfo &info = gCommonHeaderInfo[gDownloadIdx];
			if( downloader.BlockDownload( "data\\common.pkg", gStrDownUrl.c_str(), info.offset, info.packsize ) )
			{
				gSmartClientFileInfo[gDownloadIdx] = SC_EXIST;
				if (!gSmartClientFileMd5.empty())
				{
					FILE *file = fopen("data\\common.pkg", "rb");
					if (file)
					{
						fseek(file, info.offset, SEEK_SET);
						void *buff = malloc(info.packsize);
						fread(buff, info.packsize, 1, file);
						fclose(file);
						char md5[16];
						Md5Calc(md5, (char *)buff, info.packsize);
						if (memcmp(md5, gSmartClientFileMd5[gDownloadIdx].md5, sizeof(md5)) != 0)
						{
							gSmartClientFileInfo[gDownloadIdx] = 0;
						}
						free(buff);
					}
				}
			}
		}
		gDownloadIdx ++;
		if( timeGetTime() - gLastFlushTime > 60*1000 )
		{
			downloader.FlushData();
			FILE *scfiFile = fopen( "data\\SCFI.info", "wb" );
			if( scfiFile != NULL )
			{
				size_t len = gSmartClientFileInfo.size();
				fwrite( &len, sizeof(int), 1, scfiFile );
				fwrite(&gSmartClientFileInfo[0], sizeof(char) * gSmartClientFileInfo.size(), 1, scfiFile);
				fclose(scfiFile);
			}
		}
	}

	downloader.Close();
	//下载完成
	PostThreadMessage(threadId,WM_QUIT,0,0);
}
