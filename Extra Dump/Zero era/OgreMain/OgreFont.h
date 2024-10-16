
#ifndef __OgreFont_H__
#define __OgreFont_H__

#include "OgreColourValue.h"
#include "OgreRect.h"
#include "OgreUIRenderTypes.h"


namespace Ogre
{
	class IFont
	{
	public:
		virtual ~IFont(){}

		// ���ݴ���Ĳ����ж���Ҫ�����������Ƿ��뱾�����ǵȼ۵�
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType) = 0;

		virtual void GetFontSize(int &nWidth, int &nHeight) = 0;

		virtual void GetCharSize(float &fWidth, float &fHeight) = 0;

		virtual void SetCharSize(int nWidth, int nHeight) = 0;

		virtual void GetLineInterval(float &fInterval) = 0;

		virtual void SetLineInterval(float fInterval) = 0;

		//******************************************************************************
		/*! \fn     void GetCharExtent(BYTE nHighByte, BYTE nLowByte, float &fWidth, float &fHeight) = 0;
		*   \brief  ȡ��һ���ַ�����ʾ�ߴ�
		*
		*   \param  BYTE nHighByte �ַ���λ
		*   \param  BYTE nLowByte �ַ���λ
		*   \param  float &fWidth ���صĿ��
		*   \param  float &fHeight ���صĸ߶�
		*******************************************************************************/
		virtual void GetCharExtent(const unsigned char *pChar, float &fWidth, float &fHeight) = 0;

		virtual void GetCharSize(const unsigned char *pChar, float &fWidth, float &fHeight) = 0;

		//******************************************************************************
		/*! \fn     void GetTextExtent(const char *pText, float &fWidth, float &fHeight) = 0;
		*   \brief  ȡ���ַ�������ʾ�ߴ�,�ַ���������ʾʱ���ô˺������سߴ������ʾλ�á�
		*			�˺����ɴ����п��Ʒ���
		*
		*   \param  const char *pText �ַ���ָ��
		*   \param  float &fWidth ���صĿ��
		*   \param  float &fHeight ���صĸ߶�
		*******************************************************************************/
		virtual void GetTextExtent(const char *pText, float &fWidth, 
			float &fHeight, bool bDoTransfer = false) = 0;

		// ȡ�ô��ַ���ͷ�����޶���ȷ�Χ�������������Ĳ����ַ�����Ϣ
		// nCount: ���ַ���ͷ�����޶���ȷ�Χ�����������Ĳ����ַ����ֽڳ���
		virtual void GetTextExtentFitInWidth(const char *pText, float fFixedWidth, 
			float &fWidth, int& nCount, bool bDoTransfer = false) = 0;

		//******************************************************************************
		/*! \fn     virtual void TextOut(const char *pText, float fX, float fY,const ColorQuad &color);
		*   \brief  ��ʾ�ַ���
		*
		*   \param  const char *pText �ַ���ָ��
		*   \param  float fX ��ʾλ�õ�X����
		*   \param  float fY ��ʾλ�õ�Y����
		*   \param  const ColorQuad &color �ַ�����ɫ
		*******************************************************************************/
		virtual void TextOutM(const char *pText, float fX, float fY, const ColorQuad &color, 
			uint dwStyle = FONTSYTLE_NORMAL, bool bDoTransfer = false, 
			float fScale = 1.0f, float fZ = 0.0f, const ColorQuad& shadowColor = FONT_SHADOW_COLOR ) = 0;

		//******************************************************************************
		/*! \fn     virtual void TextOutRect(const char *pText, RFRect &rect, float fOffsetX, float fOffsetY, bool bAutoWrap, const ColorQuad &color);
		*   \brief  �ھ�����������ʾ�ı�������Զ����У�ˮƽ�������ƫ����Ч
		*
		*   \param  const char *pText �ַ���ָ��
		*   \param  RFRect &rect ��ʾ�ı��ľ�������
		*   \param  float fOffsetX ˮƽ�������ƫ��
		*   \param  float fOffsetY ��ֱ�������ƫ��
		*   \param  bool bAutoWrap �Ƿ��Զ�����
		*   \param  const ColorQuad &color �ַ�����ɫ
		*******************************************************************************/
		virtual void TextOutRect(const char *pText, RectFloat &rect, 
			float fOffsetX,	float fOffsetY, bool bAutoWrap, 
			const ColorQuad &color, uint dwStyle = FONTSYTLE_NORMAL, 
			bool bDoTransfer = false, float fScale = 1.0f, const ColorQuad& shadowColor = FONT_SHADOW_COLOR ) = 0;
	};
}

#endif