


proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ����Щ���ĺ���Ҳ��ᵽ���Ǽ�������˵�ʹ�࣬˭Ҳ�޷���ֹ�ң�
		task_add_option 1378 1 100
		task_add_option 1370 1 101
		task_add_option 1371 1 102
		task_add_option 1375 1 103
	
	talk
	
endproc 

PROC 100
	task_do_option 1378 1
ENDPROC
PROC 101
	task_do_option 1370 1
ENDPROC
PROC 102
	task_do_option 1371 1
ENDPROC
PROC 103
	task_do_option 1375 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1375
		task_fini 1375 1
	endif

endproc 
	
	