#include ".\mpath.h"

#include "mstream.h"
#include "TerrainTile.h"
#include "EditorModel.h"
#include "mterrainedit.h"
#include "EnvRegion.h"
#include "Coordinate.h"

#include "OgreModel.h"
#include "OgreResDB.h"
#include "OgreLines.h"
#include "OgreRay.h"
#include "GameResDBTypes.h"

using namespace TE;
using namespace Ogre;

struct PATHDESC
{
	int iD;
	MVec3 point[10];
	int type;
	int num;
	int mapIndex;
};

MPath::MPath(void)
{
     memset( &m_PathData , 0 ,sizeof( MPathData ));
	 memset( &m_PathRenderData, 0 , sizeof( MPathRenderData ));

	 m_PathData.color =  0xffaaaaaa;

	 if(!LoadHLSLData())
	 {
		 assert(0);
	 }
	 if(!CreateRenderData())
	 {
		 assert(0);
	 }

	 m_nSelectIndex = -1;
}

MPath::~MPath(void)
{
	Release();
}

void MPath::Release()
{
	m_PathData.data. clear ();

	for( size_t i = 0 ; i < m_PathData.pModel.size() ; i ++ )
	{	
		delete( m_PathData.pModel[i] );
	}

	m_PathData.pModel.clear();
}

void MPath::Render()
{
	if(!m_PathData.data.empty())
	{
		g_TerrainEdit.m_pLinesRenderer->addLineStrip(&m_PathData.data[0], m_PathData.data.size(), m_PathData.color, false);
	}
}

bool   MPath::Find(std::string &srcName,std::string &destName)
{
    if( m_PathData.srcName == srcName && m_PathData.destName == destName )
		return true;

	return false;
};	
	
bool   MPath::LoadHLSLData()
{
 	return true;
};
	
bool   MPath::SetColor(unsigned int color)
{
	m_PathData.color = color;
	return true;
};
	
bool   MPath::SetColor(int index,unsigned int color)
{
	m_PathData.color = color;
	return true;
};
bool  MPath::UpdataData()
{
	return true;
}

bool  MPath::CreateRenderData()
{
     return true;
};

	
bool   MPath::DelCurPos()
{
	if( m_PathData.data .empty ())
		return false;

	m_PathData.data .pop_back ();
	UpdataData();
		
	int index = m_PathData.pModel.size() - 1;
	delete( m_PathData.pModel[index] );

	m_PathData.pModel.pop_back();
	return true;
};

void   MPath::UpdateModelPosToPath()
{
	if( m_nSelectIndex == -1 )
		return;

	if( m_nSelectIndex >= (int)m_PathData.pModel.size() )
		return ;

	MovePos( m_PathData.pModel[m_nSelectIndex]->GetModelData()->pos , m_nSelectIndex+1 );

}

bool   MPath::TestPathModel( MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos )
{
	bool bFind = false;
	m_nSelectIndex = -1;

	Ray ray(pos, dir);

	for( size_t i = 0 ; i < m_PathData.pModel.size() ; i ++ )
	{
		TerrModel* pModel = m_PathData.pModel[i] ;
  
		EditorModel* pMayModel = (EditorModel*)pModel->GetModelData()->pModel ;
		   
		if( pMayModel == NULL )		
			continue;
          
		Model* pNode = pMayModel->GetShowModel()->getMainModel();
		if(pNode == NULL)
			 continue;

		if(pNode->intersectRay(INTERSECT_PICK, ray, NULL) && !bFind)
		{
			pModel->GetModelData ()->pDrawbox ->SetColor ( 0xff00ff00 );

			//if( mode == 1)
			{
				pEditor->m_pCurSelectModel  =  pModel ;
			}	
			pEditor->m_EditOption.pSelectModel = pModel ;

			pEditor->GetCoordinate()->bindObject ( pModel->GetModelData()->pModel  );

			m_nSelectIndex = i;

			return true ;
		}
		else
		{
			if( pModel->GetModelData ()->pDrawbox )
			{
				pModel->GetModelData ()->pDrawbox ->SetColor ( 0xffaaaaaa );
			}
		}
	}

	return false;
};

bool   MPath::MovePos( MVec3 pos , int index)
{
	if( (int)m_PathData.data .size () < index )
		return false;

	m_PathData.data[ index - 1] = pos ;
    m_PathData.pModel[index - 1]->GetModelData()->pModel->SetPos(pos);
	UpdataData();

	return true;
}

bool   MPath::MoveLastPos( MVec3 pos)
{
	if( m_PathData.data .empty ())
		return false;

	m_PathData.data[m_PathData.data.size ()-1] = pos ;

	m_PathData.pModel[m_PathData.data.size ()-1]->GetModelData()->pos = pos ;
	m_PathData.pModel[m_PathData.data.size ()-1]->ResetMat();

	UpdataData();
	return true;
     
}

void   MPath::AddPos( MVec3 pos )
{

	if( m_PathData.data .size () >= MAX_PAHT_POS_NUM)
		return;

    m_PathData.data .push_back ( pos );

	UpdataData();

    TerrModel *pModel = new TerrModel;
	pModel->GetModelData()->pos = pos;
	strcpy( pModel->GetModelData()->name , "toolres\\mapeditor\\anchor.omod" );
	pModel->Init();

	EditorModel * pMayModel = (EditorModel*)( pModel->GetModelData()->pModel );
	if( pMayModel )
	{		
		pMayModel->SetPos( pos );
	}

	pModel->AttachRoom();
	pModel->ResetMat();

	m_PathData.pModel.push_back( pModel );
};
	
void   MPath::SetName( const char* srcName,const char* destName)
{

};


MPathSet::MPathSet ()
{
	m_nCurSelectPathID = -1;
}
bool   MPathSet::MoveCurPos(MVec3 &pos)
{
	if( m_PathSet.empty ())
		return false;

	m_PathSet[m_PathSet.size () - 1]->MoveLastPos  ( pos );

     return true;
}

void  MPathSet::UpdateSelectPathPos()
{
	if( m_nCurSelectPathID == -1 )
		return;

	if( m_nCurSelectPathID >= (int)m_PathSet.size() )
		return;

	m_PathSet[m_nCurSelectPathID]->UpdateModelPosToPath();
}

bool   MPathSet::TestPathModel( MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos )
{
	m_nCurSelectPathID = -1;

	for( size_t i = 0 ; i <  m_PathSet.size() ; i ++ )
	{
		if( m_PathSet[i]->TestPathModel( pEditor , dir , pos ) )
		{
			m_nCurSelectPathID = i ;
			return true ;
		}
	}

	return false;
};	

int   MPathSet::DelCurPath()
{
    if( m_PathSet.empty ())
		 return -1;

	OGRE_DELETE(m_PathSet[m_PathSet.size ()-1]);

	m_PathSet.pop_back ();
	
	return m_PathSet.size();
};
	
bool   MPathSet::DelCurPathPos()
{
     if( m_PathSet.empty ())
		 return false;

	 m_PathSet[m_PathSet.size () - 1]->DelCurPos();
	 return true;
};
	
void   MPathSet::Release()
{      
	for(unsigned int i = 0 ; i < m_PathSet.size () ; i++)
	{
		OGRE_DELETE( m_PathSet[i] );
	}
	m_PathSet.clear ();
};

bool   MPathSet::AddCurPathPos( MVec3 &pos )
{
	if( m_PathSet.empty ())
		return false;

	m_PathSet[m_PathSet.size () - 1]->AddPos ( pos );
     return true;
};

MPathSet::~MPathSet()
{
   Release();
}	
	
void      MPathSet::Render ()
{
	for(unsigned int i = 0 ; i < m_PathSet.size () ; i++)
	{
		m_PathSet[i]->Render ();
	}
}

MPath*    MPathSet::GetPath(const char* srcName,const char *destName)
{
	std::string strSrc,strDest;
	strSrc = srcName;
	strDest = destName;
    for(unsigned int i = 0 ; i < m_PathSet.size () ; i++)
	{
		if( m_PathSet[i]->Find ( strSrc , strDest ))
		{
			return m_PathSet[i];
		}
	}

	return NULL;
};

MPath* MPathSet::GetPath(size_t index)
{
	if(index >= 0 && index < m_PathSet.size())
		return m_PathSet[index];
	else
		return NULL;
}

int   MPathSet::Add( const char* srcName , const char* destName)
{
	MPath *pPath = new MPath;
	pPath->SetColor( 0xffff0000 );
	pPath->SetName ( srcName, destName );
    m_PathSet.push_back (pPath);
	return m_PathSet.size()-1;
};

void MPathSet::Modify(int id , unsigned int type , char* description)
{
	if( m_PathSet.empty ())
		return;

	MPathData* pPath = m_PathSet[m_nCurSelectPathID]->GetPathData();
	pPath->pathID = id;
	pPath->uiType = type;
	int len = strlen(description);
	strncpy(pPath->strDescription , description , len > 127 ? 127 : len);
}

void MPathSet::Add(int id , unsigned int type , char* description)
{
	MPath *pPath = new MPath;
	pPath->SetColor( 0xffff0000 );
	pPath->GetPathData()->pathID = id;
	pPath->GetPathData()->uiType = type;
	int len = strlen(description);
	strncpy(pPath->GetPathData()->strDescription , description , len > 127 ? 127 : len);
	m_PathSet.push_back (pPath);
	m_nCurSelectPathID = m_PathSet.size()-1;
}

bool MPathSet::ReadFromDB()
{/*
	ResTable *ptable = g_pdb->GetTable("PathDesc");
	if(ptable)
	{
		assert(0);
		unsigned int condition = 0;   ///< 前提是必须要填写一个mapid
		size_t num = ptable->FindRecord(NULL, 0, "MapIndex" , condition);
		if( num == 0 )
			return false;

		PATHDESC* pPathDataSet = new PATHDESC [num];

		if(ptable->FindRecord( pPathDataSet , num , "MapIndex" , condition) < 0)
			return false;
		Release();
		for(size_t i = 0; i < num; ++i)
		{
			m_nCurSelectPathID = i;
			Add(pPathDataSet[i].iD , pPathDataSet[i].type , "无");
			for(size_t j = 0; j < pPathDataSet[i].num; ++j)
			{
				MVec3 pos;
				pos.x = (float)pPathDataSet[i].point[j].x;
				pos.y = (float)pPathDataSet[i].point[j].y;
				pos.z = (float)pPathDataSet[i].point[j].z;
				AddCurPathPos(pos);
			}
		}

		MSAFE_DELETE_ARRAY(pPathDataSet)
	}
	else
		return false;
*/
	return true;
}

bool MPathSet::WriteToDB()
{    /*
	PATHDESC pathdata;
	MPathData* pPath = NULL;
	ResTable *ptable = g_pdb->GetTable("PathDesc");
	if(ptable)
	{
		for(size_t i = 0; i < m_PathSet.size(); ++i)
		{
			pPath = m_PathSet[i]->GetPathData();
			pathdata.iD = (unsigned int)pPath->pathID;
			pathdata.type = pPath->uiType;
			pathdata.mapIndex = 0;  ///< 前提是必须要填写一个mapid
			pathdata.num = pPath->data.size();
			for(size_t j = 0; j < pathdata.num; ++j)
			{
				pathdata.point[j].x = (int)pPath->data[j].x;
				pathdata.point[j].y = (int)pPath->data[j].y;
				pathdata.point[j].z = (int)pPath->data[j].z;
			}
			if( ptable->FindRecord( NULL , 1,&pathdata ) == 0 )	
			{
				if(ptable->InsertRecord( &pathdata ) < 0)
					return false;
			}
			else	
			{
				if(ptable->UpdateRecord( &pathdata ) < 0)
					return false;
			}
		}
	}
	else
		return false;
*/
	return true;
}

bool MPathSet::DeleteCurPathFromDB()
{/*
	PATHDESC pathdata;
	MPathData* pPath = NULL;
	ResTable *ptable = g_pdb->GetTable("PathDesc");
	if(ptable)
	{
		unsigned int id = m_PathSet[m_nCurSelectPathID]->GetPathData()->pathID;

		if(ptable->FindRecord( &pathdata , 1 , "ID" , id) < 0)
			return false;
		if(ptable->DeleteRecord(&pathdata) < 0)
			return false;
	}
	else
		return false;
*/
	return true;
}

bool   MPathSet::Read(const char* filename)
{
    MStream   mStream;
	if(!mStream.ReadFile  ( filename ))
	{
         return false;
	}

	Release();

    PathFileHead  _fileHead;
	mStream.Read ( _fileHead );
  
	PathHead _pathHead;
	
	for( int i = 0 ; i < _fileHead.pathNum  ; i++)
	{
		MPath  *pPath = new MPath;
		mStream.Read  ( _pathHead );
		pPath->SetName ( _pathHead.srcName , _pathHead.destName );
		m_PathSet.push_back ( pPath );

		m_PathSet[i]->GetPathData ()->data .resize ( _pathHead.posNum );
		for( int j = 0 ; j < _pathHead.posNum ; j++)
		{
			mStream.Read ( &m_PathSet[i]->GetPathData ()->data [j],sizeof(MVec3) );
		}

		m_PathSet[i]->UpdataData ();
	}
  
	return true;
};
		
bool   MPathSet::Write(const char* filename)
{
    MStream  mStream;
	mStream.WriteFile ( filename );

	PathFileHead  _fileHead;
	_fileHead.version = 0x01;
	_fileHead.pathNum = m_PathSet.size ();
	mStream.Write ( _fileHead );
  
	PathHead _pathHead;
	for(unsigned int i = 0 ; i < m_PathSet.size () ; i++)
	{
        strcpy( _pathHead.srcName , m_PathSet[i]->GetPathData ()->srcName .c_str ());
		strcpy( _pathHead.destName , m_PathSet[i]->GetPathData ()->destName .c_str ());
		_pathHead.posNum = m_PathSet[i]->GetPosNum();
		mStream.Write ( _pathHead );
		for( int j = 0 ; j < _pathHead.posNum ; j++)
		{
			mStream.Write ( &m_PathSet[i]->GetPathData ()->data [j],sizeof(MVec3) );
		}
	}
	mStream.WriteEnd ();

	return true;
};