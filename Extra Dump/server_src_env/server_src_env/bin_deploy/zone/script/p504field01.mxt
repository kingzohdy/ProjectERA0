;雷神出现

PROC 1
	strvar Ret
	task_stat 3111 1 Ret
	if Ret = 2
		get_pworld_var 0 32 1 10
		if 10 != 1
			var 10 0
			set_pworld_var 0 32 @10 1
		endif
		if 10 = 0
;			effect 0 0 playEffct(7779,8542,"effect\\entity\\282018_18.ent")
			NEW_MON 282018 1 7779 8542 0 0 0 0 0
			get_pworld_var 0 32 1 10
			var 10 1
			set_pworld_var 0 32 @10 1
		endif
	endif
	
ENDPROC
;雷神逻辑区域
proc 2
	strvar Ret1 RetID So So3
	TASK_STAT 3114 1 Ret1

	if Ret1 = 2
		get_task_var 3114 0 8 1 So 

		get_cmp_fairyid 1 RetID	
		if So != 10 
			if RetID = 0
				new_mon 202005 1 7779 8542 0 0 0 0 0

				add_sys_msg_start
				add_sys_msg 恶魔之影已经出现，记住他！杀了他！
				set_sys_msg_mode 1 0 0
				msg_info
				get_task_var 3114 0 8 1 So 
				var So 10
				set_task_var 3114 0 8 @So 1 
			endif
		endif
		
		if RetID = -1
			get_task_var 3114 3 8 1 So3
			add So3 1
			set_task_var 3114 3 8 @So3 1
			if So3 = 10
				add_sys_msg_start
				add_sys_msg 为了您的生命安全，请先将雷神【快捷键 P 】召唤出来！
				set_sys_msg_mode 1 0 0
				msg_info
				var So3 0
				set_task_var 3114 3 8 @So3 1
			endif
			
			get_task_var 3114 0 8 1 So 
			if So = 10
				TASK_FAIL 3114 1
			endif
			
		endif

	endif
	
endproc

