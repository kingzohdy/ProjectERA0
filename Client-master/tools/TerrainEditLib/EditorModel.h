#pragma once

#include "MapLights.h"

namespace TE
{

	class EditorModel
	{
	public:
		EditorModel(void);
		virtual ~EditorModel(void);

		virtual bool LoadModel(const char* filename  );	
		virtual bool LoadBuildingModel(const char* filename );

		virtual void SetPos(Ogre::Vector3 pos);
		virtual void SetDir(Ogre::Vector3 dir);
		virtual void SetScale(float scale);

		virtual void Release();

		void          SetShowModel( Ogre::Entity * pModel ){ m_pModel = pModel ; }

		Ogre::Entity*                        GetShowModel(){ return m_pModel ; }

		void               Update( float time );
		void               UpdateMesh(Ogre::Vector3 startPos);

		void                UpdateNormal();
		float             GetLinkMeshHeight( int index );
		bool HasLinkMesh(){ return m_pLinkMesh!=NULL; }
		void SetLinkMeshTexture(Ogre::Texture * ptex);

		void             ClearLightColor();
		void             CountLightColor( LightInfo &lightInfo );

	private:
		Ogre::Entity                          *m_pModel;
		Ogre::MeshInstance                 *m_pLinkMesh;

		Ogre::Vector3                                   m_wPos,m_wSavePos;	
		Ogre::Vector3                                   m_Dir;	
		float                                   m_Scale;
		//����Ķ�������
		std::vector<Ogre::Vector3>                       m_LinkMeshVec;

		enum   MODEL_TYPE
		{
			LINK_MESH = 0 ,
			LINK_TEXTURE ,
			PAINT_TEXTURE_MESH ,
			BRAE_MESH,//б��
		};
		//��Ӧ��ģ������
		MODEL_TYPE                           m_ucModleType;

	public:
		//��ӦLINKMESH�ĸ߶�����
		std::vector<float>                       m_LinkMeshHeight;
		//��ӦLINKMESH����ɫ����
		std::vector<unsigned int>                m_LinkMeshColor;
	private:
		void UpdatePos();
	};

};