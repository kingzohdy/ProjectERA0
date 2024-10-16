
#pragma once

#include "EditEventHandler.h"

#include <string>
#include <deque>

namespace TE
{
//б����Ϣ
struct MBreaInfo
{
	int gx,gy;//����λ��, 
	int mx,my;//�����е�λ�� 
	int  x,y ;//��ͼMAPλ��
	TerrModel*   pLModel;//б�����ģ��
	TerrModel*   pRmodel;//б���ұ�ģ��

	float  fHeight ;//�߶�
};
//����б����Ϣ
struct MAddBreaInfo
{
	int x,y;//x,y����
	int nType;//����
	MVec3 dir,pos;//
};

class MBraeEditProcess
		: public IEventProcess
{
public:
	MBraeEditProcess(void);
	virtual ~MBraeEditProcess(void);
    //�������;����( �༭��ָ��, ��Ϣ��Ӧ�¼� ) ; ���� ( Ϊ���ʾ�ɹ� )
	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
    //���˲��� ; ����( �༭��ָ�� )
	virtual  void  Undo( MTerrainEdit *pEditor );
	//�������� ; ����( �༭��ָ��)
	virtual  void  SaveData( MTerrainEdit* pEditor );
    //����б��ģ��; ����( �ļ��� )
	void           SetBraeModelName( std::string &name );
    //����
	void           Update();
	//ɾ����ǰб��; ����( �༭��ָ�� )
	void           DelCurBrae( MTerrainEdit* pEditor );
    //����;����( �ļ��� ) ; ���� ( Ϊ���ʾ�ɹ� )
	bool           Save( const char* filename );
	//��ȡ;����( �༭��ָ��, �ļ��� ) ;����( Ϊ���ʾ�ɹ�)
	bool           Read( MTerrainEdit* pEditor , const char* filename );
	//��̬����; ����( �༭��ָ�� )
	void          Loading( MTerrainEdit* pEditor );
private:
	//����б��; ����( �༭��ָ��,���߷���,����λ��)
	void          AddBrea( MTerrainEdit* pEditor , MVec3 dir , MVec3 pos ) ;
	//ɾ��б��; ����( �༭��ָ��, б����Ϣ )
	void          DelBrea(  MTerrainEdit* pEditor ,MBreaInfo*  pInfo);
    //����ģ��; ����( �ļ���,λ�� )
	TerrModel*   AddModel( const char* filename , MVec3 pos );
    //����chunk�߶�; ����( ��������X,Y ,С�������� X,Y )
	void          UpdateChunkHeight( int x, int y  ,int mx ,int my );
	//���±����CHUNK�߶� ; ����( ��������X,Y ,С�������� X,Y , �߶� )
	void          UpdateSaveChunkHeight( int x ,int y ,int mx ,int my , float fHeight );
    //����б��; ����( ��������X,Y ,С�������� X,Y ) ; ����( Ϊ���ʾ�ҵ� )
	bool           FindeBrea( int gx ,int gy ,int mx ,int my );
private:
	//�Ƿ�����
	bool           m_bLoaded;
    //б����С�߶Ⱥ����߶�
	float         m_fMinHeight,m_fMaxHeight ;
    //��ǰ�ƶ��ĸ���λ�� ( gx , gy )
	int            m_nCurMoveGx , m_nCurMoveGy;
	//��ǰ�ƶ���С����λ��( mx , my )
	int            m_nCurMoveMx , m_nCurMoveMy;
    //б����Ϣ����
	typedef std::vector<MBreaInfo>     MBREAINFO_VEC;
	std::vector<MBreaInfo>       m_BreaInfoSet;

	typedef std::vector<MAddBreaInfo>  MADDBREA_VEC;
	//�������ӵ�б����Ϣ
	std::vector<MAddBreaInfo>    m_SaveBreaInfoSet;
	//б����Ϣ
	std::vector<MAddBreaInfo>    m_BreaSet;
};

};
