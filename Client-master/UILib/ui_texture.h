#pragma once
#include "ui_layoutframe.h"
#include "OgreLoadWrap.h"

//-----------------------------------------------------------------------------
// Texture
//-----------------------------------------------------------------------------

struct TextureUV
{
	TextureUV():texX(0),texY(0),texWidth(0),texHeight(0){};
	TextureUV(int x,int y,int w,int h):texX(x),texY(y),texWidth(w),texHeight(h){};
	int texX;
	int texY;
	int texWidth;
	int texHeight;
	void set(TextureUV& uv)
	{
		texX = uv.texX;
		texY = uv.texY;
		texWidth = uv.texWidth;
		texHeight = uv.texHeight;
	}
};

enum TEXTURE_DRAW_TYPE{
	eDraw_Normal,		//正常画
	eDraw_Tile,			//back drop 的画
	eDraw_NineSquare,	// 九宫格
	eDraw_Center,		//按钮用，左中右
	eDraw_Height,		//滑动条用，上中下
};

class Texture : public LayoutFrame, public Ogre::LoadWrap { //tolua_export
	DECLARE_UIOBJECT_CLONE(Texture)
public:
	Texture();
	~Texture();

	virtual void Draw();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	void SetBlendMode(Ogre::UIBlendMode blendmode);

	virtual const char *GetTypeName(){ return "Texture"; }

public:
	//tolua_begin
	void SetUVAnimation(int timeslot, bool loopmode);

	void SetTextureHuires(Ogre::HUIRES hTex);
	Ogre::HUIRES GetTextureHuires();

	void SetColor(int r, int g, int b);

	void SetGray (bool bGray);
	bool IsGray ();

	void SetTexture(const char *path);
	const char *GetTexture();

	void	SetAngle(float fAngle);
	float	GetAngle();

	void	SetTexUV(int x, int y, int width, int height);
	void	SetTexRelUV(float width, float height);
	void	SetTexRelUV(float x, float y, float width, float height);
	int		getRelWidth();
	int		getRelHeight();

	void	setTextOffsetY( float person );
	void	setTextOffsetX( float person );
	float	getTextOffsetY();
	float	getTextOffsetX();

	void SetBlendAlpha(float alpha);
	float GetBlendAlpha();

	void setMask( const char* szmaskpath );

	void setUvType( int nType);

	void setRotateTopLeft( bool b ){m_bRotateTopLeft = b;}

	void ChangeTextureTemplate( const char* template_name );

	void StopUVAnim();

	void StartAlphaAmin( float speed );

	void StopAlphaAmin();

	bool IsPlayAlphaAnim(){ return m_bAlphaAmin; }
	//tolua_end
	void setMask(const char *path, bool block);
	void setMask(Ogre::Resource *pres);
	void SetTexture(const char *path, bool block);
	void SetTexture(Ogre::Resource *pres);
	void SetTextureTemplate( std::string template_name );
	void ResourceLoaded(Ogre::Resource *rec, Ogre::ResourceHandle h);

public:
	Ogre::HUIRES m_hTex;
	Ogre::HUIRES m_hLastTex;
	Ogre::HUIRES m_hMaskTex;
	Ogre::ResourceHandle m_hTexRes;
	Ogre::ResourceHandle m_hMaskTexRes;
	Ogre::UIBlendMode m_BlendMod;
	std::string m_TexturePath;
	std::string m_MaskTexturePath;

	Ogre::ColorQuad m_Color;
	bool m_bGray;

	int m_iRelWidth;
	int m_iRealHeight;

	int m_iTexRows;
	int m_iTexCols;

	int m_iTexX;
	int m_iTexY;
	int m_iTexWidth;
	int m_iTexHeight;

	float m_fTexRelX;
	float m_fTexRelY;
	float m_fTexRelWidth;
	float m_fTexRelHeight;

	float m_fAngle;
	float m_fOffsetX;
	float m_fOffsetY;

	bool m_bUVAnim;
	bool m_bUIAnimLoop;
	bool m_bScanl;

	Ogre::RectFloat	m_CacheUV;

	int m_iTileSize;
	Ogre::RectInt m_BackgroundInsets;

	TextureUV m_TopLeftUV;
	TextureUV m_TopUV;
	TextureUV m_TopRightUV;
	TextureUV m_LeftUV;
	TextureUV m_CenterUV;
	TextureUV m_RightUV;
	TextureUV m_BottomLeftUV;
	TextureUV m_BottomUV;
	TextureUV m_BottomRightUV;
	int	m_nNineSquareOffsetX;
	int m_nNineSquareOffsetY;

	TEXTURE_DRAW_TYPE m_DrawType;

	int m_UvType;

	bool m_bRotateTopLeft;
	bool m_bAlphaAmin;
	float m_fSpeed;
protected:
	int m_iTimeSlot;

	int m_iCurTime;
public:
	void UpdateSelf(float deltatime);
private:
	void DrawAsBackDrop();
	void DrawAsNineSquare();
	void DrawAsCenter();
	void DrawAsHeight();
	void AddDrawRect(float left,float top,float width,float height,unsigned int color,bool bGray,TextureUV uv);

};//tolua_export