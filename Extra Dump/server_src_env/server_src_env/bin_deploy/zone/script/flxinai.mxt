
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bһ����һ���������ǵĵ��˶���������һ����һ�������Ƕ����������
		task_add_option 2053 1 100
		task_add_option 2054 1 101

	talk
endproc 

proc 100
	task_do_option 2053 1
endproc 
proc 101
	task_do_option 2054 1
endproc 

