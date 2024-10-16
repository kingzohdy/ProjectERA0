/**
    Place a player in scene in order to preview.
    @author ashluo
    @date oct. 20, 2008
    @note
        对于背景音乐的处理应该需判断当前正在播放的是否与区域异动后的背景音乐一样，如果不一致才load，否则不load。
        这样能避免断层问题。
*/

#pragma once

#include "EditEventHandler.h"
#include "GameCamera.h"

namespace TE
{
    class MPlacePlayer : public IEventProcess
    {
    private:
		Ogre::Entity *m_pPlayer;           ///< 指向player节点的指针
		Ogre::Vector3 m_StartPos;
        Ogre::Vector3 m_vCurPos;         ///< Player当前位置
		float m_RunLength;
        Ogre::Vector3 m_vTargetPos;      ///< 目的点
        bool m_bGo;                     ///< 是否走动
		int m_CurRegionIndex;

		FollowCamera *m_pCamera;
        bool m_bInterpolateMode;        ///< 插值模式：true为时间插值，false为距离插值
    private:
        void UpdateCamera(MTerrainEdit* pEditor , unsigned int tick);            ///< 更新摄像机
    public:
        MPlacePlayer(void);
        ~MPlacePlayer(void);
        virtual void MsgProc(MTerrainEdit* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
        bool  Process( MTerrainEdit*  pEditor , ActionEvent* event ) { return true;}
        void  Update(MTerrainEdit* pEditor  , float tick);  ///< 注意：此处的tick是endTime - startTime，在这里不适用
	    void  Init(MTerrainEdit* pEditor);
        void  Release();
		const Ogre::Vector3 &getCurPos(){return m_vCurPos;}
    };
}
