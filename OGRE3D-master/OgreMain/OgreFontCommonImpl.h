
#ifndef __OgreFontCommonImpl_H__
#define __OgreFontCommonImpl_H__

#include "OgreFontBase.h"
#include <hash_map>

namespace Ogre
{
	//����ʹ����ʷ��¼������ڵ�
	struct CacheUseLinkNode
	{
		int m_nIndexToCache;					// ���浥Ԫ
		uint m_nCharUniqueID;					// �ַ���Ӧ��ΨһID
		CacheUseLinkNode *m_pPrev;			// ǰһ�ڵ�
		CacheUseLinkNode *m_pNext;			// ��һ�ڵ�
	};

	typedef stdext::hash_map< uint, int > CacheMap;

	class RFontCommonImpl : public RFontBase
	{
	protected:
		Texture* m_pFontTex;
		HUIRES m_hUiRes;							//������ͼ���

		uchar *m_pTexData;							//������ͼ����ָ��
		int m_nPitch;								//������ͼ�����п�

		int m_nCharCountW;							//�����ַ�һ�и���
		int m_nCharCountH;							//�����ַ�������

		CacheMap CharCacheState;					//��¼�ַ��Ļ���״̬
		CacheUseLinkNode *m_cacheUseArrayList;		//������ʷ��¼��������
		CacheUseLinkNode *m_pHead , *m_pTail;		//������ʷ��¼���������ͷ��β

		bool CreateTexture();

		void JustBeforeRender();
		virtual void TextureMap(const unsigned char* szChar, HUIRES& hUiRes, RectFloat &rectUV);

		//******************************************************************************
		/*! \fn     void UpdateTexture(BYTE nHighByte, BYTE nLowByte, int nCell);
		*   \brief  ��nCell��Ԫ����ͼ����Ϊָ���ַ�
		*
		*   \param  BYTE nHighByte �ַ����ֽ�
		*   \param  BYTE nLowByte �ַ����ֽ�
		*   \param  int nCell Ҫ���µĵ�Ԫ
		*******************************************************************************/
		void UpdateTexture(const unsigned char* szChar, int nCell);

		//******************************************************************************
		/*! \fn     void UseCacheCell(CacheUseLinkNode *p);
		*   \brief  ʹ��ָ���Ļ��浥Ԫ���Ὣ������ʷ��¼�е������Ԫ�ᵽ����β
		*
		*   \param  CacheUseLinkNode *p ������ʷ��¼����ڵ�ָ��
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

		// ���ݴ���Ĳ����ж���Ҫ�����������Ƿ��뱾�����ǵȼ۵�
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType);
	};
}

#endif