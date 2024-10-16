#ifndef __ParticleEmitter_H__
#define __ParticleEmitter_H__

#include "OgreColourValue.h"
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreRenderableObject.h"
#include "OgreParticleEmitterData.h"

namespace Ogre
{
	struct ParticleVertex
	{
		Vector3 pos;
		RGBA color;
		Vector2 uv;
		Vector2 uvMask;
	};

	class _OgreExport ParticleEmitter : public RenderableObject
	{
		DECLARE_RTTI(ParticleEmitter)
	public:
		ParticleEmitter(){}
		ParticleEmitter(ParticleEmitterData *pdata);

		// begin specific for ForcedPE
		void SetForcePE (bool force);
		void SetForcePEPos (Vector3 pos);
		void SetForcePERange (float fRange);
		// end specific for ForcedPE;

		void setPreUpdate (bool bPreUpdate);
		float isPreUpdate ();

		void forceStopEmit (bool bStop);
		bool isForceStopEmit ();

		void EmitCustomParticle(Vector3 pos);
		void emitCustomParticles(Vector3* ppos, size_t count, const ParticleEmitterFrameData &animdata);
		void emitCustomParticles(size_t count, const ParticleEmitterFrameData &animdata);

		virtual void update(uint dtick);
		void calculateUpdate (float dtime, Matrix4 mWorld);
		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void render(SceneRenderer* prenderer, const ShaderEnvData &envdata);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

		void renderFace(SceneRenderer* prenderer, const ShaderEnvData &envdata);
		void renderObject(SceneRenderer* prenderer, const ShaderEnvData &envdata);
		void getCollisionFaceTrans(PECollisionFace& face, Matrix4& tm);

	protected:
		~ParticleEmitter();
		size_t emitParticles(size_t num, const ParticleEmitterFrameData &animdata);
		void updateParticles(float dtime, const ParticleEmitterFrameData &animdata);

		void fillParticleVert(ParticleVertex *pvert, unsigned short basevert, unsigned short *pindex, Particle &pt, const Matrix4 &tm);
		void fillParticleVertGpu(ParticleVertex *pvert, unsigned short basevert, unsigned short *pindex, Particle &pt, const Matrix4 &tm);
		void fillParticleVertNew(Particle &pt, const Matrix4 &tm, int i);

		void transformCollision(Particle& pt, Vector3 oldpos, float dtime);
		void transformParticle(Particle& pt, float dtime, const ParticleEmitterFrameData &animdata);
		void calWorldBounds();

	protected:
		ParticleEmitterData *m_pEmitter;
		std::vector<Particle>m_Particles;
		size_t m_nLiveParticle;
		float m_AccumEmitNum;
		int m_nEmittedParticesNum; // 已经发射的粒子数量

		Vector3 m_AccumForce;

		VertexDeclHandle m_VertDecl;
		Material *m_pMtl;

		uint m_CurTick;
		int m_CurSeq;
		float m_Scale;
		bool  m_bReady;
		Matrix4 m_View;

		ParticleVertex* m_pVerts;
		unsigned short*	m_pIndex;

		Vector2 m_uv0, m_uv1, m_uv2, m_uv3;
		Vector2 m_uvMask0, m_uvMask1, m_uvMask2, m_uvMask3;

		ParticleEmitterFrameData m_LastFrameAnimData;
		bool m_InitFrameData;

		float m_fTimeForSlowdown;

		bool m_bForceStopEmit;

		// begin specific for ForcedPE
		bool m_bIsForcePE;
		Vector3 m_ForcePos;
		float m_ForcePERange;
		// end specific for ForcedPE;
	};
}

#endif