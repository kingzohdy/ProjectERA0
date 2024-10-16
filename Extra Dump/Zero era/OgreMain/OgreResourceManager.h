
#ifndef __ResourceManager_H__
#define __ResourceManager_H__

#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreFixedString.h"
#include "OgreHashTable.h"
#include "OgreLocker.h"
#include "OgreResource.h"
#include "OgreLoadWrap.h"
#include <set>
#include <map>
#include <list>
#include <deque>
#include "OgrePackageFile.h"
#include <set>

#define DOWNLOADER_COUNT	1
#define DOWNLOADER_SPEED_RATE	0

namespace Ogre
{
	class Downloader;

	void _OgreExport ValidateFileName(char *buffer, size_t maxbuf, const char *psrc);

	class _OgreExport ResourceManager : public Singleton<ResourceManager>
	{
	public:
		typedef void *OSEventHandle;
		typedef void *OSThreadHandle;

		static int askResourceTimes;

		/// Enum identifying the loading state of the resource
		enum LoadingState
		{
			LOADSTATE_UNLOADED = 0, /// Not loaded
			LOADSTATE_LOADQUE, //in loadque
			LOADSTATE_LOADED, /// Fully loaded
		};

		struct LoadRecord
		{
			// 用于调试和逆向在HashTable检索
			FixedString filename;
			Resource *pres;

			LoadingState state;
			// 资源在引用计数变为1后，最后一次被缓存确认的时间，
			// 超过此时间一定长度后资源将可能被系统回收，
			// 如果要避免这种情况可用通过定期向系统提出对此资源的预加载请求来更新这个值
			uint lasttouched;	
			bool refcount;
		};

		struct PkgFileMd5
		{
			char md5[16];
		};

		class PairSecondGreaterThan
		{
		public:
			template <class First, class Second>
			bool operator () (const std::pair<First, Second> &lhs, const std::pair<First, Second> &rhs)
			{
				return lhs.second > rhs.second;
			}
		};

		typedef HashTable<FixedString, LoadRecord, FixedStringHashCoder> ResHashTable;
		typedef ResHashTable::Element * ResHashTableIter;

	private:
		class DataStreamDownload : public DataStreamHandle
		{
		public:
			DataStreamDownload(const uint64 &k);
			virtual bool getValue(DataStream *&ps);

		private:
			uint64 key;
			int filenumber;
			FilePackage *pkg;
		};

		friend class DataStreamDownload;

	public:
		ResourceManager();
		~ResourceManager();

		void update(uint dtick);

		void loadedCallback();

		// begin PatchPkg supprot

		// filenames
		// ...
		void SetCalculatePatchPkg (bool cal);
		bool IsCalculatePatchPkg ();
		void SetCurPatchFileMapId(int mapId, const std::vector<int> &nearMaps);
		bool isSmartResOK(const char *filename); // 微端中资源是否准备好可以使用block加载
		void downloadSmartRes(const char *filename, int priority);	// 请求下载资源
		void MakeAttach();
		void MergeAttach();
		void MakeMD5();
		void AddDownloadSpeed(size_t nSize, unsigned int time);
		void UpdatePrintDownloadSpeed();
		void PrintDownloadSpeed();
		// end PatchPkg support

		bool addFilePackage(const String &pkgfile);
		void setFileRootPath(const String &rootpath)
		{
			m_RootPath = rootpath;
		}

		int getFileAttrib(const char *path); //0-not exist,  1-raw type,   2-in pkg
		DataStream *openFileStream(const String &path, bool read);
		DataStreamHandle * openFileStreamHandle(const String &filename, int priority, bool read);
		//打开本地目录下的文件
		DataStream *openLocalFileStream(const String &path, bool read);

		Resource *blockLoad(const FixedString &filename);
		void setKeepAliveHandle(void handle(void *), void *userdata = NULL);

		// 微端支持，后台加载优先级
		enum DownloadPriority
		{
			DP_DEFAULT,
			DP_DETAIL,
			DP_SCENE_SMALL,
			DP_SCENE_BIG,
			DP_SKILL,
			DP_MONSTER_NPC,
			DP_TEXTURE,
			DP_BLOCK,

			DP_MAX_PROPERTY,
		};

		enum DownloadState
		{
			SC_UNLOAD,
			SC_LOADING,
			SC_LOADFAIL,
			SC_LOADSUCC,
			SC_EXIST,
		};

		ResourceHandle	backgroundLoad(const FixedString &filename, int prop, LoadWrap *wrap);
		void	backgroundLoadBreak(ResourceHandle h);
		bool hasResource(const FixedString &filename);
		LoadingState getResourceState(ResourceHandle h);

		//when resource's refcount==1,  it'll be collected
		void garbageCollect();
		void garbageCollectIncrement(); //increment每次逐步回收

		uint _garbageCollect();

		bool tryToLoadRecord(DataStreamHandle *handle, LoadRecord *record);
		void swapLoadingCallBack(LoadRecord *record);
		uint _runLoadThread();

		// begin smart client support
		uint _runLoadThreadSmart(size_t idx);
		uint _runSwapDownloadCacheThread();
		// end smart client support

		Resource *readResourceFile(const FixedString &path, DataStream *pstream);
		bool writeResourceFile(const FixedString &path, Resource *pres);

		bool isHavePackages(){return !m_Packages.empty();}

		ResourceHandle FindResourceHandle(const FixedString& filename);

		void GenPngData( const char* pszFileName,unsigned int width, unsigned int height, unsigned int bpp, char* pData );

		void SetLoadMap( bool b );

	private:
		LoadRecord &atomicInsertRecord(const FixedString &filename);
		bool testLoadRecord(LoadRecord &rec);
		void atomicLoadRecord(LoadRecord &rec);
		void atomicLoadRecord(LoadRecord &rec, DataStream *pstream);
		void startThread();
		void startGarbageCollect();
		void startSwapDownloadCacheThread();
		bool testDownloadFile(int filenumber);
		void swapDownloadCache();
		void flushDownloadCache(size_t idx);
		void flushDownloadInfo();
		DataStream *findInDownloadCache(const uint64 &key);

		bool NeedContinueDownload();

	private:
		std::vector<FilePackage *>m_Packages;
		String m_RootPath;

		struct PkgFile
		{
			FilePackage *pkg;
			int filenumber;

//-- smart client support
			bool smartCanUse; // 微端用来判断本地文件是否可用
//-- end smart client support
		};
//-- smart client support
		struct MapFile
		{
			int mapid;
			unsigned int key1;
			unsigned int key2;
		};
//-- end smart client support
		std::map<uint64, PkgFile> m_PkgFiles;

		ResHashTable m_ResTable;
		LockSection m_ResTableMutex;

		//std::deque<LoadRecord *>m_LoadingQue;
		std::list<std::pair<LoadRecord *, int>> m_LoadingQue; // change for smart client
		LockSection m_LoadingQueMutex;

		typedef std::list<std::pair<LoadWrap *, ResourceHandle>>	WrapList;
		std::map<LoadRecord *, WrapList>	m_LoadedCallBack;
		LockSection m_LoadedCallBackMutex;

		std::map<LoadRecord *, WrapList>	m_LoadingCallBack;
		LockSection m_LoadingCallBackMutex;

		std::map<LoadRecord *, WrapList>	m_lstCallback;

		std::vector<LoadRecord *>	m_loadHandles;
		std::vector<size_t>			m_freeLoadHandles;
		LockSection m_loadHandlesMutex;

		OSEventHandle m_LoadQueSemaphore;
		LockSection m_RecordMutex;
		OSThreadHandle m_LoadThread;

		OSEventHandle m_garbageCollectSemaphore;
		OSThreadHandle m_garbageCollectThread;

		volatile uint m_GCTimeCounter;
		uint m_GCLastFrameTime;

		bool m_bQuitLoading;

		// support for packetpatch
		bool m_bCalculatePatchPkg;
		int m_CurMapId;
		std::map<int, std::set<std::string>>	m_MapFileRecords;
		std::map<int, std::list<std::pair<unsigned int, unsigned int>>>	m_MapFiles;
		// end support for packetpatch

//-- smart client support
		std::string m_PackageUrl;
		std::string m_CommonPackageUrl;

		std::vector<PkgFileMd5> mSmartClientFileMd5;
		std::vector<char> mSmartClientFileInfo;
		Downloader *downloaders[DOWNLOADER_COUNT];
		LockSection m_SmartClientLoadQueueMutex;
		LockSection m_SmartClientLoadingMutex;
		LockSection m_SmartClientIdeQueueMutex;

		std::vector<std::pair<uint64, int>>	m_SmartLoadQueue;
		std::list<std::pair<unsigned int, unsigned int>>	m_IdeSmartLoadQueue;
		OSThreadHandle m_LoadThreadSmart[DOWNLOADER_COUNT];
		bool m_LoadThreadSmartExit[DOWNLOADER_COUNT];
		OSEventHandle m_LoadSemaphoreSmart;

		LockSection m_LoadSpeedMutex;
		size_t m_LoadSpeedSize;
		unsigned int m_LoadSpeedTime;
		size_t m_lastLoadSpeedSize;
		unsigned int m_lastLoadSpeedTime;

		typedef std::map<uint64, std::pair<size_t, void *>>	DownloadCache;
		DownloadCache		m_downloadcache[2];
		size_t				m_downloadCacheSize[2];
		size_t				m_downloadcacheIdx;
		LockSection			m_downloadcacheMutex;
		LockSection			m_flushDownloadcacheMutex;
		OSThreadHandle		m_swapDownloadCacheThread;
		void				(*m_keepAliveHandle)(void *);
		void				*m_keepAliveuserdata;
		bool				m_bLoadMap;
//-- end smart client support
	};
}

#endif