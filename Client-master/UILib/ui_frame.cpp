#include "ui_frame.h"
#include "ui_fontstring.h"
#include "ui_texture.h"
#include "ui_modelview.h"
#include "ui_drawlineframe.h"
#include "ui_gameui.h"
#include <string>
#include "ui_framemgr.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// Frame control
//-----------------------------------------------------------------------------
Ogre::UIRenderer *g_pDisplay = NULL;
Ogre::ScriptVM *g_pUIScriptVM = NULL;
extern FrameManager* g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(Frame)

Frame::Frame()
{
	m_Type = FRM_FRAME;

	m_DrawRegions.clear();
	m_Children.clear();

	m_fAlpha = 1.0f;
	m_bMovable = false;
	m_bResizable = false;

	m_bEnableMouse = false;
	m_bEnableKeyboard = false;
	m_bClampedToScreen = false;
	m_bProtected = false;

	m_State = FRMSTATE_VISIBLE;
	m_Style = FRMSTYLE_NONE;

	m_iMousePositionX = m_iMousePositionY = 0;
	m_iFrameLevel = -1;
	m_bToplevel = false;

	m_iEdgeSize = 0;
	m_iTileSize = 0;
	m_BorderColor = ColorQuad(255, 255, 255, 255);
	m_EdgeColor = ColorQuad(255, 255, 255, 255);
	m_bTile = false;
	m_BackgroundInsets = RectInt(0, 0, 0, 0);

	m_bShowTooltip = false;
	m_IdleTime = 0.5f;	//默认延迟时间
	m_IdleAccTime = 0.0f;

	m_UpdateTime = 0.3f;
	m_UpdateAccTime = 0.0f;

	m_hEdgeTex = HUIRES(0);
	m_hBorderTex = HUIRES(0);

	m_MovePoint			= FP_TOPLEFT;
	m_fMoveDuration		= m_fMoveStart = 0.0f;
	m_strModalFrame = "";
}

Frame::~Frame()
{
	size_t i;
	for(i=0; i<m_Children.size(); ++i)
	{
		m_Children[i]->release();
	}

	for(i=0; i<m_DrawRegions.size(); ++i)
	{
		m_DrawRegions[i].pregion->release();
	}
	
	g_pDisplay->ReleaseUIRes(m_hBorderTex);
	g_pDisplay->ReleaseUIRes(m_hEdgeTex);
}

void Frame::CopyMembers(Frame *pdest)
{
	if (pdest)
	{
		LayoutFrame::CopyMembers(pdest);

		pdest->m_DrawRegions.resize(m_DrawRegions.size());
		for (size_t i=0; i<m_DrawRegions.size(); ++i)
		{
			pdest->m_DrawRegions[i].pregion = static_cast<LayoutFrame *>(m_DrawRegions[i].pregion->CreateClone());
			pdest->m_DrawRegions[i].pregion->m_pParent = pdest;
			pdest->m_DrawRegions[i].layer = m_DrawRegions[i].layer;
		}

		pdest->m_fAlpha = m_fAlpha;
		pdest->m_bMovable = m_bMovable;
		pdest->m_bResizable = m_bResizable;
		pdest->m_bEnableMouse = m_bEnableMouse;
		pdest->m_bEnableKeyboard = m_bEnableKeyboard;
		pdest->m_bClampedToScreen = m_bClampedToScreen;
		pdest->m_bProtected = m_bProtected;

		pdest->m_Pattern = m_Pattern;
		pdest->m_Style = m_Style;
		pdest->m_State = m_State;

		pdest->m_iFrameLevel = m_iFrameLevel;
		pdest->m_bToplevel = m_bToplevel;
		pdest->m_bInputTransparent = m_bInputTransparent;
		pdest->m_strModalFrame = m_strModalFrame;	
		
		pdest->m_Type = m_Type;

		pdest->m_Children.resize(m_Children.size());
		for (size_t i=0; i<m_Children.size(); ++i)
		{
			pdest->m_Children[i] = static_cast<Frame *>(m_Children[i]->CreateClone());
		}

		pdest->m_iEdgeSize = m_iEdgeSize;
		pdest->m_iTileSize = m_iTileSize;
		pdest->m_BorderColor = m_BorderColor;
		pdest->m_EdgeColor = m_EdgeColor;
		pdest->m_bTile = m_bTile;
		pdest->m_BackgroundInsets = m_BackgroundInsets;

		pdest->m_hEdgeTex = AssignHUIRes(m_hEdgeTex);
		pdest->m_hBorderTex = AssignHUIRes(m_hBorderTex);

		pdest->m_EdgeTexName = m_EdgeTexName;
		pdest->m_BorderTexName = m_BorderTexName;
		pdest->m_fMoveStart = m_fMoveStart;
		pdest->m_fMoveDuration = m_fMoveDuration;
	}
}

//设置frame的层级,层级是同FrameStrata下的二级排序值
void Frame::SetFrameLevel(int level) 
{
	m_iFrameLevel = level * DEFAULT_FRAME_MAX_DEPTH;
}

int Frame::GetFrameLevel()
{
	//float level = (float)m_iFrameLevel/10;
	return (int)m_iFrameLevel;
}

//把模版中的$parent替换成正确的Frame Name
void Frame::ReplaceSpecialName()
{
	size_t i;
	for (i=0; i<LayoutFrame::MAX_ANCHOR; ++i)
	{
		if(strstr(m_Anchor[i].m_RelFrame.c_str(), "$parent"))
		{
			if (m_pParent)
			{
				std::string strFrameName = m_pParent->GetName();
				strFrameName += m_Anchor[i].m_RelFrame.substr(7);
				m_Anchor[i].m_RelFrame = strFrameName;
			}
			else
			{
				m_Anchor[i].m_RelFrame = "";
			}
		}
	}

	for (i=0; i<m_DrawRegions.size(); ++i)
	{
		if(strstr(m_DrawRegions[i].pregion->m_Name.c_str(), "$parent"))
		{
			std::string strFrameName = m_Name;
			strFrameName += m_DrawRegions[i].pregion->m_Name.substr(7);
			m_DrawRegions[i].pregion->SetName(strFrameName.c_str());
		}

		for (int j=0; j<LayoutFrame::MAX_ANCHOR; j++)
		{
			std::string str = m_DrawRegions[i].pregion->m_Anchor[j].m_RelFrame;
			if(strstr(m_DrawRegions[i].pregion->m_Anchor[j].m_RelFrame.c_str(), "$parent"))
			{
				std::string strFrameName = m_Name;
				strFrameName += m_DrawRegions[i].pregion->m_Anchor[j].m_RelFrame.substr(7);
				m_DrawRegions[i].pregion->m_Anchor[j].m_RelFrame = strFrameName;
			}
		}
	}

	for (i=0; i<m_Children.size(); ++i)
	{
		if (strstr(m_Children[i]->m_Name.c_str(), "$parent"))
		{
			std::string strFrameName = m_Name;
			strFrameName += m_Children[i]->m_Name.substr(7);
			m_Children[i]->SetName(strFrameName.c_str());
		}

		m_Children[i]->m_pParent = this;

		m_Children[i]->ReplaceSpecialName();
	}
}

//面板显示或者隐藏设置
void Frame::ShowUIPanel()
{
	this->Show();
	for (size_t i=0; i<m_Children.size(); ++i)
	{
		m_Children[i]->m_pParent = this;
	}
}

void Frame::HideUIPanel()
{
	this->Hide();
	for (size_t i=0; i<m_Children.size(); ++i)
	{
		m_Children[i]->m_pParent = this;
	}
}

//初始化一级排序索引FrameStrata
void Frame::InitFrameStrata()
{
	if (m_FrameStrata == FS_NO_DEFINED)
	{
		if (m_pParent)
		{
			m_FrameStrata = m_pParent->m_FrameStrata;
		}
		else
		{
			m_FrameStrata = FS_PARENT;
		}
	}

	for (size_t i=0; i<m_Children.size(); ++i)
	{
		m_Children[i]->m_pParent = this;
		m_Children[i]->InitFrameStrata();
	}
}

void Frame::UpdateHiddenFrameRecursive()
{
	if (m_bShow)
	{
		for (size_t i=0; i<m_DrawRegions.size(); ++i)
		{
			LayoutFrame *pFrame = m_DrawRegions[i].pregion;
			if ((pFrame->m_AbsRect.m_Top + pFrame->m_ViewStartPoint.y < m_AbsRect.m_Top) ||
				(pFrame->m_AbsRect.m_Bottom + pFrame->m_ViewStartPoint.y > m_AbsRect.m_Bottom))
			{
				pFrame->Hide();
			}
		}

		for (size_t i=0; i<m_Children.size(); ++i)
		{
			if (strcmp(m_Children[i]->GetTypeName(), "Slider"))
			{
				int top = m_Children[i]->GetFrameTop();
				int bottom = m_Children[i]->GetFrameBottom();

				if ((m_Children[i]->GetFrameTop() < m_AbsRect.m_Top) ||
					(m_Children[i]->GetFrameBottom() > m_AbsRect.m_Bottom))
				{
					m_Children[i]->UpdateHiddenFrameRecursive();
				}
			}
		}
	}
}

//Upata UI ecah frame
void Frame::UpdateSelf(float deltatime)
{
	if (m_bShow)
	{
		AdjustFrameByViewPoint();

		//触发update脚本消息
		if ( hasScriptsEvent(SC_ONUPDATE) )
		{
			m_UpdateAccTime += deltatime;

			if ( m_UpdateAccTime >= m_UpdateTime )
			{
				CallScript( SC_ONUPDATE );
				m_UpdateAccTime = 0.0f;
			}
		}

		// 更新DrawRegions
		for (size_t i=0; i<m_DrawRegions.size(); ++i)
		{
			m_DrawRegions[i].pregion->UpdateSelf(deltatime);
		}

		for (size_t i=0; i<m_Children.size(); ++i)
		{
			Frame *pFrame = static_cast<Frame *>(m_Children[i]);
			if (m_Children[i]->m_bShow)
			{
				m_Children[i]->UpdateSelf(deltatime);
			}
		}

		if (m_Style & FRMSTYLE_CLIPSELF)
		{
			if (m_pParent)
			{
				//如果该窗口的父窗口有clip属性的话父窗口会为该窗口的cliprect赋值
				//如果该窗口的cliprect和absrect相同高度，则不需要重新赋值cliprect，否则是因为窗口的大小发生了改变，需要赋值
				if ((m_pParent->m_Style & FRMSTYLE_CLIPSELF) || (m_ClipRect.getHeight() == m_AbsRect.getHeight()))
				{
					return;
				}
			}

			m_ClipRect = m_AbsRect;
			m_ClipRect.m_Top = 29;
		}
		if( m_fMoveDuration != 0.0f )
		{
			m_fMoveStart += deltatime;
		}
	}
}

//把该桢需要的渲染对象加入渲染队列
void Frame::FillDrawItems()
{
	if ( !m_bFrameDraw )
	{
		return;
	}

	if (m_bShow || (m_Name == UI_ROOT_NAME.c_str()))
	{
		m_iDrawLevel = -1;

		if (m_bToplevel)
		{
			m_iDrawLevel = DEFAULT_FRAME_MAX_LEVLE;
		}
		else
		{
			if (m_iFrameLevel != -1)
			{
				m_iDrawLevel = m_iFrameLevel;
			}
			else
			{
				if (m_pParent && m_pParent->m_iFrameLevel != -1)
				{
					m_iDrawLevel  = m_pParent->m_iFrameLevel;
				}
			}
		}

		g_pFrameMgr->AddDrawItems(static_cast<LayoutFrame *>(this));

		FillChildren();
	}
}

void Frame::FillDrawLevel()
{
	if ( m_bShow )
	{
		if ( m_pParent != NULL )
		{
			m_iDrawLevel = m_pParent->m_iDrawLevel + 1;
		}

		for ( size_t i = 0, size = m_Children.size(); i < size; ++i )
		{
			m_Children[i]->FillDrawLevel();
		}
	}
}

void Frame::DrawBackDrop()
{
	float fScaleY   = g_pFrameMgr->GetScreenScaleY() * m_fSelfScale;
	int nEdgeSize   = FloatToInt( m_iEdgeSize * fScaleY )==0?1:FloatToInt( m_iEdgeSize * fScaleY );
	int nWidth      = FloatToInt( getFrameSizeX()*fScaleY );
	int nHeight     = FloatToInt( getFrameSizeY()*fScaleY );  
	//绘制背景
	if( (nWidth > 2 * nEdgeSize) && (nHeight > 2 * nEdgeSize))
	{
		g_pDisplay->BeginDraw(m_hBorderTex);

		//绘制背景的范围要去掉边界前面的几个像素，定义在xml中BackgroundInset的部分
		g_pDisplay->StretchRect((float)m_AbsRect.m_Left + m_BackgroundInsets.m_Left * fScaleY,
		                        (float)m_AbsRect.m_Top + m_BackgroundInsets.m_Top * fScaleY, 
		                        (float)m_AbsRect.getWidth() - m_BackgroundInsets.m_Left * fScaleY - m_BackgroundInsets.m_Right * fScaleY, 
		                        (float)m_AbsRect.getHeight() - m_BackgroundInsets.m_Top * fScaleY - m_BackgroundInsets.m_Bottom * fScaleY,
		                        m_BorderColor.c,
		                        0,
		                        0,
		                        m_AbsRect.getWidth(),
		                        m_AbsRect.getHeight()
		                        );

		g_pDisplay->EndDraw();
	}

	//绘制边
	if( m_hEdgeTex != NULL )
	{
		g_pDisplay->BeginDraw(m_hEdgeTex);
		//绘制四个边
		if( nHeight > 2 * nEdgeSize) //两条竖边
		{
            int num = int(nHeight / nEdgeSize);
            int offset = (nHeight - num * nEdgeSize);

			for(int i=1; i<num-1; ++i)
			{
				g_pDisplay->StretchRect((float)m_AbsRect.m_Left, 
				                        (float)m_AbsRect.m_Top + i * nEdgeSize, 
					                    (float)nEdgeSize, 
					                    (float)nEdgeSize,
					                    m_EdgeColor.c,false,0, 0, m_iEdgeSize, m_iEdgeSize);

				g_pDisplay->StretchRect((float)m_AbsRect.m_Right-nEdgeSize, 
				                        (float)m_AbsRect.m_Top + i * nEdgeSize, 
				                        (float)nEdgeSize,
					                    (float)nEdgeSize, 
					                    m_EdgeColor.c, false,m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize);
			}

			//补画最后一小段
			if( offset > 0 )
			{
				g_pDisplay->StretchRect((float)m_AbsRect.m_Left, 
				                        (float)m_AbsRect.m_Top + (num-1) * nEdgeSize, 
					                    (float)nEdgeSize, 
					                    (float)m_AbsRect.m_Bottom - (float)m_AbsRect.m_Top - num*nEdgeSize,
					                    m_EdgeColor.c, false,0, 0, m_iEdgeSize, m_iEdgeSize);

				g_pDisplay->StretchRect((float)m_AbsRect.m_Right - nEdgeSize, 
				                        (float)m_AbsRect.m_Top + (num-1) * nEdgeSize, 
				                        (float)nEdgeSize,
					                    ((float)m_AbsRect.m_Bottom - (float)m_AbsRect.m_Top - num*nEdgeSize), 
					                    m_EdgeColor.c, false,1*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize);
			}
		}

		if( nWidth > 2 * nEdgeSize) //两条横边
		{
            int num = int(nWidth / nEdgeSize);
            int offset = (nWidth - num * nEdgeSize);

			for (int i=1; i<num-1; ++i)
			{   
                g_pDisplay->StretchRect((float)m_AbsRect.m_Left + i * nEdgeSize, 
                                        (float)m_AbsRect.m_Top, 
                                        (float)nEdgeSize, 
                                        (float)nEdgeSize, 
                                        m_EdgeColor.c,false, 2*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize/*, UI_UVT_TURN90*/);
                g_pDisplay->StretchRect((float)m_AbsRect.m_Left + ( i * nEdgeSize ), 
                                        (float)m_AbsRect.m_Bottom - ( nEdgeSize), 
                                        (float)nEdgeSize, 
                                        (float)nEdgeSize, 
                                        m_EdgeColor.c, false,3*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize/*, UI_UVT_TURN90*/);
			}

			if ( offset > 0 )
			{
				//补画最后一小段
				g_pDisplay->StretchRect( (float)m_AbsRect.m_Left + (num-1) * nEdgeSize, 
                                         (float)m_AbsRect.m_Top, 
					                     (float)m_AbsRect.m_Right - (float)m_AbsRect.m_Left - num * nEdgeSize, 
					                     (float)nEdgeSize, 
					                     m_EdgeColor.c, false,2*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize/*, UI_UVT_TURN90*/);

				g_pDisplay->StretchRect( (float)m_AbsRect.m_Left + (num-1) * nEdgeSize, 
				                         (float)m_AbsRect.m_Bottom - nEdgeSize, 
				                         (float)m_AbsRect.m_Right - (float)m_AbsRect.m_Left - num * nEdgeSize,
					                     (float)nEdgeSize, 
					                      m_EdgeColor.c,false, 3*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize/*, UI_UVT_TURN90*/);
			}
		}

		//绘制四个角
		{
			g_pDisplay->StretchRect( (float)m_AbsRect.m_Left, 
                                     (float)m_AbsRect.m_Top, 
                                     (float)nEdgeSize,
				                     (float)nEdgeSize, 
				                      m_EdgeColor.c, false,4*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize);

			g_pDisplay->StretchRect((float)m_AbsRect.m_Right - nEdgeSize, 
			                        (float)m_AbsRect.m_Top,
				                    (float)nEdgeSize, 
				                    (float)nEdgeSize,
				                    m_EdgeColor.c,false, 5*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize);

			g_pDisplay->StretchRect((float)m_AbsRect.m_Left, 
			                        (float)m_AbsRect.m_Bottom - nEdgeSize,
				                    (float)nEdgeSize, 
				                    (float)nEdgeSize,
				                    m_EdgeColor.c, false,6*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize);

			g_pDisplay->StretchRect((float)m_AbsRect.m_Right- nEdgeSize, 
			                        (float)m_AbsRect.m_Bottom - nEdgeSize, 
			                        (float)nEdgeSize, 
			                        (float)nEdgeSize,
				                    m_EdgeColor.c, false,7*m_iEdgeSize, 0, m_iEdgeSize, m_iEdgeSize);
		}

		g_pDisplay->EndDraw();
	}

}

//用于调试的函数
void Frame::DrawDebugBarEdge()
{
	if (m_Name == UI_ROOT_NAME)
	{
		return;
	}

	//白色窗口边缘
	g_pDisplay->DrawBox((float)m_AbsRect.m_Left, (float)m_AbsRect.m_Top, (float)m_AbsRect.getWidth(),
		(float)m_AbsRect.getHeight(), ColorQuad(255, 255, 255, 255).c);

	//红色的拖动区边缘
	g_pDisplay->DrawBox( (float)(m_AbsRect.m_Left - 2), (float)(m_AbsRect.m_Top -2),
		(float)4, (float)4, ColorQuad(255,0,0,255).c );
}


void Frame::MoveFrame( const char *strMovePoint, float nMoveDuration, int nOffsetX, int nOffsetY )
{
	if (strMovePoint != NULL && nMoveDuration != 0.0f )
	{
		FRAMEPOINT_T movepoint = FP_TOPLEFT;
		for (size_t i=0; i<sizeof( s_FramePoint ); i++)
		{
			if (i > FP_CENTER)
			{
				break;
			}

			if( stricmp(strMovePoint, s_FramePoint[i]) == 0 )
			{
				movepoint = FRAMEPOINT_T(i);
				break;
			}
		}
		m_MovePoint			= movepoint;
		m_fMoveDuration		= nMoveDuration;
		m_fMoveStart		= 0.0f;
		m_MoveOffset.x		= nOffsetX;
		m_MoveOffset.y		= nOffsetY;
		m_InitOffset.SetAbsDim( (int)m_Anchor[0].m_Offset.GetX(), (int)m_Anchor[0].m_Offset.GetY());
	}
}

void Frame::EndMoveFrame( )
{
	m_fMoveStart = m_fMoveDuration = 0.0f;
}

void Frame::SetBackDropBorderTex( const char* pszBorderTexPath )
{
	Ogre::HUIRES h = m_hBorderTex;
	m_hBorderTex = g_pDisplay->CreateTexture( pszBorderTexPath, Ogre::BLEND_ALPHABLEND );
	g_pDisplay->ReleaseUIRes( h );
}

void Frame::SetBackDropEdgeTex( const char* pszEdgeTexPath )
{
	Ogre::HUIRES h = m_hEdgeTex;
	m_hEdgeTex = g_pDisplay->CreateTexture( pszEdgeTexPath, Ogre::BLEND_ALPHABLEND );
	g_pDisplay->ReleaseUIRes( h );
}

void Frame::SetBackDropAttr( int nEdgeSize, int nInsetLeft, int nInsetRight, int nInsetTop, int nInsetBottom )
{
	m_iEdgeSize	= nEdgeSize;
	m_BackgroundInsets.m_Left	= nInsetLeft;
	m_BackgroundInsets.m_Right	= nInsetRight;
	m_BackgroundInsets.m_Top	= nInsetTop;
	m_BackgroundInsets.m_Bottom	= nInsetBottom;
}

void Frame::SetSelfScale(float scale)
{ 
	if ( m_pParent == NULL && stricmp( GetTypeName(), "Frame" ) == 0 )
	{
		g_pFrameMgr->AddReCalPosFrame( this );
	}

	m_fSelfScale = scale; 

	for ( std::vector<DrawObj>::iterator iter = m_DrawRegions.begin(), end = m_DrawRegions.end();
		iter != end; ++iter )
	{
		LayoutFrame* pFrame = iter->pregion;
		pFrame->SetSelfScale( scale );
	}

	for ( std::vector<Frame *>::iterator iter = m_Children.begin(), end = m_Children.end();
		iter != end; ++iter )
	{
		LayoutFrame* pFrame = *iter;
		pFrame->SetSelfScale( scale );
	}
}

//绘制控件,在Update, FillDrawItems之后绘制
void Frame::Draw()
{
	if (m_Style & FRMSTYLE_CLIPSELF)
	{
		/*m_ClipRect.m_Top = 29;
		g_pDisplay->PushClipRect(RectFloat((float)21, (float)29,
			(float)42, (float)59));*/
		/*g_pDisplay->PushClipRect(RectFloat((float)m_ClipRect.m_Left, (float)m_ClipRect.m_Top,
		(float)m_ClipRect.m_Right, (float)m_ClipRect.m_Bottom));*/
	}

	if (m_bTile)
	{
		DrawBackDrop();
	}

	if( m_fMoveDuration != 0.0f )
	{
		float fRate = 0.0f;
		int nOffsetx = 0, nOffsety = 0;
		switch( m_MovePoint )
		{
		case FP_TOP:
			{
				nOffsety = -m_MoveOffset.y;
				break;
			}

		case FP_BOTTOM:
			{
				nOffsety = m_MoveOffset.y;
				break;
			}

		case FP_LEFT:
			{
				nOffsetx = -m_MoveOffset.x;
				break;
			}

		case FP_RIGHT:
			{
				nOffsetx = m_MoveOffset.x;
				break;
			}
		}
		if( m_fMoveStart < m_fMoveDuration)
		{
			fRate = m_fMoveStart/m_fMoveDuration;
		}else{
			fRate = 1.0f;
			m_fMoveStart = m_fMoveDuration = 0.0f;
			if (hasScriptsEvent(SC_ONENDMOVE))
			{
				CallScript(SC_ONENDMOVE);
			}
		}

		SetPoint(s_FramePoint[m_Anchor[0].m_MyPoint], 
			m_Anchor[0].m_RelFrame.c_str(), s_FramePoint[m_Anchor[0].m_RelPoint],
			(int)(m_InitOffset.GetX() + nOffsetx*fRate),
			(int)(m_InitOffset.GetY() + nOffsety*fRate) );
	}

	for(size_t i = 0; i<DL_NUMLAYERS; ++i)
	{
		for (size_t j=0; j<m_DrawRegions.size(); j++)
		{
			if (m_DrawRegions[j].layer == i)
			{
				if (m_DrawRegions[j].pregion->m_bShow)
				{
					m_DrawRegions[j].pregion->Draw();
				}
			}
		}
	}

	if(g_pFrameMgr->GetEditMode() == UI_MODE_DEBUG)
	{
		DrawDebugBarEdge();
	}

	/*if (m_Style & FRMSTYLE_CLIPSELF)
	{
		g_pDisplay->PopClipRect();
	}*/
}

TiXmlElement *Frame::Save(TiXmlElement *pElement)
{
	TiXmlElement *pFrame = LayoutFrame::Save(pElement);

	if (!pFrame)
	{
		return NULL;
	}

	//frameStrata
	if (m_FrameStrata != FS_NO_DEFINED)
	{
		if (m_pParent)
		{
			if (m_FrameStrata != m_pParent->m_FrameStrata)
			{
				pFrame->SetAttribute("frameStrata", s_FrameStrata[m_FrameStrata]);
			}
		}
		else
		{
			pFrame->SetAttribute("frameStrata", s_FrameStrata[m_FrameStrata]);
		}
	}

	//moveable
	if (m_bMovable)
	{
		pFrame->SetAttribute("moveable", "true");
	}

	//resizeable
	if (m_bResizable)
	{
		//pFrame->SetAttribute("resizea")
	}

	if (m_fAlpha != 1.0f)
	{
		//pFrame->SetAttribute("alpha", m_fAlpha);
	}

	//enableMouse
	if (m_bEnableMouse)
	{
		pFrame->SetAttribute("enableMouse", "true");
	}

	//enableKeyboard
	if (m_bEnableKeyboard)
	{
		pFrame->SetAttribute("enableKeyboard", "true");
	}

	//clampedToScreen
	if (m_bClampedToScreen)
	{
		pFrame->SetAttribute("clampedToScreen", "true");
	}

	//protected
	if (m_bProtected)
	{
		pFrame->SetAttribute("protected", "true");
	}

	//m_Pattern

	//m_Style

	//m_State

	if (m_bToplevel)
	{
		pFrame->SetAttribute("toplevel", "true");
	}

	//backDrop
	if (!m_EdgeTexName.empty())
	{
		TiXmlElement *pBackDrop = new TiXmlElement("Backdrop");
		pFrame->LinkEndChild(pBackDrop);

		pBackDrop->SetAttribute("edgeFile", m_EdgeTexName.c_str());
		if (!m_BorderTexName.empty())
		{
			pBackDrop->SetAttribute("bgFile", m_BorderTexName.c_str());
		}

		pBackDrop->SetAttribute("tile", "true");

		TiXmlElement *pEdgeSize = new TiXmlElement("EdgeSize");
		pBackDrop->LinkEndChild(pEdgeSize);

		TiXmlElement *pEdgeSizeVal = new TiXmlElement("AbsValue");
		pEdgeSize->LinkEndChild(pEdgeSizeVal);
		pEdgeSizeVal->SetAttribute("val", m_iEdgeSize);

		TiXmlElement *pTileSize = new TiXmlElement("TileSize");
		pBackDrop->LinkEndChild(pTileSize);

		TiXmlElement *pTileSizeVal = new TiXmlElement("AbsValue");
		pTileSize->LinkEndChild(pTileSizeVal);
		pTileSizeVal->SetAttribute("val", m_iTileSize);

		TiXmlElement *pBackgroundInsets = new TiXmlElement("BackgroundInsets");
		pBackDrop->LinkEndChild(pBackgroundInsets);

		TiXmlElement *pBackgroundInsetsAbsInset = new TiXmlElement("AbsInset");
		pBackgroundInsets->LinkEndChild(pBackgroundInsetsAbsInset);

		pBackgroundInsetsAbsInset->SetAttribute("left", m_BackgroundInsets.m_Left);
		pBackgroundInsetsAbsInset->SetAttribute("right", m_BackgroundInsets.m_Right);
		pBackgroundInsetsAbsInset->SetAttribute("top", m_BackgroundInsets.m_Top);
		pBackgroundInsetsAbsInset->SetAttribute("bottom", m_BackgroundInsets.m_Bottom);

	}

	//showtooltip



	//clientid
	if (m_ClientId != 0)
	{
		pFrame->SetAttribute("id", m_ClientId);
	}

	//Drawregion
	if (m_DrawRegions.size() > 0)
	{
		bool bNormalRegion = true;
		for (size_t i=0; i<m_DrawRegions.size(); i++)
		{
			std::string strName = m_DrawRegions[i].pregion->GetName();
			if (!strstr(strName.c_str(), "NormalRegion") && !strstr(strName.c_str(), "OverlayRegion"))
			{
				bNormalRegion = false;
			}
		}

		if (!bNormalRegion)
		{
			TiXmlElement *pLayers = new TiXmlElement("Layers");
			pFrame->LinkEndChild(pLayers);

			for (int i=0; i<DL_NUMLAYERS; i++)
			{
				bool bHasItem = false;
				for (size_t j=0; j<m_DrawRegions.size(); j++)
				{
					if (m_DrawRegions[j].layer == DRAWLAYER_T(i))
					{
						bHasItem = true;
						break;
					}
				}
				if (bHasItem)
				{
					TiXmlElement *pLayer = new TiXmlElement("Layer");
					pLayers->LinkEndChild(pLayer);

					pLayer->SetAttribute("level", s_Drawlayer[i]);

					for (size_t j=0; j<m_DrawRegions.size(); j++)
					{
						std::string strName = m_DrawRegions[j].pregion->GetName();
						if (!strstr(strName.c_str(), "NormalRegion") && !strstr(strName.c_str(), "OverlayRegion"))
						{
							if (m_DrawRegions[j].layer == DRAWLAYER_T(i))
							{
								m_DrawRegions[j].pregion->Save(pLayer);
							}
						}

					}		
				}
			}
		}
	}

	//子Frame
	if (m_Children.size() > 0)
	{
		if (strcmp(GetTypeName(), "ScrollFrame")) //scrollframe scrollchild
		{
			TiXmlElement *pFrames = new TiXmlElement("Frames");
			pFrame->LinkEndChild(pFrames);

			for (size_t i=0; i<m_Children.size(); i++)
			{
				m_Children[i]->Save(pFrames);
			}
		}
	}

	//scripts
	bool bHasScript = false;
	for (int i=0; i<SC_MAXEVENT; ++i)
	{
		if (hasScriptsEvent(i))
		{
			bHasScript = true;
			break;
		}
	}
	if (bHasScript)
	{
		TiXmlElement *pScripts = new TiXmlElement("Scripts");
		pFrame->LinkEndChild(pScripts);

		for (int i=0; i<SC_MAXEVENT; ++i)
		{
			if (hasScriptsEvent(i))
			{
				TiXmlElement *pScript = new TiXmlElement(s_EventName[i]);
				pScripts->LinkEndChild(pScript);

				TiXmlText *pText = new TiXmlText(m_pScriptStr[i].c_str());
				pScript->LinkEndChild(pText);
			}
		}
	}

	return pFrame;
}

int Frame::OnKeyDown(const Ogre::InputEvent &event)
{
//	switch(event.key.vkey) 
	{			
		//case VK_RETURN:	//enterpressed
		//	{
		//		if (!m_pScriptStr[SC_ONENTERPRESSED].empty())
		//		{
		//			CallScript(SC_ONENTERPRESSED);
		//		}else
		//		{
		//			g_pFrameMgr->GetUIClientFrame()->CallScript(SC_ONENTERPRESSED);
		//		}
		//		return 0;
		//	}
		//	break;
		//case VK_F2:
		//	{
		//	#ifdef _DEBUG
		//		//在debug模式下Reload脚本
		//		ReLoadLuaFile(g_pFrameMgr->m_TocFileName.c_str());
		//	#endif
		//	}
		//	break;
//	default:
//		return 1;
	}

	//return 0;

	return 1;
}

int Frame::OnChar(const Ogre::InputEvent &event)
{
	switch(event.chars.str[0]) 
	{			
	case VK_RETURN:	//enterpressed
		{
			if (hasScriptsEvent(SC_ONENTERPRESSED))
			{
				CallScript(SC_ONENTERPRESSED);
				return 0;
			}else
			{
				g_pFrameMgr->GetUIClientFrame()->CallScript(SC_ONENTERPRESSED);
			}
		}
		return 1;
	case VK_ESCAPE:	//enterpressed
		{
			if (hasScriptsEvent(SC_ONESCAPEPRESSED))
			{
				CallScript(SC_ONESCAPEPRESSED);
				return 0;
			}else
			{
				g_pFrameMgr->GetUIClientFrame()->CallScript(SC_ONESCAPEPRESSED);
			}
		}
		return 1;
	case VK_TAB:
		{
			if (hasScriptsEvent(SC_ONTABPRESSED))
			{
				CallScript(SC_ONTABPRESSED);
				return 0;
			}else
			{
				g_pFrameMgr->GetUIClientFrame()->CallScript(SC_ONTABPRESSED);
			}
		}
		return 1;
	default:
		break;
	}

	return 1;
}

void Frame::RegisterToFrameMgr(FrameManager *pframemgr)
{
	pframemgr->RegisterObject(this);

	size_t i;
	for(i=0; i<m_Children.size(); i++)
	{
		m_Children[i]->RegisterToFrameMgr(pframemgr);
	}

	for(i=0; i<m_DrawRegions.size(); i++)
	{
		pframemgr->RegisterObject(m_DrawRegions[i].pregion);
	}
}

Frame* Frame::transferEventToNextFrame( int nMouseX, int nMouseY )
{
	std::vector<Frame*> vFrameTemp;
	g_pFrameMgr->FindUIObjectOnPoint( vFrameTemp, nMouseX, nMouseY, false );
	std::vector<Frame*>::iterator	frameIter;
	std::vector<Frame*>::iterator	frameEndIter = vFrameTemp.end();
	for ( frameIter = vFrameTemp.begin(); frameIter != frameEndIter; ++frameIter )
	{
		if ( stricmp( (*frameIter)->GetName(), GetName() ) == 0 )
		{
			break;
		}
	}
	
	if ( frameIter == frameEndIter )
	{
		return g_pFrameMgr->GetUIClientFrame();
	}
	// 若是可穿透的，或若是当前起始控件的父控件则忽略
	std::vector<Frame*>::iterator iter;
	for ( iter = ++frameIter; iter != frameEndIter; ++iter )
	{
		if ( ( *iter )->m_bInputTransparent || strcmp( ( *iter )->GetName(), GetParent() ) == 0 )
		{
			continue;
		}

		break;
	}
	
	if ( iter == frameEndIter )
	{
		return g_pFrameMgr->GetUIClientFrame();
	}

	return *iter;
}
int Frame::OnInputMessage(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_LBTNDBLCLK:
		{
			if (m_bShow)
			{
				if (hasScriptsEvent(SC_ONDOUBLECLICK))
				{
					CallScript(SC_ONDOUBLECLICK, "s", "LDoubleClick");
					return 0;
				}
			}
		}
		break;

	case GIE_CHAR:
		return OnChar(event);

	case GIE_KEYDOWN:
		return OnKeyDown(event);

	case GIE_MOUSEMOVE:
		{
			int x = event.mouse.x;
			int y = event.mouse.y;
			if (m_bShow && (m_State &= FRMSTATE_CLICKDOWN) && m_bMovable)
			{
				Ogre::RectInt rect;
				GetAbsRect(rect);
				SetPoint(s_FramePoint[m_Anchor[0].m_MyPoint], 
					m_Anchor[0].m_RelFrame.c_str(), s_FramePoint[m_Anchor[0].m_RelPoint],
					(int)( m_Anchor[0].m_Offset.GetX() + ( m_AbsRect.m_Left - rect.m_Left ) + (ceil(x/g_pFrameMgr->GetScreenScaleY()))-
							(ceil(m_iMousePositionX/g_pFrameMgr->GetScreenScaleY())) ),
					(int)( m_Anchor[0].m_Offset.GetY() + ( m_AbsRect.m_Top - rect.m_Top) + (ceil(y/g_pFrameMgr->GetScreenScaleY()))-
							(ceil(m_iMousePositionY/g_pFrameMgr->GetScreenScaleY())) )
					);

				g_pFrameMgr->UpdateChangedFrames();
				g_pFrameMgr->m_pDragFrame = this; 

				m_iMousePositionX = x;
				m_iMousePositionY = y;
			}
			break;
		}

	case GIE_LBTNDOWN:
		{
			if ( m_bShow )
			{
				if( PointInLayoutFrame( event.mouse.x, event.mouse.y) )
				{
					m_State |= FRMSTATE_CLICKDOWN;
				}
				m_iMousePositionX = event.mouse.x;
				m_iMousePositionY = event.mouse.y;
			
				if (hasScriptsEvent(SC_ONCLICK))
				{
					CallScript(SC_ONCLICK, "s", "LeftButton");
				}
			}
			//调用子类的消息响应函数
			break;
		}

	case GIE_LBTNUP:
		if( m_State & FRMSTATE_CLICKDOWN )
		{	
			m_State &= ~FRMSTATE_CLICKDOWN;
			g_pFrameMgr->m_pDragFrame = NULL;
		}
		break;

	case GIE_MOUSEWHEEL:
		{
			// 查看Frame子控件中是否有Slider
			if (hasScriptsEvent(SC_ONMOUSEWHEEL))
			{
				CallScript( SC_ONMOUSEWHEEL, "i", (int)event.wheel.dist );
			}
			else
			{
				for ( size_t i = 0, size = m_Children.size(); i < size; ++i )
				{
					if ( stricmp( m_Children[i]->GetTypeName(), "Slider" ) == 0 )
					{
						m_Children[i]->OnInputMessage( event );
					}
				}
			}
		}
		break;

	case GIE_MOUSEOUT:
		{
			for ( size_t i = 0, size = m_Children.size(); i < size; ++i )
			{
				m_Children[i]->OnInputMessage( event );
			}
		}
		break;
	default:
		break;
	}

	//自己不处理,向父亲发送	
	if (m_Name == UI_ROOT_NAME.c_str())
	{
		return 1;
	}else
	{
		return 0;
	}
}

//为frame添加一个FontString，添加后会绘制
void Frame::AddFontString(DRAWLAYER_T layer, FontString *pfontstr)
{
	if(pfontstr)
	{
		pfontstr->addRef();
		pfontstr->m_pParent = this;

		DrawObj obj;
		obj.layer = layer;
		obj.pregion = (LayoutFrame* )pfontstr;
		pfontstr->SetSelfScale( m_fSelfScale );
		m_DrawRegions.push_back(obj);
	}
}

//为frame添加一个Texture，添加后会绘制
void Frame::AddTexture(DRAWLAYER_T layer, ::Texture *ptex)
{
	if (ptex)
	{
		ptex->addRef();
		ptex->m_pParent = this;

		DrawObj obj;
		obj.layer = layer;
		obj.pregion = (LayoutFrame* )ptex;
		ptex->SetSelfScale( m_fSelfScale );
		m_DrawRegions.push_back(obj);
	}
}

void Frame::AddModelView(DRAWLAYER_T layer, ModelView *pview)
{
	if(pview)
	{
		pview->addRef();
		pview->m_pParent = this;

		DrawObj obj;
		obj.layer = layer;
		obj.pregion = (LayoutFrame* )pview;
		pview->SetSelfScale( m_fSelfScale );
		m_DrawRegions.push_back(obj);
	}
}

void Frame::AddLineFrame( DRAWLAYER_T layer, DrawLineFrame *pline )
{
	if( pline )
	{
		pline->addRef();
		pline->m_pParent = this;

		DrawObj obj;
		obj.layer = layer;
		obj.pregion = (LayoutFrame* )pline;
		pline->SetSelfScale( m_fSelfScale );
		m_DrawRegions.push_back(obj);
	}
}

//添加一个子控件
void Frame::AddChildFrame(Frame *pframe)
{
	if(pframe)
	{
		pframe->addRef();
		pframe->m_pParent = this;
		pframe->SetSelfScale( m_fSelfScale );
		m_Children.push_back(pframe);
	}
}

//获取当前的绘制对象总数
int Frame::GetNumRegions()
{
	return int(m_DrawRegions.size());
}

//获取一个frame的上部,包含其子窗口
int Frame::GetFrameTop()
{
	int top = m_AbsRect.m_Top;
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		if (m_DrawRegions[i].pregion->m_bShow)
		{
			if (top > m_DrawRegions[i].pregion->m_AbsRect.m_Top)
			{
				top = m_DrawRegions[i].pregion->m_AbsRect.m_Top;
			}
		}
	}

	for (size_t i=0; i<m_Children.size(); i++)
	{
		if (m_Children[i]->m_bShow)
		{
			int ctop = m_Children[i]->GetFrameBottom(); 
			if (top > ctop)
			{
				top = ctop;
			}
		}
	}

	return top;
}

//获取一个frame的下部,包含其子窗口
int Frame::GetFrameBottom()
{
	int bottom = m_AbsRect.m_Bottom;
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		if (m_DrawRegions[i].pregion->m_bShow)
		{
			if (bottom < m_DrawRegions[i].pregion->m_AbsRect.m_Bottom)
			{
				bottom = m_DrawRegions[i].pregion->m_AbsRect.m_Bottom;
			}
		}
	}

	for (size_t i=0; i<m_Children.size(); i++)
	{
		if (m_Children[i]->m_bShow)
		{
			int cbottom = m_Children[i]->GetFrameBottom(); 
			if (bottom < cbottom)
			{
				bottom = cbottom;
			}
		}
	}

	return bottom;
}

void Frame::SetViewStartPointRecursive(int x, int y)
{
	SetViewStartPoint( x, y );
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		m_DrawRegions[i].pregion->SetViewStartPoint(x, y);
	}

	for (size_t i=0; i<m_Children.size(); i++)
	{
		m_Children[i]->SetViewStartPointRecursive(x, y);
	}
}

void Frame::IncreaseViewStartPointRecursive(  int x, int y )	// 增量移动frame
{
	IncreaseViewStartPoint( x, y );
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		m_DrawRegions[i].pregion->IncreaseViewStartPoint( x, y );
	}

	for (size_t i=0; i<m_Children.size(); i++)
	{
		m_Children[i]->IncreaseViewStartPointRecursive(x, y);
	}
}

void Frame::MoveFrameRecursive( int x, int y ) // 移动frame
{
	MoveFrameAbsrect( x, y );
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		m_DrawRegions[i].pregion->MoveFrameAbsrect(x, y);
	}

	for (size_t i=0; i<m_Children.size(); i++)
	{
		m_Children[i]->MoveFrameRecursive( x, y );
	}
}

//获取子控件, 目前没有使用
Frame *Frame::GetChildren()
{
	return NULL;
}

//迭代函数,遍历所有子控件
void Frame::FillChildren()
{
	for (size_t i=0; i<m_Children.size(); ++i)
	{
		if (m_Style & FRMSTYLE_CLIPSELF)
		{
			m_Children[i]->m_ClipRect = m_ClipRect;
			m_Children[i]->m_Style |= FRMSTYLE_CLIPSELF;
		}
		m_Children[i]->FillDrawItems();
	}
}

//迭代函数,每桢计算控件的实际位置
void Frame::CalFrameRectRecursive(size_t iFrame)
{
	CalAbsRectSelf(iFrame);

	for(size_t i=0; i<m_DrawRegions.size(); ++i)
	{
		m_DrawRegions[i].pregion->CalAbsRectSelf(iFrame);
	}

	for (size_t i=0; i<m_Children.size(); ++i)
	{
		m_Children[i]->CalFrameRectRecursive(iFrame);
	}
}

//查找鼠标点击的位置所有的控件
void Frame::FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *> &vFrameTemp )
{
	if ( !m_bShow )
	{
		return;
	}

	// 筛选的过程进行改变，(1)若是MOUSEMOVE事件的时候，a.若RichText透明且有超链接的时候，则在鼠标位于
	// 超链接的时候保留RichText，则只处理RichText事件；b.若RichText透明，但是鼠标没有位于超链接的时候，
	// 则将消息穿透到下一层
	if ( !m_bInputTransparent && PointInLayoutFrame(mousex, mousey) )
	{
		vFrameTemp.push_back(this);
	}

	// 解决方案: 按他们被解析的顺序逆序获取被点击的控件集合
	for ( std::vector<Frame*>::reverse_iterator riter = m_Children.rbegin(), rend = m_Children.rend(); 
		riter != rend; ++riter )
	{
		if ( m_FrameStrata > ( *riter )->m_FrameStrata )
		{
			continue;
		}
		else
		{
			( *riter )->FindFrameOnPoint(mousex, mousey, vFrameTemp );
		}
	}
}

// 鼠标点击后把该控件的FrameLevel提高，绘制的时候不会出现排序问题
void Frame::AddLevelRecursive(int iLevel)
{
	g_pFrameMgr->m_iCurMaxFrmLevel += DEFAULT_FRAME_MAX_DEPTH;

	m_iFrameLevel = g_pFrameMgr->m_iCurMaxFrmLevel;
}

void Frame::RegisterEvent(const char *event)
{
	if (event)
	{
		stdext::hash_map<std::string, stEventFrameArray>::iterator iter = EventMap.find(event);
		if (iter != EventMap.end())
		{
			iter->second.EventFrameArray.push_back(this);
		}
		else
		{
			stEventFrameArray frameArray;
			frameArray.EventFrameArray.push_back(this);
			EventMap[event] = frameArray;
		}
	}
}

void Frame::AddHitRect(const LayoutRect &rect)
{

}

void Frame::MoveTo()
{
}

void Frame::DisableDrawLayer(DRAWLAYER_T layer)
{
}

void Frame::EnableDrawLayer(DRAWLAYER_T layer)
{
}

void Frame::SetClipState(bool bClip)
{
	if (bClip)
	{
		m_Style |= FRMSTYLE_CLIPSELF;
	}
	else
	{
		m_Style &= ~FRMSTYLE_CLIPSELF;
	}
}

void Frame::SetBlendAlpha(float alpha)
{
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		if (!strcmp(m_DrawRegions[i].pregion->GetTypeName(), "Texture"))
		{
			static_cast<::Texture *>(m_DrawRegions[i].pregion)->SetBlendAlpha(alpha);
		}
		else
		{
			static_cast<FontString *>(m_DrawRegions[i].pregion)->SetBlendAlpha(alpha);
		}
	}
}

void Frame::setUpdateTime( float fUpdateTime )
{
	m_UpdateTime = fUpdateTime;
}

float Frame::GetBackDropBlendAlpha()
{
	return m_BorderColor.a / 255.0f;
}

// 设置背景的透明度
void Frame::SetBackDropBlendAlpha(float alpha)
{
	assert((alpha >= 0.0f) && (alpha <= 1.0f));

	int iAlpha = FloatToInt(alpha * 255);

	m_BorderColor.a = iAlpha;
	m_EdgeColor.a = iAlpha;
}

void Frame::SetBackDropColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a/* =255 */)
{
	m_BorderColor = ColorQuad(r,g,b,a);
}

void Frame::OnBeginDrag(unsigned int millisecond, int dx, int dy)
{

}

bool Frame::ProcessMouseWheel()
{
	// 查看Frame子控件中是否有Slider
	if (hasScriptsEvent(SC_ONMOUSEWHEEL))
	{
		return true;
	}
	else
	{
		for ( size_t i = 0, size = m_Children.size(); i < size; ++i )
		{
			if ( stricmp( m_Children[i]->GetTypeName(), "Slider" ) == 0 )
			{
				return true;
			}
		}
	}
	return false;
}

void Frame::setModalFrame( const char *str )
{
	m_strModalFrame = str;
}

/*
//-----------------------------------------------------------------------------
// frame lua function
//-----------------------------------------------------------------------------
//注册客户端事件
DECLARE_CLASS_TO_SCRIPT_EX(Frame, LayoutFrame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)
DECLARE_MEM_FUNC_TO_SCRIPT(Frame, RegisterEvent, SVT_EMPTY, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Frame, SetFrameLevel, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Frame, GetFrameLevel, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Frame, GetTextExtentWidth, SVT_INT, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Frame, SetClipState, SVT_EMPTY, SVT_BOOL);
*/
