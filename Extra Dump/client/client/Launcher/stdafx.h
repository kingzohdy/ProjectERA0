// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#pragma warning(disable:4819)

#include "proto_comm.h"
#include "proto_cs.h"
#include "resource.h"
#include "cs_net.h"

#include "Launcher_Image.h"
#include "Launcher_Control.h"
#include "Launcher_HyperLink.h"
#include "Launcher_Progress.h"
//#include "WebView.h"

#include <string>
#include <Winldap.h>
#include <process.h>



#include <stdio.h>
#include <fstream>
#include <io.h>
#include <tchar.h>
#include <Windows.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <Windows.h>
#include <string>
#include <map>
#include <vector>

const size_t MAX_STR_LEN	= 16;
const int    MAX_INT		= 2147483647;

typedef unsigned char		uchar;
typedef unsigned int		uint;
typedef unsigned char		uint8;
typedef unsigned long		uint32;
typedef	__int8				int8;
typedef	__int16				int16;
typedef	__int32				int32;
typedef	__int64				int64;

typedef	unsigned __int16	uint16;
typedef	unsigned __int64	uint64;

typedef std::string String;
using namespace std;

#pragma comment(lib, "wldap32.lib")

#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib" )
#else
#pragma comment(lib, "libcurl.lib" )
#endif

#pragma comment(lib, "Psapi.lib")

void PreMultiplyRGBChannels(BYTE* pBits, int pitch, int width, int height);
//libtbus.lib
#endif // _AFX_NO_AFXCMN_SUPPORT
