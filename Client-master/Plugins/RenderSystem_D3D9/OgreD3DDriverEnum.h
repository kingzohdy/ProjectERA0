
#ifndef __D3DDriverEnum_H__
#define __D3DDriverEnum_H__

#include "OgreD3D9Prerequisites.h"
#include "OgreRenderSystem.h"
#include <vector>

namespace Ogre
{
	enum VERTPROCESS_T
	{
		SOFTWARE_VP,
		MIXED_VP,
		HARDWARE_VP,
		PURE_HARDWARE_VP
	};

	//A depth/stencil buffer format that is incompatible with a  multisample type.
	struct D3DSMSCONFLICT_T
	{
		D3DFORMAT dsfmt;
		D3DMULTISAMPLE_TYPE mstype;
	};

	struct D3DDEVCOMBO_T
	{
		int iadapter;
		D3DDEVTYPE devtype;
		D3DFORMAT adapterfmt;
		D3DFORMAT backbuffmt;
		BOOL iswindowed;

		std::vector<D3DFORMAT>dsfmts;    //DepthStencil Formats
		std::vector<D3DMULTISAMPLE_TYPE>mstypes; // List of D3DMULTISAMPLE_TYPEs 
		std::vector<UINT32>msqualitys;           // List of DWORDs (number of quality levels for each multisample type) 

		std::vector<D3DSMSCONFLICT_T>conflicts;
		std::vector<VERTPROCESS_T>vptypes;
		std::vector<UINT32>intervals;  //present interval
	};

	struct D3DDEVICEINFO_T
	{
		int iadapter;
		D3DDEVTYPE type;
		D3DCAPS9   caps;
		std::vector<D3DDEVCOMBO_T *>devcombo;

		~D3DDEVICEINFO_T();
	};

	struct D3DADAPTER_T
	{
		UINT32 iadapter;
		D3DADAPTER_IDENTIFIER9 id;
		std::vector<D3DDISPLAYMODE>modes;
		std::vector<D3DDEVICEINFO_T *>devinfo;

		~D3DADAPTER_T();
	};

	typedef BOOL(* CONFIRMDEVICECALLBACK)( D3DCAPS9* pCaps, VERTPROCESS_T vertexProcessingType, 
		D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat );

	struct D3DSETTING_T
	{
		D3DADAPTER_T *padapter;
		D3DDEVICEINFO_T *pdevice;
		D3DDEVCOMBO_T *pdevcombo;

		D3DFORMAT dsformat;
		D3DMULTISAMPLE_TYPE mstype;
		UINT32 msquality;
		VERTPROCESS_T vptype;
		UINT32 interval;

		D3DDISPLAYMODE dispmode;
	};

	class D3DEnumeration
	{
	public:
		D3DEnumeration();
		~D3DEnumeration();
		BOOL Enumerate( IDirect3D9 *pd3d, CONFIRMDEVICECALLBACK mycallback );
		const D3DSETTING_T &ChooseD3DSettings( const RenderSystem::InitDesc &desc );
		const D3DSETTING_T &GetSettings(){ return m_Settings; }
		bool GetRenderTargetFormat(D3DFORMAT &dsfmt, D3DMULTISAMPLE_TYPE &mstype, D3DFORMAT colorfmt, int depthbits, int stencilbits, int multisample);

	public:
		std::vector<D3DFORMAT>m_AllowedFmt;  //color back buffer的格式在这里面选择
		std::vector<D3DADAPTER_T *>m_Adapters;
		CONFIRMDEVICECALLBACK m_ConfirmDeviceCallback;
		UINT32 m_MinFullScreenWidth;
		UINT32 m_MinFullScreenHeight;
		UINT32 m_MinColorChannelBits; // min color bits per channel in adapter format
		UINT32 m_MinAlphaChannelBits; // min alpha bits per pixel in back buffer format
		UINT32 m_MinDepthBits;
		UINT32 m_MinStencilBits;

		BOOL m_UseDepthBuf;
		BOOL m_UseMixedVP;
		BOOL m_RequireWindowed;
		BOOL m_RequireFullscreen;

	private:
		IDirect3D9* m_pD3D;
		D3DSETTING_T m_Settings;

	private:
		void EnumerateDevices( D3DADAPTER_T *padapter, std::vector<D3DFORMAT> &formatarray );
		void EnumerateDeviceCombo( D3DDEVICEINFO_T *pdevice, std::vector<D3DFORMAT> &formatarray );
		D3DDEVCOMBO_T *ComputeDeviceCombo( D3DDEVICEINFO_T *pdevice, D3DFORMAT dispformat, D3DFORMAT bkbufformat, BOOL iswindow );
		void BuildDepthStencilFormatList( D3DDEVCOMBO_T *pcombo );
		void BuildMultiSampleTypeList( D3DDEVCOMBO_T *pcombo );
		void BuildDSMSConflictList( D3DDEVCOMBO_T *pcombo );
		void BuildVertexProcessingTypeList( D3DDEVICEINFO_T *pdevice, D3DDEVCOMBO_T *pcombo );
		void BuildPresentIntervalList( D3DDEVICEINFO_T *pdevice, D3DDEVCOMBO_T *pcombo );
		void ConfirmDevice( D3DDEVICEINFO_T *pdevice, D3DDEVCOMBO_T *pcombo, VERTPROCESS_T vpt );

		void FindBestWindowMode( const RenderSystem::InitDesc &desc );
		void FindBestFullscreenMode( const RenderSystem::InitDesc &desc );
	};

}

#endif