
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
	std::string			m_strTextHistory;	// 文本记录
	Ogre::ColorQuad		m_lineColor;		// 当行颜色		
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

	// 渲染接口
	void AddRenderText(const char *szText, const Ogre::ColorQuad& color);

	// 渲染接口
	void SetRenderText(const char *szText, const Ogre::ColorQuad& color);

	// 改变尺寸，目前会清空所有内容
	void Resize();

	// 设置RichText左上角位置
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
	
	//查找鼠标点击的位置所有的控件
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
	// 设置透明度
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
	// 重新设置文本
	void resizeText( int Width, int Height );

	// 加入一行文字
	void AddText(const char *szText, int r, int g, int b);

	// 设置一行文字，清除以前的文字
	void SetText(const char *szText, int r, int g, int b);
	//
	void SetAutoExtend( bool bAutoExtend );
	
	// 设置超链接文字颜色
	void SetLinkTextColor(int r, int g, int b);

	// 设置字体描边颜色
	void SetShadowColor(int r, int g, int b);

	// 设置描边阴影 普通-FONTSYTLE_NORMAL 0 阴影-FONTSYTLE_SHADOW 1 描边-FONTSYTLE_BORDER	2
	void SetFontType( int nType);

	// 清除现有的内容
	void Clear();
	void clearHistory();
	// 设置垂直方向的显示位置
	void SetDispPos(float fY);
	void IncrDispPos( float fY );

	// 获取垂直方向的显示位置
	float GetDispPos();
	
	// 设置纵方向的显示位置
	void SetDispPosX(float fY);

	// 获取纵向方向的显示位置
	float GetDispPosX();

	// 获取第一屏的显示位置
	float GetStartDispPos();

	// 获取最后一屏的显示位置
	// 这用于返回最后当前显示内容中的最后一行的起始位置
	float GetEndDispPos();

	// 向上滚动一行
	void ScrollUp();
	
	// ScrollUp和ScroolDown仅仅只是改变m_newLineStartPoint的值,而不改变m_newLineStartPoint的值
	// 向下滚动一行
	void ScrollDown();

	// 滚动第一行
	void ScrollFirst();
	// 滚动到最后一行
	void ScrollEnd();

	int GetTextLines();
	int getLineRealWidth( int nLine );
	int getLineWidth( int nLine );
	// 获得指定行宽度
	int GetLineWidth(int index);
	// 获得行高度
	int GetLineHeight( int index );
	// 获得总高度
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
	
	// 绘制普通文字
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
	// 绘制普通文字

	// 绘制表情
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
	// 绘制表情

	// 绘制图片
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
	// 绘制图片

public:
	static unsigned int		m_nInstanceCount;		// RichText实例计数，用于给贴图起个为一的名字
	
	int						m_FontStyle;			// 字体类型
	int						m_nLineInterval;		// 行间距
	int						m_iFontIndex;			// 字体类型索引
	Ogre::HFONT				m_hLinkFont;			// 链接字体句柄
	Ogre::ColorQuad			m_shadowColor;			// 设置描边的颜色

	RichTextObject*			m_pLinkHightLineObj;	// 超链接高光区内容
	Ogre::ColorQuad			m_LinkTextColor;		// 超链接文字颜色

	unsigned int			m_iMaxLines;			// 最大显示的行数
	RichTextLineList		m_listLines;			// 包含的行对象列表

	RFPoint					m_newLineStartPoint;	// RichTex最下面一行文字的右下角顶点
	RFPoint					m_viewStartPoint;		// 当前可视区域的最上面一行的左上角顶点的坐标

	float					m_fAlpha;				// 当前绘制透明度
	bool                    m_bAutoExtend;          // 是否自动扩展控件区域
	float					m_fFaceXInterval;
	float					m_fFaceYInterval;

	std::deque<tagTextHistory> m_TextHistory;

private:
	Ogre::RectFloat		m_rect;					// 显示文字的矩形区域
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
	// 减去视口相对于0，0参考点的坐标得到表情相对于视口的坐标
	ptViewStartPoin		-= m_viewStartPoint;

	// 加上视口的屏幕坐标得到得到表情相对于屏幕的坐标
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

// 绘制普通文字
inline bool RichText::canShowOneNormalText( const RFPoint& ptOneNormalTextStartViewPoint, const RichTextText* pText )
{
	float fWidth	= pText->m_rect.getWidth();
	float fStartX	= ptOneNormalTextStartViewPoint.m_x;
	return fStartX < m_rect.m_Right && fStartX + fWidth > m_rect.m_Left;
}

// 绘制普通文字

// 绘制表情
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

// 绘制表情

// 绘制图片
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

// 绘制图片

#endif
