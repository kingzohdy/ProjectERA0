/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __ROOT_H__
#define __ROOT_H__

// Precompiler options
#include "OgrePrerequisites.h"

#include "OgreSingleton.h"
#include "OgreString.h"
#include "OgreXMLData.h"
#include "OgreRenderSystem.h"

namespace Ogre
{
	class GameTerrainScene;

	/** The root class of the Ogre system.
	@remarks
	The Ogre::Root class represents a starting point for the client
	application. From here, the application can gain access to the
	fundamentals of the system, namely the rendering systems
	available, management of saved configurations, logging, and
	access to other classes in the system. Acts as a hub from which
	all other objects may be reached. An instance of Root must be
	created before any other Ogre operations are called. Once an
	instance has been created, the same instance is accessible
	throughout the life of that object by using Root::getSingleton
	(as a reference) or Root::getSingletonPtr (as a pointer).
	*/
	class _OgreExport Root : public Singleton<Root>
	{
	public:
		Root(const String &config_file="ogre.cfg");
		~Root();

		// 将以前构造函数里的初始化拆分
		void Initlize ();

		// 设置是否为微端
		bool IsSmartClient ();
		void SetSmartClientServer (std::string ser);
		std::string GetSmartServer ();

		void setInWorld (bool inWorld)
		{
			m_bIsInWorld = inWorld;
		}
		bool isInWorld () { return m_bIsInWorld; }

		bool initRenderSystem(void *hwnd);

		void installPlugin(Plugin* plugin);
		void uninstallPlugin(Plugin* plugin);

		// 设置光照模式
		/*
		* 0 实时， 1 顶点，2 贴图
		*/
		void setLightMode (int mode)
		{
			m_LightMode = mode;
		}

		int getLightMode ()
		{
			return m_LightMode;
		}

		void setFirstRun (bool b);
		bool isFirstRun ();
		bool getActorTransProcess ();
		void setActorTransProcess (bool bTrans);

		bool getBackSceneProcess ();
		void setBackSceneProcess (bool bTrans);

		bool isSaveShaderEnvKeys ();
		void setSaveShaderEnvKey (bool bSave);

		bool isCompileShaderEnvKeys ();
		void setCompileShaderEnvKeys (bool compile);
		
		bool getSyncRefresh();
		void setSyncRefresh( bool b );
		bool getBloom();
		void setBloom(bool b);
		bool getDistort();
		void setDistort(bool b);
		bool getWaterReflect();
		void setWaterReflect(bool b);
		int getShadowmapSize();
		void setShadowmapSize(int w);
		int getMultiSample();
		void setMultiSample(int s);
		void setGlobalSoundVolume(float fVolume);
		void setGlobalMusicVolume(float volume);

		void setLoadShaderCache (bool load);
		bool isLoadShaderCache ();

		void setViewSize(float f);
		float getViewSize();

		// 设置当前GameTerrainScene
		void setCurrentTerrainScene (Ogre::GameTerrainScene *terrainScene)
		{
			m_CurrentTerrainScene = terrainScene;
		}

		Ogre::GameTerrainScene *getCurrentTerrainScene ()
		{
			return m_CurrentTerrainScene;
		}

	private:
		void loadPlugins();
		void unloadPlugins();
		void loadPlugin(const String &name);
		void unloadPlugin(const String &name);
		bool initResourceManager();
		bool initSoundSystem();

	public:
		XMLData m_Config;

	private:
		//Singletons
		void FirstRunDesc();
		MemPoolMgr *m_pMemPoolMgr;
		DynLibManager *m_pDynLibMgr;
		ResourceManager *m_pResourceMgr;
		SoundSystem *m_pSoundSystem;
		RenderSystem *m_pRenderSystem;
		MaterialManager *m_pDummyMtlMgr;
		SceneManager *m_pSceneMgr;
		SequenceMap *m_pSequenceMap;

		std::vector<Plugin *>m_Plugins;
		std::vector<DynLib *>m_PluginLibs;

		bool m_bFirstRun;
		bool m_bActorTransProcess;
		bool m_needLoadShaderCache;
		bool m_bBackSceneProcess;
		bool m_bSaveShaderEnvKeys;
		bool m_bCompileShaderEnvKeys;

		bool m_bIsInWorld;

		int m_LightMode;

		bool m_IsSmartClient;
		float m_fViewSize;

	private:
		Ogre::GameTerrainScene *m_CurrentTerrainScene;
		std::string m_SmartClientServer;
	};
}

#endif