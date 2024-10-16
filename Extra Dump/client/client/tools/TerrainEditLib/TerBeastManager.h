
#ifndef TerBEASTMANAGER_H
#define TerBEASTMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "TerBeastInclude.h"
#include "utils.h"
#include "vecmath.h"
#include "xmlwriter.h"
#include "textures.h"
#include "OgreSingleton.h"
#include "OgreRenderTypes.h"
#include "OgreVector3.h"
#include "OgreColourValue.h"

namespace TE
{
	class BGroup
	{
	public:
		BGroup ()
		{
			mVertexData = 0;
			mIndexBuffer = 0;
			x = -1;
			z = -1;
			modelIndex = -1;
			meshIndex = -1;
			subMeshIndex = -1;
			IsTextureTarget = false;
		}
		~BGroup (){}

		std::string ObjectName;
		Ogre::VertexData *mVertexData;
		Ogre::IndexBuffer *mIndexBuffer;

		ILBInstanceHandle hMeshInstance;
		ILBTargetHandle hTarget;
		ILBTargetEntityHandle hEntity;
		int x;
		int z;
		std::string modelFileName;
		int modelIndex;
		int meshIndex;
		int subMeshIndex;

		bool IsTextureTarget;
	};

	class EditorCamera;

	class BHeader
	{
	public:
		BHeader ()
		{
			version = 100;
			type = -1;
			number = 0;
		}
		~BHeader ()
		{

		}

		int version;
		int type; // 0 地形，1模型，3地形贴图
		int number; // 地形块数量，或者模型数量
	};

	class BTLightData
	{
	public:
		BTLightData ()
		{
			xIndex = -1;
			zIndex = -1;
		}
		~BTLightData ()
		{

		}

		int xIndex;
		int zIndex;
		int number;
		std::vector<unsigned int> colors;
	};

	class BMLightData
	{
	public:
		BMLightData ()
		{
			modelIndex = -1;
			meshIndex = -1;
			subMeshIndex = -1;
			number = 0;
		}
		~BMLightData ()
		{
		}

		std::string modelFilename;
		int modelIndex;
		int meshIndex;
		int subMeshIndex;
		int number;
		std::vector<unsigned int> colors;
	};

	class BTLightMap
	{
	public:
		BTLightMap ()
		{
			x = -1;
			z = -1;
			nameLength = 0;
			texture = 0;
		}
		~BTLightMap ()
		{

		}

		int x;
		int z;
		int nameLength;
		std::string textureName;

		Ogre::TextureData *texture;
	};

	class BeastManager : public Ogre::Singleton<BeastManager>
	{
	public:
		BeastManager ();
		~BeastManager ();

		enum StaticLightMode
		{
			SLM_Vertex,
			SLM_Texture
		};

		void SetStaticLightMode (StaticLightMode mode);
		StaticLightMode GetStaticLightMode ();

		bool Initlize ();
		bool Terminate ();

		void Reset ();

		void BeginScene ();

		void AddObject (std::string name, Ogre::VertexData *pVB, Ogre::IndexData *pIB, 
			Ogre::PrimitiveType type, Ogre::Matrix4 tm, Ogre::Material *pMaterial = 0, int x=-1, int z=-1,
			std::string modelFileName="", int modelIndex = -1, int meshIndex=-1, int subMeshIndex=-1);
		void AddDirectLight (std::string name, Ogre::Vector3 dir, Ogre::ColourValue color);
		void SetAmbientColor (std::string name, Ogre::ColourValue color);
		void AddPointLight (std::string name, Ogre::Vector3 pos, Ogre::ColourValue color, 
			float fRange);
		void SetCamera (std::string name, EditorCamera *camera);
		void SetUseCameraTarget (bool use);
		bool IsUseCameraTarget ();

		ILBTextureHandle CreateBeastTexture (std::string name, Ogre::Texture *texture);

		void EndScene ();

		void CreateXML (const std::string &fileName, bool gi);
		bool DoBake ();
		void CollectLightData ();

		//bool SaveTerrainLightData (const std::string fileName);
		//bool LoadTerrainLightData (const std::string fileName);

		bool LoadLightMapXML (const std::string fileName);

		bool SaveTerrainLightMapData (const std::string fileName);
		bool LoadTerrainLightMapData (const std::string fileName);

		bool SaveModelLightData (const std::string fileName);
		bool LoadModelLightData (const std::string fileName);

		void SetBeastShow (bool show);
		bool IsBeastLightShow ();

		static std::string GetTerrainName (std::string hitName=std::string("RandName"), int x=0, int y= 0);
		static std::string GetRandName (std::string hitName=std::string("RandName"));
		static std::string GetRandLightName (std::string hitName=std::string("RandLightName"));

		static std::string msMapDir;
	private:
		void CreateAllTarget ();
		void CreateCameraTarget ();
		void AddPassToTargets ();
		void CopyUsefulTexture(std::string mapDir);

		ILBManagerHandle mManagerHandle;
		ILBSceneHandle mScene;
		ILBCameraHandle mCamera;
		ILBJobHandle mJob;
		ILBRenderPassHandle mPass;

		std::string mXMLFilename;

		typedef std::map<std::string, BGroup> VBGROUP;
		VBGROUP mGroups;

		std::vector<ILBTargetHandle> mTargets;

		BHeader mTerrainHead;
		BHeader mModelHead;
		std::vector<BTLightData> mTLightDatas;
		std::vector<BMLightData> mMLightDatas;

		BHeader mTerrainLightMapHead;
		std::vector<BTLightMap> mTLightMap;

		std::map<Ogre::Texture*, ILBTextureHandle> mTextureMap;

		bool m_bBeastShow;
		Ogre::ColourValue m_DirectionColor;
		bex::ColorRGBA m_AmbientColor;
		bool m_bUseCameraTarget;

		StaticLightMode m_Mode;

		static int msIndex;
		static int msLightIndex;
	};

}

#endif