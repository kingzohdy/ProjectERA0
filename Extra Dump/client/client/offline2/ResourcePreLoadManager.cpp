#include <Windows.h>
#include "ResourcePreLoadManager.h"
#include "OgreLog.h"
#include "OgreDataStream.h"
#include <fstream>
using namespace Ogre;
using namespace std;
#define PRELOADCONFIG_PATH		"db\\preLoadFiles"

ResoucePreLoadManager::~ResoucePreLoadManager()
{
	unloadPublicMontions();
	CloseHandle(m_ThreadHandle);
}

DWORD WINAPI threadWorkFunction( void* data )
{
	ResoucePreLoadManager* pMgr = (ResoucePreLoadManager*) data;
	pMgr->startLoadResource();
	return 0;
}

void ResoucePreLoadManager::loadPublicMotions()
{
	DWORD threadid;
	m_ThreadHandle = (void*)CreateThread(NULL, 0, threadWorkFunction, this, 0, &threadid);
}

void ResoucePreLoadManager::startLoadResource()
{
	LockFunctor lockfunc(&m_Mutex);
	//载入public 文件夹下的所有文件，不释放引用，
	Ogre::Resource* pres = NULL;
	if( ResourceManager::getSingleton().isHavePackages() )
	{
		DataStream* fp = ResourceManager::getSingleton().openFileStream( PRELOADCONFIG_PATH,true );
		if( fp == NULL )
		{
			return;
		}
		char szPath[MAX_PATH] = {0};
		while( !fp->eof() )
		{
			if ( fp->readLine( szPath, MAX_PATH ) == 0 )
			{
				continue;
			}
			pres = ResourceManager::getSingleton().blockLoad( szPath );
			m_vResHandle.push_back(pres);
		}
		delete( fp );

	}else{
		//把4个职业的模型预先加载
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\female.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\female_fs.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\female_ms.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\female_qs.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\female_zs.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\male.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\male_fs.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\male_ms.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\male_qs.oanim");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\player\\male_zs.oanim");
		m_vResHandle.push_back(pres);

		//把创建角色的那些模型加载
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\204011\\204011.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\204012\\204012.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\204013\\204013.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\204014\\204014.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\205005\\205005.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\205006\\205006.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\205007\\205007.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\205008\\205008.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\205008\\5500004.omod");
		m_vResHandle.push_back(pres);
		pres = ResourceManager::getSingleton().blockLoad("character\\monster\\205008\\5500003.omod");
		m_vResHandle.push_back(pres);

		HANDLE hFind;
		WIN32_FIND_DATA ffd;
		string strFold = "effect\\public\\";
		string strSeach = strFold + "*.*";		

		ZeroMemory( &ffd, sizeof(WIN32_FIND_DATA) );
		hFind = FindFirstFile( strSeach.c_str(), &ffd );
		if( hFind == INVALID_HANDLE_VALUE )
			return;
		while( FindNextFile( hFind, &ffd ) )
		{
			const char *pdot = strrchr(ffd.cFileName, '.');
			if( pdot && (stricmp(pdot,".emo")==0||stricmp(pdot,".ent")==0) )
			{
				string fullPath = strFold + ffd.cFileName;
				pres = ResourceManager::getSingleton().blockLoad(fullPath.c_str());
				m_vResHandle.push_back(pres);
			}
		}
		FindClose( hFind );

		std::fstream file( PRELOADCONFIG_PATH,ios::out );
		for( size_t i = 0 ; i < m_vResHandle.size() ; i ++ )
		{
			if( m_vResHandle[i] != NULL )
			{
				const char* path = (const char*)(m_vResHandle[i]->getResPath());
				file<<path<<endl;
			}
		}
		file.flush();
		file.close();
	}
}

void ResoucePreLoadManager::unloadPublicMontions()
{
	LockFunctor lockfunc(&m_Mutex);
	for( size_t i = 0 ; i < m_vResHandle.size(); i++ )
	{
		if( m_vResHandle[i] != NULL )
		{
			OGRE_RELEASE( m_vResHandle[i] );
		}
	}
}