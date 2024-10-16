
#ifndef __EXP_MESH_H__
#define __EXP_MESH_H__

#include "exp_types.h"

namespace DKEXP
{
	class MeshConverter
	{
	public:

		struct BoneInitParam
		{
			unsigned int	boneid;
			unsigned int	nodeid;
			Ogre::Matrix4	offsettm;   //attach skin到骨骼上的时候， 骨骼矩阵的逆矩阵
		};

		int Convert(INode *pMaxNode, std::vector<INode* > &pBoneNodes, const int iMaxAttr, const int iAttrId, std::vector<MtlAnimData> &MtlAnimDataArray);

		bool FaceUsesOtherMaterial(Mesh* pMesh, int iFace, const int iMaxAttr, const int iAttrId);
		Point3 GetVertexNormal(Mesh* pkMesh, int iFaceNo, RVertex* pRV);
		void ConvertNormal(INode *pMaxNode, Mesh *pkMesh, const int iMaxAttr, const int iAttrId);
		void ConvertVertexcolor(Mesh *pkMesh, const int iMaxAttr, const int iAttrId);
		int ConvertUVs(INode *pMaxNode, Mesh *pkMesh, const int iMaxAttr, const int iAttrId); //return num of uvset
		bool ConvertTangentBinormal(int iNumFaces);

		static void Preprocess(INode* pMaxNode);
		static void Postprocess(INode* pMaxNode);
		static Modifier* FindSkinModifier(INode *pMaxNode);
		static Modifier *MeshConverter::FindPhysiqueModifier(INode *pINode);

		static int FindBoneIDFromArray(const std::vector<INode* > &pBoneNodes, INode *pbone);

		void AddOneBone( unsigned int id  ,unsigned int boneid);
		void ConvertOffsetMatrix(INode *pMaxNode, ISkin *pSkin);
		void ConvertOffsetMatrix(INode *pMaxNode, IPhysiqueExport *pPhysiqueExport);
		bool GetSkinBoneVertexData(INode *pMaxNode,  Mesh *pkMesh, std::vector<INode* > &pBoneNodes,
			const int iMaxAttr, const int iAttrId);
		void ConvertSkin(IPhyContextExport *pcontext, INode* pMaxNode, Mesh *pkMesh, 
			const std::vector<INode* > &pBoneNodes, const int iMaxAttr, const int iAttrId);
		void ConvertSkin(ISkin *pSkin, ISkinContextData *pSkinContextData, INode* pMaxNode,
			Mesh *pkMesh, const std::vector<INode* > &pBoneNodes, const int iMaxAttr, const int iAttrId);

		//顶点动画
		Ogre::MorphAnimData *ConvertVertAnimation(INode *pMaxNode, int iNumUsedFaces, const int iMaxAttr, const int iAttrId);
		int ConvertVertAnimFrameData(INode *pMaxNode, int iFrame, int iNumUsedFaces, Ogre::MorphAnimData *pvertanim, bool bGetBindBox, const int iMaxAttr, const int iAttrId);
		//char* pVertData, MAY::Vector3 *pVertMin, MAY::Vector3 *pVertMax);
		void ConvertNormal(INode *pMaxNode, Mesh *pkMesh, int iFrame, int iStride, int iNumUsedFaces, Ogre::MorphAnimData *pvertanim,
			const int iMaxAttr, const int iAttrId);
		void ConvertUVs(INode *pMaxNode, Mesh *pkMesh, int iFrame, int iStride,  unsigned int iNumUsedFaces, Ogre::MorphAnimData *pvertanim, bool bGetBindBox, const int iMaxAttr, const int iAttrId);
		void GetBindBox(int iNumVerts, Ogre::Vector3 *pVertPos, Ogre::Vector3 *pMinVert, Ogre::Vector3 *pMaxVert);
		void ConvertBoundingBox(INode *pMaxNode);
		void ComputeBoundingBox(INode *pMaxNode);

		MeshConverter(TimeValue animStart,TimeValue animEnd);
		~MeshConverter();


	public:
		std::vector<VertexData> m_MaxVertex;
		std::vector<BoneInitParam> m_BoneParams;
		Ogre::Material *m_pMtl;

		::Matrix3 m_SkinInitTM; //attach skin到骨骼上的时候， skin的矩阵
		bool m_bSkin;

		Box3 m_BoundingBox;
		Ogre::Vector3 m_MinVertexPos;
		Ogre::Vector3 m_MaxVertexPos;

		unsigned short m_iNumUVChannels;
		bool m_NeedSwapIndex;
	protected:

	private:
		TimeValue m_animStart;
		TimeValue m_animEnd;

		Ogre::PrimitiveType m_PrimType;
		unsigned int m_nPrimitive;

	};
}

#endif