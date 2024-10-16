/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __RenderSystem_H_
#define __RenderSystem_H_

// Precompiler options
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"


namespace Ogre
{

	typedef void (*RestoreLostDeviceCallBack)(void);

	enum
	{
		//清除当前backbuffer的标记
		CLEAR_STENCIL = 1,
		CLEAR_TARGET  = 2,
		CLEAR_ZBUFFER = 4,

		LOCK_READ = 1,      //只读
		LOCK_WRITE = 2,     //只写
		LOCK_READWRITE = 3, //读写

		//最大贴图stage
		MAX_TEXSTAGE = 8,
	};

	class _OgreExport RenderSystem : public Singleton<RenderSystem>
	{
	public:
		struct InitDesc
		{
			int adapter;
			bool brefrast; //用ref raster
			bool bfullscreen;
			bool manual_msgfilter;
			bool debug_vs;
			bool debug_ps;
			bool debug_nv;
			bool backbuf_lockable;
			bool sync_refresh;
			unsigned int width;
			unsigned int height;
			unsigned int colorbits;
			unsigned int alphabits;
			unsigned int depthbits;
			unsigned int stencilbits;
			int multisample;

			void *hwnd;
		};

		struct RenderStats
		{
			size_t texmem; //used texture memory in bytes
			size_t ntexture;
			size_t nprimitive;
			size_t ndraw;   //drawprimitive
			size_t ncontext; //shader context
			size_t nshaderswitch;

			//用于渲染线程累加
			size_t nprimitive_d;
			size_t ndraw_d;   //drawprimitive
			size_t ncontext_d; //shader context
			size_t nshaderswitch_d;
		};

		RenderStats m_RenderStats;

	public:
		RenderSystem();
		~RenderSystem();

		virtual bool initialise(const InitDesc &desc) = 0;
		virtual void shutdown() = 0;
		virtual RenderWindow *createRenderWindow(const InitDesc &desc) = 0;
		virtual void clear( unsigned int flags, unsigned int color, float z, unsigned int stencil ) = 0;
		virtual void setViewport(const Viewport &viewport) = 0;
		virtual bool beginFrame() = 0;
		virtual void endFrame() = 0;
		virtual int present() = 0;
		virtual RenderWindow *getMainWindow() = 0;
		virtual VertexDeclHandle getInputLayout(const VertexFormat &vertfmt) = 0;
		virtual int resetDevice() = 0;
		virtual int testDeviceReset() = 0;
		virtual bool restoreLostDevice() = 0;
		virtual RenderWindow *findRenderWindow(void *hwnd) = 0;
		virtual void setContextQueDesc(const ContextQueDesc &desc) = 0;
		virtual void setCursorProperty(TextureData *ptex, int hotspotx, int hotspoty, int u, int v, int width, int height) = 0;
		virtual void setCursorPosition(int x, int y) = 0;
		virtual void showCursor(bool b) = 0;
		virtual void snapShotAll( const char* szSavePath ) = 0;
		virtual void snapShot() = 0;

		/** Gets the capabilities of the render system. */
		const RenderSystemCapabilities* getCapabilities(void) const { return m_Capabilities; }

		ShaderTechnique *getWireframeShader()
		{
			return m_pWireframeShader;
		}

		int GetRenderLevel()
		{
			return m_iLevel;
		}

		void SetSaveCompiledShaders(bool bSave)
		{
			m_bSaveCompiledShader = bSave;
		}

		bool IsSaveCompiledShaders()
		{
			return m_bSaveCompiledShader;
		}
		void SetRestoreLostDeviceCallBack(RestoreLostDeviceCallBack pCallBack)
		{
			m_restoreDeviceCallBack = pCallBack;
		}

	public:
		bool m_bSnapShot;

	protected:
		RenderSystemCapabilities *m_Capabilities;
		ShaderTechnique *m_pWireframeShader;
		int m_iLevel; // 0不支持，1低，2中，3高
		bool m_bSaveCompiledShader;

		std::string m_strSnapShotPath;

		//
		RestoreLostDeviceCallBack	m_restoreDeviceCallBack;
	};
}

#endif