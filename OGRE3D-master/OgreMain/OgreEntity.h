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
#ifndef __Entity_H__
#define __Entity_H__

#include "OgreMovableObject.h"
#include "OgreRenderableObject.h"
#include "OgreKeyFrameArray.h"
#include "OgreEntityData.h"
#include "OgreModel.h"
#include <map>

namespace Ogre
{
	struct ANIM_PLAY
	{
		int iSeq;
		int iPriority;
		float fRestTime;
		int iLoopMod;
	};
	struct ANIM_MERGE
	{
		int iSeq;
		int priority;
		int fallback;
		int loopmode;
	};
	/*
	class _OgreExport AnimMap
	{
	public:
	AnimMap();
	~AnimMap();

	bool loadAnimNamesFromFile(const char* fileName);
	ANIM_MERGE *getAnimDepart(int iMainSeq);

	private:
	std::map<int, ANIM_MERGE>m_AnimDepartMap;

	void			clearBuffer();
	};
	*/
	enum ACTION_TYPE
	{
		AT_BONE_ANIM,
		AT_MOTION
	};

	struct ACTION_INFO
	{
		FixedString name;
		ACTION_TYPE type;
		bool bPersist;
		float fTime;
	};

	class _OgreExport Entity : public RenderableObject, public LoadWrap
	{
		DECLARE_RTTI(Entity)

	public:
		Entity();
		~Entity();

		Entity (Entity &ent);

		virtual void setCanSel(bool b = true);

		// begin specific for border
		// 如果m_pBorderMaterial为0，不使用包边特效，否则使用
		void setBoreder (Material *pMaterial);
		Material *getBoreder ();
		// end specific for border

		void setBorderColor (Vector3 color)
		{
			m_BorderColor = color;
		}
		Vector3 getBorderColor ()
		{
			return m_BorderColor;
		}

		void enableDeadEffect (bool enable);
		bool isDeadEffectEnabled ();

		void setDeadScale (float deadScale);

		virtual void enableUVMask (bool enable, bool stdmtluse);
		bool isUVMaskEnabled ();
		void setUVMaskSpeed (Vector2 vec);
		Vector2 getUVMaskSpeed ();
		void setUVMaskColor (Vector3 color);
		Vector3 getUVMaskColorType ()
		{
			return m_UVMaskColor;
		}

		virtual void setLiuGuangTexture (TextureData *texture);
		virtual void setLiuGuangTexture (const char *filename);


		void load(Resource *pres);
		void load(EntityData *pdata);
		bool load(const FixedString &entity_res, bool blockload);
		bool isLoading();
		virtual void ResourceLoaded(Resource *rec, ResourceHandle h);

		// 设置模型播放速度是原始速度的比率(1.0f为原始速度)
		void setModePlayRadio (float fRadio);
		float getModelPlayRadio ();

		virtual void resetUpdate(bool pause, uint tick=0xffffffff);
		virtual void update(uint dtick);
		void updateEffect (uint dtick); // 角色死亡时做的处理，不更新模型，只更新特效，避免模型轻微抖动的瑕疵
		virtual void attachToScene(GameScene *pscene,bool hasfather = false);
		virtual void detachFromScene();
		virtual Matrix4 getAnchorWorldMatrix(int id);
		virtual void render(SceneRenderer* pRenderer, const ShaderEnvData &envdata);
		virtual bool intersectRay(IntersectType type, const Ray &ray, float *collide_dist);
		virtual void addRenderUsageBits(RenderUsage usage);
		virtual void clearRenderUsageBits(RenderUsage usage);
		virtual void getRenderPassRequired(RenderPassDesc &desc);

		Model *findAnchorOwnerModel(int id, MovableObject *pexcludechild=NULL); //找出拥有anchor_id的模型或子模型， 但不能是pexcludechild的子模型
		size_t getNumMotion();
		ModelMotion *getIthMotion(size_t i);
		void addMotion(ModelMotion *pmotion);
		void playMotion(int idx, bool reset_play=true, int motion_class=0);
		void playMotion(const FixedString &name, bool reset_play=true, int motion_class=0);
		void playFlashChain(const FixedString &name, Ogre::Vector3 dstPos, 
			int motion_class=0, int srcAnchorid=-1);
		void playParticleEmitter (const FixedString &name, Ogre::Vector3 dstPos, float fRange);

		void stopMotion();
		void stopMotion(const FixedString &name);

		void delayStopMotion (const FixedString &name, float second = 5.0f);

		void stopMotion(int motion_class);

		void delayStopMotion (int motion_class, float second = 5.0f);

		ModelMotion *findMotion(const FixedString &name);
		void clearMotions();
		bool IsPlaying();

		void playMotion(int seqid, const char **pmotions, int num_motion, int motion_class=0);
		bool playBindAnim(int objclass, int seqid);

		Model *getMainModel()
		{
			return m_pMainModel;
		}

		size_t getAnchors(std::vector<int>&anchors, bool recursive=true);

		void bindObject(uint anchorid, MovableObject *pobj, int objclass=0, int sort_index=0);
		void unbindObject(MovableObject *pobj);
		void unbindAll(int objclass);
		void unbindRange(int min_class, int max_class);

		Matrix4 getAnchorWorldTM(int anchorid);
		Vector3 getAnchorWorldPos(uint anchorid);

		bool playAnim(int seq);
		bool hasAnimPlaying(int seq);
		void stopAnim(int seq);
		void stopAnim();

		void setInstanceAmbient(const ColourValue &color);

		void getActionList(std::vector<ACTION_INFO>& outList);
		void getActionInfo(const FixedString &name, ACTION_INFO& out);

		std::vector<MotionEventHandler*>*	getEventHandler(FixedString strEventName);
		void registerEvent(FixedString strEvent, MotionEventHandler* pHandler);
		void unregisterEvent(FixedString strEvent, MotionEventHandler* pHandler);

		void setPostSceneRenderer(SceneRenderer *prenderer);//设置受motion后期效果影响的scenerenderer
		SceneRenderer *getPostSceneRenderer();

		void addOverlayMaterial(Material* pmtl,float alpha);
		void clearAllOverlayMaterial ();
		void clearTopMaterial ();

		void setPublicMotionDir(const char *dir);
		
		struct BindObj
		{
			uint anchor_id;
			int objclass;
			int sort_index; //渲染的时候进行排序
			MovableObject *pobject;
		};

		void addNewBindObj( BindObj* poneBindObj );
		void eraseBindObj( BindObj* poneBindObj );

		// 加载完成后，播放当前动作
		void playCurAnim();

		int getCurAnimID();

		void setTextureByID(int id, const char* szPath);

		bool isMotionPlaying(const char* szEffect, int motionclass);

	public:
		std::map<FixedString, std::vector<MotionEventHandler*>>m_EventHandlers;

		std::vector<Ogre::Vector3> m_FlashChainList; // 闪电链位置列表，当前Entity列表中只支持一个闪电链

		struct BindRenderableObj
		{
			uint anchor_id;
			int objclass;
			RenderableObject *pobject;
			BIND_ANIM_MODE animmode;
		};

		//std::vector<BindRenderableObj>m_BindRenderObjs;
		//std::vector<BindObj>m_BindUnrenderObjs;
		std::vector<BindObj*> m_BindObjs;
		std::vector<BindObj*> m_NoBindObjs;
		std::vector<BindObj*> m_renderBindObjs;

		std::vector<MovableObject *>m_DeleteObjs;

		std::vector<DelayDeleteObject *> m_DelayDeleteObjs;

		static bool ms_bShowBindObject;

	private:
		EntityData *m_pEntityData;
		ResourceHandle m_hLoad;
		unsigned int m_startLoadTime;
		Model *m_pMainModel;

		bool m_bDeadEffectEnabled;
		float m_fDeadScale;

		bool m_bUVMaskEnabled;
		Vector2 m_MaskUVSpeed;
		Vector3 m_UVMaskColor;

		std::vector<ModelMotion *>m_Motions;
		std::vector<ANIM_PLAY>m_CurrAnim;

		ColourValue m_InstanceAmbient;

		bool	m_bEnablePostEffect;
		bool	m_bGameFocus;

		int m_CurAnimID;

		FixedString m_PublicMotionDir; //"effect\\public"

		// 用户数据，由使用engine的模块设定，并解释（如ShowEngine模块或者逻辑层模块），
		// engine层只简单保存这个值，并在适当的时候（如Pick）返回这个值给上层，0值代表未设定的空值状态
		void *				m_UserData;

		SceneRenderer *m_pPostSceneRenderer;
		std::vector<std::pair<Material*,float>> m_OverLayMtls;
		Material*	m_pBorderMtl;
		Vector3 m_BorderColor;

		float m_fModelRadio;

		Vector3 m_Color;
		bool m_bLastPalyIsJiFeng;

		void releaseChildObject(MovableObject *pobject);
		void playMotionSelf(const FixedString &name);
		virtual void updateWorldCache();
		virtual void invalidWorldCache();
		void clearDeleteObjs();
		void clearDelayDeleteObject (float deltaTime);
		void calRenderUsageBits();
		void updateBindFather();
		void updateNoBindFather();

		Resource *m_pRes;
		std::string m_CurMotionName;
		std::vector<std::pair<int,FixedString>> m_vIDTextures;
	};

}

#endif
