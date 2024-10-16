
#ifdef _OLD_COORD

#pragma once


#include "OgreLines.h"

namespace TE
{
//����ĳ���
#define   AXES_LENGHT   600.0f
//����Ŀ��
#define   AXES_SIZE     10.0f

#define   AXES_X_COLOR  0xffff0000  // x ����ɫ
#define   AXES_Y_COLOR  0xff00ff00  // y ����ɫ
#define   AXES_Z_COLOR  0xff0000ff  // Z ����ɫ
#define   AXES_XY_COLOR  0xffffff00  // XY ����ɫ
#define   AXES_YZ_COLOR  0xff00ffff  // YZ ����ɫ
#define   AXES_XZ_COLOR  0xffff00ff  // XZ ����ɫ
#define   AXES_GRAY_COLOR  0xff777777  //��ɫ

#define   AXES_SELECT_COLOR  0xffffff00  //ѡ����ɫ

//����ѡ������
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
//�ߵĶ����ʽ
struct LineVec_st
{
	MVec3  pos;//λ��
    unsigned int  color;//��ɫ
};
//�������Ⱦ����
struct MCoordinateRenderData
{
	LineVec_st vdata[256];
	unsigned short idata[256];
};
//������:
//����һ������3DMAX������.��֧��ѡ��ͬ��
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
    //��ģ�ͣ�������ģ��ָ�� ��
	void                BindObject( EditorModel* model  );
    //����
	void                Update(float dtime);
	//������������
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
	//ѡ�����������
	COORDINATE_SELECT         m_CurCoordSelect;
	//��ǰ�󶨵�ģ��
	EditorModel*                    m_pCurBinObject;
	//��Ӧ�� x ,y , z ��İ�Χ��
	MAABBox                     m_xAxesBox;
	MAABBox                     m_yAxesBox;
	MAABBox                     m_zAxesBox;
    //��Ӧ xy , yz , xz ��İ�Χ��
	MAABBox                     m_xyAxesBox;
	MAABBox                     m_yzAxesBox;
	MAABBox                     m_xzAxesBox;
    //����
	float                           m_fScale;
    //�ƶ�λ���������Ⱦ����
	MCoordinateRenderData           m_RenderData;
	//��תλ�õ�������Ⱦ����
	MCoordinateRenderData           m_BallRenderData;
	//���ŵ�������Ⱦ����
    MCoordinateRenderData            m_ScaleRenderData;
	//��ǰģʽ
	int                             m_nCurMode;
	//���X,Yλ��
	float                           m_BallPosX[32];
	float                           m_BallPosY[32];
    //������ű���
	float                            m_fScaleObject;
public:
	//��ǰλ��
	MVec3                            m_vCurPos;
	//���浱ǰ���λ��
	MVec3                            m_vSaveObjectPos;
};


};

#endif