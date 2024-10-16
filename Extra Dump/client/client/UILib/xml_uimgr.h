#pragma once
#include "ui_common.h"
#include "OgreXMLData.h"

class Frame;
class XMLUIObjectParser;
class XMLManager
{
public:
	XMLManager();
	~XMLManager();

	bool SaveUIToXml(const char *path);

	char* XMLManager::GetShortName(char* longName);

	bool LoadUIFromXml(const char *path);
	bool LoadTOCFile(const char *uires);	
	//static void ReplaceSpecialName(Frame *pframe);
	static void CreateObjectByType(Ogre::XMLNode pNode, UIObject *&pdestobj, XMLUIObjectParser *&parser, std::string parentname);

	static UIObject *CreateUIObjectFromXML(const char *type_name, const char *name, Ogre::XMLNode pNode);

public:
protected:
	void LoadUICursor(Ogre::XMLNode pNode);
	void LoadUIFont(Ogre::XMLNode pNode);
	void LoadUIAccels(Ogre::XMLNode pNode);
	bool LoadUIObject(Ogre::XMLNode pNode);
};

class XMLUIObjectParser
{
	friend class XMLManager;
public:
	XMLUIObjectParser();
	~XMLUIObjectParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);

	bool LoadFrameScript(UIObject *pdestobj, Ogre::XMLNode pNode);

public:
	static char* Utf8ToGb(const char* src);
	static char* GbToUtf8(const char* src);
};