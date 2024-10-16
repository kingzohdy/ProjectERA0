
#ifndef __EXP_MATERIAL_H__
#define __EXP_MATERIAL_H__

#include "exp_types.h"

namespace DKEXP
{
	class MaterialConverter
	{
	public:
		static int GetNumMaterials(Mtl* pkMaterial, bool& bForceMultiSub);

		Ogre::Material *Convert(INode *pMaxNode, const int iMaxAttr,
			const int iAttrId, std::vector<MtlAnimData> &MtlAnimDataArray);

		int CreateSimple(INode *pMaxNode);
		int ConvertCustom(Mtl *pMaterial, Ogre::Material *pmtl, INode *pMaxNode,
			std::vector<MtlAnimData> &MtlAnimDataArray);

		int ConvertSingle(Ogre::Material *pmtl, Mtl *pMaterial, 
			INode *pMaxNode);

		MaterialConverter(TimeValue animStart,TimeValue animEnd);
		~MaterialConverter();

	private:
		TimeValue m_animStart;
		TimeValue m_animEnd;
	};
}

#endif