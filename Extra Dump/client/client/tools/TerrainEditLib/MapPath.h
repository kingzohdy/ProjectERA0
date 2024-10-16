
#pragma once

#include <string>
#include <vector>
#include "TerrainEditTypes.h"



namespace TE
{

	class TerrModel;

	//��ʱ·����������Ϊ32
#define  MAX_PAHT_POS_NUM    33*4
	//·������
	struct MPathData
	{
		bool                  bDraw;
		int                   pathID;
		unsigned int          uiType;   ///< Ŀǰ���������ͣ�1�����Ը߶�ֵ��1��ʶ��2��ʹ�ø߶�ֵ��2��ʶ
		std::string   srcName,destName;
		std::vector <Ogre::Vector3>   data;
		std::vector<TerrModel*>  pModel;
		float                 r,g,b,a;
		unsigned int          color;
		char                  strDescription[128];
		MPathData(){bDraw=0;pathID=0;uiType=0;r=0;g=0;b=0;a=0;color=0;strDescription[0]=0;	};
	};
	//·����Ⱦ����
	struct MPathRenderData
	{
		std::vector<Ogre::Vector3>vdata;
	};
	//·�������ʽ
	struct PahtVec_st
	{
		Ogre::Vector3  pos;
		unsigned int  color;
	};

	//·���ļ�ͷ
	struct PathFileHead
	{
		unsigned char   version;
		unsigned char   pathNum;
	};
	//·���ṹ
	struct PathHead
	{
		char   srcName[64];
		char   destName[64];
		unsigned char   posNum;
	};


	class TerrainEditMgr ;
	//·��
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
		//����ģ�͵�λ�ø�·��
		void   UpdateModelPosToPath();
	private:
		bool   LoadHLSLData();

	private:

		MPathData            m_PathData;
		MPathRenderData      m_PathRenderData;
		int                  m_nSelectIndex;

	};
	//·������
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
		bool   DeleteCurPathFromDB();   ///< �����ݿ���ɾ����ǰѡ��path
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