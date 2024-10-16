
#ifndef __OgreUIRenderer_H__
#define __OgreUIRenderer_H__

#include "OgreColourValue.h"
#include "OgreRect.h"
#include "OgreVertexFormat.h"
#include "OgreSingleton.h"
#include "OgreFixedString.h"
#include "OgreHashTable.h"
#include "OgreRenderTypes.h"
#include "OgreUIRenderTypes.h"
#include "OgreSceneRenderer.h"
// #include "OgreUITextureHelper.h"
#include <vector>
#include <string>
#include <map>

namespace Ogre
{
	class TextureRenderTarget;
	struct UITargetEffect
	{
		Texture*				m_pUITexture;
		TextureRenderTarget*	m_pUIRT;
		HUIRES					m_hRes;
	};

	typedef void (*UIRendererCallback)();
	
	class _OgreExport UIRenderer : public Singleton<UIRenderer>, public SceneRenderer
	{
	public:
		UIRenderer();
		~UIRenderer();
		
		virtual void onLostDevice();
		virtual void onRestoreDevice();
		virtual void doRender();

		void setPreRenderCallback(UIRendererCallback pcallback)
		{
			m_pPreRenderCallback = pcallback;
		}
		
		UITargetEffect* getUITargetEffect( const char* pszPath );
		void AddUIRenderTarget( const char* pszKeyName, Texture* pTexture, TextureRenderTarget* pUIRT );
		bool IsCreatedUIRenderTarget( const std::string& strKeyName );

		void DrawScreenTexture(ScreenTexture *stex);
		
		void renderSceneToTarget( GameScene *pscene, Camera *pcamera, const RectInt &rect, const RectInt& destRect,
									RenderTarget* pTarget );
		void renderSceneToUI(GameScene *pscene, Camera *pcamera, const RectInt &rect);

		virtual HFONT CreateTrueTypeFont(int nCharWidth, int nCharHeight, const char* szFontName, ECharacterCoding eCoding, 
											unsigned int nExtType = FONTTYPE_NORMAL);
		virtual HFONT CreateBitmapFont(const char* szFontName, ECharacterCoding eCoding);

		virtual void	ReleaseTrueTypeFont(HFONT hFont);
		virtual int		GetFontHeight(HFONT hFont);

		virtual void	renderText(HFONT hFont, uint dwStyle, const char *pText,float fX, float fY,const ColorQuad &color, 
									float fScale = 1.0f, bool bDoTransfer = false, float fZ = 0.0f, const ColorQuad& scolor = FONT_SHADOW_COLOR);

		virtual void	renderTextRect(HFONT hFont, uint dwStyle, const char *pText, RectFloat &rect,
										float fOffsetX,	float fOffsetY, bool bAutoWrap, const ColorQuad &color, 
											float fScale = 1.0f, bool bDoTransfer = false, const ColorQuad& scolor = FONT_SHADOW_COLOR);

		virtual void	renderTextRect(HFONT hFont, uint dwStyle, uint dwAlign, const char *pText, RectFloat &rect, 
										const ColorQuad &color, float fScale = 1.0f, bool bDoTransfer = false, const ColorQuad& scolor = FONT_SHADOW_COLOR);

		virtual void	GetCharExtent(HFONT hFont, const char *pChar, float &fWidth, float &fHeight);
		virtual void	GetTextExtent(HFONT hFont, const char *pText, float &fWidth, float &fHeight);
		virtual void	GetTextExtentFitInWidth(HFONT hFont, const char* pText, float fFixedWidth, float& fWidth, int& nCount);
		virtual float	GetLineInterval(HFONT hFont);
		virtual void	SetLineInterval(HFONT hFont, float fLineInterval);
			
		void			GetTextureSize( HUIRES hres, int& nWidth, int& nHeight );
		HUIRES			GetTextureRes( const char* path );
		HUIRES			InsertTexture( const char* path, UIBlendMode blendmode, Texture* pTexData );
		HUIRES			UpdateTexture( const char* path, UIBlendMode blendmode, Texture* pTexData );
		virtual HUIRES	GetTexture(const char* path);
		virtual HUIRES	CreateTexture(const char* path, UIBlendMode blendmode, int* pWidth = NULL, int* pHeight = NULL);
		virtual HUIRES	CreateTexture(const char* path, Texture* pTex, UIBlendMode drawmode);
		virtual HUIRES	CreateTexture(Resource* pres, UIBlendMode blendmode, int* pWidth = NULL, int* pHeight = NULL);
		virtual void	SetUiTextureBlendModel(HUIRES hUiRes, UIBlendMode drawmode);
		virtual HUIRES	GetNullTexture();
		virtual void	AddRef(HUIRES h);
		virtual void	ReleaseUIRes(HUIRES h);

		virtual void	BeginDraw(HUIRES h,HUIRES hmasktex = HUIRES(0),float fZ = 0.0f,bool bGray = false );
		virtual void	DrawRect(int x, int y, int width, int height, unsigned long color, int tex_x=0, int tex_y=0, 
									float fAngle = 0.0f);
		virtual void	StretchRect(float x, float y, float width, float height, unsigned long color,bool bGray = false, int tex_x=0, int tex_y=0, 
										int tex_width=0, int tex_height=0, UiUvType eUvType = UI_UVT_NORMAL, float fAngle = 0.0f, 
											float offx=0.0f, float offy=0.0f,RectInt& maskRect = EMPTY_RECT_INT,bool bRotateTopLeft = false );
		virtual void	DrawLine(float x1, float y1, float x2, float y2, unsigned long color);
		virtual void	DrawBar(float x, float y, float width, float height, unsigned long dwColor);
		virtual void	DrawBox(float s, float y, float width, float height, unsigned long dwColor);
		virtual void	EndDraw();

		// 绘制三角形列表
		virtual void DrawTriangleList(Vector2* pVertex, unsigned int nTriangleCount, unsigned long dwColor);
		// 绘制三角形扇
		virtual void DrawTriangleFan(Vector2* pVertex, unsigned int nTriangleCount, unsigned long dwColor);
		virtual void DrawTriangleFan(Vector2* pVertex, unsigned int nTriangleCount, ColorQuad* pcolorInfo );

		// 将一个新的裁剪范围压栈
		virtual void PushClipRect(const RectFloat& rect);
		// 获取裁剪矩阵
		virtual const RectFloat& GetClipRect();
		// 将栈顶的裁剪区域出栈
		virtual void PopClipRect();

		virtual void setCursor(HUIRES h, int hotspotx, int hotspoty, int u, int v, int width, int height);

		Texture* getUIResTexture( HUIRES h );
// 		bool FindCacheTexture( const char* fileName, HUIRES& hUiRes, RectFloat &rectUV );
// 		void UpdateTexture(const char* szFileName,HUIRES hUiRes);

		ScreenTexture * getScreentexture(HUIRES h);

	private:
		typedef std::map<std::string, UITargetEffect> UI_TARGET_MAP;
		UI_TARGET_MAP		m_uiRenderTaregets;
		uint				m_CurTick;
		Material*			m_Mtls[MAX_BLEND_MODE][2];
		HUIRES				m_DefaultTexture;
		VertexFormat		m_VertFmt;
		VertexDeclHandle	m_VertDecl;
		std::vector<IFont*> m_vecFonts;

		bool				m_bClipEnabled;

		float				m_fZValue;		// 由BeginDraw()传入的Z值，用于之后绘制的元素

		struct UIResObject
		{
			ScreenTexture*			pscrtex;
			int						refcount;
		};
		typedef HashTable<FixedString, UIResObject, FixedStringHashCoder> UIResTable;
		UIResTable		m_AllUIRes;
		HUIRES			m_pCurUIRes;

		HUIRES			m_MaskTexture;
		RectInt			m_MaskRect;

		bool			m_bGray;

		std::vector<RectFloat>	m_vecClipRects;	// 保存裁剪矩形的堆栈

		UIRendererCallback		m_pPreRenderCallback;

		//UITextureHelper m_UITextureHelper;
	private:
		void	DrawUIElement(PrimitiveType type, VertexBuffer *pvb, size_t nprim, UIBlendMode mode, Texture *ptex, bool gray = false);
		HFONT	FindSamenessFont(int nFontWidth, int nFontHeight, const char* pFontFileName, ECharacterCoding eCoding, 
									unsigned int nExtType);
	};
}

#endif