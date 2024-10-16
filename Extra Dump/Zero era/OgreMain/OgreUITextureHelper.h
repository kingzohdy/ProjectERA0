// #pragma once
// #include "OgreUIRenderTypes.h"
// #include "OgreTexture.h"
// #include <hash_map>
// #define CACHE_TEXTURESIZE		1024			//缓存贴图的尺寸
// #define CACHE_ICONSIZE	64
// 
// namespace Ogre
// {
// 
// 	
// 
// typedef stdext::hash_map< uint64, int > IconCacheMap;
// class _OgreExport UITextureHelper
// {
// 	struct CacheUseLinkNode
// 	{
// 		int m_nIndexToCache;				// 缓存单元
// 		uint64 m_key;						// 字符对应的唯一ID
// 		CacheUseLinkNode *m_pPrev;			// 前一节点
// 		CacheUseLinkNode *m_pNext;			// 后一节点
// 	};
// 
// public:
// 
// 	UITextureHelper();
// 	~UITextureHelper();
// 	bool CreateTexture();
// 	bool FindCacheTexture(const char* fileName, HUIRES& hUiRes, RectFloat &rectUV);
// 	void UpdateTexture(const char* szFileName,HUIRES hUiRes);
// 	void UseCacheCell(CacheUseLinkNode *p);
// 	void JustBeforeRender();
// 	void setRender(UIRenderer* pRender)
// 	{
// 		pUiRenderer = pRender;
// 	}
// 	HUIRES m_hUiRes;							//缓存贴图句柄
// 
// 	void Init();
// 
// private:
// 	Texture* m_pCacheTex;
// 	int m_nPitch;								//缓存贴图数据行宽
// 	int m_nIconCountLine;							//缓存字符一行个数
// 	IconCacheMap CharCacheState;				//记录字符的缓存状态
// 	CacheUseLinkNode *m_cacheUseArrayList;		//缓存历史记录数组链表
// 	CacheUseLinkNode *m_pHead , *m_pTail;		//缓存历史记录数组链表的头和尾
// 
// 	int m_nTextureSize;
// 	int m_nIconSize;							// 图标大小
// 	int m_nMaxCacheSize;						// 最大缓存数
// public:
// 	UIRenderer* pUiRenderer;
// };
// }
// 
