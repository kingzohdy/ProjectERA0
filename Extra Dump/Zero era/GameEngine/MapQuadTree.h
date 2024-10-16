#pragma once
#include <vector>
#include <limits>
#include "OgreRect.h"

using namespace std;

#define QUADTREE_NODE_VECTOR_SIZE 200000
#define NEIGHBOR_SIZE 400000
#define MAX_QT_MAPS 16
#define QT_NODE_INVALID INT_MAX

#define MASK_WALK	0x00		// 可行走区域
#define MASK_STOP	0x01		// 行走阻挡层
#define MASK_MAGIC	0x04		// 法术阻挡层
#define MASK_WATER	0x02		// 水层
#define MASK_SPELL	0x08		// 技能层

#define DYN_MASK_STOP 0x20		// 半动态行走阻挡层
#define DYN_MASK_MAGIC 0x40		// 半动态魔法阻挡

//------------------------------------------------------
// 临节点位置
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
	int    level;		// 节点所在层
	int    xi, yi;		// 节点在其相应层中的cell索引

	int    ni;			// 节点在节点数组中的索引值
	unsigned short px, py;		// cell中心点的位置
	unsigned short cellSize; //cell的大小
	unsigned char  value;		// 节点值	
	bool bVisited;	//寻径时判断是否已被访问过
	bool bLeaf;	//是否叶子节点

	int child[4]; //孩子节点的索襥

	int nbStart;				//临节点在所有临节点列表中的起始索引
	unsigned short  nbCount;	//临节点的个数
	int nbMagicStart;	
	unsigned short  nbMagicCount;
};


class CQuadTree
{
public:

	CQuadTree(void);

	~CQuadTree(void);

	//通过mapmask创建quadtree map
	void Create(const unsigned char* maskMap, int sx, int sy, int sc);

	bool MergeMap(int** mapL, int sxL, int syL, int*** mapU, int* sxU, int* syU);

	void CreateNodeNetWork();

	void GetNeighbors(QTNode* pQTNode, NBPosType nbPos, vector<int>& vecQTNodeList, int mask = MASK_STOP | DYN_MASK_STOP);

	void GetSideNode(char level, int ix, int iy, NBPosType nbPos, vector<int>& vecQTNodeList, int mask = MASK_STOP |DYN_MASK_STOP);

	//将一个像素点映射到四叉树的一个叶子节点上,返回该叶子节点所在节点的索引
	int MapPixelToQtnode(int x, int y);
	int MapPixelToQtnode(int nodeIndex, int x, int y);

	//读写qdt文件
	bool SaveQdtFile(const char* szFileName);
	bool LoadQdtFile(const char* szFileName);

	//更新m_vecAllSearchNodes
	void UpdateSearchNodes(const unsigned char* maskMap, int sx, int sy, int sc);
public:
	int		**m_quadMaps[MAX_QT_MAPS];//分层的matrix map
	int			m_sizeX [MAX_QT_MAPS];
	int			m_sizeY [MAX_QT_MAPS];
	int			m_sizeCell [MAX_QT_MAPS];
	Ogre::Point2D			m_minCenter [MAX_QT_MAPS];	// 左上角cell的center
	int						m_nMaps;
	int						m_nTopCells;				// 最顶层cell的个数
	int						m_nLeafNodes;				// 叶节点个数

	vector<QTNode>			m_vecAllNodes;			// 四叉树所有的节点
	vector<int>				m_vecNbIndex;			// 所有节点的临节点索引			
	vector<int>				m_vecNbMagicIndex;		// 魔法层所有节点的临节点索引		
};

