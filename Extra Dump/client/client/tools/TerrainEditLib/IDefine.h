
#pragma once
#include "OgreVector3.h"
#include "OgreVector2.h"
#include "OgreMatrix4.h"
#include "OgreColourValue.h"
#include "OgrePlane.h"
#include "OgreRoot.h"
#include "OgreBounding.h"
#include "OgreEntity.h"
#include "OgreRay.h"

class IDefine
{
public:
	IDefine(void);
	~IDefine(void);
};

typedef Ogre::Texture *          LMTex;
typedef Ogre::VertexData *       LMVB;
typedef Ogre::IndexData *        LMIB;
typedef Ogre::Vector3             MVec3;
typedef Ogre::Vector2             MVec2;
typedef Ogre::Vector4             MVec4;
typedef Ogre::WorldPos           MWorldPos;

typedef Ogre::VertexDeclHandle         MIInputLayout;
typedef Ogre::ShaderTechnique    	   MIShader; 

typedef Ogre::Matrix4              MMatrix;
typedef Ogre::Plane                MPlane;
typedef Ogre::Camera   MCameraNode;

typedef struct MBox
{
	MVec3  minVec;
	MVec3  maxVec;
}MBOX;

typedef  struct MRay
{
   MVec3  pos;
   MVec3  dir;
}MRAY;

typedef  struct MBall
{
	MVec3  pos;
	float   rad;
}MBALL;

typedef struct M2DVertex
{
	float x,y,z,rhw;
	unsigned int color;

	M2DVertex(){ x = y = z = rhw = 0 ; color = 0 ;};

	M2DVertex(float _x,float _y,float _z,float _rhw,unsigned int _color)
	{
		x = _x; y = _y; z = _z ; rhw = _rhw ;
		color = _color ;
	};
}M2DVERTEX;


typedef struct ModelVertex
{
	MVec3  pos;
	MVec3  normal;
	MVec2  texcoord;
}MModelV;

typedef struct ChunkVertex
{
	MVec3 pos;
	MVec3 normal;
	unsigned int color;
	MVec2 texcoord;
	MVec2 texcoord2;
}MChunV;

#define MARK_2D_VERTEX_FVF      ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define MARK_MODEL_VERTEX_FVF   ( D3DFVF_XYZ|  D3DFVF_NORMAL | D3DFVF_TEX1 )
#define MARK_CHUNK_VERTEX_FVF   ( D3DFVF_XYZ |D3DFVF_NORMAL | D3DFVF_TEX8 )

#define MSAFE_RELEASE(x)  if(x){x->Release();x=NULL;}
#define MSAFE_DELETE(x)      if(x){delete x; x = NULL;}
#define MSAFE_DELETE_ARRAY(x)  if(x){delete[] x; x = NULL;}

#define NAME_LENGTH   256

#define MAY_PI   3.1415926535f



