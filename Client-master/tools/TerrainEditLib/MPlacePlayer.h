/**
    Place a player in scene in order to preview.
    @author ashluo
    @date oct. 20, 2008
    @note
        ���ڱ������ֵĴ���Ӧ�����жϵ�ǰ���ڲ��ŵ��Ƿ��������춯��ı�������һ���������һ�²�load������load��
        �����ܱ���ϲ����⡣
*/

#pragma once

#include "EditEventHandler.h"
#include "GameCamera.h"

namespace TE
{
    class MPlacePlayer : public IEventProcess
    {
    private:
		Ogre::Entity *m_pPlayer;           ///< ָ��player�ڵ��ָ��
		Ogre::Vector3 m_StartPos;
        Ogre::Vector3 m_vCurPos;         ///< Player��ǰλ��
		float m_RunLength;
        Ogre::Vector3 m_vTargetPos;      ///< Ŀ�ĵ�
        bool m_bGo;                     ///< �Ƿ��߶�
		int m_CurRegionIndex;

		FollowCamera *m_pCamera;
        bool m_bInterpolateMode;        ///< ��ֵģʽ��trueΪʱ���ֵ��falseΪ�����ֵ
    private:
        void UpdateCamera(MTerrainEdit* pEditor , unsigned int tick);            ///< ���������
    public:
        MPlacePlayer(void);
        ~MPlacePlayer(void);
        virtual void MsgProc(MTerrainEdit* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
        bool  Process( MTerrainEdit*  pEditor , ActionEvent* event ) { return true;}
        void  Update(MTerrainEdit* pEditor  , float tick);  ///< ע�⣺�˴���tick��endTime - startTime�������ﲻ����
	    void  Init(MTerrainEdit* pEditor);
        void  Release();
		const Ogre::Vector3 &getCurPos(){return m_vCurPos;}
    };
}
