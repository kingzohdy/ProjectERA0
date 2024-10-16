
#ifndef __OgrePrerequisites_H__
#define __OgrePrerequisites_H__

// Platform-specific stuff
#include "OgrePlatform.h"
#include <assert.h>
#include <string>
#include <vector>
#include <map>

namespace Ogre {
	// Define ogre version
#define OGRE_VERSION_MAJOR 1
#define OGRE_VERSION_MINOR 4
#define OGRE_VERSION_PATCH 7
#define OGRE_VERSION_SUFFIX ""
#define OGRE_VERSION_NAME "Eihort"

#define OGRE_VERSION    ((OGRE_VERSION_MAJOR << 16) | (OGRE_VERSION_MINOR << 8) | OGRE_VERSION_PATCH)

	// define the real number values to be used
	// default to use 'float' unless precompiler option set
#if OGRE_DOUBLE_PRECISION == 1
	/** Software floating point type.
	@note Not valid as a pointer to GPU buffers / parameters
	*/
	typedef double Real;
#else
	/** Software floating point type.
	@note Not valid as a pointer to GPU buffers / parameters
	*/
	typedef float Real;
#endif

#if OGRE_COMPILER == OGRE_COMPILER_GNUC && OGRE_COMP_VER >= 310 && !defined(STLPORT)
#   define HashMap ::__gnu_cxx::hash_map
#else
#   if OGRE_COMPILER == OGRE_COMPILER_MSVC
#       if OGRE_COMP_VER > 1300 && !defined(_STLP_MSVC)
#           define HashMap ::stdext::hash_map
#       else
#           define HashMap ::std::hash_map
#       endif
#   else
#       define HashMap ::std::hash_map
#   endif
#endif

#undef stricmp
#define stricmp _stricmp

	/** In order to avoid finger-aches :)
	*/
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef signed __int64	int64;
	typedef unsigned __int64 uint64;

	const char         MAX_CHAR    = 127;
	const uchar        MAX_UCHAR   = 255;
	const short        MAX_SHORT   = 32767;
	const ushort       MAX_USHORT  = 0xffff;
	const int        MAX_INT   = 2147483647;
	const uint       MAX_UINT  = 0xffffffff;
	const int64        MAX_INT64   = 9223372036854775807I64;
	const uint64       MAX_UINT64  = 0xffffffffffffffffI64;
	const float        MAX_FLOAT   = 3.402823466e+38F;
	const double        MAX_DOUBLE  = 1.7976931348623158e+308;

	const char         MIN_CHAR    = -127-1;
	const short        MIN_SHORT   = -32767-1;
	const int        MIN_INT   = -2147483647-1;
	const int64        MIN_INT64   = -9223372036854775807I64 - 1;
	const float        MIN_FLOAT   = 1.175494351e-38F;
	const double        MIN_DOUBLE  = 2.2250738585072014e-308;

	typedef std::string String;


#ifndef NULL
#define NULL (0)
#endif

	/// Useful macros
#define OGRE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define OGRE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define	OGRE_RELEASE(p)		 { if(p) { p->release();   (p)=NULL; } }
#define ARRAY_ELEMENTS(array) (sizeof(array)/sizeof(array[0]))


	enum IntersectType
	{
		INTERSECT_PICK = 0, //拾取
		INTERSECT_LIGHT,    //光线是否相交
	};

	enum BlendMode
	{
		BLEND_OPAQUE = 0, //不透明
		BLEND_ALPHATEST, //Alpha测试
		BLEND_ALPHABLEND, //Alpha混合
		BLEND_ADDBLEND, //相加混合
		BLEND_ADD,  //相加
		BLEND_MODULATE, //和背景相乘
		BLEND_MODULATE2X, //和背景相乘X2

		MAX_BLEND_MODE
	};

	// Pre-declare classes
	// Allows use of pointers in header files without including individual .h
	// so decreases dependencies between files
	// Pre-declare classes
	// Allows use of pointers in header files without including individual .h
	// so decreases dependencies between files
	class AlphaTexture;
	struct AmbientInfo;
	class AmbientManager;
	class AnimPlayTrack;
	class BaseAnimationData;
	class AnimationData;
	class AnimationPlayer;
	class Archive;
	class AvatarManager;
	class BaseObject;
	class BaseKeyFrameArray;
	class BeamEmitter;
	class BeamEmitterData;
	class Billboard;
	class BillboardData;
	class BitArray1D;
	class BitArray2D;
	class BoxBound;
	class BoxSphereBound;
	class BoneData;
	class BoneTrack;
	class BSPData;
	class Camera;
	class ColourValue;
	struct ContextQueDesc;
	class CullFrustum;
	class CullResult;
	class DataStream;
	class DecalData;
	class DecalNode;
	class DummyNodeData;
	class DummyNode;
	class DynLib;
	class DynLibManager;
	class DynamicBufferPool;
	class DynamicVertexBuffer;
	class DynamicIndexBuffer;
	class EffectObject;
	class Entity;
	class EntityData;
	class ModelMotion;
	class EntityMotionData;
	class FilePackage;
	class FixedString;
	class FogEffect;
	class Footprints;
	class GameScene;
	class GameTerrainScene;
	class HardwareBuffer;
	class HardwareVertexBuffer;
	class HardwareIndexBuffer;
	class HardwarePixelBuffer;
	class IndexData;
	class IndexBuffer;
	class InputManager;
	class Light;
	class LightData;
	class LooseOctreeNode;
	class LooseOctree;
	class Matrix3;
	class Matrix4;
	class Material;
	class MaterialTemplate;
	class MaterialManager;
	class MaterialParamTrack;
	class MemPoolMgr;
	class MeshData;
	class MeshInstance;
	class Model;
	class ModelData;
	class MovableObject;
	struct ModelInstanceData;
	class MotionEventHandler;
	class MotionElementData;
	class NormalSceneRenderer;
	class ParametricShape;
	class ParamShapeData;
	struct Particle;
	class ParticleEmitter;
	class ParticleEmitterData;
	class PECollisionFace;
	class Plane;
	class PlantSource;
	class Plugin;
	class Quaternion;
	class Ray;
	class RenderLines;
	class RenderableObject;
	class RenderTarget;
	class RenderWindow;
	class RenderSystem;
	class RenderSystemCapabilities;
	class Resource;
	class ResourceManager;
	class ResTable;
	class ResDB;
	class RibbonEmitter;
	class RibbonEmitterData;
	class Root;
	class SceneDebugger;
	class SceneRenderer;
	class SceneManager;
	class ScriptVM;
	struct SequenceDesc;
	class SequenceMap;
	struct ShaderEnvData;
	struct ShaderEnvFlags;
	class ShaderTechnique;	
	class ShaderContext;
	class ShaderContextPool;
	class SimpleGameScene;
	class SkeletonAnimData;
	class SkeletonData;
	class SkeletonInstance;
	class SoundSystem;
	class SoundNode;
	class SoundData;
	class SphereBound;
	class SurfaceData;
	class SubMeshData;
	class SubMeshInstance;
	struct TextureDesc;
	class TerrainBlockSource;
	class TerrainTileSource;
	class TerrainBlock;
	class TerrainTile;
	class Texture;
	class TextureData;
	class TextureRenderTarget;
	class TLiquid;
	class UIRenderer;
	class VertexData;
	class VertexBuffer;
	class VertexFormat;
	class Vector2;
	class Vector3;
	class Vector4;
	class WorldPos;
	class WorldPlane;
	class WorldRay;
	class XMLData;
	class XMLNode;

	class PlantNode;
	class SkyModel;
	class ResTable;
	class ResDB;

	struct Viewport;

	template<class T>
	void DeletePointerArray(std::vector<T *>&array)
	{
		for(size_t i=0; i<array.size(); i++)
		{
			delete array[i];
		}
		array.clear();
	}

	template<class T>
	void ReleasePointerArray(std::vector<T *>&array)
	{
		for(size_t i=0; i<array.size(); i++)
		{
			array[i]->release();
		}
		array.clear();
	}

	inline float TickToTime(uint tick)
	{
		return (float)(tick/1000.0f);
	}

	inline uint TimeToTick(float t)
	{
		return (uint)(t*1000.0f);
	}

typedef HardwarePixelBuffer HardwareTexture;
typedef uint VertexDeclHandle;
typedef MovableObject SceneNode;
typedef size_t ResourceHandle;

#pragma warning(disable : 4251)


}

#endif // __OgrePrerequisites_H__


