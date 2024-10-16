#pragma once
#include "ui_layoutanchor.h"
#include "tinyxml.h"

//-----------------------------------------------------------------------------
// Layout Frame
//-----------------------------------------------------------------------------
class Frame;

//tolua_begin
class tagRect_ToLua
{
public:
	tagRect_ToLua()
	{
		m_nLeft		= 0;
		m_nRight	= 0;
		m_nTop		= 0;
		m_nBottom	= 0;
	}

	void empty();
	float getWidth();
	float getHeight();

	float		m_nLeft;
	float		m_nRight;
	float		m_nTop;
	float		m_nBottom;
};
//tolua_end

class LayoutFrame : public UIObject {//tolua_export
	DECLARE_UIOBJECT_CLONE(LayoutFrame)

public:
	LayoutFrame();
	void AddAnchor(const LayoutAnchor &anchor);
	void DrawShow(bool bShow);
	bool PointInLayoutFrame(int x, int y);

	void GetFramePoint(FRAMEPOINT_T pt, int &x, int &y);

	void SetLayOutSize(LayoutDim ld);
	LayoutDim GetSize();


	void MoveUp(float value); //offset的Y改变

	void SetFrameStrata(FRAMESTRATA_T strata);
	FRAMESTRATA_T GetFrameStrata();
	
	void SetViewStartPoint(int x, int y);
	void IncreaseViewStartPoint(int x, int y);	

	size_t GetCalAbsRectFrame(){ return m_CalAbsRectFrame; }
	
	virtual void CalAbsRectSelf(size_t frame);
	virtual void UpdateSelf(float deltatime);
	virtual void Draw();
	virtual void FillDrawItems();
	virtual TiXmlElement *Save(TiXmlElement *pElement);

	virtual const char *GetTypeName(){ return "LayoutFrame"; }

public:
	//tolua_begin
	void Show();
	void Active();
	void Hide();
	bool IsShown();
	
	float GetAnchorOffsetX();
	float GetAnchorOffsetY();
	
	void removeRelFrames(LayoutFrame* pFrame);
	void SetPoint(const char *strpoint, const char *relFrame, const char *strrelpoint, int xoffset, int yoffset);
	void SetPointEx(const char *strpoint, const char *relFrame,
		const char *strrelpoint, int xoffset, int yoffset);
	void SetPoint( FRAMEPOINT_T myPoint, const char* relFrame, FRAMEPOINT_T relPoint, int xoffset, int yoffset);

	// 每次setpoint 的时候都重新计算位置
	void SetPointAlwaysRecalc(const char *strpoint, const char *relFrame,const char *strrelpoint, int xoffset, int yoffset);
	
	
	int GetLeft();
	int GetRight();
	int GetTop();
	int GetBottom();

	int GetWidth();
	void SetWidth(int width); //动态改变高度

	void SetHeight(int height); //动态改变高度
	int GetHeight();

	int GetRealHeight();
	int GetRealWidth();
	int GetRealLeft();
	int GetRealRight();
	int GetRealTop();
	int GetRealBottom();
	
	virtual void setAbsRect(float fLeft,float fTop, float fRight, float fBottom);
	virtual void resizeRect( int Width, int Height );
	void extendRect( int Width, int Height );

	const char *GetParent();
	void SetSizeNoRecal(int x, int y);
	void SetSize(int x, int y);
	int GetTextExtentWidth( const char *name );
	int GetTextExtentHeight( const char *name );

	//获取客户端相关数据
	int GetClientID();
	void SetClientID(int id);
	void SetClientUserData(int index, int data);
	int GetClientUserData(int index);
	void SetClientString(const char *str);
	const char *GetClientString();
	// 获取窗口绘制级别
	int		getFrameDrawLevel();
	void	setInputTransparent( bool bTransparent );
	
	void	AddRelFrame( LayoutFrame* pRelFrame );

	virtual void	SetSelfScale(float scale);
	float	GetSelfScale( )
	{
		return m_fSelfScale;
	}

	//tolua_end
	void	SetFrameDraw( bool bFrameDraw );
	void	MoveFrameAbsrect( int nOffsetX, int nOffsetY );
	virtual void AdjustFrameByViewPoint();

	float	getFrameSizeX();
	float	getFrameSizeY();

	void GetAbsRect( Ogre::RectInt& rect );

public:
	FRAMESTRATA_T m_FrameStrata;

	int m_iDrawLevel;

	bool m_bSetAllPoints;
	bool m_bShow;
	bool m_bInputTransparent; //输入消息传过来， 会让它通过传给下一级（比如游戏场景）

	Ogre::RectInt m_AbsRect;
	Ogre::RectInt m_ClipRect;

	Ogre::Point2D m_ViewStartPoint;	//显示区域的偏移位置
	bool m_bFrameDraw;

	size_t m_CalAbsRectFrame;

	Frame *m_pParent;

	enum
	{
		MAX_ANCHOR = 2
	};

	LayoutAnchor m_Anchor[MAX_ANCHOR];
	LayoutDim m_Size;

	std::vector<LayoutFrame *> m_RelFrames;	//相对于该窗口定义位置的窗口
public:
	//LayoutFrame* GetParent()
	//{
	//	return m_pParent;
	//}
protected:
	//客户端相关数据
	int m_ClientId;

	int m_ClientUserData[4];

	std::string m_ClientString;

	virtual ~LayoutFrame();

protected:
	float m_fSelfScale;

};//tolua_export

typedef int (*ScriptFuncPtr)(lua_State *ls);
