;������
; Я������

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����Ƕ�ȥ�����ˣ���Ҳ����ȥ��Ҫ�������ȥʱ��ǿ����ɡ�
	task_add_option 1021 1 101	
	task_add_option 1022 1 102
	
	task_add_option 1020 1 103
	task_add_option 1023 1 104

	talk
endproc
proc 101
    task_do_option 1021 1
endproc
proc 102
    task_do_option 1022 1
endproc
proc 103
    task_do_option 1020 1
endproc
proc 104
    task_do_option 1023 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1023
		task_fini 1023 1
	endif
endproc



