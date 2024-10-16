
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
    
	//…Ë÷√±≥æ∞“Ù¿÷
	void  SetSceneMusic(const char* filename );

private:
	//±≥æ∞“Ù¿÷
	std::string  m_SceneMusicFileName; 
};

};

extern TE::CSoundManager g_SoundManager;