
#include ".\minput.h"

#include  <windows.h>

using namespace TE;

MInput::MInput(void)
{
}

MInput::~MInput(void)
{
}

//���԰����Ƿ��� ;����( ���� ) ; ����( Ϊ0��ʾû�а��� )
int   MInput::KeyDown( char  key )
{
	return   (GetAsyncKeyState( key ) & 0x8000);
}
//��CTRL�Ƿ���; ����( Ϊ0��ʾû�а��� )
int   MInput::LControlKeyDown()
{
	return  (GetAsyncKeyState( VK_LCONTROL ) & 0x8000);
};
//��CTRL�Ƿ���; ����( Ϊ0��ʾû�а��� )
int   MInput::RControlKeyDown()
{
	return  (GetAsyncKeyState( VK_RCONTROL ) & 0x8000);
};
//��ALT�Ƿ���, ����( Ϊ0��ʾû�а��� )
int   MInput::LMenuKeyDown()
{
	return (GetAsyncKeyState( VK_LMENU ) & 0x8000);
};
//��ALT�Ƿ���, ����( Ϊ0��ʾû�а��� )	
int   MInput::RMenuKeyDown()
{
	return (GetAsyncKeyState( VK_RMENU ) & 0x8000);
};
//��SHIFT�Ƿ���;  ����( Ϊ0��ʾû�а��� )
int   MInput::LShiftKeyDown()
{
	return (GetAsyncKeyState( VK_LSHIFT ) & 0x8000);
};
//��SHIFT�Ƿ���;  ����( Ϊ0��ʾû�а��� )
int   MInput::RShiftKeyDown()
{
	return (GetAsyncKeyState( VK_RSHIFT ) & 0x8000);
};