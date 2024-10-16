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

	// ����ScrollFrame��slider�ӿؼ�
	virtual void FillDrawItems();
	virtual void Draw();
	bool CanDraw( const Ogre::RectInt& srcRect );
	virtual TiXmlElement *Save( TiXmlElement* pElement );
	
	//�����������λ�����еĿؼ�
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
	// clampˮƽλ��
	void	ClampHorizonalScroll( float fOffsetX );
	//clamp��ֱλ��
	void	ClampVerticalScroll( float fOffsetY );
public:
	int m_nValueStep;	// ��Ϊ�������ÿ�ƶ�һ�ξ����ĵ�λ����

	FontString*	m_pFontString;
	
	int		m_nVerticalScrollRange;		// ��ֱ���������룬������Ϊ��λ
	int		m_nVerticalHaveScrollDist;	// �Ѿ�����������ֱ���룬������Ϊ��λ

	int		m_nHorizonalScrollRange;		// ˮƽ���������룬������Ϊ��λ
	int		m_nHorizonalHaveScrollDist;		// �Ѿ���������ˮƽ���룬������Ϊ��λ

	int		m_nMaxVerticalScrollNum;	// ��ֱ��������������
	int		m_nOffsetY;					// �ۼ���ֱ�����ѹ������ܴ���

	int		m_nMaxHorizonalScrollNum;	// ˮƽ��������������
	int		m_nOffsetX;					// �ۼ�ˮƽ�����ѹ������ܴ���

	int		m_nLastVerticalScrollNum;
	int		m_nLastHorizonalScrollNum;
	
	int		m_nCurChangeX;
	int		m_nCurChangeY;
	
	// ʹ���߿��ܸı�ĳ���ӿؼ��Ĵ�С
	bool	m_bAdjust;			// �������ӿؼ�����㶼����Ϊ������ScrollFrame,���ǵ�ʹ����Ҫ����ʾ����ǿ�����

	int		m_nPreChildNum;		// ��һ���ӿؼ�����
	bool	m_bReCalAbs;

protected:
	void UpdateSelf(float deltatime);
};//tolua_export