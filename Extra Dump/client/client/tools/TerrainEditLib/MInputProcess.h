#pragma once


#include "mterrainedit.h"
#include "minput.h"

namespace TE
{

//用户输入信息
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

	//获得用户输入状态
	MUserInput*                    GetUserInput(){ return &m_UserInput; };
private:
	//用户输入结构
	MUserInput                      m_UserInput;
	//键盘按下处理
	MInput                          m_Inputer;
private:
	void UpdateInput( TE::MTerrainEdit* pEditor , MUserInput* pInput );
};

};

extern TE::MInputProcess  g_MInputProcess;