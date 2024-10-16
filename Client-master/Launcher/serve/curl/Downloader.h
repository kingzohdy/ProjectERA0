//---------------------------------------------------------------------------------------------------------
// Desc: 使用curl库实现的下载类，支持ftp、http协议
// Auth: saniko
// Date: 2009-07-16
// Last:
// Copyright (c) 2009 shangyoo Entertainment All rights reserved.
//-------------------------------------------------------------------------------------------------------------
#pragma once
#define CURL_STATICLIB
#include "curl/curl.h"
#include "util/locked.h"

class CDownloader
{
public:
    // 传递消息id为msgcallback给owner窗口，wparam=MsgCallbacks，lparam为参数
    enum MsgCallbacks
    {                           // lParam的意义
        CbStartupDownload,      // 无参数
        CbEndDownload,          // 下载结果CURLcode
		CbBreakDownload,        // 中途打断下载

        CbDownloadTotal,        // 需要下载的数据大小
        CbDownloadNow,          // 目前下载的数据大小
        CbDownloadProgress,     // 下载进度(0-100)
        CbDownloadDiskFull,     // 下载时磁盘已满

        CbUploadTotal,          // 需要上传的大小
        CbUploadNow,            // 目前上传的大小
        CbUploadProgress,       // 上传进度(0-100)
		CbDownloadSpeed,		//下载速度
    };

    CDownloader(HWND owner, unsigned int msgcallback);
    virtual ~CDownloader();

    // 是否准备就绪（或下载完成），可以进行新的下载任务
    bool IsDone()                
	{
		return m_bDone; 
	}
    // 设置远程ftp、http的根路径
    void SetUrl(const char *url)    
	{ 
		strncpy_s(m_Url, url, sizeof(m_Url)); 
		if (m_Url[strlen(m_Url) - 1] == '/')
		{
			m_Url[strlen(m_Url) - 1] = '\0';
		}
	}
	
	const char* GetUrl() const
	{
		return m_Url;
	}
    // 设置本地保存下载后文件的路径
    void SetLocalPath(const char *path)  
	{ 
		strncpy_s(m_Path, path, sizeof(m_Path)); 
	}
	
	const char* GetFullPath( ) const
	{
		return m_Name;
	}

	const char* getCurlErrorDesc()
	{
		return m_szCulrErrDesc;
	}

    // 开始下载文件（如果有上次下载的文件没有完成，返回false），支持续传
    bool Download(const char *filename, unsigned int resume_from = 0, bool notify = true);
	bool IsPaused();
	void StopDownload();
	void ContinueDownload();
	double getDownloadSpeed();
    CURLcode GetResult()            
	{ 
		return m_res; 
	}

protected:
    BOOL PostMessage(WPARAM wParam, LPARAM lParam);

private:
	void initCurl();
	void cleanCurl();
    // 内部状态
    enum Status
    {
        Idle,
        Startup,
        Progressing,
    };

    HWND                m_howner;               // 宿主窗口，用于传递消息进行状态通知
    unsigned int        m_msgcallback;          // 通知宿主窗口时传递的消息id
    Status              m_status;

    CURL*               m_curl;                 // curl核心结构
	char				m_szCulrErrDesc[CURL_ERROR_SIZE];	// CURL连接错误的时候的做法
    CURLcode            m_res;                  // 下载结果CURL_OK表示成功，否则失败
    HANDLE              m_hEvent;               // 通知下载线程取消挂起状态，开始下载
    HANDLE              m_hThread;              // 下载线程句柄
    bool		        m_bEndThread;           // 是否退出线程
    bool		        m_bDone;                // 是否准备就绪
    bool		        m_bNotify;              // 是否发送下载进入消息给父窗口
	bool				m_bPause;              // 是否发送下载进入消息给父窗口

    char                m_Url[MAX_PATH];        // 远程ftp、http的根路径
    char                m_Path[MAX_PATH];       // 本地保存下载后文件的路径
    char                m_Name[MAX_PATH];       // 本地保存文件的完整路径和文件名
    FILE*               m_fp;                   // 文件指针
    unsigned int        m_ResumeFrom;           // 断点续传（开始字节）

    static int          sm_NumOfInsts;          // 实例化了多少次此类
	std::string			m_fileUrl;

    static int WriteFunction(void *buffer, size_t size, size_t nmemb, void *clientp);
    static int ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
    static void DownloadFunction(void *clientp);
};

