


proc 1

	dialog 1
	add_talk $_npc$:
	add_talk #b#b���Ƿ��˺ô�������������������������֮�أ����ѡ����Ǵ󲿷��ӹ�������֮ͽ�Ľ�ɱ���������Ծ�ʧȥ�˺ܶ��ֵܡ�
		task_add_option 1957 1 100
		task_add_option 1958 1 101
		task_add_option 1959 1 102
		
		TASKCYCLE_ADD_OPTION 122 1 150
	talk
	
endproc
proc 150
	TASKCYCLE_DO_OPTION 122 1
endproc 
proc 100
	task_do_option 1957 1
endproc 
proc 101
	task_do_option 1958 1
endproc 
proc 102
	task_do_option 1959 1
endproc 