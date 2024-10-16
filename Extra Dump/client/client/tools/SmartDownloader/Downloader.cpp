#include "Downloader.h"
// #include "OgreResourceManager.h"

#ifdef _DEBUG
	#pragma comment(lib, "libcurld.lib")
#else
	#pragma comment(lib, "libcurl.lib")
#endif
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")

static int msNumInsts = 0;

Downloader::Downloader() : 
	mFile(NULL), mNeedDownloadSize(0), mCurDownloadSize(0), mRange(NULL)
{
	mAbort = false;
	if (msNumInsts++ == 0)
	{
		curl_global_init(CURL_GLOBAL_ALL);
	}
	mCurl = curl_easy_init();

	curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, &WriteFunction);
	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(mCurl, CURLOPT_PROGRESSFUNCTION, &ProgressFunction);
	curl_easy_setopt(mCurl, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt(mCurl, CURLOPT_NOPROGRESS, FALSE);
	curl_easy_setopt(mCurl, CURLOPT_ERRORBUFFER, mCulrErrDesc);
	curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(mCurl, CURLOPT_VERBOSE, 1);

	memset(mCulrErrDesc, 0, sizeof(mCulrErrDesc));

	mCriticalSection = new CRITICAL_SECTION;
	InitializeCriticalSection( mCriticalSection );
}

Downloader::~Downloader()
{
	DeleteCriticalSection( mCriticalSection );
	delete(mCriticalSection);
	if( mFile )
	{
		fclose( mFile );
	}
	if (mRange)
	{
		free(mRange);
	}

	if (mCurl)
	{
		curl_easy_cleanup(mCurl);
	}
	if(--msNumInsts == 0)
	{
		curl_global_cleanup();
	}
}

void Downloader::AbortDownload()
{
	mAbort = true;
}

void Downloader::StopDownload()
{
	EnterCriticalSection( mCriticalSection );
	curl_easy_pause(mCurl, CURLPAUSE_RECV);
	LeaveCriticalSection( mCriticalSection );
}

void Downloader::ContinueDownload()
{
	curl_easy_pause(mCurl, CURLPAUSE_RECV_CONT);
}

bool Downloader::BlockDownload(const char *filename, const char *url, int start, int size)
{
	if (strcmp(mFileName.c_str(), filename) != 0)
	{
		if (mFile)
		{
			fclose(mFile);
			mFile = NULL;
		}
		mFileName = filename;
	}
	// 创建文件
	if (!mFile)
	{
		mFile = fopen(mFileName.c_str(), "rb+");

		if (!mFile)
		{
			mFile = fopen(mFileName.c_str(), "wb");
		}

		if (!mFile)
		{
			return false;
		}
	}
	if (mFile)
		fseek(mFile, start, SEEK_SET);

	mNeedDownloadSize = size;
	return _BlockDownload(url, start, size);
}

bool Downloader::BlockDownload(const char *filename, const char *url, int start)
{
	if (strcmp(mFileName.c_str(), filename) != 0)
	{
		if (mFile)
		{
			fclose(mFile);
			mFile = NULL;
		}
		mFileName = filename;
	}
	// 创建文件
	if (!mFile)
	{
		mFile = fopen(mFileName.c_str(), "rb+");

		if (!mFile)
		{
			mFile = fopen(mFileName.c_str(), "wb");
		}

		if (!mFile)
		{
			return false;
		}
	}
	if (mFile)
		fseek(mFile, start, SEEK_SET);

	mNeedDownloadSize = 0;
	return _BlockDownload(url, start);
}

bool Downloader::BlockDownload(const char *filename, const char *url)
{
	return BlockDownload(filename, url, 0);
}

bool Downloader::_BlockDownload(const char *url, int start, int size /*= 0*/)
{
	mCurDownloadSize = 0;
	EnterCriticalSection( mCriticalSection );
	curl_easy_setopt(mCurl, CURLOPT_URL, url);
	if (mRange)
	{
		free(mRange);
	}
	if (size > 0)
	{
		int len = _snprintf(NULL, 0, "%d-%d", start, start + size - 1) + 1;
		mRange = (char *)malloc(len);
		_snprintf(mRange, len, "%d-%d", start, start + size - 1);
	}
	else
	{
		int len = _snprintf(NULL, 0, "%d-", start) + 1;
		mRange = (char *)malloc(len);
		_snprintf(mRange, len, "%d-", start);
	}
	curl_easy_setopt(mCurl, CURLOPT_RANGE, mRange);

	CURLcode result = curl_easy_perform(mCurl);

	LeaveCriticalSection( mCriticalSection );

	if (result != CURLE_OK && result != CURLE_ABORTED_BY_CALLBACK)
	{
		// 出错
		if (mCulrErrDesc[0] != 0)
		{
		}
		return false;
	}
	if (mFile)
	{
		fclose(mFile);
		mFile = fopen(mFileName.c_str(), "rb+");
	}
	return result == CURLE_OK;
}

int Downloader::OnWrite(void *buffer, size_t size)
{
	int nSize = (int)size;
	if (mNeedDownloadSize != 0)
	{
		if (mCurDownloadSize + nSize > mNeedDownloadSize)
		{
			nSize = mNeedDownloadSize - mCurDownloadSize;
		}
	}
	if (mFile)
	{
		fwrite(buffer, nSize, 1, mFile);
	}

	mCurDownloadSize += nSize;
	return nSize;
}

int Downloader::OnProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	if (mAbort)
	{
		return -1;
	}
	return 0;
}

int Downloader::WriteFunction(void *buffer, size_t size, size_t nmemb, void *clientp)
{
	Downloader *download = (Downloader *)clientp;
	return download->OnWrite(buffer, size * nmemb);
}

int Downloader::ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	Downloader *download = (Downloader *)clientp;
	return download->OnProgress(dltotal, dlnow, ultotal, ulnow);
}

void Downloader::Close()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = NULL;
	}
	mCurDownloadSize = 0;
}

int Downloader::GetCurrentSize()
{
	return mCurDownloadSize;
}

int Downloader::GetTotalSize()
{
	return mNeedDownloadSize;
}

double Downloader::GetDownloadSpeed()
{
	double speed;
	curl_easy_getinfo(mCurl, CURLINFO_SPEED_DOWNLOAD, &speed);
	return speed;
}

void Downloader::FlushData()
{
	if( mFile )
	{
		fclose(mFile);
		mFile = fopen(mFileName.c_str(), "rb+");
	}
}
