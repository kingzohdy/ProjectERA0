; ����

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b����������ǿӲ�ظ��ұ���ҽѧ־Ը��Ӳ�����ذ��Ҵ���ѧԺ��У�����˳�������ʱ���ޱȷ�ŭ���ޱ�Թ�ޣ���������������׵�ѡ������ȷ�ġ���ֻ�ǣ�̫���ˡ���
	
		task_add_option 2113 1 100
		task_add_option 2114 1 101
		task_add_option 2115 1 102
		task_add_option 2129 1 103
		task_add_option 2136 1 104
		task_add_option 2137 1 105

	
	talk
endproc

PROC 100
	task_do_option 2113 1
ENDPROC
PROC 101
	task_do_option 2114 1
ENDPROC
PROC 102
	task_do_option 2115 1
ENDPROC
PROC 103
	task_do_option 2129 1
ENDPROC
PROC 104
	task_do_option 2136 1
ENDPROC
PROC 105
	task_do_option 2137 1
ENDPROC

proc 2
	get_trig_task 1 2
	if 2 = 2115	
	TASK_FINI 2115 1
	endif

endproc



