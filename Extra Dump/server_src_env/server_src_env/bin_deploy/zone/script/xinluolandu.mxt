
proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���������ߵ������������ǲ���ѡ���ҵع����°뱲�ӣ��԰ɣ�$_player$��
	task_add_option 1050 1 101	
	task_add_option 1051 1 102
;	task_add_option 1052 1 103
	task_add_option 1053 1 104
	
	talk
endproc
proc 101
    task_do_option 1050 1
endproc
proc 102
    task_do_option 1051 1
endproc

proc 104
    task_do_option 1053 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1053
		task_fini 1053 1
	endif
endproc
