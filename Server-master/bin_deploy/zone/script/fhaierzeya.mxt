;��������

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����Ѿ������˵����飬����û�취������ʧ��ֻ����ʰ�о֡�����δ�����������ģ��һ�ƴ������ȥ��ֹ������ֹ�ڰ�������ı����ֹ�˼�Ҿ��ٴη�����
	
		task_add_option 1921 1 100
		task_add_option 1922 1 101
		task_add_option 1923 1 102
		task_add_option 1924 1 103
		task_add_option 1925 1 104
		task_add_option 1926 1 105
		task_add_option 1927 1 106
		
		task_add_option 1950 1 107
		
		
	talk
endproc

PROC 100
	task_do_option 1921 1
ENDPROC
PROC 101
	task_do_option 1922 1
ENDPROC
PROC 102
	task_do_option 1923 1
ENDPROC
PROC 103
	task_do_option 1924 1
ENDPROC
PROC 104
	task_do_option 1925 1
ENDPROC
PROC 105
	task_do_option 1926 1
ENDPROC
PROC 106
	task_do_option 1927 1
ENDPROC
PROC 107
	task_do_option 1950 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 1927
		task_fini 1927 1
	endif
	if 2 = 1950
		task_fini 1950 1
	endif
endproc
