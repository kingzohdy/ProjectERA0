; ����


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ���������ӹ����ⳡ���ѣ�˵������������������֮��Ϊ����������ʲô����һ��Ҳ��Ը�������������������⸱ģ����
	
		task_add_option 2121 1 100
		task_add_option 2122 1 101
		task_add_option 2130 1 102
		task_add_option 2131 1 103
		
	
	talk
endproc

PROC 100
	task_do_option 2121 1
ENDPROC
PROC 101
	task_do_option 2122 1
ENDPROC
PROC 102
	task_do_option 2130 1
ENDPROC
PROC 103
	task_do_option 2131 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2122
		task_fini 2122 1
	endif
endproc






