


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������졭���쳣�ġ����䰡����
	add_talk #b#b<�Ǵ�������������һ������Ŭ�����Լ��ļ���ֱͦ�����⺮��̹ǵĽ��⣬���������·���ʱ�ữ��һƬѩ����Ʈ��Զ��>
		task_add_option 1356 1 100
		task_add_option 1357 1 101
		task_add_option 1358 1 102
;		task_add_option 1359 1 103
;		task_add_option 1360 1 104
		task_add_option 1376 1 105

	
	talk
	
endproc 

PROC 100
	task_do_option 1356 1
ENDPROC
PROC 101
	task_do_option 1357 1
ENDPROC
PROC 102
	task_do_option 1358 1
ENDPROC
;PROC 103
;	task_do_option 1359 1
;ENDPROC
;PROC 104
;	task_do_option 1360 1
;ENDPROC
PROC 105
	task_do_option 1376 1
ENDPROC
proc 2

	get_trig_task 1 2
;	if 2 = 1360
;		task_fini 1360 1
;	endif

endproc 
	
	