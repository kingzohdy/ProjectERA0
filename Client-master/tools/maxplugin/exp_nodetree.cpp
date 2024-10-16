
#include "stdafx.h"
#include "dummy.h"
#include "exp_nodetree.h"
#include "exp_anim.h"
#include "exp_camera.h"
#include "exp_light.h"
#include "exp_material.h"
#include "exp_mesh.h"
#include "exp_vertex.h"
#include <vector>
#include <string>
#include <algorithm>

#include "OgreAnimTrackBone.h"
#include "OgreAnimTrackMaterialParam.h"

using namespace Ogre;

namespace DKEXP
{
	bool HierConverter::m_bExpOnly = false;

	//---------------------------------------------------------------------------
	const char * HierConverter::ID2Name(int id)
	{
		static char buffer[64];
		sprintf(buffer, "%d", id);

		return buffer;
	}

	//---------------------------------------------------------------------------
	HierConverter::HierConverter(TimeValue animStart,TimeValue animEnd)
	{
		m_animStart = animStart;
		m_animEnd = animEnd;
		m_CellNum = -1;

		m_bHasBounding = false;
		m_bExpOnly = false;
	}

	//---------------------------------------------------------------------------
	HierConverter::~HierConverter()
	{
		size_t i;
		for(i=0; i<m_BSPNodes.size(); i++)
		{
			m_BSPNodes[i].pbsp->release();
		}

		for(i=0; i<m_MeshNodes.size(); i++)
		{
			m_MeshNodes[i].pmesh->release();
		}

		for(i=0; i<m_MtlAnimDataArray.size(); i++)
		{
			delete[] m_MtlAnimDataArray[i].pticks;
			free(m_MtlAnimDataArray[i].pkeyframes);
		}

		for(i=0; i<keyframedata.size(); i++)
		{
			delete keyframedata[i];
		}
	}

	//---------------------------------------------------------------------------
	static bool IsNodeRenderable( INode *pnode, Object *pobj )
	{
		if(!pobj || !pobj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
			return false;
		if( pobj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0) ) return false;
		if( pobj->ClassID() == Class_ID(BONE_CLASS_ID, 0) ) return false;
#if MAX_RELEASE >= 4000
		if( pobj->ClassID() == BONE_OBJ_CLASSID ) return false;
#endif

		// check for biped node
		Control *pControl = pnode->GetTMController();
		if( (pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || 
			(pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID) || 
			(pControl->ClassID() == FOOTPRINT_CLASS_ID) )
		{
			return false;
		}

		return true;
	}

	//---------------------------------------------------------------------------
	int HierConverter::BuildNode(Interface* pIntf, INode* pMaxNode)
	{
		int iStatus;

		Object *pObj = pMaxNode->GetObjectRef();

		char* strNodeName = pMaxNode->GetName();

		if(stricmp(strNodeName, "BSPDATA") == 0)
		{
			iStatus = ConvertBSPData(pMaxNode);
			if (iStatus) return iStatus;
			else return W3D_STAT_OK;
		}
		else if(stricmp(strNodeName, "PORTAL") == 0)
		{
			iStatus = ConvertPortalData(pMaxNode);
			if (iStatus) return iStatus;
			else return W3D_STAT_OK;
		}
		else if(stricmp(strNodeName, "AABB") == 0)
		{
			iStatus = ConvertBoundingData(pMaxNode);
			if (iStatus) return iStatus;
			else return W3D_STAT_OK;
		}

		iStatus = AnimationConverter::BuildNoteTrack(pMaxNode, m_SequenceArray, m_TriggerArray);
		if (iStatus)
		{
			return W3D_STAT_FAILED;
		}

		//如果有相同id的sequenc报错
		for (UINT i=0; i<m_SequenceArray.size(); i++)
		{
			for (UINT j=i+1; j<m_SequenceArray.size(); j++)
			{
				if(m_SequenceArray[i].sequence.id == m_SequenceArray[j].sequence.id)
				{
					MessageBox(NULL, "有相同id的sequenc", "Error", MB_OK);
					return W3D_STAT_FAILED;
				}
			}

		}

		m_pBoneNodes.push_back(pMaxNode);
		m_BoneNodeTypes.push_back(0);

		//根据INode的种类处理
		if (pObj != NULL)
		{
			SClass_ID sID;

			sID = pObj->SuperClassID();

			while (sID == GEN_DERIVOB_CLASS_ID)
			{
				pObj = ((IDerivedObject *)pObj)->GetObjRef();
				sID = pObj->SuperClassID();
			}

			//导出所有INode的关键桢
			switch (sID)
			{
			case CAMERA_CLASS_ID:
				{
					CameraConverter cam(m_animStart, m_animEnd);

					iStatus = cam.Convert(pIntf, pMaxNode, pObj, m_pBoneNodes);

					if (iStatus)
						return iStatus;
					m_CameraArray.push_back(cam);

					break;
				}
			case LIGHT_CLASS_ID:
				{
					LightConverter light(m_animStart, m_animEnd);
					iStatus = light.Convert(pIntf, pMaxNode, pObj, m_pBoneNodes);

					if (iStatus)
						return iStatus;

					m_LightArray.push_back(light);
					break;
				}
			case GEOMOBJECT_CLASS_ID:
				{
					if (IsNodeRenderable(pMaxNode, pObj))
					{
						GeomNode gNode;
						gNode.pNode = pMaxNode;
						gNode.cellid = m_CellNum;
						m_pGeomNodes.push_back(gNode);
					}	
					break;
				}
			case HELPER_CLASS_ID:
				{
					//dummy
					Class_ID id = pObj->ClassID();
					if (id == Class_ID(DUMMY_CLASS_ID, 0))
					{
						m_BoneNodeTypes.back() = 1;

						TSTR strTemp;
						if (pMaxNode->GetUserPropString("Indoor", strTemp) ||
							pMaxNode->GetUserPropString("OutdoorLighting", strTemp))
						{
							MaxCell cell;

							BOOL bFound = false;
							bFound = pMaxNode->GetUserPropString("Indoor", strTemp);
							if (bFound == false)
								cell.m_nIndoor = 0;
							else
								cell.m_nIndoor = 1;

							bFound = pMaxNode->GetUserPropString("OutdoorLighting", strTemp);
							if (bFound == false)
								cell.m_nOutdoorLighting = 0;
							else
								cell.m_nOutdoorLighting = 1;

							m_CellNum++;
							strcpy(cell.cellname, strNodeName);
							m_CellArray.push_back(cell);
						}
					}else
					{
						//assert(0 && "unknown HELPER_CLASS_ID");
					}
					break;
				}
			default:
				{
					//unkonwn class
					break;
				}	
			}
		}

		int num = pMaxNode->NumberOfChildren();
		for (int i=0; i<pMaxNode->NumberOfChildren(); i++)
		{
			INode *pMaxChild;
			char *pChildName;

			pMaxChild = pMaxNode->GetChildNode(i);

			pChildName = pMaxChild->GetName();

			iStatus = BuildNode(pIntf, pMaxChild);
			if (iStatus)
			{
				return iStatus;
			}
		}

		return W3D_STAT_OK;
	}

	static void calMeshBounding(MeshData *pmesh)
	{
		BoxBound box;
		for(size_t i=0; i<pmesh->m_SubMeshes.size(); i++)
		{
			SubMeshData *psubmesh = pmesh->m_SubMeshes[i];
			box += psubmesh->m_pVertexData->m_MinPos;
			box += psubmesh->m_pVertexData->m_MaxPos;
		}

		pmesh->m_Bounding.fromBox(box.getMinPos(), box.getMaxPos());
	}
	//---------------------------------------------------------------------------
	int HierConverter::ConvertGeom(GeomNode node)
	{
		INode *pMaxNode = node.pNode;
		bool bHidden = pMaxNode->IsNodeHidden() ? true : false;
		if (bHidden)
			return W3D_STAT_OK;

		int iStatus;

		bool bForceMultiSub = false;
		int iNumMats = MaterialConverter::GetNumMaterials(pMaxNode->GetMtl(), bForceMultiSub);

		if (!bForceMultiSub && (iNumMats == 1))
		{
			iNumMats = 1;
		}

		MeshNodeData meshnode;
		meshnode.pmesh = new MeshData;
		meshnode.pmesh->m_Name = pMaxNode->GetName();
		meshnode.pmaxnode = node.pNode;

		m_MeshNodes.push_back(meshnode);

		for(int i=0; i<iNumMats; i++)
		{
			int attr = i;
			if(!bForceMultiSub && iNumMats==1) attr = -1;

			iStatus = ConvertMeshes(pMaxNode, iNumMats, attr, meshnode.pmesh);
		}

		calMeshBounding(meshnode.pmesh);
		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	//导出静态mesh
	int HierConverter::ConvertStaticMeshes(INode *pMaxNode, const int iMaxAttr, const int iAttrId, int cellid)
	{
		int iStatus;

		TSTR strNodeName;

		StaticMeshData mdata;
		mdata.psubmesh = new SubMeshData;

		MeshConverter mesh(m_animStart, m_animEnd);
		iStatus = mesh.Convert(pMaxNode, m_pBoneNodes, iMaxAttr, iAttrId, m_MtlAnimDataArray);
		if (iStatus)
		{
			return iStatus;
		}

		mdata.m_BoundingBox = mesh.m_BoundingBox;
		mdata.m_CellID = cellid;
		mdata.meshname = pMaxNode->GetName();

		//利用当前顶点信息生成所有导出信息的结构
		InflBoneArray bonearray(MAX_BONE_INFL);

		bonearray.Init(mesh, false);
		mdata.psubmesh->m_pVertexData = BuildVertexChunk(bonearray, mesh.m_iNumUVChannels, bonearray.m_MaxVertInfl>0);
		mdata.psubmesh->m_pIndexData = BuildIndexChunk(bonearray, mesh.m_NeedSwapIndex);

		mdata.psubmesh->m_PrimType = PRIM_TRIANGLELIST;
		mdata.psubmesh->m_NumPrimitive = nVertex2nPrimitive(mdata.psubmesh->m_PrimType, mdata.psubmesh->m_pIndexData->getNumIndex());
		mdata.psubmesh->m_pMtl = mesh.m_pMtl;
		mesh.m_pMtl->addRef();

		m_StaticMeshChunkArray.push_back(mdata);

		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	//导出动态mesh
	int HierConverter::ConvertMeshes(INode *pMaxNode, const int iMaxAttr, const int iAttrId, MeshData *pmeshdata)
	{
		int iStatus;
		SubMeshData *psubmeshdata = new SubMeshData;

		MeshConverter mesh(m_animStart, m_animEnd);
		iStatus = mesh.Convert(pMaxNode, m_pBoneNodes, iMaxAttr, iAttrId, m_MtlAnimDataArray);
		if (iStatus)
		{
			psubmeshdata->release();
			return iStatus;
		}

		//利用当前顶点信息生成所有导出信息的结构
		InflBoneArray bonearray(MAX_BONE_INFL);
		bonearray.Init(mesh, false);
		psubmeshdata->m_pVertexData = BuildVertexChunk(bonearray, mesh.m_iNumUVChannels, bonearray.m_MaxVertInfl>0);
		psubmeshdata->m_pIndexData = BuildIndexChunk(bonearray, mesh.m_NeedSwapIndex);

		psubmeshdata->m_PrimType = PRIM_TRIANGLELIST;
		psubmeshdata->m_NumPrimitive = nVertex2nPrimitive(psubmeshdata->m_PrimType, psubmeshdata->m_pIndexData->getNumIndex());
		psubmeshdata->m_pMtl = mesh.m_pMtl;
		mesh.m_pMtl->addRef();

		bool bskin = false;
		int bindbone = MeshConverter::FindBoneIDFromArray(m_pBoneNodes, pMaxNode);
		if(mesh.m_bSkin)
		{
			BuildSkinInfoChunk(psubmeshdata, bonearray);
			bskin = true;
		}

		if(iAttrId==-1 || iAttrId==0)
		{
			pmeshdata->m_bSkin = bskin;
			pmeshdata->m_BindBoneID = bindbone;
		}
		else
		{
			assert(pmeshdata->m_bSkin == bskin);
			assert(pmeshdata->m_BindBoneID == bindbone);
		}

		pmeshdata->addSubMesh(psubmeshdata);
		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	int HierConverter::Convert()
	{
		int iStatus;

		//导出所有骨骼信息
		if(g_ExpOption.exptype==EXP_ANIM_MODEL || g_ExpOption.exptype==EXP_PURE_ANIM)
		{
			for (int i=0; i<m_pBoneNodes.size(); i++)
			{
				INode *pMaxNode = m_pBoneNodes[i];
				TSTR strUserData;
				pMaxNode->GetUserPropBuffer(strUserData);
				if (strstr(strUserData, "anchorid"))
				{
					ANCHOR_T anchor;
					anchor.boneid = MeshConverter::FindBoneIDFromArray(m_pBoneNodes, pMaxNode);
					pMaxNode->GetUserPropInt("anchorid", anchor.anchorid);
					m_Anchors.push_back(anchor);
				}

				if (!m_bExpOnly)
				{
					AnimationConverter anim(m_animStart, m_animEnd);

					iStatus = anim.Convert(m_pBoneNodes[i], m_BoneNodeTypes[i]);
					if (iStatus)
					{
						return iStatus;
					}
					m_AnimArray.push_back(anim);
				}
				else
				{
					assert(m_SequenceArray.size() == 1);
					TimeValue start = m_SequenceArray[0].sequence.time_start;
					TimeValue end = m_SequenceArray[0].sequence.time_end;
					AnimationConverter anim(start, end);

					iStatus = anim.Convert(m_pBoneNodes[i], m_BoneNodeTypes[i]);
					if (iStatus)
					{
						return iStatus;
					}
					m_AnimArray.push_back(anim);
				}
				g_Interface->ProgressUpdate( i*100/(m_pBoneNodes.size() + m_pGeomNodes.size()), 
					TRUE, _T("Test:") );
			}
		}

		if(g_ExpOption.exptype != EXP_PURE_ANIM)
		{
			//导出几何体信息
			for (int i=0; i<m_pGeomNodes.size(); i++)
			{
				iStatus = ConvertGeom(m_pGeomNodes[i]);
				if (iStatus)
				{
					return iStatus;
				}
				g_Interface->ProgressUpdate((i+m_pBoneNodes.size())*100/(m_pBoneNodes.size() + m_pGeomNodes.size()),
					TRUE, _T("Test:") );
			}
		}

		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	void HierConverter::BuildSkinInfoChunk(Ogre::SubMeshData *psubmeshdata, InflBoneArray &pBonearray)
	{
		psubmeshdata->m_MaxVertInfl = pBonearray.m_MaxVertInfl;
		psubmeshdata->m_Patches = pBonearray.m_Patches;
		pBonearray.m_Patches.resize(0);
	}

	static void GetSequenceInfo(AnimationConverter &conv, const SEQUENCE_EXP_T &seq, size_t &seq_beginframe, size_t &seq_endframe)
	{
		seq_beginframe = seq.sequence.time_start/GetTicksPerFrame();
		seq_endframe = seq.sequence.time_end/GetTicksPerFrame();
		if(seq_endframe < seq_beginframe) seq_endframe = seq_beginframe;
		if(seq_endframe >= conv.m_keyFrame.size())
		{
			//MessageBox(NULL, "seq_endframe > m_AnimArray[boneid].m_keyFrame.size()", "error", MB_OK);
			seq_endframe = conv.m_keyFrame.size() - 1;
			if(seq_beginframe > seq_endframe) seq_beginframe = seq_endframe;
		}
	}
	//---------------------------------------------------------------------------
	void HierConverter::ConvertPosKeyFrames(int boneid, BonesData &bonesdata, int iSeq, float fTransParam)
	{
		if (m_AnimArray[boneid].m_keyFrame.size() == 0)
		{
			bonesdata.posarray.resize(0);
			bonesdata.postick.resize(0);
			return;
		}

		size_t seq_beginframe, seq_endframe;
		GetSequenceInfo(m_AnimArray[boneid], m_SequenceArray[iSeq], seq_beginframe, seq_endframe);

		size_t nKeys = seq_endframe-seq_beginframe+1;
		Vector3 *pAnimPos = new Vector3[nKeys];
		unsigned int *pAnimIndex = new unsigned int[nKeys];

		for (size_t j=0; j<nKeys; j++)
		{
			assert(j+seq_beginframe < m_AnimArray[boneid].m_keyFrame.size());

			pAnimPos[j] = m_AnimArray[boneid].m_keyFrame[j+seq_beginframe].m_TM.t;
			pAnimIndex[j] = m_AnimArray[boneid].m_keyFrame[j+seq_beginframe].m_tick;
		}

		Vector3 *pAnimRedPos = new Vector3[nKeys];
		unsigned int *pAnimRedIndex = new unsigned int[nKeys];

		//优化关键桢
		unsigned int uiRedKeys = SampleReduction((size_t)nKeys, pAnimPos, pAnimIndex, pAnimRedPos, pAnimRedIndex, fTransParam);

		bonesdata.vNumKeys.x = uiRedKeys;
		for (size_t j=0; j<uiRedKeys; j++)
		{
			bonesdata.posarray.push_back(pAnimRedPos[j]);
			bonesdata.postick.push_back(pAnimRedIndex[j]);
		}

		delete[] (pAnimPos);
		delete[] (pAnimIndex);
		delete[] (pAnimRedPos);
		delete[] (pAnimRedIndex);
	}

	//---------------------------------------------------------------------------
	void HierConverter::ConvertRotKeyFrames(int boneid, BonesData &bonesdata, int iSeq, float fRotParam)
	{
		if (m_AnimArray[boneid].m_keyFrame.size() == 0)
		{
			bonesdata.rotarray.resize(0);
			bonesdata.rottick.resize(0);
			return;
		}

		size_t seq_beginframe, seq_endframe;
		GetSequenceInfo(m_AnimArray[boneid], m_SequenceArray[iSeq], seq_beginframe, seq_endframe);

		size_t nKeys = seq_endframe-seq_beginframe+1;
		Quaternion *pAnimRot = new Quaternion[nKeys];
		unsigned int *pAnimIndex = new unsigned int[nKeys];
		for (size_t j=0; j<nKeys; j++)
		{
			pAnimRot[j] = m_AnimArray[boneid].m_keyFrame[j+seq_beginframe].m_TM.q;
			pAnimIndex[j] = m_AnimArray[boneid].m_keyFrame[j+seq_beginframe].m_tick;
		}

		Quaternion *pAnimRedRot = new Quaternion[nKeys];
		unsigned int *pAnimRedIndex = new unsigned int[nKeys];

		unsigned int uiRedKeys = SampleReduction((size_t)nKeys, pAnimRot, pAnimIndex, pAnimRedRot, pAnimRedIndex, fRotParam* (PI/180.0f) * (1.0f/2.0f));

		bonesdata.vNumKeys.y = uiRedKeys;
		for (size_t j=0; j<uiRedKeys; j++)
		{
			bonesdata.rotarray.push_back(pAnimRedRot[j]);
			bonesdata.rottick.push_back(pAnimRedIndex[j]);
		}


		delete[] (pAnimRot);
		delete[] (pAnimIndex);
		delete[] (pAnimRedRot);
		delete[] (pAnimRedIndex);
	}

	//---------------------------------------------------------------------------
	void HierConverter::ConvertScaleKeyFrames(int boneid, BonesData &bonesdata, int iSeq, float fScaleParam)
	{
		if (m_AnimArray[boneid].m_keyFrame.size() == 0)
		{
			bonesdata.scalearray.resize(0);
			bonesdata.scaletick.resize(0);
			return;
		}

		size_t seq_beginframe, seq_endframe;
		GetSequenceInfo(m_AnimArray[boneid], m_SequenceArray[iSeq], seq_beginframe, seq_endframe);

		size_t nKeys = seq_endframe-seq_beginframe+1;
		Vector3 *pAnimScale = new Vector3[nKeys];
		unsigned int *pAnimIndex = new unsigned int[nKeys];

		for (size_t j=0; j<nKeys; j++)
		{
			pAnimScale[j] = m_AnimArray[boneid].m_keyFrame[j+seq_beginframe].m_TM.k;
			pAnimIndex[j] = m_AnimArray[boneid].m_keyFrame[j+seq_beginframe].m_tick;
		}

		Vector3 *pAnimRedScale = new Vector3[nKeys];
		unsigned int *pAnimRedIndex = new unsigned int[nKeys];

		unsigned int uiRedKeys = SampleReduction((size_t)nKeys, pAnimScale, pAnimIndex, pAnimRedScale, pAnimRedIndex, fScaleParam);

		bonesdata.vNumKeys.z = uiRedKeys;
		for (size_t j=0; j<uiRedKeys; j++)
		{
			bonesdata.scalearray.push_back(pAnimRedScale[j]);
			bonesdata.scaletick.push_back(pAnimRedIndex[j]);
		}

		delete[] (pAnimScale);
		delete[] (pAnimIndex);
		delete[] (pAnimRedScale);
		delete[] (pAnimRedIndex);
	}

	//---------------------------------------------------------------------------
	//创建顶点数据
	Ogre::VertexData *HierConverter::BuildVertexChunk(const InflBoneArray &pBonearray, int numuv, bool skin)
	{
		int i;
		VertexFormat vertfmt;
		vertfmt.addElement(VET_FLOAT3, VES_POSITION);

		if(g_ExpOption.exp_normal)
		{
			vertfmt.addElement(VET_FLOAT3, VES_NORMAL);
		}

		if(g_ExpOption.exp_tangent)
		{
			vertfmt.addElement(VET_FLOAT3, VES_TANGENT);
			vertfmt.addElement(VET_FLOAT3, VES_BINORMAL);
		}

		if(g_ExpOption.exp_vertcolor)
		{
			vertfmt.addElement(VET_COLOUR, VES_COLOR);
		}

		int uv_index[8];
		if(numuv > 0)//如果原始uv不够， 用第一套代替
		{
			for(i=0; i<g_ExpOption.exp_numuv; i++)
			{
				vertfmt.addElement(VET_FLOAT2, VES_TEXCOORD, i);
				if(i<numuv) uv_index[i] = i;
				else uv_index[i] = 0;
			}
		}

		if(skin)
		{
			vertfmt.addElement(VET_COLOUR, VES_BLEND_WEIGHTS);
			vertfmt.addElement(VET_COLOUR, VES_BLEND_INDICES);
		}


		int nvert = (int)pBonearray.m_MaxVertex.size();
		Ogre::VertexData *pvd = new Ogre::VertexData(vertfmt, nvert);
		BoxBound boxbound;
		char buffer[4096];

		for(i=0; i<nvert; i++)
		{
			const VertexData &srcvert = pBonearray.m_MaxVertex[i];
			unsigned int offset = 0;

			for(uint j=0; j<vertfmt.getNumElements(); j++)
			{
				const VertexElement &element = vertfmt.getElement(j);

				if(element.m_Semantic == VES_POSITION)
				{
					const Vector3 &pos = srcvert.pos;
					boxbound += pos;
					memcpy(&buffer[offset], &pos, 3*sizeof(float));
					offset += 3*sizeof(float);
				}
				else if(element.m_Semantic == VES_NORMAL)
				{
					memcpy(&buffer[offset], &srcvert.normal, 3*sizeof(float));
					offset += 3*sizeof(float);
				}
				else if(element.m_Semantic == VES_TEXCOORD)
				{
					assert(element.m_Index < 8);
					int index = uv_index[element.m_Index];
					memcpy(&buffer[offset], &srcvert.uv[index], 2*sizeof(float));
					offset += 2*sizeof(float);
				}
				else if(element.m_Semantic == VES_COLOR)
				{
					ColourValue color = srcvert.vertColor;
					uint tempColor = color.getAsRGBA();
					memcpy(&buffer[offset], &tempColor, sizeof(uint));
					offset += sizeof(uint);
				}
				else if(element.m_Semantic == VES_BLEND_INDICES)
				{
					memcpy(&buffer[offset], &srcvert.m_puiBoneIndex[0], MAX_BONES*sizeof(char));
					offset += MAX_BONES*sizeof(char);
				}
				else if(element.m_Semantic == VES_BLEND_WEIGHTS)
				{
					memcpy(&buffer[offset], &srcvert.m_pfWeight[0], MAX_BONES*sizeof(char));
					offset += MAX_BONES*sizeof(char);
				}
				else if(element.m_Semantic == VES_TANGENT)
				{
					memcpy(&buffer[offset], &srcvert.tangent, 3*sizeof(float));
					offset += 3*sizeof(float);
				}
				else if(element.m_Semantic == VES_BINORMAL)
				{
					memcpy(&buffer[offset], &srcvert.binormal, 3*sizeof(float));
					offset += 3*sizeof(float);
				}
			}

			assert(offset < 1024);
			assert(offset == pvd->m_Stride);
			memcpy(&pvd->m_VertData[i*pvd->m_Stride], buffer, pvd->m_Stride);
		}

		pvd->m_MinPos = boxbound.getMinPos();
		pvd->m_MaxPos = boxbound.getMaxPos();
		return pvd;
	}

	//---------------------------------------------------------------------------
	//创建索引数据
	Ogre::IndexData *HierConverter::BuildIndexChunk(InflBoneArray &pBonearray, bool swap_index)
	{
		unsigned int nvert = static_cast<unsigned int>(pBonearray.m_MaxVertex.size());
		unsigned int nindex = static_cast<unsigned int>(pBonearray.m_Indices.size());

		IndexData *pid = new IndexData(nindex);
		pid->setVertexRange(0, nvert);
		pid->m_IndexData = pBonearray.m_Indices;

		//把索引顺序修改为顺时针
		if(swap_index)
		{
			assert((nindex%3) == 0);
			for(int i=0; i<nindex; i++)
			{
				if((i%3)==1)
				{
					Swap(pid->m_IndexData[i], pid->m_IndexData[i+1]);
				}
			}
		}

		return pid;
	}

	//---------------------------------------------------------------------------
	SkeletonData *HierConverter::BuildSkeletonChunk(INode* pMaxNode)
	{
		if(g_ExpOption.exptype == EXP_ANIM_MODEL)
		{
			SkeletonData *pskeleton = new SkeletonData;

			for (unsigned int i=0; i<m_AnimArray.size(); i++)
			{
				Ogre::BoneData *pbonedata = new Ogre::BoneData;

				//通过boneID为新的bone索引值赋值
				pbonedata->m_ID = static_cast<int>(i);

				pbonedata->m_FatherID = static_cast<int>(-1);
				for (int j=0; j<m_pBoneNodes.size(); j++)
				{
					if (m_AnimArray[i].m_bone.m_FatherID == reinterpret_cast<unsigned int>(m_pBoneNodes[j]))
					{
						pbonedata->m_FatherID = j;
						break;
					}
				}

				pbonedata->m_Name =  m_AnimArray[i].m_bone.m_Name.c_str();
				pbonedata->m_OriginTM = m_AnimArray[i].m_bone.m_originTM;
				pbonedata->m_UserData = m_AnimArray[i].m_bone.m_userData.c_str();

				pskeleton->addBone(pbonedata);
			}

			return pskeleton;
		}
		else
		{
			/*
			Ogre::BoneData *pbonedata = new Ogre::BoneData;

			//通过boneID为新的bone索引值赋值
			pbonedata->m_ID = static_cast<int>(0);

			pbonedata->m_FatherID = static_cast<int>(-1);

			pbonedata->m_Name =  pMaxNode->GetName();
			pbonedata->m_OriginTM = ConvertMatrix3(pMaxNode->GetObjectTM(m_animStart)*Max2EngineMatrix());
			pbonedata->m_UserData = "";

			pskeleton->addBone(pbonedata);
			*/
			return NULL;
		}
	}

	//---------------------------------------------------------------------------
	void HierConverter::CalculateKeyframeDataBySequence(int iSeq, float fTransParam, float fRotParam, float fScaleParam)
	{
		UINT i=0;

		if (m_SequenceArray[iSeq].bSelectBones)
		{
			for (i=0; i<m_AnimArray.size(); i++)
			{
				KeyframeData *pkeyframe = keyframedata[i];
				for (UINT j=0; j<m_SequenceArray[iSeq].bonesname.size(); j++)
				{
					if (m_SequenceArray[iSeq].bonesname[j] == m_AnimArray[i].m_bone.m_Name)
					{
						pkeyframe->boneid = i;
						pkeyframe->bonename = m_AnimArray[i].m_bone.m_Name;
						pkeyframe->type = (InterpolType)m_AnimArray[i].m_itype;
						pkeyframe->used = m_AnimArray[i].m_bUsed;

						BonesData bonesdata;
						bonesdata.seqid = iSeq;
						bonesdata.vNumKeys = Vector3(0.0f, 0.0f, 0.0f);

						ConvertPosKeyFrames(i, bonesdata, iSeq, fTransParam);
						ConvertRotKeyFrames(i, bonesdata, iSeq, fRotParam);
						ConvertScaleKeyFrames(i, bonesdata, iSeq, fScaleParam);

						pkeyframe->bonesdata.push_back(bonesdata);
					}
				}
			}
		}
		else
		{
			for (i=0; i<m_AnimArray.size(); i++)
			{
				KeyframeData *pkeyframe = keyframedata[i];

				pkeyframe->boneid = i;
				pkeyframe->bonename = m_AnimArray[i].m_bone.m_Name;
				pkeyframe->type = (InterpolType)m_AnimArray[i].m_itype;
				pkeyframe->used = m_AnimArray[i].m_bUsed;

				BonesData bonesdata;
				bonesdata.seqid = iSeq;
				bonesdata.vNumKeys = Vector3(0.0f, 0.0f, 0.0f);

				ConvertPosKeyFrames(i, bonesdata, iSeq, fTransParam);
				ConvertRotKeyFrames(i, bonesdata, iSeq, fRotParam);
				ConvertScaleKeyFrames(i, bonesdata, iSeq, fScaleParam);

				pkeyframe->bonesdata.push_back(bonesdata);
			}
		}
	}

	//---------------------------------------------------------------------------
	/*
	void HierConverter::ConvertKeyFrames(SkeletonAnimData *panim)
	{
		float fTrans = ((float)TIME_TICKSPERSEC)/GetTicksPerFrame()/1000;
		for (UINT i=0; i<keyframedata.size(); i++)
		{
			const KeyframeData &data = keyframedata[i];

			UINT nTransKeys,nRotKeys,nScalKeys;
			nTransKeys = nRotKeys = nScalKeys = 0;
			for (UINT nframes=0; nframes<data.bonesdata.size(); nframes++)
			{			
				nTransKeys += data.bonesdata[nframes].posarray.size();
				nRotKeys += data.bonesdata[nframes].rotarray.size();
				nScalKeys += data.bonesdata[nframes].scalearray.size();
			}

			if ((nTransKeys == 0) || (nRotKeys == 0) || (nScalKeys == 0))
			{
				continue;
			}

			BoneTrack *ptrack = new BoneTrack;
			ptrack->m_BoneID = data.boneid;

			ptrack->m_TranslateKeys.m_Type = ptrack->m_RotateKeys.m_Type = ptrack->m_ScaleKeys.m_Type = data.type;
			//ptrack->m_TranslateKeys.m_bUsed = ptrack->m_RotateKeys.m_bUsed = ptrack->m_ScaleKeys.m_bUsed = data.used;

			assert((nTransKeys>0) && (nRotKeys>0) && (nScalKeys>0));

			ptrack->m_TranslateKeys.m_Array.resize(nTransKeys);
			ptrack->m_TranslateKeys.m_Ranges.resize(m_SequenceArray.size());

			ptrack->m_RotateKeys.m_Array.resize(nRotKeys);
			ptrack->m_RotateKeys.m_Ranges.resize(m_SequenceArray.size());

			ptrack->m_ScaleKeys.m_Array.resize(nScalKeys);
			ptrack->m_ScaleKeys.m_Ranges.resize(m_SequenceArray.size());

			UINT TransOffset = 0;
			UINT RotOffset = 0;
			UINT ScaleOffset = 0;
			for (size_t nframes=0; nframes<data.bonesdata.size(); nframes++)
			{
				UINT seqsize, nseqs;

				seqsize = data.bonesdata[nframes].posarray.size();
				assert((TransOffset + seqsize) <= nTransKeys);
				for (nseqs=0; nseqs<seqsize; nseqs++)
				{
					ptrack->m_TranslateKeys.m_Array[TransOffset + nseqs].tick = data.bonesdata[nframes].postick[nseqs];
					ptrack->m_TranslateKeys.m_Array[TransOffset + nseqs].data = data.bonesdata[nframes].posarray[nseqs];
				}
				TransOffset += seqsize;

				seqsize = data.bonesdata[nframes].rotarray.size();
				assert((RotOffset + seqsize) <= nRotKeys);
				for (nseqs=0; nseqs<seqsize; nseqs++)
				{
					ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].tick = data.bonesdata[nframes].rottick[nseqs];
					ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].data = data.bonesdata[nframes].rotarray[nseqs];
					ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].data.w = -ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].data.w;
				}
				RotOffset += seqsize;

				seqsize = data.bonesdata[nframes].scalearray.size();
				assert((ScaleOffset + seqsize) <= nScalKeys);
				for (nseqs=0; nseqs<seqsize; nseqs++)
				{
					ptrack->m_ScaleKeys.m_Array[ScaleOffset + nseqs].tick = data.bonesdata[nframes].scaletick[nseqs];
					ptrack->m_ScaleKeys.m_Array[ScaleOffset + nseqs].data = data.bonesdata[nframes].scalearray[nseqs];
				}
				ScaleOffset += seqsize;
			}

			Vector3 vTemp = Vector3(0.0f, 0.0f, 0.0f);
			for (UINT s=0; s<m_SequenceArray.size(); s++)
			{
				ptrack->m_TranslateKeys.m_Ranges[s].begin = 0;
				ptrack->m_TranslateKeys.m_Ranges[s].end = -1;

				ptrack->m_RotateKeys.m_Ranges[s].begin = 0;
				ptrack->m_RotateKeys.m_Ranges[s].end = -1;

				ptrack->m_ScaleKeys.m_Ranges[s].begin = 0;
				ptrack->m_ScaleKeys.m_Ranges[s].end = -1;

				for (size_t nframes=0; nframes<data.bonesdata.size(); nframes++)
				{
					if (data.bonesdata[nframes].seqid == s)
					{
						ptrack->m_TranslateKeys.m_Ranges[s].begin = vTemp.x;
						ptrack->m_TranslateKeys.m_Ranges[s].end = vTemp.x + data.bonesdata[nframes].vNumKeys.x -1;

						ptrack->m_RotateKeys.m_Ranges[s].begin = vTemp.y;
						ptrack->m_RotateKeys.m_Ranges[s].end = vTemp.y + data.bonesdata[nframes].vNumKeys.y -1;

						ptrack->m_ScaleKeys.m_Ranges[s].begin = vTemp.z;
						ptrack->m_ScaleKeys.m_Ranges[s].end = vTemp.z + data.bonesdata[nframes].vNumKeys.z -1;

						vTemp += data.bonesdata[nframes].vNumKeys;
					}
				}
			}

			panim->addBoneTrack(ptrack);
		}
	}*/

	void HierConverter::convertBoneFrames(Ogre::AnimationData *panim)
	{
		float fTrans = ((float)TIME_TICKSPERSEC)/GetTicksPerFrame()/1000;
		for (UINT i=0; i<keyframedata.size(); i++)
		{
			const KeyframeData &data = *keyframedata[i];

			UINT nTransKeys,nRotKeys,nScalKeys;
			nTransKeys = nRotKeys = nScalKeys = 0;
			for (UINT nframes=0; nframes<data.bonesdata.size(); nframes++)
			{			
				nTransKeys += data.bonesdata[nframes].posarray.size();
				nRotKeys += data.bonesdata[nframes].rotarray.size();
				nScalKeys += data.bonesdata[nframes].scalearray.size();
			}

			if ((nTransKeys == 0) || (nRotKeys == 0) || (nScalKeys == 0))
			{
				continue;
			}

			BoneTrack *ptrack = new BoneTrack;
			ptrack->m_BoneName = data.bonename.c_str();
			ptrack->m_BoneID = data.boneid;
			ptrack->m_TranslateKeys.m_Type = ptrack->m_RotateKeys.m_Type = ptrack->m_ScaleKeys.m_Type = data.type;

			assert((nTransKeys>0) && (nRotKeys>0) && (nScalKeys>0));

			ptrack->m_TranslateKeys.m_Array.resize(nTransKeys);
			ptrack->m_TranslateKeys.m_Ranges.resize(m_SequenceArray.size());

			ptrack->m_RotateKeys.m_Array.resize(nRotKeys);
			ptrack->m_RotateKeys.m_Ranges.resize(m_SequenceArray.size());

			ptrack->m_ScaleKeys.m_Array.resize(nScalKeys);
			ptrack->m_ScaleKeys.m_Ranges.resize(m_SequenceArray.size());

			UINT TransOffset = 0;
			UINT RotOffset = 0;
			UINT ScaleOffset = 0;
			for (size_t nframes=0; nframes<data.bonesdata.size(); nframes++)
			{
				UINT seqsize, nseqs;

				seqsize = data.bonesdata[nframes].posarray.size();
				assert((TransOffset + seqsize) <= nTransKeys);
				for (nseqs=0; nseqs<seqsize; nseqs++)
				{
					ptrack->m_TranslateKeys.m_Array[TransOffset + nseqs].tick = data.bonesdata[nframes].postick[nseqs];
					ptrack->m_TranslateKeys.m_Array[TransOffset + nseqs].data = data.bonesdata[nframes].posarray[nseqs];
				}
				TransOffset += seqsize;

				seqsize = data.bonesdata[nframes].rotarray.size();
				assert((RotOffset + seqsize) <= nRotKeys);
				for (nseqs=0; nseqs<seqsize; nseqs++)
				{
					ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].tick = data.bonesdata[nframes].rottick[nseqs];
					ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].data = data.bonesdata[nframes].rotarray[nseqs];
					ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].data.w = -ptrack->m_RotateKeys.m_Array[RotOffset + nseqs].data.w;
				}
				RotOffset += seqsize;

				seqsize = data.bonesdata[nframes].scalearray.size();
				assert((ScaleOffset + seqsize) <= nScalKeys);
				for (nseqs=0; nseqs<seqsize; nseqs++)
				{
					ptrack->m_ScaleKeys.m_Array[ScaleOffset + nseqs].tick = data.bonesdata[nframes].scaletick[nseqs];
					ptrack->m_ScaleKeys.m_Array[ScaleOffset + nseqs].data = data.bonesdata[nframes].scalearray[nseqs];
				}
				ScaleOffset += seqsize;
			}

			Vector3 vTemp = Vector3(0.0f, 0.0f, 0.0f);
			for (UINT s=0; s<m_SequenceArray.size(); s++)
			{
				ptrack->m_TranslateKeys.m_Ranges[s].begin = 0;
				ptrack->m_TranslateKeys.m_Ranges[s].end = -1;

				ptrack->m_RotateKeys.m_Ranges[s].begin = 0;
				ptrack->m_RotateKeys.m_Ranges[s].end = -1;

				ptrack->m_ScaleKeys.m_Ranges[s].begin = 0;
				ptrack->m_ScaleKeys.m_Ranges[s].end = -1;

				for (size_t nframes=0; nframes<data.bonesdata.size(); nframes++)
				{
					if (data.bonesdata[nframes].seqid == s)
					{
						ptrack->m_TranslateKeys.m_Ranges[s].begin = vTemp.x;
						ptrack->m_TranslateKeys.m_Ranges[s].end = vTemp.x + data.bonesdata[nframes].vNumKeys.x -1;

						ptrack->m_RotateKeys.m_Ranges[s].begin = vTemp.y;
						ptrack->m_RotateKeys.m_Ranges[s].end = vTemp.y + data.bonesdata[nframes].vNumKeys.y -1;

						ptrack->m_ScaleKeys.m_Ranges[s].begin = vTemp.z;
						ptrack->m_ScaleKeys.m_Ranges[s].end = vTemp.z + data.bonesdata[nframes].vNumKeys.z -1;

						vTemp += data.bonesdata[nframes].vNumKeys;
					}
				}
			}

			panim->addBoneTrack(ptrack);
		}
	}

	AnimationData *HierConverter::buildAnimationData(float fTransParam, float fRotParam, float fScaleParam)
	{
		size_t i;
		AnimationData *panim = new AnimationData;

		if(m_SequenceArray.empty())
		{
			SEQUENCE_EXP_T s;
			s.sequence.id = 0;
			s.sequence.loopmode = ANIM_MODE_LOOP;
			s.sequence.time_start = m_animStart;
			s.sequence.time_end = m_animEnd;
			s.bSelectBones = false;
			s.bonesname.resize(0);

			m_SequenceArray.push_back(s);
		}

		keyframedata.resize(m_AnimArray.size());
		for(i=0; i<keyframedata.size(); i++)
		{
			keyframedata[i] = new KeyframeData;
		}

		panim->m_Sequences.resize(m_SequenceArray.size());
		for(i=0; i<m_SequenceArray.size(); i++)
		{
			SequenceDesc &seq = panim->m_Sequences[i];
			const SEQUENCE_EXP_T &src = m_SequenceArray[i];

			seq.id = src.sequence.id;
			seq.loopmode = src.sequence.loopmode;
			seq.time_start = src.sequence.time_start * 1000/TIME_TICKSPERSEC;
			seq.time_end = src.sequence.time_end * 1000/TIME_TICKSPERSEC;

			CalculateKeyframeDataBySequence(i, fTransParam, fRotParam, fScaleParam);
		}

		panim->m_TriggerArray.resize(m_TriggerArray.size());
		for(i=0; i<m_TriggerArray.size(); i++)
		{
			TriggerDesc &trig = m_TriggerArray[i];
			trig.tick = m_TriggerArray[i].tick;
			trig.type = m_TriggerArray[i].type;
		}

		convertBoneFrames(panim);
		convertMtlParamFrames(panim);

		return panim;
	}

	void HierConverter::convertMtlParamFrames(Ogre::AnimationData *panim)
	{
		for(size_t i=0; i<m_MtlAnimDataArray.size(); i++)
		{
			MtlAnimData &srcanim = m_MtlAnimDataArray[i];

			MaterialParamTrack *ptrack = new MaterialParamTrack;
			ptrack->m_MeshName = srcanim.meshname.c_str();
			ptrack->m_MtlName = srcanim.mtlname.c_str();
			ptrack->m_ParamName = srcanim.varname.c_str();

			if(srcanim.type == TYPE_FLOAT)
			{
				KeyFrameArray<float> *pkeyframes = new KeyFrameArray<float>(srcanim.nframe);
				pkeyframes->m_Type = INTERPOL_LINEAR;
				for(size_t iframe=0; iframe<srcanim.nframe; iframe++)
				{
					pkeyframes->setKeyFrame(iframe, srcanim.pticks[iframe], ((float *)srcanim.pkeyframes)[iframe]);
				}
				ptrack->m_pKeyframes = pkeyframes;
				ptrack->m_KeyType = Ogre::SPT_FLOAT;
			}
			else if(srcanim.type == TYPE_FRGBA)
			{
				KeyFrameArray<Vector3> *pkeyframes = new KeyFrameArray<Vector3>(srcanim.nframe);
				pkeyframes->m_Type = INTERPOL_LINEAR;
				for(size_t iframe=0; iframe<srcanim.nframe; iframe++)
				{
					pkeyframes->setKeyFrame(iframe, srcanim.pticks[iframe], ((Vector3 *)srcanim.pkeyframes)[iframe]);
				}
				ptrack->m_pKeyframes = pkeyframes;
				ptrack->m_KeyType = Ogre::SPT_FLOAT3;
			}
			else
			{
				assert(0);
			}

			panim->addMtlParamTrack(ptrack);
		}
	}

	//---------------------------------------------------------------------------
	Ogre::BoxBound HierConverter::BuildStaticMeshBoundingChunk(int cellid)
	{
		Box3 bTemp;

		for (int i=0; i<m_StaticMeshChunkArray.size(); i++)
		{
			if ((m_StaticMeshChunkArray[i].m_CellID == cellid) || (m_StaticMeshChunkArray[i].m_CellID == -1))
			{
				if(!bTemp.Contains(m_StaticMeshChunkArray[i].m_BoundingBox))
				{
					bTemp += m_StaticMeshChunkArray[i].m_BoundingBox;
				}
			}
		}

		Ogre::Matrix4 tm = ConvertMatrix3(Max2EngineMatrix());

		Ogre::BoxBound mybox;
		mybox.setRange(ConvertPoint3(bTemp.Min()) * tm, ConvertPoint3(bTemp.Max()) * tm);

		return mybox;
	}

	int HierConverter::ConvertBoundingData(INode *pMaxNode)
	{
		ObjectState os = pMaxNode->EvalWorldState(m_animStart);
		Object *obj = os.obj;
		if (!obj)
			return W3D_STAT_FAILED;

		if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		{
			TriObject *tri = NULL;
			tri = (TriObject *)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));

			if(tri)
			{
				Mesh *pkMesh = &(tri->GetMesh());
				assert(pkMesh);

				m_AnimBoundingBox = pkMesh->getBoundingBox(&pMaxNode->GetObjectTM(m_animStart));
				m_bHasBounding = true;
				return W3D_STAT_OK;
			}
		}

		return W3D_STAT_FAILED;
	}

	//---------------------------------------------------------------------------
	//获取每个mesh的bounding box，然后合成最大bounding box导出
	Ogre::BoxBound HierConverter::BuildBoundingChunk()
	{
		BoxBound boxbound;
		if(g_ExpOption.exptype == EXP_ANIM_MODEL)
		{
			if (!m_bHasBounding)
			{
				MessageBox(NULL, "没有为该动画模型需要添加包围盒", "Error", MB_OK);
				return boxbound;
			}

			Ogre::Matrix4 tm = ConvertMatrix3(Max2EngineMatrix());

			boxbound.setRange(ConvertPoint3(m_AnimBoundingBox.Min()) * tm, ConvertPoint3(m_AnimBoundingBox.Max()) * tm);
		}
		else
		{
			for(size_t i=0; i<m_MeshNodes.size(); i++)
			{
				Ogre::MeshData *pmesh = m_MeshNodes[i].pmesh;
				boxbound += pmesh->m_Bounding.getBox();
			}
		}

		return boxbound;
	}


	//---------------------------------------------------------------------------
	int HierConverter::ConvertBSPData(INode *pMaxNode)
	{
		ObjectState os = pMaxNode->EvalWorldState(m_animStart);
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
				int iNumFaces = pkMesh->getNumFaces();
				if (iNumFaces == 0)
				{
					assert(0 && "BSP has no faces");
					return W3D_STAT_FAILED;
				}
				else if (iNumFaces > USHRT_MAX)
				{
					assert(0 && "has more than max maximum faces");
					return W3D_STAT_FAILED;
				}

				BSPNodeData bspnode;
				bspnode.pbsp = new BSPData;
				bspnode.pmaxnode = pMaxNode;

				int iNumMats = ConvertBSPMtl(pMaxNode, bspnode.pbsp);

				Ogre::Matrix4 tm;
				if(g_ExpOption.exptype == EXP_LINK_MESH)
				{
					::Matrix3 parenttm = pMaxNode->GetParentTM(m_animStart);
					::Matrix3 mytm = pMaxNode->GetObjectTM(m_animStart) * Inverse(parenttm);
					parenttm.SetRow(3, ::Point3(0,0,0));
					mytm *= parenttm;
					tm = ConvertMatrix3(mytm*parenttm) * ConvertMatrix3(Max2EngineMatrix());
				}
				else tm = ConvertMatrix3(pMaxNode->GetObjectTM(m_animStart)) * ConvertMatrix3(Max2EngineMatrix());

				Point3 p;
				for (int i=0; i<pkMesh->getNumVerts(); i++)
				{
					p = pkMesh->getVert(i);	
					bspnode.pbsp->m_Vertex.push_back(ConvertPoint3(pkMesh->getVert(i)) * tm);
				}

				unsigned short iMaxVert;
				MtlID mID;
				for (int i=0; i<iNumFaces; i++)
				{
					for (int j=0; j<3; j++)
					{
						iMaxVert = pkMesh->faces[i].v[j];
						p = pkMesh->verts[iMaxVert];
						bspnode.pbsp->m_Index.push_back(iMaxVert);
					}

					if (iNumMats <= 1)
					{
						bspnode.pbsp->m_Index.push_back((unsigned short)0);
					}else
					{
						mID = pkMesh->getFaceMtlIndex(i)%iNumMats;
						bspnode.pbsp->m_Index.push_back(mID);
					}
				}

				m_BSPNodes.push_back(bspnode);
			}
		}

		return W3D_STAT_OK;
	}

	//---------------------------------------------------------------------------
	int HierConverter::ConvertBSPMtl(INode *pMaxNode, BSPData *pbsp)
	{
		Mtl *pMtl = pMaxNode->GetMtl();
		BSPData::SufaceMtl s;
		int iNumMats = 0;

		if (pMtl)
		{
			iNumMats = pMaxNode->GetMtl()->NumSubMtls();
			//多材质
			MultiMtl *pMM;
			Mtl *pTempMtl;
			TSTR strMtlName;

			pMM = (MultiMtl *)pMtl;

			bool bIsSaved;
			for (int i=0; i<iNumMats; i++)
			{
				bIsSaved = false;
				pTempMtl = pMM->GetSubMtl(i);
				strMtlName = pTempMtl->GetName();
				if (strstr(strMtlName, "normal"))
				{
					s.type = BSPData::MTL_NORMAL;
					pbsp->m_SufaceMtl.push_back(s);
					bIsSaved = true;
				}
				if (strstr(strMtlName, "walk"))
				{
					s.type = BSPData::MTL_WALK;
					pbsp->m_SufaceMtl.push_back(s);
					bIsSaved = true;
				}
				if (strstr(strMtlName, "collide"))
				{
					s.type = BSPData::MTL_COLLIDE;
					pbsp->m_SufaceMtl.push_back(s);
					bIsSaved = true;
				}

				//如果没有给材质命名成引擎格式会执行下面一行
				if (!bIsSaved)
				{
					s.type = BSPData::MTL_NORMAL;
					pbsp->m_SufaceMtl.push_back(s);
				}
			}
		}else
		{
			s.type = BSPData::MTL_NORMAL;
			pbsp->m_SufaceMtl.push_back(s);
		}

		return iNumMats;
	}

	Ogre::BSPData *HierConverter::getBSPData(INode *pnode)
	{
		for(size_t i=0; i<m_BSPNodes.size(); i++)
		{
			const BSPNodeData &bspnode = m_BSPNodes[i];
			if(bspnode.pmaxnode->GetParentNode() == pnode)
			{
				bspnode.pbsp->addRef();
				return bspnode.pbsp;
			}
		}

		return NULL;
	}

	//---------------------------------------------------------------------------
	int HierConverter::ConvertPortalData(INode *pMaxNode)
	{
		MaxPortal p;

		TSTR strUserData;
		TSTR strFront = "front";
		TSTR strBack = "back";
		TSTR strTemp;
		BOOL bFound = false;
		pMaxNode->GetUserPropBuffer(strUserData);

		bFound = pMaxNode->GetUserPropString(strFront, strTemp);
		if (bFound == false)
			MessageBox(NULL, "Portal的FrontCell名称设置出错", "Error", MB_OK);
		strcpy(p.strCellFront, strTemp);

		bFound = pMaxNode->GetUserPropString(strBack, strTemp);
		if (bFound == false)
			MessageBox(NULL, "Portal的BackCell名称设置出错", "Error", MB_OK);
		strcpy(p.strCellBack, strTemp);

		ObjectState os = pMaxNode->EvalWorldState(m_animStart);
		Object *obj = os.obj;
		if (!obj)
			return W3D_STAT_NO_RENDERER;

		if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		{
			TriObject *tri = NULL;
			tri = (TriObject *)obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));

			Matrix4 tm = ConvertMatrix3(pMaxNode->GetObjectTM(m_animStart)) *
				ConvertMatrix3(Max2EngineMatrix());

			if(tri)
			{
				Mesh *pkMesh = &(tri->GetMesh());

				pkMesh->buildNormals();
				Point3 pNormal = pkMesh->getFaceNormal(1);
				Vector3 vNormal = ConvertPoint3(pNormal) * tm;
				Normalize(vNormal);
				p.vNormal = vNormal;

				int iNumVerts = pkMesh->getNumVerts();
				int iNumFaces = pkMesh->getNumFaces();
				p.nNumPoints = iNumVerts;
				assert(iNumVerts<=10 && "Too Many Verts of Portal");

				Vector3 vTempVerts[10];
				int i=0;
				for (i=0; i<iNumVerts; i++)
				{
					Point3 p1=pkMesh->getVert(i);
					vTempVerts[i] = ConvertPoint3(pkMesh->getVert(i)) * tm;
				}

				for (i=0; i<iNumFaces; i++)
				{
					PortalMeshEdge pEdge;
					pEdge.beginVertIndex = pkMesh->faces[i].v[0];
					pEdge.endVertIndex = pkMesh->faces[i].v[1];
					m_PortalEdgeArray.push_back(pEdge);

					pEdge.beginVertIndex = pkMesh->faces[i].v[1];
					pEdge.endVertIndex = pkMesh->faces[i].v[2];
					m_PortalEdgeArray.push_back(pEdge);

					pEdge.beginVertIndex = pkMesh->faces[i].v[2];
					pEdge.endVertIndex = pkMesh->faces[i].v[0];
					m_PortalEdgeArray.push_back(pEdge);
				}

				//计算Portal的RealMesh
				Vector3 vFaceNormal1;
				Vector3 vFaceNormal2;
				for (i=0; i<m_PortalEdgeArray.size(); i++)
				{		
					bool bRealEdge = true;
					for (int j=0; j<iNumVerts; j++)
					{
						int p1 = m_PortalEdgeArray[i].beginVertIndex;
						int p2 = m_PortalEdgeArray[i].endVertIndex;
						if ((p1==j) || (p2 == j))
							continue;
						Vector3 v1 = vTempVerts[j]-vTempVerts[p1];
						Vector3 v2 = vTempVerts[j]-vTempVerts[p2];
						if (i==0)
						{					
							vFaceNormal1 = CrossProduct(v1, v2);
							continue;
						}
						else
							vFaceNormal2 = CrossProduct(v1, v2);

						if (i != 0)
						{
							if (DotProduct(vFaceNormal1, vFaceNormal2)<0)
								bRealEdge = false;
							continue;
						}
					}
					if (bRealEdge)
					{
						m_PortalRealEdgeArray.push_back(m_PortalEdgeArray[i]);
					}
				}

				for (i=0; i<10; i++)
				{
					p.vPos[i] = Vector3(0.0f, 0.0f, 0.0f);
				}

				p.vPos[0] = vTempVerts[m_PortalRealEdgeArray[0].beginVertIndex];
				p.vPos[1] = vTempVerts[m_PortalRealEdgeArray[0].endVertIndex];
				int iTempVertIndex = m_PortalRealEdgeArray[0].endVertIndex;
				for (i=2; i<iNumVerts; i++)
				{
					for (int j=0; j<m_PortalRealEdgeArray.size(); j++)
					{
						if (m_PortalRealEdgeArray[j].beginVertIndex == iTempVertIndex)
						{
							p.vPos[i] = vTempVerts[m_PortalRealEdgeArray[j].endVertIndex];
							iTempVertIndex = m_PortalRealEdgeArray[j].endVertIndex;
							break;
						}
					}
				}
			}
		}
		m_PortalArray.push_back(p);
		return W3D_STAT_OK;
	}
}
