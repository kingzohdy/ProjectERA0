#include "stdafx.h"
#include "Patcher.h"
#include "util/Utils.h"
#include "Launcher.h"
//#include "bzlib.h"
#include <io.h>
#include <assert.h>
#include <direct.h>
#include <algorithm>
#include <process.h>
#include <list>
using namespace std;


// ���°��ļ�ͷ
struct CPatcher::UpdateFileHead
{
	DWORD flag;				// ��־ 'DTUD' ���DTBZ��ʾ��bzip2
	DWORD ver;				// �汾��
	DWORD decompress_len;	// �������ݽ�ѹ��ĳ���
};

//��Դ���½�
struct CPatcher::UpdateSec
{
	DWORD flag;				// ��־
	DWORD file_len;			// �ļ���С
	DWORD pathname_len;		// ·��������
	char pathname[1];		// ·����
};

CPatcher::CPatcher(HWND hOwner, unsigned int msgcallback) : m_hOwner(hOwner), m_msgcallback(msgcallback),
    m_bEndThread(false), m_bDone(true), m_bIsFinish(false), m_hThread(INVALID_HANDLE_VALUE), m_hEvent(INVALID_HANDLE_VALUE)
{
    m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(m_hEvent != INVALID_HANDLE_VALUE);

    m_hThread = (HANDLE) _beginthread(&PatcherFunction, 1024 * 1024, this);
    assert(m_hThread != INVALID_HANDLE_VALUE);
}

CPatcher::~CPatcher()
{
    m_bEndThread = true;
    SetEvent(m_hEvent);
    WaitForSingleObject(m_hThread, INFINITE);

    CloseHandle(m_hEvent);
}

BOOL CPatcher::PostMessage(WPARAM wParam, LPARAM lParam)
{
    return ::PostMessage(m_hOwner, m_msgcallback, wParam, lParam);
}

void CPatcher::PatcherFunction(void *pthis)
{
    CPatcher *_this = static_cast<CPatcher *> (pthis);
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

        if(_this->m_bEndThread)
            break;

        assert(!_this->m_bDone);

        _this->m_bIsFinish = _this->ExecUpdate(_this->m_fn,
            _this->m_ver,
            _this->m_target_ver,
            _this->m_patch_path,
            _this->m_dst_path,
            *_this->m_updated_self,
            _this->m_offset);

        _this->m_bDone = true;
    }
}

bool CPatcher::ApplyPatch(const string& fn, int ver, int target_ver, int launcher_ver, const string& patch_path, const string& dst_path, bool* updated_self, int offset)
{
    if(!m_bDone)
        return false;
    m_bDone = false;
    m_bIsFinish = false;

    m_fn = fn;
    m_ver = ver;
    m_target_ver = target_ver;
    m_launcher_ver = launcher_ver;
    m_patch_path = patch_path;
    m_dst_path = dst_path;
    m_updated_self = updated_self;
    m_offset = offset;

    SetEvent(m_hEvent);
    return true;
}

bool CPatcher::DecompressPackfile(const string& fn, FILE *f, size_t size, char *buf, size_t buf_size)
{
    /*int bzerror;
    BZFILE *bzf = BZ2_bzReadOpen(&bzerror, f, 0, 0, NULL, 0);
    if((bzf == NULL) || (bzerror != BZ_OK))
        return false;

#define READ_BLOCK_SIZE 128 * 1024

    this->PostMessage(CbDecompressStartup, 0);
    unsigned int ofs = 0;
    size_t sSize = buf_size;
    unsigned int block_size = (unsigned int)sSize / 100;
    if(block_size < READ_BLOCK_SIZE)
        block_size = READ_BLOCK_SIZE;

    while(sSize > block_size)
    {
        BZ2_bzRead(&bzerror, bzf, buf + ofs, block_size);
        ofs += block_size;
        sSize -= block_size;

        this->PostMessage(CbDecompressProgress, (int)((double)ofs / buf_size * 100));
    }
    BZ2_bzRead(&bzerror, bzf, buf + ofs, (int)sSize);
    if(bzerror != BZ_STREAM_END)
    {
        BZ2_bzReadClose(&bzerror, bzf);
        return false;
    }
    this->PostMessage(CbDecompressDone, 0);

    BZ2_bzReadClose(&bzerror, bzf);
    if(bzerror != BZ_OK)
        return false;*/

    return true;
}

bool CPatcher::ExecUpdate(const string& fn, int ver, int target_ver, const string& patch_path, const string& dst_path, bool& updated_self, int offset)
{
	FILE *f;
	string full_filename(patch_path);

	full_filename += "/";
	full_filename += fn;

	f = fopen(full_filename.c_str(), "rb");
	if(f == NULL)
	{
		char str[100];
		sprintf(str, "�򿪸��������ļ�[%s]ʧ��!\n��ȷ�ϸ���ʱ��Ϸ�������С�",fn.c_str());
		MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
		return false;
	}
    fseek(f, offset, SEEK_SET);

    unsigned int size = file_size(full_filename.c_str()) - offset;
	UpdateFileHead ufh;
	if(sizeof(ufh) != sizeof(ufh)*fread(&ufh, sizeof(ufh), 1, f))
	{
		char str[100];
		sprintf(str, "��ȡ�ļ�ͷʧ��!",fn.c_str());
		MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	if(ufh.flag != 'DTBZ')
	{
		char str[100];
		sprintf(str, "%s���ǺϷ��ĸ����ļ�!",fn.c_str());
		MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	if(ufh.ver != DWORD(ver))
	{
		char str[100];
		sprintf(str, "%s�汾�Ŵ���!",fn.c_str());
		MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
		return false;
	}

	size -= sizeof(ufh);

    unsigned int decmp_len = ufh.decompress_len + 600;
	char* buf = buf = new char[decmp_len];
	memset(buf, 0, decmp_len);
    if(!DecompressPackfile(fn, f, size, buf, ufh.decompress_len))
    {
        delete [] buf;
	    char str[100];
	    sprintf(str, "%s��ѹ��������������!",fn.c_str());
	    MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
        return false;
    }
	fclose(f);

	bool error = false;
	UpdateSec *pSec = (UpdateSec *)buf;

    this->PostMessage(CbUpdateStartup, 0);
    // ����д����̵ĸ��£��汾���ü����³��������ȵȣ�
    list<UpdateSec *> lstDelayUpdates;
	while(pSec->flag != 0)
	{
        if(!_stricmp(pSec->pathname, "update.xml") ||
           !_stricmp(pSec->pathname, "GameLaunch.exe"))
		{
            if(strstr(pSec->pathname, "GameLaunch.exe"))
                updated_self = true;

            lstDelayUpdates.push_back(pSec);
			char *dfname = pSec->pathname;
			DWORD *pLen = (DWORD*)(dfname + pSec->pathname_len);
			BYTE *pData = (BYTE *)(pLen + 1);
			pSec = (UpdateSec *)( pData + *pLen);

			continue;
		}
        this->PostMessage(CbUpdateProgress, (int)((double)((char *)pSec - buf) / decmp_len * 100));

		error = !UpdateFile(pSec, dst_path, fn, buf, decmp_len);
		if (error)
			break;
	}
    this->PostMessage(CbUpdateDone, 0);

    if(!error)
    {
        for(list<UpdateSec *>::iterator itr = lstDelayUpdates.begin(); itr != lstDelayUpdates.end(); ++itr)
        {
            pSec = *itr;
            // �����ǰ�ǽ�����������ǰ��½���İ汾��Ҫ���µĵİ汾�ߣ��򲻸����Զ����³���
            if(strstr(pSec->pathname, "GameLaunch.exe") && ((ver > target_ver) || (m_launcher_ver >= ver)))
            {
                updated_self = false;
                continue;
            }
            UpdateFile(pSec, dst_path, fn, buf, decmp_len);
        }
    }
    delete [] (buf);

    return !error;
}	

bool CPatcher::ExecPatch(FILE *f, BYTE *data, DWORD len, const char *buf, unsigned int decmp_len)
{
	int count = 0;
	for(DWORD i=0;i<len;count++)
	{
		DWORD dw = *(DWORD *)(data+i);
		i += sizeof(DWORD);

		fseek(f, dw, SEEK_CUR);

		dw = *(DWORD *)(data+i);
		i += sizeof(DWORD);
        int wsize = fwrite(data+i, 1, dw, f);
		if(wsize != dw)
        {
            this->PostMessage(CbUpdateDiskFull, 0);
            return false;
        }
		i += dw;

        this->PostMessage(CbUpdateProgress, (int)((double)((char *)data + i - buf) / decmp_len * 100));
	}
	return true;
}


bool CPatcher::UpdateFile(UpdateSec* &pSec, const string& dst_path,const string& fn, const char *buf, unsigned int decmp_len)
{	
	string		strFileName;
    if(pSec->flag != 'MODI' && pSec->flag != 'ADDF' && pSec->flag != 'DELF')
	{
		char str[100];
		sprintf(str, "���в��Ϸ������ݽ�!",strFileName.c_str());
		MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
		return false;
	}
	char *dfname = pSec->pathname/*((char *)pSec) + sizeof(DWORD) * 2*/;
	strFileName = dst_path;
    strFileName += dfname;
	FILE *df = NULL;

	DWORD *pLen = (DWORD*)(dfname + pSec->pathname_len);
	BYTE *pData = (BYTE *)(pLen + 1);

    // �޸��ļ�����
	SetFileAttributes(strFileName.c_str(),FILE_ATTRIBUTE_NORMAL);
	if ( pSec->flag == 'MODI' ) 
	{
		df = fopen(strFileName.c_str(), "r+b");
		if(df == NULL)
		{
			char str[100];
			sprintf(str, "��Ҫ���µ��ļ�[%s]ʧ��!\n��ȷ�ϸ���ʱ��Ϸ�������С�",strFileName.c_str());
			MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);

            pSec = (UpdateSec *)( pData + *pLen);
			return true; // ���´򲻿��ļ���Ҳ���سɹ����ֶ��޸İ汾����ȥ���»�����Ҳ�����Ҫ���µ��ļ������⣩
		}
	} 
    else if ( pSec->flag == 'DELF' )
    {
        SetFileAttributes(pSec->pathname, FILE_ATTRIBUTE_NORMAL);
        DeleteFile(pSec->pathname);
    }
    else
	{
		//�滻�ļ����е��ַ�'\'Ϊ'/'
		replace(strFileName.begin(), strFileName.end(), '\\', '/');
		//ע���Ƿ���Ҫ�½�Ŀ¼
		//·��������ʽ���� C:\xy\dll/ui/bg/chatroom.gif
		//����/ui��/bg�ǿ�����Ҫ�½���Ŀ¼
		int offset = (int)strFileName.find_first_of('/');
		while(offset != -1)
		{
			offset = (int)strFileName.find_first_of('/',offset+1);
			if(offset != -1)
			{
				string new_dir = strFileName.substr(0, offset);
				string_replace(new_dir, "/", "\\");
				_mkdir(new_dir.c_str());
			}
		}

		df = fopen(strFileName.c_str(), "wb");
		if(df == NULL)
		{
			char str[100];
			sprintf(str, "�����ļ�[%s]ʧ��!\n��ȷ�ϸ���ʱ��Ϸ�������С�",strFileName.c_str());
			MessageBox(m_hOwner, str, 0, MB_OK|MB_ICONSTOP);
			return false;
		}
	}

	if(pSec->flag == 'MODI')
	{
		if( ExecPatch(df, pData, *pLen, buf, decmp_len) == false)
		{
			fclose(df);
			return false;
		}
		// ��fopen�����д�����ݵ�ƫ�Ƶ�ַ����pSec->file_len����chsize�ı��ļ���С��
		// fclose�ļ����ļ���С�ֻ���ԭ��д�����ݵ�ƫ�ƴ�С
		fflush(df);
		_chsize(_fileno(df), pSec->file_len);
	}
    else if (pSec->flag == 'ADDF')
	{
        int wsize = fwrite(pData, 1, *pLen, df);
		if(wsize != *pLen)
        {
            this->PostMessage(CbUpdateDiskFull, 0);
            return false;
        }
	}
    if(df)
	    fclose(df);	
	pSec = (UpdateSec *)( pData + *pLen);
	return true;
}

