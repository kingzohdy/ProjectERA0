
#ifndef __OgreFontCommonImpl_H__
#define __OgreFontCommonImpl_H__

#include "OgreFontBase.h"
#include <hash_map>

namespace Ogre
{
	//缓存使用历史记录的链表节点
	struct CacheUseLinkNode
	{
		int m_nIndexToCache;					// 缓存单元
		uint m_nCharUniqueID;					// 字符对应的唯一ID
		CacheUseLinkNode *m_pPrev;			// 前一节点
		CacheUseLinkNode *m_pNext;			// 后一节点
	};

	typedef stdext::hash_map< uint, int > CacheMap;

	class RFontCommonImpl : public RFontBase
	{
	protected:
		Texture* m_pFontTex;
		HUIRES m_hUiRes;							//缓存贴图句柄

		uchar *m_pTexData;							//缓存贴图数据指针
		int m_nPitch;								//缓存贴图数据行宽

		int m_nCharCountW;							//缓存字符一行个数
		int m_nCharCountH;							//缓存字符的行数

		CacheMap CharCacheState;					//记录字符的缓存状态
		CacheUseLinkNode *m_cacheUseArrayList;		//缓存历史记录数组链表
		CacheUseLinkNode *m_pHead , *m_pTail;		//缓存历史记录数组链表的头和尾

		bool CreateTexture();

		void JustBeforeRender();
		virtual void TextureMap(const unsigned char* szChar, HUIRES& hUiRes, RectFloat &rectUV);

		//******************************************************************************
		/*! \fn     void UpdateTexture(BYTE nHighByte, BYTE nLowByte, int nCell);
		*   \brief  将nCell单元的贴图更新为指定字符
		*
		*   \param  BYTE nHighByte 字符高字节
		*   \param  BYTE nLowByte 字符低字节
		*   \param  int nCell 要更新的单元
		*******************************************************************************/
		void UpdateTexture(const unsigned char* szChar, int nCell);

		//******************************************************************************
		/*! \fn     void UseCacheCell(CacheUseLinkNode *p);
		*   \brief  使用指定的缓存单元，会将缓存历史记录中的这个单元提到链表尾
		*
		*   \param  CacheUseLinkNode *p 缓存历史记录链表节点指针
		*******************************************************************************/
		void UseCacheCell(CacheUseLinkNode *p);

	public:
		RFontCommonImpl()
		{
			m_nTexWidth = m_nTexHeight = RFONT_TEXTURESIZE;
			m_pFontTex = NULL;
			m_hUiRes = NULL;
			m_pTexData = NULL;
			m_cacheUseArrayList = NULL;
		}

		~RFontCommonImpl();

		virtual bool Init(UIRenderer* pUiRenderer, int nFontWidth, int nFontHeight, 
			const char *pFontFileName, ECharacterCoding eCoding, unsigned int nExtType);

		// 根据传入的参数判断所要创建的字体是否与本字体是等价的
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType);
	};
}

#endif