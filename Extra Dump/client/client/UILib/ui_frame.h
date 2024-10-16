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
	virtual void FillDrawItems();	//把该桢需要的渲染对象加入渲染队列
	virtual void FillDrawLevel();	
	virtual void Draw();	//绘制控件,在Update, FillDrawItems之后绘制
	virtual TiXmlElement *Save(TiXmlElement *pElement);
	
	virtual Frame* transferEventToNextFrame( int nMouseX, int nMouseY );
	virtual int OnInputMessage(const Ogre::InputEvent &event);//Frame控件消息处理， UI处理的返回0， 不处理的返回1交给客户端处理
	virtual const char *GetTypeName(){ return "Frame"; }
	

	void RegisterToFrameMgr(FrameManager *pframemgr);

	int OnChar(const Ogre::InputEvent &event);
	int OnKeyDown(const Ogre::InputEvent &event);
	
	void FillChildren();	//迭代函数,遍历所有子控件

	virtual bool ProcessMouseWheel();

	virtual void OnBeginDrag(unsigned int millisecond, int dx, int dy);
	void AddHitRect(const LayoutRect &rect);

	void DisableDrawLayer(DRAWLAYER_T layer);
	void EnableDrawLayer(DRAWLAYER_T layer);

	void AddFontString(DRAWLAYER_T layer, FontString *pfontstr);	//为frame添加一个FontString，添加后会绘制
	void AddTexture(DRAWLAYER_T layer, Texture *ptex);	//为frame添加一个Texture，添加后会绘制
	void AddModelView(DRAWLAYER_T layer, ModelView *pview);
	void AddLineFrame( DRAWLAYER_T layer, DrawLineFrame *pline );

	void AddChildFrame(Frame *pframe);	//添加一个子控件
	Frame* GetChildren();

	void ShowUIPanel();
	void HideUIPanel();
	void ReplaceSpecialName();	//把模版中的$parent替换成正确的Frame Name
	void InitFrameStrata();	//初始化一级排序索引FrameStrata

	int GetNumRegions();	//获取当前的绘制对象总数

	int GetFrameBottom();	//获取一个frame的下部,包含其子窗口
	int GetFrameTop(); //获取一个frame的上部,包含其子窗口
	virtual void SetViewStartPointRecursive(int x, int y);
	void IncreaseViewStartPointRecursive(  int x, int y );	// 增量移动frame
	void MoveFrameRecursive( int x, int y ); // 移动frame

	void MoveTo();

	virtual void CalFrameRectRecursive(size_t iFrame);	//迭代函数,每桢计算控件的实际位置

	virtual void FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *> &vFrameTemp );	//查找鼠标点击的位置所有的控件

	void DrawBackDrop();
	void DrawDebugBarEdge(); //UI编辑器的绘制边框函数
	void setModalFrame( const char *str );
	
	//bool SaveXMLNode();		//保存xml Node

public:
	//tolua_begin
	void RegisterEvent(const char *event);
	void SetFrameLevel(int level);
	int GetFrameLevel();

	void SetClipState(bool bClip);

	void SetBlendAlpha(float alpha);

	void setUpdateTime( float fUpdateTime );
	
	// 设置背景的透明度
	float GetBackDropBlendAlpha();
	void SetBackDropBlendAlpha(float alpha);

	void SetBackDropColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a=255);

	void MoveFrame( const char *strMovePoint, float nMoveDuration, int nOffsetX, int nOffsetY );
	void EndMoveFrame( );
	void SetBackDropBorderTex( const char* pszBorderTexPath );
	void SetBackDropEdgeTex( const char* pszEdgeTexPath );
	void SetBackDropAttr( int nEdgeSize, int nInsetLeft, int nInsetRight, int nInsetTop, int nInsetBottom );

	virtual void	SetSelfScale(float scale);

	void AddLevelRecursive(int iLevel=1);	//鼠标点击后把该控件的FrameLevel提高，绘制的时候不会出现排序问题
	//tolua_end
public:
	struct DrawObj	
	{
		LayoutFrame *pregion;
		DRAWLAYER_T layer;
		//bool bDisable;
	};

	std::vector<DrawObj>m_DrawRegions;	//frame的绘制对象

	float m_fAlpha;	//透明度
	bool m_bMovable;	//是否可以移动
	bool m_bResizable;	//是否可以改变大小
	bool m_bEnableMouse;	//鼠标是否响应
	bool m_bEnableKeyboard;	//键盘是否响应
	bool m_bClampedToScreen;
	bool m_bProtected;
	
	float	m_fMoveStart;		//单位为秒
	float	m_fMoveDuration;	//单位为秒

	LayoutDim		m_InitOffset;
	Ogre::Point2D	m_MoveOffset;
	FRAMEPOINT_T	m_MovePoint;

	int m_Pattern;	//样式
	int m_Style;	//风格
	int m_State;	//状态

	int m_iFrameLevel;	//frame 层次
	bool m_bToplevel;	//是否是顶层

	eUIFrameType m_Type;	//frame 类型

	std::vector<Frame *> m_Children;	//子控件数组

	//backdrop
	int m_iEdgeSize;
	int m_iTileSize;
	Ogre::ColorQuad m_BorderColor;
	Ogre::ColorQuad m_EdgeColor;
	bool m_bTile;
	Ogre::RectInt m_BackgroundInsets;

	//tooltip显示
	bool m_bShowTooltip;
	float m_IdleTime;	//tooltip显示的延迟时间
	float m_IdleAccTime; //tooltip累加时间

	float m_UpdateTime;		//触发OnUpdate消息的延迟时间
	float m_UpdateAccTime;	//触发OnUpdate消息的累加时间

	Ogre::HUIRES m_hEdgeTex;
	Ogre::HUIRES m_hBorderTex;

	std::string m_EdgeTexName;
	std::string m_BorderTexName;
	std::string m_strShowSoundPath;
	std::string m_strHideSoundPath;
	std::string m_strModalFrame; //模态窗口,在子窗口添加属性,modalFrame="父窗口名"
	
protected:
	int m_iMousePositionX;
	int m_iMousePositionY;

	virtual ~Frame();

};//tolua_export