; �˿�

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������Ƭ�ط���ֻҪ�����õ����ͻ��Ϊ��������������ʤ�أ�����һ�죬���ǻ���ˮһ��ӿ����Ϊ��Ƭ���ش�����������
	
		task_add_option 2108 1 100
		task_add_option 2109 1 101
		task_add_option 2110 1 102
		task_add_option 2135 1 103

	
	talk
endproc

PROC 100
	task_do_option 2108 1
ENDPROC
PROC 101
	task_do_option 2109 1
ENDPROC
PROC 102
	task_do_option 2110 1
ENDPROC
PROC 103
	task_do_option 2135 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 2110
		task_fini 2110 1
	endif

endproc





