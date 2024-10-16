#include "GameActor_Attr.h"

void RoleAttr::setPkValue( int nPkValue )
{
	m_nPkValue = nPkValue;
}

void RoleAttr::setType( int type )
{
	m_Type = type;
}

void RoleAttr::setDesignationID( int nDesignationID )
{
	m_nDesignationID = nDesignationID;
}

void RoleAttr::setRelationType( int nRelationType )
{
	m_nRelation = nRelationType;
}

void RoleAttr::setObjCamp( int nObjCamp )
{
	m_ObjCamp = nObjCamp;
}

void RoleAttr::setHP(int hp)
{
	if( m_nHpRef > 0 )
	{
		m_nHpRef--;
	}
	CSATTRIDVAL val;
	if( m_nHpRef == 0 )
	{
		val.HP = m_nTmpHp;
	}else{
		val.HP = hp > 0 ? hp : 0;
	}

	setAttriValue( ATTR_ID_HP, val);
}

void RoleAttr::setMaxHP(int hp)
{
	CSATTRIDVAL val;
	val.HPMax = hp;
	setAttriValue( ATTR_ID_HPMAX, val);
}

short RoleAttr::getSignedShortAttriValule(int attriId) const
{
	return ((short)getShortAttriValule(attriId));
}
