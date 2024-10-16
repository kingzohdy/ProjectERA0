#pragma once
#include <string>

#define CURL_STATICLIB
#include "curl/curl.h"
	class Downloader
	{
	public:
		Downloader();
		~Downloader();

		void AbortDownload();
		void StopDownload();
		void ContinueDownload();
		bool BlockDownload(const char *filename, const char *url, int start, int size);
		bool BlockDownload(const char *filename, const char *url, int start);
		bool BlockDownload(const char *filename, const char *url);
		double GetDownloadSpeed();
		void Close();

		int GetCurrentSize();
		int GetTotalSize();
		void FlushData();

	private:
		int OnWrite(void *buffer, size_t size);
		int OnProgress(double dltotal, double dlnow, double ultotal, double ulnow);
		bool _BlockDownload(const char *url, int start, int size = 0);

	private:
		static int WriteFunction(void *buffer, size_t size, size_t nmemb, void *clientp);
		static int ProgressFunction(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

	private:
		CURL *mCurl;
		char mCulrErrDesc[CURL_ERROR_SIZE];
		char *mRange;
		FILE *mFile;
		std::string mFileName;
		int mNeedDownloadSize;
		int mCurDownloadSize;
		LPCRITICAL_SECTION	mCriticalSection;
		bool mAbort;
	};