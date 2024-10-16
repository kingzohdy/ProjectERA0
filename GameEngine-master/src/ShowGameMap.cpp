
#include "ShowGameMap.h"

#include "OgreArchive.h"
#include "OgreBitArray.h"
#include "OgreGameTerrainScene.h"
#include "OgreTexture.h"
#include "OgreResourceManager.h"
#include "OgreLight.h"
#include "OgreFog.h"
#include "OgreSoundNode.h"
#include "OgreEntity.h"
#include "OgreOSUtility.h"
#include "OgreFootprint.h"
#include "OgreBorderScene.h"
#include "GameMapRegion.h"
#include "OgreRenderSystem.h"
#include <sstream>
#include <algorithm>
#include <vector>
#include "OgreXMLData.h"
#include "OgreRoot.h"
using namespace std;

#define SKYBOX_RADIUS		25000.0f

namespace Ogre
{
	ShowGameMap::ShowGameMap() : m_CenterPos(0,0,0), m_pCurRegion(NULL), m_pAmbientMgr(NULL), m_pRegionSet(NULL), m_pFootprints(NULL), m_pScene(NULL)
	{
		m_LoadProgress = 0.0f;
		m_pMinMapTex = NULL;
		m_pMapMaskTex = NULL;
	}

	ShowGameMap::~ShowGameMap()
	{
		delete m_pAmbientMgr;
		delete m_pRegionSet;
		OGRE_RELEASE(m_pFootprints);
		OGRE_RELEASE(m_pScene);
		OGRE_RELEASE( m_pMapMaskTex );
		OGRE_RELEASE( m_pMinMapTex );
	}

	static void ReadStr(FixedString &str, DataStream *fp)
	{
		int len;
		char buffer[256];
		fp->read(len);
		assert(len < 255);
		fp->read(buffer, len);
		buffer[len] = 0;

		str = buffer;
	}

	static void LoadLightsIntoScene(GameTerrainScene *pscene, const char *filename)
	{
		DataStream *fp = ResourceManager::getSingleton().openFileStream(filename, true);
		if(fp == NULL)
		{
			return;
		}

		int version, nlight;
		fp->read(version);
		fp->read(nlight);

		struct LightInfo_100
		{
			Vector3  pos;
			float  range;
			int    type;
			ColourValue  color;
		};

		struct LightInfo
		{
			int nType;//0--点光源， 1--音效源
			Ogre::Vector3 pos;
			float  range;
			Ogre::ColourValue color;
			float volume;
			FixedString name;
			FixedString respath;

			// 102
			float time0;
			float time1;

			// 103
			float fSoundFullRange;

			// 104
			int iIsLoop;
		}lightinfo;

		for(int i=0; i<nlight; i++)
		{
			if(version <= 100)
			{
				LightInfo_100 oldinfo;
				fp->read(oldinfo);

				lightinfo.nType = 0;
				lightinfo.pos = oldinfo.pos;
				lightinfo.range = oldinfo.range;
				lightinfo.color = oldinfo.color;

				lightinfo.time0 = 0.0f;
				lightinfo.time1 = 0.0f;
				lightinfo.fSoundFullRange = 0.0f;
			}
			else if (version == 101)
			{
				fp->read(lightinfo.nType);
				fp->read(lightinfo.pos);
				fp->read(lightinfo.range);
				fp->read(lightinfo.color);
				fp->read(lightinfo.volume);
				ReadStr(lightinfo.name, fp);
				ReadStr(lightinfo.respath, fp);

				lightinfo.time0 = 0.0f;
				lightinfo.time1 = 0.0f;
				lightinfo.fSoundFullRange = 0.0f;
			}
			else if (version == 102)
			{
				fp->read(lightinfo.nType);
				fp->read(lightinfo.pos);
				fp->read(lightinfo.range);
				fp->read(lightinfo.color);
				fp->read(lightinfo.volume);
				ReadStr(lightinfo.name, fp);
				ReadStr(lightinfo.respath, fp);

				fp->read(lightinfo.time0);
				fp->read(lightinfo.time1);

				lightinfo.fSoundFullRange = 0.0f;
			}
			else if (version == 103)
			{
				fp->read(lightinfo.nType);
				fp->read(lightinfo.pos);
				fp->read(lightinfo.range);
				fp->read(lightinfo.color);
				fp->read(lightinfo.volume);
				ReadStr(lightinfo.name, fp);
				ReadStr(lightinfo.respath, fp);

				fp->read(lightinfo.time0);
				fp->read(lightinfo.time1);

				fp->read(lightinfo.fSoundFullRange);
			}
			else if (version == 104)
			{
				fp->read(lightinfo.nType);
				fp->read(lightinfo.pos);
				fp->read(lightinfo.range);
				fp->read(lightinfo.color);
				fp->read(lightinfo.volume);
				ReadStr(lightinfo.name, fp);
				ReadStr(lightinfo.respath, fp);

				fp->read(lightinfo.time0);
				fp->read(lightinfo.time1);

				fp->read(lightinfo.fSoundFullRange);

				fp->read(lightinfo.iIsLoop);
			}
			
			if(lightinfo.nType == 0)
			{
				Light *plight = new Light(LT_POINT);
				plight->setStatic(true);
				plight->setPosition(lightinfo.pos);
				plight->setColor(lightinfo.color);
				plight->m_Range = lightinfo.range;

				plight->attachToScene(pscene);
				plight->release();
			}
			else
			{
				SoundData *psounddata = new SoundData;
				psounddata->m_strFileName = lightinfo.respath;
				psounddata->m_PlayMode = PM_LIMITED;
				psounddata->m_Ambient = 1;
				psounddata->m_CreateInfo.isloop = (1==lightinfo.iIsLoop);
				psounddata->m_CreateInfo.mindistance = 0;
				psounddata->m_CreateInfo.maxdistance = lightinfo.range;
				psounddata->m_CreateInfo.volume = lightinfo.volume;
				psounddata->m_CreateInfo.pos = lightinfo.pos;
				psounddata->m_CreateInfo.velocity = Vector3(0,0,0);
				psounddata->m_CreateInfo.time0 = lightinfo.time0;
				psounddata->m_CreateInfo.time1 = lightinfo.time1;
				psounddata->m_CreateInfo.fSoundFullRange = lightinfo.fSoundFullRange;

				SoundNode *psound = new SoundNode(psounddata, false);
				psound->setPosition(lightinfo.pos);
				psound->setRandomTime0(lightinfo.time0);
				psound->setRandomTime1(lightinfo.time1);
				psound->setSoundFullRange(lightinfo.fSoundFullRange);
				psounddata->release();
				psound->attachToScene(pscene);
				psound->release();
			}
		}

		delete fp;
	}

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
		int type; // 0 地形，1模型
		int number; // 地形块数量，或者模型数量
	};


	static void LoadTerrainStaticLight (GameTerrainScene *pscene, const char *filename)
	{
		DataStream *fp = ResourceManager::getSingleton().openFileStream(filename, true);
		if(fp == NULL)
			return;

		pscene->mTLightDatas.clear();

		BHeader0 terrainHead;
		fp->read(&terrainHead, sizeof(terrainHead));

		for (int i=0; i<terrainHead.number; i++)
		{
			BTLightData0 tLD;

			if (100 == terrainHead.version)
			{
				int xIndex = -1;
				fp->read(&xIndex, sizeof(xIndex));
				int zIndex = -1;
				fp->read(&zIndex, sizeof(zIndex));
				int vertexNum = 0;
				fp->read(&vertexNum, sizeof(vertexNum));
				vector<unsigned int> colors;
				colors.resize(vertexNum);
				fp->read(&colors[0], sizeof(unsigned int)*vertexNum);

				tLD.xIndex = xIndex;
				tLD.zIndex = zIndex;
				tLD.number = vertexNum;
				tLD.colors = colors;
			}

			pscene->mTLightDatas.push_back(tLD);
		}

		delete fp;

		return;
	}

	static void LoadModelStaticLight (GameTerrainScene *pscene, const char *filename)
	{
		DataStream *fp = ResourceManager::getSingleton().openFileStream(filename, true);
		if(fp == NULL)
			return;

		pscene->mMLightDatas.clear();

		BHeader modelHead;
		fp->read(&modelHead, sizeof(modelHead));

		for (int i=0; i<modelHead.number; i++)
		{
			BMLightData0 mLD;

			std::string modelFilename;
			int modelID;
			int meshIndex;
			int subMeshIndex;
			int number;
			std::vector<unsigned int> colors;

			if (100 == modelHead.version)
			{
				int nameSize;
				fp->read(&nameSize, sizeof(nameSize));
				std::string modelName;
				modelName.resize(nameSize);
				fp->read(&modelName[0], sizeof(char)*nameSize);
				int modelIndex;
				fp->read(&modelIndex, sizeof(int));
				int meshIndex;
				fp->read(&meshIndex, sizeof(int));
				int subMeshIndex;
				fp->read(&subMeshIndex, sizeof(int));
				int number;
				fp->read(&number, sizeof(int));
				std::vector<unsigned int> colors;
				colors.resize(number);
				fp->read(&colors[0], sizeof(unsigned int)*number);

				mLD.modelFilename = modelName;
				mLD.modelIndex = modelIndex;
				mLD.meshIndex = meshIndex;
				mLD.subMeshIndex = subMeshIndex;
				mLD.number = number;
				mLD.colors = colors;
			}

			pscene->mMLightDatas.push_back(mLD);
		}

		delete fp;

		return;
	}

	static bool LoadLightMapXML (GameTerrainScene *pscene, std::string fileName)
	{
		std::string xmlName = fileName+"BeastScene.xml";

		XMLData xmldata;

		if(!xmldata.loadFile(xmlName))
		{
			return false;
		}

		for (int i=0; i<(int)(pscene->mTLightMaps.size()); i++)
		{
			if (pscene->mTLightMaps[i].texture)
				pscene->mTLightMaps[i].texture->release();
		}

		pscene->mTLightMaps.clear();

		XMLNode rootNode = xmldata.getRootNode();

		if (!rootNode.isNull())
		{
			XMLNode instanceNode = rootNode.iterateChild();

			while (!instanceNode.isNull())
			{
				const char *name = instanceNode.getName();
				std::string strName(name);

				if ("Instance" == strName)
				{
					XMLNode childNode = instanceNode.getChild("Name");
					if (!childNode.isNull())
					{
						const char *text = childNode.getText();
						std::string modeName(text);

						bool isTerrain = (string::npos != modeName.find("Terrain"));
						if (isTerrain)
						{
							int pos0 = modeName.find_first_of('_');
							int pos1 = modeName.find_last_of('_');

							int xSize = pos1 - pos0 - 1;
							int zSize = modeName.size() - pos1 - 1;

							char X[12];
							char Z[12];
							modeName.copy(X, xSize, pos0+1);
							modeName.copy(Z, zSize, pos1+1);
							X[xSize] = '\0';
							Z[zSize] = '\0';

							int terX = atoi(X);
							int terZ = atoi(Z);

							int a = 0;

							XMLNode shapeNode = instanceNode.getChild("Shape");

							const char *shapeText = shapeNode.getText();
							std::string textureName(shapeText);
							textureName = fileName + textureName + ".png";
							// 加载贴图
							Ogre::TextureData *texture= (Ogre::TextureData*)(
								ResourceManager::getSingleton().blockLoad(textureName.c_str()));
							if (texture)
							{
								BTLightMap0 lmap;
								lmap.x = terX;
								lmap.z = terZ;
								lmap.texture = texture;

								pscene->mTLightMaps.push_back(lmap);
							}
						}
					}
				}

				instanceNode = rootNode.iterateChild(instanceNode);
			}
		}

		return true;
	}

	static void LoadLightMap (GameTerrainScene *pscene, std::string fileName, std::string dirName)
	{
		DataStream *fp = ResourceManager::getSingleton().openFileStream(fileName, true);
		if(fp == NULL)
			return;

		for (int i=0; i<(int)(pscene->mTLightMaps.size()); i++)
		{
			if (pscene->mTLightMaps[i].texture)
				pscene->mTLightMaps[i].texture->release();
		}

		pscene->mTLightMaps.clear();

		BHeader modelHead;
		fp->read(&modelHead, sizeof(modelHead));

		for (int i=0; i<modelHead.number; i++)
		{
			BTLightMap0 lmap;

			if (100 == modelHead.version)
			{
				int x;
				int z;
				int nameSize;
				std::string textureName;

				fp->read(&x, sizeof(x));
				fp->read(&z, sizeof(z));
				fp->read(&nameSize, sizeof(nameSize));
				textureName.resize(nameSize);
				fp->read(&textureName[0], sizeof(char)*nameSize);

				textureName = dirName + textureName;

				// 加载贴图
				Ogre::TextureData *texture= (Ogre::TextureData*)(ResourceManager::getSingleton().blockLoad(textureName.c_str()));
				if (texture)
				{
					BTLightMap0 lmap;
					lmap.x = x;
					lmap.z = z;
					lmap.texture = texture;

					pscene->mTLightMaps.push_back(lmap);
				}
			}
		}

		delete fp;

		return;
	}

	void ShowGameMap::attachToRenderer(Ogre::SceneRenderer *prenderer)
	{
		prenderer->setRenderScene(m_pScene);
	}

	void ShowGameMap::detachFromRenderer(Ogre::SceneRenderer *prenderer)
	{
		prenderer->setRenderScene(NULL);
	}

	bool ShowGameMap::LoadFromMapDesc(const MAPDEF &desc, const WorldPos& wPosCacheAreaCenter)
	{

		m_nTileX = 1;
		m_nTileZ = 1;

		m_strMapName = desc.Name;

		m_TileTable.resize(m_nTileX*m_nTileZ, false);

		m_nMapID = desc.MapID;
		m_MapType = MAP_TERRAIN;

		char chunkname[256], minmapname[256];
		sprintf(chunkname, "maps\\%d\\%d.tile", m_nMapID, m_nMapID);
		m_strMapResName = chunkname;
		sprintf( minmapname, "map.%d_%d_%d_s", m_nMapID, 0, 0);
		m_MiniMapResName = minmapname;
		Resource *pres = ResourceManager::getSingleton().blockLoad(chunkname);

		TerrainTileSource *ptiledata = static_cast<TerrainTileSource *>(pres);
		m_nWidthGrid = ptiledata->m_TileDesc.gridX;
		m_nHeightGrid = ptiledata->m_TileDesc.gridY;
		m_fGridSize = ptiledata->m_TileDesc.gridSize;

		char path[256];
		sprintf(path,"maps\\%d\\",m_nMapID);
		GameTerrainScene *pscene = new GameTerrainScene(path, 1, 1);

		int mode = Root::getSingleton().getLightMode();

	//	if (2 == mode)
		{
			// 加载静态光
			//sprintf(path, "maps\\%d\\%d.tslight", m_nMapID, m_nMapID);
			//LoadTerrainStaticLight(pscene, path);

			//sprintf(path, "maps\\%d\\%d.mslight", m_nMapID, m_nMapID);
			//LoadModelStaticLight(pscene, path);

			char fileName[256];
			sprintf(fileName, "maps\\%d\\%d.tlmap", m_nMapID, m_nMapID);

			LoadLightMap(pscene, fileName, path);
		}

		pscene->addTerrainTile(0, 0, ptiledata);
		pscene->loadPhysicsScene2();
		ptiledata->release();
		m_pScene = pscene;

		assert(m_pAmbientMgr == NULL);
		m_pAmbientMgr = new AmbientManager(pscene);

		sprintf(path, "maps\\%d\\%d.area", m_nMapID, m_nMapID );
		m_pRegionSet = new GameMapRegionSet;
		m_pRegionSet->loadFromFile(path);
		m_pCurRegion = NULL;

		sprintf(path, "maps\\%d\\%d.light", m_nMapID, m_nMapID);
		LoadLightsIntoScene(pscene, path);

		m_pFootprints = new Footprints("effect\\texture\\Footsteps.dds");
		m_pFootprints->attachToScene(m_pScene);

		return true;
	}

	void ShowGameMap::InitMiniMap(const char* szUiTex, const char* szMiniMap)
	{	 
		OGRE_RELEASE( m_pMapMaskTex );
		OGRE_RELEASE( m_pMinMapTex );

		m_pMapMaskTex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(szUiTex));
		m_pMinMapTex = static_cast<TextureData *>(ResourceManager::getSingleton().blockLoad(szMiniMap));
	}

	void ShowGameMap::UpdateMiniMap(int nX, int nY , int nMipLevel )
	{	
		if( m_pMapMaskTex == NULL )
			return;

		static int sx = 0 ,sy = 0 ;
		static int  sMinpLevel = 0;

		if( sx != nX || sy != nY || sMinpLevel != nMipLevel )
		{
			sx = nX ;
			sy = nY ;
			sMinpLevel = nMipLevel ;
		}
		else
		{
			return;
		}

		LockResult lockresult , lockresult_1;
		void *pbits, *pbits1;
		pbits1 = m_pMinMapTex->lock(0, nMipLevel, true, lockresult_1);
		pbits = m_pMapMaskTex->lock(0, 0, false, lockresult);

		TextureDesc desc , desc_1;
		m_pMapMaskTex->getDesc(desc);
		m_pMinMapTex->getDesc(desc_1);

		int nCurWidth, nCurHeight;
		switch( nMipLevel )
		{
		case 0 :
			{
				nCurWidth = desc_1.width ;
				nCurHeight = desc_1.height ;
			}
			break;
		case 1:
			{
				nCurWidth = desc_1.width / 2 ;
				nCurHeight = desc_1.height / 2;
				nX = nX / 2;
				nY = nY / 2;
			}
			break;
		case 2:
			{
				nCurWidth = desc_1.width  / 4 ;
				nCurHeight = desc_1.height / 4 ;
				nX = nX / 4;
				nY = nY / 4;
			}
			break;
		}

		nX = nX - ( desc.width  / 2 ) ;
		nY = nY - ( desc.height  / 2 ) ;

		assert(desc.format==PF_A8R8G8B8 && desc_1.format==PF_A8R8G8B8);
		for(unsigned int y=0; y< desc.height ; y++)
		{
			const unsigned char *psrcbits = (const unsigned char *)pbits1 + lockresult_1.rowpitch*( nY + y );			
			unsigned char *pdstbits = (unsigned char *)pbits + lockresult.rowpitch*y ;

			for(unsigned int x=0; x< desc.width  ; x++)
			{	
				int pos =  x*4 + 4*nX  ;
				pdstbits[4*x] = psrcbits[ pos ];
				pdstbits[4*x +1 ] = psrcbits[ pos+ 1 ];
				pdstbits[4*x +2 ] = psrcbits[ pos+ 2 ];
			}
		}

		m_pMapMaskTex->unlock(0, 0);
		m_pMinMapTex->unlock(0, nMipLevel);
	}

	void ShowGameMap::CreateEmptyMap()
	{
		m_pScene = new SimpleGameScene;
		m_MapType = MAP_EMPTY;
	}

	enum
	{
		MAPDB_ID = 0,
		MAPDB_NAME,
		MAPDB_TYPE,
		MAPDB_NUM_TILE_X,
		MAPDB_NUM_TILE_Z,
		MAPDB_MIN_X,
		MAPDB_MIN_Y,
		MAPDB_MIN_Z,
		MAPDB_MAX_X,
		MAPDB_MAX_Y,
		MAPDB_MAX_Z,
		MAPDB_SKY,
	};

	WPOS_T ShowGameMap::getGroundPoint(WPOS_T x, WPOS_T z, Vector3 *pnormal, float *pwaterheight)
	{
		WPOS_T y;
		if(m_pScene && m_pScene->pickGround(x, z, &y, pnormal, pwaterheight))
		{
			return y;
		}
		else
		{
			if(pnormal) *pnormal = Vector3(0, 1.0f, 0);
			if(pwaterheight) *pwaterheight = -10000.0f;
			return 0;
		}
	}

	bool ShowGameMap::pickGround(const WorldRay &worldray, float* pfDis)
	{
		return m_pScene->pickGround(worldray, pfDis);
	}

	Entity *ShowGameMap::pickActor(const WorldRay& worldray, float* pfDis)
	{
		MovableObject *pobj = m_pScene->pickObject(INTERSECT_PICK, worldray, pfDis, 1);
		if(pobj)
		{
			if(pobj->getPickOwner()) pobj = pobj->getPickOwner();

			if(IS_KIND_OF(Entity, pobj)) return static_cast<Entity *>(pobj);
			else return NULL;
		}

		return NULL;
	}

	void ShowGameMap::getMapOrigin(WPOS_T &x, WPOS_T &z)
	{
		x = 0;
		z = 0;
	}

	void ShowGameMap::addFootprint(const WorldPos &pos, const Vector3 &dir)
	{
		WorldPos newpos(pos);
		Vector3 normal;
		newpos.y = getGroundPoint(newpos.x, newpos.z, &normal, NULL);

		m_pFootprints->addFootprint(newpos.toVector3(), normal, dir, 15.0f, 15.0f, 10.0f);
	}

	void ShowGameMap::update(unsigned int dtick, Camera* pCamera, const WorldPos &player_pos)
	{
		// borderscene
		if (BorderGameScene::getSingletonPtr())
			BorderGameScene::getSingleton().update(dtick);

		m_CenterPos = player_pos;

		GameMapRegion *pregion = m_pRegionSet->findRegion(player_pos.x, player_pos.z);
		if(pregion != m_pCurRegion)
		{
			m_pAmbientMgr->setCurAmbient(pregion->m_EnvInfo, m_pCurRegion==NULL, !m_OverlayMusicPath.empty());
			m_pCurRegion = pregion;
		}
		m_pAmbientMgr->updatecamera(pCamera, player_pos);
		m_pAmbientMgr->update(dtick);

		m_pFootprints->update(dtick);

		m_pScene->update(dtick);

		((GameTerrainScene*)m_pScene)->updateGrassDisturb(player_pos,dtick);

		if(IS_KIND_OF(GameTerrainScene, m_pScene))
		{
			static_cast<GameTerrainScene *>(m_pScene)->updateFocusArea(player_pos, 1000.0f);
		}
	}

	void ShowGameMap::setOverlayRegion(const char *pname, float fSpeed)
	{
		m_pRegionSet->setOverlayRegion(pname);
		m_pAmbientMgr->setAlphaAddSpeed( fSpeed );
	}

	void ShowGameMap::setOverlayMusic(const char *path, float volume)
	{
		if (path == NULL)
		{
			if (!m_OverlayMusicPath.empty())
			{
				if (m_pCurRegion != NULL)
				{
					m_pAmbientMgr->setCurAmbient(m_pCurRegion->m_EnvInfo, false);
				}
			}
			m_OverlayMusicPath.clear();
		}
		else
		{
			m_OverlayMusicPath = path;
			m_OverlayMusicVolume = volume;
			SoundSystem::getSingleton().playMusic(0, m_OverlayMusicPath.c_str(), true, 0, m_OverlayMusicVolume);
		}
	}

	void ShowGameMap::PreloadFiles()
	{
		if (m_pScene)
			m_pScene->PreloadSound();

		if (m_pRegionSet)
			m_pRegionSet->PreloadMusic();
	}

}