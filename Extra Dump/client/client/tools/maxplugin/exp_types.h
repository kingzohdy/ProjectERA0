
#ifndef __EXP_TYPES_H__
#define __EXP_TYPES_H__

namespace DKEXP
{
#define SECONDSPERTICK (1.0f/4800.0f)

	class AnimationConverter;
	class MeshConverter;
	class InflBoneArray;
	class CameraConverter;
	class LightConverter;
	class HierConverter;

	static const unsigned int MAX_UV_NUM = 8;
	static const unsigned int MAX_BONES = 4;
	static const unsigned int MAX_BONE_INFL = 70;

	struct VertexData
	{
		Ogre::Vector3 pos;
		Ogre::Vector3 normal;
		Ogre::Vector2 uv[MAX_UV_NUM];
		Ogre::ColourValue vertColor;
		Ogre::Vector3 tangent;
		Ogre::Vector3 binormal;

		// bones that influence this vertex
		//unsigned int m_uiNumBones;
		unsigned char m_pfWeight[MAX_BONES];
		unsigned char m_puiBoneIndex[MAX_BONES];

	};

	struct MtlAnimData
	{
		std::string meshname;
		std::string mtlname;
		std::string varname;

		int nframe;
		void *pkeyframes;
		unsigned int *pticks;
		int stride;
		int type;
	};

	struct SEQUENCE_EXP_T
	{
		Ogre::SequenceDesc sequence;
		bool bSelectBones;
		std::vector <Ogre::String> bonesname;
	};

	struct SEQUENCE_MTLANI_T
	{
		Ogre::SequenceDesc sequence;
		std::vector <std::string> animesh;
	};

	class DecompMatrix
	{
	public:
		Ogre::Vector3			t;
		Ogre::Quaternion		q;
		Ogre::Quaternion		u;
		Ogre::Vector3			k;
		float					f;

		DecompMatrix();
		DecompMatrix & operator =( const ::Matrix3 &src );
		bool operator != ( const DecompMatrix &mat );
	};

	extern Ogre::Vector3 ConvertPoint3(const Point3 &p3);
	extern Ogre::Vector2 ConvertPoint2(const Point2 &p2);
	extern Ogre::Quaternion ConvertQuat(const Quat q);
	extern Ogre::Matrix4 ConvertMatrix3(::Matrix3 const &matMax);
	
	//返回max坐标系转到引擎坐标系的矩阵
	extern ::Matrix3 Max2EngineMatrix();
}

#endif