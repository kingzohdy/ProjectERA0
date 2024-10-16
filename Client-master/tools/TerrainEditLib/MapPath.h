
#pragma once

#include <string>
#include <vector>
#include "TerrainEditTypes.h"



namespace TE
{

	class TerrModel;

	//暂时路径保存最多点为32
#define  MAX_PAHT_POS_NUM    33*4
	//路径数据
	struct MPathData
	{
		bool                  bDraw;
		int                   pathID;
		unsigned int          uiType;   ///< 目前有两种类型：1）忽略高度值，1标识；2）使用高度值，2标识
		std::string   srcName,destName;
		std::vector <Ogre::Vector3>   data;
		std::vector<TerrModel*>  pModel;
		float                 r,g,b,a;
		unsigned int          color;
		char                  strDescription[128];
		MPathData(){bDraw=0;pathID=0;uiType=0;r=0;g=0;b=0;a=0;color=0;strDescription[0]=0;	};
	};
	//路径渲染数据
	struct MPathRenderData
	{
		std::vector<Ogre::Vector3>vdata;
	};
	//路径顶点格式
	struct PahtVec_st
	{
		Ogre::Vector3  pos;
		unsigned int  color;
	};

	//路径文件头
	struct PathFileHead
	{
		unsigned char   version;
		unsigned char   pathNum;
	};
	//路径结构
	struct PathHead
	{
		char   srcName[64];
		char   destName[64];
		unsigned char   posNum;
	};


	class TerrainEditMgr ;
	//路径
	class MapPath
	{
	public:
		MapPath(void);
		~MapPath(void);

		void   Render();
		void   AddPos( Ogre::Vector3 pos );

		bool   MovePos( Ogre::Vector3 pos , int index);
		void   MovePickedPos(const Ogre::Vector3& pos);

		void   SetName( const char* srcName,const char* destName);
		bool   Find(std::string &srcName,std::string &destName);

		bool   CreateRenderData();	
		bool   MoveLastPos(Ogre::Vector3 pos);
		bool   DelCurPos();
		bool   SetColor(unsigned int color);
		bool  SetColor(int index,unsigned int color);

		MPathData*  GetPathData(){return &m_PathData;};
		MPathRenderData*  GetPathRenderData(){return &m_PathRenderData;}

		int         GetPosNum(){return (int)m_PathData.data .size ();}

		bool   UpdataData();
		void   Release();

		bool   TestPathModel( TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos );
		//更新模型的位置给路径
		void   UpdateModelPosToPath();
	private:
		bool   LoadHLSLData();

	private:

		MPathData            m_PathData;
		MPathRenderData      m_PathRenderData;
		int                  m_nSelectIndex;

	};
	//路径集合
	class MapPathGroup
	{
	public:
		MapPathGroup(); 
		~MapPathGroup();

		int     Add(const char* srcName , const char* destName);
		void    Add(int id , unsigned int type , char* description);
		MapPath*   GetPath(const char* srcName,const char *destName);
		void   SetCurPath(int index) { m_nCurSelectPathID = index; }
		bool   AddCurPathPos( Ogre::Vector3 &pos );
		void   Modify(int id , unsigned int type , char* description);
		void   Render();

		bool   Read(const char* filename);
		bool   ReadFromDB();
		bool   Write(const char* filename);
		bool   WriteToDB();
		bool   DeleteCurPathFromDB();   ///< 从数据库中删除当前选中path
		bool   MoveCurPos(Ogre::Vector3 &pos);
		bool   TestPathModel( TerrainEditMgr* pEditor , Ogre::Vector3 &dir , Ogre::Vector3 &pos );

		int   DelCurPath();
		bool   DelCurPathPos();

		void   Release();
		void   UpdateSelectPathPos();

		MapPath* GetPath(size_t index);
		size_t GetPathSetNum() { return m_PathSet.size(); }
	private:
		std::vector <MapPath*>              m_PathSet;
		int                                 m_nCurSelectPathID;

	};

};