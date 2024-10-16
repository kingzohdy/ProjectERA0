
#ifndef __OgreScreenTexture_H__
#define __OgreScreenTexture_H__

#include "OgreColourValue.h"
#include "OgreBaseObject.h"
#include "OgreUIRenderTypes.h"

namespace Ogre
{
	class ScreenRect
	{
	public:
		float m_sPixTop;
		float m_sPixLeft;
		float m_usPixWidth;
		float m_usPixHeight;
		unsigned short m_usTexTop;
		unsigned short m_usTexLeft;
		unsigned short m_usTexWidth;
		unsigned short m_usTexHeight;
		RectInt	m_MaskRect;
		ColorQuad m_Color;
		bool m_bGray;
		bool m_bStretch;
		UiUvType eUvType;
		float fAngle;
		float fZ;
		float m_fOffsetX;
		float m_fOffsetY;
		bool m_bRotateTopLeft;
	};

	class _OgreExport ScreenTexture : public BaseObject
	{
	public:
		ScreenTexture(Texture *ptex);

		unsigned int AddNewScreenRect(short left, short top, unsigned short width, 
			unsigned short height, unsigned short texleft, unsigned short textop,
			ColorQuad color=ColorQuad(255,255,255), bool bGray = false, float fAngle = 0.0f,bool bRotateTopLeft = false );
		unsigned int AddStretchScreenRect(float left, float top, float width, 
			float height, unsigned short texleft, 
			unsigned short textop, unsigned short texwidth, 
			unsigned short texheight,ColorQuad color=ColorQuad(255,255,255), bool bGray = false,
			UiUvType eUvType = UI_UVT_NORMAL, float fAngle = 0.0f,
			float fZ = 0.0f,bool bRotateTopLeft = false, float offx=0.0f, float offy=0.0f,RectInt& maskRect = EMPTY_RECT_INT );
		uint GetNumScreenRects()
		{ 
			return (uint)m_Rects.size(); 
		}

		const ScreenRect &GetScreenRect(unsigned int i) const;
		void Draw();
		void Reset()
		{ 
			m_Rects.resize(0); 
		}

		Texture *GetTexture()
		{ 
			return m_pTexture; 
		}

		void SetBlendMode(UIBlendMode mode)
		{ 
			m_BlendMode = mode; 
		}

		UIBlendMode GetBlendMode()
		{
			return m_BlendMode; 
		}

		unsigned int GetTextureWidth()
		{ 
			return m_TexWidth; 
		}

		unsigned int GetTextureHeight()
		{ 
			return m_TexHeight; 
		}

	protected:
		~ScreenTexture();

		std::vector<ScreenRect>m_Rects;
		Texture *m_pTexture;
		UIBlendMode m_BlendMode;
		unsigned int m_TexWidth;
		unsigned int m_TexHeight;
	};
}

#endif