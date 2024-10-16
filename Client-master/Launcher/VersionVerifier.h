#pragma once


struct tagVersionInfo
{
	tagVersionInfo():m_nIsUp(0),
					m_nVersionFrom(0),
					m_nVersionTo(0),
					m_nUpSize(0),
					m_strUpName(""),
					m_strMD5(""),
					m_strUpAddress(""),
					m_nVersionLatest(0)
	{}
	int				m_nIsUp;      
	unsigned int	m_nVersionFrom;
	unsigned int	m_nVersionTo;                       	
	__int64			m_nUpSize;                          
	string			m_strUpName;              
	string			m_strMD5;
	string			m_strUpAddress;
	unsigned int	m_nVersionLatest;	//���°汾��
};

// �������ڱ�������������Ĺ��ڿͻ��˺ͷ����2�߰汾�Ƿ�һ�µ���Ϣ
class VersionVerifier
{
public:
	VersionVerifier(void);
	
	unsigned int getLocalVersion();

	bool		requestVersionInfo( );
	int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	void OnGetVersionInfo();


	~VersionVerifier(void);

	const tagVersionInfo& getLatestVersionInfo() const;

	unsigned int GetFileVersion(const char* szFileName);

	void tryRequestVersionInfo();

	bool tryDownloadVersionInfo();

	void OnTime();

	tagVersionInfo		m_versionInfo;

	NETCLTHANDLE*		m_hNetCltHandle;
};