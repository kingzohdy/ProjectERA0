; ������

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ؼ�����ྰ����Ҫ�����۲ſ��ü��ģ�Ʃ���Ρ�
	
		task_add_option 1934 1 100
		task_add_option 1935 1 101

	talk
endproc

PROC 100
	task_do_option 1934 1
ENDPROC
PROC 101
	task_do_option 1935 1
ENDPROC



;proc 2
;	get_trig_task 1 2
;	if 2 = 1910
;		task_fini 1910 1
;	endif
;
;endproc
