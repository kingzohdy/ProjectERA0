
#include "TerBeastManager.h"
#include "EditorCamera.h"
#include "TerrainEdit.h"
#include "OgreHardwareBuffer.h"
#include "MapLights.h"
#include "EditorModel.h"

#include "OgreHardwarePixelBuffer.h"
#include "OgreXMLData.h"
#include "OgreResourceManager.h"
#include "OgreTexture.h"

#include <iostream>
using namespace bex;
using namespace TE;
using namespace Ogre;

int BeastManager::msIndex = 0;
int BeastManager::msLightIndex = 0;
string BeastManager::msMapDir = "";

BeastManager::BeastManager ()
:
m_AmbientColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	mScene = 0;
	mTerrainHead.version = 100;
	mTerrainHead.type = 0;

	mModelHead.version = 100;
	mModelHead.type = 0;

	m_bBeastShow = false;
	mJob = 0;

	m_bUseCameraTarget = false;

	m_Mode = SLM_Texture;
}
//----------------------------------------------------------------------------
BeastManager::~BeastManager ()
{

}
//----------------------------------------------------------------------------
void BeastManager::SetStaticLightMode (StaticLightMode mode)
{
	m_Mode = mode;
}
//----------------------------------------------------------------------------
BeastManager::StaticLightMode BeastManager::GetStaticLightMode ()
{
	return m_Mode;
}
//----------------------------------------------------------------------------
bool BeastManager::Initlize ()
{
	bex::apiCall(ILBSetLogTarget(ILB_LT_ERROR, ILB_LS_STDERR, 0));

#if defined(WIN32)
	//bex::apiCall(ILBSetLogTarget(ILB_LT_INFO, ILB_LS_DEBUG_OUTPUT, 0));
#else
#endif

	bex::apiCall(ILBCreateManager("BeastTemp", ILB_CS_LOCAL, &mManagerHandle));
	bex::apiCall(ILBSetBeastPath(mManagerHandle, "BeastBin"));
	bex::apiCall(ILBClearCache(mManagerHandle));

	TextureData::msNowIsForEditor = true;

	return true;
}
//----------------------------------------------------------------------------
bool BeastManager::Terminate ()
{
	Reset();

	return true;
}
//----------------------------------------------------------------------------
void BeastManager::Reset ()
{
	msIndex = 0;
	mTLightDatas.clear();
	mMLightDatas.clear();
	mTLightMap.clear();
	mTextureMap.clear();
	mGroups.clear();
	mTargets.clear();

	if (mJob)
		apiCall(ILBDestroyJob(mJob));
	mJob = 0;

	if (mScene)
		apiCall(ILBReleaseScene(mScene));
	mScene = 0;

	if (mManagerHandle)
	{
		bex::apiCall(ILBDestroyManager(mManagerHandle));
	}

	bex::apiCall(ILBCreateManager("BeastTemp", ILB_CS_LOCAL, &mManagerHandle));
	bex::apiCall(ILBSetBeastPath(mManagerHandle, "BeastBin"));
	bex::apiCall(ILBClearCache(mManagerHandle));
}
//----------------------------------------------------------------------------
void BeastManager::BeginScene ()
{
	bex::apiCall(ILBBeginScene(mManagerHandle, "BeastScene", &mScene));
}
//----------------------------------------------------------------------------
void BeastManager::AddObject (std::string name, VertexData *pVB, 
							  IndexData *pIB, Ogre::PrimitiveType type, Matrix4 tm,
							  Ogre::Material *pMaterial, 
							  int x, int z, std::string modelFileName, 
							  int modelIndex, int meshIndex, int subMeshIndex)
{
	if (0==pVB->getNumVertex() || 0==pIB->getNumIndex())
		return;

	std::cout << msIndex << endl;

	std::string objectName = name;
	std::string objectMaterialName = objectName+"Material";

	// Mesh
	ILBMeshHandle mesh;
	apiCall(ILBBeginMesh(mManagerHandle, objectName.c_str(), &mesh));

	std::vector<Vec3> positions;
	std::vector<Vec2> uvs;
	std::vector<Vec3> normals;

	// input positions and normals
	Vector3 *pos = (Vector3*)pVB->getPosition(0);
	Vector2 *texcord = (Vector2*)pVB->getVertexElement(0, VES_TEXCOORD);
	Vector3 *normal = (Vector3*)pVB->getNormal(0);
	for (size_t i=0; i<pVB->getNumVertex(); ++i)
	{
		Vector3 pt = tm.transformCoord(pos[0]);
		Vector3 nor = normal[0];

		positions.push_back(Vec3(pt.x, pt.y, -pt.z));
		if (texcord)
		{
			Vector2 uv = texcord[0];
			uvs.push_back(Vec2(uv.x, uv.y));
		}
		normals.push_back(Vec3(nor.x, nor.y, -nor.z));

		pos = (Vector3*)((char*)pos + pVB->m_Stride);
		normal = (Vector3*)((char*)normal + pVB->m_Stride);

		if (texcord)
			texcord = (Vector2*)((char*)texcord + pVB->m_Stride);
	}

	apiCall(ILBAddVertexData(mesh, &positions[0], &normals[0], (int32)positions.size()));

	// Material
	float max0 = max(m_AmbientColor.r, m_AmbientColor.g);
	float max1 = max(max0, m_AmbientColor.b);

	//bex::ColorRGBA colorEmmsive((m_AmbientColor.r/max1)*0.1f+0.1f, 
	//	(m_AmbientColor.g/max1)*0.1f+0.1f, 
	//	(m_AmbientColor.b/max1)*0.1f+0.1f, 1.0f);

	bex::ColorRGBA colorEmmsive(m_AmbientColor.r, m_AmbientColor.g, m_AmbientColor.b, 1.0f);

	//bex::ColorRGBA colorEmmsive(0.1f, 0.1f, 0.1f, 1.0f);
	bex::ColorRGBA colorDiffuse(0.3f, 0.3f, 0.3f, 1.0f);
	bex::ColorRGBA colorSpecular(0.6f, 0.6f, 0.6f, 1.0f);
	ILBMaterialHandle materialHandle;

	bex::apiCall(ILBCreateMaterial(mScene, objectMaterialName.c_str(), &materialHandle));

	bool inputedTexture = false;

	// 当材质有alpha测试时，才需要传入纹理
	if (pMaterial)
	{
		int blendMode = pMaterial->GetParamMacro("BLEND_MODE");

		if (BLEND_ALPHATEST == blendMode)
		{
			Ogre::Texture *texture = 0;
			texture = pMaterial->GetParamTexture("g_DiffuseTex");

			ILBTextureHandle tex;

			if (mTextureMap.find(texture) != mTextureMap.end())
			{
				tex = mTextureMap[texture];
			}
			else
			{
				std::string texName = GetRandName("Textrue") + "tex";
				tex = CreateBeastTexture(texName, texture);
				mTextureMap[texture] = tex;
			}

			bex::apiCall(ILBSetMaterialTexture(materialHandle, ILB_CC_DIFFUSE, tex));
			bex::apiCall(ILBSetAlphaAsTransparency(materialHandle, true));

			inputedTexture = true;
		}			
	}

	if (!inputedTexture)
		bex::apiCall(ILBSetMaterialColor(materialHandle, ILB_CC_DIFFUSE, &colorDiffuse));

	bex::apiCall(ILBSetMaterialColor(materialHandle, ILB_CC_SPECULAR, &colorSpecular));
	bex::apiCall(ILBSetMaterialColor(materialHandle, ILB_CC_EMISSIVE, &colorEmmsive));

	// MeshInstance
	if (pIB)
	{
		std::vector<int32> indexs;

		apiCall(ILBBeginMaterialGroup(mesh, objectMaterialName.c_str()));

		unsigned short *indices = (unsigned short *)pIB->lock();
                  
		// IndexBuffer
		if (type == PRIM_TRIANGLESTRIP)
		{
			bool s = false;

			for (size_t i=2; i<pIB->getNumIndex(); ++i)
			{
				if (s)
				{
					indexs.push_back(indices[i-2]);
					indexs.push_back(indices[i-1]);
					indexs.push_back(indices[i]);
				}
				else
				{
					indexs.push_back(indices[i-1]);
					indexs.push_back(indices[i-2]);
					indexs.push_back(indices[i]);
				}

				s = !s;
			}
		}
		else if (type == PRIM_TRIANGLELIST)
		{
			for (size_t i=0; i<pIB->getNumIndex()/3; ++i)
			{
				indexs.push_back(indices[i*3+1]);
				indexs.push_back(indices[i*3]);
				indexs.push_back(indices[i*3+2]);
			}
		}

		pIB->unlock();

		apiCall(ILBAddTriangleData(mesh, (const int32 *)&indexs[0],
			(int)indexs.size()));

		apiCall(ILBEndMaterialGroup(mesh));

		// Add uv's
		if ((int)uvs.size() > 0)
		{
			apiCall(ILBBeginUVLayer(mesh, _T("uv1")));
			apiCall(ILBAddUVData(mesh, &uvs[0], (int32)uvs.size()));
			apiCall(ILBEndUVLayer(mesh));
		}
	}

	apiCall(ILBEndMesh(mesh));

	ILBInstanceHandle meshInstance;
	bex::Matrix4x4 trans = bex::identity();

	bex::apiCall(ILBCreateInstance(mScene, mesh, objectName.c_str(), &trans, &meshInstance));
	bex::apiCall(ILBSetRenderStats(meshInstance, ILB_RS_SHADOW_BIAS, ILB_RSOP_ENABLE));

	mGroups[objectName].hMeshInstance = meshInstance;
	mGroups[objectName].ObjectName = objectName;
	mGroups[objectName].mVertexData = pVB;
	mGroups[objectName].mIndexBuffer = pIB;
	mGroups[objectName].x = x;
	mGroups[objectName].z = z;
	mGroups[objectName].modelFileName = modelFileName;
	mGroups[objectName].modelIndex = modelIndex;
	mGroups[objectName].meshIndex = meshIndex;
	mGroups[objectName].subMeshIndex = subMeshIndex;

	if (m_Mode == BeastManager::SLM_Texture)
	{
		if (x != -1)
		{
			mGroups[objectName].IsTextureTarget = true;
		}
	}
}
//----------------------------------------------------------------------------
void BeastManager::AddDirectLight (std::string name, Vector3 dir, ColourValue color)
{
	ILBLightHandle lightHandle;

	apiCall(ILBCreateDirectionalLight(mScene, 
		name.c_str(),
		&directionalLightOrientation(Vec3(dir.x, dir.y, -dir.z)),
		&bex::ColorRGB(color.r, color.g, color.b),
		&lightHandle));

	bex::apiCall(ILBSetCastShadows(lightHandle, true));
	bex::apiCall(ILBSetShadowSamples(lightHandle, 32));
	bex::apiCall(ILBSetShadowAngle(lightHandle, 0.0f));

	m_DirectionColor = color;
}
//----------------------------------------------------------------------------
void BeastManager::SetAmbientColor (std::string name, Ogre::ColourValue color)
{
	m_AmbientColor.r = color.r;
	m_AmbientColor.g = color.g;
	m_AmbientColor.b = color.b;
	m_AmbientColor.a = 1.0f;
}
//----------------------------------------------------------------------------
void BeastManager::AddPointLight (std::string name, Ogre::Vector3 pos,
								  Ogre::ColourValue color, float fRange)
{
	ILBLightHandle lightHandle;

	apiCall(ILBCreatePointLight(mScene,
		name.c_str(),
		&bex::translation(bex::Vec3(pos.x, pos.y, -pos.z)),
		&bex::ColorRGB(color.r, color.g, color.b),
		&lightHandle));

	bex::apiCall(ILBSetCastShadows(lightHandle, true));
	bex::apiCall(ILBSetShadowSamples(lightHandle, 32));
	bex::apiCall(ILBSetShadowRadius(lightHandle, 0.0f));
	bex::apiCall(ILBSetFalloff(lightHandle, ILB_FO_MAX_RANGE, 2.0f, fRange, true));
}
//----------------------------------------------------------------------------
void BeastManager::SetCamera (std::string name, EditorCamera *camera)
{
	Vector3 pos = camera->GetPosition();

	Vector3 dir;
	Vector3 up;
	Vector3 right;
	camera->getCameraAxis(dir, up, right);

	Vec3 camPos = Vec3(pos.x, pos.y, -pos.z);
	Vec3 camDir = Vec3(dir.x, dir.y, -dir.z);
	Vec3 camUp = Vec3(0.0f, 1.0f, 0.0f);

	bex::apiCall(ILBCreatePerspectiveCamera(mScene, name.c_str(),
		&bex::setCameraMatrix(camPos, camDir,camUp), &mCamera));
}
//----------------------------------------------------------------------------
void BeastManager::SetUseCameraTarget (bool use)
{
	m_bUseCameraTarget = use;
}
//----------------------------------------------------------------------------
bool BeastManager::IsUseCameraTarget ()
{
	return m_bUseCameraTarget;
}
//----------------------------------------------------------------------------
ILBTextureHandle BeastManager::CreateBeastTexture (std::string name, Ogre::Texture *texture)
{
	TextureDesc desc;
	texture->getDesc(desc);

	int width = desc.width;
	int height = desc.height;

	ILBTextureHandle tex;

	apiCall(ILBBeginTexture(mManagerHandle,
		name.c_str(),
		width,
		height,
		ILB_PF_RGBA_BYTE,
		&tex));

	char *color = 0;
	size_t length = 0;
	int format = 0;

	std::string dumpPath = "TempDDs//" + GetRandName("dumpdds") + ".dds";	
	texture->getHardwareTexture()->dumpToDDS(dumpPath.c_str());

	TextureData *texData = (TextureData*)texture;
	color = (char*)texData->convertDDSFile(dumpPath, format);

	if (color && format!=0 && format !=5)
	{
		const int components = 4;
		apiCall(ILBSetInputGamma(tex, ILB_IG_GAMMA, 2.2f));
		std::vector<unsigned char> lineData(width * components);
		for (int y = 0; y < height; ++y) 
		{
			for (int x = 0; x < width; ++x) 
			{
				// format == 0    IL_BGR
				// format == 1    IL_BGRA
				// format == 3    PF_A8R8G8B8
				// format == 4    PF_B8G8R8A8
				// format == 5    else

				char r, g, b, a;
				b = color[x * components + 0];
				g = color[x * components + 1];
				r = color[x * components + 2];
				a = color[x * components + 3];


				lineData[x * components + 0] = r;
				lineData[x * components + 1] = g;
				lineData[x * components + 2] = b;
				lineData[x * components + 3] = a;
			}
			apiCall(ILBAddPixelDataLDR(tex, &lineData[0], width));

			color += 4*width;
		}
	}

	apiCall(ILBEndTexture(tex));

	return tex;
}
//----------------------------------------------------------------------------
void BeastManager::EndScene ()
{
	ILBEndScene(mScene);
}
//----------------------------------------------------------------------------
void BeastManager::CreateXML (const std::string &fileName, bool gi)
{
	std::ofstream ofs(fileName.c_str(), std::ios_base::out | std::ios_base::trunc);
	XMLWriter xml(ofs);

	{ScopedTag _x(xml, "ILConfig");
		{ScopedTag _x(xml, "AASettings");
			xml.data("minSampleRate", 0);
			xml.data("maxSampleRate", 2);
		}
		{ScopedTag _x(xml, "RenderSettings");
			xml.data("bias", 0.0001f);
		}
		{ScopedTag _x(xml, "VertexBakeSettings");
			{ScopedTag _x(xml, "VertexBakeSamplingOptions");
				xml.data("Type", "TriangleSubdiv");
				xml.data("MinSamples", 0);
				xml.data("MaxSamples", 0);
				xml.data("SubDivStepSize", 0);
			}
		}
		if (gi) {
			{ScopedTag _x(xml, "EnvironmentSettings");
				xml.data("giEnvironment", "SkyLight");
				xml.data<ILBLinearRGBA>("skyLightColor", bex::ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		{ScopedTag _x(xml, "FrameSettings");
			xml.data("inputGamma", 1.0);
				{ScopedTag _x(xml, "outputCorrection");
					xml.data("colorCorrection", "Gamma");
					xml.data("gamma", 1.0);
				}
		}
		if (gi) {
			{ScopedTag _x(xml, "GISettings");
				xml.data("enableGI", true);
				xml.data("fgRays", 1000);
				xml.data("fgContrastThreshold", 0.1);
				xml.data("fgInterpolationPoints", 2);
				xml.data("primaryIntegrator", "FinalGather");
				xml.data("secondaryIntegrator", "None");
			}
		}
	}

	mXMLFilename = fileName;
}
//----------------------------------------------------------------------------
bool BeastManager::DoBake ()
{
	bex::apiCall(ILBCreateJob(mManagerHandle, "BakeJob", mScene, 
		mXMLFilename.c_str(), &mJob));
	bex::apiCall(ILBCreateFullShadingPass(mJob, "fullShading", &mPass));

	std::cout << "DoBake::CreateAllTarget" << endl;
	CreateAllTarget();

	if (m_bUseCameraTarget)
	{
		std::cout << "DoBake::CreateCameraTarget" << endl;
		CreateCameraTarget();
	}

	AddPassToTargets();

	std::cout << "DoBake::bex::renderJob";
	return bex::renderJob(mJob, std::cout, true, false);

	return true;
}
//----------------------------------------------------------------------------
void BeastManager::CollectLightData ()
{
	mTLightDatas.clear();
	mMLightDatas.clear();
	mTLightMap.clear();
	mTextureMap.clear();

	VBGROUP::iterator it = mGroups.begin();

	for (; it!=mGroups.end(); ++it)
	{		
		BGroup group = it->second;
		int x = group.x;
		int z = group.z;
		std::string modelFilename = group.modelFileName;
		int modelIndex = group.modelIndex;
		int meshIndex = group.meshIndex;
		int subMeshIndex = group.subMeshIndex;
		bool isTextureTarget = group.IsTextureTarget;

		if (isTextureTarget)
		{
			ILBTextureHandle tex;

			tex = bex::copyFrameBuffer(mManagerHandle, group.hTarget, mPass, 
				group.ObjectName, false);
		}
		else
		{
			std::vector<float> colors;
			bex::copyVertexBuffer(group.hTarget, mPass, group.hEntity, colors);
			int vertexNum = (int)colors.size()/4;

			if (x != -1)
			{
				BTLightData tLD;

				for (int i=0; i<vertexNum; i++)
				{
					ColourValue cl;
					cl.r = colors[i*4];
					cl.g = colors[i*4+1];
					cl.b = colors[i*4+2];
					cl.a = 1.0f;
					ColorQuad cq(cl);

					tLD.colors.push_back(cq.c);
				}

				// 地形光照保存
				mTerrainHead.number++;

				tLD.xIndex = x;
				tLD.zIndex = z;
				tLD.number = vertexNum;
				mTLightDatas.push_back(tLD);
			}
			else
			{
				BMLightData mLD;

				for (int i=0; i<vertexNum; i++)
				{
					ColourValue cl;
					cl.r = colors[i*4];
					cl.g = colors[i*4+1];
					cl.b = colors[i*4+2];
					cl.a = 1.0f;
					ColorQuad cq(cl);

					mLD.colors.push_back(cq.c);
				}

				mModelHead.number++;
				mLD.modelFilename = modelFilename;
				mLD.modelIndex = modelIndex;
				mLD.meshIndex = meshIndex;
				mLD.subMeshIndex = subMeshIndex;
				mLD.number = vertexNum;
				mMLightDatas.push_back(mLD);
			}
		}
	}

	std::string destDir = msMapDir;

	std::string systemString = "del " + destDir + "\*.png"; 
	system(systemString.c_str());

	systemString = "copy /y BeastTemp\\scenes\\BeastScene.xml " + destDir; 
	system(systemString.c_str());

	// 复制需要的图片
	CopyUsefulTexture(msMapDir);

	LoadLightMapXML(msMapDir);
}
//----------------------------------------------------------------------------
//void BeastManager::CopyToVisual ()
//{
//	VBGROUP::iterator it = mGroups.begin();
//
//	for (; it!=mGroups.end(); ++it)
//	{		
//		BGroup group = it->second;
//		int x = group.x;
//		int z = group.z;
//		std::string modelFilename = group.modelFileName;
//		int modelIndex = group.modelIndex;
//		int meshIndex = group.meshIndex;
//		int subMeshIndex = group.subMeshIndex;
//		VertexData *vData = group.mVertexData;
//		IndexBuffer *iBuffer = group.mIndexBuffer;
//
//		std::vector<float> colors;
//		bex::copyVertexBuffer(group.hTarget, mPass, group.hEntity, colors);
//
//		MTileData *titleData = 0;
//		titleData = g_TerrainEdit.GetEditData();
//		if (!titleData)
//			continue;
//
//		if (x!=-1)
//		{
//			BTLightData tLD;
//
//			// 渲染
//			if (!vData)
//				continue;
//
//			int vertexNum = vData->getNumVertex();
//			assert(vertexNum*4 == (int)colors.size());
//
//			TerrainVertex *tv = (TerrainVertex*)vData->lock();
//
//			for (int i=0; i<vertexNum; i++)
//			{
//				ColourValue cl;
//				cl.r = colors[i*4];
//				cl.g = colors[i*4+1];
//				cl.b = colors[i*4+2];
//				cl.a = 1.0f;
//				ColorQuad cq(cl);
//
//				// render
//				tv[i].color = cq.c;
//
//				tLD.colors.push_back(cq.c);
//			}
//
//			vData->unlock();
//
//			// 地形光照保存
//			mTerrainHead.number++;
//
//			tLD.xIndex = x;
//			tLD.zIndex = z;
//			tLD.number = vertexNum;
//			mTLightDatas.push_back(tLD);
//		}
//		else
//		{
//			BMLightData mLD;
//
//			int vertexNum = vData->getNumVertex();
//			assert(vertexNum*4 == (int)colors.size());
//
//			for (int i=0; i<vertexNum; i++)
//			{
//				unsigned int *color = 0;
//				color = (unsigned int*)vData->getColor(i);
//
//				if (color)
//				{
//					ColourValue cl;
//					cl.r = colors[i*4];
//					cl.g = colors[i*4+1];
//					cl.b = colors[i*4+2];
//					cl.a = 1.0f;
//					ColorQuad cq(cl);
//
//					*color = cq.c;
//					mLD.colors.push_back(cq.c);
//				}
//			}
//
//			if (vData->m_pVB)
//				vData->m_pVB->m_DataInvalid = true;
//
//			mModelHead.number++;
//			mLD.modelFilename = modelFilename;
//			mLD.modelIndex = modelIndex;
//			mLD.meshIndex = meshIndex;
//			mLD.subMeshIndex = subMeshIndex;
//			mLD.number = vertexNum;
//			mMLightDatas.push_back(mLD);
//		}
//	}
//}
//----------------------------------------------------------------------------
//bool BeastManager::SaveTerrainLightData (const std::string fileName)
//{
//	if ((int)mTLightDatas.size() == 0)
//		return false;
//
//	FILE *fp = fopen(fileName.c_str(), "wb");
//	if(fp == NULL) 
//		return false;
//
//	mTerrainHead.number = mTLightDatas.size();
//	mTerrainHead.type = 0;
//	fwrite(&mTerrainHead, sizeof(mTerrainHead), 1, fp);
//
//	for (int i=0; i<(int)mTLightDatas.size(); i++)
//	{
//		BTLightData data = mTLightDatas[i];
//		fwrite(&data.xIndex, sizeof(int), 1, fp);
//		fwrite(&data.zIndex, sizeof(int), 1, fp);
//		fwrite(&data.number, sizeof(int), 1, fp);
//		fwrite(&data.colors[0], sizeof(unsigned int)*data.number, 1, fp);
//	}
//
//	fclose(fp);
//
//	return true;
//}
////----------------------------------------------------------------------------
//bool BeastManager::LoadTerrainLightData (const std::string fileName)
//{
//	FILE *fp = fopen(fileName.c_str(), "rb");
//	if(fp == NULL) 
//		return false;
//
//	mTLightDatas.clear();
//
//	BHeader terrainHead;
//	fread(&terrainHead, sizeof(terrainHead), 1, fp);
//
//	for (int i=0; i<terrainHead.number; i++)
//	{
//		BTLightData tLD;
//
//		if (100 == terrainHead.version)
//		{
//			int xIndex = -1;
//			fread(&xIndex, sizeof(xIndex), 1, fp);
//			int zIndex = -1;
//			fread(&zIndex, sizeof(zIndex), 1, fp);
//			int vertexNum = 0;
//			fread(&vertexNum, sizeof(vertexNum), 1, fp);
//			vector<unsigned int> colors;
//			colors.resize(vertexNum);
//			fread(&colors[0], sizeof(unsigned int)*vertexNum, 1, fp);
//
//			tLD.xIndex = xIndex;
//			tLD.zIndex = zIndex;
//			tLD.number = vertexNum;
//			tLD.colors = colors;
//		}
//
//		mTLightDatas.push_back(tLD);
//	}
//
//	fclose(fp);
//
//	return true;
//}
//----------------------------------------------------------------------------
void BeastManager::CopyUsefulTexture(std::string mapDir)
{
	std::string xmlName = mapDir+"BeastScene.xml";

	XMLData xmldata;

	if(!xmldata.loadFile(xmlName))
	{
		return;
	}

	XMLNode rootNode = xmldata.getRootNode();

	if (!rootNode.isNull())
	{
		XMLNode instanceNode = rootNode.iterateChild();

		while (!instanceNode.isNull())
		{
			const char *name = instanceNode.getName();
			std::string strName(name);

			if ("Instance" == strName)
			{
				XMLNode childNode = instanceNode.getChild("Name");
				if (!childNode.isNull())
				{
					const char *text = childNode.getText();
					std::string modeName(text);

					bool isTerrain = (string::npos != modeName.find("Terrain"));
					if (isTerrain)
					{
						int pos0 = modeName.find_first_of('_');
						int pos1 = modeName.find_last_of('_');

						int xSize = pos1 - pos0 - 1;
						int zSize = modeName.size() - pos1 - 1;

						char X[12];
						char Z[12];
						modeName.copy(X, xSize, pos0+1);
						modeName.copy(Z, zSize, pos1+1);
						X[xSize] = '\0';
						Z[zSize] = '\0';

						int terX = atoi(X);
						int terZ = atoi(Z);

						int a = 0;

						XMLNode shapeNode = instanceNode.getChild("Shape");

						const char *shapeText = shapeNode.getText();
						std::string localtextureName(shapeText);
						std::string textureName = localtextureName + ".png";

						std::string systemString = "copy /y BeastTemp\\textures\\"+textureName + " " + mapDir; 
						system(systemString.c_str());
					}
				}
			}

			instanceNode = rootNode.iterateChild(instanceNode);
		}
	}

	return;
}
//----------------------------------------------------------------------------
bool BeastManager::LoadLightMapXML (const std::string fileName)
{
	std::string xmlName = fileName+"BeastScene.xml";

	XMLData xmldata;

	if(!xmldata.loadFile(xmlName))
	{
		return false;
	}

	for (int i=0; i<(int)mTLightMap.size(); i++)
	{
		if (mTLightMap[i].texture)
			mTLightMap[i].texture->release();
	}

	mTLightMap.clear();

	XMLNode rootNode = xmldata.getRootNode();

	if (!rootNode.isNull())
	{
		XMLNode instanceNode = rootNode.iterateChild();

		while (!instanceNode.isNull())
		{
			const char *name = instanceNode.getName();
			std::string strName(name);

			if ("Instance" == strName)
			{
				XMLNode childNode = instanceNode.getChild("Name");
				if (!childNode.isNull())
				{
					const char *text = childNode.getText();
					std::string modeName(text);

					bool isTerrain = (string::npos != modeName.find("Terrain"));
					if (isTerrain)
					{
						int pos0 = modeName.find_first_of('_');
						int pos1 = modeName.find_last_of('_');

						int xSize = pos1 - pos0 - 1;
						int zSize = modeName.size() - pos1 - 1;

						char X[12];
						char Z[12];
						modeName.copy(X, xSize, pos0+1);
						modeName.copy(Z, zSize, pos1+1);
						X[xSize] = '\0';
						Z[zSize] = '\0';

						int terX = atoi(X);
						int terZ = atoi(Z);

						int a = 0;

						XMLNode shapeNode = instanceNode.getChild("Shape");

						const char *shapeText = shapeNode.getText();
						std::string localtextureName(shapeText);
						std::string textureName = fileName + localtextureName + ".png";

						// 加载贴图
						Ogre::TextureData *texture= (Ogre::TextureData*)(
							ResourceManager::getSingleton().blockLoad(textureName.c_str()));
						if (texture)
						{
							BTLightMap lmap;
							lmap.x = terX;
							lmap.z = terZ;
							lmap.texture = texture;
							lmap.textureName = localtextureName + ".png";
							mTLightMap.push_back(lmap);
						}
					}
				}
			}

			instanceNode = rootNode.iterateChild(instanceNode);
		}
	}

	return true;
}
//----------------------------------------------------------------------------
bool BeastManager::SaveTerrainLightMapData (const std::string fileName)
{
	if ( (int)mTLightMap.size() == 0)
		return false;

	FILE *fp = fopen(fileName.c_str(), "wb");
	if (fp == NULL)
		return false;

	mTerrainLightMapHead.number = mTLightMap.size();
	mTerrainLightMapHead.type = 3;
	fwrite(&mTerrainLightMapHead, sizeof(mTerrainLightMapHead), 1, fp);

	for (int i=0; i<(int)mTLightMap.size(); i++)
	{
		BTLightMap data = mTLightMap[i];
		int x = data.x;
		int z = data.z;
		int nameSize = data.textureName.size();
		std::string textureName = data.textureName;

		fwrite(&x, sizeof(x), 1, fp);
		fwrite(&z, sizeof(z), 1, fp);
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		fwrite(&textureName[0], sizeof(unsigned char)*(int)textureName.size(), 1, fp);
	}

	fclose(fp);

	return true;
}
//----------------------------------------------------------------------------
bool BeastManager::LoadTerrainLightMapData (const std::string fileName)
{
	FILE *fp = fopen(fileName.c_str(), "rb");
	if (fp == NULL)
		return false;

	mTLightMap.clear();

	BHeader termapHead;
	fread(&termapHead, sizeof(termapHead), 1, fp);

	for (int i=0; i<termapHead.number; i++)
	{
		BTLightMap tLD;

		if (100 == termapHead.version)
		{
			int x;
			fread(&x, sizeof(x), 1, fp);
			int z;
			fread(&z, sizeof(z), 1, fp);
			int nameSize;
			fread(&nameSize, sizeof(nameSize), 1, fp);
			std::string textureName;
			textureName.resize(nameSize);
			fread(&textureName[0], sizeof(char)*nameSize, 1, fp);

			std::string loadName = msMapDir + textureName;
			Ogre::TextureData *texture= (Ogre::TextureData*)(ResourceManager::getSingleton().blockLoad(loadName.c_str()));
			if (texture)
			{
				tLD.x = x;
				tLD.z = z;
				tLD.nameLength = nameSize;
				tLD.textureName = textureName;
				tLD.texture = texture;
			}
		}

		mTLightMap.push_back(tLD);
	}

	if ((int)mTLightMap.size() == 0)
		return false;

	fclose(fp);

	return true;
}
//----------------------------------------------------------------------------
bool BeastManager::SaveModelLightData (const std::string fileName)
{
	if (!LoadLightMapXML(msMapDir))
		return false;

	if ((int)mMLightDatas.size() == 0)
		return false;

	FILE *fp = fopen(fileName.c_str(), "wb");
	if(fp == NULL) 
		return false;

	mModelHead.number = mMLightDatas.size();
	mModelHead.type = 1;
	fwrite(&mModelHead, sizeof(mModelHead), 1, fp);

	for (int i=0; i<(int)mMLightDatas.size(); i++)
	{
		BMLightData data = mMLightDatas[i];
		int nameSize = data.modelFilename.size();
		fwrite(&nameSize, sizeof(nameSize), 1, fp);
		fwrite(&data.modelFilename[0], nameSize, 1, fp);
		fwrite(&data.modelIndex, sizeof(int), 1, fp);
		fwrite(&data.meshIndex, sizeof(int), 1, fp);
		fwrite(&data.subMeshIndex, sizeof(int), 1, fp);
		fwrite(&data.number, sizeof(int), 1, fp);
		fwrite(&data.colors[0], sizeof(unsigned int)*data.number, 1, fp);
	}

	fclose(fp);

	return true;
}
//----------------------------------------------------------------------------
bool BeastManager::LoadModelLightData (const std::string fileName)
{
	FILE *fp = fopen(fileName.c_str(), "rb");
	if(fp == NULL) 
		return false;

	for (int i=0; i<(int)mTLightMap.size(); i++)
	{
		if (mTLightMap[i].texture)
			mTLightMap[i].texture->release();
	}

	mMLightDatas.clear();

	BHeader modelHead;
	fread(&modelHead, sizeof(modelHead), 1, fp);

	for (int i=0; i<modelHead.number; i++)
	{
		BMLightData mLD;

		std::string modelFilename;
		int modelIndex;
		int meshIndex;
		int subMeshIndex;
		int number;
		std::vector<unsigned int> colors;

		if (100 == modelHead.version)
		{
			int nameSize;
			fread(&nameSize, sizeof(nameSize), 1, fp);
			std::string modelName;
			modelName.resize(nameSize);
			fread(&modelName[0], sizeof(char)*nameSize, 1, fp);
			int modelIndex;
			fread(&modelIndex, sizeof(int), 1, fp);
			int meshIndex;
			fread(&meshIndex, sizeof(int), 1, fp);
			int subMeshIndex;
			fread(&subMeshIndex, sizeof(int), 1, fp);
			int number;
			fread(&number, sizeof(int), 1, fp);
			std::vector<unsigned int> colors;
			colors.resize(number);
			fread(&colors[0], sizeof(unsigned int)*number, 1, fp);

			mLD.modelFilename = modelName;
			mLD.modelIndex = modelIndex;
			mLD.meshIndex = meshIndex;
			mLD.subMeshIndex = subMeshIndex;
			mLD.number = number;
			mLD.colors = colors;
		}

		mMLightDatas.push_back(mLD);
	}

	fclose(fp);

	return true;
}
//----------------------------------------------------------------------------
void BeastManager::SetBeastShow (bool show)
{
	m_bBeastShow = show;

	if (m_bBeastShow)
	{
		MTileData *titleData = 0;
		titleData = g_TerrainEdit.GetEditData();
		if (!titleData)
			return;

		if (m_Mode == SLM_Vertex)
		{
			//for (int i=0; i<(int)mTLightDatas.size(); i++)
			//{
			//	int xIndex = mTLightDatas[i].xIndex;
			//	int zIndex = mTLightDatas[i].zIndex;
			//	int number = mTLightDatas[i].number;
			//	vector<unsigned int> colors = mTLightDatas[i].colors;

			//	if (-1 != xIndex)
			//	{

			//		EditorTerrainBlock &block = titleData->chunkSet[zIndex][xIndex];
			//		TerrBlockVertTable *vertexTable = block.GetChunkNewVecData();
			//		if (!vertexTable)
			//			continue;

			//		if (vertexTable)
			//		{
			//			for (int i=0; i<number; i++)
			//			{
			//				vertexTable->vVer[i].color = colors[i];
			//			}
			//		}

			//		// 渲染
			//		Ogre::VertexData *vData = block.m_ChunkRenderData.vb;
			//		if (!vData)
			//			continue;
			//		TerrainVertex *tv = (TerrainVertex*)vData->lock();
			//		for (int i=0; i<number; i++)
			//		{
			//			// render
			//			tv[i].color = colors[i];
			//		}
			//		vData->unlock();
			//	}
			//}
		}
		else
		{
			for (int i=0; i<(int)mTLightMap.size(); i++)
			{
				int x = mTLightMap[i].x;
				int z = mTLightMap[i].z;

				EditorTerrainBlock &block = titleData->chunkSet[z][x];
				block.m_pLightMap = mTLightMap[i].texture;
				block.m_LightMapTexturename = mTLightMap[i].textureName;
			}
		}

		for (int i=0; i<(int)mMLightDatas.size(); i++)
		{
			std::string modelFilename = mMLightDatas[i].modelFilename;
			int modelIndex = mMLightDatas[i].modelIndex;
			int meshIndex = mMLightDatas[i].meshIndex;
			int subMeshIndex = mMLightDatas[i].subMeshIndex;
			int number = mMLightDatas[i].number;
			std::vector<unsigned int> colors = mMLightDatas[i].colors;

			for (int j=0; j<titleData->modelData.modelSet.size(); j++)
			{
				TerrModel *pTerrModel = titleData->modelData.modelSet[j];
				if (modelIndex == j)
				{
					TerrModelData *pTerrModelData = pTerrModel->GetModelData();
					EditorModel *pEditorModel = pTerrModelData->pModel;
					Model *pModel = pEditorModel->GetShowModel()->getMainModel();
					pModel->setCanUseStaticLight(true);
					ModelData *pModelData = 0;
					if (pModel)
					{
						pModelData = pModel->getModelData();
					}

					MeshInstance *pMeshInst = 0;
					SubMeshInstance *pSubMeshInst = 0;
					if (meshIndex >= 0)
					{
						pMeshInst = pModel->getIthSkin(meshIndex);

						if (subMeshIndex >= 0 && pMeshInst)
						{
							pSubMeshInst = pMeshInst->getSubMesh(subMeshIndex);	

							pSubMeshInst->m_Colors = colors;
							
							pSubMeshInst->setCanUseStaticLight(true);
							pSubMeshInst->SwitchToStaticLight(true);

							VertexData *vertexData = pSubMeshInst->m_pInstVDBake;
							if (vertexData->m_pVB)
								vertexData->m_pVB->m_DataInvalid = true;
						}
					}
				}
			}
		}
	}
	else
	{
		MTileData *titleData = 0;
		titleData = g_TerrainEdit.GetEditData();
		if (!titleData)
			return;

		for (int i=0; i<(int)mMLightDatas.size(); i++)
		{
			std::string modelFilename = mMLightDatas[i].modelFilename;
			int modelIndex = mMLightDatas[i].modelIndex;
			int meshIndex = mMLightDatas[i].meshIndex;
			int subMeshIndex = mMLightDatas[i].subMeshIndex;
			int number = mMLightDatas[i].number;
			std::vector<unsigned int> colors = mMLightDatas[i].colors;

			for (int j=0; j<titleData->modelData.modelSet.size(); j++)
			{
				TerrModel *pTerrModel = titleData->modelData.modelSet[j];
				if (modelIndex == j)
				{
					TerrModelData *pTerrModelData = pTerrModel->GetModelData();
					EditorModel *pEditorModel = pTerrModelData->pModel;
					Model *pModel = pEditorModel->GetShowModel()->getMainModel();
					ModelData *pModelData = 0;
					if (pModel)
					{
						pModelData = pModel->getModelData();
					}

					MeshInstance *pMeshInst = 0;
					SubMeshInstance *pSubMeshInst = 0;
					if (meshIndex >= 0)
					{
						pMeshInst = pModel->getIthSkin(meshIndex);

						if (subMeshIndex >= 0 && pMeshInst)
						{
							pSubMeshInst = pMeshInst->getSubMesh(subMeshIndex);	
							pSubMeshInst->SwitchToStaticLight(false);
						}
					}
				}
			}
		}

		g_LightManager.flushAllLights(false);
	}
}
//----------------------------------------------------------------------------
bool BeastManager::IsBeastLightShow ()
{
	return m_bBeastShow;
}
//----------------------------------------------------------------------------
void BeastManager::CreateAllTarget ()
{
	VBGROUP::iterator it = mGroups.begin();

	for (; it!=mGroups.end(); ++it)
	{
		if (!it->second.IsTextureTarget)
		{
			std::string name = it->second.ObjectName;
			name += "VertexTarget";

			ILBTargetHandle target;
			bex::apiCall(ILBCreateVertexTarget(mJob, name.c_str(), &target));

			ILBTargetEntityHandle entity;
			bex::apiCall(ILBAddBakeInstance(target, it->second.hMeshInstance, &entity));

			it->second.hTarget = target;
			it->second.hEntity = entity;

			mTargets.push_back(target);
		}
		else
		{
			std::string name = it->second.ObjectName;

			if (name.find("Terrain") != string::npos)
			{
				name += "TextureTarget";

				ILBTargetHandle target;
				bex::apiCall(ILBCreateTextureTarget(mJob, name.c_str(), 128, 128, &target));

				ILBTargetEntityHandle entity;
				bex::apiCall(ILBAddBakeInstance(target, it->second.hMeshInstance, &entity));

				it->second.hTarget = target;
				it->second.hEntity = entity;

				mTargets.push_back(target);
			}
		}
	}
}
//----------------------------------------------------------------------------
void BeastManager::CreateCameraTarget ()
{
	ILBTargetHandle target;
	bex::apiCall(ILBCreateCameraTarget(mJob, "cameraTarget", mCamera, 800, 600,
		&target));

	mTargets.push_back(target);
}
//----------------------------------------------------------------------------
void BeastManager::AddPassToTargets ()
{
	for (int i=0; i<(int)mTargets.size(); i++)
	{
		apiCall(ILBAddPassToTarget(mTargets[i], mPass));
	}
}
//----------------------------------------------------------------------------
std::string BeastManager::GetTerrainName (std::string hitName, int x, int y)
{
	char temp[128];
	sprintf(temp, "_%d_%d", x, y);

	return (hitName+std::string(temp));
}
//----------------------------------------------------------------------------
std::string BeastManager::GetRandName (std::string hitName)
{
	msIndex++;

	char temp[128];
	sprintf(temp, "%d", msIndex);

	return (hitName+std::string(temp));
}
//----------------------------------------------------------------------------
std::string BeastManager::GetRandLightName (std::string hitName)
{
	msLightIndex++;

	char temp[128];
	sprintf(temp, "%d", msLightIndex);

	return (hitName+std::string(temp));
}
//----------------------------------------------------------------------------