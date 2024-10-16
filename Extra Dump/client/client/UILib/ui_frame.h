#pragma once
#include "ui_layoutframe.h"
#include "ui_layoutanchor.h"
#include "ui_framemgr.h"

class FontString;
class Texture;
class ModelView;
class DrawLineFrame;
struct AccelItem;

class LayoutRect
{
public:
	LayoutDim m_Start;
	LayoutDim m_End;
};

//-----------------------------------------------------------------------------
// Frame control
//-----------------------------------------------------------------------------
class Frame : public LayoutFrame { //tolua_export
	DECLARE_UIOBJECT_CLONE(Frame)
public:
	Frame();

	static void ScriptRelease( Frame* pButton )
	{
	}

	void UpdateHiddenFrameRecursive();

	virtual void UpdateSelf(float deltatime);	//Upata UI each frame
	virtual void FillDrawItems();	//�Ѹ�����Ҫ����Ⱦ���������Ⱦ����
	virtual void FillDrawLevel();	
	virtual void Draw();	//���ƿؼ�,��Update, FillDrawItems֮�����
	virtual TiXmlElement *Save(TiXmlElement *pElement);
	
	virtual Frame* transferEventToNextFrame( int nMouseX, int nMouseY );
	virtual int OnInputMessage(const Ogre::InputEvent &event);//Frame�ؼ���Ϣ���� UI����ķ���0�� ������ķ���1�����ͻ��˴���
	virtual const char *GetTypeName(){ return "Frame"; }
	

	void RegisterToFrameMgr(FrameManager *pframemgr);

	int OnChar(const Ogre::InputEvent &event);
	int OnKeyDown(const Ogre::InputEvent &event);
	
	void FillChildren();	//��������,���������ӿؼ�

	virtual bool ProcessMouseWheel();

	virtual void OnBeginDrag(unsigned int millisecond, int dx, int dy);
	void AddHitRect(const LayoutRect &rect);

	void DisableDrawLayer(DRAWLAYER_T layer);
	void EnableDrawLayer(DRAWLAYER_T layer);

	void AddFontString(DRAWLAYER_T layer, FontString *pfontstr);	//Ϊframe���һ��FontString����Ӻ�����
	void AddTexture(DRAWLAYER_T layer, Texture *ptex);	//Ϊframe���һ��Texture����Ӻ�����
	void AddModelView(DRAWLAYER_T layer, ModelView *pview);
	void AddLineFrame( DRAWLAYER_T layer, DrawLineFrame *pline );

	void AddChildFrame(Frame *pframe);	//���һ���ӿؼ�
	Frame* GetChildren();

	void ShowUIPanel();
	void HideUIPanel();
	void ReplaceSpecialName();	//��ģ���е�$parent�滻����ȷ��Frame Name
	void InitFrameStrata();	//��ʼ��һ����������FrameStrata

	int GetNumRegions();	//��ȡ��ǰ�Ļ��ƶ�������

	int GetFrameBottom();	//��ȡһ��frame���²�,�������Ӵ���
	int GetFrameTop(); //��ȡһ��frame���ϲ�,�������Ӵ���
	virtual void SetViewStartPointRecursive(int x, int y);
	void IncreaseViewStartPointRecursive(  int x, int y );	// �����ƶ�frame
	void MoveFrameRecursive( int x, int y ); // �ƶ�frame

	void MoveTo();

	virtual void CalFrameRectRecursive(size_t iFrame);	//��������,ÿ�����ؼ���ʵ��λ��

	virtual void FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *> &vFrameTemp );	//�����������λ�����еĿؼ�

	void DrawBackDrop();
	void DrawDebugBarEdge(); //UI�༭���Ļ��Ʊ߿���
	void setModalFrame( const char *str );
	
	//bool SaveXMLNode();		//����xml Node

public:
	//tolua_begin
	void RegisterEvent(const char *event);
	void SetFrameLevel(int level);
	int GetFrameLevel();

	void SetClipState(bool bClip);

	void SetBlendAlpha(float alpha);

	void setUpdateTime( float fUpdateTime );
	
	// ���ñ�����͸����
	float GetBackDropBlendAlpha();
	void SetBackDropBlendAlpha(float alpha);

	void SetBackDropColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a=255);

	void MoveFrame( const char *strMovePoint, float nMoveDuration, int nOffsetX, int nOffsetY );
	void EndMoveFrame( );
	void SetBackDropBorderTex( const char* pszBorderTexPath );
	void SetBackDropEdgeTex( const char* pszEdgeTexPath );
	void SetBackDropAttr( int nEdgeSize, int nInsetLeft, int nInsetRight, int nInsetTop, int nInsetBottom );

	virtual void	SetSelfScale(float scale);

	void AddLevelRecursive(int iLevel=1);	//�������Ѹÿؼ���FrameLevel��ߣ����Ƶ�ʱ�򲻻������������
	//tolua_end
public:
	struct DrawObj	
	{
		LayoutFrame *pregion;
		DRAWLAYER_T layer;
		//bool bDisable;
	};

	std::vector<DrawObj>m_DrawRegions;	//frame�Ļ��ƶ���

	float m_fAlpha;	//͸����
	bool m_bMovable;	//�Ƿ�����ƶ�
	bool m_bResizable;	//�Ƿ���Ըı��С
	bool m_bEnableMouse;	//����Ƿ���Ӧ
	bool m_bEnableKeyboard;	//�����Ƿ���Ӧ
	bool m_bClampedToScreen;
	bool m_bProtected;
	
	float	m_fMoveStart;		//��λΪ��
	float	m_fMoveDuration;	//��λΪ��

	LayoutDim		m_InitOffset;
	Ogre::Point2D	m_MoveOffset;
	FRAMEPOINT_T	m_MovePoint;

	int m_Pattern;	//��ʽ
	int m_Style;	//���
	int m_State;	//״̬

	int m_iFrameLevel;	//frame ���
	bool m_bToplevel;	//�Ƿ��Ƕ���

	eUIFrameType m_Type;	//frame ����

	std::vector<Frame *> m_Children;	//�ӿؼ�����

	//backdrop
	int m_iEdgeSize;
	int m_iTileSize;
	Ogre::ColorQuad m_BorderColor;
	Ogre::ColorQuad m_EdgeColor;
	bool m_bTile;
	Ogre::RectInt m_BackgroundInsets;

	//tooltip��ʾ
	bool m_bShowTooltip;
	float m_IdleTime;	//tooltip��ʾ���ӳ�ʱ��
	float m_IdleAccTime; //tooltip�ۼ�ʱ��

	float m_UpdateTime;		//����OnUpdate��Ϣ���ӳ�ʱ��
	float m_UpdateAccTime;	//����OnUpdate��Ϣ���ۼ�ʱ��

	Ogre::HUIRES m_hEdgeTex;
	Ogre::HUIRES m_hBorderTex;

	std::string m_EdgeTexName;
	std::string m_BorderTexName;
	std::string m_strShowSoundPath;
	std::string m_strHideSoundPath;
	std::string m_strModalFrame; //ģ̬����,���Ӵ����������,modalFrame="��������"
	
protected:
	int m_iMousePositionX;
	int m_iMousePositionY;

	virtual ~Frame();

};//tolua_export