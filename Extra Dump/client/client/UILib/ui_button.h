#pragma once
#include "ui_frame.h"
#include "ui_fontstring.h"
#include "ui_layoutanchor.h"
#include "ui_texture.h"
#include "ui_framemgr.h"

//-----------------------------------------------------------------------------
// Button control
//-----------------------------------------------------------------------------

struct ButtonOverlay
{
	tagTexUV uv;
	bool bTemplate;
	std::string templateName;
};

class Button : public Frame { //tolua_export
	DECLARE_UIOBJECT_CLONE(Button)
public:
	Button();

	virtual void Init();

	void SetPushedTexture(Ogre::HUIRES h);
	
	void DrawCooldownBaseTriangle(Ogre::Vector2 *pVertexBuffer, int iThTriangle, float width, float height);
	void DrawCooldownLastTriangle(Ogre::Vector2 *pVertexBuffer, int numTriangle, float width, float height, float fAngle);
	
	void DrawFadeBaseTriangle(Ogre::Vector2 *pVertexBuffer, int iThTriangle, float width, float height);
	void DrawFadeLastTriangle(Ogre::Vector2 *pVertexBuffer, int numTriangle, float width, float height, float fAngle);

	virtual void FillDrawItems();
	void DrawCooldown();
	void DrawFade();
	virtual void Draw();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	virtual void OnBeginDrag(unsigned int millisecond, int dx, int dy);

	virtual int OnInputMessage(const Ogre::InputEvent &event);//Frame�ؼ���Ϣ���� UI����ķ���0�� ������ķ���1�����ͻ��˴���
	virtual const char *GetTypeName(){ return "Button"; }
	virtual void SetViewStartPointRecursive(int x, int y);
public:
	//tolua_begin
	void SetCooldownTimer(float start, float duration, bool enable, bool bFade = false );
	bool IsCooldown();
    void AddLock(); ///< ����
	
	void	SetNormalRegionWidth(float width);
	void	SetNormalRegionAbsWidth(float width);
	float	GetNormalRegionWidth();
	void	SetNormalRegionHight(float Hight);
	float	GetNormalRegionHight();

	void	SetOverlayRegionAbsWidth(float width);
	float	GetOverlayRegionWidth();

	void SetNormalTexture(const char *path, const char *strdrawmode);
	void SetPushedTexture(const char *path, const char *strdrawmode);
	void SetDisableTexture(const char *path, const char *strdrawmode);
	std::string GetNormalTexture();
	void SetNormalTextureHuires(Ogre::HUIRES h);

	void SetText(const char *buttontext);
	const char *GetText();

	void SetTextColor(int r, int g, int b);

	void SetIntonateTimer(float start, float total);

	void SetHightlightTexture(const char *path, const char *strdrawmode);

	void SetDisableTextureHuires(Ogre::HUIRES h);
	Ogre::HUIRES GetDisableTextureHuires();

	void SetHighlightTextureHuires(Ogre::HUIRES h);
	Ogre::HUIRES GetHighlightTextureHuires();
	
	void SetBlendAlpha(float alpha);
	void SetGray (bool bGray);
	void SetGrayEx(bool bGray);

	void SetPushedState();
	bool GetPushedState();

	void Disable();
	void Enable();
    bool IsEnable();

	void Highlight();
	void DisHighlight();
	bool IsHighlight();

	void Checked();
	void DisChecked();
    bool IsChecked();

	int OnLButtonDown();
	int OnLButtonUp();

	void SwapCooldownMembers( Button *pdest );
	void CopyCooldownMembers( Button* pdest );

	// ������ȴ��ͼ�Ļ�������
	void SetCooldownTextureRect( int offsetleft, int offsettop, int width, int height );

	//����button��ͼƬ����ת�Ƕ�
	void SetAngle(float fAngle);
	float GetAngle();

	// ����button��NormalTexture
	void	SetNormalTextureSize( int width, int height );
	void	ChangeNormalTexture( int x, int y, int width, int height );
	void	SetNormalTextureRelUV( float x, float y );

	//void	SetHighlightTextureSize( int width, int height );
	void	ChangeHighlightTexture( int x, int y, int width, int height );

	//void	SetPushedTextureSize( int width, int height );
	void	ChangePushedTexture( int x, int y, int width, int height );
	
	//void	SetDisableTextureSize( int width, int height );
	void	ChangeDisableTexture( int x, int y, int width, int height );

	void	SetOverlayRegion( int offsetX, int offsetY, int width, int height );

	void	SetNormalTextureTemplate( const char* template_name );
	void	SetHighlightTextureTemplate( const char* template_name );
	void	SetPushedTextureTemplate( const char* template_name );
	void	SetDisableTextureTemplate( const char* template_name );
	
	float GetCoolStart()
	{
		return m_fCooldownStart;
	}
	float GetCoolTotal()
	{
		return m_fCooldownDuration;
	}

	void ClearPushState();

	void	SetNormalTextColor(int r,int g,int b);
	void	SetPushedTextColor(int r,int g,int b);
	void	SetHighlightTextColor(int r,int g,int b);
	void	SetDisableTextColor(int r,int g,int b);

	void	SetButtonColor( int r,int g,int b );
	//tolua_end

public:
	Texture *m_pNormalRegion;
	Texture *m_pOverlayRegion;
    UIObject* m_pAddon;     ///< ���Ӳ������缼���ͷ�����ͼ�꣩

	Ogre::HUIRES m_hNormalTex;
	Ogre::HUIRES m_hPushedTex;
	Ogre::HUIRES m_hDisableTex;
	Ogre::HUIRES m_hHighlightTex;

	std::string m_NormalTexName;
	std::string m_PushedTexName;
	std::string m_DisabledTexName;
	std::string m_HighlightTexName;
	std::string m_strClickSoundPath;		//��ť���ʱ���ŵ�����·�� 
	
	typedef std::map<std::string, ButtonOverlay>::iterator OVERLAYIT;

	std::map<std::string, ButtonOverlay> m_OverlayTexUV;

	FontString*		m_pNormalFontString;

	Ogre::ColorQuad m_NormalColor;
	Ogre::ColorQuad m_HighlightColor;
	Ogre::ColorQuad m_DisableColor;
	Ogre::ColorQuad m_PushedColor;
	LayoutDim m_PushedTextOffset;

	Ogre::RectInt m_CooldownRect;

	bool	m_bCooldown;
	float	m_fCooldownStart;		//��λΪ��
	float	m_fCooldownDuration;	//��λΪ��
	bool	m_bFade;
    bool m_bLocked;             ///< �Ƿ����

	//������������ʱ������
	bool m_bIntonate;
	float m_fIntonateStart;
	float m_fIntonateTotal;

	//Button��ͼƬ����ת�Ƕ�,����С��ͼ��ָ����ѵļ�ͷ��Ҫ�����������Σ���������Ӧ����׼ȷ
	float m_fAngle;

	std::string m_Caption;
protected:
	virtual ~Button();
	void UpdateSelf(float deltatime);
};//tolua_export