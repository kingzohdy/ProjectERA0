
#include "xml_uimgr.h"
#include "ui_framemgr.h"
#include "ui_button.h"
#include "ui_layoutframe.h"
#include "xml_frameparser.h"
#include "xml_buttonparser.h"
#include "xml_editboxparser.h"
#include "xml_checkbuttonparser.h"
#include "xml_sliderparser.h"
#include "xml_scrollframeparser.h"
#include "xml_richtextparser.h"
#include "xml_modelviewparser.h"
#include "xml_drawlineframeparser.h"
#include "xml_multieditboxparser.h"
#include "xml_webbrowerframeparser.h"
#include "xml_textureparser.h"

#include "OgreScriptLuaVM.h"
#include "OgreStringUtil.h"
#include "OgreResourceManager.h"
#include "OgreDataStream.h"

#include <fstream>
#include <string>

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML Manager
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

XMLManager::XMLManager()
{
}

XMLManager::~XMLManager()
{
}

bool XMLManager::LoadTOCFile( const char *uires )
{
	if (uires && uires != "")
	{
		Ogre::DataStream *fp = ResourceManager::getSingleton().openFileStream(uires, true);
		if(fp == NULL)
		{
			return false;
		}

		char buffer[1024];
		while(!fp->eof())
		{
			fp->readLine(buffer, sizeof(buffer));
			std::string inbuf = buffer;

			size_t pos;
			pos = inbuf.find(".xml");
			if ((pos!=inbuf.npos) &&((inbuf.find("##"))>0))
			{
				if (!LoadUIFromXml(inbuf.c_str()))
				{
					std::string str = "\tload xml file error!\n\nFileName:";
					str += inbuf;
					MessageBox(NULL, str.c_str(), "Error", MB_OK);
					return false;
				}
			}

			pos = inbuf.find(".lua");
			if ((pos!=inbuf.npos) && ((inbuf.find("##"))>0))
			{
				if(!g_pUIScriptVM->callFile(inbuf.c_str()))
				{
					std::string str = "\tload lua file error!\n\nFileName:";
					str += inbuf;
					MessageBox(NULL, str.c_str(), "Error", MB_OK);

					return false;
				}
			}
		}

		delete fp;
		return true;

		
/*
		std::ifstream fin(uires);

		if(!fin.is_open())
		{
			return false;
		}

		while (!fin.eof())
		{
			std::string inbuf;
			getline(fin, inbuf, '\n');

			int pos;
			pos = inbuf.find(".xml");
			if ((pos>0) &&((inbuf.find("##"))>0))
			{
				if (!LoadUIFromXml(inbuf.c_str()))
				{
					std::string str = "\tload xml file error!\n\nFileName:";
					str += inbuf;
					MessageBox(NULL, str.c_str(), "Error", MB_OK);
					return false;
				}
			}

			pos = inbuf.find(".lua");
			if ((pos>0) && ((inbuf.find("##"))>0))
			{
				if(!g_pUIScriptVM->callFile(inbuf.c_str()))
				{
					std::string str = "\tload lua file error!\n\nFileName:";
					str += inbuf;
					MessageBox(NULL, str.c_str(), "Error", MB_OK);

					return false;
				}
			}
		}

		fin.close();

		return true;*/
	}

	return false;
}

void XMLManager::CreateObjectByType(XMLNode pNode, UIObject *&pdestobj, XMLUIObjectParser *&parser, std::string parentname)
{
	if (pNode.attribToString("name"))
	{
		std::string name = pNode.attribToString("name");

		assert(!name.empty() && "name is empty");
		if(stricmp(pNode.getName(), "Texture") == 0)
		{
			parser = new XMLTextureParser;
			pdestobj = CreateUIObjectFromXML("Texture", name.c_str(), pNode);
		}
		else if ( stricmp( pNode.getName(), "DrawLineFrame") == 0 )
		{
			parser		= new XMLDrawLineFrameParser;
			pdestobj	= CreateUIObjectFromXML( "DrawLineFrame", name.c_str(), pNode );
		}
		else if(stricmp(pNode.getName(), "FontString") == 0)
		{
			parser = new XMLFrameParser;
			pdestobj = CreateUIObjectFromXML("FontString", name.c_str(), pNode);
		}
		else if(stricmp(pNode.getName(), "ModelView") == 0)
		{
			parser = new XMLModelViewParser;
			pdestobj = CreateUIObjectFromXML("ModelView", name.c_str(), pNode);
		}
		else if(stricmp(pNode.getName(), "Frame") == 0)
		{
			parser = new XMLFrameParser;
			pdestobj = CreateUIObjectFromXML("Frame", name.c_str(), pNode);
		}else if(stricmp(pNode.getName(), "Button") == 0)
		{
			parser = new XMLButtonParser;
			pdestobj = CreateUIObjectFromXML("Button", name.c_str(), pNode);
		}else if(stricmp(pNode.getName(), "EditBox") == 0)
		{
			parser = new XMLEditBoxParser;
			pdestobj = CreateUIObjectFromXML("EditBox", name.c_str(), pNode);
		}
		else if(stricmp(pNode.getName(), "CheckButton") == 0)
		{
			parser = new XMLCheckButtonParser;
			pdestobj = CreateUIObjectFromXML("CheckButton", name.c_str(), pNode);
		}
		else if(stricmp(pNode.getName(), "Slider") == 0)
		{
			parser = new XMLSliderParser;
			pdestobj = CreateUIObjectFromXML("Slider", name.c_str(), pNode);
		}
		else if(stricmp(pNode.getName(), "ScrollFrame") == 0)
		{
			parser = new XMLScrollFrameParser;
			pdestobj = CreateUIObjectFromXML("ScrollFrame", name.c_str(), pNode);
		}
		else if(stricmp(pNode.getName(), "RichText") == 0)
		{
			parser = new XMLRichTextParser;
			pdestobj = CreateUIObjectFromXML("RichText", name.c_str(), pNode);
		}
		else if ( stricmp( pNode.getName(), "MultiEditBox") == 0 )
		{
			parser		= new XMLMultiEditBoxParser;
			pdestobj	= CreateUIObjectFromXML( "MultiEditBox", name.c_str(), pNode );
		}
		else if( stricmp( pNode.getName(), "WebBrowerFrame" ) == 0 )
		{
			parser		= new XMLWebBrowerFrameParser;
			pdestobj	= CreateUIObjectFromXML( "WebBrowerFrame", name.c_str(),pNode );
		}
		else
		{
			assert(0 && "Wrong Type!");
			return;
		}
	}
	else
	{
		assert(0 && "Res has no name!");
	}
}

void XMLManager::LoadUICursor(Ogre::XMLNode pNode)
{
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
        wchar_t buff[1024];
        StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
        char buff2[1024];
        StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);    
		int time = pChildNode.attribToInt("time");
		int row  = pChildNode.attribToInt("row");
		int col  = pChildNode.attribToInt("col");

		g_pFrameMgr->m_pCurCursor->addCursor(buff2, pChildNode.getName(), time, row, col);

		//if (stricmp(pChildNode.getName(), "normal") == 0)
		//{
		//	g_pFrameMgr->AddCursor(pChildNode.attribToString("file"), CURSOR_NORMAL);
		//}

		//if (stricmp(pChildNode.getName(), "click") == 0)
		//{
		//	g_pFrameMgr->AddCursor(pChildNode.attribToString("file"), CURSOR_POINT);
		//}

		//if (stricmp(pChildNode.getName(), "attack") == 0)
		//{
		//	g_pFrameMgr->AddCursor(pChildNode.attribToString("file"), CURSOR_ATTACK);
		//}
		pChildNode = pNode.iterateChild(pChildNode);
	}
}

void XMLManager::LoadUIFont(Ogre::XMLNode pNode)
{
	UIFont font;
	font.h = Ogre::HFONT(0);
	font.name = "";
	font.type = "";
	font.height = 0;
	font.extType = FONTTYPE_NORMAL;

	if (pNode.attribToString("name"))
	{
		font.name = pNode.attribToString("name");
	}
	
	if (pNode.attribToString("type"))
	{
		char *p = XMLUIObjectParser::Utf8ToGb(pNode.attribToString("type"));
		font.type = p;
		delete(p);
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "FontHeight") == 0)
		{
			if (pChildNode.attribToString("value"))
			{
				font.height = pChildNode.attribToInt("value");
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}
	if( pNode.hasAttrib("extType") )
	{
		if( stricmp(pNode.attribToString("extType"), "normal") == 0 )
		{
			font.extType = FONTTYPE_NORMAL;
		}else if( stricmp(pNode.attribToString("extType"), "underline") == 0 )
		{
			font.extType = FONTTYPE_UNDERLINE;
		}else if( stricmp(pNode.attribToString("extType"),"italic") == 0 )
		{
			font.extType = FONTTYPE_ITALIC;
		}
	}

	if (pNode.attribToBool("bitmap"))
	{
		font.h = g_pDisplay->CreateBitmapFont(font.type.c_str(), CCODING_GBK);

		int scale = g_pDisplay->GetFontHeight(font.h);

		font.scale = (float)font.height / scale;
	}
	else
	{
		font.h = g_pDisplay->CreateTrueTypeFont(font.height, font.height, font.type.c_str(), CCODING_GBK, font.extType );
		font.scale = 1.0f;
	}

	g_pFrameMgr->AddGameFont(font);
}

void XMLManager::LoadUIAccels(Ogre::XMLNode pNode)
{
	AccelItem accel;
	if (pNode.attribToString("funcname"))
	{
		strncpy(accel.accel_script, pNode.attribToString("funcname"),UI_ACCEL_SCRIPT);
	}

	if (pNode.attribToString("key"))
	{
		accel.accel_key = pNode.attribToInt("key");
	}

	if (pNode.attribToString("alt"))
	{
		accel.modifier_alt = pNode.attribToBool("alt");
	}

	if (pNode.attribToString("ctrl"))
	{
		accel.modifier_ctrl = pNode.attribToBool("ctrl");
	}

	if (pNode.attribToString("shift"))
	{
		accel.modifier_shift = pNode.attribToBool("shift");
	}

	g_pFrameMgr->AddGameAccels(accel);
}

void LoadUIFaceTexture(Ogre::XMLNode pNode)
{
	std::string codemap;
	std::string filepath;
	unsigned int width = 0;
	unsigned int height = 0;
	if (stricmp(pNode.getName(), "FaceTexture") == 0)
	{
		XMLNode pChildNode = pNode.iterateChild();
		while(!pChildNode.isNull())
		{
			if (stricmp(pChildNode.getName(), "codemap") == 0)
			{
                wchar_t buff[1024];
                StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
                char buff2[1024];
                StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);				
                codemap = buff2;
				g_pFrameMgr->m_CodeMapName = codemap;
			}

			if (stricmp(pChildNode.getName(), "popofaceTex") == 0)
			{
				if (pChildNode.attribToString("faceWidth"))
				{
					g_pFrameMgr->m_nFaceWidth = pChildNode.attribToInt("faceWidth");
				}

				if (pChildNode.attribToString("faceHeight"))
				{
					g_pFrameMgr->m_nFaceHeight = pChildNode.attribToInt("faceHeight");
				}

                wchar_t buff[1024];
                StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
                char buff2[1024];
                StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);	
				filepath = buff2;
				g_pFrameMgr->m_PopofaceTexName = filepath;

				XMLNode p1thChildNode = pChildNode.iterateChild();
				while(!p1thChildNode.isNull())
				{
					if (stricmp(p1thChildNode.getName(), "AbsDimension") == 0)
					{
						width = p1thChildNode.attribToInt("x");
						height = p1thChildNode.attribToInt("y");
					}

					p1thChildNode = pChildNode.iterateChild(p1thChildNode);
				}
			}
			pChildNode = pNode.iterateChild(pChildNode);
		}

		assert(!codemap.empty() && !filepath.empty() && "RichText Config Error!");
		g_pFrameMgr->InitFaceTexture(width, height);
		//m_pDestRichText->Init(codemap.c_str(), filepath.c_str(), width, height);
	}
}

void LoadUIPictureTexture( Ogre::XMLNode pNode )
{
	std::string		strConfigPath;
	std::string		strResourcePath;
	unsigned int width = 0;
	unsigned int height = 0;
	RPictureCodeMap* pPictureMapBuffer = g_pFrameMgr->getPictureCodeMap();
	if ( stricmp( pNode.getName(), "PictureTexture" ) == 0 )
	{
		XMLNode pChildNode = pNode.iterateChild();
		while ( !pChildNode.isNull() )
		{
			if ( stricmp( pChildNode.getName(), "configPath" ) == 0 )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode( buff , 1024 , pChildNode.attribToString( "file" ) );
				char buff2[1024];
				StringUtil::UnicodeToAnsi( buff2 , 1024 , buff );				
				strConfigPath = buff2;
				pPictureMapBuffer->SetConfigPath( strConfigPath );
			}

			if ( stricmp( pChildNode.getName(), "popPictureTex" ) == 0 )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode( buff , 1024 , pChildNode.attribToString( "file" ) );
				char buff2[1024];
				StringUtil::UnicodeToAnsi( buff2 , 1024 , buff );	
				strResourcePath = buff2;
				pPictureMapBuffer->SetResourcePath( strResourcePath );

				XMLNode p1thChildNode = pChildNode.iterateChild();
				while ( !p1thChildNode.isNull() )
				{
					if ( stricmp( p1thChildNode.getName(), "AbsDimension" ) == 0 )
					{
						width	= p1thChildNode.attribToInt("x");
						height	= p1thChildNode.attribToInt("y");
					}

					p1thChildNode = pChildNode.iterateChild( p1thChildNode );
				}
			}
			pChildNode = pNode.iterateChild(pChildNode);
		}

		assert( !strConfigPath.empty() && !strResourcePath.empty() && "RichText Config Error!" );
		g_pFrameMgr->InitPictureTexture( width, height );
	}
}


bool XMLManager::LoadUIObject(Ogre::XMLNode pNode)
{
	UIObject *pdestobj = NULL;
	XMLUIObjectParser *parser = NULL;

	CreateObjectByType(pNode, pdestobj, parser, "");

	if(!pdestobj)
	{
		delete(parser);
		assert(0);
		return false;//´´½¨Ê§°Ü
	}

	if(!parser->LoadUIObjectParam(pdestobj, pNode, pdestobj->m_bTemplate))
	{
		return false;
	}

	if(pdestobj->m_bTemplate) g_pFrameMgr->RegisterObject(pdestobj);
	else
	{
		const char *parent_name = pNode.attribToString("parent");
		if(parent_name)
		{
			assert(strcmp(UI_ROOT_NAME.c_str(), parent_name) == 0);
		}

		g_pFrameMgr->AddRootFrame(static_cast<Frame *>(pdestobj));

		/*
		if ( && strcmp(pNode.attribToString("parent"), UI_ROOT_NAME.c_str()))
		{
			static_cast<Frame *>(g_pFrameMgr->FindLayoutFrame(pNode.attribToString("parent")))->
				AddChildFrame(static_cast<Frame *>(pdestobj));

			static_cast<LayoutFrame *>(pdestobj)->m_pParent = 
				static_cast<Frame *>(g_pFrameMgr->FindLayoutFrame(pNode.attribToString("parent")));
		}
		else
		{
			g_pFrameMgr->AddRootFrame(static_cast<Frame *>(pdestobj));
		}*/
	}

	pdestobj->release();
	delete(parser);
	return true;
}

bool XMLManager::LoadUIFromXml(const char *path)
{
	if (!path)
	{
		return false;
	}

	XMLData xmldata;

	if(!xmldata.loadFile(path))
	{
		return false;
	}
	
	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if(stricmp(pChildNode.getName(), "Cursor") == 0)
		{
			LoadUICursor(pChildNode);
		}
		else if(stricmp(pChildNode.getName(), "font") == 0)
		{
			LoadUIFont(pChildNode);
		}
		else if(stricmp(pChildNode.getName(), "Accel") == 0)
		{
			LoadUIAccels(pChildNode);
		}
		else if(stricmp(pChildNode.getName(), "FaceTexture") == 0)
		{
			LoadUIFaceTexture(pChildNode);
		}
		else if ( stricmp(pChildNode.getName(), "PictureTexture") == 0 )
		{
			LoadUIPictureTexture( pChildNode );
		}
		else if(stricmp(pChildNode.getName(), "Script") == 0)//load lua ½Å±¾
		{			   
            std::string strLuaFileName = pChildNode.attribToString("file");
			g_pFrameMgr->m_LuaFile.push_back(strLuaFileName);
			if (!strLuaFileName.empty())
			{
				if(!g_pUIScriptVM->callFile(strLuaFileName.c_str()))
				{
					std::string str = "\tload lua file error!\n\nFileName:";
					str += strLuaFileName;
					MessageBox(NULL, str.c_str(), "Error", MB_OK);

					return false;
				}
			}
			else
			{
				assert(0 && "lua file name or path is wrong!");
			}
		}
		else
		{
			LoadUIObject(pChildNode);
		}

		pChildNode = pRootNode.iterateChild(pChildNode);
	}
	return true;
}

UIObject *XMLManager::CreateUIObjectFromXML(const char *type_name, const char *name, Ogre::XMLNode pNode)
{
	if(type_name && name && !pNode.isNull())
	{
		UIObject *pobj = NULL;
		if (pNode.attribToString("inherits"))
		{
			pobj = g_pFrameMgr->CreateObject(type_name, name, pNode.attribToString("inherits"));
		}
		if( pobj == NULL )
		{
			pobj = g_pFrameMgr->CreateObject(type_name, name);
		}
		if (pNode.attribToBool("virtual"))
		{			
			pobj->m_bTemplate = true;
		}		
		return pobj;
	}
	else
	{
		assert(0);

		return NULL;
	}

	return NULL;
}

char* XMLManager::GetShortName(char* longName)
{
	if (longName)
	{
		const char* pEx = strrchr(longName,'\\');

		if (!pEx)
		{
			return longName;
		}
		pEx++;

		static char sNameShort[256];
		strncpy(sNameShort,pEx,256);
		return sNameShort;
	}

	return NULL;
}

bool XMLManager::SaveUIToXml(const char *path)
{
	if (path)
	{	
		TiXmlDocument doc;

		TiXmlDeclaration *decl = new TiXmlDeclaration( "1.0", "utf-8", "yes" );
		doc.LinkEndChild( decl );

		TiXmlElement *root = new TiXmlElement( "Ui" );
		doc.LinkEndChild( root );

		TiXmlElement *pscript = new TiXmlElement("Script");
		root->LinkEndChild(pscript);

		//Script
		{
			std::string shorname = path;
			size_t pos = shorname.rfind('\\');
			std::string name = shorname.substr(pos+1, shorname.length());
			name = name.substr(0, name.length() - 4);
			std::string luaname = "data/uires/uidemo/";
			luaname += name;
			luaname += ".lua";

			pscript->SetAttribute("file", luaname.c_str());
		}

		for (size_t i=0; i<g_pFrameMgr->m_RootFrames.size(); i++)
		{
			g_pFrameMgr->m_RootFrames[i]->Save(root);
		}

		delete(decl);
		delete(root);
		delete(pscript);

		if (doc.SaveFile(path))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// UIObject XML Parser
//-----------------------------------------------------------------------------
XMLUIObjectParser::XMLUIObjectParser()
{
}

XMLUIObjectParser::~XMLUIObjectParser()
{
}

bool XMLUIObjectParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	return true;
}

bool XMLUIObjectParser::LoadFrameScript(UIObject *pdestobj, XMLNode pNode)
{
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		for (int i=0; i<SC_MAXEVENT; ++i)
		{
			if (stricmp(pChildNode.getName(), s_EventName[i]) == 0)
			{
				if(pChildNode.getText() == NULL)
				{
					char buffer[256];
					pChildNode.getFullName(buffer, 256);
				}

				const char *pstr = pChildNode.getText();
				if( pstr != NULL )
				{
					pdestobj->m_pScriptStr[i] = pstr;
				}
// 				if(pstr == NULL) pdestobj->m_pScriptStr[i] = "";
// 				else pdestobj->m_pScriptStr[i] = pstr;
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}

char* XMLUIObjectParser::Utf8ToGb(const char* src)
{
	wchar_t *tmp = new wchar_t[512];
	char *ret = new char[512];

	int end = StringUtil::UTF8ToUnicode(tmp, 512, src);
	end = StringUtil::UnicodeToAnsi(ret, 512, tmp);
	delete(tmp);
	ret[end] = 0;
	return ret;
}

char* XMLUIObjectParser::GbToUtf8(const char* src)
{
	wchar_t *tmp = new wchar_t[512];
	char *ret = new char[512];

	int end = StringUtil::AnsiToUnicode(tmp, 512, src);
	end = StringUtil::UnicodeToUTF8(ret, 512, tmp);
	delete(tmp);
	ret[end] = 0;
	return ret;
}
