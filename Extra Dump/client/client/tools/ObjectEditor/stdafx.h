// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0410 // ��������Ϊ�ʺ� Windows Me ����߰汾����Ӧֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 6.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0600	// ����ֵ����Ϊ��Ӧ��ֵ���������� IE �������汾��ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <BCGCBProInc.h>			// BCGControlBar Pro
#include "ObjectEditorTypes.h"
#include "Resource.h"
#include "proto_comm.h"

#define EEM_DELETE_EFFECT	(WM_USER + 1000)
#define EEM_APPLY			(WM_USER + 1001)
#define EEM_SET_BGCOLOR		(WM_USER + 1002)
#define EEM_SAVE			(WM_USER + 1003)
//#define EEM_LOAD			(WM_USER + 1004)
#define EEM_SET_SPEED		(WM_USER + 1005)
#define EEM_EFFECT_LIST_SELECTED	(WM_USER + 1006)
#define EEM_UPDATA_LIST		(WM_USER + 1019)
#define EEM_GOTO_PAGE		(WM_USER + 1008)
#define EEM_DROPRES			(WM_USER + 1009)
#define EEM_LOAD_ACTOR_EFFECT	(WM_USER + 1010)
#define EEM_ACTOR_LOADED	(WM_USER + 1011)
#define	EEM_NEW_EFFECT		(WM_USER + 1012)
#define EEM_CLIENT_CREATED	(WM_USER + 1013)
#define EEM_DROPITEM		(WM_USER + 1014)
#define EEM_EDITITEM		(WM_USER + 1015) //lparam: item id
#define EEM_FINISH_EDITITEM	(WM_USER + 1016) //lparam: item id
#define EEM_UPDATE_ANCHOR_ITEM (WM_USER + 1017)
#define EEM_UPDATE_SKILLBAR (WM_USER + 1018)
#define EEM_AVATAR_UPDATED	 (WM_USER + 19)

#define EEM_REQUEST_KFWND	 (WM_USER + 20)
#define EEM_RELEASE_KFWND	 (WM_USER + 21)

#define EEM_REQ_LOAD_ACTOR	(WM_USER + 1050)
#define EEM_REQ_LOAD_AVATAR (WM_USER + 1051)
#define EEM_REQ_LOAD_EFFECT	(WM_USER + 1052)

#define EEM_SELECT_MOTION	(WM_USER + 23)

#define EEM_GET_CURR_MOTION_DIR	(WM_USER + 24)

#define EEN_PROP_END_EDIT		(WM_USER + 2000)
#define EEN_PROP_CLICK_BUTTON	(WM_USER + 2001)
#define EEN_PROP_SELECT_COMBO (WM_USER + 2002) //lp: nm
#define EEN_PROP_UPDATE_VALUE	(WM_USER + 2003)
#define EEN_LINE_UPDATE_DOT	(WM_USER + 2004)

#define EEN_ADD_EFXFILE_TO_MODELMOTION	(WM_USER + 2005) //��ѡ�е�efx�ļ���ӵ���ǰ��modelmotion����Ϊһ��MOT_COMPOSITE��
#define ADD_SKILL_FAST_KEY	(WM_USER + 2006)	//���ÿ��ܵĿ�ݼ�

#define UNIT_INDEX_ALL		-10
#define UNIT_INDEX_NONE		-11

#define UNIT_VIEW_SHOW		1
#define UNIT_VIEW_SHOWONLY	2
#define UNIT_VIEW_HIDE		3

#define UE_EFFECT_LIST			0x0001
#define UE_MOTION				0x0002
#define UE_MOTION_BRO_COMMON	0x0004
#define UE_MOTION_BRO_PRIVATE	0x0008
#define UE_ALL				(UE_EFFECT_LIST | UE_MOTION | UE_MOTION_BRO_COMMON | UE_MOTION_BRO_PRIVATE)


#define ID_ANMINLIST		3003
#define ID_ANIMPROP			3004
#define ID_BONEPROP			3005
#define ID_BONETREE			3006
#define ID_PARTICLETREE		3007
#define ID_PARTICLEPROP		3008
#define ID_EFFECTLIST		3009
#define ID_LIGHTEPROP		3010
#define ID_AVAPROP			3016
#define ID_RIBBONTREE		3018
#define ID_RIBBONPROP		3019
#define ID_BILLBOARDEPROP	3020
#define ID_BEAMPROP			3021
#define ID_ITEMLIST			3023
#define ID_ITEMEDITPROP		3025
#define ID_AVAMODPROP		3026
#define ID_DIRTREE			3027
#define ID_CHUNKLIST		3028
#define ID_ANCHORPROP		3029
#define ID_ANCHORTREE		3030
#define ID_MOTIONTREE		3033
#define ID_MONTIONTB		3034
#define ID_ITEMPVLIST		3036
#define ID_ITEMPVGEOLIST	3037
#define	ID_ITEMPVPROP		3038
#define	ID_UNKNOWNPROP		3039
#define ID_SKLPROP			3040
#define ID_SKLTARGETPROP	3041
#define ID_SOUNDPROP		3043
#define ID_KEYFRAMECTRL		3044
#define ID_DECALTREE		3046
#define ID_DECALPROP		3047
#define ID_ITEMLIST_2		3048
#define ID_SRT_PROP			3049
#define ID_GEOMTREE			3052
#define ID_GEOMPROP			3053
#define ID_BOUNDPROP		3054
#define ID_MOTIONBROTREE	3060
#define ID_MOTIONEDIT		3061
#define ID_BILLBOARDTREE	3062
#define ID_VIRTUALTREE		3063
#define ID_VIRTUALPROP		3064
#define ID_SKILLTREE		3065
#define ID_BEAMTREE			3066

#define ID_WND_EDIT			3011
#define ID_WND_MODE			3012
#define ID_WND_ANIM			3013
#define ID_WND_BONE			3014	
#define ID_WND_BROWSER1		3015
#define ID_WND_AVATAR		3017
#define ID_WND_ITEMVIEW		3022
#define ID_WND_ITEMEDIT		3024
#define ID_WND_ANCHOR		3031
#define ID_WND_MOTION		3032
#define ID_WND_ITEMPREVIEW	3035
#define ID_WND_SKILLPREVEW	3042
#define ID_WND_SKILLTEMPLATE 3096
#define ID_WND_KEYFRAMEWND	3045
#define ID_WND_SRTBAR		3051
#define ID_WND_BOUND		3055
#define ID_WND_BROWSER2		3056
#define ID_WND_BROWSER3		3057
#define ID_WND_BROWSER4		3058
#define ID_WND_MOTION_BRO	3059

#define ID_SKILLTEMPLATE_SKILLPROPLIST	4000
#define ID_SKILLTEMPLATE_TARGETPROPLIST	4001
#define ID_SKILL_FAST_KEY	4002
#define ID_SKILL_FAST_KEY_REVERSE 4003 //�����޸Ŀ�ݼ���ֵ


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


