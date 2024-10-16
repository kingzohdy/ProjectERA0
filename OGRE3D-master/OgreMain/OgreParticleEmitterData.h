
#ifndef __ParticleEmitterData_H__
#define __ParticleEmitterData_H__

#include "OgreColourValue.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreMatrix4.h"
#include "OgreRandomNumber.h"
#include "OgreKeyFrameArray.h"
#include "OgreResource.h"
#include <vector>

namespace Ogre
{
	struct Particle
	{
	public:
		Particle ()
		{
			groundPicked = false;
		}
		~Particle ()
		{}

		Vector3 pos;
		Vector3 normal; // �͵��������ʱ������
		bool groundPicked; // �Ƿ��Ѿ��͵��������
		Vector3 down;
		Vector3 dir;
		float speed;
		float life;
		float age;
		int tile;
		float sizevar;
		MovableObject *pnode;
		float randomrot;
		ColourValue initcolor;
	};

	enum EmitterType
	{
		EMITTER_PLANE = 0,
		EMITTER_SPHERE,
		EMITTER_SPHERE_FACE,
		EMITTER_CIRCLE,
		EMITTER_COLUMN_SPREAD,
		EMITTER_COLUMN_UP, // ����
		EMITTER_COLUMN // ����
	};

	enum ParticleDrawType
	{
		PARTICLE_NORMAL = 0,
		PARTICLE_QUAD = 1,
		PARTICLE_UNKNOWN = 2,
		PARTICLE_OBJECT = 3
	};

	enum
	{
		PBLEND_COPY = 0,  //= srccolor
		PBLEND_ALPHA,     //= srccolor*srcalpha + destcolor*(1-srcalpha)
		PBLEND_ADD, //=srccolor*srcalpha + destcolor
	};

	enum
	{
		PEFLAG_LOCAL = 1,
		PEFLAG_CUSTOM_EMIT = 2,
		PEFLAG_EX_ROT = 4,
		PEFLAG_MORE = 8,
		PEFLAG_EMIT_ONCE = 16
	};

	enum ParticleDirType
	{
		PR_FACE_CAMERA, //�������
		PR_ROT_ABOUT_UP, //��y����ת
		PR_FACE_UP,   //����y��
		PR_ROT_ABOUT_DIR, //���˶�������ת
		PR_FACE_DIR,  //�����˶�����
		PR_FACE_UP_NOROT //����y�᲻��ת
	};

	enum ParticleCollisionType
	{
		PC_NO_COLLISION,
		PC_COLLISION_TERREN,
		PC_COLLISION_FACE
	};

	enum ParticleCollisionFace
	{
		PCF_REFLECT,
		PCF_REPLACE_OBJ,
		PCF_END_LIFE
	};

	class PEmitterDesc
	{
	public:
		EmitterType m_EmitterType;
		ParticleDrawType m_ParticleType;
		unsigned int m_Flags;
		int m_BlendType;

		Vector3 m_Pos;
		unsigned int m_MaxParticles;

		float m_MidPoint;  //midpoint in lifespan
		float m_Slowdown;
		float m_Rotation;
		int m_TexRows;
		int m_TexCols;

		float m_TimeSlot; //���Ҫ�������,��ֵ����Ϊ0.0f

		bool m_bFixColor;

		ColourValue m_ColorKeys[3];
		float m_SizeKeys[3];
		float m_AspectRatioKeys[3];
	};


	class PEmitterDescEx
	{
	public:
		ParticleCollisionType m_Collision;
		float				 m_TerrenAttenuation;
		float				 m_ColorMid;
		float				 m_SizeMid;
		float				 m_AspectMid;
		float				 m_UVRotate;
		float				 m_UVScale;
		bool				 m_bRandomTile;
		bool				 m_bDistort;
		int					 m_iUVAddressMode;
		int					 m_iUAddressMode;
		int					 m_iVAddressMode;

		float				m_fRandomRot;

		int					m_iMaskUVAddrMode;
		float				m_MaskUVRotate;
		float				m_MaskUVScale;
		int					m_MaskTexRows;
		int					m_MaskTexCols;

		bool m_CenterAbsOffset;//���Ի�����Է�ʽ
		Vector2 m_CenterOffset;//���ĵ��ƫ�ƣ� �������Է�ʽ�� �����ӵ�ǰ��СΪ��λ
	};

	class PEmitterDescExx
	{
	public:
		bool m_CollideWithTerrain;
	};

	class PECollisionFace
	{
	public:
		int			m_iUpLevel;				//����ڵڼ��㸸�ڵ�
		bool		m_bReverseUpLevel;		//��������Ƿ�Ϊ����
		int			m_iAnchor;
		Vector3		m_Offset;
		Quaternion	m_Rotation;
		float		m_fWidth;
		float		m_fLength;
		float		m_fAttenuation;

		//ParticleCollisionFace m_faceType;
		//Resource*	m_pReflectReplace;

		Matrix4		m_mBufferedTrans;
		Matrix4		m_mBufferedTransInv;
	};

	struct ParticleEmitterFrameData
	{
		Matrix4 tm, mrot;
		float speed;
		float speedvar;
		float spread;
		float spreadoffset;
		float latitude;
		Vector3 accelerationdir;
		float acceleration;
		float lifespan;
		float rate;
		float areal;
		float areaw;
		float aread; // ���
		float deacceleration;
		ColourValue color;
		float size;
		float aspectratio;
		float scale;

		float sizevar;
		float lifevar;

		float randomspread;
	};

	class _OgreExport ParticleEmitterData : public Resource
	{
		DECLARE_RTTI(ParticleEmitterData)
	public:
		ParticleEmitterData();

		void prepareGenParticle(ParticleEmitterFrameData &framedata, int seq, uint tick, const Matrix4 &tm);
		void genParticle(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticlePlane(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticleSphere(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticleSphereFace(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticleCircle(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticleColumnSpread(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticleColumnUp(Particle &pt, const ParticleEmitterFrameData &framedata);
		void genParticleColumn(Particle &pt, const ParticleEmitterFrameData &framedata);

		void transformDirRandom(Particle &pt, const ParticleEmitterFrameData &framedata);

		float getSizeInLife(float fLifePercentige);
		float getAspectInLife(float fLifePercentige);
		float getAlphaInLife(float fLifePercentige);
		ColourValue getColorInLife(float fLifePercentige);

	protected:
		~ParticleEmitterData();

	public:
		static RandomGenerator m_Rand;

	public:
		int m_version; // ����Ҫ�־û�

		ParticleDirType m_RotStyle;
		PEmitterDesc m_Desc;
		PEmitterDescEx m_DescEx;
		PEmitterDescExx m_DescExx;

		// 105
		int m_PreTime;

		KeyFrameArray<float> m_ParticleSizeKey;
		KeyFrameArray<float> m_ParticleAspectKey;
		KeyFrameArray<ColourValue> m_ParticleColorKey;
		KeyFrameArray<float> m_ParticleAlphaKey;

		std::vector<PECollisionFace> m_CollisionFaces;

		KeyFrameArray<float>m_Speed;
		KeyFrameArray<float>m_SpeedVariation;
		KeyFrameArray<float>m_Spread;
		KeyFrameArray<float>m_Latitude;
		KeyFrameArray<Vector3>m_AccelerationDir;
		KeyFrameArray<float>m_Acceleration;
		KeyFrameArray<float>m_Lifespan;
		KeyFrameArray<float>m_Rate;
		KeyFrameArray<float>m_AreaLength;
		KeyFrameArray<float>m_AreaWidth;
		KeyFrameArray<float>m_AreaDepth; // ���
		KeyFrameArray<float>m_Deacceleration;
		KeyFrameArray<ColourValue>m_Color;
		KeyFrameArray<float>m_Size;
		KeyFrameArray<float>m_AspectRatio;
		KeyFrameArray<float>m_SpreadOffset;

		KeyFrameArray<float>m_SizeVariation;
		KeyFrameArray<float>m_LifeVariation;

		KeyFrameArray<float>m_RandomSpread;

		TextureData *m_pTexture;
		TextureData *m_pMaskTexture;

		std::vector<Resource*> m_pReplaceObjects;

	private:
		virtual void _serialize(Archive &ar, int version);
	};
}

#endif