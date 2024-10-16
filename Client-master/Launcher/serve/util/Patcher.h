#pragma once
#include "util/locked.h"

class CPatcher
{
public:
    CPatcher(HWND hOwner, unsigned int msgcallback);
    virtual ~CPatcher();

    enum MsgCallbacks
    {                                   // lParam的意义
        CbDecompressStartup     = 100,  // 开始解压
        CbDecompressProgress,           // 解压进度(0-100)
        CbDecompressDone,               // 解压完成
        CbUpdateStartup,                // 开始更新
        CbUpdateProgress,               // 更新进度(0-100)
        CbUpdateDiskFull,               // 更新时磁盘已满
        CbUpdateDone,                   // 更新完成
    };

    bool IsDone()                       { return m_bDone; }
    bool IsFinish()                     { return m_bIsFinish; }

    bool ApplyPatch(const string& fn, int ver, int target_ver, int launcher_ver, const string& patch_path, const string& dst_path, bool* updated_self, int offset = 0);

protected:
    BOOL PostMessage(WPARAM wParam, LPARAM lParam);

private:
    HWND                m_hOwner;               // 宿主窗口句柄
    unsigned int        m_msgcallback;          // 通知宿主窗口时传递的消息id

    HANDLE              m_hEvent;               // 通知线程取消挂起状态，开始更新
    HANDLE              m_hThread;              // 更新线程句柄
    locked<bool>        m_bEndThread;           // 是否退出线程
    locked<bool>        m_bDone;                // 是否准备就绪
    locked<bool>        m_bIsFinish;            // 是否成功更新

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
