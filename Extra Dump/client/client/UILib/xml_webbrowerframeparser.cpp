#include "xml_webbrowerframeparser.h"
#include "ui_framemgr.h"
#include "OgreScreenTexture.h"
#include "OgreRenderTypes.h"
#include "OgreTexture.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML WebBrowerFrame Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

XMLWebBrowerFrameParser::XMLWebBrowerFrameParser()
{
}

XMLWebBrowerFrameParser::~XMLWebBrowerFrameParser()
{
}

bool XMLWebBrowerFrameParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
 	XMLFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);
 
 	WebBrowerFrame *pDestWebBrowerFrame = static_cast<WebBrowerFrame *>(pdestobj);
 
 	XMLNode pChildNode = pNode.iterateChild();
	return true;
}
