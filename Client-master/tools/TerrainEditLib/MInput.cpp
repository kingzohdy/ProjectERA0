
#include ".\minput.h"

#include  <windows.h>

using namespace TE;

MInput::MInput(void)
{
}

MInput::~MInput(void)
{
}

//测试按键是否按下 ;参数( 按键 ) ; 返回( 为0表示没有按下 )
int   MInput::KeyDown( char  key )
{
	return   (GetAsyncKeyState( key ) & 0x8000);
}
//左CTRL是否按下; 返回( 为0表示没有按下 )
int   MInput::LControlKeyDown()
{
	return  (GetAsyncKeyState( VK_LCONTROL ) & 0x8000);
};
//右CTRL是否按下; 返回( 为0表示没有按下 )
int   MInput::RControlKeyDown()
{
	return  (GetAsyncKeyState( VK_RCONTROL ) & 0x8000);
};
//左ALT是否按下, 返回( 为0表示没有按下 )
int   MInput::LMenuKeyDown()
{
	return (GetAsyncKeyState( VK_LMENU ) & 0x8000);
};
//右ALT是否按下, 返回( 为0表示没有按下 )	
int   MInput::RMenuKeyDown()
{
	return (GetAsyncKeyState( VK_RMENU ) & 0x8000);
};
//左SHIFT是否按下;  返回( 为0表示没有按下 )
int   MInput::LShiftKeyDown()
{
	return (GetAsyncKeyState( VK_LSHIFT ) & 0x8000);
};
//右SHIFT是否按下;  返回( 为0表示没有按下 )
int   MInput::RShiftKeyDown()
{
	return (GetAsyncKeyState( VK_RSHIFT ) & 0x8000);
};