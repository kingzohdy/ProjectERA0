
#include "stdafx.h"
#include "TimeBar.h"
#include "Resource.h"
#include "EffectManager.h"
#include "EffectLayerInfo.h"
#include "MainFrm.h"
#include "ObjectEditor.h"
#include "ModelMotionInfo.h"

// CTimeAixes
OpData gLayersOpObj;

IMPLEMENT_DYNAMIC(CTimeAixes, CWnd)
CTimeAixes::CTimeAixes()
{
	m_fUnitValue = 0.04f;
	m_iExactInStep = 25;
	m_iStepPixes = 200;
	m_iLayerHeight = 30;
	m_iHorPos = 0;
	m_iVetPos = 0;
	m_iListWidth = 100;

	for(int i=0; i<MAX_LAYER_NUM; i++)
		m_bSelectLayer[i] = false;
	m_eLastKeyState = NONE;
	m_bLButtonDown = false;
	m_bCanRelate = false;
	m_bCanCgePos = false;
	m_iSelectedLayer = -1;
	m_iEndLine = 10;
	m_fPlayLine = -1;
	m_iSelectedBox = -1;

	m_iDragHoverBox = -1;

	m_iSeperaterByBox = 10;
	m_bShowGrid = true;
	m_iLayerInsertAfter = -1;
	m_CurrMouse = MT_ARROW;
	m_iLayerRelateTo = -1;

	m_hMouseArrow = LoadCursor(NULL, IDC_ARROW);
	m_hMouseSizeAll = LoadCursor(NULL, IDC_SIZEALL);
	m_hMouseSizeWE = LoadCursor(NULL, IDC_SIZEWE);

	m_eSelLodState = HIGH_STATE;
}

CTimeAixes::~CTimeAixes()
{
}


BEGIN_MESSAGE_MAP(CTimeAixes, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()

	ON_COMMAND(ID_SET_HIGHLOD, OnSetHighLod)
	ON_COMMAND(ID_SET_MIDDLELOD, OnSetMiddleLod)
	ON_COMMAND(ID_SET_LOWLOD, OnSetLowLod)
	ON_COMMAND(ID_EXIT_LOD, OnExitLodPopMenu)
	ON_COMMAND(ID_SET_NOHANDLE, OnNoHandleLod)

	ON_COMMAND(ID_COPY_LAYER, OnCopyLayers)
	ON_COMMAND(ID_CUT_LAYER, OnCutLaysers)
	ON_COMMAND(ID_PASTE_LAYER, OnPasteLayers)

	ON_COMMAND(ID_SEL_ALLKEYS, OnSelAllKeys)

	ON_COMMAND(ID_TB_LAYER_HLOD, OnShowHighLodLayers)
	ON_UPDATE_COMMAND_UI(ID_TB_LAYER_HLOD, OnUpdateShowHighLodLayers)
	ON_COMMAND(ID_TB_LAYER_MLOD, OnShowMiddleLodLayers)
	ON_UPDATE_COMMAND_UI(ID_TB_LAYER_MLOD, OnUpdateShowMiddleLodLayers)
	ON_COMMAND(ID_TB_LAYER_LLOD, OnShowLowLodLayers)
	ON_UPDATE_COMMAND_UI(ID_TB_LAYER_LLOD, OnUpdateShowLowLodLayers)
END_MESSAGE_MAP()

void CTimeAixes::SetUnitValue(float fUnitValue)
{
	m_fUnitValue = fUnitValue;
	Invalidate(FALSE);
}

void CTimeAixes::SetExactInStep(int iExact)
{
	m_iExactInStep = iExact;
	Invalidate(FALSE);
}

void CTimeAixes::SetStepPixes(int iStepPixes)
{
	m_iStepPixes = iStepPixes;
	Invalidate(FALSE);
}

void CTimeAixes::SetLayerHeight(int iLayerHeight)
{
	m_iLayerHeight = iLayerHeight;
	Invalidate(FALSE);
}

void CTimeAixes::SetListWidth(int iListWidth)
{
	m_iListWidth = iListWidth;
	Invalidate(FALSE);
}

int CTimeAixes::AddLayer(CTimeLayer& layer)
{
	if(m_layers.size() >= MAX_LAYER_NUM)
	{
		AfxMessageBox("层数达到最大值(64)， 不能再增加层了");
		return 0;
	}

	CTimeLayer* ptl = new CTimeLayer(layer);
	m_layers.push_back(ptl);

	return (int)m_layers.size() - 1;
}

int CTimeAixes::AddLayer(CString name, float startVal, float lenVal)
{
	if(m_layers.size() >= MAX_LAYER_NUM)
	{
		AfxMessageBox("层数达到最大值(64)， 不能再增加层了");
		return 0;
	}

	CTimeLayer* ptl = new CTimeLayer();
	ptl->m_strName = name;

	float tmp = startVal / m_fUnitValue + 0.5f;
	ptl->SetFirst((int)tmp);

	tmp = lenVal / m_fUnitValue + 0.5f;
	ptl->SetLen( (int)tmp);

	m_layers.push_back(ptl);
	Invalidate(FALSE);
	return (int)m_layers.size() - 1;
}

bool CTimeAixes::RemoveLayerByIndex(int index)
{
	if(index >=0 && index < (int)m_layers.size())
	{
		m_layers.erase(m_layers.begin() + index);
		return true;
	}

	return false;
}

bool CTimeAixes::RemoveLayer(CTimeLayer* pLayer)
{
	for(int i = 0; i < (int) m_layers.size(); i++)
	{
		if(m_layers[i] == pLayer)
		{
			m_layers.erase(m_layers.begin() + i);
			return true;
		}
	}

	return false;
}

void CTimeAixes::ClearLayers()
{
	for(int i = 0; i < (int)m_layers.size(); i++)
		delete m_layers[i];
	for(int j = 0; j<MAX_LAYER_NUM; j++)
		m_bSelectLayer[j] = false;
	m_eLastKeyState = NONE;
	m_layers.clear();
	m_bLButtonDown = false;
	m_bCanRelate = false;
	m_bCanCgePos = false;
	m_iSelectedLayer = -1;
	m_iHoverLayer = HL_NONE;
	m_iIsHoverOnSel = false;
	m_iHoverBox = -1;
	//m_iHorPos = 0;
	//m_iVetPos = 0;
	Invalidate(FALSE);
}

CTimeLayer* CTimeAixes::GetLayer(int index)
{
	if(index >=0 && index < (int)m_layers.size())
	{
		return m_layers[index];
	}
	return NULL;;
}

int CTimeAixes::GetLayerCount()
{
	return (int)m_layers.size();
}

CRect CTimeAixes::GetLayerListRect(int layer)
{
	CRect rect;
	GetClientRect(rect);
	CRect listRect(0, 0, m_iListWidth - 2, rect.bottom);

	CRect layerRect( listRect.left
		, (layer + 1) * m_iLayerHeight + listRect.top + m_iVetPos
		, listRect.right
		, (layer + 2) * m_iLayerHeight + listRect.top + m_iVetPos);
	return layerRect;
}

float CTimeAixes::GetBoxValue(int box)
{
	return box * m_fUnitValue;
}

int CTimeAixes::GetBoxIndex(float value)
{
	int ret = int((value / m_fUnitValue) + 0.5f);
	return ret;
}

int CTimeAixes::GetLayerOption(int layer, int option)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		CTimeLayer* pLayer = m_layers[layer];
		if(option >= 0 && option < pLayer->m_usedOption)
		{
			return pLayer->m_options[option];
		}
	}
	return -1;
}
void CTimeAixes::SetLayerOption(int layer, int option, int value)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		CTimeLayer* pLayer = m_layers[layer];
		if(option >= 0 && option < pLayer->m_usedOption)
		{
			pLayer->m_options[option] = value;
		}
	}


}

float CTimeAixes::GetLayerStartValue(int layer, int sub)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		return m_layers[layer]->GetFirst(sub) * m_fUnitValue;
	}
	return 0.0f;
}

float CTimeAixes::GetLayerLenValue(int layer, int sub)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		return m_layers[layer]->GetLen(sub) * m_fUnitValue;
	}
	return 0.0f;
}

void CTimeAixes::SetLayerStartValue(int layer, float val, int sub)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		float tmp = val / m_fUnitValue + 0.5f;
		m_layers[layer]->SetFirst((int)tmp, sub);
		Invalidate(FALSE);
	}
}

void CTimeAixes::SetLayerLenValue(int layer, float val, int sub)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		float tmp = val / m_fUnitValue + 0.5f;
		m_layers[layer]->SetLen((int)tmp, sub);
		Invalidate(FALSE);
	}
}

float CTimeAixes::GetEndlineValue()
{
	return m_fUnitValue * m_iEndLine;
}

void CTimeAixes::SetEndlineByValue(float val)
{
	float tmp = val / m_fUnitValue + 0.5f;
	m_iEndLine = (int)tmp;
}

float CTimeAixes::GetPlayLine()
{
	return m_fUnitValue * m_fPlayLine;
}

void CTimeAixes::SetPlayLine(float val)
{
	m_fPlayLine = (val / m_fUnitValue);
}

void CTimeAixes::SetSelectedLayer(int layer)
{
	if(layer >= 0 && layer < (int)m_layers.size())
	{
		m_iSelectedLayer = layer;
	}
	else
	{
		m_iSelectedLayer = -1;
	}

	if(GetSafeHwnd())
		Invalidate(FALSE);
}

void CTimeAixes::SetSelectedBox(int box)
{
	if(box >= 0)
	{
		m_iSelectedBox = box;
	}
	else
	{
		m_iSelectedBox = -1;
	}

	if(GetSafeHwnd())
		Invalidate(FALSE);
}

CRect CTimeAixes::GetLayerGraphicRect(int layer)
{
	CRect rect;
	GetClientRect(rect);
	CRect graphicRect(m_iListWidth + 2, 0, rect.right, rect.bottom);

	CRect layerRect( graphicRect.left
		, (layer + 1) * m_iLayerHeight + graphicRect.top + m_iVetPos
		, graphicRect.right
		, (layer + 2) * m_iLayerHeight + graphicRect.top + m_iVetPos);

	return layerRect;
}

CRect CTimeAixes::GetLayerRangeRect(int layer)
{
	CRect ret = GetLayerGraphicRect(layer);

	int startpos = m_layers[layer]->GetFirst() * m_iStepPixes / m_iExactInStep;
	int endpos = (m_layers[layer]->GetFirst() + m_layers[layer]->GetLen()) * m_iStepPixes / m_iExactInStep;

	ret.left = startpos + m_iHorPos + m_iListWidth + 1;
	ret.right = endpos + m_iHorPos + m_iListWidth + 1;

	return ret;
}

CRect CTimeAixes::GetLayerOptionRect(int layer, int option)
{
	int width = m_iLayerHeight;
	CRect ret;
	ret.right = m_iListWidth - 2 - m_iLayerHeight * option;
	ret.left = ret.right - m_iLayerHeight;
	ret.top = (layer + 1) * m_iLayerHeight + m_iVetPos;
	ret.bottom = ret.top + m_iLayerHeight;

	return ret;
}

CRect CTimeAixes::GetEndLineRect()
{
	int pos = m_iListWidth + 2 + m_iEndLine * (m_iStepPixes / m_iExactInStep) + m_iHorPos;
	return CRect(pos - 2, 0, pos + 2 , m_iLayerHeight);
}

CRect CTimeAixes::GetPointerRect()
{
	int pos = m_iListWidth + 2 + m_iSelectedBox * (m_iStepPixes / m_iExactInStep) + m_iHorPos;
	int boxwidth = (m_iStepPixes / m_iExactInStep);
	return CRect(pos + 1,  0, pos + boxwidth, m_iLayerHeight - 1 );
}

int CTimeAixes::GetBoxByPoint(POINT* ppt)
{

	CRect graRect = GetLayerGraphicRect(m_iHoverLayer);
	int pos = ppt->x - graRect.left - m_iHorPos;
	return pos / (m_iStepPixes / m_iExactInStep);

	//return -1;
}




BOOL CTimeAixes::Create(DWORD style, CRect rect, CWnd* pParent, UINT nID)
{
	if( CWnd::Create(AfxRegisterWndClass(CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW), _T(""), style | WS_CHILD, rect, pParent, nID, NULL))
	{
		ShowWindow(SW_SHOW);

		static bool isFirst = true;
		if(isFirst)
		{
			m_ToolBar.Create(this);
			m_ToolBar.LoadToolBar(IDR_TOOLBAR_LAYERLOD);
			m_ToolBar.SetRouteCommandsViaFrame(FALSE);
			m_ToolBar.SetWindowPos(NULL, 0, 0, m_iListWidth-2, 20, SWP_NOZORDER);
			isFirst = false;
		}

		return TRUE;
	}
	return FALSE;
}

void CTimeAixes::OnClickLayerList(int layer)
{
	int oldLayer = m_iSelectedLayer;
	m_iSelectedLayer = layer;
	Invalidate(FALSE);
	if(oldLayer != layer)//if(oldLayer != layer && layer >= 0)
	{
		Notify(TAN_SELECT_CHANGE, layer);
	}
}

void CTimeAixes::Notify(UINT code, int layer, int area, DWORD param)
{
	TANMHDR* phdr = new TANMHDR();
	phdr->code = code;
	phdr->hwndFrom = GetSafeHwnd();
	phdr->idFrom = GetDlgCtrlID();
	phdr->layer = layer;
	phdr->area = area;
	phdr->otherParam = param;
	phdr->selLayers = NULL;
	phdr->maxLayer = 0;
	ReleaseCapture();
	//if(code == TAN_RED_BOX_CHANGE)
	//{
	//	::PostMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, (WPARAM)phdr->idFrom, (LPARAM)(NMHDR*)phdr);
	//}
	//else
	//{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, (WPARAM)phdr->idFrom, (LPARAM)(NMHDR*)phdr);
	delete phdr;

	//}
}

void CTimeAixes::NotifyMultiSel(UINT code, int layer, int area, DWORD param)
{
	TANMHDR* phdr = new TANMHDR();
	phdr->code = code;
	phdr->hwndFrom = GetSafeHwnd();
	phdr->idFrom = GetDlgCtrlID();
	phdr->layer = layer;
	phdr->area = area;
	phdr->otherParam = param;
	phdr->selLayers = m_bSelectLayer; //多选的指针
	phdr->maxLayer = MAX_LAYER_NUM;
	ReleaseCapture();

	::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, (WPARAM)phdr->idFrom, (LPARAM)(NMHDR*)phdr);
	delete phdr;
}

void CTimeAixes::UpdateHovingState(POINT pt)
{
	if(pt.x < m_iListWidth - 2)
		m_hoverArea = HA_LIST;
	else if(pt.x >= m_iListWidth - 2 && pt.x < m_iListWidth + 2)
		m_hoverArea = HA_SPLITTER;
	else if(pt.x >= m_iListWidth + 2)
	{
		CRect endline = GetEndLineRect();
		CRect pointerRc = GetPointerRect();
		if(endline.PtInRect(pt))
			m_hoverArea = HA_ENDLINE;
		else if(pointerRc.PtInRect(pt))
			m_hoverArea = HA_POINTER;
		else
			m_hoverArea = HA_GRAPHIC;
	}


	int lv;
	if(pt.y / m_iLayerHeight == 0)
	{
		lv = HL_RULER;
	}
	else
	{
		lv = (pt.y - m_iVetPos) / m_iLayerHeight - 1;
		if(lv >= (int)m_layers.size() )
			lv = HL_NONE;
	}
	m_iHoverLayer = lv;

	if(m_hoverArea == HA_GRAPHIC)
		m_iHoverBox = GetBoxByPoint(&pt);
	else
		m_iHoverBox = -1;
}


void CTimeAixes::ShiftRedBox(int iRedBox)
{
	m_iSelectedBox = iRedBox;
	Notify(TAN_RED_BOX_CHANGE, m_iHoverLayer);
	Invalidate(FALSE);
}

// CTimeAixes message handlers

void CTimeAixes::FillSpecialRect(CDC* pDC, CRect rect, COLORREF color)
{
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	int rc, bc, gc;
	for(int y = 0; y < rect.Height(); y++)
	{
		float val = 1.0f;
		if(y > rect.Height() / 2)
		{
			val = y / (rect.Height() / 2.0f);
		}
		else
		{
			val = 1 - y / (rect.Height() / 2.0f);
		}

		rc = (int)(val * r + (1-val) * 255);
		gc = (int)(val * g + (1-val) * 255);
		bc = (int)(val * b + (1-val) * 255);

		COLORREF clr;
		clr = RGB(rc, gc,bc);

		CPen pen(PS_SOLID, 1, clr);
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(rect.left, rect.top + y);
		pDC->LineTo(rect.right, rect.top + y);

		pDC->SelectObject(pOldPen);

	}
}

void CTimeAixes::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(rect);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

	CBrush br(RGB(210, 210, 210));
	CBrush brSp(RGB(170, 170, 255));
	CBrush brWhite(RGB(255, 255, 255));
	CBrush brBlue(RGB(255, 170, 255));

	dcMemory.FillRect(rect, &brWhite);


	CRect listRect(0, m_iVetPos, m_iListWidth - 2, rect.bottom);
	CRect graphicRect(m_iListWidth + 2, m_iVetPos, rect.right, rect.bottom);
	CRect splitterRect(m_iListWidth - 2, m_iVetPos, m_iListWidth + 2, rect.bottom);
	CRect graphicValidRect(m_iListWidth + 2, m_iVetPos, m_iListWidth + 2 + m_iEndLine * (m_iStepPixes / m_iExactInStep) + m_iHorPos, m_iLayerHeight * (1 + m_layers.size()) + m_iVetPos);
	CRect pointerRect = GetPointerRect();

	dcMemory.FillRect(graphicRect, &br);
	dcMemory.FillRect(graphicValidRect, &brWhite);

	PaintGraphic(graphicRect, &dcMemory);
	if(pointerRect.left > splitterRect.right)
		dcMemory.FillRect(pointerRect, &brBlue);
	PaintList(listRect, &dcMemory);
	dcMemory.FillRect(splitterRect, &brSp);


	dc.BitBlt(0,0 , rect.Width(),rect.Height(),  &dcMemory, 
		0, 0, SRCCOPY);
	dcMemory.SelectObject(pOldBitmap);
}

void CTimeAixes::PaintList(CRect& size, CDC* pDC)
{
	CFont font;
	font.CreatePointFont(90, _T("宋体"));
	CFont* poldfont = (CFont*)pDC->SelectObject(&font);
	COLORREF oldclr = pDC->GetTextColor();

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));
	CBrush br(RGB(120, 120, 120));
	//	CBrush brSel(RGB(200, 200, 200));
	CBrush brSel(RGB(163, 158, 86));
	//	CBrush brRelate(RGB(80, 180, 80));
	CBrush brRelate(RGB(200, 0, 0));
	CBrush brAnim(RGB(170, 170, 170));
	CBrush brPost(RGB(220, 220, 220));
	int nCount = (int)m_layers.size();
	for(int i = 0; i < nCount; i++)
	{
		int iShiftUnitSize = m_iLayerHeight;
		CRect layerRect( size.left
			, (i + 1) * m_iLayerHeight + size.top
			, size.right
			, (i + 2) * m_iLayerHeight + size.top);

		CBrush* pBrBack = NULL;
		if(m_bSelectLayer[i])
		{
			pBrBack = &brSel;
		}
		else if(m_layers[i]->m_eObjType == Ogre::MOT_ANIM || m_layers[i]->m_eObjType == Ogre::MOT_ANIM_ADD)
		{
			pBrBack = &brAnim;
		}
		else if(m_layers[i]->m_eObjType == Ogre::MOT_POSTEFFECT)
		{
			pBrBack = &brPost;
		}
		else
			pBrBack = &br;

		if(m_iLayerRelateTo == i)
			pBrBack = &brRelate;

		pDC->FillRect(layerRect, pBrBack);

		//画插入标记
		if(m_iLayerInsertAfter == i)
		{
			CRect insertLineRect(layerRect.left, layerRect.bottom - 2, layerRect.right, layerRect.bottom);
			CBrush br(RGB(255, 0, 0));
			pDC->FillRect(insertLineRect, &br);
		}

		//画层选项
		for(int j = 0; j < (int) m_layers[i]->m_usedOption; j++)
		{
			CRect optionRect = GetLayerOptionRect(i, j);
			optionRect.left += 4;
			optionRect.right -= 3;
			optionRect.top += 4;
			optionRect.bottom -= 3;

			if(m_layers[i]->m_eET == EFFECT_VIRTUALNODE && j == 1)
			{
				POINT left, right, up, down;
				left.x = optionRect.left + 2;
				left.y = optionRect.top + optionRect.Height()/2;
				right.x = optionRect.right - 2;
				right.y = left.y;
				up.x = optionRect.left + optionRect.Width()/2;
				up.y = optionRect.top + 2;
				down.x = up.x;
				down.y = optionRect.bottom - 2;

				CBrush objBrush(RGB(255, 255, 255));
				CBrush* pOldBrush = pDC->SelectObject(&objBrush);
				pDC->Rectangle(optionRect);
				pDC->SelectObject(pOldBrush);

				CPen objPen(PS_SOLID, 1, RGB(0,0,0));
				CPen* pOldPenT = pDC->SelectObject(&objPen);
				if(m_layers[i]->m_options[j] == 0) //未折叠
				{
					pDC->MoveTo(left);
					pDC->LineTo(right);
				}
				else if(m_layers[i]->m_options[j] == 1)
				{
					pDC->MoveTo(left);
					pDC->LineTo(right);
					pDC->MoveTo(up);
					pDC->LineTo(down);
				}
				pDC->SelectObject(pOldPenT);
			}
			else
			{
				if(m_layers[i]->m_options[j] != 0)
				{
					CBrush brRed(RGB(60, 60, 255));
					pDC->FillRect(optionRect, &brRed);
				}
				else
				{
					CPen pen(PS_SOLID, 2, RGB(60, 60, 255));
					CPen* pOldPen = pDC->SelectObject(&pen);
					CBrush* pOldBr = pDC->SelectObject(pBrBack);

					pDC->Rectangle(optionRect);
					pDC->SelectObject(pOldPen);
					pDC->SelectObject(pOldBr);
				}
			}			
		}

		//写层标题文字
		CRect layerTextRect = layerRect;
		layerTextRect.left += m_layers[i]->m_level * iShiftUnitSize;
		CRect lodTextRect(layerTextRect.left+2, layerTextRect.top+4, layerTextRect.left+16, layerTextRect.top+16);
		layerTextRect.left = lodTextRect.right;

		CString str;
		int nLodLevel = m_layers[i]->m_iLodLevel;
		switch (nLodLevel)
		{
		case 10000:	str = "N";		break;
		case 100:	str = "L";		break;
		case 200:	str = "M";		break;
		case 300:	str = "H";		break;
		default:	::MessageBox(NULL, "CTimeAixes::PaintList LOD Wrong !", "警告", MB_OK);	break;
		}
		CString strName = m_layers[i]->m_strName;
		pDC->DrawTextEx(m_layers[i]->m_strName , layerTextRect, DT_WORD_ELLIPSIS | DT_LEFT |DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, NULL);

		COLORREF oriColor = pDC->SetTextColor(RGB(255, 0, 0));
		pDC->DrawTextEx(str, lodTextRect, DT_WORD_ELLIPSIS | DT_LEFT |DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, NULL);

		//绘制间隔线
		if(i < (nCount-1))
		{
			CRect oFlag(lodTextRect.left, layerTextRect.bottom-1, layerTextRect.right, layerTextRect.bottom+1);
			if(m_layers[i]->m_eObjType != m_layers[i+1]->m_eObjType)
			{
				CBrush brFlag(RGB(60, 60, 0));
				pDC->FillRect(oFlag, &brFlag);
			}
		}
		pDC->SetTextColor(oriColor);
	}
	CBrush brWhite(RGB(255, 255, 255));
	CRect topWhite(size.left, 0, size.right, m_iLayerHeight);
	pDC->FillRect(topWhite, &brWhite);

	pDC->SelectObject(poldfont);
}

void CTimeAixes::PaintGraphic(CRect& size, CDC* pDC)
{
	CFont font;
	font.CreatePointFont(90, _T("宋体"));
	CFont* poldfont = (CFont*)pDC->SelectObject(&font);
	COLORREF oldclr = pDC->GetTextColor();

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));

	int currStep = (-m_iHorPos) / m_iStepPixes;
	int lastPos = 0;
	int boxcount = 0;

	for(int i = 0; i < (int)m_layers.size(); i++)
	{
		PaintLayerGraphic(i, size,pDC);
	}

	CPen penBlack(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOld = pDC->SelectObject(&penBlack);
	while(currStep * m_iStepPixes < size.Width() + m_iStepPixes - m_iHorPos)
	{
		// draw ruler
		CRect rulerRect(size.left, 0, size.right, m_iLayerHeight);
		CBrush br(RGB( 255, 255, 255));
		//pDC->FillRect(rulerRect, &br);
		if(currStep != 0)
		{
			for(int i = 1; i < m_iExactInStep; i++)
			{
				int pos = lastPos + m_iStepPixes * i / m_iExactInStep;
				if(pos >= size.left && pos <= size.right)
				{
					pDC->MoveTo(pos, m_iLayerHeight * 2 / 3 + size.top - m_iVetPos);
					pDC->LineTo(pos, m_iLayerHeight + size.top - m_iVetPos);

					if((currStep * m_iExactInStep + i) % m_iSeperaterByBox == 0)
					{
						pDC->MoveTo(pos, m_iLayerHeight * 1 / 2 + size.top - m_iVetPos);
						pDC->LineTo(pos, m_iLayerHeight + size.top - m_iVetPos);
					}
				} 
			}
		}
		int stepPos = currStep * m_iStepPixes + m_iHorPos + size.left;
		if(stepPos >= size.left && stepPos <= size.right)
		{
			pDC->MoveTo(stepPos, m_iLayerHeight * 1 / 2 + size.top - m_iVetPos);
			pDC->LineTo(stepPos, m_iLayerHeight + size.top - m_iVetPos);

			CString str;
			str.Format("%g", currStep * m_fUnitValue * m_iExactInStep);
			CRect textRect(stepPos - 10,  0, stepPos + 10, m_iLayerHeight / 2 + size.top - m_iVetPos);
			pDC->DrawTextEx(str, textRect, DT_WORD_ELLIPSIS | DT_CENTER |DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, NULL);
		}
		lastPos = stepPos;

		currStep++;
	}
	pDC->SelectObject(pOld);


	CPen redPen(PS_SOLID, 3, RGB(255, 130, 130));
	CPen* poldpen = pDC->SelectObject(&redPen);
	int endpos = size.left + m_iEndLine * (m_iStepPixes / m_iExactInStep) + m_iHorPos;
	if(endpos > size.left)
	{
		pDC->MoveTo(endpos, 0);
		pDC->LineTo(endpos, m_iLayerHeight);
	}

	CPen bluePen(PS_SOLID, 1, RGB(150, 150, 255));
	if(m_fPlayLine >= 0.0f)
	{
		pDC->SelectObject(&bluePen);
		int playlinepos = size.left + (int)(m_fPlayLine * (m_iStepPixes / m_iExactInStep)) + m_iHorPos;
		if(playlinepos > size.left)
		{
			pDC->MoveTo(playlinepos, 0);
			pDC->LineTo(playlinepos, m_iLayerHeight * (m_layers.size() + 1));
		}
	}
	pDC->SelectObject(poldpen);
	pDC->SelectObject(poldfont);
}

void CTimeAixes::PaintLayerGraphic(int layer, CRect containRect, CDC* pDC)
{
	CPen grayPen(PS_SOLID, 1, RGB(170, 170, 170));
	CPen* pOldPen = pDC->SelectObject(&grayPen);
	//CBrush selBr(RGB(150, 255, 150));
	//CBrush selBrFirst(RGB(190, 255, 150));
	//CBrush selBrLast(RGB(170, 225, 170));

	CRect selBoxRc;
	bool bHasSelectBox = false;
	std::vector<CRect> hightLightRc;

	int currStep = (-m_iHorPos) / m_iStepPixes;
	int lastPos = 0;
	int top = (layer + 1) * m_iLayerHeight + containRect.top;
	int bottom = (layer + 2) * m_iLayerHeight + containRect.top;

	if(top < m_iLayerHeight)
	{
		top = m_iLayerHeight;
	}
	if(bottom < m_iLayerHeight)
	{
		return;
	}
	pDC->MoveTo(containRect.left, top);
	pDC->LineTo(containRect.right, top);
	pDC->MoveTo(containRect.left, bottom);
	pDC->LineTo(containRect.right, bottom);

	while(currStep * m_iStepPixes < containRect.Width() + m_iStepPixes - m_iHorPos)
	{

		int stepPos = currStep * m_iStepPixes + m_iHorPos + containRect.left;
		lastPos = stepPos;
		for(int i = 0; i < m_iExactInStep; i++)
		{
			int pos = lastPos + m_iStepPixes * i / m_iExactInStep;
			int currBlock = currStep * m_iExactInStep + i;
			CRect rc(pos, top + 1, pos +  m_iStepPixes / m_iExactInStep , bottom);

			int areaEnd = m_layers[layer]->IsEndBlock(currBlock);
			int areaFirst = m_layers[layer]->IsFirstBlock(currBlock);
			int areaContain = m_layers[layer]->IsContainBlock(currBlock);

			if(areaEnd >= 0 && m_layers[layer]->IsAllowSize(areaEnd) == true) 
			{
				CBrush selBrLast(m_layers[layer]->m_blocks[areaEnd].m_clrEnd);
				pDC->FillRect(rc, &selBrLast);
				//FillSpecialRect(pDC, rc, m_layers[layer]->m_blocks[areaEnd].m_clrEnd);
			}
			else if(areaFirst >= 0 && m_layers[layer]->IsAllowPos(areaFirst) == true) 
			{
				CBrush selBrFirst(m_layers[layer]->m_blocks[areaFirst].m_clrFirst);
				pDC->FillRect(rc, &selBrFirst);
				//FillSpecialRect(pDC, rc, m_layers[layer]->m_blocks[areaEnd].m_clrFirst);
			}
			else if (areaContain >= 0)
			{	
				CBrush selBr(m_layers[layer]->m_blocks[areaContain].m_clrContain);
				pDC->FillRect(rc, &selBr);
				//FillSpecialRect(pDC, rc, m_layers[layer]->m_blocks[areaEnd].m_clrContain);
			}

			if(layer == m_iHoverLayer && m_iDragHoverBox >= 0 && m_iDragHoverBox == currBlock)
			{
				CBrush dragBr(RGB(160, 160, 255));
				pDC->FillRect(rc, &dragBr);
			}

			if(pos >= containRect.left && pos <= containRect.right)
			{
				if(currBlock % m_iSeperaterByBox == 0)
				{
					CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
					CPen* pOldPen = pDC->SelectObject(&pen);
					pDC->MoveTo(pos, top);
					pDC->LineTo(pos, bottom);
					pDC->SelectObject(pOldPen);
				}
				else if(!m_bShowGrid)
				{

					CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
					CPen* pOldPen = pDC->SelectObject(&pen);
					pDC->MoveTo(pos, top);
					pDC->LineTo(pos, bottom);
					pDC->SelectObject(pOldPen);
				}
				else
				{
					pDC->MoveTo(pos, top);
					pDC->LineTo(pos, bottom);
				}
			}

			if(layer == m_iSelectedLayer && m_iSelectedBox == currBlock)
			{
				bHasSelectBox = true;
				selBoxRc = rc;
			}
			if(areaFirst >= 0 && m_layers[layer]->GetHightLight(areaFirst) == true)
			{
				hightLightRc.push_back(rc);
			}

		}
		currStep++;
	}

	for(int i = 0; i < (int)hightLightRc.size(); i++)
	{
		CRect curRc = hightLightRc[i];

		CPen pen(PS_SOLID, 3, RGB(130, 130, 255));
		CPen* pOldPen1 = pDC->SelectObject(&pen);
		pDC->MoveTo( curRc.left, curRc.top);
		pDC->LineTo( curRc.right, curRc.top);
		pDC->LineTo( curRc.right, curRc.bottom);
		pDC->LineTo( curRc.left, curRc.bottom);
		pDC->LineTo( curRc.left,curRc.top);
		pDC->SelectObject(pOldPen1);
	}

	if(bHasSelectBox == true)
	{
		CPen pen(PS_SOLID, 3, RGB(255, 130, 130));
		CPen* pOldPen1 = pDC->SelectObject(&pen);
		pDC->MoveTo( selBoxRc.left, selBoxRc.top);
		pDC->LineTo( selBoxRc.right, selBoxRc.top);
		pDC->LineTo( selBoxRc.right, selBoxRc.bottom);
		pDC->LineTo( selBoxRc.left, selBoxRc.bottom);
		pDC->LineTo( selBoxRc.left,selBoxRc.top);
		pDC->SelectObject(pOldPen1);
	}
	pDC->SelectObject(pOldPen);
}

bool CTimeAixes::DoSplitter()
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	GetCursorPos(&pos);
	ScreenToClient(&pos);
	int d = m_iListWidth - pos.x;

	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			pos.x = LOWORD(msg.lParam);
			pos.y = HIWORD(msg.lParam);

			m_iListWidth = pos.x + d;

			m_ToolBar.SetWindowPos(this, 0, 0, m_iListWidth-2, 20, SWP_NOZORDER);

			//TRACE("%d, %d\n", pos.y , m_iListWidth);
			Invalidate(FALSE);
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	return false;
}

bool CTimeAixes::DoEndLine()
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	GetCursorPos(&pos);
	ScreenToClient(&pos);
	//int d = m_i - pos.x;

	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			pos.x = LOWORD(msg.lParam);
			pos.y = HIWORD(msg.lParam);


			m_iEndLine = (pos.x - (m_iListWidth + 2) - m_iHorPos) / (m_iStepPixes / m_iExactInStep);
			//m_iListWidth = pos.x + d;
			//TRACE("%d, %d\n", pos.y , m_iListWidth);
			Invalidate(FALSE);
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
	Notify(TAN_ENDLINE_CHANGE, m_iHoverLayer);
	return false;
}

bool CTimeAixes::DoRulerDrag()
{
	::MSG msg;
	POINT pos;
	POINT orginPos;
	POINT orginPosScreen;

	bool bExit = false;
	GetCursorPos(&orginPosScreen);
	GetCursorPos(&orginPos);
	ScreenToClient(&orginPos);
	int dst = 0;
	int oldHor = m_iHorPos;

	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			pos.x = LOWORD(msg.lParam);
			pos.y = HIWORD(msg.lParam);
			dst = pos.x - orginPos.x;
			m_iHorPos = oldHor + dst;
			if (m_iHorPos > 0) 
				m_iHorPos = 0;
			Invalidate(FALSE);
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	return false;
}

bool CTimeAixes::DoVDrag()
{
	::MSG msg;
	POINT pos;
	POINT orginPos;
	POINT orginPosScreen;

	bool bExit = false;
	GetCursorPos(&orginPosScreen);
	GetCursorPos(&orginPos);
	ScreenToClient(&orginPos);
	int dst = 0;
	int oldVet = m_iVetPos;

	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			pos.x = LOWORD(msg.lParam);
			pos.y = HIWORD(msg.lParam);
			dst = pos.y - orginPos.y;
			m_iVetPos = oldVet + dst;
			if (m_iVetPos > 0) 
				m_iVetPos = 0;
			Invalidate(FALSE);
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	return false;
}

bool CTimeAixes::DoLayerBoxSize(int area)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	int box = 0;
	int len = 0;
	CTimeLayer& theLayer = *m_layers[m_iHoverLayer];

	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			pos.x = LOWORD(msg.lParam);
			pos.y = HIWORD(msg.lParam);
			box = GetBoxByPoint(&pos);
			theLayer.SetLen( box - theLayer.GetFirst() + 1, area);
			m_iSelectedBox = box;
			if(theLayer.GetLen() <= 0)
			{
				m_iSelectedBox = theLayer.GetFirst(area);
				theLayer.SetLen( 1, area);
			}
			Notify(TAN_RED_BOX_CHANGE, m_iHoverLayer);
			//TRACE("theLayer.GetLen() = %d \n", theLayer.GetLen());
			Invalidate(FALSE);
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}
	Notify(TAN_SIZE_CHANGE, m_iHoverLayer, area);

	return false;
}


struct HIGHT_LIGHT_AREA
{
	int iLayer;
	int iArea;
	int iPos;
};

bool CTimeAixes::DoLayerBoxPos(int area)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	int box = 0;
	int len = 0;
	CTimeLayer& theLayer = *m_layers[m_iHoverLayer];
	int least = 0;
	int most = -1;

	int newbox = -1;
	int oldbox = theLayer.GetFirst(area);
	int first = 0;
	//TRACE("Box Size Start, From %d \n", oldbox);
	if(area > 0)
	{
		least = theLayer.GetEndBox(area - 1) + 1;
	}
	if(area < theLayer.GetSubItemCount() - 1)
	{
		most = theLayer.GetEndBox(area + 1) - theLayer.GetLen(area);
	}

	std::vector<HIGHT_LIGHT_AREA> hightlight;
	for(int i = 0; i < (int) m_layers.size(); i++)
	{
		for(int k = 0; k < (int)m_layers[i]->m_blocks.size(); k++)
		{
			if(m_layers[i]->GetHightLight(k) == true)
			{
				HIGHT_LIGHT_AREA box;
				box.iArea = k;
				box.iLayer = i;
				hightlight.push_back(box);
			}
		}
	}

	int left = -1;
	int right = -1;

	for(int i = 0; i < (int) hightlight.size(); i++)
	{
		int currLeast = 0;
		int currMost = -1;
		CTimeLayer& curLayer = *m_layers[hightlight[i].iLayer];
		int curPos = curLayer.GetFirst(hightlight[i].iArea);
		hightlight[i].iPos = curPos;

		int leastArea = hightlight[i].iArea - 1;
		int mostArea = hightlight[i].iArea + 1;

		if(hightlight[i].iArea > 0)
		{
			while(leastArea >= 0 && curLayer.GetHightLight(leastArea) == true)
				leastArea--;

			if(leastArea >= 0)
				currLeast = curLayer.GetEndBox(leastArea) + 1;
		}

		if(hightlight[i].iArea < curLayer.GetSubItemCount() - 1)
		{
			while(mostArea>0 && mostArea < curLayer.GetSubItemCount() && curLayer.GetHightLight(mostArea) == true)
				mostArea--;

			if(mostArea < curLayer.GetSubItemCount())
			{
				int tmp = (mostArea>0?curLayer.GetLen(mostArea-1):1);
				currMost = curLayer.GetEndBox(mostArea) - tmp;
			}
		}

		int currLeft = curPos - currLeast;
		int currRight = -1;
		if(currMost >= curPos)
		{
			currRight = currMost - curPos;
		}

		if(left == -1 || currLeft < left)
			left = currLeft;
		if(right == -1 || (currRight < right && currRight >= 0))
			right = currRight;
	}


	int iMouseAreaPos = theLayer.GetFirst(area);
	int totalLeast = 0;
	int totalMost = -1;
	if(left >= 0)
		totalLeast = iMouseAreaPos - left;
	if(right >= 0)
		totalMost = iMouseAreaPos + right;

	most = totalMost;
	least = totalLeast;
	int shift = 0;
	int oldsel = m_iSelectedBox;


	while(!bExit && ::GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			{
				pos.x = LOWORD(msg.lParam);
				pos.y = HIWORD(msg.lParam);
				box = GetBoxByPoint(&pos);
				shift = box - iMouseAreaPos;
				if(shift < -left && left != -1)
					shift = -left;
				if(shift > right && right != -1)
					shift = right;

				if(shift != 0)
				{
					for(int i = 0; i < (int)hightlight.size(); i++)
					{
						CTimeLayer& currLayer = *m_layers[hightlight[i].iLayer];
						currLayer.SetFirst(hightlight[i].iPos + shift, hightlight[i].iArea);
						//TRACE("Set Area:%d  From:%d with Shift:%d\n", hightlight[i].iArea, hightlight[i].iPos, shift);
					}
					m_iSelectedBox = oldsel + shift;
					Invalidate(FALSE);
				}

			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}
	}


	int currbox = theLayer.GetFirst(area);

	if(shift != 0)
	{
		for(int i = 0; i < (int)hightlight.size(); i++)
		{
			Notify(TAN_POS_CHANGE, hightlight[i].iLayer, hightlight[i].iArea);
		}
		Notify(TAN_RED_BOX_CHANGE, m_iHoverLayer);

	}
	//TRACE("Box Size End, From %d To %d\n", oldbox, currbox);

	return false;
}

bool CTimeAixes::DoPointerDrag()
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	int box = 0;
	bool bNeedNotify = false;


	while(!bExit )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == FALSE)
		{
			static_cast<CMainFrame*>(theApp.m_pMainWnd)->DoFrame();
		}
		//else
		{
			TranslateMessage(&msg);
			switch(msg.message) 
			{
			case WM_MOUSEMOVE: 
				pos.x = LOWORD(msg.lParam);
				pos.y = HIWORD(msg.lParam);
				box = GetBoxByPoint(&pos);
				if(m_iSelectedBox != box && box >= 0)
				{
					m_iSelectedBox = box;
					Invalidate(FALSE);
					Notify(TAN_RED_BOX_CHANGE, 0, 0);
				}
				break;
			case WM_LBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
				bExit = true;
				break;
			default:
				DispatchMessage(&msg);
				break;
			}
		}
	}

	return false;
}

bool CTimeAixes::DoCtrlDrag(int iArea)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	int box = 0;
	bool bNeedNotify = false;

	int iLayer = m_iHoverLayer;
	int iHovArea = -1;
	bool bConform = false;

	while(!bExit )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == FALSE)
		{
			static_cast<CMainFrame*>(theApp.m_pMainWnd)->DoFrame();
		}

		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			pos.x = LOWORD(msg.lParam);
			pos.y = HIWORD(msg.lParam);
			box = GetBoxByPoint(&pos);
			iHovArea = m_layers[iLayer]->IsContainBlock(box);
			if(m_iDragHoverBox != box && box >= 0 && iHovArea < 0)
			{
				m_iDragHoverBox = box;
				Invalidate(FALSE);
				//Notify(TAN_RED_BOX_CHANGE, 0, 0);
			}
			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			bConform = true;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}

	}

	//Notify(TAN_RED_BOX_CHANGE, 0, 0);

	if(bConform == true)
	{
		Notify(TAN_CTRL_DRAG, m_iHoverLayer, iArea, m_iDragHoverBox);
	}

	//拖动小标回位
	m_iDragHoverBox = -1;
	Invalidate(FALSE);
	return false;
}

bool CTimeAixes::DoLayerPos(int iLayer)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	bool bNeedNotify = false;

	bool bConform = false;

	while(!bExit )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == FALSE)
		{
			static_cast<CMainFrame*>(theApp.m_pMainWnd)->DoFrame();
		}

		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			{
				pos.x = LOWORD(msg.lParam);
				pos.y = HIWORD(msg.lParam);
				for(int i = 0; i < (int)m_layers.size(); i++)
				{
					CRect layerTitleRect(0
						, m_iVetPos + (i+1) * m_iLayerHeight, m_iListWidth - 2
						, m_iVetPos + (i + 2) * m_iLayerHeight);

					if(layerTitleRect.bottom < m_iLayerHeight)
					{
						continue;
					}

					if(layerTitleRect.top < m_iLayerHeight)
					{
						layerTitleRect.top = m_iLayerHeight;
					}

					if(layerTitleRect.PtInRect(pos) == TRUE)
					{
						int iOld = m_iLayerInsertAfter;
						m_iLayerInsertAfter = i;
						if(iOld != m_iLayerInsertAfter)
						{
							Invalidate(FALSE);
						}
					}
				}
			}

			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			bConform = true;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}

	}

	//Notify(TAN_RED_BOX_CHANGE, 0, 0);

	if(bConform == true)
	{
		//		Notify(TAN_LAYER_POS , iLayer, 0, m_iLayerInsertAfter);
		if(!m_bSelectLayer[m_iLayerInsertAfter])
			NotifyMultiSel(TAN_LAYER_POS , 0, 0, m_iLayerInsertAfter);
	}

	//插入标记回位
	m_iLayerInsertAfter = -1;
	Invalidate(FALSE);
	return false;
}

bool CTimeAixes::DoLayerRelate(int iLayer)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	bool bNeedNotify = false;

	bool bConform = false;

	while(!bExit )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == FALSE)
		{
			static_cast<CMainFrame*>(theApp.m_pMainWnd)->DoFrame();
		}

		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			{
				pos.x = LOWORD(msg.lParam);
				pos.y = HIWORD(msg.lParam);
				for(int i = 0; i < (int)m_layers.size(); i++)
				{
					CRect layerTitleRect(0
						, m_iVetPos + (i+1) * m_iLayerHeight, m_iListWidth - 2
						, m_iVetPos + (i + 2) * m_iLayerHeight);

					if(layerTitleRect.bottom < m_iLayerHeight)
					{
						continue;
					}

					if(layerTitleRect.top < m_iLayerHeight)
					{
						layerTitleRect.top = m_iLayerHeight;
					}

					if(layerTitleRect.PtInRect(pos) == TRUE)
					{
						int iOld = m_iLayerRelateTo;
						m_iLayerRelateTo = i;
						if(iOld != m_iLayerRelateTo)
						{
							Invalidate(FALSE);
						}
					}
				}
			}

			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			bConform = true;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}

	}

	//Notify(TAN_RED_BOX_CHANGE, 0, 0);

	if(bConform == true)
	{
		//		Notify(TAN_LAYER_RELATE , iLayer, 0, m_iLayerRelateTo);
		if(!m_bSelectLayer[m_iLayerRelateTo])
			NotifyMultiSel(TAN_LAYER_RELATE , 0, 0, m_iLayerRelateTo);
	}

	//关j标记回位
	m_iLayerRelateTo = -1;
	Invalidate(FALSE);
	return false;
}

struct BOX_SCALE_INFO
{
	int iArea;
	float fRatio;
	float fPos;
	int iPos;
	int iOldPos;
};
bool CTimeAixes::DoLayerScale(int iLayer)
{
	::MSG msg;
	POINT pos;
	bool bExit = false;
	bool bNeedNotify = false;
	bool bConform = false;

	if(iLayer < 0 || iLayer >= (int) m_layers.size())
		return false;

	CTimeLayer* pLayer = m_layers[iLayer];

	std::vector<BOX_SCALE_INFO> scalepos;

	int last = -1;
	int firstblock = -1;
	int lastblock = -1;
	int nextAreaPos = -1;
	bool startedsel = false;

	for(int i = 0; i < (int) pLayer->GetSubItemCount(); i++)
	{
		if(pLayer->GetHightLight(i) == true)
		{
			//int kstart = last + 1;
			//if(startedsel == false)
			//{
			//	kstart = i;
			//	startedsel = true;
			//}
			//for(int k = last + 1; k <= i; k++)
			//{
			//	BOX_SCALE_INFO bsi;
			//	bsi.iArea = k;
			//	bsi.fRatio = 1.0f;
			//	bsi.fPos = pLayer->m_blocks[k].m_iFirst;
			//	bsi.iPos = pLayer->m_blocks[k].m_iFirst;
			//	bsi.iOldPos = pLayer->m_blocks[k].m_iFirst;
			//	if(firstblock == -1)
			//	{
			//		firstblock = bsi.fPos;
			//	}
			//	lastblock = bsi.fPos;
			//	scalepos.push_back(bsi);
			//	if(k + 1 < pLayer->GetSubItemCount())
			//	{
			//		nextAreaPos = pLayer->m_blocks[k + 1].m_iFirst;
			//	}
			//	else
			//	{
			//		nextAreaPos = -1;
			//	}
			//}
			//last = i;

			if(firstblock == -1)
			{
				firstblock = i;
			}
			lastblock = i;
		}
	}

	if(firstblock == -1 || lastblock == -1)
		return false;

	for(int k = firstblock; k <= (int)lastblock; k++)
	{
		BOX_SCALE_INFO bsi;
		bsi.iArea = k;
		bsi.fRatio = 1.0f;
		bsi.fPos = pLayer->m_blocks[k].m_iFirst;
		bsi.iPos = pLayer->m_blocks[k].m_iFirst;
		bsi.iOldPos = pLayer->m_blocks[k].m_iFirst;

		scalepos.push_back(bsi);
		if(k + 1 < pLayer->GetSubItemCount())
		{
			nextAreaPos = pLayer->m_blocks[k + 1].m_iFirst;
		}
		else
		{
			nextAreaPos = -1;
		}
	}

	int len = pLayer->m_blocks[lastblock].m_iFirst  - pLayer->m_blocks[firstblock].m_iFirst;
	for(int i = 0; i < (int)scalepos.size(); i++)
	{
		int offset = scalepos[i].fPos - pLayer->m_blocks[firstblock].m_iFirst;
		scalepos[i].fRatio = (float)offset / (float)len;
	}

	int oldpos = m_iHoverBox;
	int offsetbox = 0;;
	while(!bExit )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == FALSE)
		{
			static_cast<CMainFrame*>(theApp.m_pMainWnd)->DoFrame();
		}

		TranslateMessage(&msg);
		switch(msg.message) 
		{
		case WM_MOUSEMOVE: 
			{
				pos.x = LOWORD(msg.lParam);
				pos.y = HIWORD(msg.lParam);

				int box = GetBoxByPoint(&pos);
				if(box <= scalepos[0].iPos || (nextAreaPos >= 0 && box >= nextAreaPos))
				{
				}
				else
				{

					offsetbox = box - oldpos;

					float scaleratio = offsetbox / (float)len;

					for(int i = 0; i < (int)scalepos.size(); i++)
					{
						float fposoff = offsetbox * scalepos[i].fRatio;
						scalepos[i].iPos = scalepos[i].iOldPos + (int)fposoff;
						pLayer->SetFirst(scalepos[i].iPos, scalepos[i].iArea);
					}

					Invalidate(FALSE);
				}
			}

			break;
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			bConform = true;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			bExit = true;
			break;
		default:
			DispatchMessage(&msg);
			break;
		}

	}

	if(offsetbox > 0)
	{
		for(int i = 0; i < (int) scalepos.size(); i++)
		{
			Notify(TAN_POS_CHANGE, iLayer, scalepos[scalepos.size() - 1 - i].iArea);
		}
	}
	else if(offsetbox < 0)
	{
		for(int i = 0; i < (int) scalepos.size(); i++)
		{
			Notify(TAN_POS_CHANGE, iLayer, scalepos[i].iArea);
		}
	}

	Invalidate(FALSE);
	return false;
}

void CTimeAixes::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CRect clientRect;
	GetClientRect(clientRect);
	int i = 0;
	if(clientRect.PtInRect(point))
	{
		//SetCapture();
		//SetFocus();
		//Set Hover State
		UpdateHovingState(point);

		if(m_hoverArea == HA_SPLITTER)
		{
			SetMouseArrow( MT_SIZEWE);
		}
		else if(m_hoverArea == HA_GRAPHIC && m_iHoverLayer >= 0)
		{
			int areaEnd = m_layers[m_iHoverLayer]->IsEndBlock(m_iHoverBox);
			int areaFirst = m_layers[m_iHoverLayer]->IsFirstBlock(m_iHoverBox);

			if(areaFirst >= 0 && m_layers[m_iHoverLayer]->IsAllowPos(areaFirst))
			{
				SetMouseArrow(MT_SIZEALL);
			}
			else if(areaEnd >= 0 && m_layers[m_iHoverLayer]->IsAllowSize(areaEnd))
			{
				SetMouseArrow(MT_SIZEWE);
			}
			else
			{
				SetMouseArrow(MT_ARROW);
			}
		}
		else if(m_hoverArea == HA_ENDLINE)
		{
			SetMouseArrow(MT_SIZEWE);
		}
		else if(m_hoverArea == HA_POINTER)
		{
			SetMouseArrow(MT_SIZEALL);
		}
		else
		{
			SetMouseArrow( MT_ARROW);
		}
	}
	else
	{
		//ReleaseCapture();
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CTimeAixes::SetMouseArrow(MOUSE_TYPE type)
{
	if(type == m_CurrMouse)
		return;

	switch(type)
	{
	case MT_ARROW:
		SetCursor(m_hMouseArrow);
		break;
	case MT_SIZEALL:
		SetCursor(m_hMouseSizeAll);
		break;
	case MT_SIZEWE:
		SetCursor(m_hMouseSizeWE);
		break;
	default:
		break;
	}
	m_CurrMouse = type;
}

void CTimeAixes::OnLButtonUp(UINT nFlags, CPoint point)
{
	int templayer = m_iButtonDownLayer;
	CWnd::OnLButtonUp(nFlags,point);
	m_iButtonDownLayer = -1;
	m_bLButtonDown = false;
}

void CTimeAixes::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	m_bLButtonDown = true;

	bool bSelectCurrent = false;
	m_iButtonDownLayer = m_iHoverLayer;

	//	SetFocus();
	if((m_hoverArea == HA_LIST || m_hoverArea == HA_GRAPHIC) && m_iHoverLayer >= 0)
	{
		OnClickLayerList(m_iHoverLayer);
		if(m_hoverArea == HA_GRAPHIC)
		{
			m_iSelectedBox = m_iHoverBox;
			Notify(TAN_RED_BOX_CHANGE, 0, 0);
		}

		if(m_iHoverLayer >= 0 && m_iHoverLayer < (int)m_layers.size())
		{
			CTimeLayer* pLayer = m_layers[m_iHoverLayer];
			for(int i = 0; i < pLayer->m_usedOption; i++)
			{
				CRect optionRect = GetLayerOptionRect(m_iHoverLayer, i);
				if(optionRect.PtInRect(point) == TRUE)
				{
					Notify(TAN_CLICK_OPTION, m_iHoverLayer, 0, (DWORD)i);
					break;

				}
			}
		}

	}

	if(m_hoverArea == HA_LIST && m_iHoverLayer >= 0)
	{
		if((GetAsyncKeyState(VK_CONTROL)&0xff00) != 0)
		{
			if(m_eLastKeyState != SHIFT)
			{
				for(int i=0; i<MAX_LAYER_NUM; i++)
					m_bSelectLayer[i] = false;
				m_bSelectLayer[m_iHoverLayer] = true;
				m_iSelectedLayer = m_iHoverLayer;
			}
			m_eLastKeyState = CTRL;

			DoLayerPos(m_iHoverLayer);
		}
		else if((GetAsyncKeyState(VK_SHIFT)&0xff00) != 0)
		{//只负责多选
			if(m_eLastKeyState != SHIFT)
			{
				for(int i=0; i<MAX_LAYER_NUM; i++)
					m_bSelectLayer[m_iHoverLayer] = false;
				m_bSelectLayer[m_iHoverLayer] = true;
			}
			else
			{
				m_bSelectLayer[m_iHoverLayer] = !m_bSelectLayer[m_iHoverLayer];
			}
			m_eLastKeyState = SHIFT;
		}
		else if((GetAsyncKeyState(VK_MENU)&0xff00) != 0)
		{
			if(m_eLastKeyState != SHIFT)
			{
				for(int i=0; i<MAX_LAYER_NUM; i++)
					m_bSelectLayer[i] = false;
				m_bSelectLayer[m_iHoverLayer] = true;
			}
			m_eLastKeyState = ALT;

			DoLayerRelate(m_iHoverLayer);
		}
		else
		{
			for(int i=0; i<MAX_LAYER_NUM; i++)
				m_bSelectLayer[i] = false;
			m_bSelectLayer[m_iHoverLayer] = true;
			m_eLastKeyState = NONE;

			DoVDrag();
		}
	}
	else if(m_hoverArea == HA_SPLITTER)
	{
		DoSplitter();
	}
	else if(m_hoverArea == HA_GRAPHIC && m_iHoverLayer == HL_RULER )
	{
		DoRulerDrag();
	}
	else if(m_hoverArea == HA_ENDLINE )
	{
		DoEndLine();
	}
	else if(m_hoverArea == HA_POINTER)
	{
		DoPointerDrag();
	}
	else if(m_hoverArea == HA_GRAPHIC && m_iHoverLayer >= 0 )
	{
		for(int i=0; i<MAX_LAYER_NUM; i++)
			m_bSelectLayer[i] = false;
		m_bSelectLayer[m_iHoverLayer] = true;

		int box = GetBoxByPoint(&point);
		int areaEnd = m_layers[m_iHoverLayer]->IsEndBlock(box);
		int areaFirst = m_layers[m_iHoverLayer]->IsFirstBlock(box);

		bool bCtrl = (GetAsyncKeyState(VK_CONTROL)&0xff00)!=0 ? true : false;
		bool bShift = (GetAsyncKeyState(VK_SHIFT)&0xff00)!=0 ? true : false;

		if(!bShift)
		{
			for(int i = 0; i < (int)m_layers.size(); i++)
			{
				m_layers[i]->ClearHightLight();
			}			
		}	

		if(m_iSelectedLayer >= 0 && areaFirst >= 0)
		{

			m_layers[m_iSelectedLayer]->SetHightLight(true, areaFirst);
		}


		if(areaEnd >= 0 && m_layers[m_iHoverLayer]->IsAllowSize(areaEnd) == true)
		{
			DoLayerBoxSize(areaEnd);
		}
		else if(areaFirst >= 0 && m_layers[m_iHoverLayer]->IsAllowPos(areaFirst) == true)
		{
			if(bCtrl && m_layers[m_iHoverLayer]->IsAllowDrag(areaFirst) == true )
			{
				DoCtrlDrag(areaFirst);
			}
			else
			{
				DoLayerBoxPos(areaFirst);
			}

		}
	}

	Invalidate(FALSE);
}

void CTimeAixes::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_hoverArea == HA_LIST && m_iHoverLayer >= 0)
	{
		Notify(TAN_LAYER_ACTIVE, m_iHoverLayer, 0, m_iHoverBox );
	}
	else 
	{
		if(m_hoverArea == HA_GRAPHIC && m_iHoverLayer >= 0)
		{
			int area = m_layers[m_iHoverLayer]->IsContainBlock(m_iHoverBox);
			Notify(TAN_LAYER_ACTIVE, m_iHoverLayer, area, m_iHoverBox);
		}
	}

	if(m_iHoverLayer == HL_RULER)
	{
		m_iVetPos = 0;
		m_iHorPos = 0;
		Invalidate(FALSE);
	}
	CWnd::OnLButtonDblClk(nFlags, point);

}

void CTimeAixes::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_hoverArea == HA_LIST && m_iHoverLayer >= 0)
	{
		if(!m_bSelectLayer[m_iHoverLayer])
		{
			for(int i=0; i<MAX_LAYER_NUM; i++)
				m_bSelectLayer[i] = false;
			m_bSelectLayer[m_iHoverLayer] = true;
		}
		m_iSelectedLayer = m_iHoverLayer;

		CPoint point;
		::GetCursorPos(&point);

		CBCGPPopupMenu* lodMenu = new CBCGPPopupMenu();
		lodMenu->Create(this, point.x, point.y, ::GetSubMenu(::LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_LAYERLOD)),0), FALSE, TRUE);

		int id = -1000;
		CModelMotionInfo* pInfo = CEffectMgr::GetInstance()->GetModelMotion();
		assert(pInfo->m_iSelMontion >= 0 && pInfo->m_iSelMontion < pInfo->m_motions.size());
		CModelMotionInfo::MOTION* pstMotion = pInfo->m_motions[pInfo->m_iSelMontion];
		int nLodLevel = pstMotion->objs[m_iHoverLayer]->m_iObjLayerLodLevel;
		switch (nLodLevel)
		{
		case 10000:	id = ID_SET_NOHANDLE;	break;
		case 100:	id = ID_SET_LOWLOD;		break;
		case 200:	id = ID_SET_MIDDLELOD;	break;
		case 300:	id = ID_SET_HIGHLOD;	break;
		default:	MessageBox("CTimeAixes::OnRButtonDown LOD Wrong !");	break;
		}
		lodMenu->SetDefaultItem(id);
		lodMenu->RemoveItem(3);
		lodMenu->UpdateShadow();
		lodMenu->SetAutoDestroy(true);
	}

	if((GetAsyncKeyState(VK_SHIFT)&0xff00)!=0)
	{
		DoLayerScale(m_iHoverLayer);
	}
	else
	{
		if(m_iSelectedLayer>=0 && m_iSelectedLayer<(int)m_layers.size())
		{
			OnClickLayerList(m_iSelectedLayer);
			Notify(TAN_SELECT_CHANGE, m_iSelectedLayer); //这里的代码其实是有问题的，所以无论怎样都Notify这个消息
		}
		else OnClickLayerList(HL_NONE);
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CTimeAixes::OnSetFocus(CWnd* pOldWnd)
{
	SetFocus();
	//	CWnd::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
}

BOOL CTimeAixes::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP 
		|| pMsg->message == WM_LBUTTONUP || pMsg->message == WM_LBUTTONDOWN
		|| pMsg->message == WM_RBUTTONUP || pMsg->message == WM_RBUTTONDOWN)
	{
		SetFocus();
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		Notify(TAN_KEY_DOWN, 0, 0, (DWORD)pMsg->wParam);
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CTimeAixes::OnSetHighLod()
{
	//	Notify(TAN_LOD_CHANGE, m_iHoverLayer, 0, Ogre::MOTION_LOD_HIGH);
	NotifyMultiSel(TAN_LOD_CHANGE, 0, 0, Ogre::MOTION_LOD_HIGH);
	Invalidate(FALSE);
}

void CTimeAixes::OnSetMiddleLod()
{
	//	Notify(TAN_LOD_CHANGE, m_iHoverLayer, 0, Ogre::MOTION_LOD_MIDDLE);
	NotifyMultiSel(TAN_LOD_CHANGE, 0, 0, Ogre::MOTION_LOD_MIDDLE);
	Invalidate(FALSE);
}

void CTimeAixes::OnSetLowLod()
{
	//	Notify(TAN_LOD_CHANGE, m_iHoverLayer, 0, Ogre::MOTION_LOD_LOW);
	NotifyMultiSel(TAN_LOD_CHANGE, 0, 0, Ogre::MOTION_LOD_LOW);
	Invalidate(FALSE);
}

void CTimeAixes::OnExitLodPopMenu()
{}

void CTimeAixes::OnNoHandleLod()
{//因为不处理层现在不用了，所以暂时先定为：Notify，而不是NotifyMultiSel
	Notify(TAN_LOD_CHANGE, m_iHoverLayer, 0 ,Ogre::MOTION_LOD_NOHANDLE);
	Invalidate(FALSE);
}

void CTimeAixes::OnCopyLayers()
{
	gLayersOpObj.Reset();
	CModelMotionInfo* pModelMotion = CEffectMgr::GetInstance()->GetModelMotion();
	int nMotion = pModelMotion->m_iSelMontion;
	assert(nMotion>=0 && nMotion<(int)pModelMotion->m_motions.size());
	for(int i=0; i<MAX_LAYER_NUM; i++)
	{
		if(m_bSelectLayer[i])
			gLayersOpObj.vLayers.push_back(i);
	}
	gLayersOpObj.stMotion = nMotion;
	gLayersOpObj.stOpType = COPY;
	Notify(TAN_LAYERS_COPY_OP, 0, 0, 0);
}

void CTimeAixes::OnCutLaysers()
{
	gLayersOpObj.Reset();
	CModelMotionInfo* pModelMotion = CEffectMgr::GetInstance()->GetModelMotion();
	int nMotion = pModelMotion->m_iSelMontion;
	assert(nMotion>=0 && nMotion<(int)pModelMotion->m_motions.size());
	for(int i=0; i<MAX_LAYER_NUM; i++)
	{
		if(m_bSelectLayer[i])
			gLayersOpObj.vLayers.push_back(i);
	}
	gLayersOpObj.stMotion = nMotion;
	gLayersOpObj.stOpType = CUT;
	Notify(TAN_LAYERS_CUT_OP, 0, 0, 0);
}

void CTimeAixes::OnPasteLayers()
{
	if(gLayersOpObj.stOpType == NONE_LAYERS_SEL)
		return;
	Notify(TAN_LAYERS_PASTE_OP, 0, 0, 0);
}

void CTimeAixes::OnSelAllKeys()
{
	assert(m_iSelectedLayer>=0 && m_iSelectedLayer<(int)m_layers.size());
	CTimeLayer* pTimeLayer = m_layers[m_iSelectedLayer];
	for(int i=0; i<(int)pTimeLayer->m_blocks.size(); i++)
	{
		pTimeLayer->SetHightLight(true, i);
	}
	Invalidate(FALSE);
}

void CTimeAixes::OnShowHighLodLayers()
{
	m_eSelLodState = HIGH_STATE;
	Notify(TAN_LOD_SHOWSTATE, 0, 0, HIGH_STATE);
}

void CTimeAixes::OnUpdateShowHighLodLayers(CCmdUI* pCmdUI)
{
	if(m_eSelLodState == HIGH_STATE)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CTimeAixes::OnShowMiddleLodLayers()
{
	m_eSelLodState = MIDDLE_STATE;
	Notify(TAN_LOD_SHOWSTATE, 0, 0, MIDDLE_STATE);
}

void CTimeAixes::OnUpdateShowMiddleLodLayers(CCmdUI* pCmdUI)
{
	if(m_eSelLodState == MIDDLE_STATE)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CTimeAixes::OnShowLowLodLayers()
{
	m_eSelLodState = LOW_STATE;
	Notify(TAN_LOD_SHOWSTATE, 0, 0, LOW_STATE);
}

void CTimeAixes::OnUpdateShowLowLodLayers(CCmdUI* pCmdUI)
{
	if(m_eSelLodState == LOW_STATE)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}
