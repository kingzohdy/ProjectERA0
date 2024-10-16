#ifndef __SCREENCAP_H__
#define __SCREENCAP_H__
#include <string>
#include <vector>
#include <Windows.h>

// Desktop screenshot capture
class CScreenCapture
{
public:

	// Constructor
	CScreenCapture();
	~CScreenCapture();

	// Returns virtual screen rectangle
	void GetScreenRect(LPRECT rcScreen);


	// Captures the specified screen area and returns the list of image files
	bool CaptureScreenRect( std::vector<RECT> arcCapture,	
							std::vector<std::string>& out_file_list );

	POINT m_ptCursorPos;                 // Current mouse cursor pos
	std::vector<RECT> m_arcCapture;      // Array of capture rectangles
	CURSORINFO m_CursorInfo;              // Cursor info  
	std::vector<std::string> m_out_file_list; // The list of output image files
};

#endif //__SCREENCAP_H__


