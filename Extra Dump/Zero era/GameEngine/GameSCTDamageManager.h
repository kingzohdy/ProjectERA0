#pragma once

#include <map>
#include <set>
#include "GameSCTDamage.h"
#include "OgreSingleton.h"

class SCTDamageManager : public Ogre::Singleton<SCTDamageManager>
{
public:
	SCTDamageManager();
	~SCTDamageManager();
	SCTDamageBase*	getActiveSCT( int id );
	SCTDamageBase*	getPassiveSCT( int id );
	void			addDamageActor( int nActorID );
	void			clearDamageActor( );
	void			setEntityAttr( int nHurtType, int nChgValue, int nActorID );

private:

	std::map<int, SCTDamageBase*>	m_activeSCT;	
	std::map<int, SCTDamageBase*>	m_passiveSCT;		
	std::set<int>					m_actorsShowDamage;
};