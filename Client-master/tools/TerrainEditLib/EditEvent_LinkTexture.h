
#pragma once


#include "EditEventHandler.h"

namespace TE
{

	class LinkTexEditHandler : public BaseEditHandler
	{
	public:
		LinkTexEditHandler(void);
		~LinkTexEditHandler(void);
		//�������; ����( �༭��ָ��, ��Ӧ�¼� ) ; ���� ( Ϊ���ʾ�ɹ� )
		virtual bool handleEditEvent( TerrainEditMgr* pEditor , ActionEvent *event );
		//���˲���
		virtual void  Undo( TerrainEditMgr* pEditor );
	protected:
		//�����Զ�ƴ�ӵı�� ; ����( �༭��ָ��, ���߷���, ����λ�� )
		void  SetAutoLinkMeshSign(  TerrainEditMgr* pEditor ,Ogre::Vector3 &dir ,Ogre::Vector3 &pos);
	};


};