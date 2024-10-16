
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

		// 根据传入的参数判断所要创建的字体是否与本字体是等价的
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType) = 0;

		virtual void GetFontSize(int &nWidth, int &nHeight) = 0;

		virtual void GetCharSize(float &fWidth, float &fHeight) = 0;

		virtual void SetCharSize(int nWidth, int nHeight) = 0;

		virtual void GetLineInterval(float &fInterval) = 0;

		virtual void SetLineInterval(float fInterval) = 0;

		//******************************************************************************
		/*! \fn     void GetCharExtent(BYTE nHighByte, BYTE nLowByte, float &fWidth, float &fHeight) = 0;
		*   \brief  取得一个字符的显示尺寸
		*
		*   \param  BYTE nHighByte 字符高位
		*   \param  BYTE nLowByte 字符低位
		*   \param  float &fWidth 返回的宽度
		*   \param  float &fHeight 返回的高度
		*******************************************************************************/
		virtual void GetCharExtent(const unsigned char *pChar, float &fWidth, float &fHeight) = 0;

		virtual void GetCharSize(const unsigned char *pChar, float &fWidth, float &fHeight) = 0;

		//******************************************************************************
		/*! \fn     void GetTextExtent(const char *pText, float &fWidth, float &fHeight) = 0;
		*   \brief  取得字符串的显示尺寸,字符串居中显示时可用此函数返回尺寸计算显示位置。
		*			此函数可处理换行控制符。
		*
		*   \param  const char *pText 字符串指针
		*   \param  float &fWidth 返回的宽度
		*   \param  float &fHeight 返回的高度
		*******************************************************************************/
		virtual void GetTextExtent(const char *pText, float &fWidth, 
			float &fHeight, bool bDoTransfer = false) = 0;

		// 取得从字符串头部到限定宽度范围内所完整包含的部分字符串信息
		// nCount: 从字符串头部到限定宽度范围内完整包含的部分字符串字节长度
		virtual void GetTextExtentFitInWidth(const char *pText, float fFixedWidth, 
			float &fWidth, int& nCount, bool bDoTransfer = false) = 0;

		//******************************************************************************
		/*! \fn     virtual void TextOut(const char *pText, float fX, float fY,const ColorQuad &color);
		*   \brief  显示字符串
		*
		*   \param  const char *pText 字符串指针
		*   \param  float fX 显示位置的X坐标
		*   \param  float fY 显示位置的Y坐标
		*   \param  const ColorQuad &color 字符串颜色
		*******************************************************************************/
		virtual void TextOutM(const char *pText, float fX, float fY, const ColorQuad &color, 
			uint dwStyle = FONTSYTLE_NORMAL, bool bDoTransfer = false, 
			float fScale = 1.0f, float fZ = 0.0f, const ColorQuad& shadowColor = FONT_SHADOW_COLOR ) = 0;

		//******************************************************************************
		/*! \fn     virtual void TextOutRect(const char *pText, RFRect &rect, float fOffsetX, float fOffsetY, bool bAutoWrap, const ColorQuad &color);
		*   \brief  在矩形区域里显示文本，如果自动换行，水平方向滚动偏移无效
		*
		*   \param  const char *pText 字符串指针
		*   \param  RFRect &rect 显示文本的矩形区域
		*   \param  float fOffsetX 水平方向滚动偏移
		*   \param  float fOffsetY 垂直方向滚动偏移
		*   \param  bool bAutoWrap 是否自动换行
		*   \param  const ColorQuad &color 字符串颜色
		*******************************************************************************/
		virtual void TextOutRect(const char *pText, RectFloat &rect, 
			float fOffsetX,	float fOffsetY, bool bAutoWrap, 
			const ColorQuad &color, uint dwStyle = FONTSYTLE_NORMAL, 
			bool bDoTransfer = false, float fScale = 1.0f, const ColorQuad& shadowColor = FONT_SHADOW_COLOR ) = 0;
	};
}

#endif