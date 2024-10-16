#pragma once

#include "OgreSingleton.h"
#include <map>
#include <Windows.h>

enum eConfigType
{
	CONFIGTYPE_CHAT,			//聊天快捷发言
	CONFIGTYPE_SHOW_DAMAGE,		//是否显示伤害统计
	CONFIGTYPE_KEYMAP,			//快捷键设置
	eCONFIGMAX,
};
#define MD5_DIGEST_LENGTH	16

struct ConFigFileHead 
{
	char		szMD5[MD5_DIGEST_LENGTH];	//md5 值
	unsigned int		headSize;	// 文件头大小
	unsigned int		configNum;	// 保存的配置数量
	unsigned int		dataOffset;	// 配置数据偏移位置
	DWORD		reverse1;	//保留
	DWORD		reverse2;	//保留
};

struct ConfigHead
{
	eConfigType				type;			//配置类型
	unsigned int			start_offset;	//在配置数据中的偏移
	unsigned int			dataLength;		//配置文件大小
};

struct ConfigData
{
	ConfigHead head;
	char* data;
};

//tolua_begin
enum
{
	CLIENT_SET_PET_FIRST_MIN_HP = 0,
	CLIENT_SET_PET_SECOND_MIN_HP,
	CLIENT_SET_PET_MIN_ZHONG_CHENG,
	CLIENT_SET_PET_USE_PET_ITEM_MAX,
};

struct tagOnePetUseConfig
{
	bool			m_bOpen;
	unsigned int	m_uMinValue;
	int				m_uItemID;
};

struct tagPetUseItemConfig 
{
	tagOnePetUseConfig	m_petUseItemConfigs[CLIENT_SET_PET_USE_PET_ITEM_MAX];
};

struct tagLastSendMailConfig 
{
	int	m_uLastSendMailTime;
};
//tolua_end

typedef std::map<eConfigType,ConfigData>	CONFIGMAP;
typedef std::map<eConfigType,ConfigData>::iterator CONFIGMAPIT;

class GameUserConfig	//tolua_export
	:public Ogre::Singleton<GameUserConfig>
{//tolua_export
public:

	void loadData( const char* path,CONFIGMAP& dataMap );
	void writeData( const char* path,CONFIGMAP& dataMap );

	// 设置配置数据,会覆盖相同type的数据
	void	setConfigData(eConfigType type,const char* data,unsigned int nLen);
	//获取配置数据
 	void*	getConfigData(eConfigType type);
	void	release();

	//加载用户配置
	void	loadData();
	//写用户配置
	void	writeData();

	void*	getGlobalData(eConfigType type);
	void	setGlobalData(eConfigType type,char* data,unsigned int nLen);

	
	GameUserConfig();
	~GameUserConfig();

	//tolua_begin
	void	setDamageConfigData(int value);
	int		getDamageConfigData();
	tagPetUseItemConfig* getPetUseItemConfig();
	tagOnePetUseConfig& getOnePetConfig( int nIndex );
	void setOnePetConfig( int nIndex, bool bOpen, unsigned int uMinValue, int uItemID );
	void savePetUseItemConfig();
	
	void setLastCommitMailTime( int uTime );
	tagLastSendMailConfig* getLastCommitMailTime();
	void saveLastCommitMailTime( );

	const char *	loadKeyMap();
	void	storeKeyMap(const char *data);
	//tolua_end
protected:
private:	
	CONFIGMAP	m_ConfigDataMap;
	bool		m_bLoaded;

	CONFIGMAP	m_GlobalDataMap;
	tagPetUseItemConfig	m_petUseItemConfig;
	tagLastSendMailConfig	m_LastCommitMailConfig;
};//tolua_export