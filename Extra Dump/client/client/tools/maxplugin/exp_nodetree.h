
#ifndef __EXP_NODETREE_H__
#define __EXP_NODETREE_H__

#include "exp_types.h"

namespace DKEXP
{
	class HierConverter
	{
	public:
		struct StaticMeshData
		{
			Box3 m_BoundingBox;
			int m_CellID;
			std::string meshname;
			Ogre::SubMeshData *psubmesh;
		};

		struct MeshNodeData
		{
			Ogre::MeshData *pmesh;
			INode *pmaxnode;
		};
		struct BSPNodeData
		{
			Ogre::BSPData *pbsp;
			INode *pmaxnode;
		};

		struct ANCHOR_T
		{
			int boneid;
			int anchorid;
			/*
			Ogre::Vector3 offset;

			Ogre::Vector3 trans;
			Ogre::Quaternion quat;
			*/
		};

		struct GeomNode
		{
			INode* pNode;
			int cellid;
		};

		struct MaxCell
		{
			char cellname[256];

			int m_nOutdoorLighting;
			int m_nIndoor;
		};

		struct MaxPortal
		{
			Ogre::Vector3			vPos[10];
			Ogre::Vector3			vNormal;

			int				nNumPoints;							// 点数目
			char			strCellFront[256];					// 正面连接的区域
			char			strCellBack[256];					// 反面连接的区域
		};

		struct PortalMeshEdge
		{
			int beginVertIndex;
			int endVertIndex;
		};

		int ConvertGeom(GeomNode node);
		int ConvertStaticMeshes(INode *pMaxNode, const int iMaxAttr, const int iAttrId, int cellid);
		int ConvertMeshes(INode *pMaxNode, const int iMaxAttr, const int iAttrId, Ogre::MeshData *pmeshdata);
		int BuildNode(Interface *pIntf, INode* pMaxNode);
		int Convert();
		int ConvertBSPData(INode *pMaxNode);
		int ConvertBillboardData(INode *pMaxNode);
		int ConvertBSPMtl(INode *pMaxNode, Ogre::BSPData *pbsp);
		Ogre::BSPData *getBSPData(INode *pnode); //bsp node must be the child of pnode
		const char *ID2Name(int id);

		HierConverter(TimeValue animStart,TimeValue animEnd);
		~HierConverter();

		Ogre::VertexData *BuildVertexChunk(const InflBoneArray &pBonearray, int numuv, bool skin);
		Ogre::IndexData *BuildIndexChunk(InflBoneArray &pBonearray, bool swap_index);
		void BuildSkinInfoChunk(Ogre::SubMeshData *psubmeshdata, InflBoneArray &pBonearray);

		Ogre::SkeletonData *BuildSkeletonChunk(INode* pMaxNode);
		Ogre::AnimationData *buildAnimationData(float fTransParam, float fRotParam, float fScaleParam);

		Ogre::BoxBound BuildStaticMeshBoundingChunk(int cellid);
		Ogre::BoxBound BuildBoundingChunk();

		int ConvertPortalData(INode *pMaxNode);

	public:
		std::vector<StaticMeshData> m_StaticMeshChunkArray;
		std::vector<MeshNodeData>m_MeshNodes;
		std::vector<CameraConverter> m_CameraArray;
		std::vector<LightConverter> m_LightArray;

		std::vector<INode *> m_pBoneNodes;
		std::vector<int>m_BoneNodeTypes; //0:正常node, 1:需要清除scale信息

		std::vector<BSPNodeData>m_BSPNodes;

		std::vector<ANCHOR_T> m_Anchors;

		int m_CellNum;
		std::vector<MaxCell> m_CellArray;
		std::vector<MaxPortal> m_PortalArray;
		std::vector<PortalMeshEdge> m_PortalEdgeArray;
		std::vector<PortalMeshEdge> m_PortalRealEdgeArray;

		bool m_bHasBounding;	//是否有动画模型包围盒

		static bool m_bExpOnly;	//只导出选中的sequence

	private:
		TimeValue m_animStart;
		TimeValue m_animEnd;

		std::vector<GeomNode> m_pGeomNodes;

		std::vector<AnimationConverter> m_AnimArray;
		std::vector<SEQUENCE_EXP_T> m_SequenceArray;
		//std::vector<SEQUENCE_MTLANI_T> m_MtlAniArray;
		std::vector<Ogre::TriggerDesc> m_TriggerArray;

		struct BonesData
		{
			int seqid;
			Ogre::Vector3 vNumKeys;
			std::vector<Ogre::Vector3> posarray;
			std::vector<UINT> postick;

			std::vector<Ogre::Quaternion> rotarray;
			std::vector<UINT> rottick;

			std::vector<Ogre::Vector3> scalearray;
			std::vector<UINT> scaletick;

		};

		struct KeyframeData
		{
			int boneid;
			Ogre::String	bonename;
			Ogre::InterpolType type;
			bool used;
			std::vector<BonesData> bonesdata;
		};

		std::vector<KeyframeData *>keyframedata;

		Box3 m_AnimBoundingBox;

		std::vector<MtlAnimData> m_MtlAnimDataArray;

	private:
		//void ConvertKeyFrames(Ogre::SkeletonAnimData *panim);
		void convertBoneFrames(Ogre::AnimationData *panim);
		void convertMtlParamFrames(Ogre::AnimationData *panim);
		void ConvertPosKeyFrames(int boneid, BonesData &bonesdata, int iSeq, float fTransParam);
		void ConvertRotKeyFrames(int boneid, BonesData &bonesdata, int iSeq, float fRotParam);
		void ConvertScaleKeyFrames(int boneid, BonesData &bonesdata, int iSeq, float fScaleParam);
		void CalculateKeyframeDataBySequence(int iSeq, float fTransParam, float fRotParam, float fScaleParam);
		int ConvertBoundingData(INode *pMaxNode);
	};
}

#endif