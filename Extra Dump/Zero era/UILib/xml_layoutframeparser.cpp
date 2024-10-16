#pragma warning( disable : 4996 )

#include "xml_layoutframeparser.h"
#include "ui_frame.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UILayoutframe XML Parser
//-----------------------------------------------------------------------------
XMLLayoutFrameParser::XMLLayoutFrameParser()
{
}

XMLLayoutFrameParser::~XMLLayoutFrameParser()
{
}


bool XMLLayoutFrameParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	m_pDestLayoutFrame = static_cast<LayoutFrame *>(pdestobj);

	XMLUIObjectParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);
	
	if (pNode.attribToString("hidden"))
	{
		if (stricmp(pNode.attribToString("hidden"), "true"))
		{
			m_pDestLayoutFrame->DrawShow(true);
		}
		else
		{
			m_pDestLayoutFrame->DrawShow(false);
		}
	}

	if (pNode.attribToString("name"))
	{
		std::string strFrameName = pNode.attribToString("name");
		m_pDestLayoutFrame->SetName(strFrameName.c_str());
	}

	m_pDestLayoutFrame->m_bInputTransparent = pNode.attribToBool("input_transparent");

	//if (pNode.attribToString("frameStrata"))
	//{
	//	FrameStrataParser(pNode.attribToString("frameStrata"));
	//}else
	//{
	//	if (m_pDestLayoutFrame->m_pParent)
	//	{
	//		m_pDestLayoutFrame->m_FrameStrata = m_pDestLayoutFrame->m_pParent->m_FrameStrata;
	//	}
	//	else
	//	{
	//		m_pDestLayoutFrame->m_FrameStrata = FS_MEDIUM;
	//	}
	//}

	bool bSize = false;
	bool bAnchor = false;
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if(stricmp(pChildNode.getName(), "Size") == 0)
		{
			m_pDestLayoutFrame->SetLayOutSize(SizeParser(pChildNode));
			bSize = true;
		}

		if(stricmp(pChildNode.getName(), "Anchors") == 0)
		{
			std::string nodename;
			Frame *pFrame = static_cast<Frame *>(pdestobj);
			if (pFrame->m_pParent)
				nodename = pFrame->m_pParent->GetName();
			else
				nodename = "";

			m_pDestLayoutFrame->AddAnchor(AnchorsParser(pChildNode, nodename, pdestobj, bTemplate));
			bAnchor = true;
		}
		pChildNode = pNode.iterateChild(pChildNode);
	}

	//如果没有设置size属性,并且size为0, 默认设置为父结点的大小
	if (!bSize && (m_pDestLayoutFrame->GetSize().GetX() == 0))
	{
		if (m_pDestLayoutFrame->m_pParent)
			m_pDestLayoutFrame->SetLayOutSize(m_pDestLayoutFrame->m_pParent->GetSize());
		else
		{
			LayoutDim dim;
			m_pDestLayoutFrame->SetLayOutSize(dim);
			//assert(0 && "Some Objects has not correct size");
		}
	}

	//如果没有设置anchor属性,并且m_RelFrame为空, 默认设置为父结点的anchor
	if (!bAnchor && (m_pDestLayoutFrame->m_Anchor[0].m_RelFrame.empty()))
	{
		if (m_pDestLayoutFrame->m_pParent)
			m_pDestLayoutFrame->m_Anchor[0] = m_pDestLayoutFrame->m_pParent->m_Anchor[0];
		else
		{
			LayoutAnchor anchor;
			m_pDestLayoutFrame->m_Anchor[0] = anchor;
			//assert(0 && "Some Objects has not correct anchor");
		}
	}

	return true;
}

LayoutDim XMLLayoutFrameParser::SizeParser(XMLNode pNode)
{
	LayoutDim ld;

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if(stricmp(pChildNode.getName(), "AbsDimension") == 0)
		{
			int a = pChildNode.attribToInt("y");
			ld.SetAbsDim((int)(pChildNode.attribToInt("x")) , (int)(pChildNode.attribToInt("y")));
		}
		else if(stricmp(pChildNode.getName(), "RelDimension") == 0)
		{
			float x = pChildNode.attribToFloat("x");
			float y = pChildNode.attribToFloat("y");
			assert(x<=1.0f && x>=-1.0f && y<=1.0f && y>=-1.0f);
			ld.SetRelDim(x, y);
		}
		else
		{
			return ld;
		}
		pChildNode = pNode.iterateChild(pChildNode);
	}

	return ld;
}

LayoutAnchor XMLLayoutFrameParser::AnchorsParser(XMLNode pNode, const std::string nodename, UIObject *pdestobj, bool bTemplate)
{
	LayoutAnchor la;

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if(stricmp(pChildNode.getName(), "Anchor") == 0)
		{
			FRAMEPOINT_T fpMypt = FP_TOPLEFT;
			FRAMEPOINT_T fpRelpt = FP_TOPLEFT;

			if (pChildNode.attribToString("point") != NULL)
			{
				for (int i=0; i<FP_MAXPOINTPOS; ++i)
				{
					if (stricmp(pChildNode.attribToString("point"), s_FramePoint[i]) == 0)
					{
						fpMypt = static_cast<FRAMEPOINT_T>(i);
						la.m_MyPoint = fpMypt;
						break;
					}
				}
			}

			std::string strRelFrame;
			if (pChildNode.attribToString("relativeTo") != NULL)
			{
				std::string strTemp = pChildNode.attribToString("relativeTo");

				la.SetRelFrame(strTemp);

				LayoutFrame *p = static_cast<LayoutFrame *>(pdestobj);
				if ((strcmp(pChildNode.attribToString("relativeTo"), "$parent") != 0))
				{
					if (p->m_pParent)
					{
						if(strcmp(pChildNode.attribToString("relativeTo"), p->m_pParent->GetName()) != 0)
						{
							LayoutFrame *pRelFrame = static_cast<LayoutFrame *>(g_pFrameMgr->FindLayoutFrame(strTemp));
							if (pRelFrame)
							{
								pRelFrame->m_RelFrames.push_back(p);
							}
						}
					}
					else
					{
						LayoutFrame *pRelFrame = static_cast<LayoutFrame *>(g_pFrameMgr->FindLayoutFrame(strTemp));
						if (pRelFrame)
						{
							pRelFrame->m_RelFrames.push_back(p);
						}
					}
				}
			}else
			{
				la.SetRelFrame("$parent");
			}

			if (pChildNode.attribToString("relativePoint") != NULL)
			{
				for (int i=0; i<FP_MAXPOINTPOS; ++i)
				{
					if (stricmp(pChildNode.attribToString("relativePoint"), s_FramePoint[i]) == 0)
					{
						fpRelpt = static_cast<FRAMEPOINT_T>(i);
						la.m_RelPoint = fpRelpt;
						break;
					}
				}
			}else
			{
				fpRelpt = fpMypt;
				la.m_RelPoint = fpMypt;
			}

			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				if (stricmp(pGrandChild.getName(), "Offset") == 0)
				{
					la.SetPoint(fpMypt, fpRelpt, SizeParser(pGrandChild));
				}

				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
			return la;
		}
		pChildNode = pNode.iterateChild(pChildNode);
	}

	return la;
}

void XMLLayoutFrameParser::FrameStrataParser(const char *pstr)
{
	if (pstr)
	{
		for (int i=0; i<FS_MAX_STRATA; ++i)
		{
			if (stricmp(pstr, s_FrameStrata[i]) == 0)
			{
				m_pDestLayoutFrame->SetFrameStrata(static_cast<FRAMESTRATA_T>(i));
			}
		}
	}
	else
	{
		assert(0);
	}
}