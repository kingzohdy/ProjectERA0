
#ifndef __EXP_ATTRIBS_H__
#define __EXP_ATTRIBS_H__

#include "exp_types.h"

namespace DKEXP
{
	class CustAttribConverter
	{
	public:
		int Convert(INode *pMaxNode, Ogre::Material *pdestmtl, Animatable *obj, std::vector<MtlAnimData> &MtlAnimDataArray);

		bool ConvertParamBlock(INode *pMaxNode, Ogre::Material *pdestmtl, Animatable *obj,
			IParamBlock2* pkParamBlock, std::vector<MtlAnimData> &MtlAnimDataArray);

		bool IsValidName(char* pcStr);
		bool ConvertStringAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);	
		bool ConvertFloatAttrib(INode *pMaxNode, IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl, std::vector<MtlAnimData> &MtlAnimDataArray);
		bool ConvertIntAttrib(INode *pMaxNode, Animatable *obj, IParamBlock2* pkParamBlock,
			int iIndex, Ogre::Material *pdestmtl);
		bool ConvertColorAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl, int& iIncrement);
		bool ConvertPoint3Attrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);
		bool ConvertPoint4Attrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);
		bool ConvertBoolAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);
		bool ConvertFloatTabAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);
		bool ConvertIntListBoxAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);
		bool ConvertTexMapAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);
		bool ConvertFRGBAAttrib(INode *pMaxNode, IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl, std::vector<MtlAnimData> &MtlAnimDataArray);
		bool ConvertBitMapAttrib(IParamBlock2* pkParamBlock, int iIndex, 
			Ogre::Material *pdestmtl);

		static bool SaveTextureChunk(const char *texName, Ogre::Material *pdestmtl, const char *texParamName);

		void ChangeTextureNameStyle(const char* str, char * temp);

		static char* GetShortName(char* longName);
		static void GetFilePath (char *szFile,char *szPath, char separator);

		CustAttribConverter(TimeValue animStart, TimeValue animEnd);
		~CustAttribConverter();

	protected:

	private:
		int m_iMapChannel;

		TimeValue m_animStart;
		TimeValue m_animEnd;
	};
}

#endif