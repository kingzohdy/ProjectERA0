
#pragma once

#include "mpath.h"
#include "mboxobject.h"

#include "OgreAmbientRegion.h"


namespace TE
{
	struct AreaProcInfo
	{
		int link_mapid;
		int link_mapdir;
		Ogre::Point2D link_mappos;
		int fail_mapdir;
		Ogre::Point2D fail_mappos;
	};

	class MapArea
	{
	public:
		MapArea();
		~MapArea();
		//设置区域ID;参数( ID )
		void  SetID(unsigned int id){ m_nID = id;};
		//设置区域名称;参数（名字）
		void  SetName(const char* name){ m_strName = name;}
		//设置对应的LUA脚本名;参数( 脚本文件名)
		void  SetLuaName(const char* name){ m_strLua = name ;}
		//设置区域类型0:环境区域， 1：逻辑区域
		void  SetType(unsigned int type){ m_nType = type; }
		//设置区域颜色
		void   SetColor(unsigned int color){ if(m_pPath)m_pPath->SetColor (color) ;}
		//更新中心位置
		void   UpdataCenterPos();
		//获得区域BOX
		MAABBox         GetAABBox(){ return m_Box ;};
		//获得ID
		unsigned int        GetID(){ return m_nID ;};
		//获得类型
		unsigned int        GetType() { return m_nType ; };
		//获得路径
		MPath*              GetPath(){ return m_pPath ; };
		//获得名称
		std::string         GetName(){return m_strName; };
		//获得LUA脚本
		std::string         GetLua(){ return m_strLua ;};   
		//设置环境信息
		void                SetAmbientInfo( Ogre::AmbientInfo& info );
		void setProcInfo(const AreaProcInfo &info);
		//得到环境信息
		Ogre::AmbientInfo*         GetAmbientInfo(){ return  &m_AmbientInfo;};
		//得到包围盒
		MBoxObject*         GetObjectBox( ){ return m_pDrawBox ; };
		//更新当前环境信息
		void                UpdateAmbientInfo();

		AreaProcInfo &getProcInfo(){ return m_ProcInfo;}
	private:	
		//包围盒
		MAABBox          m_Box;
		//绘制包围盒
		MBoxObject           *m_pDrawBox;
		//路径
		MPath          *m_pPath;
		//ID
		unsigned int   m_nID;
		//类型
		unsigned int   m_nType;
		//名称
		std::string    m_strName;
		//LUA脚本
		std::string    m_strLua;
		//每个区域对应的环境信息
		Ogre::AmbientInfo     m_AmbientInfo; 

		//逻辑信息
		AreaProcInfo m_ProcInfo;
	};

	//区域文件头
	struct AREA_HEAD
	{
		char magic_number[4];
		unsigned int  version;
		unsigned int  numArea;
	};

	//区域信息
	struct AREA_INFO
	{
		unsigned int nID;
		int type;
		char         name[32];
		char         scriptName[64];
		unsigned int nPosNum;

		MVec3  minPos,maxPos;
		Ogre::AmbientInfo   ambientInfo;
		AreaProcInfo procinfo;
	};

	//区域集合
	class  MapAreaManager
	{
	public:
		MapAreaManager();
		~MapAreaManager();

		void init();
		void release();

		void saveProcAreaData(int mapid);

		int newAreaID();
		void    addArea(MapArea*  pArea);

		void    Render();
		void    UpdateData();

		void    AddCurAreaPos( MVec3 pos );
		bool    MoveCurPos(MVec3 &pos);

		bool Load(const char*  filename);
		bool Save(const char*  filename);

		void    ClearColor( unsigned int color );

		void    DelAreaCurPos();
		void    SetCurSelectID( int idx );
		int     GetCurSelectID()
		{
			return m_nCurSelectAreaID;
		}

		void   DelCurArea();

		bool  SelectArea( MVec3 &dir , MVec3 &pos);

		unsigned int     GetAreaSize(){ return (unsigned int)m_AreaSet.size() ; };

		MapArea*           GetArea(int idx){ return m_AreaSet[idx];};

		MapArea*           GetCurSelectArea() ; 

		void             SetShowArea( bool bShow ){ m_bShowArea = bShow ;};

	private:
		//是否显示区域线框
		bool                     m_bShowArea;
		//当前选择区域ID
		int                       m_nCurSelectAreaID;
		//区域数组
		std::vector <MapArea*>      m_AreaSet;
	};
};

extern TE::MapAreaManager   g_AreaSet;
