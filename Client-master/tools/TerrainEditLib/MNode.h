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
   int     m_NodeID;//KKXXYYRRTT kk-��ʾ�ڵ������;xx,yy-��ʾtitle�����ꣻrr,tt��ʾtitle�ڲ�������;

};

};