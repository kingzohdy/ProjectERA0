
#pragma once

#include <vector>

#include "mpath.h"

namespace TE
{
//Բ�Ķ�����
#define  ROUND_MAX_NUM   64
//һ��Բ��ʵ�ֶ�Ӧ��λ��.
class MRound
{
public:
	MRound(){ memset( m_RoundPosSet , 0 , sizeof(MVec2)*ROUND_MAX_NUM ); m_nCurPosNum = 0 ;};
	~MRound(){};

	void  SetDivideNum( int num ) ;
public:
	MVec2       m_RoundPosSet[ROUND_MAX_NUM];   
	int         m_nCurPosNum;
};

//һ�����
class MSign
{
public:
	MSign(void);
	~MSign(void);

	void Render();
	void SetPos(MVec3 pos);

	void SetColor(unsigned int color){m_nColor = color;if( m_pPath ) m_pPath->SetColor ( color );}
public:
	MPath    *m_pPath;
	unsigned  int  m_nColor;
};

//һ����ǵļ���
class MSignSet
{
public:
	MSignSet();
	~MSignSet();

	void  Release();
    void  Render();

	void  UpdateData();
	//ͨ�����ĵ�Ͱ뾶�����»�Բ
	void  UpdateData(MVec3 pos,float rad , int numRound);

	void  PushData(MVec3 pos);
	void  ReSetPos(){ m_nCurPos = 0;}
 
	void  SetSignSize( int size ){ m_PosSet.resize ( size );};
	void  SetSignNum( int num ) { m_nNumPos = num; }
public:
	int                    m_nCurPos;
	int                    m_nNumPos;

	std::vector <MVec3>    m_PosSet;
	MSign                  *m_pSign;

	int                     m_nColorIndex;
	
	MRound                  m_Round;

};

};