
#include "stdafx.h"
#include "ResourceReader.h"
#include "EffectUnit.h"
#include "ResourceReaderBar_Default.h"
#include "ResourceReaderBar_Entity.h"
#include "ResourceReaderBar_Texture.h"
#include "ResourceReaderBar_Motion.h"
#include "ResourceReaderBar_PreviewModel.h"

#include "OgreResourceManager.h"
#include "OgreTexture.h"
#include "OgreEntityData.h"
#include "OgreModel.h"
#include "OgreHardwarePixelBuffer.h"

using namespace Ogre;

void CChunkAnsycLoader::Start()
{
	m_bStop = false;
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	DWORD dwOutId;

	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkFun, (void*)this, 0, &dwOutId);
	assert(m_hThread != NULL);

}

void CChunkAnsycLoader::End()
{
	TRACE("Begin Ending\n");
	m_bStop = true;
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	CloseHandle(m_hMutex);
	TRACE("Finish Ending\n");
}

void CChunkAnsycLoader::AddNewWork(CString strAddr, CString strExt, DWORD data)
{
	::WaitForSingleObject(m_hMutex, INFINITE);

	CChunkAnsycLoaderWorkData workData;
	workData.data = data;
	workData.strChunkName = strAddr;
	workData.strChunkExt = strExt;
	m_strWorking.push(workData);

	ReleaseMutex(m_hMutex);
}
void CChunkAnsycLoader::ClearWork()
{
	::WaitForSingleObject(m_hMutex, INFINITE);

	while(m_strWorking.size() > 0)
		m_strWorking.pop();

	ReleaseMutex(m_hMutex);
}

void CChunkAnsycLoader::SetNotifyTarget(CChunkAnsycLoaderReceiver* pReceiver)
{
	m_pReceiver = pReceiver;
}

int	 CChunkAnsycLoader::GetWorkCount()
{
	::WaitForSingleObject(m_hMutex, INFINITE);
	int iCount = (int) m_strWorking.size();
	ReleaseMutex(m_hMutex);

	return iCount;
}

DWORD CChunkAnsycLoader::WorkFun (void* pParam)
{
	CChunkAnsycLoader* pThis = (CChunkAnsycLoader*)pParam;

	while(pThis->m_bStop != true)
	{
		::WaitForSingleObject(pThis->m_hMutex, INFINITE);
		if(pThis->m_strWorking.size() == 0)
		{
			ReleaseMutex(pThis->m_hMutex);
			Sleep(10);
		}
		else
		{

			CString str = pThis->m_strWorking.front().strChunkName;
			CString ext = pThis->m_strWorking.front().strChunkExt;
			DWORD data = pThis->m_strWorking.front().data;
			pThis->m_strWorking.pop();
			ReleaseMutex(pThis->m_hMutex);


			CString strStateWord;
			strStateWord.Format("Reading Chunk: %s", str.GetBuffer());
			SetStateWord(strStateWord);
			TRACE("Read Chunk File (%s): %s\n", ext.GetBuffer(), str.GetBuffer()); 
			CChunkDetail* pChunkDetail = CChunkDetail::CreateChunkDetailFromChunk(str, ext);

			pThis->m_pReceiver->OnReceiveAnsyncChunk(pChunkDetail, data);

			strStateWord.Format("Finished Chunk: %s", str.GetBuffer());
			SetStateWord(strStateWord);
		}
	}
	return 0;
}

void CChunkAnsycLoader::SetStateWord(CString str)
{
	FILE* pFile = fopen(".\\log\\state.log", "w");
	if(pFile)
	{
		fprintf(pFile, "%s", str.GetBuffer());
		fclose(pFile);
	}
}

// ----------------------CChunkDetail------------------------- [6/3/2008]
CChunkDetail::CChunkDetail(void)
{
}

CChunkDetail::~CChunkDetail(void)
{
}

void CChunkDetail::LoadChunkData(Ogre::Resource* pChunk)
{
}

CBitmap* CChunkDetail::GetIcon()
{
	return CBitmap::FromHandle(::LoadBitmap((HINSTANCE)AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LOGO_ICON)));
}

CChunkDetail* CChunkDetail::CreateDefaultChunkDetail(CString strChunk)
{
	CChunkDetail* pRet = new CChunkDetailDefault();

	pRet->SetChunkName(strChunk);

	return pRet;
}

CChunkDetail* CChunkDetail::CreateChunkDetailFromChunk(CString strChunk, CString ext)
{
	Ogre::Resource* pChk = NULL;
	CChunkDetail* pRet = NULL;

	ext.MakeLower();
	if(ext=="otex" || ext=="omod" || ext=="ent" || ext=="emo" || ext=="dds")
	{
		pChk = ResourceManager::getSingleton().blockLoad(strChunk.GetBuffer());
		TRACE("Load Resource: %s\n", strChunk.GetBuffer());
	}
	else
	{
		TRACE("Ignore Resource: %s\n", strChunk.GetBuffer());
	}


	if(!pChk)
	{
		pRet = new CChunkDetailDefault();
	}
	else
	{
		if(IS_KIND_OF(TextureData, pChk))
			pRet = new CChunkDetailTexture();
		else if(IS_KIND_OF(EntityData, pChk))
			pRet = new CChunkDetailShowModel();
		else if(IS_KIND_OF(EntityMotionData, pChk))
			pRet = new CChunkDetailMotion();
		else if(CEffectUnit::CanEdit(pChk) )
			pRet = new CChunkDetailPreviewModel();
		else
			pRet = new CChunkDetailDefault();
	}
	if(pChk != NULL)
	{
		pRet->LoadChunkData(pChk);
		OGRE_RELEASE(pChk);
	}
	pRet->SetChunkName(strChunk);


	return pRet;
}



// ---------------CChunkDetailDefault---------------------------------- [6/3/2008]

CChunkDetailDefault::CChunkDetailDefault()
{
}

CChunkDetailDefault::~CChunkDetailDefault()
{
}

CChunkDetailBar* CChunkDetailDefault::CreateBar(CWnd* pParent)
{
	CChunkDetailBarDefault* pRet = new CChunkDetailBarDefault();
	pRet->m_strChunkName = m_strChunkName;
	pRet->m_pChunkDtl = this;
	pRet->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 100, 100), pParent, 1);

	return pRet;
}


// -----------------CChunkDetailTexture------------------------------- [6/3/2008]

CChunkDetailTexture::CChunkDetailTexture()
{
	m_pBitmap = NULL;
}
CChunkDetailTexture::~CChunkDetailTexture()
{
}

IPicture* CChunkDetailTexture::LoadPictureFromBuffer(char* pSrcBuffer, int iBufLen)
{
	HGLOBAL hGloble = GlobalAlloc(GMEM_MOVEABLE, iBufLen);

	char* pBuf = (char*)GlobalLock(hGloble);
	memcpy((void*)pBuf, pSrcBuffer, iBufLen);
	GlobalUnlock(hGloble);

	IStream* pStream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(hGloble, TRUE, &pStream);
	if(hr != S_OK)
	{
		GlobalFree(hGloble);
		return NULL;
	}

	IPicture* pPicture = NULL;
	hr = OleLoadPicture(pStream, iBufLen, TRUE, IID_IPicture, (void**)&pPicture);
	pStream->Release();
	if(hr != S_OK)
		return NULL;

	return pPicture;
}

HBITMAP CChunkDetailTexture::DrawPictureOnBitmap(IPicture* pPic)
{
	HWND hDesktop = ::GetDesktopWindow();
	HDC hDC = ::GetDC(hDesktop);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, 64, 64 );

	::SelectObject(hMemDC, hBmp);

	OLE_YSIZE_HIMETRIC ysize;
	OLE_XSIZE_HIMETRIC xsize;
	pPic->get_Height(&ysize);
	pPic->get_Width(&xsize);

	//Y方向需要镜像一下
	pPic->Render(hMemDC,  0, 64, 64, -64, 0, 0, xsize, ysize, NULL);
	::DeleteDC(hMemDC);

	return hBmp;
}

void CChunkDetailTexture::LoadChunkData(Ogre::Resource* pChunk)
{
	CChunkDetail::LoadChunkData(pChunk);

	m_iHeight = 0;
	m_iWidth = 0;

	if(pChunk == NULL)
	{
		return;
	}

	if(!IS_KIND_OF(TextureData, pChunk))
	{
		return;
	}


	TextureData *ptexres = static_cast<TextureData *>(pChunk);
	m_iWidth = ptexres->getWidth();
	m_iHeight = ptexres->getHeight();

	size_t buflen;
	void *pbuffer = ptexres->getHardwareTexture()->dumpToBuffer("bmp", buflen);

	IPicture* pPicture = LoadPictureFromBuffer((char*)pbuffer, buflen);
	free(pbuffer);

	if(pPicture == NULL)
	{
		return;
	}
	HBITMAP hBmp = DrawPictureOnBitmap(pPicture);
	m_pBitmap = CBitmap::FromHandle(hBmp);
	pPicture->Release();

	/*
	Ogre::Resource* pPreviewChunk = ((Ogre::ComplexChunk*)pChk)->GetAttribByName("Preview");
	if(pPreviewChunk != NULL)
	{

	size_t datalen = 0;
	unsigned char* pData = (unsigned char*)pPreviewChunk->asRawData(datalen);
	CDib dib;
	if(TRUE == dib.LoadFromMem((char*)pData, datalen))
	{
	delete(pChk);
	m_pBitmap = dib.CreateDib();
	return;
	}
	}

	if(pChk->GetType() == Ogre::CHUNK_TEXTURE)
	{
	Ogre::TextureChunk* pTexChk = (Ogre::TextureChunk*) pChk;
	if(!pChk->Parse())
	{
	delete(pChk);
	return ;
	}
	m_iWidth = pTexChk->m_Desc.width;
	m_iHeight = pTexChk->m_Desc.height;

	Ogre::ITexture* pPreviewTex = NULL;
	if(pTexChk->m_pTex->m_AnimFrames.size() == 0)
	pPreviewTex = pTexChk->m_pTex->m_pSysTex;
	else
	pPreviewTex = pTexChk->m_pTex->m_AnimFrames[0];

	if(pPreviewTex == NULL)
	return ;

	Ogre::TextureDX9* pTexD9 = (Ogre::TextureDX9*) pPreviewTex;

	CEnginAgent::GetInstance()->RequestEngineDevice();
	IDirect3DDevice9* pDev = ((Ogre::RendererDX9*)CEnginAgent::GetInstance()->m_pEngine->GetRenderer())->m_pd3dDevice;

	IDirect3DTexture9* pTex;
	IDirect3DSurface9* pSurf;



	IDirect3DSurface9* pOldSur = NULL;
	pDev->GetRenderTarget(0, &pOldSur);

	DWORD oldfvf ;
	pDev->GetFVF(&oldfvf);
	DWORD oldCulMode;
	pDev->GetRenderState(D3DRS_CULLMODE, &oldCulMode);

	D3DXCreateTexture(pDev,64,64,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&pTex);		
	pTex->GetSurfaceLevel(0,&pSurf);

	pDev->BeginScene();
	pDev->SetRenderTarget(0,pSurf );

	D3DVIEWPORT9 vp;
	pDev->GetViewport( &vp);
	struct QuadVertex
	{
	D3DXVECTOR4 pos;
	D3DXVECTOR2 tex;
	};

	QuadVertex v[4];
	v[0].pos = D3DXVECTOR4(-0.5f, -0.5f, 0.5f, 1.0f);
	v[0].tex = D3DXVECTOR2(0.0f,0.0f);

	v[1].pos = D3DXVECTOR4(-0.5f, (float)vp.Height,0.5f,1.0f);
	v[1].tex = D3DXVECTOR2(0.0f,1.0f);

	v[2].pos = D3DXVECTOR4( (float)vp.Width, 0.5f,0.5f,1.0f);
	v[2].tex = D3DXVECTOR2( 1.0f,0.0f);

	v[3].pos = D3DXVECTOR4( (float)vp.Width, (float)vp.Height,0.5f,1.0f);
	v[3].tex = D3DXVECTOR2(1.0f,1.0f);

	pDev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0); //clear texture

	pDev->SetFVF(D3DFVF_XYZRHW|D3DFVF_TEX1);
	pDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	pDev->SetTexture(0,pTexD9->GetSysTex());

	pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,(void*)v,sizeof(QuadVertex) );

	pDev->EndScene();

	pDev->SetFVF(oldfvf);
	pDev->SetRenderState(D3DRS_CULLMODE,oldCulMode);
	//pDev->SetTexture(0,pTexD9->GetSysTex());
	pDev->SetRenderTarget(0, pOldSur);
	CEnginAgent::GetInstance()->ReleaseEngineDevice();

	ID3DXBuffer* pBuffer = NULL;

	D3DXSaveTextureToFileInMemory(&pBuffer, D3DXIFF_BMP,pTex,NULL);

	CDib dib;
	if(pBuffer != NULL)
	{
	dib.LoadFromMem((char*)pBuffer->GetBufferPointer(), (int)pBuffer->GetBufferSize());
	}

	OGRE_RELEASE(pSurf);
	OGRE_RELEASE(pOldSur);
	OGRE_RELEASE(pBuffer);
	OGRE_RELEASE(pTex);



	m_pBitmap =  dib.CreateDib();
	return;

	}
	*/
}

CBitmap* CChunkDetailTexture::GetIcon()
{
	if(m_pBitmap != NULL)
		return m_pBitmap;
	return CChunkDetail::GetIcon();
}

CChunkDetailBar* CChunkDetailTexture::CreateBar(CWnd* pParent)
{
	CChunkDetailBarTex* pRet = new CChunkDetailBarTex();

	pRet->m_pTexDtl = this;
	pRet->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 100, 100), pParent, 1);

	return pRet;
}


// -----------------CChunkDetailShowMod------------------------------- [6/10/2008]

CChunkDetailShowModel::CChunkDetailShowModel()
{
}

CChunkDetailShowModel::~CChunkDetailShowModel()
{
}

void CChunkDetailShowModel::LoadChunkData(Ogre::Resource* pChunk)
{
	assert(IS_KIND_OF(Ogre::EntityData, pChunk));
	Ogre::EntityData *pentitydata = static_cast<Ogre::EntityData *>(pChunk);

	size_t i;
	for(i=0; i<pentitydata->m_Motions.size(); i++)
	{
		Ogre::EntityMotionData *pmotion = pentitydata->m_Motions[i];
		m_MotionNames.push_back(CString((const char *)pmotion->m_strName));
	}

	for(i=0; i<pentitydata->m_AddtionAnims.size(); i++)
	{
		Ogre::EntityMotionData *pmotion = pentitydata->m_AddtionAnims[i];
		m_MotionNames.push_back(CString((const char *)pmotion->m_strName));
	}
}

CChunkDetailBar* CChunkDetailShowModel::CreateBar(CWnd* pParent)
{
	CChunkDetailBarShowMod* pRet = new CChunkDetailBarShowMod();

	pRet->m_pModDtl = this;
	pRet->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 100, 100), pParent, 1);

	return pRet;
}

// ------------------CChunkDetailMotion---------------------------- [7/22/2008]

void CChunkDetailMotion::LoadChunkData(Ogre::Resource* pChunk)
{

}

CChunkDetailBar*	CChunkDetailMotion::CreateBar(CWnd* pParent)
{
	CChunkDetailBarMotion* pRet = new CChunkDetailBarMotion();

	CRect rect(0, 0, 100, 100);
	CRect rect2;
	pParent->GetClientRect(&rect2);
	rect += rect2;

	pRet->m_strChunkName = GetChunkName();
	pRet->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0, 0, 100, 100), pParent, 1);

	return pRet;
}

// -------------------CChunkDetailPreviewModel------------------------- [9/3/2008]

CChunkDetailPreviewModel::~CChunkDetailPreviewModel()
{
	OGRE_RELEASE(m_pNode); 
}

void CChunkDetailPreviewModel::LoadChunkData(Ogre::Resource* pChunk)
{
	assert(IS_KIND_OF(ModelData, pChunk));
	m_pNode = new Model(static_cast<ModelData *>(pChunk));
}

CChunkDetailBar* CChunkDetailPreviewModel::CreateBar(CWnd* pParent)
{
	CChunkDetailBarPreviewModel* pRet = new CChunkDetailBarPreviewModel();

	CRect rect(0, 0, 100, 100);
	CRect rect2;
	pParent->GetClientRect(&rect2);
	rect += rect2;

	pRet->m_pDetail = this;
	pRet->Create(NULL, NULL, 	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, pParent, 1);

	return pRet;
}
