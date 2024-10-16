
#include "OgreResourceManager.h"
#include "OgreDataStream.h"

#include "MapQuadTree.h"
#include <Windows.h>
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////
void QTNode::Set(char lv,int ix,int iz,int i_n,unsigned short x,unsigned short y,short cs,unsigned char ev, bool b, bool b1)
{
	level = lv;
	xi    = ix;
	yi    = iz;
	ni    = i_n;
	px    = x;
	py    = y;
	cellSize = cs;
	value = ev;

	nbStart = -1;
	nbCount = 0;
	nbMagicStart = -1;
	nbMagicCount = 0;

	bVisited = b;
	bLeaf = b1;
	memset(child, -1, 4*sizeof(int));
}

///////////////////////////////////////////////////////////////////////////////
CQuadTree::CQuadTree(void)
{
	memset(m_quadMaps, 0, MAX_QT_MAPS*sizeof(int**));
	m_nMaps = 0;
	m_nTopCells = 0;
	//m_vecAllNodes.reserve(QUADTREE_NODE_VECTOR_SIZE);
	//m_vecNbIndex.reserve(NEIGHBOR_SIZE);
	//m_vecNbMagicIndex.reserve(NEIGHBOR_SIZE);

	int iSize1 = sizeof(QTNode) * QUADTREE_NODE_VECTOR_SIZE/1024;
	int iSize2 = 4*NEIGHBOR_SIZE/1024;

	
}

///////////////////////////////////////////////////////////////////////////////
CQuadTree::~CQuadTree(void)
{/*
	//删除搜索节点
	for (int j=0; j<m_vecAllSearchNodes.size(); ++j)
	{
		if(m_vecAllSearchNodes[j] != NULL)
			delete m_vecAllSearchNodes[j];
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////
//通过mapmask创建quadtree map
void CQuadTree::Create(const unsigned char* maskMap, int sx, int sy, int sc)
{
	Ogre::Point2D minc(sc/2, sc/2);
	Ogre::Point2D maxc(((sx-1)*sc + sc/2), ((sy-1)*sc + sc/2));

	//从coverMap中获得最底层的地图
	int** map = new int*[sy];
	for(int i=0; i<sy; ++i)
	{
		map[i] = new int[sx];
		for(int j=0; j<sx; ++j)
		{
			map[i][j] = maskMap[i*sx + j] & (MASK_STOP | MASK_MAGIC | DYN_MASK_STOP | DYN_MASK_MAGIC);
		}
	}

	m_quadMaps[0] = map;
	m_sizeX[0] = sx;	
	m_sizeY[0] = sy;
	m_sizeCell[0] = sc;
	m_minCenter[0] = minc;

	//合并地图
	int cx, cy;
	m_nMaps = MAX_QT_MAPS;
	for( int i = 1; i < m_nMaps; ++i )
	{
		if( m_sizeX[i-1] >= 2 && m_sizeY[i-1] >= 2)
		{
			m_sizeCell[i] = m_sizeCell[i-1] * 2;
			cx = minc.x - m_sizeCell[0]/2 + m_sizeCell[i]/2;
			cy = minc.y - m_sizeCell[0]/2 + m_sizeCell[i]/2;
			m_minCenter[i] = Ogre::Point2D(cx,cy);
			if(!MergeMap(m_quadMaps[i-1], m_sizeX[i-1], m_sizeY[i-1], &m_quadMaps[i], &m_sizeX[i], & m_sizeY[i]))
			{
				m_nMaps = i;
				break;
			}
		}
		else
		{
			m_nMaps = i-1;
		}
	}

	CreateNodeNetWork();

	//删除地图
	for(int k=0; k<MAX_QT_MAPS; ++k)
	{
		if(m_quadMaps[k] == NULL) continue;
		for(int i=0; i<m_sizeY[k]; ++i)
			delete[] m_quadMaps[k][i];
		delete[] m_quadMaps[k];
	}
}

///////////////////////////////////////////////////////////////////////////////
//合并最原始的cell map
bool CQuadTree::MergeMap(int** mapL, int sxL, int syL, int*** mapU, int* sxU, int* syU)
{
	int sx = (sxL + 1) / 2;
	int sy = (syL + 1) / 2;
	int** map = new int*[sy];
	for(int i=0; i<sy; ++i)
	{
		map[i] = new int[sx];
		for(int j=0; j<sx; ++j)
		{
			map[i][j] = QT_NODE_INVALID;
		}
	}

	bool bMergedAtLeastOnce = false;
	int i2, j2;
	int cell1, cell2, cell3, cell4;
	for( int i = 0; i < sy; ++i )
		for(int j=0; j<sx; ++j)
		{
			i2 = i + i;
			j2 = j + j;
			cell1 = mapL[i2][j2];
			// warning:sxL是奇数时访问越界
			int addi, addj;
			if (i2 + 1 < syL)
			{
				addi = i2 + 1;
			}
			else
			{
				addi = i2;
			}
			if (j2 + 1 < sxL)
			{
				addj = j2 + 1;
			}
			else
			{
				addj = j2;
			}
			cell2 = mapL[i2][addj];
			cell3 = mapL[addi][j2];
			cell4 = mapL[addi][addj];

			if(cell1 != QT_NODE_INVALID && cell1 == cell2 && cell2 == cell3 && cell3 == cell4)
			{
				map[i][j] = cell1;
				mapL[i2][j2] = mapL[i2][addj] = mapL[addi][j2] = mapL[addi][addj] = QT_NODE_INVALID;
				bMergedAtLeastOnce = true;
			}
			else
			{
				map[i][j] = QT_NODE_INVALID;
			}
		}

		(*mapU) = map;
		(*sxU) = sx;
		(*syU) = sy;
		return bMergedAtLeastOnce;
}

///////////////////////////////////////////////////////////////////////////////
// 从最顶层开始创建四叉树：
//	1、如果map[i][j]的值不是QT_NODE_INVALID,
//		则将其创建为叶子节点，
//		加入节点数组，
//		改变map的值为该节点在节点数组中的索引
//		设置其父节点的子节点
//	2、否则
//		如果map[i][j]存在是叶子节点的父节点，则将map[i][j]的值改为其父节点的索引
//		否则
//			将map[i][j]创建为一个非叶子节点
//			加入节点数组
//			改变map的值为该节点在节点数组中的索引
//			设置其父节点的子节点		
//  3、查找所有叶子节点的临节点
void CQuadTree::CreateNodeNetWork()
{
	int** map;
	int sc, mincx, mincy;

	m_vecAllNodes.clear();
	for(int k=m_nMaps-1; k>=0; --k)
	{
		map = m_quadMaps[k];
		sc = m_sizeCell[k];
		mincx = m_minCenter[k].x;
		mincy = m_minCenter[k].y;

		int i1, j1, k1;
		for(int i=0; i<m_sizeY[k]; ++i)
		{
			for(int j=0; j<m_sizeX[k]; ++j)
			{
				if(map[i][j] != QT_NODE_INVALID)
				{
					QTNode qtnode(k,j,i,(int)m_vecAllNodes.size(), mincx+j*sc,mincy+i*sc, m_sizeCell[k], (unsigned char)map[i][j], false, true);
					m_vecAllNodes.push_back(qtnode);
					map[i][j] = qtnode.ni;
					if(m_nMaps - 1 == k)
					{
						++m_nTopCells;
					}
					//设置该节点为其父节点的子节点
					i1 = i / 2;
					j1 = j / 2;
					k1 = k + 1;	
					int iChild = 0;
					if(k1 < m_nMaps)
					{
						while(-1 != m_vecAllNodes[m_quadMaps[k1][i1][j1]].child[iChild]) ++iChild;
						m_vecAllNodes[m_quadMaps[k1][i1][j1]].child[iChild] = qtnode.ni;
					}
				}
				else
				{
					i1 = i / 2;
					j1 = j / 2;
					k1 = k + 1;
					if(k1 < m_nMaps && m_vecAllNodes[m_quadMaps[k1][i1][j1]].bLeaf)
					{
						map[i][j] = m_quadMaps[k1][i1][j1];
					}
					else
					{
						QTNode qtnode(k,j,i,(int)m_vecAllNodes.size(), mincx+j*sc,mincy+i*sc, m_sizeCell[k],(unsigned char)map[i][j], false, false);
						m_vecAllNodes.push_back(qtnode);
						map[i][j] = qtnode.ni;
						if(m_nMaps - 1 == k)
						{
							++m_nTopCells;
						}
						//设置该节点为其父节点的子节点
						i1 = i / 2;
						j1 = j / 2;
						k1 = k + 1;	
						int iChild = 0;
						if(k1 < m_nMaps)
						{
							while(-1 != m_vecAllNodes[m_quadMaps[k1][i1][j1]].child[iChild]) ++iChild;
							m_vecAllNodes[m_quadMaps[k1][i1][j1]].child[iChild] = qtnode.ni;
						}
					}
				}
			}
		}
	}

	//查找临节点
	vector<int> vecQTNodeNb;
	m_vecNbIndex.clear();
	m_vecNbMagicIndex.clear();
	for( size_t k = 0; k < m_vecAllNodes.size(); ++k )
	{
		QTNode* pQtNode = &m_vecAllNodes[k];
		if(pQtNode->bLeaf && !( (MASK_STOP | DYN_MASK_STOP) & pQtNode->value ) )
		{
			pQtNode->nbStart = (int)m_vecNbIndex.size();
			for(int j=0; j<eNumNBPosType; ++j)
			{
				GetNeighbors(pQtNode,(NBPosType)j, vecQTNodeNb, MASK_STOP | DYN_MASK_STOP);
				if(!vecQTNodeNb.empty())
				{
					pQtNode->nbCount += (unsigned short)vecQTNodeNb.size();
					m_vecNbIndex.insert(m_vecNbIndex.end(), vecQTNodeNb.begin(), vecQTNodeNb.end());
				}
			}
		}

		pQtNode = &m_vecAllNodes[k];
		if(pQtNode->bLeaf && !( (MASK_MAGIC | DYN_MASK_MAGIC) & pQtNode->value ) )
		{
			pQtNode->nbMagicStart = (int)m_vecNbMagicIndex.size();
			for(int j=0; j<eNumNBPosType; ++j)
			{
				GetNeighbors(pQtNode,(NBPosType)j, vecQTNodeNb, MASK_MAGIC | DYN_MASK_MAGIC);
				if(!vecQTNodeNb.empty())
				{
					pQtNode->nbMagicCount += (unsigned short)vecQTNodeNb.size();
					m_vecNbMagicIndex.insert(m_vecNbMagicIndex.end(), vecQTNodeNb.begin(), vecQTNodeNb.end());
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// 找到某个方向的所有临节点
void CQuadTree::GetNeighbors(QTNode* pQTNode, NBPosType nbPos, vector<int>& vecQTNodeNb, int mask)
{
	vecQTNodeNb.clear();
	int ix = pQTNode->xi + NbOffset[nbPos][0];
	int iy = pQTNode->yi + NbOffset[nbPos][1];

	if(ix < 0 || ix >= m_sizeX[pQTNode->level] || iy < 0 || iy >= m_sizeY[pQTNode->level]) return;
	int k = m_quadMaps[pQTNode->level][iy][ix];
	if(m_vecAllNodes[k].bLeaf)
	{
		if(mask & m_vecAllNodes[k].value)
		{
			return;
		}
		else
		{
			vecQTNodeNb.push_back(m_vecAllNodes[k].ni);
			return;
		}
	}

	GetSideNode(pQTNode->level, ix, iy, nbPos, vecQTNodeNb,mask);
}

void CQuadTree::GetSideNode(char level, int ix, int iy, NBPosType nbPos, vector<int>& vecQTNodeNb, int mask)
{
	if(level < 0) return;

	int k = m_quadMaps[level][iy][ix];
	if(m_vecAllNodes[k].bLeaf && m_vecAllNodes[k].level == level)
	{
		if(mask & m_vecAllNodes[k].value)
		{
			return;
		}
		else
		{
			vecQTNodeNb.push_back(m_vecAllNodes[k].ni);
			return;
		}
	}

	int ix1,iy1,ix2,iy2;
	switch (nbPos)
	{
	case eNBEast:	ix1=0; iy1=0; ix2=0; iy2=1; break;
	case eNBSouth:	ix1=1; iy1=0; ix2=0; iy2=0; break;
	case eNBWest:	ix1=1; iy1=1; ix2=1; iy2=0; break;
	case eNBNorth:	ix1=0; iy1=1; ix2=1; iy2=1; break;
	}
	ix1 += ix*2;
	iy1 += iy*2;
	ix2 += ix*2;
	iy2 += iy*2;

	vector<int> vecList1;
	vector<int> vecList2;
	GetSideNode(level-1, ix1, iy1, nbPos, vecList1, mask);
	GetSideNode(level-1, ix2, iy2, nbPos, vecList2, mask);

	vecQTNodeNb.insert(vecQTNodeNb.end(), vecList1.begin(), vecList1.end());
	vecQTNodeNb.insert(vecQTNodeNb.end(), vecList2.begin(), vecList2.end());
}

struct QdtHeader
{
	char magic_num[4];
	int version;
};
bool CQuadTree::SaveQdtFile(const char* szFileName)
{
	ofstream qdtFile(szFileName,ios::binary);
	if(!qdtFile)
	{
		//cout << "打开qdt文件失败！" << endl;
		return false;
	}

	QdtHeader header;
	strncpy(header.magic_num, "MQT",4);
	header.version = 100;
	qdtFile.write((char *)&header, sizeof(header));

	//写入最顶层节点个数
	qdtFile.write((char*)&m_nTopCells, sizeof(m_nTopCells));
	//写入叶节点个数
	int pointCount = 0;
	for(size_t i=0; i<m_vecAllNodes.size(); ++i)
	{
		if(m_vecAllNodes[i].bLeaf)
		{
			++pointCount;
		}
	}
	qdtFile.write((char*)&pointCount, sizeof(pointCount));

	//写入总结点个数
	int nAllNodes = (int)m_vecAllNodes.size();
	qdtFile.write((char*)&nAllNodes, sizeof(int));
	qdtFile.write((char *)&m_vecAllNodes[0], nAllNodes*sizeof(QTNode));
	
	int n = (int)m_vecNbIndex.size();
	qdtFile.write((char *)&n, sizeof(int));
	qdtFile.write((char *)&m_vecNbIndex[0], n*sizeof(int));

	n = (int)m_vecNbMagicIndex.size();
	qdtFile.write((char *)&n, sizeof(int));
	qdtFile.write((char *)&m_vecNbMagicIndex[0], n*sizeof(int));

	qdtFile.close();
	return true;

	//写入节点数据,只写入对寻径有用的数据:
	//int    ni;			// 节点在节点数组中的索引值
	//unsigned short px, py;		// cell中心点的位置
	//unsigned short cellSize; //cell的大小
	//unsigned char  value;		// 节点值	
	//bool bVisited;	//寻径时判断是否已被访问过
	//bool bLeaf;	//是否叶子节点
	//int child[4]; //孩子节点的索引
	//unsigned short nbCount;//临节点的个数
	//int* nbList;//临节点列表
	QTNode* pQtNode = NULL;
	for( size_t i=0; i < m_vecAllNodes.size(); ++i )
	{
		pQtNode = &m_vecAllNodes[i];
		qdtFile.write((char*)&pQtNode->ni, sizeof(pQtNode->ni));
		qdtFile.write((char*)&pQtNode->px, sizeof(pQtNode->px));
		qdtFile.write((char*)&pQtNode->py, sizeof(pQtNode->py));
		qdtFile.write((char*)&pQtNode->cellSize, sizeof(pQtNode->cellSize));
		qdtFile.write((char*)&pQtNode->value, sizeof(pQtNode->value));
		qdtFile.write((char*)&pQtNode->bVisited, sizeof(pQtNode->bVisited));
		qdtFile.write((char*)&pQtNode->bLeaf, sizeof(pQtNode->bLeaf));
		if(!pQtNode->bLeaf)
		{
			qdtFile.write((char*)&pQtNode->child[0], sizeof(int)*4);
		}
		qdtFile.write((char*)&pQtNode->nbCount, sizeof(pQtNode->nbCount));
		//if(pQtNode->nbCount > 0)
		//	qdtFile.write((char*)pQtNode->nbList, sizeof(int)*pQtNode->nbCount);
	}
	qdtFile.close();
	return true;
}

bool CQuadTree::LoadQdtFile(const char* szFileName)
{
	Ogre::DataStream *fp = Ogre::ResourceManager::getSingleton().openFileStream(szFileName, true);
	if(fp == NULL)
	{
		return false;
	}

	QdtHeader header;
	fp->read(&header, sizeof(header));

	fp->read((char*)&m_nTopCells, sizeof(m_nTopCells));
	fp->read((char*)&m_nLeafNodes, sizeof(m_nLeafNodes));

	int nAllNodes;

	fp->read(&nAllNodes, sizeof(nAllNodes));
	m_vecAllNodes.resize(nAllNodes);
	fp->read(&m_vecAllNodes[0], nAllNodes*sizeof(QTNode));

	fp->read((char*)&nAllNodes, sizeof(nAllNodes));
	m_vecNbIndex.resize(nAllNodes);
	fp->read(&m_vecNbIndex[0], nAllNodes*sizeof(int));

	fp->read((char*)&nAllNodes, sizeof(nAllNodes));
	m_vecNbMagicIndex.resize(nAllNodes);
	fp->read(&m_vecNbMagicIndex[0], nAllNodes*sizeof(int));

	delete fp;
	return true;
}

int CQuadTree::MapPixelToQtnode(int x, int y)
{
	//这里假设x y一定在地图范围内部
	int i;
	for(i=0; i<m_nTopCells; ++i)
	{
		QTNode* pQtNode = &m_vecAllNodes[i];
		int ltx = pQtNode->px - pQtNode->cellSize/2;
		int lty = pQtNode->py - pQtNode->cellSize/2;
		int rdx = pQtNode->px + pQtNode->cellSize/2;
		int rdy = pQtNode->py + pQtNode->cellSize/2;
		if(x>=ltx && x<rdx && y >= lty && y < rdy)
			break;
	}
	return MapPixelToQtnode(i, x, y);
}

int CQuadTree::MapPixelToQtnode(int nodeIndex, int x, int y)
{
	QTNode* pQtNode = &m_vecAllNodes[nodeIndex];
	QTNode* pChild = NULL;
	if(pQtNode->bLeaf)
	{
		return nodeIndex;
	}
	else
	{
		int i;
		for(i=0; i<4; ++i)
		{
			pChild = &m_vecAllNodes[pQtNode->child[i]];
			int ltx = pChild->px - pChild->cellSize/2;
			int lty = pChild->py - pChild->cellSize/2;
			int rdx = pChild->px + pChild->cellSize/2;
			int rdy = pChild->py + pChild->cellSize/2;
			if(x>=ltx && x<rdx && y >= lty && y < rdy)
				break;
		}
		return MapPixelToQtnode(pChild->ni, x, y);
	}
}

void CQuadTree::UpdateSearchNodes(const unsigned char* maskMap, int sx, int sy, int sc)
{
	Create(maskMap, sx, sy, sc);
	//_beginthreadex(NULL, 0, LoadMainFunc, this, 0, NULL);
}