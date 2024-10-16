#include "GameUserConfig.h"
#include "GameLoginManager.h"
#include "md5.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameClientOpt.h"

#define GLOBAL_PATH	"users\\global"


GameUserConfig::GameUserConfig():m_bLoaded(false)
{
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_PET_AUTO_USE_ITEM, sizeof( tagPetUseItemConfig ) );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_LAST_COMMIT_MAIL_TIME, sizeof( tagLastSendMailConfig ) );
}

GameUserConfig::~GameUserConfig()
{
	release();
}

void GameUserConfig::release()
{
	CONFIGMAPIT it = m_ConfigDataMap.begin();

	for (; it!= m_ConfigDataMap.end();it ++ )
	{
		if(it->second.data != NULL )
		{
			delete[] (it->second.data);
			it->second.data = NULL;
		}
	}
	m_ConfigDataMap.clear();
	
	it = m_GlobalDataMap.begin();

	for (; it!= m_GlobalDataMap.end();it ++ )
	{
		if(it->second.data != NULL )
		{
			delete[] (it->second.data);
			it->second.data = NULL;
		}
	}
	m_GlobalDataMap.clear();

	m_bLoaded = false;
}

void GameUserConfig::loadData(const char* path,CONFIGMAP& dataMap)
{
	FILE* f = fopen(path,"rb");
	if (f == NULL )
	{
		return ;
	}
	fseek(f,0,SEEK_END);
	size_t size = ftell(f);

	fseek(f,0,SEEK_SET);
	ConFigFileHead fileHead;
	if (1 != fread(&fileHead,sizeof(ConFigFileHead),1,f) )
	{
		fclose(f);
		return;
	}
	if( fileHead.headSize > size )
	{
		fclose(f);
		::DeleteFile(path);
		return;
	}
	size_t dataSize = size - fileHead.headSize;
	char* buf = new char[dataSize];
	fseek(f,(long)fileHead.dataOffset,SEEK_SET);
	if( 1 != fread(buf,dataSize,1,f) )
	{
		fclose(f);
		delete[] buf;
		return;
	}
	char szMD5[MD5_DIGEST_LENGTH];
	Md5String(szMD5,buf,(int)dataSize);
	if( memcmp(szMD5,fileHead.szMD5,MD5_DIGEST_LENGTH) != 0 )
	{
		fclose(f);
		delete[] buf;
		return;
	}
	size_t nOffset = 0;
	for( size_t i = 0 ; i < fileHead.configNum ; i ++)
	{
		ConfigData data;
		memcpy(&data.head,buf+nOffset,sizeof(ConfigHead));
		nOffset += sizeof(ConfigHead);
		char* databuf = new char[data.head.dataLength];
		memcpy(databuf,buf+nOffset,data.head.dataLength);
		data.data = databuf;
		nOffset += data.head.dataLength;
		dataMap[data.head.type] = data;
	}
	delete[] buf;
	fclose(f);
}

void GameUserConfig::loadData()
{
	MainGamePlayer* mainplayer = ActorManager::getSingleton().getMainPlayer();
	if( mainplayer == NULL)
	{
		return;
	}
	release();
	m_bLoaded = true;
	char path[MAX_PATH];
	sprintf(path,"users\\%s_%s",LoginManager::getSingleton().getUserName(),mainplayer->getName());
	loadData( path,m_ConfigDataMap );
	
	loadData( GLOBAL_PATH,m_GlobalDataMap );
}

void GameUserConfig::writeData( const char* path,CONFIGMAP& dataMap )
{
	if( dataMap.size() == 0  )
	{
		return;
	}
	FILE* f = fopen(path,"wb");
	if( f == NULL )
	{
		return;
	}
	ConFigFileHead fileHead;
	fileHead.headSize = sizeof(ConFigFileHead);
	fileHead.configNum = (unsigned int)dataMap.size();
	fileHead.dataOffset = fileHead.headSize;

	CONFIGMAPIT it = dataMap.begin();
	unsigned int offset = 0;
	for (; it != dataMap.end() ; it ++ )
	{
		it->second.head.start_offset = offset + sizeof(ConfigHead) ;
		fseek(f,fileHead.dataOffset + offset,SEEK_SET);
		if( 1 != fwrite(&(it->second.head),sizeof(ConfigHead),1,f) )
		{
			fclose(f);
			return;
		}
		offset += sizeof(ConfigHead);
		fseek(f,fileHead.dataOffset + offset,SEEK_SET);
		if( 1 != fwrite(it->second.data,it->second.head.dataLength,1,f) )
		{
			fclose(f);
			return;
		}
		offset += it->second.head.dataLength;
	}
	MD5_CTX md_ctx;
	char md5[MD5_DIGEST_LENGTH];
	MD5Init(&md_ctx);
	it = dataMap.begin();
	for (; it != dataMap.end() ; it ++ )
	{
		MD5Update(&md_ctx,(unsigned char*)(&(it->second.head)),sizeof(ConfigHead));
		MD5Update(&md_ctx,(unsigned char*)it->second.data,it->second.head.dataLength);
	}
	MD5Final((unsigned char*)md5,&md_ctx);

	memcpy(fileHead.szMD5,md5,MD5_DIGEST_LENGTH);
	fseek(f,0,SEEK_SET);
	if( 1 != fwrite(&fileHead,sizeof(ConFigFileHead),1,f) )
	{
		fclose(f);
		return;
	}
	fclose(f);
}

void GameUserConfig::writeData()
{
	if( !m_bLoaded )
	{
		return;
	}
	MainGamePlayer* mainplayer = ActorManager::getSingleton().getMainPlayer();
	if(mainplayer == NULL)
	{
		return;
	}
	//user Ŀ¼
	CreateDirectory("users",NULL);
	char path[MAX_PATH];
	sprintf(path,"users\\%s_%s",LoginManager::getSingleton().getUserName(),mainplayer->getName());
	writeData( path,m_ConfigDataMap );
	writeData( GLOBAL_PATH,m_GlobalDataMap );
}

void* GameUserConfig::getConfigData(eConfigType type)
{
	CONFIGMAPIT it = m_ConfigDataMap.find(type);
	if( it != m_ConfigDataMap.end() )
	{
		return it->second.data;
	}
	return NULL;
}

void GameUserConfig::setConfigData(eConfigType type, const char* data,unsigned int nLen)
{
	CONFIGMAPIT it = m_ConfigDataMap.find(type);
	if (it != m_ConfigDataMap.end() )
	{
		if( nLen > it->second.head.dataLength )
		{
			char* buf = new char[nLen];			
			memcpy(buf,data,nLen);
			char* old = it->second.data;
			it->second.data = buf;
			it->second.head.dataLength = nLen;
			delete[] old;
		}else{
			memcpy(it->second.data,data,nLen);
			it->second.head.dataLength = nLen;
		}
	}else{
		char* buf = new char[nLen];		
		memcpy(buf,data,nLen);
		ConfigData configData;
		configData.head.type = type;
		configData.head.dataLength = nLen;
		configData.head.start_offset = 0 ;
		configData.data = buf;
		m_ConfigDataMap[type] = configData;
	}
}

void* GameUserConfig::getGlobalData(eConfigType type)
{
	CONFIGMAPIT it = m_GlobalDataMap.find(type);
	if( it != m_GlobalDataMap.end() )
	{
		return it->second.data;
	}
	return NULL;
}
void GameUserConfig::setGlobalData(eConfigType type,char* data,unsigned int nLen)
{
	CONFIGMAPIT it = m_GlobalDataMap.find(type);
	if (it != m_GlobalDataMap.end() )
	{
		if( nLen > it->second.head.dataLength )
		{
			char* buf = new char[nLen];
			memcpy(buf,data,nLen);
			char* old = it->second.data;
			it->second.data = buf;
			it->second.head.dataLength = nLen;
			delete[] old;
		}else{
			memcpy(it->second.data,data,nLen);
			it->second.head.dataLength = nLen;
		}
	}else{
		char* buf = new char[nLen];		
		memcpy(buf,data,nLen);
		ConfigData configData;
		configData.head.type = type;
		configData.head.dataLength = nLen;
		configData.head.start_offset = 0 ;
		configData.data = buf;
		m_GlobalDataMap[type] = configData;
	}
}


void GameUserConfig::setDamageConfigData(int value)
{
	setGlobalData(CONFIGTYPE_SHOW_DAMAGE,(char*)&value,sizeof(int));
}

int GameUserConfig::getDamageConfigData()
{
	void* data = getGlobalData( CONFIGTYPE_SHOW_DAMAGE );
	if( data == NULL )
	{
		return 0;
	}
	return *(int*)data;
}

tagPetUseItemConfig* GameUserConfig::getPetUseItemConfig()
{
	return &m_petUseItemConfig;
}


tagOnePetUseConfig& GameUserConfig::getOnePetConfig( int nIndex )
{
	return m_petUseItemConfig.m_petUseItemConfigs[nIndex];
}

void GameUserConfig::setOnePetConfig( int nIndex, bool bOpen, unsigned int uMinValue, int uItemID )
{
	m_petUseItemConfig.m_petUseItemConfigs[nIndex].m_bOpen		= bOpen;
	m_petUseItemConfig.m_petUseItemConfigs[nIndex].m_uMinValue	= uMinValue;
	m_petUseItemConfig.m_petUseItemConfigs[nIndex].m_uItemID	= uItemID;
}

void GameUserConfig::savePetUseItemConfig( )
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_PET_AUTO_USE_ITEM, (char*)&m_petUseItemConfig );
}

void GameUserConfig::setLastCommitMailTime( int uTime )
{
	m_LastCommitMailConfig.m_uLastSendMailTime = uTime;
}

tagLastSendMailConfig* GameUserConfig::getLastCommitMailTime()
{
	return &m_LastCommitMailConfig;
}

void GameUserConfig::saveLastCommitMailTime()
{
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_LAST_COMMIT_MAIL_TIME, (char*)&m_LastCommitMailConfig );
}

const char * GameUserConfig::loadKeyMap()
{
	return (char *)getConfigData(CONFIGTYPE_KEYMAP);
}

void GameUserConfig::storeKeyMap(const char *data)
{
	setConfigData(CONFIGTYPE_KEYMAP, data, strlen(data) + 1);
	writeData();
}
