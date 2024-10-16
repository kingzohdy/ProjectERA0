#pragma once

#include "TerrainEditTypes.h"

namespace TE
{

class MNode
{
public:
	MNode(void);
	~MNode(void);

	virtual bool Lock() {return true;};
	virtual bool Unlock() {return true;};
public:
   MBox    m_Box;
   MBall   m_Ball;  
   MMatrix   m_Mat;
   int     m_NodeID;//KKXXYYRRTT kk-表示节点的种类;xx,yy-表示title的坐标；rr,tt表示title内部的坐标;

};

};