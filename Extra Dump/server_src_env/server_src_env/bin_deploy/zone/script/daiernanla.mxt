

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����ϸ�۲��˺ܾã����ڷ���һֻ��ϸ�������֣�����һֻŮ�˵��֣���Ȼ�Ѿ���������������ܴ�ĳЩϸ���Ϸֱ������
	
	task_add_option 1412 1 101
	task_add_option 1413 1 102
	talk
endproc 
PROC 101
	task_do_option 1412 1
ENDPROC
PROC 102
	task_do_option 1413 1
ENDPROC

proc 2
	get_trig_task 1 2

	if 2 = 1413
		task_fini 1413 1
	endif

endproc
