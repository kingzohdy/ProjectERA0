#ifndef __GAMEDIATHESIS_H__
#define __GAMEDIATHESIS_H__

#include <vector>
#include <map>
#include "GameEngineTypes.h"
#include "GameSkill_DefToLua.h"


//tolua_begin
const tagDiathesisDef_ToLua* getDiathesisDef( int id, int lv );
//tolua_end
struct tagGeniusSortInfo 
{
	int		m_nIndex;
	int		m_nRace;
	int		m_nId;
};

class DiathesisManager //tolua_export
	: public NetMsgHandler
{//tolua_export
public:
	DiathesisManager( void );
	~DiathesisManager( void );

public:
	//tolua_begin
	const tagDiathesisInfo_toLua& getDiathesisInfo() const;
	// 是否已经学习过该天赋
	bool isLearnedGenius( int nId, int nLv );
	int getLearnedGeniusLevel( int nId );
	const tagDiathesisDef_ToLua& getDiathesis( int nIndex );
	int getAllGeniusNum();

	int getLearnedDiathesisNum();
	const tagDiathesisData_toLua& getDiathesisData( int nIndex );
	//tolua_end
	
	virtual int	HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );	
	void setDiathesisInfo( const DIATHESISINFO& diathesInfo );
	void addDiathesisInfo( const DIATHESISDATA& newDiathesInfo );

	void ResetDiathesisInfo();
	
	// 将整张素质数据库表划分为素质表和天赋表
	void mergeDiathesisTable( );
	 
private:
	// 是否是天赋
	bool isGenius( const tagDiathesisDef& diathesis );
	
	tagDiathesisInfo_toLua					m_DiathesisInfo;
	// 包含所有素质表的定义
	std::vector<tagDiathesisDef>			m_DiathesisDefTable;
	std::vector<tagDiathesisDef_ToLua>		m_AllGeniusDef;
	std::vector<tagDiathesisDef_ToLua>		m_AllDiathesisDef;
};//tolua_export
#endif