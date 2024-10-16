
#pragma once
#include <vector>
#include "EffectUnit.h"

#include "OgreEntityMotionElement.h"

struct TANMHDR
{
	HWND hwndFrom;
	UINT idFrom;
	UINT code;
	int layer;
	int area;
	DWORD otherParam;
	bool *selLayers;
	int  maxLayer;
};


#define TAN_POS_CHANGE			1
#define TAN_SIZE_CHANGE			2
#define TAN_SELECT_CHANGE		3
#define TAN_LAYER_ACTIVE		4
#define TAN_ENDLINE_CHANGE		5
#define TAN_RED_BOX_CHANGE		6
#define TAN_KEY_DOWN			7
#define TAN_SPECAL_SELECT_SHIFT	8
#define TAN_SPECAL_SELECT_CTRL	9
#define TAN_SPECAL_SELECT_ALT	10
#define TAN_CTRL_DRAG			11
#define TAN_CLICK_OPTION		12
#define TAN_LAYER_POS			13
#define TAN_LAYER_RELATE		14
#define TAN_LOD_CHANGE			15
#define TAN_LOD_SHOWSTATE		16 //H、M、L

#define TAN_LAYERS_COPY_OP		17 //层复制
#define TAN_LAYERS_CUT_OP		18 //
#define TAN_LAYERS_PASTE_OP		19

class CRuler
{
};

class CTimeArea
{
public:
	CTimeArea() : m_iFirst(-1), m_iLen(0), m_bAllowPos(true), m_bAllowSize(true), m_bAllowDrag(false), m_bHightLight(false)
	{
		m_clrContain = RGB(60, 170, 60);
		m_clrFirst = RGB(90, 170, 90);
		m_clrEnd = RGB(90, 170, 90);
	};

	CTimeArea(const CTimeArea& tb) : m_iFirst(tb.m_iFirst), m_iLen(tb.m_iLen), m_bAllowPos(tb.m_bAllowPos), m_bAllowSize(tb.m_bAllowSize), m_bAllowDrag(tb.m_bAllowDrag)
	{
		m_clrContain = tb.m_clrContain;
		m_clrEnd = tb.m_clrEnd;
		m_clrFirst = tb.m_clrFirst;
		m_bHightLight = tb.m_bHightLight;
	};

	CTimeArea& operator = (const CTimeArea& tb)
	{
		m_iFirst = tb.m_iFirst;
		m_iLen = tb.m_iLen;
		m_bAllowSize = tb.m_bAllowSize;
		m_bAllowPos = tb.m_bAllowPos;
		m_clrContain = tb.m_clrContain;
		m_clrEnd = tb.m_clrEnd;
		m_clrFirst = tb.m_clrFirst;
		m_bAllowDrag = tb.m_bAllowDrag;
		m_bHightLight = tb.m_bHightLight;
		return *this;
	};

	int m_iFirst;
	int m_iLen;
	bool m_bAllowSize;
	bool m_bAllowPos;
	bool m_bAllowDrag;
	bool m_bHightLight;
	COLORREF	m_clrFirst;
	COLORREF	m_clrEnd;
	COLORREF	m_clrContain;
};

class CTimeLayer
{
public:
	CTimeLayer() : m_strName(""), m_usedOption(0), m_bReverseSearch(false), m_level(0),m_bChecked(false),m_iLodLevel(100)
	{
		CTimeArea tb;
		m_blocks.push_back(tb);
	};
	CTimeLayer(const CTimeLayer& src)
	{
		m_strName = src.m_strName;
		memcpy(m_options, src.m_options, sizeof(int) * 10);
		m_usedOption = src.m_usedOption;
		m_blocks = src.m_blocks;
		m_bReverseSearch = src.m_bReverseSearch;
		m_level = src.m_level;
		m_bChecked = src.m_bChecked;
		m_iLodLevel = src.m_iLodLevel;
		m_eObjType = src.m_eObjType;
		m_eET = src.m_eET;
	};
	CTimeLayer& operator = (const CTimeLayer& src)
	{
		m_strName = src.m_strName;
		memcpy(m_options, src.m_options, sizeof(int) * 10);
		m_usedOption = src.m_usedOption;
		m_blocks = src.m_blocks;
		m_bReverseSearch = src.m_bReverseSearch;
		m_level = src.m_level;
		m_bChecked = src.m_bChecked;
		m_iLodLevel = src.m_iLodLevel;
		m_eObjType = src.m_eObjType;
		m_eET = src.m_eET;
	};

	int GetEndBox(int sub = 0)		{ 	return GetFirst(sub) + GetLen(sub) - 1; };
	int GetFirst(int sub = 0)		{ 	return m_blocks[sub].m_iFirst;	};
	int GetLen(int sub = 0)			{	return m_blocks[sub].m_iLen;	};
	int GetSubItemCount()			{	return (int)m_blocks.size();	};
	bool IsAllowPos(int sub = 0)	{	return m_blocks[sub].m_bAllowPos;	};
	bool IsAllowSize(int sub = 0)	{	return m_blocks[sub].m_bAllowSize;	};
	bool IsAllowDrag(int sub = 0)	{	return m_blocks[sub].m_bAllowDrag;	};

	void SetFirst(int first, int sub = 0)		{	m_blocks[sub].m_iFirst = first;	};
	void SetLen(int len, int sub = 0)			{	m_blocks[sub].m_iLen = len;	};
	void AllowPos(bool bAllow, int sub = 0)		{	m_blocks[sub].m_bAllowPos = bAllow;	};
	void AllowSize(bool bAllow, int sub = 0)	{	m_blocks[sub].m_bAllowSize = bAllow;};

	void SetHightLight(bool bHightLight, int area)	{ m_blocks[area].m_bHightLight = true; };
	bool GetHightLight(int area)					{ return m_blocks[area].m_bHightLight; };

	void ClearHightLight()
	{
		for(int i = 0; i < (int)m_blocks.size(); i++)
		{
			m_blocks[i].m_bHightLight = false;
		}
	};

	int IsFirstBlock(int block)
	{
		if(m_bReverseSearch == false)
		{
			for(int i = 0; i < (int) m_blocks.size(); i++)
			{
				if(m_blocks[i].m_iFirst == block)
					return i;
			}
		}
		else
		{
			for(int i = (int) m_blocks.size() - 1; i >= 0 ; i--)
			{
				if(m_blocks[i].m_iFirst == block)
					return i;
			}
		}
		return  -1;
	};

	int IsEndBlock(int block)
	{
		if(m_bReverseSearch == false)
		{
			for(int i = 0; i < (int) m_blocks.size(); i++)
			{
				if(GetEndBox(i) == block)
					return i;
			}
		}
		else
		{
			for(int i = (int) m_blocks.size() - 1; i >= 0 ; i--)
			{
				if(GetEndBox(i) == block)
					return i;
			}
		}
		return  -1;
	};

	int IsContainBlock(int block)
	{
		if(m_bReverseSearch == false)
		{
			for(int i = 0; i < (int) m_blocks.size(); i++)
			{
				if(m_blocks[i].m_iFirst <= block && GetEndBox(i) >= block)
					return i;
			}
		}
		else
		{
			for(int i = (int) m_blocks.size() - 1; i >= 0 ; i--)
			{
				if(m_blocks[i].m_iFirst <= block && GetEndBox(i) >= block)
					return i;
			}
		}
		return  -1;
	};

	CString m_strName;
	int m_options[10];
	int m_usedOption;
	int m_level;
	bool m_bReverseSearch;
	bool m_bChecked;

	int m_iLodLevel; //对应对象层的lod
	EFFECT_TYPE m_eET;
	Ogre::MOTIONOBJTYPE_T m_eObjType;

	std::vector<CTimeArea> m_blocks;
};


enum HOVAREA
{
	HA_LIST,
	HA_GRAPHIC,
	HA_SPLITTER,
	HA_ENDLINE,
	HA_POINTER
};

#define HL_NONE		-2
#define HL_RULER	-1
// CTimeAixes


enum MOUSE_TYPE
{
	MT_ARROW,
	MT_SIZEALL,
	MT_SIZEWE
};

enum ASSIST_KEY_STATE
{
	NONE,
	CTRL,
	SHIFT,
	ALT
};

enum AIXES_LOD_SHOW_MODEL
{
	HIGH_STATE = 30000,
	MIDDLE_STATE,
	LOW_STATE
};

//对象层的快捷操作
enum OBJLAYERS_OP
{
	COPY,
	CUT,
	PASTE,
	NONE_LAYERS_SEL
};
struct OpData
{
	enum OBJLAYERS_OP stOpType;
	int stMotion;
	std::vector<int> vLayers;
	void Reset()
	{
		stOpType = NONE_LAYERS_SEL;
		stMotion = -1;
		vLayers.clear();
	}
	OpData():stOpType(NONE_LAYERS_SEL){}
};

const int MAX_LAYER_NUM = 64; //最大特下层数

class CTimeAixes : public CWnd
{
	DECLARE_DYNAMIC(CTimeAixes)

public:
	CTimeAixes();
	virtual ~CTimeAixes();

	float	m_fUnitValue;
	int 	m_iExactInStep;
	int 	m_iStepPixes;
	int		m_iLayerHeight;
	int		m_iHorPos;
	int		m_iListWidth;
	int		m_iVetPos;
	int		m_iSeperaterByBox;
	int		m_bShowGrid;

	std::vector<CTimeLayer*> m_layers;

	float	GetUnitValue()		{ return m_fUnitValue; };
	int		GetExactInStep()	{ return m_iExactInStep; };
	int		GetStepPixes()		{ return m_iStepPixes; };
	int		GetLayerHeight()	{ return m_iLayerHeight; };
	int		GetListWidth()		{ return m_iListWidth; };

	void	SetUnitValue(float fUnitValue);
	void	SetExactInStep(int iExact);
	void	SetStepPixes(int iStepPixes);
	void	SetLayerHeight(int iLayerHeight);
	void	SetListWidth(int iListWidth);

	int				AddLayer(CTimeLayer& layer);
	int				AddLayer(CString name, float startVal, float lenVal);
	bool			RemoveLayerByIndex(int index);
	bool			RemoveLayer(CTimeLayer* pLayer);
	void			ClearLayers();
	CTimeLayer*		GetLayer(int index);
	int				GetLayerCount();

	float			GetBoxValue(int box);
	float			GetLayerStartValue(int layer, int sub = 0);
	float			GetLayerLenValue(int layer, int sub = 0);
	int				GetBoxIndex(float value);
	int				GetLayerOption(int layer, int option);

	void			SetLayerOption(int layer, int option, int value);
	void			SetLayerStartValue(int layer, float val, int sub = 0);
	void			SetLayerLenValue(int layer, float val, int sub = 0);


	void	PaintList(CRect& size, CDC* pDC);
	void	PaintGraphic(CRect& size, CDC* pDC);
	void	PaintLayerGraphic(int layer, CRect containRect, CDC* pDC);

	CRect	GetLayerListRect(int layer);
	CRect	GetLayerGraphicRect(int layer);
	CRect	GetLayerRangeRect(int layer);

	CRect	GetLayerOptionRect(int layer, int option);

	CRect	GetEndLineRect();
	CRect	GetPointerRect();
	int		GetBoxByPoint(POINT* ppt);


	BOOL	Create(DWORD style, CRect rect, CWnd* pParent, UINT nID);
	void	OnClickLayerList(int layer);
	void	Notify(UINT code, int layer, int area = 0, DWORD param = 0);
	void	NotifyMultiSel(UINT code, int layer, int area = 0, DWORD param = 0);
	//void	PostNotify(UINT code, int layer, int area = 0, DWORD param = 0);
	void	UpdateHovingState(POINT pt);
	void	ShiftRedBox(int iRedBox);

	void	FillSpecialRect(CDC* pDC, CRect rect, COLORREF color);

	//Drags
	bool	DoSplitter();
	bool	DoRulerDrag();
	bool	DoVDrag();
	bool	DoLayerBoxSize(int area = 0);
	bool	DoLayerBoxPos(int area = 0);
	bool	DoEndLine();
	bool	DoPointerDrag();
	bool	DoCtrlDrag(int iArea);
	bool	DoLayerPos(int iLayer);
	bool	DoLayerRelate(int iLayer);
	bool	DoLayerScale(int iLayer);

	//Hover State
	HOVAREA		m_hoverArea;
	int			m_iHoverLayer;
	bool		m_iIsHoverOnSel;
	int			m_iHoverBox;

	//Select State
	int			m_iButtonDownLayer;
	bool		m_bCanRelate;
	bool		m_bCanCgePos;
	bool		m_bLButtonDown;
	ASSIST_KEY_STATE m_eLastKeyState;
	bool		m_bSelectLayer[MAX_LAYER_NUM]; //多选时的特效层

	int			m_iSelectedLayer;
	int			m_iSelectedBox;
	int			m_iEndLine;
	float		m_fPlayLine;

	//Special Drag
	int			m_iDragHoverBox;
	int			m_iLayerInsertAfter;
	int			m_iLayerRelateTo;

	//Mouse Arror
	MOUSE_TYPE		m_CurrMouse;

	void		SetMouseArrow(MOUSE_TYPE mouse);
	HCURSOR		m_hMouseArrow;
	HCURSOR		m_hMouseSizeWE;
	HCURSOR		m_hMouseSizeAll;

	CBCGPToolBar m_ToolBar; //lod按钮
	AIXES_LOD_SHOW_MODEL m_eSelLodState;

	//此处的20临时，为MAX_LAYER_NUM值
	bool*			GetMultiSelLayer(int* num) { num[0] = MAX_LAYER_NUM; return &m_bSelectLayer[0];};
	int				GetSelectedLayer()	{ return m_iSelectedLayer;	};
	void			SetSelectedLayer(int layer);
	int				GetSelectedBox()	{ return m_iSelectedBox;	};
	void			SetSelectedBox(int box);
	int				GetEndPos()	{ return m_iEndLine ; };
	float			GetEndlineValue();
	void			SetEndlineByValue(float val);
	float			GetPlayLine();
	void			SetPlayLine(float val);
protected:
	DECLARE_MESSAGE_MAP()
public:

	//void			MouseAction(int button, UINT nFlags, CPoint point);

	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//设置对象层的 LOD 水平
	afx_msg void OnSetHighLod();
	afx_msg void OnSetMiddleLod();
	afx_msg void OnSetLowLod();
	afx_msg void OnExitLodPopMenu();
	afx_msg void OnNoHandleLod();

	afx_msg void OnCopyLayers();
	afx_msg void OnCutLaysers();
	afx_msg void OnPasteLayers();

	afx_msg void OnSelAllKeys();


	//工具栏
	afx_msg void OnShowHighLodLayers();
	afx_msg void OnUpdateShowHighLodLayers(CCmdUI* pCmdUI);
	afx_msg void OnShowMiddleLodLayers();
	afx_msg void OnUpdateShowMiddleLodLayers(CCmdUI* pCmdUI);
	afx_msg void OnShowLowLodLayers();
	afx_msg void OnUpdateShowLowLodLayers(CCmdUI* pCmdUI);
};