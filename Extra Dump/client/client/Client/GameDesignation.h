#pragma once
#include "NetMessage.h"
#include "cs_net.h"
#include <map>
#include <vector>

//tolua_begin 
struct tagDesignationDef_ToLua
{
	unsigned int ID;						/*   称号ID */
	char Name[SHORT_DESC_LEN];            	/*   称号文本 */
	char CategoryName[SHORT_DESC_LEN];    	/*   称号类别 */
	unsigned int StatusID;					/*   状态ID */
	unsigned int StatusLevel;				/*   状态等级 */
	int Time;                            	/*   状态持续时间 */
    char DescText[SHORT_DESC_LEN];        	/*   对此称号的描述 */
    int VimCost;                         	/*  Ver.224 消耗的精力值 */
	int DesTime;                         	/*  Ver.1523 称号存活时间0-永久,单位:秒 */
	char Level;                           	/*  Ver.1524 称号品质 */
	char NotShow;                         	/*  Ver.1525 1-没有获取此称号时在面板中不显示 */
	unsigned int RGB;                       /*  Ver.1527  */
	char EffectPath[DESIG_EFFECT_LEN];    	/*  Ver.1527  */
};
//tolua_end 

typedef std::map<std::string,std::vector<tagDesignationDef*>>	MAPDESIGNATIONDEF;
typedef std::vector<tagDesignationDef*>							VECTDESIGNATIONDEF;
typedef std::vector<DESIGNATIONENTRY>							VECTHAVEDESIGNATION;

class GameDesignation//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameDesignation>
{//tolua_export
public:
	GameDesignation(void);
	~GameDesignation(void);
	
	//tolua_begin 
	// 向服务器发送设置称号请求
	void sendSetDesignationReq( int nId );
	//tolua_end
	
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

public:
	//tolua_begin 
	// 获取全部的称号总数
	int getDesignationNum();
	// 按类型获取称号的类型总数
	int getDesignationNum( std::string strCategoryName );
	// 获取当前使用的称号ID
	int getCurDesignationId(){ return m_nCurDesignationId; }
	// 获取称号
	tagDesignationDef_ToLua* getDesignation( int nIndex );
	// 获取称号
    tagDesignationDef_ToLua* getDesignation( std::string strCategory, int nIndex );
    // 根据称号ID获取称号信息
    tagDesignationDef_ToLua* getDesignationById( int nId );
	// 获取称号状态名称
	const char* getDesignationBufName( int nBufId, int nBufLv );

	// 是否当日首次设置称号
	bool isFirstSetDesignation( int nId );
	// 是否已拥有的称号
	bool isHaveDesignation( int nId  );
	// 获取称号到期时间
	int getDesignationEndTime( int nId );

	int GetDesignationMapSize();

	const char* GetDesignationCateName( int idx );
	//tolua_end 

public:
	void setActorDesignation( int nDesignationId, GameActor* pActor );
	void sortDesignation( VECTDESIGNATIONDEF& vectDesignationDef );
	void sortAllDesignation();

	// 是否当天时间
	bool isToday( int nTime );

private:
	static bool compareDesignations( const tagDesignationDef* lhs, const tagDesignationDef* rhs );

private:
	MAPDESIGNATIONDEF				m_mapDesignationDef;	// 分类存放的称号数据
	VECTDESIGNATIONDEF				m_vectDesignationDef;	// 全部的称号数据
	VECTHAVEDESIGNATION				m_vectHaveDesignation;  // 已拥有的称号
	int                             m_nCurDesignationId;    // 玩家当前称号ID
};//tolua_export
