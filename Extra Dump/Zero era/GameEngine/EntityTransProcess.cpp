
#include "EntityTransProcess.h"
using namespace Ogre;
using namespace std;

EntityTransProcess::EntityTransProcess()
{
	m_fStepPerSecond = 0.8f;
}

EntityTransProcess::~EntityTransProcess()
{
	clear();
}

void EntityTransProcess::addEntity (Entity *pEntity)
{
	if (!pEntity)
		return;

	bool bIn = false;

	for (int i=0; i<(int)m_TransEntities.size(); i++)
	{
		TransEntity *pTransEntity = m_TransEntities[i];

		if (pTransEntity->pEntity == pEntity)
		{
			bIn = true;
			return;
		}
	}

	if (!bIn)
	{
		TransEntity *pTransEntity = new TransEntity();
		pTransEntity->pEntity = pEntity;
		pTransEntity->fOriginTransparent = pEntity->getTransparent();
		pEntity->setTransparent(0.1f);

		m_TransEntities.push_back(pTransEntity);
	}
}

void EntityTransProcess::removeEntity (Entity *pEntity)
{
	if (!pEntity)
		return;

	vector<TransEntity*>::iterator it = m_TransEntities.begin();
	for (it; it!=m_TransEntities.end(); it++)
	{
		if ((*it)->pEntity == pEntity)
		{
			delete (*it);
			m_TransEntities.erase(it);
			
			return;
		}
	}
}

void EntityTransProcess::clear ()
{
	vector<TransEntity*>::iterator it = m_TransEntities.begin();
	for (it; it!=m_TransEntities.end(); it++)
	{
		delete (*it);
	}

	m_TransEntities.clear();
}

void EntityTransProcess::update (unsigned int tick)
{
	float fStep = (tick/1000.0f)*m_fStepPerSecond;

	// 渐变透明度
	for (int i=0; i<(int)m_TransEntities.size(); i++)
	{
		if (m_TransEntities[i])
		{
			float fTransparent = m_TransEntities[i]->pEntity->getTransparent();
			float fOriginTransParent = m_TransEntities[i]->fOriginTransparent;

			fTransparent += fStep;

			if (fTransparent <= fOriginTransParent)
			{
				m_TransEntities[i]->pEntity->setTransparent(fTransparent);
			}
			else
			{
				m_TransEntities[i]->pEntity->setTransparent(fOriginTransParent);
			}
		}
	}

	// 移除无需再渐变的
	vector<TransEntity*>::iterator it = m_TransEntities.begin();
	for (it; it!=m_TransEntities.end();)
	{
		float fTransparent = (*it)->pEntity->getTransparent();
		float fOriginTransParent = (*it)->fOriginTransparent;

		if (fTransparent == fOriginTransParent)
		{
			delete (*it);
			it = m_TransEntities.erase(it);
		}
		else
		{
			it++;
		}
	}
}