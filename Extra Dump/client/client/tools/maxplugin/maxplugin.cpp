
#include "stdafx.h"
#include "maxplugin.h"
#include "exp_nodetree.h"
#include "exp_mesh.h"
#include "OgreBuildingData.h"

#include <iostream>
#include <fstream>

#include "Maxscrpt.h"
#include "Numbers.h"    
#include "Arrays.h"
#include "definsfn.h"
def_visible_primitive(AddBlankNoteKey, "AddBlankNoteKey");

using namespace Ogre;


#define MAXPLUGIN_CLASS_ID	Class_ID(0x22f459be, 0x6b113bf5)

namespace DKEXP
{
	static MaxPlugin theMaxPlugin;

	class MaxPluginClassDesc:public ClassDesc {
	public:
		int 			IsPublic() {return 1;}
		void *			Create(BOOL loading = FALSE) {return &theMaxPlugin;}
		const TCHAR *	ClassName() {return GetString(IDS_CLASSNAME);}
		SClass_ID		SuperClassID() {return UTILITY_CLASS_ID;}
		Class_ID		ClassID() {return MAXPLUGIN_CLASS_ID;}
		const TCHAR* 	Category() {return _T("");}
	};

	static MaxPluginClassDesc MaxPluginDesc;
	ClassDesc* GetMaxPluginDesc() {return &MaxPluginDesc;}


	//---------------------------------------------------------------------------
	static ::INT_PTR CALLBACK MaxPluginDlgProc(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		switch (msg) {
	case WM_INITDIALOG:
		theMaxPlugin.Init(hWnd);
		break;

	case WM_DESTROY:
		theMaxPlugin.Destroy(hWnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDC_CLOSEBUTTON:
			theMaxPlugin.m_iu->CloseUtility();
			break;

		case IDC_BUTTON_PREVIEW:

			break;

		default:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		theMaxPlugin.m_ip->RollupMouseMessage(hWnd,msg,wParam,lParam); 
		break;

	default:
		return FALSE;
		}
		return TRUE;
	}	

	//---------------------------------------------------------------------------
	MaxPlugin::MaxPlugin()
	{
		m_iu = NULL;
		m_ip = NULL;	
		hPanel = NULL;
	}

	//---------------------------------------------------------------------------
	MaxPlugin::~MaxPlugin()
	{
	}

	//---------------------------------------------------------------------------
	void MaxPlugin::BeginEditParams(Interface *ip,IUtil *iu) 
	{
		this->m_iu = iu;
		this->m_ip = ip;
		g_Interface = ip;
		hPanel = ip->AddRollupPage(
			g_hInstance,
			MAKEINTRESOURCE(IDD_PANEL),
			MaxPluginDlgProc,
			GetString(IDS_PARAMETERS),
			0);
	}

	//---------------------------------------------------------------------------	
	void MaxPlugin::EndEditParams(Interface *ip,IUtil *iu) 
	{
		this->m_iu = NULL;
		this->m_ip = NULL;
		ip->DeleteRollupPage(hPanel);
		hPanel = NULL;
	}

	//---------------------------------------------------------------------------
	void MaxPlugin::Init(HWND hWnd)
	{
	}

	//---------------------------------------------------------------------------
	void MaxPlugin::Destroy(HWND hWnd)
	{
	}
}

Value* AddBlankNoteKey_cf(Value** arg_list, int count)
{
	check_arg_count("AddKey", 2, count);

	Value* pkNoteTrackTabValue = arg_list[0];
	Value* pkTimeValue = arg_list[1];

	if (!pkNoteTrackTabValue || !pkTimeValue)
		return &ok;

	if (pkNoteTrackTabValue == &undefined ||  
		pkNoteTrackTabValue == &unsupplied ||
		pkTimeValue == &undefined ||  
		pkTimeValue == &unsupplied)
	{
		return &ok;
	}

	TimeValue kTime = pkTimeValue->to_timevalue();

	ReferenceTarget* pkNoteTrackRefTarget = pkNoteTrackTabValue->to_reftarg();
	Class_ID cID = pkNoteTrackRefTarget->ClassID();
	if (cID == Class_ID(NOTETRACK_CLASS_ID,0))
	{
		DefNoteTrack* pkNoteTrack = (DefNoteTrack*) pkNoteTrackRefTarget;
		pkNoteTrack->AddNewKey(kTime, 0);
	}

	return &ok;
}