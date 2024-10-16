#pragma once


#include "mterrainedit.h"
#include "minput.h"

namespace TE
{

//�û�������Ϣ
struct MUserInput
{
    bool bRotateUp;
    bool bRotateDown;
    bool bRotateLeft;
    bool bRotateRight;
	bool bMoveNear;
	bool bMoveLong;
	bool bAddAnim;
	bool bDecAnim;
	bool bAddAnimSpeed;
	bool bDecAnimSpeed;
	bool bRoateXAdd;
	bool bRoateXDec;
	bool bRoateYAdd;
	bool bRoateYDec;
	bool bRoateZAdd;
	bool bRoateZDec;
	bool bFirstCamera;
	bool bFreeCamera;
	bool bLockCamera;

	bool bSetRoate;
	bool bSetScale;

	bool bZKeyDown;
	bool bCKeyDown;
	bool bVKeyDown;
	bool bSkeyDown;

	bool bLAlt;
	bool bLCtrl;

	bool bLShift;
};

class MInputProcess
{
public:
	MInputProcess(void);
	~MInputProcess(void);

	void Process( MTerrainEdit* pEditor );

	//����û�����״̬
	MUserInput*                    GetUserInput(){ return &m_UserInput; };
private:
	//�û�����ṹ
	MUserInput                      m_UserInput;
	//���̰��´���
	MInput                          m_Inputer;
private:
	void UpdateInput( TE::MTerrainEdit* pEditor , MUserInput* pInput );
};

};

extern TE::MInputProcess  g_MInputProcess;