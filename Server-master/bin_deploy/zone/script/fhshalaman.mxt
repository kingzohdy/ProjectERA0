



proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bˮ�ڱ����Ĺ����У������������ɳ������ԶҲ�����������ˡ�

		task_add_option 1901 1 100
		task_add_option 1902 1 101
		task_add_option 1939 1 102
	
	
	talk
endproc

PROC 100
	task_do_option 1901 1
ENDPROC
PROC 101
	task_do_option 1902 1
ENDPROC
PROC 102
	task_do_option 1939 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 1902
		task_fini 1902 1
	endif

endproc
