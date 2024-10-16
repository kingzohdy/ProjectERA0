
#include "stdafx.h"
#include "RotateCoord.h"
#include "EditorManager.h"

CRotCood::CRotCood(void)
{
	m_fHrot = 0.0f;
	m_fVrot = 0.0f;
	m_fLen = 100.0f;
	//LineVertex lv;
	//LineVertex lastLv;
	//for(int i = 0; i < 100; i+=2)
	//{
	//	float angle = 360.0f / 50.0f;
	//	lv.pos.x = Ogre::Cos(angle);

	//}
}

CRotCood::~CRotCood(void)
{
}

void CRotCood::update(Ogre::uint dtick)
{
	RenderLines::update(dtick);

	Ogre::Matrix4 mVrot;
	Ogre::Matrix4 mHrot;
	mVrot.makeRotateZ(m_fVrot);
	mHrot.makeRotateY(-m_fHrot);

	LineVertex lv;
	lv.pos = Ogre::Vector3(0.0f, 0.0f, 0.0f);
	lv.color = Ogre::ColorQuad(255, 0, 0, 255);
	LineVertex lastLv = lv;
	lastLv.pos = Ogre::Vector3(m_fLen, 0.0f, 0.0f);

	LineVertex pLine[300];

	for(int i = 0; i < 102; i+=2)
	{
		float angle = i / 100.0f * 360.0f   ;
		lv.pos.x = Ogre::Cos(angle) * m_fLen;
		lv.pos.y = Ogre::Sin(angle) * m_fLen;

		pLine[i] = lastLv;
		pLine[i+1] = lv;

		pLine[i].pos = pLine[i].pos * mHrot;
		pLine[i+1].pos = pLine[i+1].pos * mHrot;

		addLine(pLine[i].pos, pLine[i+1].pos, pLine[i].color);

		lastLv = lv;
	}

	lastLv.pos = Ogre::Vector3(m_fLen, 0.0f, 0.0f);
	lastLv.color = Ogre::ColorQuad(0, 255, 0, 255);
	lv.color = Ogre::ColorQuad(0, 255, 0, 255);
	for(int i = 102; i < 204; i+=2)
	{
		float angle = (i - 102) / 100.0f * 360.0f;
		lv.pos.x = Ogre::Cos(angle) * m_fLen;
		lv.pos.z = Ogre::Sin(angle) * m_fLen;
		lv.pos.y = 0;

		pLine[i] = lastLv;
		pLine[i+1] = lv;

		pLine[i].pos = pLine[i].pos * mVrot *mHrot;
		pLine[i+1].pos = pLine[i+1].pos * mVrot * mHrot;

		addLine(pLine[i].pos, pLine[i+1].pos, pLine[i].color);

		lastLv = lv;
	}

	lv.color = Ogre::ColorQuad(0, 0, 255, 255);
	addLine(lv.pos, Ogre::Vector3(-200.0f, 0.0f, 0.0f), lv.color);

	addLine(lv.pos, Ogre::Vector3(200.0f, 0.0f, 0.0f), lv.color);
	addLine(lv.pos, Ogre::Vector3(0.0f, -200.0f, 0.0f), lv.color);
	addLine(lv.pos, Ogre::Vector3(0.0f, 200.0f, 0.0f), lv.color);
	addLine(lv.pos, Ogre::Vector3(0.0f, 0.0f, -200.0f), lv.color);
	addLine(lv.pos, Ogre::Vector3(0.0f, 0.0f, 200.0f), lv.color);

	lv.color = Ogre::ColorQuad(255, 255, 255, 255);
	Ogre::Vector3 pos(m_fLen, 0.0f, 0.0f);
	pos = pos * mVrot * mHrot;

	addLine(Ogre::Vector3(0.0f, 0.0f, 0.0f), pos, lv.color);
}
