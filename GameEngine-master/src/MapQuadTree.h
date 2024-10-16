#pragma once
#include <vector>
#include <limits>
#include "OgreRect.h"

using namespace std;

#define QUADTREE_NODE_VECTOR_SIZE 200000
#define NEIGHBOR_SIZE 400000
#define MAX_QT_MAPS 16
#define QT_NODE_INVALID INT_MAX

#define MASK_WALK	0x00		// ����������
#define MASK_STOP	0x01		// �����赲��
#define MASK_MAGIC	0x04		// �����赲��
#define MASK_WATER	0x02		// ˮ��
#define MASK_SPELL	0x08		// ���ܲ�

#define DYN_MASK_STOP 0x20		// �붯̬�����赲��
#define DYN_MASK_MAGIC 0x40		// �붯̬ħ���赲

//------------------------------------------------------
// �ٽڵ�λ��
//
typedef enum NBPosType
{
	eNBEast = 0,
	eNBSouth,
	eNBWest,
	eNBNorth,
	eNumNBPosType

} NBPosType;

const  int NbOffset[eNumNBPosType][2] = {{1,0},{0,1},{-1,0},{0,-1}};

class QTNode
{
	friend class GameMaskMap;
	friend class CQuadTree;

public:
	QTNode() { Set(0,0,0,0,0,0,0,0,false,false); }
	QTNode(char lv,int ix,int iz,int i_n,unsigned short x,unsigned short y, short cs, unsigned char ev, bool b, bool b1)
	{ Set(lv,ix,iz,i_n,x,y,cs,ev,b,b1); }
	void Set(char lv,int ix,int iz,int i_n,unsigned short x,unsigned short y,short cs,unsigned char ev,bool b, bool b1);

private:
	int    level;		// �ڵ����ڲ�
	int    xi, yi;		// �ڵ�������Ӧ���е�cell����

	int    ni;			// �ڵ��ڽڵ������е�����ֵ
	unsigned short px, py;		// cell���ĵ��λ��
	unsigned short cellSize; //cell�Ĵ�С
	unsigned char  value;		// �ڵ�ֵ	
	bool bVisited;	//Ѱ��ʱ�ж��Ƿ��ѱ����ʹ�
	bool bLeaf;	//�Ƿ�Ҷ�ӽڵ�

	int child[4]; //���ӽڵ�����i

	int nbStart;				//�ٽڵ��������ٽڵ��б��е���ʼ����
	unsigned short  nbCount;	//�ٽڵ�ĸ���
	int nbMagicStart;	
	unsigned short  nbMagicCount;
};


class CQuadTree
{
public:

	CQuadTree(void);

	~CQuadTree(void);

	//ͨ��mapmask����quadtree map
	void Create(const unsigned char* maskMap, int sx, int sy, int sc);

	bool MergeMap(int** mapL, int sxL, int syL, int*** mapU, int* sxU, int* syU);

	void CreateNodeNetWork();

	void GetNeighbors(QTNode* pQTNode, NBPosType nbPos, vector<int>& vecQTNodeList, int mask = MASK_STOP | DYN_MASK_STOP);

	void GetSideNode(char level, int ix, int iy, NBPosType nbPos, vector<int>& vecQTNodeList, int mask = MASK_STOP |DYN_MASK_STOP);

	//��һ�����ص�ӳ�䵽�Ĳ�����һ��Ҷ�ӽڵ���,���ظ�Ҷ�ӽڵ����ڽڵ������
	int MapPixelToQtnode(int x, int y);
	int MapPixelToQtnode(int nodeIndex, int x, int y);

	//��дqdt�ļ�
	bool SaveQdtFile(const char* szFileName);
	bool LoadQdtFile(const char* szFileName);

	//����m_vecAllSearchNodes
	void UpdateSearchNodes(const unsigned char* maskMap, int sx, int sy, int sc);
public:
	int		**m_quadMaps[MAX_QT_MAPS];//�ֲ��matrix map
	int			m_sizeX [MAX_QT_MAPS];
	int			m_sizeY [MAX_QT_MAPS];
	int			m_sizeCell [MAX_QT_MAPS];
	Ogre::Point2D			m_minCenter [MAX_QT_MAPS];	// ���Ͻ�cell��center
	int						m_nMaps;
	int						m_nTopCells;				// ���cell�ĸ���
	int						m_nLeafNodes;				// Ҷ�ڵ����

	vector<QTNode>			m_vecAllNodes;			// �Ĳ������еĽڵ�
	vector<int>				m_vecNbIndex;			// ���нڵ���ٽڵ�����			
	vector<int>				m_vecNbMagicIndex;		// ħ�������нڵ���ٽڵ�����		
};

