// #pragma once
// #include "OgreUIRenderTypes.h"
// #include "OgreTexture.h"
// #include <hash_map>
// #define CACHE_TEXTURESIZE		1024			//������ͼ�ĳߴ�
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
// 		int m_nIndexToCache;				// ���浥Ԫ
// 		uint64 m_key;						// �ַ���Ӧ��ΨһID
// 		CacheUseLinkNode *m_pPrev;			// ǰһ�ڵ�
// 		CacheUseLinkNode *m_pNext;			// ��һ�ڵ�
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
// 	HUIRES m_hUiRes;							//������ͼ���
// 
// 	void Init();
// 
// private:
// 	Texture* m_pCacheTex;
// 	int m_nPitch;								//������ͼ�����п�
// 	int m_nIconCountLine;							//�����ַ�һ�и���
// 	IconCacheMap CharCacheState;				//��¼�ַ��Ļ���״̬
// 	CacheUseLinkNode *m_cacheUseArrayList;		//������ʷ��¼��������
// 	CacheUseLinkNode *m_pHead , *m_pTail;		//������ʷ��¼���������ͷ��β
// 
// 	int m_nTextureSize;
// 	int m_nIconSize;							// ͼ���С
// 	int m_nMaxCacheSize;						// ��󻺴���
// public:
// 	UIRenderer* pUiRenderer;
// };
// }
// 
