
#pragma once

#include "igame.h"

#include <string>

namespace TE
{

class CSoundManager
{
public:
	CSoundManager(void);
	~CSoundManager(void);
    
	//���ñ�������
	void  SetSceneMusic(const char* filename );

private:
	//��������
	std::string  m_SceneMusicFileName; 
};

};

extern TE::CSoundManager g_SoundManager;