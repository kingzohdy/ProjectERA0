
#pragma once


#include "EditEventHandler.h"

namespace TE
{

class MAutoLinkMeshProcess
		:public IEventProcess
{
public:
	MAutoLinkMeshProcess(void);
	~MAutoLinkMeshProcess(void);
    //�������; ����( �༭��ָ��, ��Ӧ�¼� ) ; ���� ( Ϊ���ʾ�ɹ� )
	virtual bool Process( MTerrainEdit* pEditor , ActionEvent *event );
    //���˲���
	virtual void Undo( MTerrainEdit* pEditor );
protected:
	//�����Զ�ƴ�ӵı�� ; ����( �༭��ָ��, ���߷���, ����λ�� )
	void  SetAutoLinkMeshSign(  MTerrainEdit* pEditor ,MVec3 &dir ,MVec3 &pos);
};


};