

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b������Զ�޷�������ϣ�����ţ��������ϵĻ��ţ���ͱ���Ҫ�����������Կ���ЩΣ�����硢���������ϵ������
	add_talk #b#b��Ȼ����Ҳ���Եȴ����ȱ����������㣬���ÿ���˶�����һ�����뷨����ô�������ֻ����ֹ���ĺڰ������Ϊʲô�㲻Ը��������������������أ�
	add_talk #b#b�������ÿ���˶�������������СС��Ѫɫ׷�����������ʲô��
	
	
	task_add_option 1540 1 101
	task_add_option 1541 1 102
	task_add_option 1542 1 103
	task_add_option 1543 1 104
	task_add_option 1544 1 105
	task_add_option 1549 1 106
	task_add_option 1550 1 107
	task_add_option 1551 1 108
	task_add_option 1552 1 109



	talk
endproc 


PROC 101
	task_do_option 1540 1
ENDPROC
PROC 102
	task_do_option 1541 1
ENDPROC
PROC 103
	task_do_option 1542 1
ENDPROC
PROC 104
	task_do_option 1543 1
ENDPROC
PROC 105
	task_do_option 1544 1
ENDPROC
PROC 106
	task_do_option 1549 1
ENDPROC
PROC 107
	task_do_option 1550 1
ENDPROC
PROC 108
	task_do_option 1551 1
ENDPROC
PROC 109
	task_do_option 1552 1
ENDPROC


proc 2
	get_trig_task 1 2
 
	if 2 = 1543
		task_fini 1543 1
	endif 
	if 2 = 1550
		task_fini 1550 1
	endif 
endproc
