
#pragma once

#include "TerrainEditTypes.h"
#include <vector>

namespace TE
{

class CHeightMap
{

public:
	CHeightMap(void);
	~CHeightMap(void);

public:
	//修改高度模式
	enum  HEIGHT_PROC_EVEN
	{
		SOFT_MODE = 0,  
		HARD_MODE ,
		SMOOTH_MODE ,
		LEVEL_OFF,
		RAMP_MODE ,
		DITCH_MODE ,
		SMOOTH_ALL_MODE,
	};
    //修改高度范围
	enum HEIGHT_AREA
	{
		PAINT_CIRCLE = 0,		//圆形
		PAINT_QUAD,             //方形
	};
    //改变高度方式
	enum  HEIGHT_CHANGE_MODE
	{
		ALPHA_ADD = 0,        //增加
		ALPHA_DEC ,           //减少
	};
	//选中的高度，对应信息
	struct Select_Info
	{
		int  vecIndex;
		float  length;
	};;
    //创建高度图；参数（ 宽度，高度 ，开始位置，大小，基础高度 ，缩放） 
    void                    CreateMap(MPoint2D nblock, MPoint2D ngrid, MVec3 startPos , float fSize ,float fbaseHeight = 1.0f,float fScale = 1.0f);
	//摧毁
	void                    Destory();	
	//获得高度信息；参数（ X，Z ，返回高度 ） ；返回（为真表示成功）
	bool                    GetHeight(float x, float z,float*  height);
	//获得格子高度信息；参数（ X，Z ，返回高度 ） ；返回（为真表示成功）
	bool                    GetGridHeight(int x,int z , float* height );
	//获得高度图宽度，高度
	int                   GetMapWidth(){return m_nWidth;};
	int                   GetMapHeight(){return m_nHeight;};
	//处理高度图 ， 参数（操作信息， 修改模式，修改范围模式 ，修改的中心位置， 修改高度， 修改范围  ,是否进行修改)
	bool                    
	ProcessMap(HEIGHT_PROC_EVEN  even ,HEIGHT_CHANGE_MODE mode,HEIGHT_AREA area,  MVec3  centerPos , float height ,float fsize,bool bChange = true);
	//读取BMP；参数（文件名）；返回（为真表示成功）
	bool                    ReadBmp(const char*  filename);
	//保存为BMP；参数（文件名）；返回（为真表示成功）
	bool                    SaveBmp(const char* filename);
    //设置高度数据；参数（ 宽度，高度，输入数据 ） 返回（ 为真表示成功）
	bool                   SetHeightData( int width ,int height , unsigned char *data);	
	//计算法线
	void                    CountNormal();
	//更新法线（ X ，Y ）
	void                    UpdateNormal(int x,int y);
	//更新法线；参数（ 索引ID ）
	void                    UpdateNormal(int index );
    //得到格子数
	int                     GerGridNum(){return m_nGridNum;}
public:
	//具体操作;平滑所有地形数据
	void                    SmoothHeightMapData();
private:
    //使高度线性统一升高,
	void                    ChangeSelectHeigthLine(float height , HEIGHT_CHANGE_MODE mode);
	//使高度平滑升高,按弧度升高
	void                    ChangeSelectHeightRadio(float height , HEIGHT_CHANGE_MODE mode, float fsize);
	//平整所选择顶点
	void                    LevelOffSelectHeight(float height);
    //平滑所选中的顶点
	void                    SmoothSelectHeightMapData();
	//
private:
	//计算法线；参数（ 顶点0 ， 顶点1 ，顶点2 ，顶点3 ，顶点4 ）； 返回（ 计算后的法线 ）
	MVec3                   CountNormal( MVec3 &v0,MVec3 &v1,MVec3 &v2,MVec3 &v3,MVec3 &v4 );
	//设置地图信息；参数(中心位置, 范围  , 区域方式 )；返回（为真表示成功）
	bool                    SelectMapInfo(MVec3 pos ,float fsize,HEIGHT_AREA area);
public:
	//高度
	std::vector<float>      m_vHeight;
	//顶点
	std::vector<MVec3>      m_vVec;
	//法线
	std::vector<MVec3>      m_vNormal;
private:
	//选择信息
	std::vector<Select_Info>   m_vSelectInfo;
    //多少格子数
	int                     m_nGridNum;
	//高度图,宽度和高度
	int                     m_nWidth,m_nHeight;
    //开始位置
	MVec3                   m_vStartPos;
	//格子大小
	float                   m_fSize;
	//引入高度图，对应的比例
	float                   m_fScale;
};

}