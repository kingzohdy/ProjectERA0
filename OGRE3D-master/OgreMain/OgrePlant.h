
#ifndef __Plant_H__
#define __Plant_H__

#include "OgreRenderableObject.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgrePlantData.h"
#include "OgreVertexFormat.h"

namespace Ogre
{
	struct PlantVerts
	{
		Vector3 m_Pos;
		uint m_Color;
		Vector2 m_UV;
	};

	class Disturb
	{
	public:
		Disturb ()
		{
			IsDisturbing = false;
			GrassMoveDir = Vector3(0.0f, 0.0f, 1.0f);
			DisturbTime = 0.0f;
			DisturbValue = 0.0f;
		}

		~Disturb ()
		{

		}

		void Reset ()
		{
			IsDisturbing = false;
			GrassMoveDir = Vector3(0.0f, 0.0f, 1.0f);
			DisturbTime = 0.0f;
			DisturbValue = 0.0f;
		}

		bool IsDisturbing;		//< 是否正在被扰动
		Vector3 GrassMoveDir;	//< 人扰动草时，草的运动方向
		float DisturbTime;		//< 当前扰动了的时间
		float DisturbValue;		//< 扰动值
	};

	class _OgreExport PlantNode : public RenderableObject
	{
		DECLARE_RTTI(PlantNode)

	public:
		PlantNode(PlantSource* pSource = NULL);

		virtual void update(uint dtick);
		void updateGrassDisturb (
			Ogre::Vector3 PlayerCurPos, 
			uint dtick);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);

		bool init(PlantVecInfo_T &instdata, std::string fileName);

	protected:
		~PlantNode();

		bool         createVBIB();
		void         addPos( Vector3 *posset, Vector3 pos , float fScale ,int index );
		void         updateData(Vector3 *posset, uint *pcolors);

	protected:
		PlantSource   *m_pPlantSource;

		Matrix4 m_View;

	public:

		// 扰动
		Ogre::Vector3 m_PlayerPosBefore; // 上一帧角色的位置
		Ogre::Vector3 m_PlayerDir;
		static float ms_DisturbDistance;
		static float ms_DisturbTime;
		std::vector<Disturb> m_Disturbs;
		bool m_bHasDisturb;


		std::vector<Vector3>   m_PosSet; //< 草的位置
		Vector3 *m_VertexPos; //< 备份顶点位置，早扰动后，顶点数据不会毁坏

		VertexFormat m_VertexFormat;
		VertexData *m_pVB;
		IndexData *m_pIB;
		VertexDeclHandle m_VertDecl;
		Texture *m_pTexture;
		Material *m_pMtl;

		int                  m_nPosNum;		
		float                m_fHeight,m_fWidth ;
		Vector3              m_MovePos[4];
		float                m_fMovetime ;
		float                m_fMovetime1;

		Vector3 m_MinPos;
		Vector3 m_MaxPos;
	};

	class _OgreExport PlantSetNode : public RenderableObject
	{
		DECLARE_RTTI(PlantSetNode)

	public:
		PlantSetNode(PlantSource* psource = NULL);

		virtual void update(uint dtick);

		void updateGrassDisturb (
			Ogre::Vector3 PlayerCurPos, 
			uint dtick);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);

	protected:
		~PlantSetNode();

	protected:
		PlantSource   *m_pPlantSource;

	public:
		std::vector<PlantNode*>               m_PlantNodeSet;
	};
}

#endif