//---------------------------------------------------------------------------------------------------------
// Desc: ʹ��curl��ʵ�ֵ������֧࣬��ftp��httpЭ��
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
    // ������ϢidΪmsgcallback��owner���ڣ�wparam=MsgCallbacks��lparamΪ����
    enum MsgCallbacks
    {                           // lParam������
        CbStartupDownload,      // �޲���
        CbEndDownload,          // ���ؽ��CURLcode
		CbBreakDownload,        // ��;�������

        CbDownloadTotal,        // ��Ҫ���ص����ݴ�С
        CbDownloadNow,          // Ŀǰ���ص����ݴ�С
        CbDownloadProgress,     // ���ؽ���(0-100)
        CbDownloadDiskFull,     // ����ʱ��������

        CbUploadTotal,          // ��Ҫ�ϴ��Ĵ�С
        CbUploadNow,            // Ŀǰ�ϴ��Ĵ�С
        CbUploadProgress,       // �ϴ�����(0-100)
		CbDownloadSpeed,		//�����ٶ�
    };

    CDownloader(HWND owner, unsigned int msgcallback);
    virtual ~CDownloader();

    // �Ƿ�׼����������������ɣ������Խ����µ���������
    bool IsDone()                
	{
		return m_bDone; 
	}
    // ����Զ��ftp��http�ĸ�·��
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
    // ���ñ��ر������غ��ļ���·��
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

    // ��ʼ�����ļ���������ϴ����ص��ļ�û����ɣ�����false����֧������
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
    // �ڲ�״̬
    enum Status
    {
        Idle,
        Startup,
        Progressing,
    };

    HWND                m_howner;               // �������ڣ����ڴ�����Ϣ����״̬֪ͨ
    unsigned int        m_msgcallback;          // ֪ͨ��������ʱ���ݵ���Ϣid
    Status              m_status;

    CURL*               m_curl;                 // curl���Ľṹ
	char				m_szCulrErrDesc[CURL_ERROR_SIZE];	// CURL���Ӵ����ʱ�������
    CURLcode            m_res;                  // ���ؽ��CURL_OK��ʾ�ɹ�������ʧ��
    HANDLE              m_hEvent;               // ֪ͨ�����߳�ȡ������״̬����ʼ����
    HANDLE              m_hThread;              // �����߳̾��
    bool		        m_bEndThread;           // �Ƿ��˳��߳�
    bool		        m_bDone;                // �Ƿ�׼������
    bool		        m_bNotify;              // �Ƿ������ؽ�����Ϣ��������
	bool				m_bPause;              // �Ƿ������ؽ�����Ϣ��������

    char                m_Url[MAX_PATH];        // Զ��ftp��http�ĸ�·��
    char                m_Path[MAX_PATH];       // ���ر������غ��ļ���·��
    char                m_Name[MAX_PATH];       // ���ر����ļ�������·�����ļ���
    FILE*               m_fp;                   // �ļ�ָ��
    unsigned int        m_ResumeFrom;           // �ϵ���������ʼ�ֽڣ�

    static int          sm_NumOfInsts;          // ʵ�����˶��ٴδ���
	std::string			m_fileUrl;

    static int WriteFunction(void *buffer, size_t size, size_t nmemb, void *clientp);
    static int ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
    static void DownloadFunction(void *clientp);
};

