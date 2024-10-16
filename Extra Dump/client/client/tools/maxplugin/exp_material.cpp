
#include "stdafx.h"
#include "shaders.h"
#include "exp_material.h"
#include "exp_attribs.h"

using namespace Ogre;

namespace DKEXP
{
	MaterialConverter::MaterialConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;
	}
	MaterialConverter::~MaterialConverter()
	{
	}

	int MaterialConverter::GetNumMaterials(Mtl* pkMaterial, bool& bForceMultiSub)
	{
		bForceMultiSub = false;
		if (pkMaterial == NULL)
			return(1);

		if (pkMaterial->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
		{
			MultiMtl *pMM = (MultiMtl *) pkMaterial;
			bForceMultiSub = true;
			return(pMM->NumSubMtls());
		}

		return(1);
	}

	//---------------------------------------------------------------------------
	Ogre::Material *MaterialConverter::Convert(INode *pMaxNode, const int iMaxAttr,
		const int iAttrId, std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		Mtl *pMaterial = pMaxNode->GetMtl();
		int iStatus = 0;

		Ogre::Material *pdestmtl = new Ogre::Material;

		if (pMaterial == NULL)
		{
			iStatus = CreateSimple(pMaxNode);
		}
		else if(pMaterial->ClassID() == Class_ID(CMTL_CLASS_ID, 0) || pMaterial->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		{
			Shader *pShader = ((StdMat2 *) pMaterial)->GetShader();

			Class_ID cID = pShader->ClassID();

			iStatus = ConvertSingle(pdestmtl, pMaterial, pMaxNode);
			return pdestmtl;

		}
		else if (pMaterial->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
		{
			//多材质
			MultiMtl *pMM;

			pMM = (MultiMtl *)pMaterial;
			pMaterial = pMM->GetSubMtl(iAttrId);

			if (pMaterial && (pMaterial->ClassID() == DIRECTX_9_SHADER_CLASS_ID))
			{
				iStatus = ConvertCustom(pMaterial, pdestmtl, pMaxNode, MtlAnimDataArray);
				return pdestmtl;
			}
			else if(pMaterial && (pMaterial->ClassID()==Class_ID(CMTL_CLASS_ID, 0) || pMaterial->ClassID()==Class_ID(DMTL_CLASS_ID, 0)))
			{
				iStatus = ConvertSingle(pdestmtl, pMaterial, pMaxNode);
				return pdestmtl;
			}
			else
			{
				iStatus = CreateSimple(pMaxNode);
			}
		}
		else if(pMaterial->ClassID() == DIRECTX_9_SHADER_CLASS_ID)
		{
			iStatus = ConvertCustom(pMaterial, pdestmtl, pMaxNode, MtlAnimDataArray);
			return pdestmtl;
		}
		else
		{
			iStatus = W3D_STAT_NO_MATERIAL;
		}

		if (iStatus == W3D_STAT_NO_MATERIAL)
		{
			////如果没有材质，添加默认材质
			pdestmtl->setTemplateName("stdmtl");

			char strBitMapName[256];
			sprintf(strBitMapName, "%s\\%s", g_ExpOption.src_rootdir, OGRE_DEFAULT_TEXTURE);

			CustAttribConverter::SaveTextureChunk(strBitMapName, pdestmtl, "g_DiffuseTex");

			return pdestmtl;
		}

		return NULL;
	}

	//---------------------------------------------------------------------------
	// Create a simple material for an object that doesn't have a material
	// in MAX.
	int MaterialConverter::CreateSimple(INode *pMaxNode)
	{

		return W3D_STAT_NO_MATERIAL;
	}

	//---------------------------------------------------------------------------
	int MaterialConverter::ConvertCustom(Mtl *pMaterial, Ogre::Material *pdestmtl, INode *pMaxNode,	std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		int iStatus;

		pdestmtl->setName(pMaterial->GetName());
		CustAttribConverter kAttribConverter(m_animStart, m_animEnd);

		iStatus = kAttribConverter.Convert(pMaxNode, pdestmtl, pMaterial, MtlAnimDataArray);
		return iStatus;
	}

	//---------------------------------------------------------------------------
	int MaterialConverter::ConvertSingle(Ogre::Material *pdestmtl, Mtl *pMaterial, INode *pMaxNode)
	{

		StdMat2 *pSM;
		float fShine, fSelfIllum, fOpacity, fShinStr;
		Color amb, diff, spec;
		int iShading;

		std::string strMtlName = (const char*)pMaterial->GetName();
		pdestmtl->setName(pMaterial->GetName());

		pSM = (StdMat2 *) pMaterial;
		pSM->Update(m_animStart, FOREVER);

		// First get the simple material data out of MAX
		amb = pSM->GetAmbient(m_animStart);
		diff = pSM->GetDiffuse(m_animStart);
		spec = pSM->GetSpecular(m_animStart);

		fSelfIllum = pSM->GetSelfIllum(m_animStart);
		// Max's exported shine and shinestr ranges from 0.0-1.0, 
		//gamebryo expects 0-100
		fShine = 100.0f * pSM->GetShininess(m_animStart);
		fOpacity = pSM->GetOpacity(m_animStart);
		fShinStr = 100.0f * pSM->GetShinStr(m_animStart);

		iShading = pSM->GetShading();

		pdestmtl->setTemplateName("stdmtl");
		if(fOpacity < 1.0f) pdestmtl->setParamMacro("BLEND_MODE", Ogre::BLEND_ALPHABLEND);
		else if(pSM->MapEnabled(ID_OP))pdestmtl->setParamMacro("BLEND_MODE", Ogre::BLEND_ALPHATEST);

		if(pSM->GetTwoSided()) pdestmtl->setParamMacro("DOUBLE_SIDE", 1);

		char strBitMapName[256];
		//漫反射通道
		if (pSM->MapEnabled(ID_DI))
		{
			BitmapTex *pTm = (BitmapTex*) pSM->GetSubTexmap(ID_DI);

			BitmapInfo kBI;
			const char* pcMapName = pTm->GetMapName();
			TheManager->GetImageInfo(&kBI, pcMapName);

			strcpy(strBitMapName, kBI.Name());
		}
		else
		{
			sprintf(strBitMapName, "%s\\%s", g_ExpOption.src_rootdir, OGRE_DEFAULT_TEXTURE);
		}
		CustAttribConverter::SaveTextureChunk(strBitMapName, pdestmtl, "g_DiffuseTex");

		Vector3 selfcolor(0,0,0);
		if(pSM->GetSelfIllumColorOn())
		{
			selfcolor = Vector3(pSM->GetSelfIllumColor(0).r,pSM->GetSelfIllumColor(0).g,pSM->GetSelfIllumColor(0).b);
		}

		//高光通道，自发光通道
		if(pSM->MapEnabled(ID_SP))
		{
			BitmapTex *pTm = (BitmapTex*) pSM->GetSubTexmap(ID_SP);
			BitmapInfo kBI;
			const char* pcMapName = pTm->GetMapName();
			TheManager->GetImageInfo(&kBI, pcMapName);
			strcpy(strBitMapName, kBI.Name());

			pdestmtl->setParamMacro("USE_SELFILLUM_TEX", 1);
			CustAttribConverter::SaveTextureChunk(strBitMapName, pdestmtl, "g_SpecSelfTex");

			if(pSM->MapEnabled(ID_SI))
			{
				selfcolor = Vector3(1.0f, 1.0f, 1.0f);
			}
		}
		pdestmtl->setParamValue("g_SelfPower", &selfcolor);

		float specpower = pSM->GetShinStr(0);
		float shiness = pSM->GetShininess(0);
		if(specpower == 0) shiness = 0;
		if(shiness > 32) shiness = 32;

		pdestmtl->setParamValue("g_SpecShiness",&shiness);
		pdestmtl->setParamValue("g_SpecPower",&specpower);

		return W3D_STAT_OK;
	}
}
