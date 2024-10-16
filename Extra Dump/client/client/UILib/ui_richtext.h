
#ifndef __UI_RICHTEXT_H__
#define __UI_RICHTEXT_H__

#include "ui_common.h"
#include "ui_framemgr.h"
#include "ui_frame.h"
#include "RichTextBuilder.h"
#include "RichTextCommonType.h"

struct tagBlendTexture
{
	tagBlendTexture( )
	{
		m_hTex			= NULL;
		m_nRelWidth		= 0;
		m_nRealHeight	= 0;
	}

	std::string		m_strTexturePath;
	Ogre::HUIRES	m_hTex;
	int				m_nRelWidth;
	int				m_nRealHeight;
	int				m_nTexX;
	int				m_nTexY;
	int				m_nTexWidth;
	int				m_nTexHeight;
};

struct tagTextHistory 
{
	std::string			m_strTextHistory;	// �ı���¼
	Ogre::ColorQuad		m_lineColor;		// ������ɫ		
};

class RRichTextBuilder;
class RichTextText;
class RichTextFace;
class RichTextPicture;
class RichTextObject;
class RichTextLine;

typedef std::list<RichTextLine*> RichTextLineList;

class RichText : public Frame { //tolua_export
	DECLARE_UIOBJECT_CLONE(RichText)

public:
	RichText();
	~RichText();

	// ��Ⱦ�ӿ�
	void AddRenderText(const char *szText, const Ogre::ColorQuad& color);

	// ��Ⱦ�ӿ�
	void SetRenderText(const char *szText, const Ogre::ColorQuad& color);

	// �ı�ߴ磬Ŀǰ�������������
	void Resize();

	// ����RichText���Ͻ�λ��
	virtual void CalAbsRectSelf(size_t frame);
	void SetPosition(float fX, float fY);

	virtual void FillDrawItems();
	virtual void UpdateSelf(float deltatime);
	void DrawNormalText( const RichTextText* pText, const Ogre::RectFloat& rectLine, unsigned int dwCurTime );
	
	void DrawFace( RichTextFace* pFace, const Ogre::RectFloat& rectLine, unsigned int dwCurTime, 
					int& nFaceNum, Ogre::DrawRect* faces );

	void DrawPicture( RichTextPicture* pPicture, const Ogre::RectFloat& rectLine, unsigned int dwCurTime, 
						int& nPictureNum, Ogre::DrawRect* pictures );
	virtual void Draw();
	virtual TiXmlElement *Save( TiXmlElement *pElement );

	virtual int OnInputMessage(const Ogre::InputEvent &event);
	virtual const char *GetTypeName()
	{ 
		return "RichText"; 
	}
	
	//�����������λ�����еĿؼ�
	virtual void FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *>& vFrameTemp );	

	void RenderFaces(Ogre::DrawRect* pPoly, int nNum);
	void RenderPictures( Ogre::DrawRect* pictures, int nNum );
	
	int		OnClick(const Ogre::InputEvent &event);
	int		OnClickOneRichObject(const Ogre::InputEvent &event, const RichTextObject* pOneRichObject );
	bool	isMouseInLink( int x, int y );
	void	OnMouseMoveInLink(const Ogre::InputEvent &event); 
	void	setShadowColor();
	friend class RRichTextBuilder;

public:
	//tolua_begin
	void ReplacePartialText( const char* pszDestText, const char* pszNewText );
	void setTwoFaceInterval( float fXInterval, float fYInterval );
	tagRect_ToLua& getLinkTextRect( const char* pszText );
	int getFaceWidth();
	int getFaceHeight();
	void SetFaceTexture(const char *path);
	void SetFaceTexUV(int x, int y, int width, int height);
	void SetFaceTexRelUV( float x, float y );
	// ����͸����
	void SetAlpha(float fAlpha)
	{
		m_fAlpha = fAlpha;
		if( m_fAlpha < 0.0f ) m_fAlpha = 0.0f;
		if( m_fAlpha > 1.0f ) m_fAlpha = 1.0f;
	}

	float	GetAlpha()
	{
		return m_fAlpha;
	}
	
	void resizeRect( int Width, int Height );
	void resizeRichWidth( int Width );
	void resizeRichHeight( int Height );
	// ���������ı�
	void resizeText( int Width, int Height );

	// ����һ������
	void AddText(const char *szText, int r, int g, int b);

	// ����һ�����֣������ǰ������
	void SetText(const char *szText, int r, int g, int b);
	//
	void SetAutoExtend( bool bAutoExtend );
	
	// ���ó�����������ɫ
	void SetLinkTextColor(int r, int g, int b);

	// �������������ɫ
	void SetShadowColor(int r, int g, int b);

	// ���������Ӱ ��ͨ-FONTSYTLE_NORMAL 0 ��Ӱ-FONTSYTLE_SHADOW 1 ���-FONTSYTLE_BORDER	2
	void SetFontType( int nType);

	// ������е�����
	void Clear();
	void clearHistory();
	// ���ô�ֱ�������ʾλ��
	void SetDispPos(float fY);
	void IncrDispPos( float fY );

	// ��ȡ��ֱ�������ʾλ��
	float GetDispPos();
	
	// �����ݷ������ʾλ��
	void SetDispPosX(float fY);

	// ��ȡ���������ʾλ��
	float GetDispPosX();

	// ��ȡ��һ������ʾλ��
	float GetStartDispPos();

	// ��ȡ���һ������ʾλ��
	// �����ڷ������ǰ��ʾ�����е����һ�е���ʼλ��
	float GetEndDispPos();

	// ���Ϲ���һ��
	void ScrollUp();
	
	// ScrollUp��ScroolDown����ֻ�Ǹı�m_newLineStartPoint��ֵ,�����ı�m_newLineStartPoint��ֵ
	// ���¹���һ��
	void ScrollDown();

	// ������һ��
	void ScrollFirst();
	// ���������һ��
	void ScrollEnd();

	int GetTextLines();
	int getLineRealWidth( int nLine );
	int getLineWidth( int nLine );
	// ���ָ���п��
	int GetLineWidth(int index);
	// ����и߶�
	int GetLineHeight( int index );
	// ����ܸ߶�
	int GetTotalHeight( );
	int GetViewLines();
	int GetAccurateViewLines();
	virtual void setAbsRect(float fLeft,float fTop, float fRight, float fBottom);
	
	int GetTextExtentWidth( const char* pszContent );
	int GetTextExtentHeight( const char* pszContent );

	void setCenterLine( bool bCenter ){ m_bCenterLine = bCenter; };

	void setCursorNormal();
	//tolua_end

private:
	void OnGameFontUpdated();
	void UpdateRichRect( const Ogre::RectInt& srcRect );
	void UpdateRichRect( float left, float top, float right, float bottom );
	void initViewStartPoint( RFPoint& ptViewStartPoin, const Ogre::RectFloat& rectLine );
	bool isMouseOnOneLine( const Ogre::RectFloat& rectLine, float fYCursorPos );
	
	// ������ͨ����
	void calculateNormalTextColor( Ogre::ColorQuad& color, const RichTextText* pText, unsigned int dwCurTime );
	bool canShowOneNormalText( const RFPoint& ptOneNormalTextStartViewPoint, const RichTextText* pText );
	void calculateNormalTextViewport( RFPoint& ptViewStartPoint, RFPoint& ptViewOffsetPoint, 
		float& fWidth, float& fHeight );
	void calculateNormalTextXViewport( RFPoint& ptViewStartPoint, RFPoint& ptViewOffsetPoint,
		float& fWidth );
	void calculateNormalTextYViewport( RFPoint& ptViewStartPoint, RFPoint& ptViewOffsetPoint,
		float& fHeight );
	void FinishDrawNormalText( const RichTextText* pText, const RFPoint& ptViewOffsetPoint, 
		Ogre::RectFloat& viewRect, unsigned int dwCurTime );
	// ������ͨ����

	// ���Ʊ���
	bool canShowOneFace( const RFPoint& ptOneFaceViewStartPoint );
	void calculateOneFaceViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
		RFSize& viewSize, RFSize& uvSize );
	void calculateOneFaceXViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
		RFSize& viewSize, RFSize& uvSize);
	void calculateOneFaceYViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
		RFSize& viewSize, RFSize& uvSize );
	void initOneFaceUVStartPoint( RFPoint& ptUVStartPoin, RichTextFace* pFace, unsigned int dwCurTime );
	void validateMaxFacesToRender( int& nFaceNum, Ogre::DrawRect* faces );
	void DrawFaceHighlightBackTex( const RFPoint& ptViewStartPoint, const RFSize& viewSize, 
		int& nFaceNum, Ogre::DrawRect* faces );
	void FinishDrawFace( RichTextFace* pFace, const RFPoint& ptViewStartPoint, const RFPoint& ptUVStartPoint, 
		const RFSize& viewSize, const RFSize& uvSize, int& nFaceNum, Ogre::DrawRect* faces );
	// ���Ʊ���

	// ����ͼƬ
	bool canShowOnePicture( const RFPoint& ptViewStartPoint, PictureData* pPictureData );
	void initOnePictureUVStartPoint( RFPoint& ptUVStartPoin, PictureData* pPictureData, RichTextPicture* pPicture, 
		unsigned int dwCurTime );
	void calculateOnePictureViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
		RFSize& viewSize, RFSize& uvSize, const PictureData* pPictureData,
		RPictureCodeMap* pPictureMapBuffer );
	void calculateOnePictureXViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
		RFSize& viewSize, RFSize& uvSize, const PictureData* pPictureData,
		RPictureCodeMap* pPictureMapBuffer );
	void calculateOnePictureYViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
		RFSize& viewSize, RFSize& uvSize, const PictureData* pPictureData,
		RPictureCodeMap* pPictureMapBuffer );

	void FinishDrawPicture( const RFPoint& ptViewStartPoint, const RFPoint& ptUVStartPoint, 
		const RFSize& viewSize, const RFSize& uvSize, 
		int& nPictureNum, Ogre::DrawRect* pictures );
	void validateMaxPictureToRender( int& nPictureNum, Ogre::DrawRect* pictures );
	// ����ͼƬ

public:
	static unsigned int		m_nInstanceCount;		// RichTextʵ�����������ڸ���ͼ���Ϊһ������
	
	int						m_FontStyle;			// ��������
	int						m_nLineInterval;		// �м��
	int						m_iFontIndex;			// ������������
	Ogre::HFONT				m_hLinkFont;			// ����������
	Ogre::ColorQuad			m_shadowColor;			// ������ߵ���ɫ

	RichTextObject*			m_pLinkHightLineObj;	// �����Ӹ߹�������
	Ogre::ColorQuad			m_LinkTextColor;		// ������������ɫ

	unsigned int			m_iMaxLines;			// �����ʾ������
	RichTextLineList		m_listLines;			// �������ж����б�

	RFPoint					m_newLineStartPoint;	// RichTex������һ�����ֵ����½Ƕ���
	RFPoint					m_viewStartPoint;		// ��ǰ���������������һ�е����ϽǶ��������

	float					m_fAlpha;				// ��ǰ����͸����
	bool                    m_bAutoExtend;          // �Ƿ��Զ���չ�ؼ�����
	float					m_fFaceXInterval;
	float					m_fFaceYInterval;

	std::deque<tagTextHistory> m_TextHistory;

private:
	Ogre::RectFloat		m_rect;					// ��ʾ���ֵľ�������
	bool				m_bCenterLine;
	static const int	m_sMaxDrawRectNum = 200;

	tagBlendTexture		m_faceBorderTex;
	tagRect_ToLua		m_linkTextRect;
};//tolua_export

inline void RichText::UpdateRichRect( const Ogre::RectInt& srcRect )
{
	m_rect = srcRect;
}

inline void RichText::UpdateRichRect( float left, float top, float right, float bottom )
{
	m_rect.m_Left	= left;
	m_rect.m_Right	= right;
	m_rect.m_Top	= top;
	m_rect.m_Bottom = bottom;
}

inline void RichText::initViewStartPoint( RFPoint& ptViewStartPoin, const Ogre::RectFloat& rectLine )
{
	ptViewStartPoin.m_x += rectLine.m_Left;
	ptViewStartPoin.m_y += rectLine.m_Bottom;
	// ��ȥ�ӿ������0��0�ο��������õ�����������ӿڵ�����
	ptViewStartPoin		-= m_viewStartPoint;

	// �����ӿڵ���Ļ����õ��õ������������Ļ������
	ptViewStartPoin.m_x += m_rect.m_Left;
	ptViewStartPoin.m_y += m_rect.m_Top;
}

inline bool RichText::isMouseOnOneLine( const Ogre::RectFloat& rectLine, float fYCursorPos )
{
	return ( m_AbsRect.m_Top + rectLine.m_Top < m_viewStartPoint.m_y + fYCursorPos ) &&
			( m_AbsRect.m_Top + rectLine.m_Bottom >= m_viewStartPoint.m_y + fYCursorPos );
}

inline void RichText::calculateNormalTextViewport( RFPoint& ptViewStartPoint, RFPoint& ptViewOffsetPoint, 
										   float& fWidth, float& fHeight )
{
	calculateNormalTextXViewport( ptViewStartPoint, ptViewOffsetPoint, fWidth );
	calculateNormalTextYViewport( ptViewStartPoint, ptViewOffsetPoint, fHeight );
}

// ������ͨ����
inline bool RichText::canShowOneNormalText( const RFPoint& ptOneNormalTextStartViewPoint, const RichTextText* pText )
{
	float fWidth	= pText->m_rect.getWidth();
	float fStartX	= ptOneNormalTextStartViewPoint.m_x;
	return fStartX < m_rect.m_Right && fStartX + fWidth > m_rect.m_Left;
}

// ������ͨ����

// ���Ʊ���
inline bool RichText::canShowOneFace( const RFPoint& ptViewStartPoint )
{
	return ptViewStartPoint.m_x < m_rect.m_Right && 
		ptViewStartPoint.m_x + g_pFrameMgr->m_nFaceWidth * g_pFrameMgr->GetScreenScaleX() > m_rect.m_Left;
}

inline void RichText::calculateOneFaceViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
												 RFSize& viewSize, RFSize& uvSize )
{
	calculateOneFaceXViewportAndUVRect( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize );
	calculateOneFaceYViewportAndUVRect( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize );
}

inline void RichText::validateMaxFacesToRender( int& nFaceNum, Ogre::DrawRect* faces )
{
	if ( nFaceNum == m_sMaxDrawRectNum )
	{
		RenderFaces( faces, nFaceNum );
		nFaceNum = 0;
	}
}

// ���Ʊ���

// ����ͼƬ
inline bool RichText::canShowOnePicture( const RFPoint& ptViewStartPoint, PictureData* pPictureData )
{
	const float PICTURE_WIDTH	= (float)pPictureData->m_nTexWidth   * g_pFrameMgr->GetScreenScaleY();
	return ( ptViewStartPoint.m_x < m_rect.m_Right ) && ( ptViewStartPoint.m_x + PICTURE_WIDTH > m_rect.m_Left );
}

inline void RichText::calculateOnePictureViewportAndUVRect( RFPoint& ptViewStartPoint, RFPoint& ptUVStartPoint, 
													RFSize& viewSize, RFSize& uvSize, const PictureData* pPictureData,
													RPictureCodeMap* pPictureMapBuffer )
{
	calculateOnePictureXViewportAndUVRect( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize, pPictureData, pPictureMapBuffer );
	calculateOnePictureYViewportAndUVRect( ptViewStartPoint, ptUVStartPoint, viewSize, uvSize, pPictureData, pPictureMapBuffer );
}

inline void RichText::validateMaxPictureToRender( int& nPictureNum, Ogre::DrawRect* pictures )
{
	if ( nPictureNum == m_sMaxDrawRectNum )
	{
		RenderPictures( pictures, nPictureNum );
		nPictureNum = 0;
	}
}

// ����ͼƬ

#endif
