
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
	//�޸ĸ߶�ģʽ
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
    //�޸ĸ߶ȷ�Χ
	enum HEIGHT_AREA
	{
		PAINT_CIRCLE = 0,		//Բ��
		PAINT_QUAD,             //����
	};
    //�ı�߶ȷ�ʽ
	enum  HEIGHT_CHANGE_MODE
	{
		ALPHA_ADD = 0,        //����
		ALPHA_DEC ,           //����
	};
	//ѡ�еĸ߶ȣ���Ӧ��Ϣ
	struct Select_Info
	{
		int  vecIndex;
		float  length;
	};;
    //�����߶�ͼ�������� ��ȣ��߶� ����ʼλ�ã���С�������߶� �����ţ� 
    void                    CreateMap(MPoint2D nblock, MPoint2D ngrid, MVec3 startPos , float fSize ,float fbaseHeight = 1.0f,float fScale = 1.0f);
	//�ݻ�
	void                    Destory();	
	//��ø߶���Ϣ�������� X��Z �����ظ߶� �� �����أ�Ϊ���ʾ�ɹ���
	bool                    GetHeight(float x, float z,float*  height);
	//��ø��Ӹ߶���Ϣ�������� X��Z �����ظ߶� �� �����أ�Ϊ���ʾ�ɹ���
	bool                    GetGridHeight(int x,int z , float* height );
	//��ø߶�ͼ��ȣ��߶�
	int                   GetMapWidth(){return m_nWidth;};
	int                   GetMapHeight(){return m_nHeight;};
	//����߶�ͼ �� ������������Ϣ�� �޸�ģʽ���޸ķ�Χģʽ ���޸ĵ�����λ�ã� �޸ĸ߶ȣ� �޸ķ�Χ  ,�Ƿ�����޸�)
	bool                    
	ProcessMap(HEIGHT_PROC_EVEN  even ,HEIGHT_CHANGE_MODE mode,HEIGHT_AREA area,  MVec3  centerPos , float height ,float fsize,bool bChange = true);
	//��ȡBMP���������ļ����������أ�Ϊ���ʾ�ɹ���
	bool                    ReadBmp(const char*  filename);
	//����ΪBMP���������ļ����������أ�Ϊ���ʾ�ɹ���
	bool                    SaveBmp(const char* filename);
    //���ø߶����ݣ������� ��ȣ��߶ȣ��������� �� ���أ� Ϊ���ʾ�ɹ���
	bool                   SetHeightData( int width ,int height , unsigned char *data);	
	//���㷨��
	void                    CountNormal();
	//���·��ߣ� X ��Y ��
	void                    UpdateNormal(int x,int y);
	//���·��ߣ������� ����ID ��
	void                    UpdateNormal(int index );
    //�õ�������
	int                     GerGridNum(){return m_nGridNum;}
public:
	//�������;ƽ�����е�������
	void                    SmoothHeightMapData();
private:
    //ʹ�߶�����ͳһ����,
	void                    ChangeSelectHeigthLine(float height , HEIGHT_CHANGE_MODE mode);
	//ʹ�߶�ƽ������,����������
	void                    ChangeSelectHeightRadio(float height , HEIGHT_CHANGE_MODE mode, float fsize);
	//ƽ����ѡ�񶥵�
	void                    LevelOffSelectHeight(float height);
    //ƽ����ѡ�еĶ���
	void                    SmoothSelectHeightMapData();
	//
private:
	//���㷨�ߣ������� ����0 �� ����1 ������2 ������3 ������4 ���� ���أ� �����ķ��� ��
	MVec3                   CountNormal( MVec3 &v0,MVec3 &v1,MVec3 &v2,MVec3 &v3,MVec3 &v4 );
	//���õ�ͼ��Ϣ������(����λ��, ��Χ  , ����ʽ )�����أ�Ϊ���ʾ�ɹ���
	bool                    SelectMapInfo(MVec3 pos ,float fsize,HEIGHT_AREA area);
public:
	//�߶�
	std::vector<float>      m_vHeight;
	//����
	std::vector<MVec3>      m_vVec;
	//����
	std::vector<MVec3>      m_vNormal;
private:
	//ѡ����Ϣ
	std::vector<Select_Info>   m_vSelectInfo;
    //���ٸ�����
	int                     m_nGridNum;
	//�߶�ͼ,��Ⱥ͸߶�
	int                     m_nWidth,m_nHeight;
    //��ʼλ��
	MVec3                   m_vStartPos;
	//���Ӵ�С
	float                   m_fSize;
	//����߶�ͼ����Ӧ�ı���
	float                   m_fScale;
};

}