

proc 1
	add_talk $_npc$:
	add_talk #b#b�ø����Ӿ����ȥս��ô��û�м��ܣ��������ž����á�������Ҳ������������
	task_add_option 1094 1 101
	task_add_option 1095 1 102
	talk
endproc 


PROC 101
	task_do_option 1094 1
ENDPROC
PROC 102
	task_do_option 1095 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1095
		task_fini 1095 1
	endif  
endproc
