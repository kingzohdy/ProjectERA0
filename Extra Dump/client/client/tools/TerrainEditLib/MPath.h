
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
	std::vector <MVec3>   data;
	std::vector<TerrModel*>  pModel;
	float                 r,g,b,a;
	unsigned int          color;
	char                  strDescription[128];
};
//·����Ⱦ����
struct MPathRenderData
{
	std::vector<MVec3>vdata;
};
//·�������ʽ
struct PahtVec_st
{
	MVec3  pos;
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


class MTerrainEdit ;
//·��
class MPath
{
public:
	MPath(void);
	~MPath(void);

	void   Render();
	void   AddPos( MVec3 pos );

	bool   MovePos( MVec3 pos , int index);
	void   MovePickedPos(const MVec3& pos);

	void   SetName( const char* srcName,const char* destName);
	bool   Find(std::string &srcName,std::string &destName);

	bool   CreateRenderData();	
	bool   MoveLastPos(MVec3 pos);
	bool   DelCurPos();
	bool   SetColor(unsigned int color);
	bool  SetColor(int index,unsigned int color);

	MPathData*  GetPathData(){return &m_PathData;};
	MPathRenderData*  GetPathRenderData(){return &m_PathRenderData;}

	int         GetPosNum(){return (int)m_PathData.data .size ();}

	bool   UpdataData();
	void   Release();

	bool   TestPathModel( MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos );
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
class MPathSet
{
public:
	MPathSet(); 
	~MPathSet();

	int     Add(const char* srcName , const char* destName);
	void    Add(int id , unsigned int type , char* description);
	MPath*   GetPath(const char* srcName,const char *destName);
	void   SetCurPath(int index) { m_nCurSelectPathID = index; }
	bool   AddCurPathPos( MVec3 &pos );
	void   Modify(int id , unsigned int type , char* description);
	void   Render();

	bool   Read(const char* filename);
	bool   ReadFromDB();
	bool   Write(const char* filename);
	bool   WriteToDB();
	bool   DeleteCurPathFromDB();   ///< �����ݿ���ɾ����ǰѡ��path
	bool   MoveCurPos(MVec3 &pos);
	bool   TestPathModel( MTerrainEdit* pEditor , MVec3 &dir , MVec3 &pos );

	int   DelCurPath();
	bool   DelCurPathPos();

	void   Release();
	void   UpdateSelectPathPos();

	MPath* GetPath(size_t index);
	size_t GetPathSetNum() { return m_PathSet.size(); }
private:
	std::vector <MPath*>              m_PathSet;
	int                                 m_nCurSelectPathID;

};

};