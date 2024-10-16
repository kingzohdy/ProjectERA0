
#pragma once

#include <vector>

#include "MapPath.h"

namespace TE
{
	//Բ�Ķ�����
#define  ROUND_MAX_NUM   64
	//һ��Բ��ʵ�ֶ�Ӧ��λ��.
	class MapMarkRound
	{
	public:
		MapMarkRound(){ memset( m_RoundPosSet , 0 , sizeof(Ogre::Vector2)*ROUND_MAX_NUM ); m_nCurPosNum = 0 ;};
		~MapMarkRound(){};

		void  SetDivideNum( int num ) ;
	public:
		Ogre::Vector2       m_RoundPosSet[ROUND_MAX_NUM];   
		int         m_nCurPosNum;
	};

	//һ�����
	class MapMark
	{
	public:
		MapMark(void);
		~MapMark(void);

		void Render();
		void SetPos(Ogre::Vector3 pos);

		void SetColor(unsigned int color){m_nColor = color;if( m_pPath ) m_pPath->SetColor ( color );}
	public:
		MapPath    *m_pPath;
		unsigned  int  m_nColor;
	};

	//һ����ǵļ���
	class MapMarkGroup
	{
	public:
		MapMarkGroup();
		~MapMarkGroup();

		void  Release();
		void  Render();

		void  UpdateData();
		//ͨ�����ĵ�Ͱ뾶�����»�Բ
		void  UpdateData(Ogre::Vector3 pos,float rad , int numRound);

		void  PushData(Ogre::Vector3 pos);
		void  ReSetPos(){ m_nCurPos = 0;}

		void  SetSignSize( int size ){ m_PosSet.resize ( size );};
		void  SetSignNum( int num ) { m_nNumPos = num; }
	public:
		int                    m_nCurPos;
		int                    m_nNumPos;

		std::vector <Ogre::Vector3>    m_PosSet;
		MapMark                  *m_pSign;

		int                     m_nColorIndex;

		MapMarkRound                  m_Round;

	};

};