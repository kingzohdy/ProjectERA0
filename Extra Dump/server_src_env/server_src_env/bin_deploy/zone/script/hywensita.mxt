;��ţ����,��˹��
;��˹�� 
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������ţ�е���������Ҫ׬ȡ�����Ҫ��Ϊ�������۵Ķ�ţ��ʿ��
	task_add_option 2000 1 100
	option 102 ��ţ�ؾ�
;	TASK_STAT 2000 1 10
;	if 10 = 2
;		OPTION 101 ��ȡ��ţ
;	endif
	talk
endproc 
proc 2
	get_trig_task 1 2 
	if 2 = 2000 
		call 101 
	endif
endproc
proc 100
	task_do_option 2000 1
endproc
proc 101
;	NEW_CONVOY_MON 213018 2639 5225 500 0 0 0 360 2000 1 2
	add_sys_msg_start
	add_sys_msg �ø�ţ�޳����ţ��ʹ���ƶ�
	set_sys_msg_mode 4 0 0
	msg_info	
endproc
proc 102
	DIALOG 1
;	add_talk $_npc$:
	add_talk ��ţ��ʵ�ܼ򵥣��������£�
	add_talk #b#b#c186506�����ţ�ޣ��������ţ��ƨ�ɣ����ͻᳯ������Եķ����ܡ�#n
	add_talk #b
	add_talk ��ţ��ʵ�����ѣ�ע��Ҫ�㣺
	add_talk #b#b#c186506 1��Ҫ���ϵ�������#n
	add_talk #b#b#c186506 2��Ҫ�ڹ涨��ʱ���ڽ�ţ����ũ��#n
	option 1 ����
	talk
endproc
