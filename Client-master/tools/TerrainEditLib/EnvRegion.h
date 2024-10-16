
#pragma once

#include "MapPath.h"
#include "WireframeBox.h"

#include "OgreAmbientRegion.h"


namespace TE
{
	struct AreaProcInfo
	{
		int link_mapid;
		int link_mapdir;
		int link_mapcamdir;
		Ogre::Point2D link_mappos;
		int fail_mapdir;
		Ogre::Point2D fail_mappos;
		char script[256];
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
		Ogre::BoxBound         GetAABBox(){ return m_Box ;};
		//���ID
		unsigned int        GetID(){ return m_nID ;};
		//�������
		unsigned int        GetType() { return m_nType ; };
		//���·��
		MapPath*              GetPath(){ return m_pPath ; };
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
		WireframeBox*         GetObjectBox( ){ return m_pDrawBox ; };
		//���µ�ǰ������Ϣ
		void                UpdateAmbientInfo();

		AreaProcInfo &getProcInfo(){ return m_ProcInfo;}
	private:	
		//��Χ��
		Ogre::BoxBound          m_Box;
		//���ư�Χ��
		WireframeBox           *m_pDrawBox;
		//·��
		MapPath          *m_pPath;
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

		Ogre::Vector3  minPos,maxPos;
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
		void loadProcAreaData (int mapid);

		int newAreaID();
		void    addArea(MapArea*  pArea);

		void    Render();
		void    UpdateData();

		void    AddCurAreaPos( Ogre::Vector3 pos );
		bool    MoveCurPos(Ogre::Vector3 &pos);

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

		bool  SelectArea( Ogre::Vector3 &dir , Ogre::Vector3 &pos);

		unsigned int     GetAreaSize(){ return (unsigned int)m_AreaSet.size() ; };

		MapArea*           GetArea(int idx){ return m_AreaSet[idx];};

		MapArea*           GetCurSelectArea() ; 

		void             SetShowArea( bool bShow ){ m_bShowArea = bShow ;};

		void copyFromCurArea();
		void pasteToCurArea();

	private:
		//�Ƿ���ʾ�����߿�
		bool                     m_bShowArea;
		//��ǰѡ������ID
		int                       m_nCurSelectAreaID;
		//��������
		std::vector <MapArea*>      m_AreaSet;

		MapArea *m_pCopyArea;
	};

	extern MapAreaManager   g_AreaSet;
}

