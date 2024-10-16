
#ifdef _OLD_COORD

#pragma once


#include "OgreLines.h"

namespace TE
{
//坐标的长度
#define   AXES_LENGHT   600.0f
//坐标的宽度
#define   AXES_SIZE     10.0f

#define   AXES_X_COLOR  0xffff0000  // x 轴颜色
#define   AXES_Y_COLOR  0xff00ff00  // y 轴颜色
#define   AXES_Z_COLOR  0xff0000ff  // Z 轴颜色
#define   AXES_XY_COLOR  0xffffff00  // XY 面颜色
#define   AXES_YZ_COLOR  0xff00ffff  // YZ 面颜色
#define   AXES_XZ_COLOR  0xffff00ff  // XZ 面颜色
#define   AXES_GRAY_COLOR  0xff777777  //灰色

#define   AXES_SELECT_COLOR  0xffffff00  //选中颜色

//坐标选中类型
enum COORDINATE_SELECT
{
	NO_AXES_SELECT = -1,
    SELECT_X_AXES = 0,
	SELECT_Y_AXES,
	SELECT_Z_AXES,
	SELECT_XY_AXES,
	SELECT_YZ_AXES,
	SELECT_XZ_AXES,
};
//线的顶点格式
struct LineVec_st
{
	MVec3  pos;//位置
    unsigned int  color;//颜色
};
//坐标的渲染数据
struct MCoordinateRenderData
{
	LineVec_st vdata[256];
	unsigned short idata[256];
};
//坐标类:
//绘制一个类似3DMAX的坐标.并支持选择不同轴
class EditorModel;
class MCoordinate
{
public:
	MCoordinate(void);
	~MCoordinate(void);

    COORDINATE_SELECT   TestIntersect( MVec3  &dir , MVec3 &pos ,int mode = 0 );
    COORDINATE_SELECT   TestIntersectEx( MVec3  &dir , MVec3 &pos ,int mode = 0 );

    COORDINATE_SELECT   TestIntersectBall( MVec3  &dir , MVec3 &pos ,int mode = 0 );
	COORDINATE_SELECT   GetCurCoordinate(){return m_CurCoordSelect;}
    //绑定模型；参数（模型指针 ）
	void                BindObject( EditorModel* model  );
    //更新
	void                Update(float dtime);
	//更新球体坐标
	void                UpdateBall(float dtime);
   
	void                UpdateEx(float dtime);

	void                Render();
	void                RenderBall();

	void                RenderEx();
	bool                Init();
	bool                InitEx();
	void                Release();

	void                SetCurMode(int mode){m_nCurMode = mode;};
	int                 GetCurMode(){return m_nCurMode ;}

	MVec3               GetRayIntersectXY( MVec3& pos , MVec3&dir );
	MVec3               GetRayIntersectXZ( MVec3& pos , MVec3&dir );
	MVec3               GetRayIntersectYZ( MVec3& pos , MVec3&dir );
private:
	bool                CreateData();
	bool                CreateBallData();
	bool                CreateSacleData();

	bool                CreateDateEx();
	bool                CreateHLSLData();
private:
	//选择坐标的类型
	COORDINATE_SELECT         m_CurCoordSelect;
	//当前绑定的模型
	EditorModel*                    m_pCurBinObject;
	//对应的 x ,y , z 轴的包围盒
	MAABBox                     m_xAxesBox;
	MAABBox                     m_yAxesBox;
	MAABBox                     m_zAxesBox;
    //对应 xy , yz , xz 面的包围盒
	MAABBox                     m_xyAxesBox;
	MAABBox                     m_yzAxesBox;
	MAABBox                     m_xzAxesBox;
    //缩放
	float                           m_fScale;
    //移动位置坐标的渲染数据
	MCoordinateRenderData           m_RenderData;
	//旋转位置的坐标渲染数据
	MCoordinateRenderData           m_BallRenderData;
	//缩放的坐标渲染数据
    MCoordinateRenderData            m_ScaleRenderData;
	//当前模式
	int                             m_nCurMode;
	//球的X,Y位置
	float                           m_BallPosX[32];
	float                           m_BallPosY[32];
    //物件缩放比例
	float                            m_fScaleObject;
public:
	//当前位置
	MVec3                            m_vCurPos;
	//保存当前物件位置
	MVec3                            m_vSaveObjectPos;
};


};

#endif