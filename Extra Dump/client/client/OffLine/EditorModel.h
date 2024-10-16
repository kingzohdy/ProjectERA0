#pragma once

#include "OgreModel.h"
#include "OgreEntity.h"

class EditorModel
{
public:
	EditorModel ();
	virtual ~EditorModel ();

	virtual bool LoadModel (const char* filename);	

	virtual void SetPos(Ogre::Vector3 pos);
	virtual void SetDir(Ogre::Vector3 dir);
	virtual void SetScale(float scale);

	virtual void Release();

	void   SetShowModel( Ogre::Entity * pModel ){ m_pModel = pModel ; }
	Ogre::Entity*  GetShowModel(){ return m_pModel ; }

	void Update( float time );

	void UpdateNormal();

private:
	Ogre::Entity *m_pModel;

	Ogre::Vector3 m_wPos,m_wSavePos;	
	Ogre::Vector3 m_Dir;	
	float m_Scale;

private:
	void UpdatePos();

};