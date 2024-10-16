
#include "stdafx.h"
#include "export.h"
#include <Shlobj.h>
#include <set>
#include "exp_nodetree.h"
#include "exp_mesh.h"

using namespace Ogre;

namespace DKEXP
{
#define EXP_OPT_VERSION 102

	std::set<INode *>g_SelectNodes;

	void LoadExpOptionFromFile(const char *filename)
	{
		memset(&g_ExpOption, 0, sizeof(g_ExpOption));
		g_ExpOption.exp_normal = 1;
		g_ExpOption.exp_numuv = 1;


		FILE *fp = fopen(filename, "rb");
		if(fp == NULL)
		{
			return;
		}

		int version;
		fread(&version, sizeof(version), 1, fp);
		if(version == EXP_OPT_VERSION)
		{
			fread(&g_ExpOption, sizeof(ExportOption), 1, fp);
		}

		fclose(fp);
	}

	void SaveExpOptionToFile(const char *filename)
	{
		FILE *fp = fopen(filename, "wb");
		if(fp == NULL)
		{
			return;
		}

		int version = EXP_OPT_VERSION;
		fwrite(&version, sizeof(version), 1, fp);
		fwrite(&g_ExpOption, sizeof(ExportOption), 1, fp);

		fclose(fp);
	}

	void InitSelectNodes()
	{
		g_SelectNodes.clear();
		for(int i=0; i<g_Interface->GetSelNodeCount(); i++)
		{
			g_SelectNodes.insert(g_Interface->GetSelNode(i));
		}
	}

	bool IsNodeInSelect(INode *pnode)
	{
		return g_SelectNodes.find(pnode)!=g_SelectNodes.end();
	}


#define DARK_EXPORT_CLASS_ID Class_ID(0x6d1d3fc9, 0x3a491d6c)
	class ExporterClassDesc:public ClassDesc
	{
	public:
		int 			IsPublic() {return 1;}
		void *			Create(BOOL loading = FALSE) {return new DarkExport;}
		const TCHAR *	ClassName() {return _T("DarkExport");}
		SClass_ID		SuperClassID() {return SCENE_EXPORT_CLASS_ID;}
		Class_ID		ClassID() {return DARK_EXPORT_CLASS_ID;}
		const TCHAR* 	Category() {return _T("");}
	};

	static ExporterClassDesc DarkExportDesc;
	ClassDesc* GetDarkExportDesc() {return &DarkExportDesc;}

	DarkExport::DarkExport() {
	}

	DarkExport::~DarkExport() {
	}

	int
		DarkExport::ExtCount() {
			return 2;
	}

	const TCHAR *
		DarkExport::Ext(int n) {		// Extensions supported for import/export modules
			switch(n) {
		case 0:
			return _T("omod");
		case 1:
			return _T("oanim");
			}
			return _T("");
	}

	const TCHAR *
		DarkExport::LongDesc() {			// Long ASCII description (i.e. "Targa 2.0 Image File")
			return _T("Dark Model File");
	}

	const TCHAR *
		DarkExport::ShortDesc() {			// Short ASCII description (i.e. "Targa")
			return "Dark";
	}

	const TCHAR *
		DarkExport::AuthorName() {			// ASCII Author name
			return _T("");
	}

	const TCHAR *
		DarkExport::CopyrightMessage() {	// ASCII Copyright message
			return _T("");
	}

	const TCHAR *
		DarkExport::OtherMessage1() {		// Other message #1
			return _T("");
	}

	const TCHAR *
		DarkExport::OtherMessage2() {		// Other message #2
			return _T("");
	}

	unsigned int
		DarkExport::Version() {				// Version number * 100 (i.e. v3.01 = 301)
			return 200;
	}

	void
		DarkExport::ShowAbout(HWND hWnd) {			// Optional
	}

	void UpdateDlgData(HWND hdlg, bool getdata)
	{
		if(getdata)
		{
			g_ExpOption.exptype = SendMessage(GetDlgItem(hdlg, IDC_COMBO_EXPTYPE), CB_GETCURSEL, 0, 0);
			g_ExpOption.exp_normal = IsDlgButtonChecked(hdlg, IDC_CHECK_NORMAL) != 0;
			g_ExpOption.exp_vertcolor = IsDlgButtonChecked(hdlg, IDC_CHECK_VERTEXCOLOR) != 0;
			g_ExpOption.exp_tangent = IsDlgButtonChecked(hdlg, IDC_CHECK_TANGENT) != 0;
			g_ExpOption.exp_numuv = 1+SendMessage(GetDlgItem(hdlg, IDC_COMBO_UVSET), CB_GETCURSEL, 0, 0);

/*
			g_ExpOption.exp_boneanim = IsDlgButtonChecked(hdlg, IDC_CHECK_BONEANIM) != 0;
			g_ExpOption.exp_vertanim = IsDlgButtonChecked(hdlg, IDC_CHECK_VERTANIM) != 0;
			g_ExpOption.exp_uvanim = IsDlgButtonChecked(hdlg, IDC_CHECK_UVANIM) != 0;
			g_ExpOption.exp_mtlanim = IsDlgButtonChecked(hdlg, IDC_CHECK_MTLANIM) != 0;
*/
			g_ExpOption.anim_start = GetDlgItemInt(hdlg, IDC_EDIT_ANIMFRAMESTART, NULL, FALSE);
			g_ExpOption.anim_end = GetDlgItemInt(hdlg, IDC_EDIT_ANIMFRAMEEND, NULL, FALSE);
			g_ExpOption.keydiff_translate = GetDlgItemFloat(hdlg, IDC_EDIT_KEYDIFF_TRANSLATE, NULL);
			g_ExpOption.keydiff_rotate = GetDlgItemFloat(hdlg, IDC_EDIT_KEYDIFF_ROTATE, NULL);
			g_ExpOption.keydiff_scale = GetDlgItemFloat(hdlg, IDC_EDIT_KEYDIFF_SCALE, NULL);

			GetDlgItemText(hdlg, IDC_EDIT_SRCROOTDIR, g_ExpOption.src_rootdir, 256);
			GetDlgItemText(hdlg, IDC_EDIT_DESTROOTDIR, g_ExpOption.dest_rootdir, 256);
		}
		else
		{
			SendMessage(GetDlgItem(hdlg, IDC_COMBO_EXPTYPE), CB_SETCURSEL, g_ExpOption.exptype, 0);
			CheckDlgButton(hdlg, IDC_CHECK_NORMAL, g_ExpOption.exp_normal);
			CheckDlgButton(hdlg, IDC_CHECK_VERTEXCOLOR, g_ExpOption.exp_vertcolor);
			CheckDlgButton(hdlg, IDC_CHECK_TANGENT, g_ExpOption.exp_tangent);
			SendMessage(GetDlgItem(hdlg, IDC_COMBO_UVSET), CB_SETCURSEL, g_ExpOption.exp_numuv-1, 0);
/*
			CheckDlgButton(hdlg, IDC_CHECK_BONEANIM, g_ExpOption.exp_boneanim);
			CheckDlgButton(hdlg, IDC_CHECK_VERTANIM, g_ExpOption.exp_vertanim);
			CheckDlgButton(hdlg, IDC_CHECK_UVANIM, g_ExpOption.exp_uvanim);
			CheckDlgButton(hdlg, IDC_CHECK_MTLANIM, g_ExpOption.exp_mtlanim);
*/

			SetDlgItemInt(hdlg, IDC_EDIT_ANIMFRAMESTART, g_ExpOption.anim_start, FALSE);
			SetDlgItemInt(hdlg, IDC_EDIT_ANIMFRAMEEND, g_ExpOption.anim_end, FALSE);
			SetDlgItemFloat(hdlg, IDC_EDIT_KEYDIFF_TRANSLATE, g_ExpOption.keydiff_translate);
			SetDlgItemFloat(hdlg, IDC_EDIT_KEYDIFF_ROTATE, g_ExpOption.keydiff_rotate);
			SetDlgItemFloat(hdlg, IDC_EDIT_KEYDIFF_SCALE, g_ExpOption.keydiff_scale);

			SetDlgItemText(hdlg, IDC_EDIT_SRCROOTDIR, g_ExpOption.src_rootdir);
			SetDlgItemText(hdlg, IDC_EDIT_DESTROOTDIR, g_ExpOption.dest_rootdir);
		}
	}

	/*
	int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
	switch(uMsg)
	{
	case BFFM_INITIALIZED:
	//::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)g_strFilePath);
	//::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)g_strSrcTexPath);   
	//::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)g_strFileSubPath);   
	break;
	case BFFM_SELCHANGED:
	{
	char curr[MAX_PATH];
	SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);   
	if(curr[strlen(curr)-1]==92)   
	sprintf(curr,"%sSictech",curr);
	else
	sprintf(curr,"%s\\Sictech",curr);
	::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)curr);   
	}   
	break;   
	}
	return 0;
	} */  

	static int SelectFolder(HWND hParent, char* strFolder)
	{
		LPMALLOC lpMalloc;

		if (::SHGetMalloc(&lpMalloc) != NOERROR) return 0;

		char szDisplayName[_MAX_PATH];
		char szBuffer[_MAX_PATH];

		ITEMIDLIST *ppidl;
		SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP ,&ppidl);

		BROWSEINFO browseInfo;
		browseInfo.hwndOwner = hParent;
		browseInfo.pidlRoot = ppidl; // set root at Desktop
		browseInfo.pszDisplayName = szDisplayName;
		browseInfo.lpszTitle = "选择目录";
		browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
		browseInfo.lpfn = NULL;//BrowseCallbackProc;
		browseInfo.lParam = 0;

		LPITEMIDLIST lpItemIDList;
		if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
		{
			// Get the path of the selected folder from the    item ID list.
			if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
			{
				// At this point, szBuffer contains the path the user chose.
				if (szBuffer[0] == '\0') return 0;

				// We have a path in szBuffer! Return it.
				strcpy(strFolder, szBuffer);
				size_t len = strlen(strFolder);
				if(strFolder[len]=='\\') strFolder[len] = 0;

				return 1;
			}
			else return 1; // strResult is empty

			lpMalloc->Free(lpItemIDList);
			lpMalloc->Release();
		}

		return 1;
	}

	static INT_PTR CALLBACK ExportOptionsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
	{
		static DarkExport* exp;

		switch(message)
		{
		case WM_INITDIALOG:
			//SetWindowContextHelpId(hDlg, idh_3dsexp_export);
			exp = (DarkExport*) lParam;
			CenterWindow(hDlg,GetParent(hDlg));
			SetFocus(hDlg); // For some reason this was necessary.  DS-3/4/96
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_EXPTYPE), CB_ADDSTRING, 0, (LPARAM)"动画模型");
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_EXPTYPE), CB_ADDSTRING, 0, (LPARAM)"静态模型");
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_EXPTYPE), CB_ADDSTRING, 0, (LPARAM)"拼接用模型");
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_EXPTYPE), CB_ADDSTRING, 0, (LPARAM)"单个的动画");
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_UVSET), CB_ADDSTRING, 0, (LPARAM)"1");
			SendMessage(GetDlgItem(hDlg, IDC_COMBO_UVSET), CB_ADDSTRING, 0, (LPARAM)"2");
			UpdateDlgData(hDlg, false);
			return FALSE;

		case WM_DESTROY:
			return FALSE;

		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{
			case IDOK:
				UpdateDlgData(hDlg, true);
				EndDialog(hDlg, 1);
				return TRUE;
			case IDCANCEL:
				EndDialog(hDlg, 0);
				return TRUE;
			case IDC_BTN_SRCROOTDIR:
				SelectFolder(hDlg, g_ExpOption.src_rootdir);
				UpdateDlgData(hDlg, false);
				break;
			case IDC_BTN_DESTROOTDIR:
				SelectFolder(hDlg, g_ExpOption.dest_rootdir);
				UpdateDlgData(hDlg, false);
				break;
			}
		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_CONTEXTHELP)
			{
				//DoHelp(HELP_CONTEXT, idh_3dsexp_export);
				return FALSE;
			}
		}
		return FALSE;
	}

	static DWORD WINAPI ProgressBarCallback(LPVOID arg)
	{
		return(0);
	}

	int DarkExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
	{
		g_Interface = i;
		g_ExpOption.anim_start = i->GetAnimRange().Start() / GetTicksPerFrame();
		g_ExpOption.anim_end = i->GetAnimRange().End() / GetTicksPerFrame();

		if(!suppressPrompts)
		{
			int res = DialogBoxParam(g_hInstance, MAKEINTRESOURCE(IDD_DLG_EXPORTPARAM), GetActiveWindow(), ExportOptionsDlgProc, (LPARAM)this);
			if(res <= 0) return 0; //cancel
		}

		char cfgpath[256];
		sprintf(cfgpath, "%splugins\\DarkPlugin.cfg", GetMaxRootDir());
		SaveExpOptionToFile(cfgpath);

		strcpy(g_ExpOption.savepath, name);
		strlwr(g_ExpOption.savepath);

		i->ProgressStart(_T("正在导出:"), TRUE, ProgressBarCallback, NULL );
		bool b = DoActualExport();
		i->ProgressEnd();

		return b;
	}

	BOOL DarkExport::SupportsOptions(int ext, DWORD options)
	{
		return TRUE;
	}

	bool DarkExport::DoActualExport()
	{
		InitSelectNodes();

		if(g_ExpOption.anim_start < 0)
		{
			MessageBox(NULL, "动画起始帧不能为负数", "Error", MB_OK);
			return false;
		}
		HierConverter hier(g_ExpOption.anim_start*GetTicksPerFrame(), g_ExpOption.anim_end*GetTicksPerFrame());

		INode *prootnode = g_Interface->GetRootNode();
		MeshConverter::Preprocess(prootnode);
		int iStatus = hier.BuildNode(g_Interface, prootnode);

		if ((int)hier.m_pBoneNodes.size() > 200)
		{
			MessageBox(0, "骨骼数目>200过多!", "Nodes", 0);
			goto error_exit;
		}
		
		if(iStatus)
		{
			goto error_exit;
		}

		if(!hier.m_bHasBounding && g_ExpOption.exptype==EXP_ANIM_MODEL)
		{
			MessageBox(NULL, "没有为该动画模型需要添加包围盒", "Error", MB_OK);
			goto error_exit;
		}

		iStatus = hier.Convert();
		if(iStatus)
		{
			goto error_exit;
		}

		if(g_ExpOption.exptype == EXP_PURE_ANIM)
		{
			AnimationData *panim = hier.buildAnimationData(g_ExpOption.keydiff_translate, g_ExpOption.keydiff_rotate, g_ExpOption.keydiff_scale);
			char tmppath[1024];
			strcpy(tmppath, g_ExpOption.savepath);
			char *pdot = strrchr(tmppath, '.');
			if(pdot) strcpy(pdot, ".oanim");

			panim->save(tmppath);
			panim->release();
		}
		else if(g_ExpOption.exptype==EXP_ANIM_MODEL || g_ExpOption.exptype==EXP_STATIC_MODEL)
		{
			ModelData *pmodeldata = BuildModel(prootnode, hier);
			pmodeldata->save(g_ExpOption.savepath);
			pmodeldata->release();
		}
		else if(g_ExpOption.exptype == EXP_LINK_MESH)
		{
			BuildAndSaveModels(g_ExpOption.savepath, prootnode, hier);
		}

		MeshConverter::Postprocess(prootnode);

		return true;

error_exit:
		MeshConverter::Postprocess(prootnode);
		return false;
	}

	void DarkExport::BuildAndSaveModels(const char *path, INode *prootnoe, HierConverter &pHier)
	{
		char modelname[1024];
		strcpy(modelname, path);
		char *pdot = strrchr(modelname, '\\');
		if(pdot) *pdot = NULL;

		for(size_t i=0; i<pHier.m_MeshNodes.size(); i++)
		{
			ModelData *pmodeldata = new ModelData;
			HierConverter::MeshNodeData &meshnode = pHier.m_MeshNodes[i];

			pmodeldata->m_pBspData = pHier.getBSPData(meshnode.pmaxnode);

			Ogre::MeshData *pmeshdata = meshnode.pmesh;
			pmeshdata->addRef();
			pmodeldata->addMeshData(pmeshdata);
			pmodeldata->m_Bounding = pmeshdata->m_Bounding.getBox();

			char fullpath[1024];
			sprintf(fullpath, "%s\\%s.omod", modelname, (const char *)pmeshdata->m_Name);

			pmodeldata->save(fullpath);
		}
	}

	Ogre::ModelData *DarkExport::BuildModel(INode* prootnode, HierConverter &pHier)
	{
		ModelData *pmodeldata = new ModelData;
		pmodeldata->m_pSkeleton = pHier.BuildSkeletonChunk(prootnode);

		if(g_ExpOption.exptype == EXP_ANIM_MODEL)
		{
			pmodeldata->addAnimation(pHier.buildAnimationData(g_ExpOption.keydiff_translate, g_ExpOption.keydiff_rotate, g_ExpOption.keydiff_scale));
		}

		pmodeldata->m_Bounding = pHier.BuildBoundingChunk();

		pmodeldata->m_pBspData = pHier.getBSPData(prootnode);

		size_t iNumAnchors;
		iNumAnchors = pHier.m_Anchors.size();
		pmodeldata->m_Anchors.resize(iNumAnchors);
		for (size_t i=0; i<iNumAnchors; i++)
		{
			pmodeldata->m_Anchors[i].boneid = pHier.m_Anchors[i].boneid;
			pmodeldata->m_Anchors[i].anchorid = pHier.m_Anchors[i].anchorid;
		}

		for(size_t i=0; i<pHier.m_MeshNodes.size(); i++)
		{
			Ogre::MeshData *pmesh = pHier.m_MeshNodes[i].pmesh;
			pmesh->addRef();
			pmodeldata->addMeshData(pmesh);
		}

		//pmodeldata->addAnimation(pHier.BuildMtlAnimChunk());

		return pmodeldata;
		
		return NULL;
	}
}
