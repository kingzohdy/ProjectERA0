#include "xml_frameparser.h"
#include "ui_button.h"
#include "xml_buttonparser.h"
#include "xml_fontstringparser.h"
#include "xml_textureparser.h"
#include "xml_modelviewparser.h"
#include "xml_drawlineframeparser.h"
#include "ui_framemgr.h"
#include "ui_gameui.h"
#include "OgreStringUtil.h"
using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML Frame Parser
//-----------------------------------------------------------------------------
XMLFrameParser::XMLFrameParser()
{
}

XMLFrameParser::~XMLFrameParser()
{
}

bool XMLFrameParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLLayoutFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestFrame = static_cast<Frame *>(pdestobj);

	if (pNode.attribToString("id"))
	{
		m_pDestFrame->SetClientID(pNode.attribToInt("id"));
	}

	if (pNode.attribToString("movable"))
	{
		m_pDestFrame->m_bMovable = pNode.attribToBool("movable");
	}

	if (pNode.attribToString("frameStrata"))
	{
		FrameStrataParser(pNode.attribToString("frameStrata"));
	}else
	{
		//GameUI初始化的时候会检查FS_NO_DEFINED
		m_pDestFrame->SetFrameStrata(FS_NO_DEFINED);
	}

	if (pNode.attribToString("framelevel"))
	{
		m_pDestFrame->m_iFrameLevel = pNode.attribToInt("framelevel") * DEFAULT_FRAME_MAX_DEPTH;
		
		g_pFrameMgr->m_iCurMaxFrmLevel = m_pDestFrame->m_iFrameLevel > g_pFrameMgr->m_iCurMaxFrmLevel?m_pDestFrame->m_iFrameLevel:g_pFrameMgr->m_iCurMaxFrmLevel;
	}

	if (pNode.attribToString("toplevel"))
	{
		m_pDestFrame->m_bToplevel = pNode.attribToBool("toplevel");
	}

	if (pNode.attribToString("clipped"))
	{
		m_pDestFrame->SetClipState(pNode.attribToBool("clipped"));
	}

	if (pNode.attribToString("modalFrame"))
	{
		std::string strFrameName = pNode.attribToString("modalFrame");
		m_pDestFrame->setModalFrame(strFrameName.c_str());
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "Backdrop") == 0)
		{
			if (!BackDropParser(m_pDestFrame, pChildNode))
			{
				assert(0 && "Backdrop parse error!");
				return false;
			}
		}

		if (stricmp(pChildNode.getName(), "Layers") == 0)
		{
			if (!LayersParser(pChildNode, bTemplate))
			{
				assert(0 && "Layers parse error");
				return false;
			}
		}
		
		if( stricmp( pChildNode.getName(), "OnShowSound" ) == 0 )
		{
			if (pChildNode.attribToString("file"))
			{
				m_pDestFrame->m_strShowSoundPath = pChildNode.attribToString("file");
			}
		}

		if( stricmp( pChildNode.getName(), "OnHideSound" ) == 0 )
		{
			if (pChildNode.attribToString("file"))
			{
				m_pDestFrame->m_strHideSoundPath = pChildNode.attribToString("file");
			}
		}

		//if (stricmp(pChildNode.getName(), "Accel") == 0)
		//{
		//	AccelItem accel;
		//	sprintf(accel.accel_script, pChildNode.attribToString("funcname"));
		//	std::string str = pChildNode.attribToString("key");
		//	accel.accel_key = (char)(str.c_str()[0]);
		//	//accel.modifier_alt = pChildNode.attribToBool("alt");
		//	accel.modifier_alt = false;
		//	accel.modifier_ctrl = false;
		//	accel.modifier_shift = false;
		//	accel.accel_type = UI_KEY_PRESS;

		//	//accel.modifier_alt = pChildNode.attribToString("alt");
		//	//accel.modifier_ctrl = pChildNode.attribToString("ctrl");
		//	//accel.modifier_shift = pChildNode.attribToString("shift");
		//	//accel.accel_type=UI_KEY_PRESS;

		//	m_pDestFrame->m_Accels.push_back(accel);
		//}

		if (stricmp(pChildNode.getName(), "Frames") == 0)
		{
			if (!FrameParserRecursive(m_pDestFrame, pChildNode))
			{
				return false;
			}
		}
		
		if (stricmp(pChildNode.getName(), "Scripts") == 0)
		{
			if (!LoadFrameScript(m_pDestFrame, pChildNode))
			{
				assert(0 && "Scripts parse error");
				return false;
			}

			//if (!m_pDestFrame->m_pScriptStr[SC_ONEVENT].empty())
			//{
			//	m_pDestFrame->CallScript(SC_ONLOAD);
			//}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}


bool XMLFrameParser::FrameParserRecursive(Frame *pFrame, XMLNode pNode)
{
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "Cooldown") == 0)
		{
			static_cast<Button *>(pFrame)->m_bCooldown = false;
			pChildNode = pNode.iterateChild(pChildNode);
			continue;
		}

		UIObject *pdestobj = NULL;
		XMLUIObjectParser *parser = NULL;

		XMLManager::CreateObjectByType(pChildNode, pdestobj, parser, m_pDestFrame->GetName());

		if (!pdestobj)
		{
			delete(parser);
			return false;//创建失败
		}

		//static_cast<LayoutFrame *>(pdestobj)->SetFrameStrata(pFrame->GetFrameStrata());
		if (!parser->LoadUIObjectParam(pdestobj, pChildNode, pdestobj->m_bTemplate))
		{
			delete parser;
			pdestobj->release();
			return false;
		}

		if(pdestobj->m_bTemplate) g_pFrameMgr->RegisterObject(pdestobj);
		else pFrame->AddChildFrame(static_cast<Frame *>(pdestobj));

		pChildNode = pNode.iterateChild(pChildNode);

		pdestobj->release();
		delete(parser);
	}

	return true;
}

bool XMLFrameParser::BackDropParser(Frame *pFrame, XMLNode pNode)
{
	if (pNode.attribToString("bgFile"))
	{
		wchar_t buff[1024];
		StringUtil::UTF8ToUnicode(buff , 1024 , pNode.attribToString("bgFile"));
		char buff2[1024];
		StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);				
		pFrame->m_BorderTexName = buff2;

		pFrame->m_hBorderTex = g_pDisplay->CreateTexture( pFrame->m_BorderTexName.c_str(), BLEND_ALPHABLEND);
	}

	if (pNode.attribToString("edgeFile"))
	{
		wchar_t buff[1024];
		StringUtil::UTF8ToUnicode(buff , 1024 , pNode.attribToString("edgeFile"));
		char buff2[1024];
		StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);				
		pFrame->m_EdgeTexName = buff2;

		pFrame->m_hEdgeTex = g_pDisplay->CreateTexture(pFrame->m_EdgeTexName.c_str(), BLEND_ALPHABLEND);
	}

	if (pNode.attribToString("tile"))
	{
		pFrame->m_bTile = pNode.attribToBool("tile");
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "EdgeSize") == 0)
		{
			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				if (stricmp(pGrandChild.getName(), "AbsValue") == 0)
				{
					if (pGrandChild.attribToString("val"))
					{
						pFrame->m_iEdgeSize = pGrandChild.attribToInt("val");
					}
				}

				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
		}

		if (stricmp(pChildNode.getName(), "TileSize") == 0)
		{
			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				if (stricmp(pGrandChild.getName(), "AbsValue") == 0)
				{
					if (pGrandChild.attribToString("val"))
					{
						pFrame->m_iTileSize = pGrandChild.attribToInt("val");
					}
				}
				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
		}

		if (stricmp(pChildNode.getName(), "BackgroundInsets") == 0)
		{
			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				if (stricmp(pGrandChild.getName(), "AbsInset") == 0)
				{
					pFrame->m_BackgroundInsets = RectInt(5, 6, 6, 5);//给个默认值避免出错
					if (pGrandChild.attribToString("left"))
					{
						pFrame->m_BackgroundInsets.m_Left = pGrandChild.attribToInt("left");
					}
					if (pGrandChild.attribToString("right"))
					{
						pFrame->m_BackgroundInsets.m_Right = pGrandChild.attribToInt("right");
					}
					if (pGrandChild.attribToString("top"))
					{
						pFrame->m_BackgroundInsets.m_Top = pGrandChild.attribToInt("top");
					}
					if (pGrandChild.attribToString("bottom"))
					{
						pFrame->m_BackgroundInsets.m_Bottom = pGrandChild.attribToInt("bottom");
					}
				}
				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}
	return true;
}

bool XMLFrameParser::LayersParser(XMLNode pNode, bool bTemplate)
{
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "Layer") == 0)
		{

			DRAWLAYER_T level = DL_ARTWORK;
			if (pChildNode.attribToString("level"))
			{
				for (size_t i=0; i<DL_NUMLAYERS; i++)
				{
					if (stricmp(pChildNode.attribToString("level"), s_Drawlayer[i]) == 0)
						level = DRAWLAYER_T(i);
				}
			}

			XMLNode pGrandChild = pChildNode.iterateChild();
			while(!pGrandChild.isNull())
			{
				UIObject *pdestobj = NULL;
				XMLUIObjectParser *parser = NULL;

				XMLManager::CreateObjectByType(pGrandChild, pdestobj, parser, "");

				if (!pdestobj)
				{
					delete(parser);
					return false;//创建失败
				}
				
				if (stricmp(pGrandChild.getName(), "Texture") == 0)
				{
					XMLTextureParser* pTexParser = new XMLTextureParser;

					m_pDestFrame->AddTexture(level, static_cast<::Texture *>(pdestobj));
					if (!pTexParser->LoadUIObjectParam(pdestobj, pGrandChild, bTemplate))
					{
						assert(0);
						return false;
					}

					delete pTexParser;
				}
				else if ( stricmp( pGrandChild.getName(), "DrawLineFrame" ) == 0 )
				{
					XMLDrawLineFrameParser* pLineParser = new XMLDrawLineFrameParser;

					m_pDestFrame->AddLineFrame( level, static_cast<DrawLineFrame*>( pdestobj ) );
					if ( !pLineParser->LoadUIObjectParam( pdestobj, pGrandChild, bTemplate ) )
					{
						assert( false );
						return false;
					}

					delete pLineParser;
				}
				else if (stricmp(pGrandChild.getName(), "FontString") == 0)
				{
					XMLFontStringParser *pTexParser = new XMLFontStringParser;

					m_pDestFrame->AddFontString(level, static_cast<FontString *>(pdestobj));
					if (!pTexParser->LoadUIObjectParam(pdestobj, pGrandChild, bTemplate))
					{
						assert(0 && "Layer Pars Error!");
						return false;
					}

					delete(pTexParser);
				}
				else if(stricmp(pGrandChild.getName(), "ModelView") == 0)
				{
					XMLModelViewParser *pTexParser = new XMLModelViewParser;

					m_pDestFrame->AddModelView(level, static_cast<ModelView *>(pdestobj));
					if (!pTexParser->LoadUIObjectParam(pdestobj, pGrandChild, bTemplate))
					{
						assert(0 && "Layer Pars Error!");
						return false;
					}

					delete(pTexParser);
				}
				else
				{
					assert(0 && "Layers child Frame type error");
				}

				pdestobj->release();
				delete(parser);

				pGrandChild = pChildNode.iterateChild(pGrandChild);
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}
	return true;
}