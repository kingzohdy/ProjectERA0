; Ƥ��ϣ

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ÿ�춼�����Լ�����Ϊһ�����Ҹ���Щʲô�����ǲ���Ҫվ������������������֤���ҵļ�ֵ�������ǲ�����ζ���ҵ�һ���ͽ��ķ������������̵����磿
	
		task_add_option 2111 1 100
		task_add_option 2112 1 101
		task_add_option 2113 1 102
		task_add_option 2118 1 103
		task_add_option 2119 1 104

	
	talk
endproc

PROC 100
	task_do_option 2111 1
ENDPROC
PROC 101
	task_do_option 2112 1
ENDPROC
PROC 102
	task_do_option 2113 1
ENDPROC
PROC 103
	task_do_option 2118 1
ENDPROC
PROC 104
	task_do_option 2119 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 2113
		task_fini 2113 1
	endif

endproc





