#include "xml_sliderparser.h"
#include "ui_framemgr.h"
#include "xml_fontstringparser.h"
#include "xml_textureparser.h"

using namespace Ogre;
//-----------------------------------------------------------------------------
// XML Slider Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLSliderParser::XMLSliderParser()
{
}

XMLSliderParser::~XMLSliderParser()
{
}

bool XMLSliderParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestSlider = static_cast<Slider *>(pdestobj);

	std::string nodename = pdestobj->m_Name;

	//添加Slider基本属性
	if (pNode.attribToString("orientation"))
	{
		if(!stricmp(pNode.attribToString("orientation"), "HORIZONTAL"))
		{
			m_pDestSlider->m_Orientation = ORIENTATION_HORIZONTAL;
		}else if(!stricmp(pNode.attribToString("orientation"), "VERTICAL"))
		{
			m_pDestSlider->m_Orientation = ORIENTATION_VERTICAL;
		}else
		{
			assert(0 && "Slider orientation wrong");
		}
	}

	if (pNode.attribToString("minValue"))
	{
		m_pDestSlider->SetMinValue(pNode.attribToFloat("minValue"));
	}

	if (pNode.attribToString("maxValue"))
	{
		m_pDestSlider->SetMaxValue(pNode.attribToFloat("maxValue"));
	}

	if (pNode.attribToString("valueStep"))
	{
		m_pDestSlider->SetValueStep(pNode.attribToFloat("valueStep"));
	}

	if (pNode.attribToString("defaultValue"))
	{
		m_pDestSlider->SetValue(pNode.attribToFloat("defaultValue"));
	}

	//添加必要部分
	if (m_pDestSlider->m_pThumbRegion)
	{
		m_pDestSlider->m_pThumbRegion->SetName("$parentThumbRegion");
		m_pDestSlider->m_pThumbRegion->m_Anchor[0].m_RelFrame = "$parent";
	}
	else
	{
		m_pDestSlider->m_pThumbRegion = static_cast<::Texture *>(g_pFrameMgr->CreateObject("Texture", (nodename + "ThumbRegion").c_str()));
		m_pDestSlider->m_pThumbRegion->m_Anchor[0].m_RelFrame = nodename;
	}

	m_pDestSlider->m_pThumbRegion->m_FrameStrata = m_pDestSlider->m_FrameStrata;
	
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		std::string name;
		if(pChildNode.attribToString("name"))
		{
			name = pChildNode.attribToString("name");
		}
		
		if (stricmp(pChildNode.getName(), "ThumbTexture") == 0)
		{
			XMLUIObjectParser *parser = NULL;
			parser = new XMLTextureParser;
			if (!parser->LoadUIObjectParam(m_pDestSlider->m_pThumbRegion, pChildNode, bTemplate))
			{
				return false;
			}

			m_pDestSlider->AddTexture(DL_ARTWORK, m_pDestSlider->m_pThumbRegion);

			delete(parser);
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}