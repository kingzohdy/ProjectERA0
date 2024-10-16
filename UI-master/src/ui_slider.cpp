#include "ui_slider.h"

#include "OgreScriptLuaVM.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// Slider control
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(Slider)

Slider::Slider() : m_pThumbRegion(NULL)
{
	m_fMinValue = 0.0f;
	m_fMaxValue = 0.0f;
	m_fValue = 0.0f;
	m_fValueStep = 1.0f;
	m_Orientation = ORIENTATION_HORIZONTAL;

	m_fLastFrameValue = 0.0f;
}

Slider::~Slider()
{
	OGRE_RELEASE(m_pThumbRegion);
}

void Slider::CopyMembers(Slider* pdest)
{
	if (pdest)
	{
		Frame::CopyMembers(pdest);

		if(m_pThumbRegion)
		{
			pdest->m_pThumbRegion = static_cast<::Texture *>(m_pThumbRegion->CreateClone());
			//pdest->AddTexture(DL_ARTWORK, pdest->m_pThumbRegion);
		}

		pdest->m_fMinValue = m_fMinValue;
		pdest->m_fMaxValue = m_fMaxValue;
		pdest->m_fValue = m_fValue;
		pdest->m_fValueStep = m_fValueStep;
		pdest->m_Orientation = m_Orientation;
		pdest->m_fLastFrameValue = m_fLastFrameValue;
	}
}

void Slider::FillDrawItems()
{
	if (m_bShow)
	{
		if (m_pParent)
		{
			m_iDrawLevel = m_pParent->m_iDrawLevel + 1;
		}

		g_pFrameMgr->AddDrawItems(static_cast<LayoutFrame *>(this));

		Frame::FillChildren();
	}
}

void Slider::Draw()
{
	Frame::Draw();

	float ratio = 0.0f;
	if ( m_fValue >= m_fMaxValue )
	{
		ratio = 1.0f;
	}
	else
	{
		ratio = ( m_fValue - m_fMinValue ) / ( m_fMaxValue - m_fMinValue );
	}

	float offset = 0.0f;
	// 图片的可移动范围实质上是在0 - (size - thumbTex.Size())之间移动,而不是0-this_>GetSize()之间
	if ( m_Orientation == ORIENTATION_HORIZONTAL )
	{
		offset = ratio * ( GetSize().GetX() - m_pThumbRegion->GetSize().GetX() );	//移动范围的计算修正
		m_pThumbRegion->SetPoint( "left", m_Name.c_str(), "left", FloatToInt(offset), 0 );
	}
	else
	{
		offset = ratio * ( GetSize().GetY() - m_pThumbRegion->GetSize().GetY() );	//移动范围的计算修正
		m_pThumbRegion->SetPoint( "top", m_Name.c_str(), "top", 0, FloatToInt( offset ) );
	}

	m_pThumbRegion->Draw();
}

TiXmlElement *Slider::Save(TiXmlElement *pElement)
{
	TiXmlElement *pSlider = Frame::Save(pElement);

	//orientation
	if (m_Orientation != ORIENTATION_HORIZONTAL)
	{
		pSlider->SetAttribute("orientation", "VERTICAL");
	}

	pSlider->SetDoubleAttribute("minValue", m_fMinValue);
	pSlider->SetDoubleAttribute("maxValue", m_fMaxValue);
	pSlider->SetDoubleAttribute("valueStep", m_fValueStep);
	pSlider->SetDoubleAttribute("defaultValue", m_fValue);

	////ThumbTexture
	//if (m_pThumbRegion)
	//{
	//	TiXmlElement *pThumbTexture = m_pThumbRegion->Save(pElement);
	//	
	//	pThumbTexture->SetValue("ThumbTexture");
	//}

	return pSlider;
}

int Slider::OnInputMessage(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_MOUSEMOVE:
		{
			if ( m_State & FRMSTATE_CLICKDOWN )
			{
				if ( m_Orientation == ORIENTATION_HORIZONTAL )
				{
					if ( m_fLastMouseValue == event.mouse.x )
					{
						return 0;
					}

					float width = float(m_AbsRect.m_Right - event.mouse.x);

					if ( width < 0 )
					{
						width = 0;
					}
					else if ( width > m_AbsRect.getWidth() )
					{
						width = float(m_AbsRect.getWidth());
					}

					if (m_AbsRect.getWidth() > 0)
					{
						m_fValue = width/m_AbsRect.getWidth() * (m_fMinValue - m_fMaxValue) + m_fMaxValue;
						m_fValue = FloatToInt(m_fValue/m_fValueStep) * m_fValueStep; //取步长的整数倍，四舍五入
					}
				}else
				{
					if( m_fLastMouseValue == event.mouse.y )
					{
						return 0;
					}

					float height = float(m_AbsRect.m_Bottom - event.mouse.y);

					if(height<0)
					{
						height = 0;
					}
					else if(height > m_AbsRect.getHeight())
					{
						height = float(m_AbsRect.getHeight());
					}

					if (m_AbsRect.getHeight() > 0)
					{
						m_fValue = height/m_AbsRect.getHeight() * (m_fMinValue - m_fMaxValue) + m_fMaxValue;
					}
				}
				g_pFrameMgr->m_pDragFrame = this;
				//CallScript(SC_ONVALUECHANGED);
			} 
		}
		return 0;

	case GIE_LBTNUP:
		{
			if( m_State & FRMSTATE_CLICKDOWN )
			{	
				m_State &= ~FRMSTATE_CLICKDOWN;
				g_pFrameMgr->m_pDragFrame = NULL;
				//CallSafeScript(SCRIPT_FUNC_ON_CLICK);

				/*if (!m_pScriptStr[SC_ONVALUECHANGED].empty())
				{
				CallScript(SC_ONVALUECHANGED, "i", FloatToInt(m_fValue));
				}	*/
			}
			return 0;
		}

	case GIE_LBTNDOWN:
		{
			//g_pFrameMgr->SetFocusFrame(this);

			m_State |= FRMSTATE_CLICKDOWN;
			m_fLastMouseValue = event.mouse.y;
			if ( m_Orientation == ORIENTATION_HORIZONTAL )
			{
				m_fLastMouseValue = event.mouse.x;
			}

			if ( ( event.mouse.x > m_pThumbRegion->m_AbsRect.m_Left ) &&
					( event.mouse.x < m_pThumbRegion->m_AbsRect.m_Right	) && 
					( event.mouse.y > m_pThumbRegion->m_AbsRect.m_Top  ) && 
						( event.mouse.y < m_pThumbRegion->m_AbsRect.m_Bottom ) )
			{
				return 0;
			}

			if ( m_Orientation == ORIENTATION_HORIZONTAL )
			{
				float width = float( event.mouse.x - m_AbsRect.m_Left );

				if ( width < 0 )
				{
					width = 0;
				}
				else if ( width > m_AbsRect.getWidth() )
				{
					width = float( m_AbsRect.getWidth() );
				}

				if ( m_AbsRect.getWidth() > 0 )
				{
					m_fValue = width / m_AbsRect.getWidth() * ( m_fMaxValue - m_fMinValue );
				}
			}
			else
			{
				float height = float( event.mouse.y - m_AbsRect.m_Top );

				if ( height < 0 )
				{
					height = 0;
				}
				else if ( height > m_AbsRect.getHeight() )
				{
					height = float( m_AbsRect.getHeight() );
				}

				if (m_AbsRect.getHeight() > 0)
				{
					m_fValue = height / m_AbsRect.getHeight() * ( m_fMaxValue - m_fMinValue );
				}
			}
		}
		return 0;

	case GIE_MOUSEWHEEL:
		//return 1;
		{
			if (hasScriptsEvent(SC_ONMOUSEWHEEL))
			{
				CallScript( SC_ONMOUSEWHEEL, "i", (int)event.wheel.dist );
			}
		}
		return 0;

	default:
		{
			return Frame::OnInputMessage(event);
		}
	}

	return 1;
}

void Slider::SetThumbTexture(std::string path, Ogre::UIBlendMode drawmode)
{
	m_pThumbRegion->SetTexture(path.c_str());
	m_pThumbRegion->SetBlendMode(drawmode);
}

void Slider::SetValue(float value)
{
	m_fValue = value; 
	if (m_fValue > m_fMaxValue)
	{
		m_fValue = m_fMaxValue;
	}
	else if (m_fValue < m_fMinValue)
	{
		m_fValue = m_fMinValue;
	}
}

float Slider::GetValue()
{
	//return m_fLastFrameValue; 
	return FloatToInt( m_fValue / m_fValueStep ) * m_fValueStep;
}

float Slider::GetLastValue()
{
	return m_fLastFrameValue;
}

void Slider::SetMinValue(float value)
{
	m_fMinValue = value;
	m_fMaxValue = m_fMaxValue > value ? m_fMaxValue : value;
}

float Slider::GetMinValue()
{
	return m_fMinValue; 
}

void Slider::SetMaxValue(float value) 
{
	m_fMaxValue = value;
	m_fMinValue = m_fMinValue < value ? m_fMinValue : value;
}

float Slider::GetMaxValue()
{
	return m_fMaxValue; 
}

void Slider::SetValueStep(float value)
{ 
	m_fValueStep = value; 
}

float Slider::GetValueStep()
{ 
	return m_fValueStep; 
}

void Slider::UpdateSelf(float deltatime)
{
	if ( !m_bShow )
	{
		return;
	}
	

	Frame::UpdateSelf( deltatime );

	assert( m_fValueStep != 0 );

	// 将移动的步数精确到整数次
	float scale = (float)FloatToInt( m_fValue / m_fValueStep );
	
	// 要保证最后一次移动总位移总是精确到整数的浮点数
	if ( m_fLastFrameValue != ( FloatToInt( scale ) * m_fValueStep ) )
	{
		if ( hasScriptsEvent(SC_ONVALUECHANGED) )
		{
			CallScript( SC_ONVALUECHANGED, "i", FloatToInt( m_fLastFrameValue ) );
			m_fLastFrameValue  =  FloatToInt( scale ) * m_fValueStep; //取步长的整数倍，四舍五入
		}
	}
}

/*
//-----------------------------------------------------------------------------
// Slider lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(Slider, Frame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//Slider
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, SetValue, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, GetValue, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, SetMinValue, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, GetMinValue, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, SetMaxValue, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, GetMaxValue, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, SetValueStep, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Slider, GetValueStep, SVT_FLOAT);

*/
