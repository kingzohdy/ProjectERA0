
proc 1
	STRVAR m1 m2 m3 id
	var m1 0
	set_task_var 1731 0 8 @m1 1
	var m2 0
	set_task_var 1731 1 8 @m2 1
	var m3 0
	set_task_var 1731 2 8 @m3 1
endproc

proc 6
	GET_MAP_NPC_ID 303029 20 1
	GET_MAP_NPC_ID 301001 21 1
	GET_MAP_NPC_ID 302022 22 1
	get_task_var 1731 0 8 1 m1
	get_task_var 1731 1 8 1 m2
	get_task_var 1731 2 8 1 m3	
	GET_PLAYER_ID 1 id
	if @m1 = 1  
		mon_speech @20 2 0 1 �����ζ����������û���κ����Կ�����������		
			if 2 = 0
				var @m1 2	
				RUN_TASK_PROGRESS @id 1731 1 1
			endif
	endif
	if @m2 = 1 
		mon_speech @21 2 0 1 ¹�⣬�ó԰����Ҵ���û�Թ���ô�óԵ�¹�⣡
			if 2 = 0
				var @m2 2
				RUN_TASK_PROGRESS @id 1731 1 1
			endif
	endif
	if @m3 = 1 
		mon_speech @22 2 0 1 ֭����������ƽ��ӻ�ۣ���ڼ������ۣ���¹��̫���ˣ�		
			if 2 = 0
				var @m3 2
				RUN_TASK_PROGRESS @id 1731 1 1
			endif
	endif
	;if @m1 = 2 and @m2 = 2 and @m3 = 2
	;	task_fini 1731 0
	;endif
	set_task_var 1731 0 8 @m1 1
	set_task_var 1731 1 8 @m2 1
	set_task_var 1731 2 8 @m3 1
endproc

