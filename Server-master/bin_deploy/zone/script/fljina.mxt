
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�Ҳ�������ط�����Դ����;����Щͨ������������ʩ��������Ĺ����룬���������Ѿ�û�����̣���ȴ��Ȼ�ܸо����������ĺڰ�����
		
		task_add_option 2047 1 100
		task_add_option 2048 1 101
		task_add_option 2049 1 102
		task_add_option 2050 1 103
		task_add_option 2055 1 104

	talk
endproc 

proc 100
	task_do_option 2047 1
endproc 
proc 101
	task_do_option 2048 1
endproc 
proc 102
	task_do_option 2049 1
endproc 
proc 103
	task_do_option 2050 1
endproc 
proc 104
	task_do_option 2055 1
endproc 
 
proc 2
	get_trig_task 1 2
	if 2 = 2050
		task_fini 2050 1
	endif

endproc

