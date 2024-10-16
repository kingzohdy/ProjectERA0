
#ifndef __OgreInputHandler_H__
#define __OgreInputHandler_H__

namespace Ogre
{
	enum
	{
		GIE_CHAR=0,
		GIE_KEYDOWN,
		GIE_KEYUP,
		GIE_LBTNDOWN,
		GIE_LBTNUP,
		GIE_LBTNDBLCLK,
		GIE_RBTNDOWN,
		GIE_RBTNUP,
		GIE_RBTNDBLCLK,
		GIE_MOUSEMOVE,
		GIE_MOUSEWHEEL,
		GIE_MOUSEOUT,
		GIE_MOUSEIN,
		GIE_WINSIZE,
		GIE_GAINFOCUS,
		GIE_LOSTFOCUS,

		MAX_INPUT_EVENT
	};

#ifndef MK_LBUTTON
#define MK_LBUTTON          0x0001
#define MK_RBUTTON          0x0002
#define MK_SHIFT            0x0004
#define MK_CONTROL          0x0008
#define MK_MBUTTON          0x0010
#define MK_ALT              0x0020
#endif

#ifndef VK_LBUTTON

#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#define VK_BACK           0x08
#define VK_TAB            0x09

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

#define VK_SLEEP          0x5F

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#endif

	struct GeneralEventData
	{
		unsigned int wparam;
		unsigned int lparam;
	};

	struct MouseEventData
	{
		short x;
		short y;
		unsigned int flags;
	};

	struct KeyEventData
	{
		int vkey;
		unsigned int flags;
	};

	struct MouseWheelEventData
	{
		float dist;
		unsigned int flags;
		short x;
		short y;
	};

	struct CharEventData
	{
		char str[8];
	};

	struct WinSizeEventData
	{
		int sizex;
		int sizey;
	};

	struct InputEvent
	{
		int msg;
		union
		{
			GeneralEventData data;
			KeyEventData key;
			MouseEventData mouse;
			MouseWheelEventData wheel;
			CharEventData chars;
			WinSizeEventData winsize;
		};
	};

	class InputHandler
	{
	public:
		virtual int HandleInput(const InputEvent &event) = 0;
	};
}


#endif
