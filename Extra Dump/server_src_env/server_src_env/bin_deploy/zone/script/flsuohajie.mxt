
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҵļ��������ഫ���ǿ���ר�ң������㿴��ȥ����������ģ���δ���Ŀ��Ŵ�ʦ��������ô����
		
		task_add_option 2040 1 100
		task_add_option 2041 1 101
		task_add_option 2042 1 102

	talk
endproc 

proc 100
	task_do_option 2040 1
endproc 
proc 101
	task_do_option 2041 1
endproc 
proc 102
	task_do_option 2042 1
endproc 

