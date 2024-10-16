
#include "EnvRegion.h"
#include "EditorStream.h"
#include "TerrainEdit.h"
#include "OgreRay.h"
#include "proto_comm.h"
#include "resdb_meta.h"
#include "TerrainTile.h"
#include "GameDBData.h"
#include "Terrain.h"

#include <Windows.h>

using namespace Ogre;

namespace TE
{
	//全局管理区域对象
	MapAreaManager   g_AreaSet;

#define AREA_FILE_VER 108
#define PROC_AREA_XGRID	6400
#define PROC_AREA_YGRID	6400

	MapArea::MapArea()
	{
		m_pPath = new MapPath;
		m_pDrawBox = new WireframeBox ;
		m_pDrawBox->Init( Ogre::Vector3(0,0,0) , Ogre::Vector3(100,100,100) );
		m_pDrawBox->SetColor( 0xffffff00 );

		memset(&m_AmbientInfo , 0 , sizeof(m_AmbientInfo));
		m_AmbientInfo.dirlight_angle = Vector2(0, 60.0f);
		m_AmbientInfo.dirlight_color = ColourValue(0.8f, 0.8f, 0.8f);
		m_AmbientInfo.specular_color = ColourValue(1.0f, 1.0f, 1.0f);
		m_AmbientInfo.ambient_color = ColourValue(0.2f, 0.2f, 0.2f);
		m_AmbientInfo.specular_power = 10.0f;
		m_AmbientInfo.distfog_color = ColourValue(1.0f, 1.0f, 0.5f);
		m_AmbientInfo.heightfog_color = ColourValue(0.5f, 0.5f, 1.0f);
		m_AmbientInfo.distfog_near = 0;
		m_AmbientInfo.distfog_far = 20000.0f;
		m_AmbientInfo.heightfog_near = 0;
		m_AmbientInfo.heightfog_far = 5000.0f;
		m_AmbientInfo.toplight_shadowbits = 0xffffffff;
		m_AmbientInfo.musicvol = 1.0f;
		m_AmbientInfo.bgsoundvol = 1.0f;

		memset(&m_ProcInfo, 0, sizeof(m_ProcInfo));

		m_nType = 0;
	}

	MapArea::~MapArea(void)
	{
		OGRE_DELETE( m_pPath );
		OGRE_DELETE( m_pDrawBox );
	}

	//设置区域对应的环境信息文件
	void   MapArea::SetAmbientInfo( AmbientInfo&  info )
	{
		memcpy( &m_AmbientInfo , &info , sizeof( AmbientInfo ) ) ;
	}

	void MapArea::setProcInfo(const AreaProcInfo &info)
	{
		memcpy(&m_ProcInfo, &info, sizeof(AreaProcInfo));
	}

	void   MapArea::UpdateAmbientInfo() 
	{
		g_TerrainEdit.m_bUpdateAmbientSettingDlg = true;
	}
	//更新当前位置
	void   MapArea::UpdataCenterPos()
	{ 
		if( m_pPath )
		{   
			if( m_pPath->GetPathData ()->data.empty ())
				return;

			Ogre::Vector3   countPos(0,0,0);    
			Ogre::Vector3   minPos(9999999,9999999,9999999) ,maxPos(-9999999,-9999999,-9999999);

			Ogre::Vector3   savePos;
			int size = m_pPath->GetPathData ()->data .size () ;

			for( int i = 0 ; i < size ; i++)
			{
				savePos = m_pPath->GetPathData ()->data[i] ;

				if( minPos.x > savePos.x ) minPos.x = savePos.x ;
				if( minPos.y > savePos.y ) minPos.y = savePos.y ;
				if( minPos.z > savePos.z ) minPos.z = savePos.z ;

				if( maxPos.x < savePos.x ) maxPos.x = savePos.x ;
				if( maxPos.y < savePos.y ) maxPos.y = savePos.y ;
				if( maxPos.z < savePos.z ) maxPos.z = savePos.z ;
			}

			m_Box.setRange( minPos,maxPos );
			if( m_pDrawBox )
			{
				Ogre::Vector3 center,extent;
				center = m_Box.getCenter();
				extent = m_Box.getExtension() ;
				m_pDrawBox->SetRange( center , extent );
			}

		}
	};

	MapAreaManager::MapAreaManager()
	{
	};

	MapAreaManager::~MapAreaManager()
	{
	}; 

	void MapAreaManager::init()
	{
		m_bShowArea = true;

		/// 生成一个缺省的全局区域
		MapArea* pArea = new MapArea;
		pArea->SetType(0);
		pArea->SetID(0);
		pArea->SetName("全局区域");
		pArea->SetLuaName("");
		addArea(pArea);
		m_nCurSelectAreaID = 0; /// 缺省选中全局区域
	}

	void MapAreaManager::release()
	{
		for(size_t i=0; i<m_AreaSet.size(); i++)
		{
			delete m_AreaSet[i];
		}
		m_AreaSet.clear();
		m_nCurSelectAreaID = 0;
	}

	void MapAreaManager::saveProcAreaData(int mapid)
	{
		if(g_TerrainEdit.GetEditData() == NULL) return;

		std::vector<PROCEVENT>procs;

		int mapsize = int(g_TerrainEdit.GetEditData()->gw*TERRBLOCK_SIZE);
		int narea_width = (mapsize+PROC_AREA_XGRID-1)/PROC_AREA_XGRID;

		for(size_t i=0; i<m_AreaSet.size(); i++)
		{
			MapArea *parea = m_AreaSet[i];
			if(parea->GetType()!=1 || parea->GetPath()->GetPosNum()<3)
			{
				continue;
			}

			PROCEVENT proc;
			proc.MapID = mapid;
			proc.ProcID = parea->GetID();
			proc.LinkMapID = parea->getProcInfo().link_mapid;
			proc.LinkMapDir = parea->getProcInfo().link_mapdir;
			proc.CameraDir = parea->getProcInfo().link_mapcamdir;
			proc.LinkMapPos.X = parea->getProcInfo().link_mappos.x;
			proc.LinkMapPos.Y = parea->getProcInfo().link_mappos.y;
			proc.FailMapDir = parea->getProcInfo().fail_mapdir;
			proc.FailMapPos.X = parea->getProcInfo().fail_mappos.x;
			proc.FailMapPos.Y = parea->getProcInfo().fail_mappos.y;
			strncpy(proc.Script, parea->getProcInfo().script, sizeof(proc.Script));
			proc.Script[sizeof(proc.Script)-1] = 0;

			proc.ProcPoly.Count = 0;
			MapPath *ppath = parea->GetPath();
			proc.ProcPoly.Count = ppath->GetPosNum();
			if(proc.ProcPoly.Count > MAX_POLY_POINT)
			{
				char buffer[256];
				sprintf(buffer, "区域%s的顶点数目超过16个，会发生错误", parea->GetName().c_str());
				MessageBox(NULL, buffer, "Error", MB_OK);
				proc.ProcPoly.Count = MAX_POLY_POINT;
			}

			int minx, miny, maxx, maxy;
			for(int j=0; j<proc.ProcPoly.Count; j++)
			{
				Ogre::Vector3 pos = ppath->GetPathData()->data[j];
				int x = int(pos.x);
				int y = int(pos.z);
				proc.ProcPoly.PointList[j].X = x;
				proc.ProcPoly.PointList[j].Y = y;

				if(j == 0)
				{
					minx = maxx = x;
					miny = maxy = y;
				}
				else
				{
					if(minx > x) minx = x;
					if(maxx < x) maxx = x;
					if(miny > y) miny = y;
					if(maxy < y) maxy = y;
				}
			}
			proc.ProcPoly.MinX = minx;
			proc.ProcPoly.MaxX = maxx;
			proc.ProcPoly.MinY = miny;
			proc.ProcPoly.MaxY = maxy;

			proc.AreaNum = 0;
			for(int y=(miny/PROC_AREA_YGRID); y<=(maxy/PROC_AREA_YGRID); y++)
			{
				if(y < 0) y = 0;
				if(y >= narea_width) y = narea_width-1;

				for(int x=(minx/PROC_AREA_XGRID); x<=(maxx/PROC_AREA_XGRID); x++)
				{
					if(x < 0) x = 0;
					if(x >= narea_width) x = narea_width-1;

					if(proc.AreaNum >= MAX_PROC_AREA)
					{
						char buffer[256];
						sprintf(buffer, "逻辑区域覆盖的服务器区域数目超过%d个，会发生错误", MAX_PROC_AREA);
						MessageBox(NULL, buffer, "Error", MB_OK);
						break;
					}
					proc.AreaIndexList[proc.AreaNum++] = y*narea_width + x;
				}
			}

			procs.push_back(proc);
		}

		if(!DBDataManager::getSingleton().saveMapProcEvent(mapid, procs))
		{
			MessageBox(NULL, "保存到数据库错误， 可能是此用户名不能修改数据库", "错误", MB_OK);
		}
	}

	void MapAreaManager::loadProcAreaData (int mapid)
	{
		if(g_TerrainEdit.GetEditData() == NULL) return;

		std::vector<PROCEVENT>procs;

		DBDataManager::getSingleton().loadMapProcEvent(mapid, procs);

		// 清除所有现有逻辑区域
		std::vector<MapArea*>::iterator it = m_AreaSet.begin();
		for (; it!=m_AreaSet.end();)
		{
			if ((*it)->GetType() == 1)
				it = m_AreaSet.erase(it);
			else
				it++;
		}

		for (int i=0; i<(int)procs.size(); i++)
		{
			PROCEVENT &ent = procs[i];

			MapArea *pArea = new MapArea();
			pArea->SetType(1);
			pArea->SetID(ent.ProcID);
	//		pArea->SetName(ent.)
			pArea->getProcInfo().link_mapid = ent.LinkMapID;
			pArea->getProcInfo().link_mapdir = ent.LinkMapDir;
			pArea->getProcInfo().link_mapcamdir = ent.CameraDir;
			pArea->getProcInfo().link_mappos.x = ent.LinkMapPos.X;
			pArea->getProcInfo().link_mappos.y = ent.LinkMapPos.Y;
			pArea->getProcInfo().fail_mapdir = ent.FailMapDir;
			pArea->getProcInfo().fail_mappos.x = ent.FailMapPos.X;
			pArea->getProcInfo().fail_mappos.y = ent.FailMapPos.Y;
			strncpy(pArea->getProcInfo().script, ent.Script, sizeof(ent.Script));

			int pathPointNum = ent.ProcPoly.Count;
			for (int i=0; i<pathPointNum; i++)
			{
				float height = 0.0f;
				g_Terrain.GetHeightEx(ent.ProcPoly.PointList[i].X, ent.ProcPoly.PointList[i].Y, &height);
				Ogre::Vector3 pos(ent.ProcPoly.PointList[i].X, height, ent.ProcPoly.PointList[i].Y);
				pArea->GetPath()->AddPos(pos);
			}	

			pArea->UpdataCenterPos();

			m_AreaSet.push_back(pArea);
		}
	}

	int MapAreaManager::newAreaID()
	{
		int id = 0;
		for(size_t i=0; i<m_AreaSet.size(); i++)
		{
			int tmpid = m_AreaSet[i]->GetID();
			if(tmpid > id) id = tmpid;
		}

		return id+1;
	}

	void MapAreaManager::addArea(MapArea*  pArea)
	{
		m_AreaSet.push_back( pArea );
		m_nCurSelectAreaID = int(m_AreaSet.size())-1;
	}

	//对当前区域增加顶点;参数( 位置 )
	void    MapAreaManager::AddCurAreaPos( Ogre::Vector3 pos )
	{
		assert(!m_AreaSet.empty());
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size ());

		m_AreaSet[ m_nCurSelectAreaID]->GetPath()->DelCurPos ();
		m_AreaSet[ m_nCurSelectAreaID]->GetPath() ->AddPos ( pos );
		m_AreaSet[ m_nCurSelectAreaID]->GetPath() ->AddPos ( 	m_AreaSet[m_nCurSelectAreaID]->GetPath()->GetPathData ()->data [0] ) ;

		m_AreaSet[ m_nCurSelectAreaID]->UpdataCenterPos ();
	};

	//删除区域当前顶点		
	void    MapAreaManager::DelAreaCurPos()
	{
		assert(!m_AreaSet.empty());
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size ());

		if( m_AreaSet[m_nCurSelectAreaID]->GetPath()->GetPathData ()->data .empty () )
			return ;

		int size = m_AreaSet[ m_nCurSelectAreaID]->GetPath()->GetPathData ()->data .size () ;

		m_AreaSet[m_nCurSelectAreaID]->GetPath()->DelCurPos ();
		m_AreaSet[ m_nCurSelectAreaID]->GetPath()->DelCurPos ();
		if( size > 2 )
			m_AreaSet[ m_nCurSelectAreaID]->GetPath()->AddPos ( m_AreaSet[m_nCurSelectAreaID]->GetPath()->GetPathData ()->data [0] ) ;
		m_AreaSet[ m_nCurSelectAreaID]->UpdataCenterPos ();

	};

	//设置当前选择区域ID;参数( 索引ID )	
	void    MapAreaManager::SetCurSelectID( int idx )
	{
		assert(idx>=0 && idx<int(m_AreaSet.size()));

		m_nCurSelectAreaID = idx;
	};

	//选择区域;参数 ( 射线方向, 起点 )  ;返回(为真表示成功)
	bool  MapAreaManager::SelectArea( Ogre::Vector3 &dir , Ogre::Vector3 &pos)
	{
		Ogre::Ray  ray;
		ray.m_Dir = dir;
		ray.m_Origin = pos ;

		ClearColor( 0xffffff00 );

		for( size_t i = 0 ; i < m_AreaSet.size() ; i ++ )
		{
			if( ray.intersectBox( &m_AreaSet[i]->GetAABBox() ,NULL ))
			{
				m_AreaSet[i]->SetColor( 0xff00ff00 );
				//如果被选中，则替换当前环境信息
				if( i != m_nCurSelectAreaID )
				{
					m_AreaSet[i]->UpdateAmbientInfo();
				}

				m_nCurSelectAreaID = i;

				return true;
			}
		}

		/// 如果没有选中任何区域，则选中全局
		m_AreaSet[0]->UpdateAmbientInfo();
		m_nCurSelectAreaID = 0;

		return false;
	};

	//设置清楚颜色;参数( 颜色值 )
	void    MapAreaManager::ClearColor( unsigned int color )
	{
		for( size_t i = 0 ; i < m_AreaSet.size () ; i++)
		{
			m_AreaSet[i]->SetColor ( color );
		}  
	};
	//渲染
	void    MapAreaManager::Render()
	{
		if( m_bShowArea )
		{
			for( size_t i = 1 ; i < m_AreaSet.size () ; i++)
			{
				m_AreaSet[i]->GetPath()->Render();
				m_AreaSet[i]->GetObjectBox()->Render();
			}
		}
	};
	//更新数据	
	void    MapAreaManager::UpdateData()
	{

	};
	//移动当前位置;参数( 位置 ) ;返回(为真表示成功)
	bool   MapAreaManager::MoveCurPos(Ogre::Vector3 &pos)
	{
		assert(!m_AreaSet.empty());
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size ());

		int index = m_AreaSet[m_nCurSelectAreaID ]->GetPath()->GetPathData ()->data .size ();
		index = index - 1 ;

		if( index < 0 )
		{
			m_AreaSet[m_nCurSelectAreaID]->GetPath() ->MoveLastPos  ( pos );
		}
		else
		{
			m_AreaSet[m_nCurSelectAreaID]->GetPath() ->MovePos ( pos , index );
		}

		m_AreaSet[m_nCurSelectAreaID]->UpdataCenterPos();

		return true;
	}

	struct AmbientInfo_100
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		ColourValue specular_color;
		float specular_power;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		char skyfile[128];
		char musicfile[128];
	};

	struct AREA_INFO_100
	{
		unsigned int nID;
		char         name[32];
		char         scriptName[64];
		unsigned int nPosNum;

		Ogre::Vector3  minPos,maxPos;
		AmbientInfo_100 ambient;
	};

	struct AmbientInfo_101
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
	};

	struct AmbientInfo_103
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		float bloom_gray;
		float bloom_high;
		float bloom_blur;

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char effectfile[128];//特效文件
	};

	struct AmbientInfo_104
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		ColourValue toplight_color;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		float bloom_gray;
		float bloom_high;
		float bloom_blur;

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char effectfile[128];//特效文件
	};

	struct AmbientInfo_105
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		uint toplight_shadowbits;
		ColourValue toplight_color;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		float bloom_gray;
		float bloom_high;
		float bloom_blur;

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char effectfile[128];//特效文件
	};

	struct AmbientInfo_106
	{
		bool distfog_enable;
		bool heightfog_enable;
		bool specular_enable;

		ColourValue ambient_color;
		ColourValue dirlight_color;
		Vector2 dirlight_angle;  //dir.x=Cos(y)Cos(x), dir.y=-Sin(y), dir.z=Cos(y)Sin(x)
		bool  shaow;
		float shadowdensity;
		ColourValue specular_color;
		float specular_power;

		uint toplight_shadowbits;
		ColourValue toplight_color;

		ColourValue distfog_color;
		float distfog_near;
		float distfog_far;

		ColourValue heightfog_color;
		float heightfog_near;
		float heightfog_far;

		float bloom_gray;
		float bloom_high;
		float bloom_blur;

		float musicvol; //音乐大小

		char skyfile[128]; //天空文件
		char musicfile[128];//音乐文件
		char effectfile[128];//特效文件
	};

	struct AreaProcInfo_107
	{
		int link_mapid;
		int link_mapdir;
		Ogre::Point2D link_mappos;
		int fail_mapdir;
		Ogre::Point2D fail_mappos;
	};

	void ReadAreaInfo(AREA_INFO &areaInfo, EditorStream &stream, int version)
	{
		memset(&areaInfo, 0, sizeof(AREA_INFO));

		stream.Read(areaInfo.nID);
		if(version >= 103) stream.Read(areaInfo.type);
		else areaInfo.type = 0;
		stream.Read(areaInfo.name);
		stream.Read(areaInfo.scriptName);
		stream.Read(areaInfo.nPosNum);
		stream.Read(areaInfo.minPos);
		stream.Read(areaInfo.maxPos);

		if(areaInfo.type == 0) //环境区域
		{
			if(version == 100)
			{
				AmbientInfo_100 ambient;
				stream.Read(ambient);

				areaInfo.ambientInfo.distfog_enable = ambient.distfog_enable;
				areaInfo.ambientInfo.heightfog_enable = ambient.heightfog_enable;
				areaInfo.ambientInfo.specular_enable = ambient.specular_enable;
				areaInfo.ambientInfo.ambient_color = ambient.ambient_color;
				areaInfo.ambientInfo.dirlight_color = ambient.dirlight_color;
				areaInfo.ambientInfo.dirlight_angle = ambient.dirlight_angle;
				areaInfo.ambientInfo.shaow = ambient.shaow;
				areaInfo.ambientInfo.shadowdensity = 1.0f;
				areaInfo.ambientInfo.specular_color = ambient.specular_color;
				areaInfo.ambientInfo.specular_power = ambient.specular_power;
				areaInfo.ambientInfo.distfog_color = ambient.distfog_color;
				areaInfo.ambientInfo.distfog_near = ambient.distfog_near;
				areaInfo.ambientInfo.distfog_far = ambient.distfog_far;
				areaInfo.ambientInfo.heightfog_color = ambient.heightfog_color;
				areaInfo.ambientInfo.heightfog_near = ambient.heightfog_near;
				areaInfo.ambientInfo.heightfog_far = ambient.heightfog_far;

				areaInfo.ambientInfo.bloom_gray = 0.3f;
				areaInfo.ambientInfo.bloom_high = 1.0f;
				areaInfo.ambientInfo.bloom_blur = 1.0f;

				strcpy(areaInfo.ambientInfo.skyfile, ambient.skyfile);
				strcpy(areaInfo.ambientInfo.musicfile, ambient.musicfile);
				strcpy(areaInfo.ambientInfo.effectfile, "");
			}
			else if(version == 101)
			{
				AmbientInfo_101 ambient;
				stream.Read(ambient);

				areaInfo.ambientInfo.distfog_enable = ambient.distfog_enable;
				areaInfo.ambientInfo.heightfog_enable = ambient.heightfog_enable;
				areaInfo.ambientInfo.specular_enable = ambient.specular_enable;
				areaInfo.ambientInfo.ambient_color = ambient.ambient_color;
				areaInfo.ambientInfo.dirlight_color = ambient.dirlight_color;
				areaInfo.ambientInfo.dirlight_angle = ambient.dirlight_angle;
				areaInfo.ambientInfo.shaow = ambient.shaow;
				areaInfo.ambientInfo.shadowdensity = ambient.shadowdensity;
				areaInfo.ambientInfo.specular_color = ambient.specular_color;
				areaInfo.ambientInfo.specular_power = ambient.specular_power;
				areaInfo.ambientInfo.distfog_color = ambient.distfog_color;
				areaInfo.ambientInfo.distfog_near = ambient.distfog_near;
				areaInfo.ambientInfo.distfog_far = ambient.distfog_far;
				areaInfo.ambientInfo.heightfog_color = ambient.heightfog_color;
				areaInfo.ambientInfo.heightfog_near = ambient.heightfog_near;
				areaInfo.ambientInfo.heightfog_far = ambient.heightfog_far;

				areaInfo.ambientInfo.bloom_gray = 0.3f;
				areaInfo.ambientInfo.bloom_high = 1.0f;
				areaInfo.ambientInfo.bloom_blur = 1.0f;

				strcpy(areaInfo.ambientInfo.skyfile, ambient.skyfile);
				strcpy(areaInfo.ambientInfo.musicfile, ambient.musicfile);
				strcpy(areaInfo.ambientInfo.effectfile, "");
			}
			else if(version <= 103)
			{
				AmbientInfo_103 ambient;
				stream.Read(ambient);

				areaInfo.ambientInfo.distfog_enable = ambient.distfog_enable;
				areaInfo.ambientInfo.heightfog_enable = ambient.heightfog_enable;
				areaInfo.ambientInfo.specular_enable = ambient.specular_enable;
				areaInfo.ambientInfo.ambient_color = ambient.ambient_color;
				areaInfo.ambientInfo.dirlight_color = ambient.dirlight_color;
				areaInfo.ambientInfo.dirlight_angle = ambient.dirlight_angle;
				areaInfo.ambientInfo.shaow = ambient.shaow;
				areaInfo.ambientInfo.shadowdensity = ambient.shadowdensity;
				areaInfo.ambientInfo.specular_color = ambient.specular_color;
				areaInfo.ambientInfo.specular_power = ambient.specular_power;

				areaInfo.ambientInfo.toplight_color = ColourValue(0,0,0,1.0f);

				areaInfo.ambientInfo.distfog_color = ambient.distfog_color;
				areaInfo.ambientInfo.distfog_near = ambient.distfog_near;
				areaInfo.ambientInfo.distfog_far = ambient.distfog_far;
				areaInfo.ambientInfo.heightfog_color = ambient.heightfog_color;
				areaInfo.ambientInfo.heightfog_near = ambient.heightfog_near;
				areaInfo.ambientInfo.heightfog_far = ambient.heightfog_far;

				areaInfo.ambientInfo.bloom_gray = ambient.bloom_gray;
				areaInfo.ambientInfo.bloom_high = ambient.bloom_high;
				areaInfo.ambientInfo.bloom_blur = ambient.bloom_blur;

				strcpy(areaInfo.ambientInfo.skyfile, ambient.skyfile);
				strcpy(areaInfo.ambientInfo.musicfile, ambient.musicfile);
				strcpy(areaInfo.ambientInfo.effectfile, "");
			}
			else if(version <= 104)
			{
				AmbientInfo_104 ambient;
				stream.Read(ambient);

				areaInfo.ambientInfo.distfog_enable = ambient.distfog_enable;
				areaInfo.ambientInfo.heightfog_enable = ambient.heightfog_enable;
				areaInfo.ambientInfo.specular_enable = ambient.specular_enable;
				areaInfo.ambientInfo.ambient_color = ambient.ambient_color;
				areaInfo.ambientInfo.dirlight_color = ambient.dirlight_color;
				areaInfo.ambientInfo.dirlight_angle = ambient.dirlight_angle;
				areaInfo.ambientInfo.shaow = ambient.shaow;
				areaInfo.ambientInfo.shadowdensity = ambient.shadowdensity;
				areaInfo.ambientInfo.specular_color = ambient.specular_color;
				areaInfo.ambientInfo.specular_power = ambient.specular_power;

				areaInfo.ambientInfo.toplight_shadowbits = 0xffffffff;
				areaInfo.ambientInfo.toplight_color = ColourValue(0,0,0,1.0f);

				areaInfo.ambientInfo.distfog_color = ambient.distfog_color;
				areaInfo.ambientInfo.distfog_near = ambient.distfog_near;
				areaInfo.ambientInfo.distfog_far = ambient.distfog_far;
				areaInfo.ambientInfo.heightfog_color = ambient.heightfog_color;
				areaInfo.ambientInfo.heightfog_near = ambient.heightfog_near;
				areaInfo.ambientInfo.heightfog_far = ambient.heightfog_far;

				areaInfo.ambientInfo.bloom_gray = ambient.bloom_gray;
				areaInfo.ambientInfo.bloom_high = ambient.bloom_high;
				areaInfo.ambientInfo.bloom_blur = ambient.bloom_blur;

				strcpy(areaInfo.ambientInfo.skyfile, ambient.skyfile);
				strcpy(areaInfo.ambientInfo.musicfile, ambient.musicfile);
				strcpy(areaInfo.ambientInfo.effectfile, "");
			}
			else if(version <= 105)
			{
				AmbientInfo_105 ambient;
				stream.Read(ambient);

				areaInfo.ambientInfo.distfog_enable = ambient.distfog_enable;
				areaInfo.ambientInfo.heightfog_enable = ambient.heightfog_enable;
				areaInfo.ambientInfo.specular_enable = ambient.specular_enable;
				areaInfo.ambientInfo.ambient_color = ambient.ambient_color;
				areaInfo.ambientInfo.dirlight_color = ambient.dirlight_color;
				areaInfo.ambientInfo.dirlight_angle = ambient.dirlight_angle;
				areaInfo.ambientInfo.shaow = ambient.shaow;
				areaInfo.ambientInfo.shadowdensity = ambient.shadowdensity;
				areaInfo.ambientInfo.specular_color = ambient.specular_color;
				areaInfo.ambientInfo.specular_power = ambient.specular_power;

				areaInfo.ambientInfo.toplight_shadowbits = ambient.toplight_shadowbits;
				areaInfo.ambientInfo.toplight_color = ambient.toplight_color;

				areaInfo.ambientInfo.distfog_color = ambient.distfog_color;
				areaInfo.ambientInfo.distfog_near = ambient.distfog_near;
				areaInfo.ambientInfo.distfog_far = ambient.distfog_far;
				areaInfo.ambientInfo.heightfog_color = ambient.heightfog_color;
				areaInfo.ambientInfo.heightfog_near = ambient.heightfog_near;
				areaInfo.ambientInfo.heightfog_far = ambient.heightfog_far;

				areaInfo.ambientInfo.bloom_gray = ambient.bloom_gray;
				areaInfo.ambientInfo.bloom_high = ambient.bloom_high;
				areaInfo.ambientInfo.bloom_blur = ambient.bloom_blur;

				areaInfo.ambientInfo.musicvol = 1.0f;

				strcpy(areaInfo.ambientInfo.skyfile, ambient.skyfile);
				strcpy(areaInfo.ambientInfo.musicfile, ambient.musicfile);
				strcpy(areaInfo.ambientInfo.effectfile, ambient.effectfile);
			}
			else if(version <= 106)
			{
				AmbientInfo_106 ambient;
				stream.Read(ambient);

				areaInfo.ambientInfo.distfog_enable = ambient.distfog_enable;
				areaInfo.ambientInfo.heightfog_enable = ambient.heightfog_enable;
				areaInfo.ambientInfo.specular_enable = ambient.specular_enable;
				areaInfo.ambientInfo.ambient_color = ambient.ambient_color;
				areaInfo.ambientInfo.dirlight_color = ambient.dirlight_color;
				areaInfo.ambientInfo.dirlight_angle = ambient.dirlight_angle;
				areaInfo.ambientInfo.shaow = ambient.shaow;
				areaInfo.ambientInfo.shadowdensity = ambient.shadowdensity;
				areaInfo.ambientInfo.specular_color = ambient.specular_color;
				areaInfo.ambientInfo.specular_power = ambient.specular_power;

				areaInfo.ambientInfo.toplight_shadowbits = ambient.toplight_shadowbits;
				areaInfo.ambientInfo.toplight_color = ambient.toplight_color;

				areaInfo.ambientInfo.distfog_color = ambient.distfog_color;
				areaInfo.ambientInfo.distfog_near = ambient.distfog_near;
				areaInfo.ambientInfo.distfog_far = ambient.distfog_far;
				areaInfo.ambientInfo.heightfog_color = ambient.heightfog_color;
				areaInfo.ambientInfo.heightfog_near = ambient.heightfog_near;
				areaInfo.ambientInfo.heightfog_far = ambient.heightfog_far;

				areaInfo.ambientInfo.bloom_gray = ambient.bloom_gray;
				areaInfo.ambientInfo.bloom_high = ambient.bloom_high;
				areaInfo.ambientInfo.bloom_blur = ambient.bloom_blur;

				areaInfo.ambientInfo.musicvol = ambient.musicvol;
				areaInfo.ambientInfo.bgsoundvol = 1.0f;

				strcpy(areaInfo.ambientInfo.skyfile, ambient.skyfile);
				strcpy(areaInfo.ambientInfo.musicfile, ambient.musicfile);
				strcpy(areaInfo.ambientInfo.bgsoundfile, "");
				strcpy(areaInfo.ambientInfo.effectfile, ambient.effectfile);
			}
			else stream.Read (areaInfo.ambientInfo);
		}
		else
		{
			if(version < 108)
			{
				AreaProcInfo_107 procinfo;
				stream.Read(procinfo);

				areaInfo.procinfo.link_mapid = procinfo.link_mapid;
				areaInfo.procinfo.link_mapdir = procinfo.link_mapdir;
				areaInfo.procinfo.link_mapcamdir = 0;
				areaInfo.procinfo.link_mappos = procinfo.link_mappos;
				areaInfo.procinfo.fail_mapdir = procinfo.fail_mapdir;
				areaInfo.procinfo.fail_mappos = procinfo.fail_mappos;
				areaInfo.procinfo.script[0] = 0;
			}
			else stream.Read(areaInfo.procinfo);
		}

	}

	//引导区域文件; 参数 ( 文件名 ) ;返回 ( 为真表示成功 )
	bool    MapAreaManager::Load( const char*  filename )
	{
		assert( filename );

		EditorStream stream;

		if( !stream.ReadFile ( filename ) )
		{		
			return false;
		}

		release();

		AREA_HEAD areaHead;

		if(!stream.Read(areaHead))
			return false;

		AREA_INFO  areaInfo;
		for(unsigned int i=0; i<areaHead.numArea; i++)
		{
			MapArea*  pArea = new MapArea;

			ReadAreaInfo(areaInfo, stream, areaHead.version);

			pArea->SetType(areaInfo.type);
			pArea->SetID(areaInfo.nID );
			pArea->SetName(areaInfo.name );
			pArea->SetLuaName(areaInfo.scriptName);
			pArea->SetAmbientInfo( areaInfo.ambientInfo );
			pArea->setProcInfo(areaInfo.procinfo);

			Ogre::Vector3  pos;

			for( unsigned int j = 0 ; j < areaInfo.nPosNum; j++)
			{
				stream.ReadFloat ( &pos.x );
				stream.ReadFloat ( &pos.y );
				stream.ReadFloat ( &pos.z );

				pArea->GetPath() ->AddPos ( pos );
			}

			if( areaInfo.nPosNum > 0 )
				pArea->GetPath()->AddPos( pArea->GetPath() ->GetPathData ()->data [0] );
			pArea->UpdataCenterPos ();

			m_AreaSet.push_back ( pArea );
		}

		m_nCurSelectAreaID = 0;

		return true ;
	};

	//保存区域信息;参数( 文件名 ) ;返回( 为真表示成功 ) 
	bool    MapAreaManager::Save( const char*  filename )
	{
		assert( filename );

		EditorStream  stream;

		if( !stream.WriteFile ( filename ) )
		{
			return false;
		}

		AREA_HEAD   areaHead;
		memcpy(areaHead.magic_number, "AREA", 4);
		areaHead.version = AREA_FILE_VER;
		areaHead.numArea = m_AreaSet.size ();

		if( !stream.Write ( areaHead ) )
			return false;

		AREA_INFO  areaInfo;
		for(unsigned int i = 0 ; i < m_AreaSet.size () ; i++)
		{
			MapArea *parea = m_AreaSet[i];
			areaInfo.nID = parea->GetID() ;
			areaInfo.type = parea->GetType();

			strcpy( areaInfo.name , parea->GetName ().c_str () );
			strcpy( areaInfo.scriptName , parea->GetLua().c_str () );
			if(0 < i)
			{
				int npos = parea->GetPath()->GetPosNum();
				if(npos == 0) areaInfo.nPosNum = 0;
				else areaInfo.nPosNum = npos - 1;
			}
			else areaInfo.nPosNum = 0;

			areaInfo.minPos = parea->GetAABBox().getMinPos();
			areaInfo.maxPos = parea->GetAABBox().getMaxPos();
			memcpy( &areaInfo.ambientInfo, parea->GetAmbientInfo(), sizeof(AmbientInfo ) );
			memcpy(&areaInfo.procinfo, &parea->getProcInfo(), sizeof(AreaProcInfo));

			stream.Write(areaInfo.nID);
			stream.Write(areaInfo.type);
			stream.Write(areaInfo.name);
			stream.Write(areaInfo.scriptName);
			stream.Write(areaInfo.nPosNum);
			stream.Write(areaInfo.minPos);
			stream.Write(areaInfo.maxPos);

			if(areaInfo.type == 0) stream.Write(areaInfo.ambientInfo);
			else if(areaInfo.type == 1) stream.Write(areaInfo.procinfo);

			for(unsigned int j = 0 ; j<areaInfo.nPosNum ; j++)
			{
				stream.WriteFloat ( parea->GetPath() ->GetPathData ()->data [j] , 3 );
			}
		}

		stream.WriteEnd ();

		return true;
	};
	//得到当前选择的区域	
	MapArea*   MapAreaManager::GetCurSelectArea()
	{
		assert(!m_AreaSet.empty());
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size ());

		return m_AreaSet[ m_nCurSelectAreaID ] ;
	};

	//删除当前区域		
	void   MapAreaManager::DelCurArea()
	{
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size());
		if(m_nCurSelectAreaID == 0) return;

		delete m_AreaSet[m_nCurSelectAreaID];
		m_AreaSet.erase(m_AreaSet.begin()+m_nCurSelectAreaID);

		m_nCurSelectAreaID--;
	}

	void MapAreaManager::copyFromCurArea()
	{
		if(m_nCurSelectAreaID < 0)
		{
			MessageBox(NULL, "需要选中一个区域", "错误", MB_OK);
			return;
		}
		assert(!m_AreaSet.empty());
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size ());

		MapArea *pselarea = m_AreaSet[m_nCurSelectAreaID];

		delete m_pCopyArea;
		m_pCopyArea = new MapArea;
		m_pCopyArea->SetAmbientInfo(*pselarea->GetAmbientInfo());
	}

	void MapAreaManager::pasteToCurArea()
	{
		if(m_nCurSelectAreaID < 0)
		{
			MessageBox(NULL, "需要选中一个区域", "错误", MB_OK);
			return;
		}
		assert(!m_AreaSet.empty());
		assert(m_nCurSelectAreaID>=0 && m_nCurSelectAreaID<(int)m_AreaSet.size ());

		if(m_pCopyArea == NULL)
		{
			MessageBox(NULL, "需要先拷贝一个区域", "错误", MB_OK);
			return;
		}

		MapArea *pselarea = m_AreaSet[m_nCurSelectAreaID];
		pselarea->SetAmbientInfo(*(m_pCopyArea->GetAmbientInfo()));

		delete m_pCopyArea;
		m_pCopyArea = NULL;
	}
}
