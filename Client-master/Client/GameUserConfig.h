#pragma once

#include "OgreSingleton.h"
#include <map>
#include <Windows.h>

enum eConfigType
{
	CONFIGTYPE_CHAT,			//�����ݷ���
	CONFIGTYPE_SHOW_DAMAGE,		//�Ƿ���ʾ�˺�ͳ��
	CONFIGTYPE_KEYMAP,			//��ݼ�����
	eCONFIGMAX,
};
#define MD5_DIGEST_LENGTH	16

struct ConFigFileHead 
{
	char		szMD5[MD5_DIGEST_LENGTH];	//md5 ֵ
	unsigned int		headSize;	// �ļ�ͷ��С
	unsigned int		configNum;	// �������������
	unsigned int		dataOffset;	// ��������ƫ��λ��
	DWORD		reverse1;	//����
	DWORD		reverse2;	//����
};

struct ConfigHead
{
	eConfigType				type;			//��������
	unsigned int			start_offset;	//�����������е�ƫ��
	unsigned int			dataLength;		//�����ļ���С
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

	// ������������,�Ḳ����ͬtype������
	void	setConfigData(eConfigType type,const char* data,unsigned int nLen);
	//��ȡ��������
 	void*	getConfigData(eConfigType type);
	void	release();

	//�����û�����
	void	loadData();
	//д�û�����
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