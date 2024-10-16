
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

		bool IsDisturbing;		//< �Ƿ����ڱ��Ŷ�
		Vector3 GrassMoveDir;	//< ���Ŷ���ʱ���ݵ��˶�����
		float DisturbTime;		//< ��ǰ�Ŷ��˵�ʱ��
		float DisturbValue;		//< �Ŷ�ֵ
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

		// �Ŷ�
		Ogre::Vector3 m_PlayerPosBefore; // ��һ֡��ɫ��λ��
		Ogre::Vector3 m_PlayerDir;
		static float ms_DisturbDistance;
		static float ms_DisturbTime;
		std::vector<Disturb> m_Disturbs;
		bool m_bHasDisturb;


		std::vector<Vector3>   m_PosSet; //< �ݵ�λ��
		Vector3 *m_VertexPos; //< ���ݶ���λ�ã����Ŷ��󣬶������ݲ���ٻ�

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