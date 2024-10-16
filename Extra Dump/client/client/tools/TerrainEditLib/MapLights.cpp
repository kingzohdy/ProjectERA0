
#include "MapLights.h"
#include "EditorModel.h"
#include "TerrainEdit.h"
#include "Terrain.h"
#include "Coordinate.h"
#include "EnvRegion.h"
#include "EditorMessageQue.h"

#include "OgreLight.h"
#include "OgreSoundData.h"
#include "OgreSoundNode.h"
#include "OgreGameScene.h"
#include "TerBeastManager.h"

using namespace Ogre;

namespace TE
{
	extern std::string g_path;

	const char *LIGHT_MODEL_PATH = "toolres\\mapeditor\\light.omod";
	const char *SOUND_MODEL_PATH = "toolres\\mapeditor\\sound.omod";
	MapLightsManager g_LightManager;

	MapLightsManager::MapLightsManager(void)
	{
		m_CurEditMode = 0;
		m_bLockHeight   = false;
	}

	MapLightsManager::~MapLightsManager(void)
	{
		Release();
	}			

	void MapLightsManager::update(float dtime)
	{
		unsigned int dtick = TimeToTick(dtime);
		for(size_t i=0; i<m_LightSet.size(); i++)
		{
			LightNodeInfo &nodeinfo = m_LightSet[i];
			nodeinfo.peffectnode->update(dtick);
			nodeinfo.pModel->Update(dtime);
		}
	}

	void   MapLightsManager::Release()
	{
		DetachLightToRoom();

		for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
		{
			OGRE_RELEASE(m_LightSet[i].peffectnode);
			delete( m_LightSet[i].pModel );
		}

		m_LightSet.clear();
		m_LightInfoSet.clear();
		m_SaveLightInfo.clear();
	};

	static void SerializeStr(std::string &str, FILE *fp, bool isread)
	{
		if(isread)
		{
			int len;
			char buffer[256];
			fread(&len, sizeof(len), 1, fp);
			assert(len < 255);
			fread(buffer, len, 1, fp);
			buffer[len] = 0;
			str = buffer;
		}
		else
		{
			int len = (int)str.length();
			fwrite(&len, sizeof(len), 1, fp);
			fwrite(str.c_str(), len, 1, fp);
		}
	}

	bool   MapLightsManager::LoadLightInfo( const char* filename ,TerrainEditMgr* pEditor )
	{
		Release();

		FILE *fp = fopen(filename, "rb");
		if(fp == NULL) return false;

		LightInfoHead head;
		fread(&head, sizeof(head), 1, fp);
		for(int i=0; i<head.nLightNum; i++)
		{
			LightInfo lightinfo;
			if(head.nVersion <= 100)
			{
				LightInfo_100 oldinfo;
				fread(&oldinfo, sizeof(LightInfo_100), 1, fp);
				lightinfo.nType = 0;
				lightinfo.pos = oldinfo.pos;
				lightinfo.fRange = oldinfo.fRange;
				lightinfo.color = oldinfo.color;

				lightinfo.time0 = 0.0f;
				lightinfo.time1 = 0.0f;

				lightinfo.fSoundFullRange = 0.0f;
				lightinfo.iIsLoop = 1;
			}
			else if(head.nVersion == 101)
			{
				fread(&lightinfo.nType, sizeof(lightinfo.nType), 1, fp);
				fread(&lightinfo.pos, sizeof(lightinfo.pos), 1, fp);
				fread(&lightinfo.fRange, sizeof(lightinfo.fRange), 1, fp);
				fread(&lightinfo.color, sizeof(lightinfo.color), 1, fp);
				fread(&lightinfo.volume, sizeof(lightinfo.volume), 1, fp);

				SerializeStr(lightinfo.name, fp, true);
				SerializeStr(lightinfo.respath, fp, true);

				lightinfo.time0 = 0.0f;
				lightinfo.time1 = 0.0f;

				lightinfo.fSoundFullRange = 0.0f;
				lightinfo.iIsLoop = 1;
			}
			else if (head.nVersion == 102)
			{
				fread(&lightinfo.nType, sizeof(lightinfo.nType), 1, fp);
				fread(&lightinfo.pos, sizeof(lightinfo.pos), 1, fp);
				fread(&lightinfo.fRange, sizeof(lightinfo.fRange), 1, fp);
				fread(&lightinfo.color, sizeof(lightinfo.color), 1, fp);
				fread(&lightinfo.volume, sizeof(lightinfo.volume), 1, fp);
				SerializeStr(lightinfo.name, fp, true);
				SerializeStr(lightinfo.respath, fp, true);
				fread(&lightinfo.time0, sizeof(lightinfo.time0), 1, fp);
				fread(&lightinfo.time1, sizeof(lightinfo.time1), 1, fp);

				lightinfo.fSoundFullRange = 0.0f;
				lightinfo.iIsLoop = 1;
			}
			else if (head.nVersion == 103)
			{
				fread(&lightinfo.nType, sizeof(lightinfo.nType), 1, fp);
				fread(&lightinfo.pos, sizeof(lightinfo.pos), 1, fp);
				fread(&lightinfo.fRange, sizeof(lightinfo.fRange), 1, fp);
				fread(&lightinfo.color, sizeof(lightinfo.color), 1, fp);
				fread(&lightinfo.volume, sizeof(lightinfo.volume), 1, fp);
				SerializeStr(lightinfo.name, fp, true);
				SerializeStr(lightinfo.respath, fp, true);
				fread(&lightinfo.time0, sizeof(lightinfo.time0), 1, fp);
				fread(&lightinfo.time1, sizeof(lightinfo.time1), 1, fp);
				fread(&lightinfo.fSoundFullRange, sizeof(lightinfo.fSoundFullRange), 1, fp);
				lightinfo.iIsLoop = 1;
			}
			else if (head.nVersion == 104)
			{
				fread(&lightinfo.nType, sizeof(lightinfo.nType), 1, fp);
				fread(&lightinfo.pos, sizeof(lightinfo.pos), 1, fp);
				fread(&lightinfo.fRange, sizeof(lightinfo.fRange), 1, fp);
				fread(&lightinfo.color, sizeof(lightinfo.color), 1, fp);
				fread(&lightinfo.volume, sizeof(lightinfo.volume), 1, fp);
				SerializeStr(lightinfo.name, fp, true);
				SerializeStr(lightinfo.respath, fp, true);
				fread(&lightinfo.time0, sizeof(lightinfo.time0), 1, fp);
				fread(&lightinfo.time1, sizeof(lightinfo.time1), 1, fp);
				fread(&lightinfo.fSoundFullRange, sizeof(lightinfo.fSoundFullRange), 1, fp);
				fread(&lightinfo.iIsLoop, sizeof(lightinfo.iIsLoop), 1, fp);
			}

			AddLight(lightinfo);
		}

		fclose(fp);
		return true;
	}

	bool   MapLightsManager::SaveLightInfo( const char* filename )
	{
		FILE *fp = fopen(filename, "wb");
		if(fp == NULL) return false;

		LightInfoHead head;
		head.nVersion = 104;
		head.nLightNum = m_LightInfoSet.size();

		fwrite(&head, sizeof(head), 1, fp);
		for(int i=0; i<head.nLightNum; i++)
		{
			LightInfo &lightinfo = m_LightInfoSet[i];

			fwrite(&lightinfo.nType, sizeof(lightinfo.nType), 1, fp);
			fwrite(&lightinfo.pos, sizeof(lightinfo.pos), 1, fp);
			fwrite(&lightinfo.fRange, sizeof(lightinfo.fRange), 1, fp);
			fwrite(&lightinfo.color, sizeof(lightinfo.color), 1, fp);
			fwrite(&lightinfo.volume, sizeof(lightinfo.volume), 1, fp);
			SerializeStr(lightinfo.name, fp, false);
			SerializeStr(lightinfo.respath, fp, false);
			// 102
			fwrite(&lightinfo.time0, sizeof(lightinfo.time0), 1, fp);
			fwrite(&lightinfo.time1, sizeof(lightinfo.time1), 1, fp);
			//103
			fwrite(&lightinfo.fSoundFullRange, sizeof(lightinfo.fSoundFullRange), 1, fp);
			//104
			fwrite(&lightinfo.iIsLoop, sizeof(lightinfo.iIsLoop), 1, fp);
		}

		fclose(fp);
		return true;
	};

	LightInfo &MapLightsManager::getLightInfo(int i)
	{
		assert(i>=0 && i<m_LightInfoSet.size());
		return m_LightInfoSet[i];
	}

	LightNodeInfo &MapLightsManager::getLightNodeInfo(int i)
	{
		assert(i>=0 && i<m_LightSet.size());
		return m_LightSet[i];
	}

	void MapLightsManager::updateNodeInfo(int i)
	{
		if(i < 0) return;
		assert(i <m_LightSet.size());

		LightNodeInfo &nodeinfo = m_LightSet[i];
		LightInfo &lightinfo = m_LightInfoSet[i];

		if(lightinfo.nType == 0)
		{
			Light *plight = static_cast<Light *>(nodeinfo.peffectnode);
			plight->setColor(lightinfo.color);
			plight->m_Range = lightinfo.fRange;
			CountChunkLight(lightinfo.pos, lightinfo.color, lightinfo.fRange);
		}
		else
		{
			SoundNode *psound = static_cast<SoundNode *>(nodeinfo.peffectnode);
			psound->setDistance(0, lightinfo.fRange);
			psound->setVolume(lightinfo.volume);
			psound->setRandomTime0(lightinfo.time0);
			psound->setRandomTime1(lightinfo.time1);
			psound->setSoundFile(lightinfo.respath.c_str());
			psound->setSoundFullRange(lightinfo.fSoundFullRange);
			psound->setIsLoop(lightinfo.iIsLoop==1);
			//psound->play();
		}

		nodeinfo.peffectnode->setPosition(lightinfo.pos);
		nodeinfo.pModel->GetModelData()->pos = lightinfo.pos;
		nodeinfo.pModel->ResetMat();
	}

	void MapLightsManager::flushAllLights(bool bake_dirlight)
	{
		int nblockx = g_Terrain.GetTitle(0)->GetHeadData()->gridX;
		int nblockz = g_Terrain.GetTitle(0)->GetHeadData()->gridY;

		Ogre::AmbientInfo &ambient = *g_AreaSet.GetArea(0)->GetAmbientInfo();
		Vector3 dir;
		dir.x = Cos(ambient.dirlight_angle.y)*Cos(ambient.dirlight_angle.x);
		dir.y = Sin(ambient.dirlight_angle.y);
		dir.z = Cos(ambient.dirlight_angle.y)*Sin(ambient.dirlight_angle.x);

		//更新计算中间3X3格子的光照
		for(int i=0; i<nblockx; i++)
		{
			for(int j=0; j<nblockz; j++)
			{
				EditorTerrainBlock &chunk = g_TerrainEdit.GetEditData()->chunkSet[j][i];
				//清除光照颜色
				chunk.SetLightColor( 0 );

				for( size_t k = 0 ; k < m_LightInfoSet.size() ; k ++ )
				{
					LightInfo &info = m_LightInfoSet[k];
					if(info.nType == 0) chunk.CountLightColor(0, info.pos, *(ColourValue *)&info.color, ColourValue(0,0,0,0), info.fRange);
				}

				if(bake_dirlight)
				{
					float shadowdensity = ambient.shadowdensity;
					if(!ambient.shaow) shadowdensity = 0;
					chunk.CountLightColor(1, dir, ambient.dirlight_color, ambient.ambient_color, 0, shadowdensity);
				}

				//更新植物光照颜色。
				chunk.UpdatePlantLightColor();
			}
		}
	}

	void MapLightsManager::AddLightsToBeast ()
	{
		Ogre::AmbientInfo &ambient = *g_AreaSet.GetArea(0)->GetAmbientInfo();
		Vector3 dir;
		dir.x = Cos(ambient.dirlight_angle.y)*Cos(ambient.dirlight_angle.x);
		dir.y = Sin(ambient.dirlight_angle.y);
		dir.z = Cos(ambient.dirlight_angle.y)*Sin(ambient.dirlight_angle.x);
		dir.x = -dir.x;
		dir.y = -dir.y;
		dir.z = -dir.z;

		if (ambient.toplight_shadowbits != 2)
		{
			BeastManager::getSingleton().AddDirectLight(
				BeastManager::getSingleton().GetRandLightName("Light"), dir, ambient.dirlight_color);
		}

		BeastManager::getSingleton().SetAmbientColor(BeastManager::getSingleton().GetRandName("Light"), 
			ambient.ambient_color);


		for (int i=0; i<(int)m_LightInfoSet.size(); i++)
		{
			LightInfo &info = m_LightInfoSet[i];
			if (info.nType == 0)
			{
				BeastManager::getSingleton().AddPointLight(
					BeastManager::getSingleton().GetRandLightName("Light"), info.pos, info.color, info.fRange);
			}
		}

	}

	//计算CHUNK的光照，记录在顶点颜色里面 ；参数（ 位置，颜色，范围 ）
	void   MapLightsManager::CountChunkLight( Ogre::Vector3 pos , Ogre::ColourValue color , float fRange )
	{
		int gx,gz ;

		if( !g_Terrain.GetGridX( pos.x , &gx ) )
			return;

		if( !g_Terrain.GetGridZ( pos .z , &gz ) )
			return;

		int nblockx = g_Terrain.GetTitle(0)->GetHeadData()->gridX;
		int nblockz = g_Terrain.GetTitle(0)->GetHeadData()->gridY;
		//更新计算中间3X3格子的光照
		for( int i = gx - 1 ; i <= gx + 1 ; i ++ )
		{
			for( int j = gz - 1 ; j <= gz + 1 ; j ++ )
			{
				if(i<0||j<0||i>=nblockx||j>=nblockz)
				{
					continue;
				}

				EditorTerrainBlock &chunk = g_TerrainEdit.GetEditData()->chunkSet[j][i];

				//清除光照颜色
				chunk.SetLightColor( 0 );

				for( size_t k = 0 ; k < m_LightInfoSet.size() ; k ++ )
				{
					LightInfo &info = m_LightInfoSet[k];
					if(info.nType == 0) chunk.CountLightColor(0, info.pos, info.color, ColourValue(0,0,0,0), info.fRange);
				}
				//更新植物光照颜色。
				chunk.UpdatePlantLightColor();
			}
		}
	};

	int MapLightsManager::getIndexByModel(TerrModel *pmodel)
	{
		for(size_t i=0; i<m_LightSet.size(); i++)
		{
			if(m_LightSet[i].pModel == pmodel)
			{
				return i;
			}
		}

		return -1;
	}

	void MapLightsManager::deleteLight(int i)
	{
		if(i<0 || i>=m_LightSet.size()) return;

		m_LightSet[i].peffectnode->detachFromScene();
		OGRE_RELEASE(m_LightSet[i].peffectnode);
		delete(m_LightSet[i].pModel);

		Ogre::Vector3  pos = m_LightInfoSet[i].pos;

		m_LightSet.erase(m_LightSet.begin()+i);
		m_LightInfoSet.erase(m_LightInfoSet.begin()+i);

		CountChunkLight(pos , Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f), 0.01f);
	}

	void MapLightsManager::onModelMove(TerrModel *pmodel)
	{
		for(size_t i=0; i<m_LightSet.size(); i++)
		{
			if(m_LightSet[i].pModel == pmodel)
			{
				m_LightInfoSet[i].pos = pmodel->GetModelData()->pos;
				updateNodeInfo(i);
				break;
			}
		}
	}

	TerrModel *MapLightsManager::intersectRay(Ogre::Ray &ray)
	{
		for(size_t i=0; i<m_LightSet.size(); i++)
		{
			TerrModel *pmodel = m_LightSet[i].pModel;
			if(pmodel->GetModelData()->pModel->GetShowModel()->intersectRay(Ogre::INTERSECT_PICK, ray, NULL))
			{
				return pmodel;
			}
		}

		return NULL;
	}

	void MapLightsManager::onCull()
	{
		for(size_t i=0; i<m_LightSet.size(); i++)
		{
			m_LightSet[i].pModel->onCull();
		}
	}

	LightNodeInfo MapLightsManager::AddLight(LightInfo &lightinfo)
	{
		LightNodeInfo lightNodeInfo;

		lightNodeInfo.pModel = new TerrModel;
		lightNodeInfo.pModel->GetModelData()->pos = lightinfo.pos;

		if(lightinfo.nType == 0)
		{
			strcpy(lightNodeInfo.pModel->GetModelData()->name, LIGHT_MODEL_PATH);
			Light *plight = new Light(LT_POINT);
			plight->m_Attenuation = 1.0f;
			plight ->setStatic(true);
			plight->setColor(lightinfo.color);
			plight->m_Range = lightinfo.fRange;

			lightNodeInfo.peffectnode = plight;
		}
		else
		{
			strcpy(lightNodeInfo.pModel->GetModelData()->name, SOUND_MODEL_PATH);
			SoundData *psounddata = new SoundData;
			psounddata->m_strFileName = lightinfo.respath.c_str();
			psounddata->m_PlayMode = PM_LIMITED;
			psounddata->m_Ambient = 1;
			psounddata->m_CreateInfo.isloop = (lightinfo.iIsLoop == 1);
			psounddata->m_CreateInfo.mindistance = 0;
			psounddata->m_CreateInfo.maxdistance = lightinfo.fRange;
			psounddata->m_CreateInfo.volume = lightinfo.volume;
			psounddata->m_CreateInfo.pos = lightinfo.pos;
			psounddata->m_CreateInfo.velocity = Vector3(0,0,0);
			psounddata->m_CreateInfo.time0 = lightinfo.time0;
			psounddata->m_CreateInfo.time1 = lightinfo.time1;
			psounddata->m_CreateInfo.fSoundFullRange = lightinfo.fSoundFullRange;

			SoundNode *psoundnode = new SoundNode(psounddata);
			psounddata->release();
			lightNodeInfo.peffectnode = psoundnode;
		}

		lightNodeInfo.pModel->Init();
		lightNodeInfo.pModel->AttachRoom();

		EditorModel *pmodel = (EditorModel*)lightNodeInfo.pModel->GetModelData()->pModel;	
		assert( pmodel );
		pmodel->SetPos(lightNodeInfo.pModel->GetModelData()->pos);

		lightNodeInfo.peffectnode->setPosition(lightinfo.pos);
		lightNodeInfo.peffectnode->attachToScene(&g_Terrain);
		m_LightSet.push_back( lightNodeInfo );

		m_LightInfoSet.push_back(lightinfo);

		if(lightinfo.nType == 0)
			CountChunkLight(lightinfo.pos , lightinfo.color , lightinfo.fRange);

		return lightNodeInfo;
	};

	void   MapLightsManager::AttachLightModelToRoom()
	{
		for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
		{
			m_LightSet[i].pModel->AttachRoom();
		}
	};

	//移除对应的灯光模型
	void   MapLightsManager::DetachLightModelToRoom()
	{
		for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
		{
			m_LightSet[i].pModel->DetachRoom();
		}
	};

	void   MapLightsManager::AttachLightToRoom()
	{
		for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
		{
			m_LightSet[i].peffectnode->attachToScene(&g_Terrain);
			m_LightSet[i].pModel->AttachRoom();
		}

	}

	void   MapLightsManager::DetachLightToRoom()
	{
		for( size_t i = 0 ; i < m_LightSet.size() ; i ++ )
		{
			m_LightSet[i].peffectnode->detachFromScene();

			m_LightSet[i].pModel->DetachRoom();
		}
	}
}

