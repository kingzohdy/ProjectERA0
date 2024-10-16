#pragma once
#include "ui_frame.h"
#include "ui_fontstring.h"
#include "ui_framemgr.h"
#include "OgreRect.h"

//-----------------------------------------------------------------------------
// ScrollFrame control
//-----------------------------------------------------------------------------
class ScrollFrame : public Frame { //tolua_export
	DECLARE_UIOBJECT_CLONE(ScrollFrame)
public:
	ScrollFrame();
	~ScrollFrame();

	//unsigned int GetVerticalScroll();

	// 设置ScrollFrame中slider子控件
	virtual void FillDrawItems();
	virtual void Draw();
	bool CanDraw( const Ogre::RectInt& srcRect );
	virtual TiXmlElement *Save( TiXmlElement* pElement );
	
	//查找鼠标点击的位置所有的控件
	virtual void FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *>& vFrameTemp );	
	virtual void CalFrameRectRecursive(unsigned int iFrame);
	virtual int OnInputMessage( const Ogre::InputEvent& event );
	virtual const char* GetTypeName()
	{ 
		return "ScrollFrame"; 
	}
	virtual void AdjustFrameByViewPoint();

	void	AdjustChildren();
	void	reCalChildrenAbs();
	
public:
	//tolua_begin
	void	CalVerticalScrollRange();
	void	CalHorizonalScrollRange();
	void SetValueStep( int value );
	int GetValueStep();
	
	void SetScrollChildRect( float fX, float fY );
	void IncreaseScrollChildRect( float nOffsetX, float nOffsetY );

	void	SetVerticalScroll( float value );
	void	IncreaseVerticalScroll( float value );
	int		GetVerticalOffset();
	void	SetVerticalScrollRange( int nRange );
	int		GetVerticalScrollRange();

	void	SetHorizonalScroll( float value );
	void	IncreaseHorizonalScroll( float value );
	int		GetHorizonalOffset();
	void	SetHorizonalScrollRange( int nRange );
	int		GetHorizonalScrollRange();
	
	void	ResetChildAnchor()
	{
		m_bAdjust = false;
	}
	//tolua_end
private:
	// clamp水平位移
	void	ClampHorizonalScroll( float fOffsetX );
	//clamp竖直位移
	void	ClampVerticalScroll( float fOffsetY );
public:
	int m_nValueStep;	// 此为滚动面板每移动一次经过的单位距离

	FontString*	m_pFontString;
	
	int		m_nVerticalScrollRange;		// 竖直最大滚动距离，以像素为单位
	int		m_nVerticalHaveScrollDist;	// 已经滚动过的竖直距离，以像素为单位

	int		m_nHorizonalScrollRange;		// 水平最大滚动距离，以像素为单位
	int		m_nHorizonalHaveScrollDist;		// 已经滚动过的水平距离，以像素为单位

	int		m_nMaxVerticalScrollNum;	// 竖直方向最大滚动次数
	int		m_nOffsetY;					// 累计竖直方向已滚动的总次数

	int		m_nMaxHorizonalScrollNum;	// 水平方向最大滚动次数
	int		m_nOffsetX;					// 累计水平方向已滚动的总次数

	int		m_nLastVerticalScrollNum;
	int		m_nLastHorizonalScrollNum;
	
	int		m_nCurChangeX;
	int		m_nCurChangeY;
	
	// 使用者可能改变某个子控件的大小
	bool	m_bAdjust;			// 将所有子控件的描点都调整为相对这个ScrollFrame,考虑到使用者要求显示重新强制描点

	int		m_nPreChildNum;		// 上一次子控件个数
	bool	m_bReCalAbs;

protected:
	void UpdateSelf(float deltatime);
};//tolua_export