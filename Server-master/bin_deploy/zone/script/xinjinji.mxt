;�������̹� 
; Я������\���븱��500

proc 1
	TASK_STAT 1054 1 3
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ�����޵�������Ͷ�뵽���޵�Ϊ�������˷�����ȥ��ѵ��������ǿ��ľ�Ӣʿ������������İ�ȫ��
	task_add_option 1052 1 101	
	task_add_option 1054 1 102
	task_add_option 1055 1 103
	task_add_option 1057 1 104

	
	if 3 = 2
		OPTION 110 #����������
	endif
	
	talk
endproc
proc 101
    task_do_option 1052 1
endproc
proc 102
    task_do_option 1054 1
endproc
proc 103
    task_do_option 1055 1
endproc
proc 104
    task_do_option 1057 1
endproc
proc 110
    enter_pworld 500 0 1
endproc


proc 2
	get_trig_task 1 2

	if 2 = 1054 
		dialog 1
		add_talk $_npc$:
		add_talk #b#b��Ҫ�����޵�������Ͷ�뵽���޵�Ϊ�������˷�����ȥ��ѵ��������ǿ��ľ�Ӣʿ������������İ�ȫ��
	
		OPTION 110 #����������
		talk
	endif

endproc

