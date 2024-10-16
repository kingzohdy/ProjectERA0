
#pragma once
#include <queue>
#include "OgrePrerequisites.h"

class CChunkDetailBar;
class CChunkDetail;

class CChunkAnsycLoaderReceiver
{
public:
	virtual void OnReceiveAnsyncChunk(CChunkDetail* pReceivedChunk, DWORD data) = 0;
};

struct CChunkAnsycLoaderWorkData
{
	CString strChunkName;
	CString strChunkExt;
	DWORD	data;
};
class CChunkAnsycLoader
{
public:
	void Start();
	void End();

	void AddNewWork(CString strAddr, CString strExt, DWORD data);
	void ClearWork();
	void SetNotifyTarget(CChunkAnsycLoaderReceiver* pReceiver);
	int	 GetWorkCount();

	static DWORD WorkFun (void* pParam);

	static void SetStateWord(CString str);
	bool m_bStop;
	std::queue<CChunkAnsycLoaderWorkData> m_strWorking;
	CChunkAnsycLoaderReceiver* m_pReceiver;
	HANDLE	m_hMutex;
	HANDLE  m_hThread;
};


class CChunkDetail
{
public:
	CChunkDetail(void);
	virtual ~CChunkDetail(void);

	virtual void		LoadChunkData(Ogre::Resource* pChunk);
	virtual CBitmap*	GetIcon();
	virtual void		SetChunkName(CString strChunkName) { m_strChunkName = strChunkName; };

	CString				GetChunkName()	{ return m_strChunkName; };

	virtual CChunkDetailBar*	CreateBar(CWnd* pParent) = 0;
	CString		m_strChunkName;


	static CChunkDetail* CreateChunkDetailFromChunk(CString strChunk, CString ext);
	static CChunkDetail* CreateDefaultChunkDetail(CString strChunk);
};

class CChunkDetailDefault : public CChunkDetail
{
public:
	CChunkDetailDefault();
	virtual ~CChunkDetailDefault();

	//virtual void LoadChunkData(Ogre::Resource* pChunk);
	virtual CChunkDetailBar*	CreateBar(CWnd* pParent);
};

class CChunkDetailTexture : public CChunkDetail
{
public:
	CChunkDetailTexture();
	virtual ~CChunkDetailTexture();

	virtual void		LoadChunkData(Ogre::Resource* pChunk);
	virtual CBitmap*	GetIcon();
	virtual CChunkDetailBar*	CreateBar(CWnd* pParent);

	IPicture *LoadPictureFromBuffer(char* pSrcBuffer, int iBufLen);
	HBITMAP DrawPictureOnBitmap(IPicture* pPic);

	CBitmap*	m_pBitmap;
	int		m_iWidth;
	int		m_iHeight;
};



//class CChunkDetailModel : public CChunkDetail
//{
//public:
//	CChunkDetailModel();
//	virtual ~CChunkDetailModel();
//};
//
class CChunkDetailShowModel : public CChunkDetail
{
public:
	CChunkDetailShowModel();
	virtual ~CChunkDetailShowModel();


	virtual void				LoadChunkData(Ogre::Resource* pChunk);
	virtual CChunkDetailBar*	CreateBar(CWnd* pParent);
	std::vector<CString>		m_MotionNames;
};


class CChunkDetailMotion : public CChunkDetail
{
public:
	virtual ~CChunkDetailMotion(){};


	virtual void				LoadChunkData(Ogre::Resource* pChunk);
	virtual CChunkDetailBar*	CreateBar(CWnd* pParent);

};


class CChunkDetailPreviewModel : public CChunkDetail
{
public:
	CChunkDetailPreviewModel() : m_pNode(NULL)
	{};

	virtual ~CChunkDetailPreviewModel();

	virtual void				LoadChunkData(Ogre::Resource* pChunk);
	virtual CChunkDetailBar*	CreateBar(CWnd* pParent);

	Ogre::MovableObject *GetPreviewObject()
	{
		return m_pNode;
	}

protected:
	Ogre::MovableObject *m_pNode;
};