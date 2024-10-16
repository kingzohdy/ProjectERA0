#ifndef __OGREDOWNLOADER_H__
#define __OGREDOWNLOADER_H__

#define CURL_STATICLIB
#include "curl/curl.h"
#include <assert.h>
#include <Windows.h>
#include <process.h>
#include <string>
#include <vector>
#include "OgreLocker.h"

namespace Ogre
{
	class Downloader
	{
	public:
		Downloader();
		~Downloader();

		enum DownType
		{
			DT_FILE,
			DT_MEMORY,
			DT_FILEMEMORY,

			DT_MAX_TYPE
		};
		void AbortDownload();
		void StopDownload();
		void ContinueDownload();
		void SetDownloadMaxSpeed(curl_off_t speed);
		bool BlockDownload(const char *filename, const char *url, int start, int size);
		bool BlockDownload(const char *filename, const char *url, int start);
		bool BlockDownload(const char *filename, const char *url);
		bool BlockDownload(const char *url, int start, int size);
		const char *GetDownloadMemory();
		int GetDownloadSize();
		double GetDownloadSpeed();
		void Close();

		int GetCurrentSize();
		int GetTotalSize();

	private:
		int OnWrite(void *buffer, size_t size);
		int OnProgress(double dltotal, double dlnow, double ultotal, double ulnow);
		bool _BlockDownload(const char *url, int start, int size = 0);

	private:
		static int WriteFunction(void *buffer, size_t size, size_t nmemb, void *clientp);
		static int ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

		static int msNumInsts;

	private:
		CURL *mCurl;
		char mCulrErrDesc[CURL_ERROR_SIZE];
		char *mRange;
		LockSection mMutex;
		FILE *lastFile;
		FILE *mFile;
		char *lastBuffer;
		char *mBuffer;
		std::string mFileName;
		int mMaxMemorySize;
		int mNeedDownloadSize;
		int mCurDownloadSize;

		unsigned int lastWriteTime;
		curl_off_t mSpeed;
		curl_off_t lastSpeed;
		bool mAbort;
	};
}

#endif
