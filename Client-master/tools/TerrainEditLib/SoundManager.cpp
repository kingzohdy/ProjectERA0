
#include ".\soundmanager.h"

using namespace TE;

CSoundManager g_SoundManager;

CSoundManager::CSoundManager(void)
{
}

CSoundManager::~CSoundManager(void)
{
}
//设置播放背景音乐;参数( 文件名 )
void  CSoundManager::SetSceneMusic(const char* filename )
{
       m_SceneMusicFileName = filename ;

	   //g_pEngine->GetAudioSys()->PlayMusic( 0 , filename , true , 8000 );

};
