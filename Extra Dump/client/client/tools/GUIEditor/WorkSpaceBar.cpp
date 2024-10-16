// workspace.cpp : implementation of the CWorkSpaceBar class
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "WorkSpaceBar.h"

#include "xml_uimgr.h"
#include "OgreResourceManager.h"

#include "ui_gameui.h"
#include "ui_framemgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;
/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar

//void CWorkSpaceBar::DoDataExchange(CDataExchange* pDX)
//{
//	CBCGPDockingControlBar::DoDataExchange(pDX);
//	DDX_Control(pDX, IDT_OBJECT_TREE, m_wndTree);
//}


BEGIN_MESSAGE_MAP(CWorkSpaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkSpaceBar)
	ON_NOTIFY(TVN_SELCHANGED, IDT_OBJECT_TREE, &CWorkSpaceBar::OnTvnSelchangedObjTree)	
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_FILE_OPEN, &CWorkSpaceBar::OnFileOpen)
//	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar construction/destruction

CWorkSpaceBar::CWorkSpaceBar()
{
	// TODO: add one-time construction code here
}

CWorkSpaceBar::~CWorkSpaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceBar message handlers

int CWorkSpaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, IDT_OBJECT_TREE))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Setup trees content:
	

	return 0;
}

void CWorkSpaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkSpaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

bool CWorkSpaceBar::TocFileOpen()
{
	CFileDialog fileOpenDlg( TRUE );
	std::string xmlPostfix = ".xml";
	//打开toc文件
	fileOpenDlg.m_ofn.lpstrFilter	= _T("Toc Files(*.toc)\0*.toc\0All Files(*.*)\0*.*\0");

	if(IDOK == fileOpenDlg.DoModal())
	{	
		HTREEITEM hRootToc = m_wndTree.InsertItem ( fileOpenDlg.GetFileName() );
		std::string tocPath( fileOpenDlg.GetPathName() );
		Ogre::DataStream *fp = Ogre::ResourceManager::getSingleton().openFileStream( tocPath, true );

		if( fp == NULL )
		{
			return false;
		}
		//读取toc文件并用vector保存xml文件名（以后如有需要可以同时保存每个xml文件路径）
		char buffer[1024];
		std::vector <std::string> xmlFileSet;
		while ( !fp->eof() )
		{
			fp->readLine( buffer, sizeof(buffer) );
			std::string inbuf = buffer;
			size_t pos;
			pos = inbuf.find( xmlPostfix );
			if ((pos!=inbuf.npos) &&((inbuf.find("##"))>0))
			{
				size_t pos2 = inbuf.find_last_of("/")+1;
				xmlFileSet.push_back( inbuf.substr( pos2,inbuf.length()-pos2 ) );
			}
		}
		//遍历vector分别读取xml文件
		std::string xmlName;
		std::vector <std::string>::iterator xmlFileIter;
		HTREEITEM hRootXML;
		for( xmlFileIter = xmlFileSet.begin(); xmlFileIter != xmlFileSet.end(); ++xmlFileIter )
		{			
			xmlName = *xmlFileIter;
			//设置Toc文件为根节点
			hRootXML = m_wndTree.InsertItem( xmlName.c_str(), hRootToc );
			Ogre::XMLData xmldata;

			if(!xmldata.loadFile(xmlName.c_str()))
			{
				return false;
			}
			//解析xml，获得标签节点
			Ogre::XMLNode pRootNodeXML = xmldata.getRootNode();
			Ogre::XMLNode pObjectNode = pRootNodeXML.iterateChild();

			while( !pObjectNode.isNull())
			{
				const char *objectType(pObjectNode.getName());
				//若为Frame标签且存在name属性则读取name属性
				if ( !stricmp(objectType, "Frame")&&pObjectNode.hasAttrib("name") )
				{
					const char *objectName( pObjectNode.attribToString("name") );
					m_wndTree.InsertItem ( objectName, hRootXML );		
				}
				pObjectNode = pRootNodeXML.iterateChild( pObjectNode );
			}
		}		
		m_wndTree.Expand( hRootToc, TVE_EXPAND );
		m_strCurObjName = m_wndTree.GetItemText( hRootXML );
		return true;
	}
	return true;
}

void CWorkSpaceBar::OnTvnSelchangedObjTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// //TODO: Add your control notification handler code here
	//UpdateData(true);
	HTREEITEM selectItem = m_wndTree.GetSelectedItem();
	
	m_strLastObjName = m_strCurObjName;
	m_strCurObjName = m_wndTree.GetItemText(selectItem);
	::SendMessage( AfxGetMainWnd()->m_hWnd, WM_MY_MESSAGE, NULL, NULL );
}
