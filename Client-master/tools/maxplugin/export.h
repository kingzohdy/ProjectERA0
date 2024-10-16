
#ifndef __EXPORT_H__
#define __EXPORT_H__

#include "exp_types.h"

namespace DKEXP
{
	class DarkExport : public SceneExport
	{
		friend INT_PTR CALLBACK ExportOptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		DarkExport();
		~DarkExport();
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);	// Export file
		BOOL			SupportsOptions(int ext, DWORD options);

		bool DoActualExport();

	private:
		Ogre::ModelData *BuildModel(INode* prootnode, HierConverter &pHier);
		void BuildAndSaveModels(const char *path, INode *prootnoe, HierConverter &pHier);
	};

	extern ClassDesc* GetDarkExportDesc();
	extern void LoadExpOptionFromFile(const char *filename);
	extern void SaveExpOptionToFile(const char *filename);
}

#endif