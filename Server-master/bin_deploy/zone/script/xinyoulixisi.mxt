;������˹ 
; Я������

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�Ҿ�������������Ҫ�ģ�������������û�������ǽ�����˵ġ�
	add_talk #b#b����ÿһ���˶��������˵Ĺ��Լ������е������ӵ���Լ���Ҫ��һ�й�Ȼ�ܺã��������������ǡ�ɳ�����ս���������û��ǿ�������壬�ѵ���Ҫȥ���������أ�����һ��Ҫ�������������������Ŭ��������������������һ�죬���������ʵ��
	task_add_option 1069 1 101	
;	task_add_option 1071 1 102
	task_add_option 1072 1 103
	task_add_option 1073 1 104
	task_add_option 1078 1 105
	task_add_option 1079 1 106

	talk
endproc
proc 101
    task_do_option 1069 1
endproc

proc 103
    task_do_option 1072 1
endproc
proc 104
    task_do_option 1073 1
endproc
proc 105
    task_do_option 1078 1
endproc
proc 106
    task_do_option 1079 1
endproc

proc 2
	get_trig_task 1 2
	if 2 = 1073
		effect 0 0 UseTaskItemToMonsterLimit(1073, 99, 250001)
	endif
	if 2 = 1079
		task_fini 1079 1
		
		autofly 121 1
	endif

endproc 
