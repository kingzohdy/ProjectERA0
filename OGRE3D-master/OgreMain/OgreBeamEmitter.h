
#ifndef __BeamEmitter_H__
#define __BeamEmitter_H__

#include "OgreRenderableObject.h"
#include "OgreRandomNumber.h"
#include "OgreColourValue.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreBeamEmitterData.h"

namespace Ogre
{
	struct BEAM_T
	{
	public:
		Vector3 basepos;
		float basepoint;
		float randomoffset;
		RGBA rgba;
		Vector2 uv;
	};

	struct BEAM_VERT;
	struct BEAM_DATA;
	class _OgreExport BeamEmitter : public RenderableObject
	{
		DECLARE_RTTI(BeamEmitter)

	public:
		BeamEmitter(BeamEmitterData *psource=NULL);

		// begin specific for controlled FlashChain
		void SetTargetPos (Ogre::Vector3 target);
		// end specific for controlled FlashChain

		virtual void update(uint dtick);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

		void	UpdateBeamLife(uint dtick);
		void	EmitBeam();
		void	FillBeamVert(BEAM_VERT* pVB, unsigned short *pIB, unsigned int uIndexStart, BEAM_DATA& beam);
		Vector2 ComputeUV(Vector2 srcuv, int texcol, int texrow, int tile, float rot, float uoff, float voff, float uscale, float vscale);

	protected:
		virtual ~BeamEmitter();

	public:
		static RandomGenerator m_Rand;
		BeamEmitterData *m_pBeamEmitter;
	protected:
		Vector3		m_target;
		int			m_iRefAnchor;
		Vector3		m_vRefOffset;
		int			m_iMaxBeam;
		size_t			m_samplerstate;


		std::vector<BEAM_T>	m_BeamVertices;
		std::vector<unsigned short> m_BeamIndices;

		std::vector<float> m_LFPoints;
		std::list<BEAM_DATA> m_BeamDatas;

		size_t m_AccumEmitNum;

		Vector3 m_ViewPos;

		int m_VerticesPerParticle;
		int m_IndicesPerParticle;

		Material *m_pMtl;

		uint m_CurTick;
		float m_fAccu;
		float m_fPlaySpeed;
	};
}

#endif