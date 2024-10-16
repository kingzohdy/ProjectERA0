#include "xml_checkbuttonparser.h"
#include "xml_textureparser.h"
#include "ui_framemgr.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML CheckButton Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLCheckButtonParser::XMLCheckButtonParser()
{
}

XMLCheckButtonParser::~XMLCheckButtonParser()
{
}

bool XMLCheckButtonParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLButtonParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestCheckButton = static_cast<CheckButton *>(pdestobj);

	std::string nodename = pdestobj->m_Name;

	if (pNode.attribToString("checked"))
	{
		bool bChecked = pNode.attribToBool("checked");
		m_pDestCheckButton->m_bChecked = bChecked;
	}

	LayoutDim size = static_cast<LayoutFrame *>(pdestobj)->m_Size;
	size.SetAbsDim((int)size.GetY(), (int)size.GetY());

	//m_pDestCheckButton->AddTexture(DL_ARTWORK, m_pDestCheckButton->m_pNormalRegion);
	m_pDestCheckButton->m_pNormalRegion->m_Size = size;
	//m_pDestCheckButton->SetNormalTexture("ui.Common.checkbutton_disabled", "blend");

	//m_pDestCheckButton->AddTexture(DL_ARTWORK, m_pDestCheckButton->m_pOverlayRegion);
	m_pDestCheckButton->m_pOverlayRegion->m_Size = size;
	//m_pDestCheckButton->SetPushedTexture("ui.Common.checkbutton_enabled", "blend");

	//if (m_pDestCheckButton->m_pCheckRegion)
	//{
	//	m_pDestCheckButton->m_pCheckRegion->SetName((nodename + "CheckRegion").c_str());
	//}
	//else
	//{
	//	m_pDestCheckButton->m_pCheckRegion = static_cast<Texture *>(g_pFrameMgr->CreateObject("Texture", (nodename + "CheckRegion").c_str()));
	//}

	//LayoutDim size = static_cast<LayoutFrame *>(pdestobj)->m_Size;
	//size.SetAbsDim((int)size.GetY(), (int)size.GetY());

	//m_pDestCheckButton->m_pCheckRegion->m_Size = size;
	//m_pDestCheckButton->m_pCheckRegion->m_Anchor[0].m_MyPoint = m_pDestCheckButton->m_pCheckRegion->m_Anchor[0].m_RelPoint = FP_TOPLEFT;
	//m_pDestCheckButton->m_pCheckRegion->m_Anchor[0].m_Offset.SetRelDim(0,0);
	//if (bTemplate)
	//{
	//	m_pDestCheckButton->m_pCheckRegion->m_Anchor[0].m_RelFrame = "$parent";
	//}
	//else
	//{
	//	m_pDestCheckButton->m_pCheckRegion->m_Anchor[0].m_RelFrame = nodename;
	//}

	//m_pDestCheckButton->AddTexture(DL_ARTWORK, m_pDestCheckButton->m_pCheckRegion);
	//m_pDestCheckButton->m_hDisabledTex = g_pDisplay->CreateTexture("ui.checkbutton_disabled", UIDRAW_OPAQUE);
	//m_pDestCheckButton->m_hEnabledTex = g_pDisplay->CreateTexture("ui.checkbutton_enabled", UIDRAW_OPAQUE);

	//m_pDestCheckButton->SetCurrentTexture(m_pDestCheckButton->m_hDisabledTex);

	//Set fontstring Position
	size = static_cast<LayoutFrame *>(pdestobj)->m_Size;
	size.SetAbsDim((int)(size.GetX()-size.GetY()), (int)size.GetY());
	m_pDestCheckButton->m_pNormalFontString->m_Size = size;

	int offsetx = (int)m_pDestCheckButton->m_pNormalFontString->m_Anchor[0].m_Offset.GetX() + (int)(size.GetY() * 0.86); //0.86 = 800/1024 * 1.1
	int offsety = (int)m_pDestCheckButton->m_pNormalFontString->m_Anchor[0].m_Offset.GetX();

	//暂时解决，后面再调试 20080923
	m_pDestCheckButton->m_pNormalFontString->m_Anchor[0].m_Offset.SetAbsDim(10, offsety);

	return true;
}