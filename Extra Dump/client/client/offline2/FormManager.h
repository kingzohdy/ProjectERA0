#ifndef __FormManager_H__
#define __FormManager_H__
#include <Windows.h>
#include "winexception.h"
#include "OgreOSUtility.h"
#include "resource.h"
#include "OgreSingleton.h"
class FormManager
{
public:
	FormManager();
	~FormManager();
private:
	HBITMAP m_hMaskBmplogin;
	BITMAP SplashBmp;
};
#endif