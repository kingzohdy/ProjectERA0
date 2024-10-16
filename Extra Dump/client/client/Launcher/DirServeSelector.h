#ifndef _DIR_SERVE_SELECTOR
#define _DIR_SERVE_SELECTOR

#include <vector>
#include <string>
#include "OgreLocker.h"
#include "Ping.h"

struct LauncherDirWorldInfo
{
	char			name[32];
	bool			recommend;
	int				busy; //tagDirBusyDegree
	char			ip[48];
	unsigned short	port;
	unsigned short	nWorldID; 
};

class LauncherDirBigWorldInfo
{
public:
	LauncherDirBigWorldInfo();

	const char *getName();
	int getNumWorld();
	LauncherDirWorldInfo& getIthWorld(int i);

	void addWorldInfo(const CSDIRWORLDINFO &srcinfo);
	void addWorldInfo( const LauncherDirWorldInfo& src );
	void updateWorldInfo( const CSDIRLATESTINFO& src ); 

	char			m_Name[64];
	std::string		m_strUrl;
	unsigned int	m_uStatusFlag;
private:
	std::vector<LauncherDirWorldInfo>	m_Worlds;
};

struct DirServerData
{
	std::string ip;
	int port;
};

class DirServeSelector:public IPingSink
{
public:
	DirServeSelector(void);
	~DirServeSelector( void );

	void initDirInfo(const CSDIRWORLDLIST& dirinfo);

	bool requestDirInfo();
	
	bool LoadLocalSvr();
 	void SaveLocalSvr();

	bool tryRequestDirInfo();
	const char* getBestUrl();

	bool UpdateBestUrl();

	std::string getDirServer( int nIdx );
	int getDirServerIdx( std::string dirServer );
	bool tryLoadDirFromDirSever();
	void dispatchDirNetCL( NETCLTHANDLE *pnetcl, int nIdx );
	void HandleDirServerMsg( int msgid, const tagCSPkgBody& msgbody, int nIdx );
	void onTime();

	virtual void OnResult(LPCSTR szHost, DWORD dwIntervel);
public:
	std::vector<LauncherDirBigWorldInfo>	m_BigWorlds;
	std::string								m_strBestUrl;
	unsigned int							m_nBestPing;

private:
	std::vector<std::string>				m_vecUrl;
	LockSection m_Mutex;
	std::vector<CPing*>						m_vecPing;
	std::vector<NETCLTHANDLE*>				m_vecNetCltHandle;
};
#endif