;焚木村区域1

PROC 1

	TASK_STAT 1314 1 20	
	
	if 20 = 2 
		
		GET_ITEM_NUM_INPAK 5300408 0 49
		if 49 = 0
			add_sys_msg_start
			add_sys_msg 这片区域似乎有些不同，仔细探索一下吧！
			set_sys_msg_mode 3 0 0
			msg_info
			get_task_var 1314 0 32 1 10
			var 10 1
			set_task_var 1314 0 32 @10 1
		endif
	endif

ENDPROC

proc 2
	
	strvar Ret Mid
	get_task_var 1314 4 32 1 11
	GET_ID_MON_INFO @11 Ret 1 1 1 1 1 1
	if Ret != -1
		return
	endif
	
	TASK_STAT 1314 1 20
	if 20 = 2 

		get_task_var 1314 0 32 1 10
		add 10 1
		set_task_var 1314 0 32 @10 1
		if 10 = 3	
			GET_ITEM_NUM_INPAK 5300408 0 50
			if 50 = 0		
				add_sys_msg_start
				add_sys_msg 这里好像有什么东西？
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		
		endif 
		
		if 10 = 5
		
			GET_ITEM_NUM_INPAK 5300408 0 51
			if 51 = 0
				GET_PLAYER_MAPINFO 0 0 0 30 31
				call_mon 1 293076 0 30 @30 @31 Mid
				
				get_task_var 1314 4 32 1 11
				var 11 @Mid
				set_task_var 1314 4 32 @11 1
				
				add_sys_msg_start
				add_sys_msg 附近出现了一个祭器！
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	
	endif

endproc
