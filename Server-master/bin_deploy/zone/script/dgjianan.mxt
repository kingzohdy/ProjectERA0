

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�������ϵĶ����������ǿ��ü��ģ����ǿ������ģ����������˵Ļ��ǻ��˵ģ�ֻҪ���˸��򣬲��Ҽ۸���ʣ��Ҿ͸�����
	add_talk #b#b
	add_talk #c186506ע�⣺��������������������һ�Ρ�
	add_talk #c186506ע�⣺��ȡ����󣬽�����ʹ�á��Һ�æ������������ɡ�#n
	
	task_add_option 1534 1 101
	task_add_option 1533 1 102
	task_add_option 1560 1 103
;	task_add_option 1573 1 104
;	task_add_option 1574 1 105
;
;	task_add_option 1575 1 106
;	task_add_option 1576 1 107 

	TASKCYCLE_ADD_OPTION 103 1 140
	
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	
	if lv >= 35 and lv < 45
		option 210 ��Ҫ������ɡ����顿��Ŭ��˹���
	endif

	talk
endproc 
proc 140
	TASKCYCLE_DO_OPTION 103 1
endproc

PROC 101
	task_do_option 1534 1
ENDPROC
PROC 102
	task_do_option 1533 1
ENDPROC
PROC 103
	task_do_option 1560 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1560
		task_fini 1560 0
	endif 

endproc
proc 210
	strvar lv
	GET_PLAYER_BASE_ATT 0 0 lv 1
	if lv >= 35 and lv < 45
		PLAYER_BUSY_UI 16 1
	endif
endproc



