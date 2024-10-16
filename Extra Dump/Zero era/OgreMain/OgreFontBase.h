
#ifndef __OgreFontBase_H__
#define __OgreFontBase_H__

#include "OgreFont.h"
#include "OgreCharacterCoding.h"

namespace Ogre
{
#define RFONT_MAXPOLYNUM		2048		//һ����Ⱦ��������θ���
#define RFONT_TEXTURESIZE		1024		//������ͼ�ĳߴ�

	class IFontGlyphMap;

	enum RCharSet
	{
		RCHARSET_ENGLISH = 0,					//Ӣ��
		RCHARSET_CHINESE_GB,					//���Ĺ������
		RCHARSET_CHINESE_GBK,					//���Ĺ������
		RCHARSET_CHINESE_BIG5,					//����BIG5����
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
	/*! \class    ���� ��Ҫ������ͷ�ļ������Բ�Ҫ�� 
	*   \brief    ������Ļ���
	*
	*   \author   �������
	*   \version  �汾��
	*   \sa       �о�һЩ�ο�����
	*******************************************************************************/
	class RFontBase : public IFont
	{
	protected:
		static RectFloat ms_curParseTextNoClipScreenRect;
		static RectFloat ms_curParseTextOffsetScreenRect;
		static uint m_nInstanceCount;				//RFontʵ�����������ڸ���ͼ���Ϊһ������

		std::string m_strFontFileName;
		ECharacterCoding m_eCharacterCoding;
		unsigned int m_nExtType;

		UIRenderer* m_pUiRenderer;
		int			m_nFontWidth;				// ������
		int			m_nFontHeight;				// ����߶�
		float		m_fCharWidht;				// �ַ���ʾ���
		float		m_fCharHeight;				// �ַ���ʾ�߶�
		float		m_fLineInterval;			// �м��
		int			m_nMaxCacheSize;			// ��󻺴���

		int			m_nTexWidth;				//������ͼ���
		int			m_nTexHeight;				//������ͼ�߶ȶ�

		ICharacterCoding*	m_pCharacterCoding;		// ���뷽ʽ�������
		IFontGlyphMap*		m_pGlyphMap;			// ����ӳ�����

		int					m_nShowPolyNum;			//��ǰ׼������Ķ������
		int					m_nShowCharNum;			// ��ǰҪ�����������Ŀ

		HUIRES					m_hUiRes;			// ��ǰ��Ҫ���������ʹ�õ���ͼ
		std::vector<DrawRect>	m_vecDrawRect;		// ������һ֡Ҫ��Ⱦ�����ж����

		//******************************************************************************
		/*! \fn     int GetMaxCacheSize(){return m_nMaxCacheSize;}
		*   \brief  ������󻺴��ַ�������������ַ���������������޷����棬���Ա�����Ⱦ
		*
		*   \return int ��󻺴��ַ�����
		*******************************************************************************/
		int GetMaxCacheSize(){return m_nMaxCacheSize;}

		//���ַ�����ʼ������Ŀɻ���λ�õľ���
		float MinDisToNewLine(const char *pText, bool bDoTransfer = false);

		//******************************************************************************
		/*! \fn     virtual void JustBeforeRender() = 0;
		*   \brief  �����Ѷ���֮�����ύ���������ṩһ�����ᣬÿ���ֶ��ύЧ�ʻ�ܵ�
		*
		*******************************************************************************/
		virtual void JustBeforeRender() = 0;

		//******************************************************************************
		/*! \fn     virtual void TextureMap(BYTE nHighByte, BYTE nLowByte, HTEXTURE &m_hUiRes, RRectUV &rectUV) = 0;
		*   \brief  ���ַ�ӳ�䵽��ͼ
		*
		*   \param  BYTE nHighByte �ַ���λ
		*   \param  BYTE nLowByte �ַ���λ
		*   \param  HTEXTURE &m_hUiRes ���ص���ͼ���
		*   \param  RRectUV &rectUV ���ص���������
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
		*   \brief  ��ʼ��
		*
		*   \param  int nFontWidth ������
		*   \param  int nFontHeight ����߶�
		*   \param  const char *pFontFileName �����ļ���
		*   \return virtual bool ��ʼ���Ƿ�ɹ�
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

		// ���ݴ���Ĳ����ж���Ҫ�����������Ƿ��뱾�����ǵȼ۵�
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
		*   \brief  ��Ⱦ�Ѿ����������Ϣ���ַ������
		*
		*   \param  HTEXTURE hTex ��ͼ���
		*   \param  int nNum ��Ⱦ����θ���
		*******************************************************************************/
		void RenderText(float fZ = 0.0f);

		//******************************************************************************
		/*! \fn     void GetCharExtent(BYTE nHighByte, BYTE nLowByte, float &fWidth, float &fHeight) = 0;
		*   \brief  ȡ��һ���ַ�����ʾ�ߴ�
		*
		*   \param  BYTE nHighByte �ַ���λ
		*   \param  BYTE nLowByte �ַ���λ
		*   \param  float &fWidth ���صĿ��
		*   \param  float &fHeight ���صĸ߶�
		*******************************************************************************/
		void GetCharExtent(const unsigned char *pChar, float &fWidth, float &fHeight);

		void GetCharSize(const unsigned char *pChar, float &fWidth, float &fHeight);

		//******************************************************************************
		/*! \fn     void GetTextExtent(const char *pText, float &fWidth, float &fHeight) = 0;
		*   \brief  ȡ���ַ�������ʾ�ߴ�,�ַ���������ʾʱ���ô˺������سߴ������ʾλ�á�
		*			�˺����ɴ����п��Ʒ���
		*
		*   \param  const char *pText �ַ���ָ��
		*   \param  float &fWidth ���صĿ��
		*   \param  float &fHeight ���صĸ߶�
		*******************************************************************************/
		void GetTextExtent(const char *pText, float &fWidth, float &fHeight, bool bDoTransfer = false);

		// ȡ�ô��ַ���ͷ�����޶���ȷ�Χ�������������Ĳ����ַ�����Ϣ
		// nCount: ���ַ���ͷ�����޶���ȷ�Χ�����������Ĳ����ַ����ֽڳ���
		void GetTextExtentFitInWidth(const char *pText, float fFixedWidth, 
			float &fWidth, int& nCount, bool bDoTransfer = false);

		//******************************************************************************
		/*! \fn     virtual void TextOut(const char *pText, float fX, float fY,const ColorQuad &color);
		*   \brief  ��ʾ�ַ���
		*
		*   \param  const char *pText �ַ���ָ��
		*   \param  float fX ��ʾλ�õ�X����
		*   \param  float fY ��ʾλ�õ�Y����
		*   \param  const ColorQuad &color �ַ�����ɫ
		*******************************************************************************/
		void TextOutM(const char *pText, float fX, float fY, const ColorQuad &color, 
			uint dwStyle = FONTSYTLE_NORMAL, bool bDoTransfer = false, 
			float fScale = 1.0f, float fZ = 0.0f, const ColorQuad& sColor = FONT_SHADOW_COLOR );

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
		//�����ۼ�X����
		fCurX = fNewLineX;
		fCurY += ( m_fCharHeight + m_fLineInterval ) * fScale;
	}
	
	inline void RFontBase::PrepareRenderOneCharacter( unsigned char* szCurRenderCharacter, float& fOneCharWidth, float& fOneCharHeight,
		RectFloat& rectUV, HUIRES& hUiResTp, int& nPassBytes, ColorQuad& shadowColor, 
		const ColorQuad& curColor, float fScale, const char* pszParseText )
	{
		nPassBytes = m_pCharacterCoding->GetAChar( pszParseText, szCurRenderCharacter );
		//ȡ�ַ��Ŀ���
		GetCharExtent( szCurRenderCharacter, fOneCharWidth, fOneCharHeight );
		//ȡ�ַ�����ͼ����
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
		// �����ͼ���������߶���λ������������л�����ͼ����ѵ�ǰ�Ķ���λ��������
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
			//�Զ����У������ۼ�X����
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