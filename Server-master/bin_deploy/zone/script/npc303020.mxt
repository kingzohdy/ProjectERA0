proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��ã��кι�ɡ�
	add_talk #b#b��������ṩ�Ѹ�������ת�Ƹ���һ��װ���ġ�#c186506װ������ת��#n������
	add_talk #b#b�������װ����������ֵ�ġ�#c186506������������#n������
	; add_talk #b#b#c186506ת��װ�����ԿɰѸ�������ת����һ��װ����#n
	; add_talk #b#b#c186506�����������Կ����װ���ĸ�������ֵ��#n
	add_talk #b#bӵ����ʵ�Ļ���������վ�ڸ��ߵĶ��ˡ�
	; add_talk #b#b��#c186506װ������ת��#n����#c186506������������#n���������װ����ʤһ�����������һ�޶��ļ�Ʒװ����
	; add_talk #b#bӵ����ʵ�Ļ���������վ�ڸ��ߵĶ��ˡ�
	
	task_stat 2259 1 10
	task_stat 3174 1 30
	if 10 = 2 or 10 = 0
		add_talk #b#b
		add_talk #c186506װ������ת��-��ɡ������ʦ����������[56��]#n
	endif
	if 30 = 2 or 30 = 0
		add_talk #b#b
		add_talk #c186506������������-��ɡ�˹�Ϸ���Ŀ��顱����[65��]#n
	endif
	TASK_ADD_OPTION 2259 0 150
	TASK_ADD_OPTION 3174 0 160
	
	if 10 = 3
		option 101 #��װ������ת�ơ� 
	endif
	if 30 = 3
		option 102 #�������������ԡ�
	endif
	
	task_stat 4494 1 20
	if 20 = 2
		option 200 #��װ������ʦ˹�Ϸ�������һ������
	endif
	talk
	
	
endproc 

proc 101
	DIALOG 1
	add_talk ��ת�����á�
	add_talk #b#b�ɽ�װ���ġ�#cff4e00��������#n��ת�Ƹ�Ŀ��װ����
	; add_talk #b#b
	add_talk ��ת��˵����
	add_talk #b#bת�ƺ��ṩ����ת�Ƶ�װ����������ʧ��
	add_talk #b#b#c186506ֻת����ɫ�ĸ������ԣ���ת���������ԡ�#n
	; add_talk #b#b
	; add_talk ��ת�Ʊ��ϡ�
	; add_talk #b#b����Ŀ��װ���ġ�#cff4e00ǿ���Ǽ�#n��
	; add_talk #b#b����Ŀ��װ���ġ�#cff4e00��������#n��
	; add_talk #b#b����Ŀ��װ���ġ�#cff4e00ϴ������#n��
	; add_talk #b#b����Ŀ��װ���ġ�#cff4e00��ħ����#n��
	; add_talk #b#b����Ŀ��װ���ġ�#cff4e00��Ƕ��ʯ#n��
	
		option 100 #�ͽ�ת�ƣ�ͬƷ��װ����ת�ƣ�
		option 250 #�߽�ת�ƣ���Ʒ��װ����ת�ƣ�
		option 300 #ת��ʯ����������÷�ʽ
	
	talk
	
endproc

proc 102
	DIALOG 1
	add_talk ���������á�
	add_talk #b#b������װ����#cff4e00��������#n����#cff4e00ϴ������#n��������ֵ��
	add_talk ������˵����
	add_talk #b#b�ɵ���ѡ��ÿ�����Խ���������
	add_talk #b#b#c186506ֻ������ѡ�������Ե�ֵ���������������͡�#n
	
		option 110 #�����������ԣ�ֻ��������ֵ��
	
	talk
	
endproc

proc 100
	open_ui 79
endproc

proc 110
	open_ui 80
endproc

proc 150
	TASK_DO_OPTION 2259 0
endproc

proc 160
	TASK_DO_OPTION 3174 0
endproc

proc 250
	open_ui 85
endproc 
proc 300
	DIALOG 1
	add_talk ��ת��ʯ��
	add_talk #b#b��ʱ������˺������м��ʻ��#n
	add_talk #b#b��ʧ��֮�����㡱���������#n
	add_talk #b#b
	add_talk ���̾���������
	add_talk #b#b65����������-������������#n
	add_talk ��������������
	add_talk #b#b��ʧ��֮�����㡱���������#n

	
		option 1 ����

		
	talk
endproc 
 
proc 200
	dialog 3
	add_talk $_npc$:
	add_talk #b#b��������ĸ����з���#c186506��˼��#n��#c186506������#n
	add_talk #b#b�����û�����ӵĻ�����ȥ�����Ƕ������ɣ�
	
	NPC_COMMIT_OPT 201
	
	talk
endproc 
proc 201
	strvar m_ID m_Pos1 m_Pos2 m_Col Ret RetTask
	GET_COMMIT_ITEM_INFO 1 m_ID
	if m_ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif
	
	GET_COMMIT_ITEM_INFO 2 m_Pos1
	GET_COMMIT_ITEM_INFO 3 m_Pos2
	GET_COMMIT_ITEM_INFO 4 m_Col
	task_stat 4494 1 100
	if 100 = 2
		if m_ID = 5012123 or m_ID = 5012124
			switch m_ID
				case 5012123
					DEC_ITEM_NUM_BYIDX 5012123 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4494 1
					call 502
				endcase
				case 5012124
					DEC_ITEM_NUM_BYIDX 5012124 1 @m_Pos1 @m_Pos2 RetTask
					task_fini 4494 1
					call 502
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 1 0 0
			msg_info
		endif
	
	endif
endproc 
proc 502
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc 
