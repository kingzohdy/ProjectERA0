
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������磬�Ӳ����һ�����ۡ���Թ�԰��߰䷢���ơ�ֻ���������ж����ˣ���ԶҲ��᲻����������ʱ��ϲ�á�
	add_talk #b#b
	add_talk #c186506ע�⣺��������������������һ�Ρ�
	add_talk #c186506ע�⣺��ȡ����󣬽�����ʹ�á��Һ�æ������������ɡ�#n

		task_add_option 1865 1 100
		task_add_option 1866 1 101
		TASKCYCLE_ADD_OPTION 109 1 140
		
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 65 and lv < 75
		option 240 ��Ҫ������ɡ����顿����ʥ��
	endif
	
	talk
	
endproc 
PROC 100
	task_do_option 1865 1
ENDPROC
PROC 101
	task_do_option 1866 1
ENDPROC

proc 140
	TASKCYCLE_DO_OPTION 109 1
endproc
proc 200
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc 
proc 240
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 65 and lv < 75
		PLAYER_BUSY_UI 19 1
	endif
endproc






