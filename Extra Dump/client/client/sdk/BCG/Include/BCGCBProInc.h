//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library (Professional Version)
// Copyright (C) 1998-2001 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

#ifndef __BCGCBPROINC_H
#define __BCGCBPROINC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#include "BCGCBProVer.h"	// Library version info.

#ifdef _BCGCBPRO_IN_OTHER_DLL
BCGCBPRODLLEXPORT void BCGCBPRODllInitialize ();
#endif // _BCGCBPRO_IN_OTHER_DLL

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
	//------------------------------------------
	// MFC shared DLL, BCGControlBar shared DLL
	//------------------------------------------
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCBPRO674UD.lib") 
			#pragma message("Automatically linking with BCGCBPRO674UD.dll")
		#else
			#pragma comment(lib,"BCGCBPRO674d.lib") 
			#pragma message("Automatically linking with BCGCBPRO674d.dll")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCBPRO674U.lib") 
			#pragma message("Automatically linking with BCGCBPRO674U.dll") 
		#else
			#pragma comment(lib,"BCGCBPRO674.lib") 
			#pragma message("Automatically linking with BCGCBPRO674.dll") 
		#endif
	#endif
#elif defined _BCGCBPRO_STATIC_
	//---------------------------------------------
	// MFC shared DLL, BCGControlBar static library
	//---------------------------------------------
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCBPROStaticuds674.lib") 
			#pragma message("Automatically linking with BCGCBPROStaticuds674.lib") 
		#else
			#pragma comment(lib,"BCGCBPROStaticds674.lib") 
			#pragma message("Automatically linking with BCGCBPROStaticds674.lib") 
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCBPROStaticus674.lib") 
			#pragma message("Automatically linking with BCGCBPROStaticus674.lib") 
		#else
			#pragma comment(lib,"BCGCBPROStatics674.lib") 
			#pragma message("Automatically linking with BCGCBPROStatics674.lib") 
		#endif
	#endif
#else
	//--------------------------------------------------
	// MFC static library, BCGControlBar static library
	//--------------------------------------------------
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCBPROStaticud674.lib") 
			#pragma message("Automatically linking with BCGCBPROStaticud674.lib") 
		#else
			#pragma comment(lib,"BCGCBPROStaticd674.lib") 
			#pragma message("Automatically linking with BCGCBPROStaticd674.lib") 
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCBPROStaticU674.lib") 
			#pragma message("Automatically linking with BCGCBPROStaticU674.lib") 
		#else
			#pragma comment(lib,"BCGCBPROStatic674.lib") 
			#pragma message("Automatically linking with BCGCBPROStatic674.lib") 
		#endif
	#endif
#endif 

//------------------
// BCG control bars:
//------------------
#include "BCGPToolBar.h"
#include "BCGPStatusBar.h"
#include "BCGPMenuBar.h"
#include "BCGPDialogBar.h"
#include "BCGPOutlookBar.h"
#include "BCGPOutlookWnd.h"
#include "BCGPColorBar.h"
#include "BCGPCaptionBar.h"
#include "BCGPOutlookBarPane.h"
#include "BCGPTabbedControlBar.h"
#include "BCGPTabbedToolbar.h"
#include "BCGPToolBox.h"
#include "BCGPTasksPane.h"
#include "BCGPCalendarBar.h"
#include "BCGPCalculator.h"

//-------------------------
// BCG control bar buttons:
//-------------------------
#include "BCGPToolbarButton.h"
#include "BCGPToolbarComboBoxButton.h"
#include "BCGPToolbarMenuButton.h"
#include "BCGPToolbarEditBoxButton.h"
#include "BCGPToolbarSpinEditBoxButton.h"
#include "BCGPDropDown.h"
#include "BCGPColorMenuButton.h"
#include "BCGPToolbarFontCombo.h"
#include "BCGPToolbarDateTimeCtrl.h"
#include "BCGPCalendarMenuButton.h"
#include "BCGPOutlookButton.h"

//-------------------------
// Autohide bars
//-------------------------
#include "BCGPAutoHideButton.h"
#include "BCGPAutoHideToolBar.h"

//-------------------------------------------------------------------
// BCG frame and miniframe windows (replaces CFrameWnd, CMDIFrameWnd, 
// CMDIChildWnd, COleIPFrameWnd and COleDocIPFrameWnd):
//-------------------------------------------------------------------
#include "BCGPFrameWnd.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPOleIPFrameWnd.h"
#include "BCGPOleDocIPFrameWnd.h"
#include "BCGPOleCntrFrameWnd.h"

#include "BCGPMiniFrameWnd.h"
#include "BCGPMultiMiniFrameWnd.h"

//-------------------------
// BCG customization stuff:
//-------------------------
#include "BCGPToolbarCustomize.h"
#include "BCGPCustomizeMenuButton.h"

#include "BCGPContextMenuManager.h"
#include "BCGPKeyboardManager.h"
#include "BCGPMouseManager.h"

#include "BCGPUserTool.h"
#include "BCGPKeyHelper.h"

//-----------------------
// BCG workspace manager
//-----------------------
#include "BCGPWorkspace.h"
#include "BCGPRegistry.h"
#include "BCGPRebarState.h"

//-------------
// BCG helpers:
//-------------
#include "BCGPTearOffManager.h"
#include "BCGPDrawManager.h"
#include "BCGPDockBarRow.h"

//-----------------------
// BCG menu replacements:
//-----------------------
#include "BCGPPopupMenu.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPToolBarImages.h"

//------------------
// BCG docking bars:
//------------------
#include "BCGPDockingControlBar.h"
#include "BCGPTabWnd.h"
#include "BCGPDockingCBWrapper.h"

//--------------
// BCG controls:
//--------------
#include "BCGPButton.h"
#include "BCGPEdit.h"
#include "BCGPColorButton.h"
#include "BCGPMenuButton.h"
#include "BCGPURLLinkButton.h"
#include "BCGPEditListBox.h"
#include "BCGPAnimCtrl.h"
#include "BCGPPowerColorPicker.h"
#include "BCGPFontComboBox.h"
#include "BCGPReBar.h"
#include "BCGPShellTree.h"
#include "BCGPShellList.h"
#include "BCGPPropList.h"
#include "BCGPEditCtrl.h"
#include "BCGPIntelliSenseWnd.h"
#include "BCGPIntelliSenseLB.h"
#include "BCGPMaskEdit.h"
#include "BCGPDateTimeCtrl.h"
#include "BCGPDurationCtrl.h"
#include "BCGPHotSpotImageCtrl.h"
#include "BCGPSplitterWnd.h"
#include "BCGPSpinButtonCtrl.h"

//-------------
// BCG dialogs:
//-------------
#include "BCGPDialog.h"
#include "BCGPFileDialog.h"
#include "BCGPWindowsManagerDlg.h"
#include "BCGPPrintPreviewView.h"
#include "BCGPImageEditDlg.h"
#include "BCGPColorDialog.h"
#include "BCGPKeyMapDlg.h"
#include "BCGPPropertySheet.h"
#include "BCGPPropertyPage.h"

//-----------
// BCG views:
//-----------
#include "BCGPTabView.h"
#include "BCGPEditView.h"

//--------------------
// Visualization stuf:
//--------------------
#include "BCGPVisualManager.h"
#include "BCGPVisualManagerXP.h"
#include "BCGPWinXPVisualManager.h"
#include "BCGPVisualManager2003.h"
#include "BCGPSkinManager.h"


//-------------------------
// OLE Full Server Support:
//-------------------------
#include "BCGPOleServerDoc.h"

//-------------------------
// Global Utils:
//-------------------------
#include "BCGPGlobalUtils.h"

#endif // __BCGCBPROINC_H
