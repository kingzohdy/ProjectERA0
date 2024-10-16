#pragma once

namespace TE
{

	class KeyInputHelper
	{
	public:
		KeyInputHelper(void);
		~KeyInputHelper(void);

		int   KeyDown(int key);

		int   LControlKeyDown();
		int   RControlKeyDown();

		int   LMenuKeyDown();
		int   RMenuKeyDown();

		int   LShiftKeyDown();
		int   RShiftKeyDown();
	};


	struct InputParam
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

	class TerrainEditMgr;
	class EditorInputMgr
	{
	public:
		EditorInputMgr(void);
		~EditorInputMgr(void);

		void handleEditEvent( TerrainEditMgr* pEditor );

		//获得用户输入状态
		InputParam*                    GetUserInput(){ return &m_UserInput; }
		bool isKeyDown(int vkey)
		{
			return m_Inputer.KeyDown(vkey)!=0;
		}

	private:
		//用户输入结构
		InputParam                      m_UserInput;
		//键盘按下处理
		KeyInputHelper                          m_Inputer;
	private:
		void UpdateInput( TE::TerrainEditMgr* pEditor , InputParam* pInput );
	};

	extern EditorInputMgr  g_MInputProcess;
}

