
#include "EditorModel.h"

using namespace Ogre;

EditorModel::EditorModel() 
{
	m_pModel = NULL;
	m_Dir = Ogre::Vector3(0, 0, 1.0f);
	m_Scale = 1.0f;
}

EditorModel::~EditorModel()
{
	Release();
}

void  EditorModel::Release()
{
	OGRE_RELEASE(m_pModel);	
}

bool EditorModel::LoadModel(const char* filename  )
{
	std::string  filenameex = filename;

	m_pModel = new Entity;
	if (!m_pModel->load(filename, true))
	{
		m_pModel->release();
		m_pModel = NULL;
		return false;
	}

	m_pModel->playAnim(0);
	m_pModel->update(0);

	return true;
};	

void  EditorModel::Update( float time )
{
	if( m_pModel )
	{
		m_pModel->update(TimeToTick(time));
	}
};

void EditorModel::UpdatePos()
{
	Ogre::Vector3  scale;
	scale.x = scale.y = scale.z = m_Scale ;

	if (m_pModel)
	{
		m_pModel->setPosition (m_wPos);
		m_pModel->setScale(scale);
		m_pModel->setRotation(m_Dir.x, m_Dir.y, m_Dir.z);
	}
}

void EditorModel::SetPos(Ogre::Vector3 pos)
{
	m_wPos = pos ;

	if(m_pModel)
	{
		m_pModel->setPosition(m_wPos);
	}
};

void EditorModel::SetDir (Ogre::Vector3 dir)
{
	m_Dir = dir;

	if (m_pModel)
	{
		m_pModel->setRotation(dir.x , dir.y , dir.z);
	}

};

void EditorModel::SetScale (float scale)
{
	m_Scale = scale;

	Ogre::Vector3  vScale;
	vScale.x = vScale.y = vScale.z = m_Scale ;

	if (m_pModel)
	{
		m_pModel->setScale(vScale);
	}
};