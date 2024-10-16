#pragma once
#include "util/locked.h"

class CPatcher
{
public:
    CPatcher(HWND hOwner, unsigned int msgcallback);
    virtual ~CPatcher();

    enum MsgCallbacks
    {                                   // lParam������
        CbDecompressStartup     = 100,  // ��ʼ��ѹ
        CbDecompressProgress,           // ��ѹ����(0-100)
        CbDecompressDone,               // ��ѹ���
        CbUpdateStartup,                // ��ʼ����
        CbUpdateProgress,               // ���½���(0-100)
        CbUpdateDiskFull,               // ����ʱ��������
        CbUpdateDone,                   // �������
    };

    bool IsDone()                       { return m_bDone; }
    bool IsFinish()                     { return m_bIsFinish; }

    bool ApplyPatch(const string& fn, int ver, int target_ver, int launcher_ver, const string& patch_path, const string& dst_path, bool* updated_self, int offset = 0);

protected:
    BOOL PostMessage(WPARAM wParam, LPARAM lParam);

private:
    HWND                m_hOwner;               // �������ھ��
    unsigned int        m_msgcallback;          // ֪ͨ��������ʱ���ݵ���Ϣid

    HANDLE              m_hEvent;               // ֪ͨ�߳�ȡ������״̬����ʼ����
    HANDLE              m_hThread;              // �����߳̾��
    locked<bool>        m_bEndThread;           // �Ƿ��˳��߳�
    locked<bool>        m_bDone;                // �Ƿ�׼������
    locked<bool>        m_bIsFinish;            // �Ƿ�ɹ�����

    //{
    string              m_fn;
    int                 m_ver;
    int                 m_target_ver;
    int                 m_launcher_ver;
    string              m_patch_path;
    string              m_dst_path;
    bool*               m_updated_self;
    int                 m_offset;
    //}

	struct UpdateFileHead;
	struct UpdateSec;

    bool ExecUpdate(const string& fn, int ver, int target_ver, const string& patch_path, const string& dst_path, bool& updated_self, int offset);

    bool UpdateFile(UpdateSec* &pSec, const string& dst_path,const string& fn, const char *buf, unsigned int decmp_len);
    bool ExecPatch(FILE *f,BYTE *data,DWORD len, const char *buf, unsigned int decmp_len);

    bool DecompressPackfile(const string& fn, FILE *f, size_t size, char *buf, size_t buf_size);

    static void PatcherFunction(void *pthis);
};
