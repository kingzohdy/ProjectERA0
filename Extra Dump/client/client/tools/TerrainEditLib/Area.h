
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
		//��������ID;����( ID )
		void  SetID(unsigned int id){ m_nID = id;};
		//������������;���������֣�
		void  SetName(const char* name){ m_strName = name;}
		//���ö�Ӧ��LUA�ű���;����( �ű��ļ���)
		void  SetLuaName(const char* name){ m_strLua = name ;}
		//������������0:�������� 1���߼�����
		void  SetType(unsigned int type){ m_nType = type; }
		//����������ɫ
		void   SetColor(unsigned int color){ if(m_pPath)m_pPath->SetColor (color) ;}
		//��������λ��
		void   UpdataCenterPos();
		//�������BOX
		MAABBox         GetAABBox(){ return m_Box ;};
		//���ID
		unsigned int        GetID(){ return m_nID ;};
		//�������
		unsigned int        GetType() { return m_nType ; };
		//���·��
		MPath*              GetPath(){ return m_pPath ; };
		//�������
		std::string         GetName(){return m_strName; };
		//���LUA�ű�
		std::string         GetLua(){ return m_strLua ;};   
		//���û�����Ϣ
		void                SetAmbientInfo( Ogre::AmbientInfo& info );
		void setProcInfo(const AreaProcInfo &info);
		//�õ�������Ϣ
		Ogre::AmbientInfo*         GetAmbientInfo(){ return  &m_AmbientInfo;};
		//�õ���Χ��
		MBoxObject*         GetObjectBox( ){ return m_pDrawBox ; };
		//���µ�ǰ������Ϣ
		void                UpdateAmbientInfo();

		AreaProcInfo &getProcInfo(){ return m_ProcInfo;}
	private:	
		//��Χ��
		MAABBox          m_Box;
		//���ư�Χ��
		MBoxObject           *m_pDrawBox;
		//·��
		MPath          *m_pPath;
		//ID
		unsigned int   m_nID;
		//����
		unsigned int   m_nType;
		//����
		std::string    m_strName;
		//LUA�ű�
		std::string    m_strLua;
		//ÿ�������Ӧ�Ļ�����Ϣ
		Ogre::AmbientInfo     m_AmbientInfo; 

		//�߼���Ϣ
		AreaProcInfo m_ProcInfo;
	};

	//�����ļ�ͷ
	struct AREA_HEAD
	{
		char magic_number[4];
		unsigned int  version;
		unsigned int  numArea;
	};

	//������Ϣ
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

	//���򼯺�
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
		//�Ƿ���ʾ�����߿�
		bool                     m_bShowArea;
		//��ǰѡ������ID
		int                       m_nCurSelectAreaID;
		//��������
		std::vector <MapArea*>      m_AreaSet;
	};
};

extern TE::MapAreaManager   g_AreaSet;
