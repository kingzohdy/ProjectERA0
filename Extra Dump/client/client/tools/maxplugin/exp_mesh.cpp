
#include "stdafx.h"
#include "MeshNormalSpec.h"
#include "exp_mesh.h"
#include "exp_material.h"
#include "NmFileIO.h"

using namespace Ogre;

namespace DKEXP
{
	const int ANIMVERT_POS_OFFSET = 0;
	const int ANIMVERT_NORMAL_OFFSET = 4;
	const int ANIMVERT_UV_OFFSET = 8;

	//---------------------------------------------------------------------------
	MeshConverter::MeshConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;
		m_iNumUVChannels = 0x0000;

		m_BoundingBox = Box3(Point3(0,0,0), Point3(0,0,0));

		m_bSkin = false;
		m_pMtl = NULL;
		m_NeedSwapIndex = true;
	}

	//---------------------------------------------------------------------------
	MeshConverter::~MeshConverter()
	{
		OGRE_RELEASE(m_pMtl);
	}

	//---------------------------------------------------------------------------
	struct In 
	{ 
		float data; 
		int index; 
	};

	//---------------------------------------------------------------------------
	int cmp( const void *a ,const void *b)
	{ 
		return (*(In *)b).data > (*(In *)a).data ? 1 : -1; 

	} 

	//---------------------------------------------------------------------------
	void MeshConverter::Preprocess(INode* pMaxNode)
	{
		for (int i = 0; i < pMaxNode->NumberOfChildren(); i++)
			Preprocess(pMaxNode->GetChildNode(i));

		ObjectState kOState = pMaxNode->EvalWorldState(0);
		Object* pkEvalObj = kOState.obj;
		if (!pkEvalObj) 
			return;

		// Disable Morph Modifier so that we can get the morph in its
		// base form
		Modifier* pkMod = FindPhysiqueModifier(pMaxNode);
		if (pkMod)
			pkMod->DisableMod();
		else
		{
			pkMod = FindSkinModifier(pMaxNode);
			if (pkMod)
				pkMod->DisableMod();	
		}

	}

	//---------------------------------------------------------------------------
	void MeshConverter::Postprocess(INode* pMaxNode)
	{
		// Re-Enable Morph Modifier
		Modifier* pkMod = FindPhysiqueModifier(pMaxNode);
		if (pkMod)
			pkMod->EnableMod();
		else
		{
			pkMod = FindSkinModifier(pMaxNode);
			if(pkMod)
				pkMod->EnableMod();
		}

		for (int i = 0; i < pMaxNode->NumberOfChildren(); i++)
			Postprocess(pMaxNode->GetChildNode(i));
	}

	//---------------------------------------------------------------------------
	Modifier* MeshConverter::FindSkinModifier(INode *pMaxNode)
	{
		// get the object reference of the node
		Object *pObject;
		pObject = pMaxNode->GetObjectRef();
		if(pObject == 0) return 0;

		// loop through all derived objects
		while(pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
		{
			IDerivedObject *pDerivedObject;
			pDerivedObject = static_cast<IDerivedObject *>(pObject);

			// loop through all modifiers
			int stackId;
			for(stackId = 0; stackId < pDerivedObject->NumModifiers(); stackId++)
			{
				// get the modifier
				Modifier *pModifier;
				pModifier = pDerivedObject->GetModifier(stackId);

				// check if we found the skin modifier
				if(pModifier->ClassID() == SKIN_CLASSID) return pModifier;
			}

			// continue with next derived object
			pObject = pDerivedObject->GetObjRef();
		}

		return 0;
	}

	//---------------------------------------------------------------------------
	bool MeshConverter::FaceUsesOtherMaterial(Mesh* pMesh, int iFace,
		const int iMaxAttr, const int iAttrId)
	{
		MtlID mID = pMesh->getFaceMtlIndex(iFace) % iMaxAttr;

		return (iAttrId != -1 && mID != iAttrId);
	}

	//---------------------------------------------------------------------------
	Modifier *MeshConverter::FindPhysiqueModifier(INode *pMaxNode)
	{
		// get the object reference of the node
		Object *pObject;
		pObject = pMaxNode->GetObjectRef();
		if(pObject == 0) return 0;

		// loop through all derived objects
		while(pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
		{
			IDerivedObject *pDerivedObject;
			pDerivedObject = static_cast<IDerivedObject *>(pObject);

			// loop through all modifiers
			int stackId;
			for(stackId = 0; stackId < pDerivedObject->NumModifiers(); stackId++)
			{
				// get the modifier
				Modifier *pModifier;
				pModifier = pDerivedObject->GetModifier(stackId);

				// check if we found the physique modifier
				if(pModifier->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)) return pModifier;
			}

			// continue with next derived object
			pObject = pDerivedObject->GetObjRef();
		}

		return 0;
	}

	//---------------------------------------------------------------------------
	bool MeshConverter::GetSkinBoneVertexData(INode *pMaxNode,  Mesh *pkMesh, std::vector<INode* > &pBoneNodes,
		const int iMaxAttr, const int iAttrId)
	{
		int modifier_type;
		Modifier *pmodifier = FindPhysiqueModifier(pMaxNode);
		if( pmodifier ) modifier_type = 1; //physique
		else
		{
			pmodifier = FindSkinModifier( pMaxNode );
			if( pmodifier ) modifier_type = 2; //skin
			else return FALSE;
		}

		if (modifier_type == 1)//MODIFIER_PHYSIQUE
		{
			// create a physique export interface
			IPhysiqueExport *pPhysiqueExport;
			pPhysiqueExport = (IPhysiqueExport *)pmodifier->GetInterface(I_PHYINTERFACE);
			if(pPhysiqueExport == 0)
			{
				MessageBox(NULL, _TEXT("Physique modifier interface not found."), 
					_TEXT("Error"), MB_OK);
				return FALSE;
			}

			// create a context export interface
			IPhyContextExport *pContextExport;
			pContextExport = (IPhyContextExport *)pPhysiqueExport->GetContextInterface(pMaxNode);
			if(pContextExport == 0)
			{
				pmodifier->ReleaseInterface(I_PHYINTERFACE, pPhysiqueExport);
				MessageBox(NULL, _TEXT("Context export interface not found."), _T("Error"), MB_OK);
				return FALSE;
			}

			// set the flags in the context export interface
			pContextExport->ConvertToRigid(TRUE);
			pContextExport->AllowBlending(TRUE);

			ConvertSkin(pContextExport, pMaxNode, pkMesh, pBoneNodes, iMaxAttr, iAttrId);

			ConvertOffsetMatrix(pMaxNode, pPhysiqueExport);

			// release all interfaces
			pPhysiqueExport->ReleaseContextInterface(pContextExport);
			pmodifier->ReleaseInterface(I_PHYINTERFACE, pPhysiqueExport);
		}
		else if( modifier_type == 2 )
		{
			// create a skin interface
			ISkin *pSkin;
			pSkin = (ISkin*)pmodifier->GetInterface(I_SKIN);
			if(pSkin == 0)
			{
				MessageBox(NULL, _TEXT("Skin modifier interface not found."), 
					_TEXT("Error"), MB_OK);
				return FALSE;
			}

			// create a skin context data interface
			ISkinContextData *pSkinContextData;
			pSkinContextData = (ISkinContextData *)pSkin->GetContextInterface(pMaxNode);
			if(pSkinContextData == 0)
			{
				pmodifier->ReleaseInterface(I_SKIN, pSkin);
				MessageBox(NULL, _TEXT("Skin modifier interface not found."), 
					_TEXT("Error"), MB_OK);
				return FALSE;
			}

			ConvertSkin(pSkin, pSkinContextData, pMaxNode, pkMesh, pBoneNodes, iMaxAttr, iAttrId);

			ConvertOffsetMatrix(pMaxNode, pSkin);

			// release all interfaces
			pmodifier->ReleaseInterface(I_SKIN, pSkin);
		}

		return true;
	}

	static float GetDeterminant(const ::Matrix3 &tm)
	{
		AffineParts parts;
		decomp_affine(tm, &parts);
		return parts.f;
	}
	int MeshConverter::Convert(INode *pMaxNode, std::vector<INode* > &pBoneNodes, const int iMaxAttr, const int iAttrId, std::vector<MtlAnimData> &MtlAnimDataArray)
	{
		//int iStatus;
		unsigned int iNumUsedFaces; //used by this material
		std::string strNodeName = pMaxNode->GetName();

		ObjectState os = pMaxNode->EvalWorldState(m_animStart);
		Object *obj = os.obj;
		if (!obj)
			return W3D_STAT_NO_RENDERER;
		bool delMesh = false;

		if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		{
			TriObject *tri = NULL;
			tri = (TriObject *)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
			if(obj != tri)
				delMesh = TRUE;

			if(tri)
			{
				Mesh *pkMesh = &(tri->GetMesh());
				assert(pkMesh);

				if (iAttrId == -1)
				{
					// '-1' means ignore all materials so we will be converting all the
					// triangles
					iNumUsedFaces = pkMesh->getNumFaces();
				}
				else
				{
					iNumUsedFaces = 0;
					// otherwise we need to loop and count
					for(int i = 0; i < pkMesh->getNumFaces(); i++)
					{
						if (!FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
						{
							iNumUsedFaces++;
						}
					}
				}

				if (iNumUsedFaces == 0)
				{
					MessageBox (NULL, TEXT ("0 Face Mesh！"), 
						TEXT ("Warning!"), MB_OK | MB_ICONWARNING) ;
					return(W3D_STAT_FAILED);
				}
				else if (iNumUsedFaces > USHRT_MAX)
				{
					assert(0 && "has more than max maximum faces");
				}

				m_PrimType = PRIM_TRIANGLELIST;
				m_nPrimitive = iNumUsedFaces;

				//为顶点分配空间并清零
				int uiNumVerts = 3*iNumUsedFaces;
				m_MaxVertex.resize(uiNumVerts);
				VertexData vert;
				ZeroMemory(&vert, sizeof(vert));
				fill(m_MaxVertex.begin(), m_MaxVertex.end(), vert);

				//导出影响顶点的骨骼ID和权重
				m_NeedSwapIndex = true;
				if(g_ExpOption.exptype == EXP_ANIM_MODEL)
				{
					m_SkinInitTM = pMaxNode->GetObjectTM(m_animStart) * Inverse(pMaxNode->GetNodeTM(m_animStart));
					m_bSkin = GetSkinBoneVertexData(pMaxNode, pkMesh, pBoneNodes, iMaxAttr, iAttrId); //在这个函数里面做m_SkinInitTM *= MeshNode->init_nodetm

					if(GetDeterminant(m_SkinInitTM) < 0) m_NeedSwapIndex = false;
				}
				else if(g_ExpOption.exptype == EXP_LINK_MESH)
				{
					/*
					AffineParts partobj;
					decomp_affine(pMaxNode->GetObjectTM(m_animStart), &partobj);
					m_SkinInitTM = ScaleMatrix(partobj.k*partobj.f);
					*/

					/*
					m_SkinInitTM = pMaxNode->GetObjectTM(m_animStart);
					m_SkinInitTM.SetRow(3, Point3(0,0,0));
					*/

					::Matrix3 nodetm = pMaxNode->GetNodeTM(m_animStart);
					m_SkinInitTM = pMaxNode->GetObjectTM(m_animStart) * Inverse(nodetm);
					nodetm.SetRow(3, ::Point3(0,0,0));
					m_SkinInitTM *= nodetm;
				}
				else
				{
					m_SkinInitTM = pMaxNode->GetObjectTM(m_animStart);
				}
				m_SkinInitTM = m_SkinInitTM * Max2EngineMatrix();

				unsigned int facecount = 0;
				for(int i = 0; i<pkMesh->getNumFaces(); i++)
				{
					if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
					{
						// skip any face that doesn't belong in this trishape
						continue;
					}

					for(int j=0; j<3; j++)
					{
						unsigned int uiMaxVert = pkMesh->faces[i].v[j];
						Point3 &p = pkMesh->verts[uiMaxVert];
						m_MaxVertex[facecount*3+j].pos = ConvertPoint3(p * m_SkinInitTM);
					}
					facecount++;
				}
				assert(facecount == iNumUsedFaces);

				//导出mesh的bindbox
				ConvertBoundingBox(pMaxNode);


				//导出材质
				MaterialConverter mtl(m_animStart, m_animEnd);

				m_pMtl = mtl.Convert(pMaxNode, iMaxAttr, iAttrId, MtlAnimDataArray);

				ConvertNormal(pMaxNode, pkMesh, iMaxAttr, iAttrId);
				ConvertVertexcolor(pkMesh, iMaxAttr, iAttrId);
				m_iNumUVChannels = ConvertUVs(pMaxNode, pkMesh, iMaxAttr, iAttrId);
				if(!ConvertTangentBinormal(iNumUsedFaces))
					return W3D_STAT_TANGENT_FAILED;

				//导出顶点动画
				//ConvertVertAnimation(pMaxNode, iNumUsedFaces, iMaxAttr, iAttrId);
			}
		}
		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	Point3 MeshConverter::GetVertexNormal(Mesh* pkMesh, int iFaceNo, RVertex* pRV)
	{
		Face* pFace = &pkMesh->faces[iFaceNo];
		DWORD smGroup = pFace->smGroup;
		int iNumNormals;
		Point3 vertexNormal;

		// Is normal specified
		// SPECIFIED is not currently used, but may be used in future versions.
		if (pRV->rFlags & SPECIFIED_NORMAL) 
			vertexNormal = pRV->rn.getNormal();

		// If normal is not specified it's only available if the face belongs
		// to a smoothing group
		else if ((iNumNormals = pRV->rFlags & NORCT_MASK) && smGroup) 
		{
			// If there is only one vertex is found in the rn member.
			if (iNumNormals == 1) 
				vertexNormal = pRV->rn.getNormal();
			else
			{
				// If two or more vertices are there you need to step 
				// through them and find the vertex with the same 
				// smoothing group as the current face.
				// You will find multiple normals in the ern member.
				for (int i = 0; i < iNumNormals; i++) 
				{
					vertexNormal = pRV->ern[i].getNormal();
					if (pRV->ern[i].getSmGroup() & smGroup)
						break;
				}
			}
		}
		else 
		{
			// Get the normal from the Face if no smoothing groups are there
			vertexNormal = pkMesh->getFaceNormal(iFaceNo);
		}

		return vertexNormal;
	}

	//---------------------------------------------------------------------------
	static Point3 NfGetVertexNormal(Mesh* pkMesh, int iFaceNo, RVertex* pRV)
	{
		Face* pFace = &pkMesh->faces[iFaceNo];
		DWORD smGroup = pFace->smGroup;
		int iNumNormals;
		Point3 vertexNormal;

		// Is normal specified
		// SPECIFIED is not currently used, but may be used in future versions.
		if (pRV->rFlags & SPECIFIED_NORMAL) 
			vertexNormal = pRV->rn.getNormal();

		// If normal is not specified it's only available if the face belongs
		// to a smoothing group
		else if ((iNumNormals = pRV->rFlags & NORCT_MASK) && smGroup) 
		{
			// If there is only one vertex is found in the rn member.
			if (iNumNormals == 1) 
				vertexNormal = pRV->rn.getNormal();
			else
			{
				// If two or more vertices are there you need to step 
				// through them and find the vertex with the same 
				// smoothing group as the current face.
				// You will find multiple normals in the ern member.
				for (int i = 0; i < iNumNormals; i++) 
				{
					vertexNormal = pRV->ern[i].getNormal();
					if (pRV->ern[i].getSmGroup() & smGroup)
						break;
				}
			}
		}
		else {
			// Get the normal from the Face if no smoothing groups are there
			vertexNormal = pkMesh->getFaceNormal(iFaceNo);
		}

		return vertexNormal;
	}


	//---------------------------------------------------------------------------
	static Modifier* FindModifier(Object* pObj, Class_ID kModID)
	{
		if (!pObj)
			return(NULL);

		IDerivedObject *pDerived = NULL;
		if (pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
		{
			pDerived = (IDerivedObject *) pObj;

			for (int i = 0; i < pDerived->NumModifiers(); i++)
			{
				Modifier* pMod = pDerived->GetModifier(i);

				Class_ID cID = pMod->ClassID();
				if (cID == kModID)
					return(pMod);
			}
		}

		if (pDerived)
			return FindModifier(pDerived->GetObjRef(), kModID);
		else
			return(NULL);
	}

	static Vector3 ConvertTranslateNormal(const Point3 &norm, const ::Matrix3 &tm)
	{
		Point3 tmp = norm;
		tmp.Normalize();
		tmp = tm.VectorTransform(tmp);
		tmp.Normalize();

		return ConvertPoint3(tmp);
	}

	void MeshConverter::ConvertNormal(INode *pMaxNode, Mesh *pkMesh, const int iMaxAttr, const int iAttrId)
	{
		pkMesh->buildNormals();

		int iRealMesh = 0;
		for(int i = 0; i<pkMesh->getNumFaces(); i++)
		{
			if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
			{
				// skip any face that doesn't belong in this trishape
				continue;
			}

			RVertex *pRVert;
			Point3 norm;

			for(int j=0; j<3; j++)
			{
				pRVert = pkMesh->getRVertPtr(pkMesh->faces[i].v[j]);
				m_MaxVertex[iRealMesh*3+j].normal = ConvertTranslateNormal(NfGetVertexNormal(pkMesh, i, pRVert), m_SkinInitTM);
			}
			iRealMesh++;
		}

		Modifier *pModifier = FindModifier(pMaxNode->GetObjectRef(), Class_ID(0x4aa52ae3, 0x35ca1cde));
		if (pModifier)
		{
			//如果修改了法线,需要处理一下,需要max6.0以上版本
			MeshNormalSpec* pkNormalSpec = pkMesh->GetSpecifiedNormals();

			int iCurFace, i;
			for (iCurFace = i = 0; i < pkMesh->getNumFaces(); i++)
			{
				if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
				{
					// skip any face that doesn't belong in this trishape
					continue;
				}
				Point3 norm0 = pkNormalSpec->GetNormal(iCurFace, 0);
				Point3 norm1 = pkNormalSpec->GetNormal(iCurFace, 1);
				Point3 norm2 = pkNormalSpec->GetNormal(iCurFace, 2);

				m_MaxVertex[3*iCurFace+0].normal = ConvertTranslateNormal(norm0, m_SkinInitTM);
				m_MaxVertex[3*iCurFace+1].normal = ConvertTranslateNormal(norm1, m_SkinInitTM);
				m_MaxVertex[3*iCurFace+2].normal = ConvertTranslateNormal(norm2, m_SkinInitTM);

				iCurFace++;
			}
		}
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertVertexcolor(Mesh *pkMesh, const int iMaxAttr, const int iAttrId)
	{
		unsigned int i;
		unsigned int uiRealMesh = 0;

		bool bPerVertexAlpha = 
			(pkMesh->mapSupport(-VDATA_ALPHA) ? true : false);
		if (!bPerVertexAlpha)
		{
			bPerVertexAlpha = 
				(pkMesh->mapSupport(MAP_ALPHA) ? true : false);
		}

		for(i = 0; i<pkMesh->getNumFaces(); i++)
		{
			if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
			{
				// skip any face that doesn't belong in this trishape
				continue;
			}

			for(int j=0; j<3; j++)
			{
				ColourValue color;
				//Vector3 colorVector;
				//if ((pkMesh->vertColArray != NULL) || bPerVertexAlpha)
				if (pkMesh->vertColArray != NULL)
				{				
					if(pkMesh->vertCol)
					{
						Point3 vertCol = pkMesh->vertCol[pkMesh->vcFace[i].t[j]];
						color.r = vertCol.x;
						color.g = vertCol.y;
						color.b = vertCol.z;
					}
					else
					{
						color.r = 1.0f;
						color.g = 1.0f;
						color.b = 1.0f;
					}
				}else
				{
					color.r = 1.0f;
					color.g = 1.0f;
					color.b = 1.0f;
				}
				if(bPerVertexAlpha)
				{
					UVVert * pMapVerts = 
						pkMesh->mapVerts(MAP_ALPHA);
					if (pMapVerts) 
					{
						TVFace* tvf = 
							&pkMesh->mapFaces(MAP_ALPHA)[i];
						Color PickCol(0,0,0);
						PickCol = pMapVerts[tvf->t[j]];
						color.a = PickCol.r;
					}
				}else
				{
					color.a = 1.0f;
				}

				m_MaxVertex[uiRealMesh*3 + j].vertColor = color;
			}

			uiRealMesh++;
		}

	}

	//---------------------------------------------------------------------------
	int MeshConverter::ConvertUVs(INode *pMaxNode, Mesh *pkMesh, const int iMaxAttr, const int iAttrId)
	{
		Mtl* pkNodeMtl = pMaxNode->GetMtl();

		int iNumberOfMaxChannels = pkMesh->getNumMaps();
		int num_uv = 0; //写入引擎的第k套uv
		for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
		{
			if (pkMesh->mapSupport(iMaxChannelID) && pkMesh->getNumMapVerts(iMaxChannelID))
			{
				TVFace* pkMapFaces = pkMesh->mapFaces(iMaxChannelID);

				if(!pkMapFaces)
				{
					continue;
				}

				UVVert *uvverts = pkMesh->mapVerts(iMaxChannelID);

				int iRealMesh = 0;
				for(int i = 0; i < pkMesh->getNumFaces(); i++)
				{
					assert(iRealMesh < pkMesh->getNumFaces());

					if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
					{
						// skip any face that doesn't belong in this trishape
						continue;
					}

					Point2 uv;
					for (int j = 0; j < 3; j++)
					{
						int iMapVertsIndex = pkMapFaces[i].t[j];
						uv.x = uvverts[iMapVertsIndex].x;
						uv.y = 1 - uvverts[iMapVertsIndex].y;

						m_MaxVertex[iRealMesh*3 + j].uv[num_uv] = ConvertPoint2(uv);
					}
					iRealMesh++;
				}
				num_uv++;
				if(num_uv == 8) break;
			}
		}

		return num_uv;

		//bool bHasUV = false;
		//for (i=0; i<MAX_UV_NUM; i++)
		//{
		//	if(m_Texcoord.m_Texcoord[i] != -1000)
		//	{
		//		bHasUV = true;
		//		break;
		//	}
		//}
		//if (!bHasUV)
		//{
		//	m_Texcoord.m_Texcoord[0] = 1;
		//}

		//for (int k = 0; k<MAX_UV_NUM; k++)
		//{
		//	if(m_Texcoord.m_Texcoord[k] != -1000)
		//		iMaxChannelID = m_Texcoord.m_Texcoord[k];
		//	else
		//		continue;
		//	if (pkMesh->mapSupport(iMaxChannelID) && pkMesh->getNumMapVerts(iMaxChannelID))
		//	{
		//		TVFace* pkMapFaces = pkMesh->mapFaces(iMaxChannelID);

		//		if(!pkMapFaces)
		//		{
		//			continue;
		//		}

		//		UVVert *uvverts = pkMesh->mapVerts(iMaxChannelID);

		//		int iRealMesh = 0;
		//		for (i = 0; i < pkMesh->getNumFaces(); i++)
		//		{
		//			assert(iRealMesh < pkMesh->getNumFaces());

		//			if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
		//			{
		//				// skip any face that doesn't belong in this trishape
		//				continue;
		//			}

		//			for (int j = 0; j < 3; j++)
		//			{
		//				int iMapVertsIndex = pkMapFaces[i].t[j];

		//				//float* pMaxUV = (float *)&pkMesh->mapVerts(iMaxChannelID)[iMapVertsIndex];

		//				Point2 uv;
		//				uv.x = uvverts[iMapVertsIndex].x;
		//				uv.y = 1 - uvverts[iMapVertsIndex].y;
		//				//uv.x = pMaxUV[0];
		//				//uv.y = 1.0f - pMaxUV[1];

		//				m_MaxVertex[iRealMesh*3 + j].uv[k] = ConvertPoint2(uv);
		//			}
		//			iRealMesh++;
		//		}
		//	}	
		//}
	}

	//---------------------------------------------------------------------------
	void MeshConverter::AddOneBone( unsigned int nodeid , unsigned int boneid)
	{
		for( int i=0; i<m_BoneParams.size(); i++ )
		{
			if( m_BoneParams[i].nodeid == nodeid ) return;
		}

		BoneInitParam param;
		param.nodeid = nodeid;
		param.boneid = boneid;
		m_BoneParams.push_back(param);
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertOffsetMatrix(INode *pMaxNode, IPhysiqueExport *pPhysiqueExport)
	{
		::Matrix3 init_nodetm;
		int rval = pPhysiqueExport->GetInitNodeTM(pMaxNode, init_nodetm);
		assert(rval == 0);
		m_SkinInitTM = m_SkinInitTM * init_nodetm;

		//compute offset matrix of bones
		::Matrix3 tmpxform;
		for(int i=0; i<m_BoneParams.size(); i++ )
		{
			INode *pbone = (INode *)m_BoneParams[i].nodeid;
			int rval = pPhysiqueExport->GetInitNodeTM( pbone, tmpxform );
			assert( rval == 0 );

			tmpxform.Invert();
			tmpxform = Inverse(Max2EngineMatrix()) * tmpxform * Max2EngineMatrix();
			m_BoneParams[i].offsettm = ConvertMatrix3(tmpxform);
		}

	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertOffsetMatrix(INode *pMaxNode, ISkin *pSkin)
	{
		::Matrix3 init_nodetm;
		int rval = pSkin->GetSkinInitTM( pMaxNode, init_nodetm, TRUE);//object tm
		assert( rval == SKIN_OK );
		//m_SkinInitTM = m_SkinInitTM * init_nodetm;
		m_SkinInitTM = init_nodetm;

		//compute offset matrix of bones
		::Matrix3 tmpxform;
		for(int i=0; i<m_BoneParams.size(); i++ )
		{
			INode *pbone = (INode *)m_BoneParams[i].nodeid;
			int rval = pSkin->GetBoneInitTM( pbone, tmpxform );
			assert( rval == SKIN_OK );

			tmpxform.Invert();
			tmpxform = Inverse(Max2EngineMatrix()) * tmpxform * Max2EngineMatrix();
			m_BoneParams[i].offsettm = ConvertMatrix3(tmpxform);
		}
	}

	//---------------------------------------------------------------------------
	int MeshConverter::FindBoneIDFromArray(const std::vector<INode* > &pBoneNodes, INode *pbone)
	{
		for (unsigned short i=0; i<pBoneNodes.size(); i++)
		{
			if (pbone == pBoneNodes[i])
			{
				return i;
			}
		}
		const TCHAR *pname = pbone->GetName();
		assert(0);
		return -1;
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertSkin(IPhyContextExport *pcontext,  INode* pMaxNode, 
		Mesh *pkMesh, const std::vector<INode* > &pBoneNodes, const int iMaxAttr, const int iAttrId)
	{
		int iRealMesh = 0;
		for (int i=0; i<pkMesh->getNumFaces(); i++)
		{
			if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
			{
				// skip any face that doesn't belong in this trishape
				continue;
			}

			for (unsigned int j=0; j<3; j++)
			{
				unsigned int uiMaxVert = pkMesh->faces[i].v[j];
				unsigned int uiStVert = 3*iRealMesh+j;
				unsigned int ibone;

				// get the vertex export interface
				IPhyVertexExport *pVertexExport;
				pVertexExport = (IPhyVertexExport *)pcontext->GetVertexInterface(uiMaxVert);

				assert(pVertexExport != 0);

				// get the vertex type
				int vertexType = pVertexExport->GetVertexType();

				// handle the specific vertex type
				if(vertexType == RIGID_TYPE)
				{
					// typecast to rigid vertex
					IPhyRigidVertex *pTypeVertex = (IPhyRigidVertex *)pVertexExport;

					// add the influence to the vertex candidate
					// get the influencing bone
					INode *pbone = pTypeVertex->GetNode();
					assert( pbone );

					m_MaxVertex[uiStVert].m_pfWeight[0] = static_cast<unsigned char>(255.0f);

					int new_boneid = FindBoneIDFromArray(pBoneNodes, pbone);
					m_MaxVertex[uiStVert].m_puiBoneIndex[0] = static_cast<unsigned char>(new_boneid);
					AddOneBone(reinterpret_cast<unsigned int>(pbone), static_cast<unsigned int>(new_boneid));
				}
				else if(vertexType == RIGID_BLENDED_TYPE)
				{
					// typecast to blended vertex
					IPhyBlendedRigidVertex *pTypeVertex;
					pTypeVertex = (IPhyBlendedRigidVertex *)pVertexExport;

					int uiNumVertBones = pTypeVertex->GetNumberNodes();
					float fTotalWeight = 0.0f;

					//如果顶点受到大于MAX_BONES根骨骼的影响，取权值大的前MAX_BONES个
					In *pSortBone = new In[uiNumVertBones];

					for(ibone=0; ibone<uiNumVertBones; ibone++)
					{
						pSortBone[ibone].data = pTypeVertex->GetWeight(ibone);
						pSortBone[ibone].index = ibone;
					}
					qsort(pSortBone, uiNumVertBones, sizeof(pSortBone[0]),cmp);
					if(uiNumVertBones > MAX_BONES) uiNumVertBones = MAX_BONES;

					unsigned int uiBone;
					for (uiBone = 0; uiBone < uiNumVertBones; uiBone++)
					{
						INode *pbone = pTypeVertex->GetNode(pSortBone[uiBone].index);
						assert(pbone);

						int new_boneid = FindBoneIDFromArray(pBoneNodes, pbone);
						m_MaxVertex[uiStVert].m_puiBoneIndex[uiBone] = static_cast<unsigned char>(new_boneid);
						AddOneBone(reinterpret_cast<unsigned int>(pbone), static_cast<unsigned int>(new_boneid));

						fTotalWeight += pSortBone[uiBone].data;
					}

					//weight
					if (fTotalWeight > 1.1f || fTotalWeight < 0.6f)
					{
						assert(0);
					}

					for(uiBone=0; uiBone<uiNumVertBones; uiBone++)
					{
						float weight = (pSortBone[uiBone].data/fTotalWeight)*255.0f;
						m_MaxVertex[uiStVert].m_pfWeight[uiBone] = static_cast<unsigned char>(weight);
					}

					delete[] (pSortBone);

				}
				else
				{
					MessageBox(NULL, _TEXT("Physique vertex type is not rigid nor blend"), _TEXT("Error"), MB_OK);
					return ;
				}

				pcontext->ReleaseVertexInterface(pVertexExport);

				iRealMesh++;
			}
		}
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertSkin(ISkin *pSkin, ISkinContextData *pSkinContextData, 
		INode* pMaxNode, Mesh *pkMesh,const std::vector<INode* > &pBoneNodes,
		const int iMaxAttr, const int iAttrId)
	{
		int iRealMesh = 0;
		// fill in bone assignments and weights per vertex
		for (int i=0; i<pkMesh->getNumFaces(); i++)
		{
			if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
			{
				// skip any face that doesn't belong in this trishape
				continue;
			}

			for (unsigned int j=0; j<3; j++)
			{
				unsigned int uiMaxVert = pkMesh->faces[i].v[j];
				unsigned int uiStVert = 3*iRealMesh+j;
				unsigned int ibone;

				unsigned int uiNumVertBones = pSkinContextData->GetNumAssignedBones(uiMaxVert);

				float fTotalWeight = 0.0f;

				//如果顶点受到大于MAX_BONES根骨骼的影响，取权值大的前MAX_BONES个
				In *pSortBone = new In[uiNumVertBones];
				for(ibone=0; ibone<uiNumVertBones; ibone++)
				{
					pSortBone[ibone].data = pSkinContextData->GetBoneWeight(uiMaxVert, ibone);
					pSortBone[ibone].index = ibone;
				}
				qsort(pSortBone, uiNumVertBones, sizeof(pSortBone[0]),cmp);
				if(uiNumVertBones > MAX_BONES)
				{
					uiNumVertBones = MAX_BONES;
				}

				unsigned int uiBone;
				for(uiBone = 0; uiBone < uiNumVertBones; uiBone++)
				{
					//assignment, get the bone id
					int boneId;
					boneId = pSkinContextData->GetAssignedBone(uiMaxVert, pSortBone[uiBone].index);
					if(boneId < 0)
					{
						assert(boneId >= 0);
						continue;
					}

					INode *pbone = pSkin->GetBone(boneId);
					assert(pbone);

					int new_boneid = FindBoneIDFromArray(pBoneNodes, pbone);
					m_MaxVertex[uiStVert].m_puiBoneIndex[uiBone] = static_cast<unsigned char>(new_boneid);
					AddOneBone(reinterpret_cast<unsigned int>(pbone), static_cast<unsigned int>(new_boneid));

					fTotalWeight += pSortBone[uiBone].data;
				}

				//weight
				if (fTotalWeight > 1.1f || fTotalWeight < 0.6f)
				{
					assert(0);
				}

				for(uiBone=0; uiBone<uiNumVertBones; uiBone++)
				{
					float weight = (pSortBone[uiBone].data/fTotalWeight)*255.0f;
					m_MaxVertex[uiStVert].m_pfWeight[uiBone] = static_cast<unsigned char>(weight);
				}

				delete[] (pSortBone);
			}

			iRealMesh++;
		}
	}

	//---------------------------------------------------------------------------
	void MeshConverter::GetBindBox(int iNumVerts, Ogre::Vector3 *pVertPos, 
		Ogre::Vector3 *pMinVert, Ogre::Vector3 *pMaxVert)
	{
		for (int i=0; i<iNumVerts; i++)
		{
			pMinVert->x = pVertPos[i].x > pMinVert->x?pMinVert->x:pVertPos[i].x;
			pMinVert->y = pVertPos[i].y > pMinVert->y?pMinVert->y:pVertPos[i].y;
			pMinVert->z = pVertPos[i].z > pMinVert->z?pMinVert->z:pVertPos[i].z;

			pMaxVert->x = pVertPos[i].x > pMaxVert->x?pVertPos[i].x:pMaxVert->x;
			pMaxVert->y = pVertPos[i].y > pMaxVert->y?pVertPos[i].y:pMaxVert->y;
			pMaxVert->z = pVertPos[i].z > pMaxVert->z?pVertPos[i].z:pMaxVert->z;
		}
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertUVs(INode *pMaxNode, Mesh *pkMesh, int iFrame, int iStride, unsigned int iNumUsedFaces,
		MorphAnimData *pmorphanim, bool bGetBindBox, const int iMaxAttr, const int iAttrId)
	{
		Mtl* pkNodeMtl = pMaxNode->GetMtl();
		int iNumberOfMaxChannels = pkMesh->getNumMaps();
		for (int k = 1; k<iNumberOfMaxChannels; k++)
		{
			int iMaxChannelID = k;
			BOOL b = pkMesh->mapSupport(iMaxChannelID);
			int ib = pkMesh->getNumMapVerts(iMaxChannelID);
			if (pkMesh->mapSupport(iMaxChannelID) && pkMesh->getNumMapVerts(iMaxChannelID))
			{
				TVFace* pkMapFaces = pkMesh->mapFaces(iMaxChannelID);

				if(!pkMapFaces)
				{
					assert(0);
				}

				Ogre::Vector3 *pVertUV = new Ogre::Vector3[iNumUsedFaces*3];
				Ogre::Vector3 tempVert = Vector3(0.0f, 0.0f, 0.0f);

				int facecount = 0;
				int i;
				for(i = 0; i < pkMesh->getNumFaces(); i++)
				{
					if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
					{
						// skip any face that doesn't belong in this trishape
						continue;
					}

					UVVert *uvverts = pkMesh->mapVerts(iMaxChannelID);
					for (int j = 0; j < 3; j++)
					{
						int iMapVertsIndex = pkMapFaces[i].t[j];

						float* pMaxUV = (float *)&pkMesh->mapVerts(iMaxChannelID)[iMapVertsIndex];

						Point2 uv;
						uv.x = uvverts[iMapVertsIndex].x;
						uv.y = 1 - uvverts[iMapVertsIndex].y;

						tempVert.x = uv.x;
						tempVert.y = uv.y;
						tempVert.z = 0.0f;

						pVertUV[facecount*3+j] = tempVert;
					}
					facecount++;
				}
				assert(facecount == iNumUsedFaces);

				if (bGetBindBox)
				{
					GetBindBox(iNumUsedFaces*3, pVertUV, &pmorphanim->m_MinValue[1], &pmorphanim->m_MaxValue[1]);
					delete[] (pVertUV);
					return;
				}

				Ogre::Vector3 vLen = Vector3((pmorphanim->m_MaxValue[1].x - pmorphanim->m_MinValue[1].x), 
					(pmorphanim->m_MaxValue[1].y - pmorphanim->m_MinValue[1].y),
					(pmorphanim->m_MaxValue[1].z - pmorphanim->m_MinValue[1].z));

				unsigned char* pTempVert = new unsigned char[3];
				for (i=0; i<iNumUsedFaces*3; i++)
				{
					pTempVert[0] = static_cast<unsigned char>((pVertUV[i].x - pmorphanim->m_MinValue[1].x)/vLen.x * 255);
					pTempVert[1] = static_cast<unsigned char>((pVertUV[i].y - pmorphanim->m_MinValue[1].y)/vLen.y * 255);
					pTempVert[2] = static_cast<unsigned char>((pVertUV[i].z - pmorphanim->m_MinValue[1].z)/vLen.z * 255);

					memcpy(&pmorphanim->m_pVertData->m_VertData[(iFrame*iNumUsedFaces*3+i)*iStride], &pTempVert[0], 2*sizeof(unsigned char));
				}

				delete[] (pTempVert);
				delete[] (pVertUV);

				return;
			}
		}

	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertNormal(INode *pMaxNode, Mesh *pkMesh, int iFrame, int iStride, int iNumUsedFaces, MorphAnimData *pmorphanim,
		const int iMaxAttr, const int iAttrId)
	{
		pkMesh->buildNormals();

		unsigned char* pTempVert = new unsigned char[3];
		int facecount = 0;
		for(int i = 0; i<pkMesh->getNumFaces(); i++)
		{
			if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
			{
				// skip any face that doesn't belong in this trishape
				continue;
			}	

			RVertex *pRVert;
			Point3 norm;	
			for(int j=0; j<3; j++)
			{
				pRVert = pkMesh->getRVertPtr(pkMesh->faces[i].v[j]);
				norm = NfGetVertexNormal(pkMesh, i, pRVert);
				norm.Normalize();
				pTempVert[0] = static_cast<unsigned char>(-((norm.x+1.0f) * 0.5f)*255);
				pTempVert[1] = static_cast<unsigned char>(-((norm.y+1.0f) * 0.5f)*255);
				pTempVert[2] = static_cast<unsigned char>(-((norm.z+1.0f) * 0.5f)*255);

				int nvert = (iFrame*iNumUsedFaces + facecount)*3 + j;
				memcpy(&pmorphanim->m_pVertData->m_VertData[nvert*iStride+ANIMVERT_NORMAL_OFFSET],&pTempVert[0], 3*sizeof(unsigned char));
			}
			facecount++;
		}

		Modifier *pModifier = FindModifier(pMaxNode->GetObjectRef(), 
			Class_ID(0x4aa52ae3, 0x35ca1cde));

		if (pModifier)
		{
			//如果修改了法线,需要处理一下,需要max6.0以上版本
			MeshNormalSpec* pkNormalSpec = pkMesh->GetSpecifiedNormals();

			int facecount = 0;
			for(int i=0; i < pkMesh->getNumFaces(); i++)
			{
				if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
				{
					// skip any face that doesn't belong in this trishape
					continue;
				}
				Point3 norm0 = pkNormalSpec->GetNormal(i, 0);
				Point3 norm1 = pkNormalSpec->GetNormal(i, 1);
				Point3 norm2 = pkNormalSpec->GetNormal(i, 2);

				norm0.Normalize();
				norm1.Normalize();
				norm2.Normalize();

				int nvert = (iFrame*iNumUsedFaces + facecount)*3;
				pTempVert[0] = static_cast<unsigned char>(-((norm0.x+1.0f) * 0.5f)*255);
				pTempVert[1] = static_cast<unsigned char>(-((norm0.y+1.0f) * 0.5f)*255);
				pTempVert[2] = static_cast<unsigned char>(-((norm0.z+1.0f) * 0.5f)*255);			
				memcpy(&pmorphanim->m_pVertData->m_VertData[(nvert+0)*iStride+ANIMVERT_NORMAL_OFFSET],
					&pTempVert[0], 3*sizeof(unsigned char));

				pTempVert[0] = static_cast<unsigned char>(-((norm1.x+1.0f) * 0.5f)*255);
				pTempVert[1] = static_cast<unsigned char>(-((norm1.y+1.0f) * 0.5f)*255);
				pTempVert[2] = static_cast<unsigned char>(-((norm1.z+1.0f) * 0.5f)*255);
				memcpy(&pmorphanim->m_pVertData->m_VertData[(nvert+1)*iStride+ANIMVERT_NORMAL_OFFSET],
					&pTempVert[0], 3*sizeof(unsigned char));

				pTempVert[0] = static_cast<unsigned char>(-((norm2.x+1.0f) * 0.5f)*255);
				pTempVert[1] = static_cast<unsigned char>(-((norm2.y+1.0f) * 0.5f)*255);
				pTempVert[2] = static_cast<unsigned char>(-((norm2.z+1.0f) * 0.5f)*255);
				memcpy(&pmorphanim->m_pVertData->m_VertData[(nvert+2)*iStride+ANIMVERT_NORMAL_OFFSET],
					&pTempVert[0], 3*sizeof(unsigned char));

				facecount++;
			}
		}
		delete[] (pTempVert);
	}

	//---------------------------------------------------------------------------
	int MeshConverter::ConvertVertAnimFrameData(INode *pMaxNode, int iFrame, int iNumUsedFaces,
		Ogre::MorphAnimData *pmorphanim, bool bGetBindBox, const int iMaxAttr, const int iAttrId)
		//char* pVertData, Ogre::Vector3 *pVertMin, Ogre::Vector3 *pVertMax)
	{
		uint iStride = pmorphanim->m_pVertData->m_Stride;

		ObjectState os = pMaxNode->EvalWorldState(iFrame * GetTicksPerFrame());
		Object *obj = os.obj;
		if (!obj)
			return W3D_STAT_NO_RENDERER;

		if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		{
			TriObject *tri = NULL;
			tri = (TriObject *)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));

			if(tri)
			{
				Mesh *pkMesh = &(tri->GetMesh());
				assert(pkMesh);

				if(pkMesh->getNumFaces() == 0)
					return W3D_STAT_NO_RENDERER;
/*
				if(g_ExpOption.exp_vertanim)
				{
					::Matrix3 tm = pMaxNode->GetObjectTM(m_animStart);

					unsigned int i;
					int uiNumVerts = iNumUsedFaces*3;
					Ogre::Vector3 *pVertPos = new Ogre::Vector3[uiNumVerts];
					Ogre::Vector3 minVert = Vector3(0.0f, 0.0f, 0.0f);
					Ogre::Vector3 maxVert = Vector3(0.0f, 0.0f, 0.0f);

					int facecount = 0;
					for(int i = 0; i<pkMesh->getNumFaces(); i++)
					{
						if(FaceUsesOtherMaterial(pkMesh, i, iMaxAttr, iAttrId))
						{
							// skip any face that doesn't belong in this trishape
							continue;
						}

						for(int j=0; j<3; j++)
						{
							unsigned int uiMaxVert = pkMesh->faces[i].v[j];
							Point3 p = pkMesh->verts[uiMaxVert];
							pVertPos[facecount*3+j] = ConvertPoint3(p * m_SkinInitTM);
						}
						facecount++;
					}
					assert(facecount == iNumUsedFaces);

					if (bGetBindBox)
					{
						GetBindBox(uiNumVerts, pVertPos, &pmorphanim->m_MinValue[0], &pmorphanim->m_MaxValue[0]);
						delete[] pVertPos;
						return W3D_STAT_OK;
					}

					Ogre::Vector3 vLen = Vector3((pmorphanim->m_MaxValue[0].x - pmorphanim->m_MinValue[0].x),
						(pmorphanim->m_MaxValue[0].y - pmorphanim->m_MinValue[0].y),
						(pmorphanim->m_MaxValue[0].z - pmorphanim->m_MinValue[0].z));

					unsigned char* pTempVert = new unsigned char[3];
					for (i=0; i<uiNumVerts; i++)
					{
						pTempVert[0] = static_cast<unsigned char>((pVertPos[i].x - pmorphanim->m_MinValue[0].x)/vLen.x * 255);
						pTempVert[1] = static_cast<unsigned char>((pVertPos[i].y - pmorphanim->m_MinValue[0].y)/vLen.y * 255);
						pTempVert[2] = static_cast<unsigned char>((pVertPos[i].z - pmorphanim->m_MinValue[0].z)/vLen.z * 255);

						memcpy(&pmorphanim->m_pVertData->m_VertData[(iFrame*uiNumVerts+i)*iStride], &pTempVert[0], 3*sizeof(unsigned char));
					}

					ConvertNormal(pMaxNode, pkMesh, iFrame, iStride, iNumUsedFaces, pmorphanim, iMaxAttr, iAttrId);

					delete[] (pVertPos);
					delete[] (pTempVert);
				}
				else if (g_ExpOption.exp_uvanim)
				{
					ConvertUVs(pMaxNode, pkMesh, iFrame, iStride, iNumUsedFaces, pmorphanim, bGetBindBox, iMaxAttr, iAttrId);
				}
*/
				//ConvertVertexcolor(pkMesh, iNumFaces);
			}
		}
		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	MorphAnimData *MeshConverter::ConvertVertAnimation(INode *pMaxNode, int iNumUsedFaces, const int iMaxAttr, const int iAttrId)
	{
		return NULL;
		/*
		int iStatus;

		VertexFormat vertfmt;
		uint channel = 0;
		if(g_ExpOption.exp_vertanim)
		{
			vertfmt.addElement(VET_COLOUR, VES_POSITION);
			vertfmt.addElement(VET_COLOUR, VES_NORMAL);
			channel |= MorphAnimData::CHANNEL_POS;
		}
		if(g_ExpOption.exp_uvanim)
		{
			vertfmt.addElement(VET_COLOUR, VES_TEXCOORD);
			channel |= MorphAnimData::CHANNEL_UV0;
		}

		MorphAnimData *pmorphanim = new MorphAnimData;

		pmorphanim->m_Channel = channel;
		pmorphanim->m_nFrame = ((float)(m_animEnd - m_animStart))/GetTicksPerFrame() + 1 + 0.5f;
		pmorphanim->m_Ranges.resize(1);
		pmorphanim->m_Ranges[0].begin = (float)m_animStart/GetTicksPerFrame() + 0.5f;
		pmorphanim->m_Ranges[0].end = (float)m_animEnd/GetTicksPerFrame() + 0.5f;

		pmorphanim->m_nVertex = iNumUsedFaces*3;
		pmorphanim->m_pVertData = new Ogre::VertexData;
		pmorphanim->m_pVertData->init(vertfmt, pmorphanim->m_nFrame * pmorphanim->m_nVertex);

		memset(&pmorphanim->m_pVertData->m_VertData[0], 0, pmorphanim->m_pVertData->m_VertData.size()); 

		for (int i=0; i<4; i++)
		{
			pmorphanim->m_MinValue[i] = Vector3(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
			pmorphanim->m_MaxValue[i] = -Vector3(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
		}

		//获取BindBox
		bool bGetBindBox = true;
		for (int iFrame=0; iFrame<pmorphanim->m_nFrame; iFrame++)
		{	
			iStatus = ConvertVertAnimFrameData(pMaxNode, iFrame, iNumUsedFaces, pmorphanim, bGetBindBox, iMaxAttr, iAttrId);
			if (iStatus)
			{
				assert(0);
			}
		}

		bGetBindBox = false;
		for (int iFrame=0; iFrame<pmorphanim->m_nFrame; iFrame++)
		{
			uint frametick = (m_animStart + iFrame*GetTicksPerFrame()) * 1000/TIME_TICKSPERSEC;
			pmorphanim->m_FrameTicks.push_back(frametick);
			iStatus = ConvertVertAnimFrameData(pMaxNode, iFrame, iNumUsedFaces, pmorphanim, bGetBindBox, iMaxAttr, iAttrId);
			if (iStatus)
			{
				assert(0);
			}
		}

		return pmorphanim;*/
	}

	//---------------------------------------------------------------------------
	bool MeshConverter::ConvertTangentBinormal(int iNumUsedFaces)
	{
		NmRawTriangle *pRawTriangles = new NmRawTriangle[iNumUsedFaces];

		int i;
		for (i=0; i<iNumUsedFaces; i++)
		{
			pRawTriangles[i].vert[0].x = m_MaxVertex[i*3].pos.x;
			pRawTriangles[i].vert[0].y = m_MaxVertex[i*3].pos.y;
			pRawTriangles[i].vert[0].z = m_MaxVertex[i*3].pos.z;

			pRawTriangles[i].vert[1].x = m_MaxVertex[i*3+1].pos.x;
			pRawTriangles[i].vert[1].y = m_MaxVertex[i*3+1].pos.y;
			pRawTriangles[i].vert[1].z = m_MaxVertex[i*3+1].pos.z;

			pRawTriangles[i].vert[2].x = m_MaxVertex[i*3+2].pos.x;
			pRawTriangles[i].vert[2].y = m_MaxVertex[i*3+2].pos.y;
			pRawTriangles[i].vert[2].z = m_MaxVertex[i*3+2].pos.z;

			pRawTriangles[i].norm[0].x = m_MaxVertex[i*3].normal.x;
			pRawTriangles[i].norm[0].y = m_MaxVertex[i*3].normal.y;
			pRawTriangles[i].norm[0].z = m_MaxVertex[i*3].normal.z;

			pRawTriangles[i].norm[1].x = m_MaxVertex[i*3+1].normal.x;
			pRawTriangles[i].norm[1].y = m_MaxVertex[i*3+1].normal.y;
			pRawTriangles[i].norm[1].z = m_MaxVertex[i*3+1].normal.z;

			pRawTriangles[i].norm[2].x = m_MaxVertex[i*3+2].normal.x;
			pRawTriangles[i].norm[2].y = m_MaxVertex[i*3+2].normal.y;
			pRawTriangles[i].norm[2].z = m_MaxVertex[i*3+2].normal.z;

			pRawTriangles[i].texCoord[0].u = m_MaxVertex[i*3].uv[0].x;
			pRawTriangles[i].texCoord[0].v = m_MaxVertex[i*3].uv[0].y;

			pRawTriangles[i].texCoord[1].u = m_MaxVertex[i*3+1].uv[0].x;
			pRawTriangles[i].texCoord[1].v = m_MaxVertex[i*3+1].uv[0].y;

			pRawTriangles[i].texCoord[2].u = m_MaxVertex[i*3+2].uv[0].x;
			pRawTriangles[i].texCoord[2].v = m_MaxVertex[i*3+2].uv[0].y;
		}


		NmRawTangentSpaceD* tan;

		if (!NmComputeTangentsD (iNumUsedFaces, pRawTriangles, &tan))
		{
			return false;
		}

		for (i=0; i<iNumUsedFaces; i++)
		{
			m_MaxVertex[i*3].tangent = Vector3(tan[i].tangent[0].x, tan[i].tangent[0].y, tan[i].tangent[0].z);
			m_MaxVertex[i*3+1].tangent = Vector3(tan[i].tangent[1].x, tan[i].tangent[1].y, tan[i].tangent[1].z);
			m_MaxVertex[i*3+2].tangent = Vector3(tan[i].tangent[2].x, tan[i].tangent[2].y, tan[i].tangent[2].z);

			m_MaxVertex[i*3].binormal = Vector3(tan[i].binormal[0].x, tan[i].binormal[0].y, tan[i].binormal[0].z);
			m_MaxVertex[i*3+1].binormal = Vector3(tan[i].binormal[1].x, tan[i].binormal[1].y, tan[i].binormal[1].z);
			m_MaxVertex[i*3+2].binormal = Vector3(tan[i].binormal[2].x, tan[i].binormal[2].y, tan[i].binormal[2].z);
		}

		return true;
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ComputeBoundingBox(INode *pMaxNode)
	{
		bool bInitBoundingbox = false;
		for(TimeValue tick=m_animStart; tick<=m_animEnd; 
			tick+=GetTicksPerFrame())
		{
			ObjectState os = pMaxNode->EvalWorldState(tick);
			Object *obj = os.obj;
			if (!obj)
				return ;

			if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
			{
				TriObject *tri = NULL;
				tri = (TriObject *)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));

				if(tri)
				{
					Mesh *pkMesh = &(tri->GetMesh());
					assert(pkMesh);

					Box3 b = pkMesh->getBoundingBox(&pMaxNode->GetObjectTM(tick));
					if(!bInitBoundingbox)
					{
						m_BoundingBox = b;
						bInitBoundingbox = true;
					}
					else if(!m_BoundingBox.Contains(b))
					{
						m_BoundingBox += b;
					}
				}
			}
		}
	}

	//---------------------------------------------------------------------------
	void MeshConverter::ConvertBoundingBox(INode *pMaxNode)
	{
		ComputeBoundingBox(pMaxNode);

		Ogre::Matrix4 tm = ConvertMatrix3(Max2EngineMatrix());

		m_MinVertexPos = ConvertPoint3(m_BoundingBox.Min()) * tm;
		m_MaxVertexPos = ConvertPoint3(m_BoundingBox.Max()) * tm;
	}
}
