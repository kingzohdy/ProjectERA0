
#ifndef __XMLData_H__
#define __XMLData_H__

#include "OgrePrerequisites.h"

class TiXmlElement;
class TiXmlDocument;

namespace Ogre
{
	class _OgreExport XMLNode
	{
	public:
		XMLNode() : m_pElement(NULL)
		{
		}

		XMLNode(TiXmlElement *pelement) : m_pElement(pelement)
		{
		}

		bool isNull()
		{
			return m_pElement==NULL;
		}

		const char *getName();
		char *getFullName(char *buffer, size_t maxbuf); //from parent --->  child
		const char *getText();

		bool hasChild(const char *name);
		XMLNode getChild(const char *name);
		XMLNode iterateChild(XMLNode previous);
		XMLNode iterateChild();

		bool hasAttrib(const char *name);
		const char *attribToString(const char *name);
		int attribToInt(const char *name);
		float attribToFloat(const char *name);
		bool attribToBool(const char *name);

		bool attribToInt(const char *name, int &value);
		bool attribToFloat(const char *name, float &value);

		void setAttribInt(const char *name, int v);
		void setAttribFloat(const char *name, float v);
		void setAttribBool(const char *name, bool b);
		void setAttribStr(const char *name, const char *pstr);
		
	public:
		TiXmlElement *m_pElement;
	};

	class _OgreExport XMLData
	{
	public:
		XMLData();
		~XMLData();

		bool loadBuffer(const void *pbuf, size_t bufsize);
		bool loadStream(DataStream *pstream);
		bool loadFile(const String &filename);
		bool loadRawFile(const String &filename);

		bool saveFile(const String &filename);

		XMLNode getRootNode();
		XMLNode getNodeByPath(const char *path); //path="Config.RenderSystem.FXSetting"， 如果中间任何一个节点不存在， 返回空节点

	private:
		TiXmlDocument *m_pDoc;
	};
}

#endif