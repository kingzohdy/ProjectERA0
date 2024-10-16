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
	class MapPlayerBrower : public BaseEditHandler
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

		Ogre::Model *m_pCamAnimMod;

	private:
		void UpdateCamera(TerrainEditMgr* pEditor , unsigned int tick);            ///< ���������
		bool updateCameraAnim(unsigned int dtick);

	public:
		MapPlayerBrower(void);
		~MapPlayerBrower(void);
		virtual void MsgProc(TerrainEditMgr* pEditor, void *hWnd, unsigned int msg, unsigned int wParam, unsigned int lParam);
		bool  handleEditEvent( TerrainEditMgr*  pEditor , ActionEvent* event ) { return true;}
		void  Update(TerrainEditMgr* pEditor  , float tick);  ///< ע�⣺�˴���tick��endTime - startTime�������ﲻ����
		void  Init(TerrainEditMgr* pEditor);
		void  Release();
		const Ogre::Vector3 &getCurPos(){return m_vCurPos;}

		void beginCameraAnim(const char *modelpath);
		void endCameraAnim();

		int getPlayerDir();
		int getCameraDir();
	};
}
