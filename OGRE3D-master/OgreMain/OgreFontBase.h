
#ifndef __OgreFontBase_H__
#define __OgreFontBase_H__

#include "OgreFont.h"
#include "OgreCharacterCoding.h"

namespace Ogre
{
#define RFONT_MAXPOLYNUM		2048		//一次渲染的最大多边形个数
#define RFONT_TEXTURESIZE		1024		//缓存贴图的尺寸

	class IFontGlyphMap;

	enum RCharSet
	{
		RCHARSET_ENGLISH = 0,					//英文
		RCHARSET_CHINESE_GB,					//中文国标编码
		RCHARSET_CHINESE_GBK,					//中文国标编码
		RCHARSET_CHINESE_BIG5,					//中文BIG5编码
	};

	struct FontHeader
	{
		char			m_cIdentify[6];
		RCharSet		m_eCharSet;
		int				m_nFontWidth;
		int				m_nFontHeight;
		unsigned int	m_nLength;
	};

	//******************************************************************************
	/*! \class    类名 需要包含的头文件（可以不要） 
	*   \brief    字体类的基类
	*
	*   \author   你的名字
	*   \version  版本号
	*   \sa       列举一些参考内容
	*******************************************************************************/
	class RFontBase : public IFont
	{
	protected:
		static RectFloat ms_curParseTextNoClipScreenRect;
		static RectFloat ms_curParseTextOffsetScreenRect;
		static uint m_nInstanceCount;				//RFont实例计数，用于给贴图起个为一的名字

		std::string m_strFontFileName;
		ECharacterCoding m_eCharacterCoding;
		unsigned int m_nExtType;

		UIRenderer* m_pUiRenderer;
		int			m_nFontWidth;				// 字体宽度
		int			m_nFontHeight;				// 字体高度
		float		m_fCharWidht;				// 字符显示宽度
		float		m_fCharHeight;				// 字符显示高度
		float		m_fLineInterval;			// 行间距
		int			m_nMaxCacheSize;			// 最大缓存数

		int			m_nTexWidth;				//缓存贴图宽度
		int			m_nTexHeight;				//缓存贴图高度度

		ICharacterCoding*	m_pCharacterCoding;		// 编码方式处理对象
		IFontGlyphMap*		m_pGlyphMap;			// 字形映射对象

		int					m_nShowPolyNum;			//当前准备输出的多边形数
		int					m_nShowCharNum;			// 当前要输出的文字数目

		HUIRES					m_hUiRes;			// 当前需要输出的文字使用的贴图
		std::vector<DrawRect>	m_vecDrawRect;		// 保存这一帧要渲染的所有多边形

		//******************************************************************************
		/*! \fn     int GetMaxCacheSize(){return m_nMaxCacheSize;}
		*   \brief  返回最大缓存字符个数，输出的字符超过这个数可能无法缓存，所以必须渲染
		*
		*   \return int 最大缓存字符个数
		*******************************************************************************/
		int GetMaxCacheSize(){return m_nMaxCacheSize;}

		//从字符串开始到最近的可换行位置的距离
		float MinDisToNewLine(const char *pText, bool bDoTransfer = false);

		//******************************************************************************
		/*! \fn     virtual void JustBeforeRender() = 0;
		*   \brief  不得已而加之，给提交纹理数据提供一个机会，每个字都提交效率会很低
		*
		*******************************************************************************/
		virtual void JustBeforeRender() = 0;

		//******************************************************************************
		/*! \fn     virtual void TextureMap(BYTE nHighByte, BYTE nLowByte, HTEXTURE &m_hUiRes, RRectUV &rectUV) = 0;
		*   \brief  将字符映射到贴图
		*
		*   \param  BYTE nHighByte 字符高位
		*   \param  BYTE nLowByte 字符低位
		*   \param  HTEXTURE &m_hUiRes 返回的贴图句柄
		*   \param  RRectUV &rectUV 返回的纹理坐标
		*******************************************************************************/
		virtual void TextureMap(const unsigned char* pChar, HUIRES& hUiRes, RectFloat &rectUV) = 0;

	public:
		RFontBase()
		{
			m_nInstanceCount++;
			m_nFontWidth		= 0;
			m_nFontHeight		= 0;
			m_fCharWidht		= 0;
			m_fCharHeight		= 0;
			m_fLineInterval		= 0;
			m_nShowPolyNum		= 0;
			m_nShowCharNum		= 0;
			m_hUiRes			= NULL;
			m_pCharacterCoding	= NULL;
			m_pGlyphMap			= NULL;
			m_eCharacterCoding	= CCODING_GBK;
			m_nExtType			= 0;
			m_nMaxCacheSize		= 0;
			m_nTexWidth			= 0;
			m_nTexHeight		= 0;

			m_vecDrawRect.resize(RFONT_MAXPOLYNUM);
		}

		virtual ~RFontBase()
		{
		}

		//******************************************************************************
		/*! \fn     virtual bool Init(int nFontWidth, int nFontHeight, const char *pFontFileName) = 0;
		*   \brief  初始化
		*
		*   \param  int nFontWidth 字体宽度
		*   \param  int nFontHeight 字体高度
		*   \param  const char *pFontFileName 字体文件名
		*   \return virtual bool 初始化是否成功
		*******************************************************************************/
		virtual bool Init(UIRenderer* pUiRenderer,int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType)
		{
			if( !pUiRenderer)
				return false;
			m_pUiRenderer = pUiRenderer;

			m_strFontFileName = pFontFileName;
			m_eCharacterCoding = eCoding;
			m_nExtType = nExtType;

			return true;
		}

		// 根据传入的参数判断所要创建的字体是否与本字体是等价的
		virtual bool IsSameness(int nFontWidth, int nFontHeight, const char *pFontFileName, 
			ECharacterCoding eCoding, unsigned int nExtType) = 0;

		void GetFontSize(int &nWidth, int &nHeight)
		{
			nWidth = m_nFontWidth , nHeight = m_nFontHeight;
		}
		void GetCharSize(float &fWidth, float &fHeight)
		{
			fWidth = m_fCharWidht, fHeight = m_fCharHeight;
		}
		void SetCharSize(int nWidth, int nHeight)
		{
			m_fCharWidht = (float)nWidth , m_fCharHeight = (float)nHeight;
		}
		void GetLineInterval(float &fInterval)
		{
			fInterval = m_fLineInterval;
		}
		void SetLineInterval(float fInterval)
		{
			m_fLineInterval = fInterval;
		}

		//******************************************************************************
		/*! \fn     void RenderText(HTEXTURE hTex, int nNum);
		*   \brief  渲染已经获得纹理信息的字符多边形
		*
		*   \param  HTEXTURE hTex 贴图句柄
		*   \param  int nNum 渲染多边形个数
		*******************************************************************************/
		void RenderText(float fZ = 0.0f);

		//******************************************************************************
		/*! \fn     void GetCharExtent(BYTE nHighByte, BYTE nLowByte, float &fWidth, float &fHeight) = 0;
		*   \brief  取得一个字符的显示尺寸
		*
		*   \param  BYTE nHighByte 字符高位
		*   \param  BYTE nLowByte 字符低位
		*   \param  float &fWidth 返回的宽度
		*   \param  float &fHeight 返回的高度
		*******************************************************************************/
		void GetCharExtent(const unsigned char *pChar, float &fWidth, float &fHeight);

		void GetCharSize(const unsigned char *pChar, float &fWidth, float &fHeight);

		//******************************************************************************
		/*! \fn     void GetTextExtent(const char *pText, float &fWidth, float &fHeight) = 0;
		*   \brief  取得字符串的显示尺寸,字符串居中显示时可用此函数返回尺寸计算显示位置。
		*			此函数可处理换行控制符。
		*
		*   \param  const char *pText 字符串指针
		*   \param  float &fWidth 返回的宽度
		*   \param  float &fHeight 返回的高度
		*******************************************************************************/
		void GetTextExtent(const char *pText, float &fWidth, float &fHeight, bool bDoTransfer = false);

		// 取得从字符串头部到限定宽度范围内所完整包含的部分字符串信息
		// nCount: 从字符串头部到限定宽度范围内完整包含的部分字符串字节长度
		void GetTextExtentFitInWidth(const char *pText, float fFixedWidth, 
			float &fWidth, int& nCount, bool bDoTransfer = false);

		//******************************************************************************
		/*! \fn     virtual void TextOut(const char *pText, float fX, float fY,const ColorQuad &color);
		*   \brief  显示字符串
		*
		*   \param  const char *pText 字符串指针
		*   \param  float fX 显示位置的X坐标
		*   \param  float fY 显示位置的Y坐标
		*   \param  const ColorQuad &color 字符串颜色
		*******************************************************************************/
		void TextOutM(const char *pText, float fX, float fY, const ColorQuad &color, 
			uint dwStyle = FONTSYTLE_NORMAL, bool bDoTransfer = false, 
			float fScale = 1.0f, float fZ = 0.0f, const ColorQuad& sColor = FONT_SHADOW_COLOR );

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
		void TextOutRect(const char *pText, RectFloat &rect, float fOffsetX, float fOffsetY, 
			bool bAutoWrap, const ColorQuad &color, uint dwStyle = FONTSYTLE_NORMAL, 
			bool bDoTransfer = false, float fScale = 1.0f, const ColorQuad& scolor = FONT_SHADOW_COLOR  );

		private:
			bool IsEndParseText( EControlCode eCtrlCode );
			bool OnParseOneCharacter( const char*& pszParseText, float& fCurX, float& fCurY,
										ColorQuad& shadowColor, ColorQuad& curColor, bool& bBlink, int& nBlinkModel, float fX, 
										EControlCode eCtrlCode, float fScale, uint dwStyle, float fZ, 
										const ColorQuad &initColor );

			void OnParseNewlineCharacter( float& fCurX, float& fCurY, float fNewLineX, float fScale );

			void PrepareRenderOneCharacter( unsigned char* szCurRenderCharacter, float& fOneCharWidth, float& fOneCharHeight,
												RectFloat& rectUV, HUIRES& hUiResTp, int& nPassBytes, ColorQuad& shadowColor, 
												const ColorQuad& curColor, float fScale, const char* pszParseText );
			void DoRenderOneCharacter( const char*& pszParseText, float& fCurX, float fCurY, HUIRES hUiResTp, 
										uint dwStyle, float fOneCharWidth, float fOneCharHeight, const RectFloat& rectUV, 
										const ColorQuad& shadowColor, const ColorQuad& curColor, float fZ, int nPassBytes );
			
			void DoRenderOneShadowCharacter( float fCurX, float fCurY, float fOneCharWidth, float fOneCharHeight, 
												const RectFloat& rectUV, const ColorQuad& shadowColor );
			void DoRenderOneBorderCharacter( float fCurX, float fCurY, float fOneCharWidth, float fOneCharHeight, 
												const RectFloat& rectUV, const ColorQuad& shadowColor );
			void DoRenderOneEmBoldenCharacter( float fCurX, float fCurY, float fOneCharWidth, float fOneCharHeight, 
												const RectFloat& rectUV, const ColorQuad& shadowColor );
			void OnParseNormalCharacter( float& fCurX, float fCurY, const char*& pszParseText, 
											ColorQuad& shadowColor, float fScale, uint dwStyle, const ColorQuad& curColor, 
											float fZ );

			void OnParseTransferCharacter( const char*& pszParseText, ColorQuad& curColor, bool& bBlink, int& nBlinkModel,
											const ColorQuad& initColor );
			void ValidateMaxCharactersToRender( HUIRES hUiResTp, float fZ );

			void SetupCharPoly( DrawRect* pPolys, float fX, float fY, float fW, float fH, 
								const RectFloat &rectUV, const ColorQuad &color );

			//
			bool OnParseOneCharacterOnLimitRect(	RectFloat& limitRect, const char*& pszParseText, float& fCurX, 
													float& fCurY, ColorQuad& shadowColor, ColorQuad& curColor, bool& bBlink, 
													int& nBlinkModel, float fOffsetX, float fOffsetY, EControlCode eCtrlCode, 
													float fScale, uint dwStyle, const ColorQuad &initColor, bool bAutoWrap,
													bool bDoTransfer );
			void OnParseNormalCharacterOnLimitRect( float& fCurX, float& fCurY, RectFloat& limitRect, 
													const char*& pszParseText, ColorQuad& curColor, ColorQuad& shadowColor, 
													bool bAutoWrap, bool bDoTransfer, float fScale, 
													float fOffsetX, float fOffsetY, uint dwStyle );

			bool PrepareRenderOneCharacterOnLimitRect( float& fOneCharWidth, int& nPassBytes, float& fCurX, float& fCurY, 
														RectFloat& limitRect, const char*& pszParseText, 
														unsigned char* szCurRenderCharacter,float fScale, bool bAutoWrap, 
														bool bDoTransfer, float fOffsetX, float fOffsetY );

			void DoRenderOneCharacterOnLimitRect(	float& fCurX, const char*& pszParseText, ColorQuad& shadowColor, 
													const ColorQuad& curColor, uint dwStyle,
													unsigned char* szCurRenderCharacter, int nPassBytes, float fOneCharWidth );


			bool OnAutoWrapNewLine( float& fCurX, float& fCurY, RectFloat& limitRect, const char*& pszParseText, 
									float fScale, bool bDoTransfer );
			bool IsWidthEnoughToPutOneNewText( const RectFloat& limitRect, bool bAutoWrap );
			void InitNoClipTextScreenRect( bool bAutoWrap, float fCurX, float fOffsetX, float fOneCharWidth, 
											float fCurY, float fOffsetY, float fOneCharHeight );
			void CalculateNoClipAndOffsetScreenRect( const RectFloat& limitRect, bool bAutoWrap );
	};

	inline bool RFontBase::IsEndParseText( EControlCode eCtrlCode )
	{
		return eCtrlCode == CTRLCODE_ENDOFSTRING;
	}

	inline void RFontBase::OnParseNewlineCharacter( float& fCurX, float& fCurY, float fNewLineX, float fScale )
	{
		//从新累计X坐标
		fCurX = fNewLineX;
		fCurY += ( m_fCharHeight + m_fLineInterval ) * fScale;
	}
	
	inline void RFontBase::PrepareRenderOneCharacter( unsigned char* szCurRenderCharacter, float& fOneCharWidth, float& fOneCharHeight,
		RectFloat& rectUV, HUIRES& hUiResTp, int& nPassBytes, ColorQuad& shadowColor, 
		const ColorQuad& curColor, float fScale, const char* pszParseText )
	{
		nPassBytes = m_pCharacterCoding->GetAChar( pszParseText, szCurRenderCharacter );
		//取字符的宽、高
		GetCharExtent( szCurRenderCharacter, fOneCharWidth, fOneCharHeight );
		//取字符的贴图数据
		TextureMap( szCurRenderCharacter, hUiResTp, rectUV );

		if( NULL == m_hUiRes )	
		{
			m_hUiRes = hUiResTp;
		}

		fOneCharWidth	*= fScale;
		fOneCharHeight	*= fScale;
		shadowColor.a	= curColor.a;
	}

	inline void RFontBase::DoRenderOneShadowCharacter( float fCurX, float fCurY, float fOneCharWidth, float fOneCharHeight, 
														const RectFloat& rectUV, const ColorQuad& shadowColor )
	{
		DrawRect& firstDrawRect = m_vecDrawRect[0];
		SetupCharPoly( &firstDrawRect + m_nShowPolyNum, fCurX + 1.0f, fCurY + 1.0f, fOneCharWidth, fOneCharHeight, 
			rectUV, shadowColor);
		m_nShowPolyNum++;
	}

	inline void RFontBase::DoRenderOneBorderCharacter( float fCurX, float fCurY, float fOneCharWidth, float fOneCharHeight, 
		const RectFloat& rectUV, const ColorQuad& shadowColor )
	{
		/*DrawRect& firstDrawRect = m_vecDrawRect[0];
		SetupCharPoly( &firstDrawRect + m_nShowPolyNum, fCurX - 1.0f, fCurY - 1.0f, fOneCharWidth + 2.0f, 
						fOneCharHeight + 2.0f, rectUV, shadowColor );
		m_nShowPolyNum++;*/

		DoRenderOneShadowCharacter( fCurX, fCurY, fOneCharWidth, fOneCharHeight, rectUV, shadowColor );

		/*SetupCharPoly( &firstDrawRect + m_nShowPolyNum, fCurX - 1.0f, fCurY, fOneCharWidth, fOneCharHeight, 
			rectUV, shadowColor );
		m_nShowPolyNum++;

		SetupCharPoly( &firstDrawRect + m_nShowPolyNum, fCurX + 1.0f, fCurY, fOneCharWidth, fOneCharHeight, 
			rectUV, shadowColor );
		m_nShowPolyNum++;

		SetupCharPoly( &firstDrawRect + m_nShowPolyNum, fCurX, fCurY - 1.0f, fOneCharWidth, fOneCharHeight, 
			rectUV, shadowColor );
		m_nShowPolyNum++;

		SetupCharPoly( &firstDrawRect + m_nShowPolyNum, fCurX, fCurY + 1.0f, fOneCharWidth, fOneCharHeight, 
			rectUV, shadowColor );
		m_nShowPolyNum++;*/
	}

	inline void RFontBase::DoRenderOneEmBoldenCharacter( float fCurX, float fCurY, float fOneCharWidth, float fOneCharHeight, 
		const RectFloat& rectUV, const ColorQuad& shadowColor )
	{
		/*SetupCharPoly(&firstDrawRect + m_nShowPolyNum, fCurX - 0.2f, fCurY, fOneCharWidth, fOneCharHeight, rectUV, shadowColor);
		m_nShowPolyNum++;
		SetupCharPoly(&firstDrawRect + m_nShowPolyNum, fCurX + 0.2f, fCurY, fOneCharWidth, fOneCharHeight, rectUV, shadowColor);
		m_nShowPolyNum++;
		SetupCharPoly(&firstDrawRect + m_nShowPolyNum, fCurX, fCurY - 0.2f, fOneCharWidth, fOneCharHeight, rectUV, shadowColor);
		m_nShowPolyNum++;
		SetupCharPoly(&firstDrawRect + m_nShowPolyNum, fCurX, fCurY + 0.2f, fOneCharWidth, fOneCharHeight, rectUV, shadowColor);
		m_nShowPolyNum++;*/
	}

	inline void RFontBase::ValidateMaxCharactersToRender( HUIRES hUiResTp, float fZ )
	{
		// 如果贴图缓存满或者多边形缓冲区满或者切换了贴图，则把当前的多边形缓冲区输出
		if( m_nShowCharNum >= m_nMaxCacheSize || m_nShowPolyNum+8>=RFONT_MAXPOLYNUM || m_hUiRes != hUiResTp)
		{
			RenderText(fZ);
			m_nShowCharNum = 0;
		}
	}

	inline void RFontBase::SetupCharPoly(DrawRect* pPolys, float fX, float fY, float fW, float fH, 
		const RectFloat &rectUV, const ColorQuad &color)
	{
		RectFloat&	oneDrawScreenRect	= pPolys->rect;
		RectFloat&	oneDrawUVRect		= pPolys->rectUV;
		oneDrawScreenRect.set(fX, fY, fX+fW, fY+fH);
		oneDrawUVRect = rectUV;
		pPolys->color = color;
	}

	inline void RFontBase::OnParseNormalCharacterOnLimitRect( float& fCurX, float& fCurY, RectFloat& limitRect, 
		const char*& pszParseText, ColorQuad& curColor, ColorQuad& shadowColor, 
		bool bAutoWrap, bool bDoTransfer, float fScale, 
		float fOffsetX, float fOffsetY, uint dwStyle )
	{
		float fOneCharWidth		= 0.0f;
		int nPassBytes			= 0;
		unsigned char szCurRenderCharacter[8] = { 0 };
		bool canRenderText = PrepareRenderOneCharacterOnLimitRect( 
			fOneCharWidth, nPassBytes, fCurX, fCurY, limitRect, 
			pszParseText, szCurRenderCharacter, fScale, bAutoWrap, 
			bDoTransfer, fOffsetX, fOffsetY 
			);
		if ( !canRenderText )
		{
			return;
		}

		DoRenderOneCharacterOnLimitRect( fCurX, pszParseText, shadowColor, curColor, dwStyle, 
			szCurRenderCharacter, nPassBytes, fOneCharWidth );
	}

	inline bool RFontBase::OnAutoWrapNewLine( float& fCurX, float& fCurY, RectFloat& limitRect, const char*& pszParseText, 
		float fScale, bool bDoTransfer )
	{
		float fOneLineWidth = MinDisToNewLine( pszParseText, bDoTransfer );

		if ( fOneLineWidth > limitRect.getWidth() )
		{
			limitRect.m_Right = limitRect.m_Left + fOneLineWidth;
		}

		if ( ( fOneLineWidth + fCurX ) > limitRect.m_Right )
		{
			//自动换行，从新累计X坐标
			fCurX		= limitRect.m_Left;
			fCurY		+= ( m_fCharHeight + m_fLineInterval )  * fScale;
			int nSpace	= m_pCharacterCoding->JumpOverSpaces( pszParseText );
			if ( nSpace != 0 )
			{
				pszParseText += nSpace;
				return false;
			}
		}

		return true;
	}

	inline bool RFontBase::IsWidthEnoughToPutOneNewText( const RectFloat& limitRect, bool bAutoWrap )
	{
		if ( ms_curParseTextNoClipScreenRect.m_Top >= limitRect.m_Bottom )
		{
			return false;
		}

		if ( ms_curParseTextNoClipScreenRect.m_Bottom <= limitRect.m_Top )
		{
			return false;
		}

		if ( bAutoWrap )
		{
			return true;
		}

		if ( ms_curParseTextNoClipScreenRect.m_Left >= limitRect.m_Right )
		{
			return false;
		}

		if ( ms_curParseTextNoClipScreenRect.m_Right <= limitRect.m_Left )
		{
			return false;
		}

		return true;
	}

	inline void RFontBase::InitNoClipTextScreenRect( bool bAutoWrap, float fCurX, float fOffsetX, float fOneCharWidth, 
		float fCurY, float fOffsetY, float fOneCharHeight )
	{
		if ( bAutoWrap )
		{
			ms_curParseTextNoClipScreenRect.m_Left = fCurX;
		}
		else
		{
			ms_curParseTextNoClipScreenRect.m_Left	= fCurX + fOffsetX;
		}

		ms_curParseTextNoClipScreenRect.m_Right = ms_curParseTextNoClipScreenRect.m_Left + fOneCharWidth;

		ms_curParseTextNoClipScreenRect.m_Top		= fCurY + fOffsetY;
		ms_curParseTextNoClipScreenRect.m_Bottom	= ms_curParseTextNoClipScreenRect.m_Top + fOneCharHeight;
	}
}

#endif