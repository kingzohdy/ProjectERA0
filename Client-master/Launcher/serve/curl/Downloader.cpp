//---------------------------------------------------------------------------------------------------------
// Desc: 使用curl库实现的下载类，支持ftp、http协议
// Auth: saniko
// Date: 2009-07-16
// Last:
// Copyright (c) 2009 shangyoo Entertainment All rights reserved.
//-------------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include <assert.h>
#include <process.h>
#include "LauncherDlg.h"
#include "curl/Downloader.h"
#include "util/Utils.h"

extern const char* strip_name(const char *full_path);
int CDownloader::sm_NumOfInsts = 0;

int CDownloader::WriteFunction(void *buffer, size_t size, size_t nmemb, void *clientp)
{
    CDownloader *_this = static_cast<CDownloader *> (clientp);
    assert(_this != NULL);

    if(!_this->m_fp)
    {
        if(_this->m_ResumeFrom != 0)
            _this->m_fp = fopen(_this->m_Name, "rb+");
        else
            _this->m_fp = fopen(_this->m_Name, "wb");

        if(!_this->m_fp)
            return -1;
        fseek(_this->m_fp, _this->m_ResumeFrom, SEEK_SET);
    }
    int wsize = (int)fwrite(buffer, size, nmemb, _this->m_fp);
    if(wsize != nmemb)
        _this->PostMessage(CbDownloadDiskFull, 0);

    return wsize;
}

#define KB 1024

int CDownloader::ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    CDownloader *_this = static_cast<CDownloader *> (clientp);

    assert(_this != NULL);
	__int64 nNowFileSize = file_size( _this->GetFullPath() );
    if(_this->m_status == Startup)
    {
        _this->m_status = Progressing;
    }
    if(dltotal > 0)
	{
		unsigned int fileSize = ( (unsigned int)dltotal + _this->m_ResumeFrom )/KB;
		_this->PostMessage(CbDownloadTotal, (LPARAM) fileSize);

		static __int64 nTotalFileSize = (__int64)(_this->m_ResumeFrom + dltotal);
		_this->PostMessage(CbDownloadProgress, (LPARAM) ((unsigned int)(nNowFileSize/KB)) );
	}

    /*if(ultotal > 0)
        _this->PostMessage(CbUploadProgress, (LPARAM) (ulnow / ultotal * 100));
    if(ulnow > 0)
        _this->PostMessage(CbUploadNow, (LPARAM) ulnow);*/

    return 0;
}

void CDownloader::DownloadFunction(void *clientp)
{
    CDownloader *_this = static_cast<CDownloader *> (clientp);
    assert(_this != NULL);

    for(;;)
    {
        switch(WaitForSingleObject(_this->m_hEvent, INFINITE))
        {
        case WAIT_FAILED:
            return;

        case WAIT_OBJECT_0:
            break;

        default:
            assert(FALSE && "WaitForSingleObject failed!");
        }

        if(_this == NULL || _this->m_bEndThread)
            break;

        assert(!_this->m_bDone);
		
		if ( _this->m_curl == NULL )
		{
			return;
		}

        _this->m_status = Startup;
        _this->PostMessage(CbStartupDownload, 0);

        _this->m_res = curl_easy_perform(_this->m_curl);
		if ( _this->m_res != 0 && _this->m_szCulrErrDesc[0] != 0 ) // CURL_OK
		{
			string errDesc = string( "connect to " ) + _this->m_fileUrl + "failed， " + _this->m_szCulrErrDesc;
			//TODO HERE 暂时先去这个报错
			//::MessageBox( _this->m_howner, errDesc.c_str(), "错误", MB_OK );
		}

        if(_this->m_fp)
        {
            fclose(_this->m_fp);
            _this->m_fp = NULL;
        }
        _this->m_status = Idle;

        _this->PostMessage(CbEndDownload, _this->m_res);
        _this->m_bDone = true;
		_this->m_fileUrl.clear();
    }
}

CDownloader::CDownloader(HWND owner, unsigned int msgcallback)
    : m_bEndThread(false), m_bDone(true), m_bNotify(false), m_fp(NULL), m_howner(owner), m_msgcallback(msgcallback),
	m_status( Idle ), m_bPause( false ),m_ResumeFrom( 0 )
{
	initCurl();
    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_res = CURLE_OK;
    assert(m_hEvent != INVALID_HANDLE_VALUE);
	
    m_hThread = (HANDLE) _beginthread(&DownloadFunction, 1024 * 1024, this);
    assert(m_hThread != INVALID_HANDLE_VALUE);
	memset( m_szCulrErrDesc, 0, sizeof( char ) * CURL_ERROR_SIZE );
}

BOOL CDownloader::PostMessage(WPARAM wParam, LPARAM lParam)
{
    if(m_bNotify)
        return ::PostMessage(m_howner, m_msgcallback, wParam, lParam);
    else
        return TRUE;
}

void CDownloader::initCurl()
{
	// 第一次构造，初始化curl库
	if(sm_NumOfInsts++ == 0)
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}

	m_curl = curl_easy_init();
	assert( m_curl != NULL );

	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &WriteFunction);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

	curl_easy_setopt( m_curl, CURLOPT_PROGRESSFUNCTION, &ProgressFunction);
	curl_easy_setopt( m_curl, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt( m_curl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt( m_curl, CURLOPT_ERRORBUFFER, m_szCulrErrDesc );
	curl_easy_setopt( m_curl, CURLOPT_FOLLOWLOCATION , 1);

	// Switch on full protocol/debug output
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1);
}

void CDownloader::cleanCurl()
{
	curl_easy_cleanup(m_curl);
	m_curl = NULL;

	// 最后一次析构，清理curl库
	if(--sm_NumOfInsts == 0)
	{
		curl_global_cleanup();
	}
}

bool CDownloader::Download(const char *filename, unsigned int resume_from, bool notify)
{
    if(!m_bDone)
	{
		return false;
	}

    m_bDone = false;
    m_bNotify = notify;

    char filepath[MAX_PATH];
	string strFileName( filename );
    if(strchr(filename, ':'))
    {
        strncpy_s(filepath,   filename,       sizeof(filepath));
        _snprintf_s(m_Name,   sizeof(m_Name), "%s/%s", m_Path, strip_name(filename));
		//string_replace( m_Name, '/', '\\' );
    }
    else
    {
        _snprintf_s(filepath, sizeof(filepath), "%s/%s", m_Url, filename);
        _snprintf_s(m_Name,   sizeof(m_Name),   "%s/%s", m_Path, 
					strFileName.substr( strFileName.find_last_of( "/\\") + 1 ).c_str() );
		m_fileUrl = filepath;
    }
	
	CreateMultipleDirectory( m_Path );

    m_ResumeFrom = resume_from;

    curl_easy_setopt(m_curl, CURLOPT_URL, filepath);
    curl_easy_setopt(m_curl, CURLOPT_RESUME_FROM, resume_from);

    SetEvent(m_hEvent);
    return true;
}

bool CDownloader::IsPaused()
{
	return m_bPause;
}

void CDownloader::StopDownload()
{
	curl_easy_pause( m_curl, CURLPAUSE_RECV );
	m_bPause = true;
	this->PostMessage( CbBreakDownload, m_res );
}

void CDownloader::ContinueDownload()
{
	curl_easy_pause( m_curl, CURLPAUSE_RECV_CONT );
}

double CDownloader::getDownloadSpeed()
{
	double dReturn = 0;
	curl_easy_getinfo(m_curl,CURLINFO_SPEED_DOWNLOAD,&dReturn);
	return dReturn;
}

CDownloader::~CDownloader()
{
    m_bEndThread = true;
    SetEvent(m_hEvent);

    CloseHandle(m_hEvent);
	
	cleanCurl();
}
