#ifndef __BCGCBPRO_H
#define __BCGCBPRO_H

// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.

#ifndef __AFXCMN_H__
	#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif

#if defined _AFXDLL && !defined _BCGCBPRO_STATIC_
	#ifdef _BCGCBPRO_
	   #define BCGCBPRODLLEXPORT  _declspec(dllexport)
	#else
	   #define BCGCBPRODLLEXPORT  _declspec(dllimport)
	#endif
#else
	#define BCGCBPRODLLEXPORT
#endif

#include "BCGPUserToolsManager.h"

inline BOOL IsStandardCommand (UINT uiCmd)
{
	return	((uiCmd >= ID_FILE_MRU_FILE1 && 
				uiCmd <= ID_FILE_MRU_FILE16)		||	// MRU commands,
			(uiCmd >= 0xF000 && uiCmd < 0xF1F0)		||	// system commands,
			((int) uiCmd >= AFX_IDM_FIRST_MDICHILD)	||	// windows commands
			(uiCmd >= ID_OLE_VERB_FIRST && uiCmd <= ID_OLE_VERB_LAST) ||		// OLE commands
			g_pUserToolsManager != NULL && uiCmd == g_pUserToolsManager->GetToolsEntryCmd ());
}


BCGCBPRODLLEXPORT void BCGCBProSetResourceHandle (
	HINSTANCE hinstResDLL);

BCGCBPRODLLEXPORT void BCGCBProCleanUp ();

extern BCGCBPRODLLEXPORT CFrameWnd* g_pTopLevelFrame;

inline BCGCBPRODLLEXPORT void BCGCBProSetTopLevelFrame (CFrameWnd* pFrame)
{
	g_pTopLevelFrame = pFrame;
}

inline BCGCBPRODLLEXPORT CFrameWnd* BCGCBProGetTopLevelFrame (const CWnd* pWnd)
{
	ASSERT_VALID (pWnd);
	return g_pTopLevelFrame == NULL ? pWnd->GetTopLevelFrame () : g_pTopLevelFrame;
}

BCGCBPRODLLEXPORT CFrameWnd* BCGPGetParentFrame (const CWnd* pWnd);

#define BCG_GET_X_LPARAM(lp)		((int)(short)LOWORD(lp))
#define BCG_GET_Y_LPARAM(lp)		((int)(short)HIWORD(lp))

class BCGCBPRODLLEXPORT CBCGPMemDC
{
public:
	static BOOL	m_bUseMemoryDC;

	CBCGPMemDC(CDC& dc, CWnd* pWnd) :
		m_dc (dc),
		m_bMemDC (FALSE),
		m_pOldBmp (NULL)
	{
		ASSERT_VALID(pWnd);

		pWnd->GetClientRect (m_rect);

		m_rect.right += pWnd->GetScrollPos (SB_HORZ);
		m_rect.bottom += pWnd->GetScrollPos (SB_VERT);

		if (m_bUseMemoryDC &&
			m_dcMem.CreateCompatibleDC (&m_dc) &&
			m_bmp.CreateCompatibleBitmap (&m_dc, m_rect.Width (), m_rect.Height ()))
		{
			//-------------------------------------------------------------
			// Off-screen DC successfully created. Better paint to it then!
			//-------------------------------------------------------------
			m_bMemDC = TRUE;
			m_pOldBmp = m_dcMem.SelectObject (&m_bmp);
		}
	}

	virtual ~CBCGPMemDC()
	{
		if (m_bMemDC)
		{
			//--------------------------------------
			// Copy the results to the on-screen DC:
			//-------------------------------------- 
			CRect rectClip;
			int nClipType = m_dc.GetClipBox (rectClip);

			if (nClipType != NULLREGION)
			{
				if (nClipType != SIMPLEREGION)
				{
					rectClip = m_rect;
				}

				m_dc.BitBlt (rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(),
							   &m_dcMem, rectClip.left, rectClip.top, SRCCOPY);
			}

			m_dcMem.SelectObject (m_pOldBmp);
		}
	}

	CDC& GetDC ()			{	return m_bMemDC ? m_dcMem : m_dc;	}
	BOOL IsMemDC () const	{	return m_bMemDC;	}

protected:
	CDC&		m_dc;
	BOOL		m_bMemDC;
	CDC			m_dcMem;
	CBitmap		m_bmp;
	CBitmap*	m_pOldBmp;
	CRect		m_rect;
};

#if _MSC_VER < 1300

//------------------------------
// Windows 64 bit compatibility:
//------------------------------

#ifndef GetClassLongPtr
#define GetClassLongPtr		GetClassLong
#endif

#ifndef SetClassLongPtr
#define SetClassLongPtr		SetClassLong
#endif

#ifndef SetWindowLongPtr
#define SetWindowLongPtr	SetWindowLong
#endif

#ifndef GetWindowLongPtr
#define GetWindowLongPtr	GetWindowLong
#endif

#define	DWORD_PTR			DWORD
#define	INT_PTR				int
#define	UINT_PTR			UINT
#define	LONG_PTR			LONG

#ifndef GWLP_WNDPROC
#define	GWLP_WNDPROC		GWL_WNDPROC
#endif

#ifndef GCLP_HICON
#define	GCLP_HICON			GCL_HICON
#endif

#ifndef GCLP_HICONSM
#define	GCLP_HICONSM		GCL_HICONSM
#endif

#ifndef GCLP_HBRBACKGROUND
#define GCLP_HBRBACKGROUND	GCL_HBRBACKGROUND
#endif

#endif // _MSC_VER

#endif // __BCGCBPRO_H
