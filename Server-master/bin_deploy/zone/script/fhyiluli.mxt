



proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ڱ�����Ϣ�ĺӴ��һ�дҴ�������
	add_talk #b#b����ʲôֵ�������ӵģ�
	add_talk #b#b����һ���˰��Ϸ��ӵ���ȸ����ȸȴ�Ѳ�����Ӱ��
		task_add_option 1905 1 100
		task_add_option 1906 1 101
		task_add_option 1907 1 102
		task_add_option 1908 1 103
		task_add_option 1909 1 104
		task_add_option 1910 1 105
		task_add_option 1919 1 106
		task_add_option 1920 1 107
	
	
	talk
endproc

PROC 100
	task_do_option 1905 1
ENDPROC
PROC 101
	task_do_option 1906 1
ENDPROC
PROC 102
	task_do_option 1907 1
ENDPROC
PROC 103
	task_do_option 1908 1
ENDPROC
PROC 104
	task_do_option 1909 1
ENDPROC
PROC 105
	task_do_option 1910 1
ENDPROC

PROC 106
	task_do_option 1919 1
ENDPROC
PROC 107
	task_do_option 1920 1
ENDPROC



proc 2
	get_trig_task 1 2
	if 2 = 1910
		task_fini 1910 1
	endif

endproc
