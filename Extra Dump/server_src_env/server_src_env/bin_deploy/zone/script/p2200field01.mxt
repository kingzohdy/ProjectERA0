;获得戒指区域

PROC 1
	var 10 0
	set_task_var 1412 0 8 @10 1

	TASK_STAT 1412 1 20
		
	if 20 = 2 
			add_sys_msg_start
			add_sys_msg 这片区域似乎有些不同，仔细探索一下吧！
			set_sys_msg_mode 3 0 0
			msg_info
	endif

ENDPROC

proc 2
	
	TASK_STAT 1412 1 20
	if 20 = 2 

		get_task_var 1412 0 8 1 10
		add 10 1
		set_task_var 1412 0 8 @10 1
		if 10 = 8		
			add_sys_msg_start
			add_sys_msg 咦，这里好像有什么东西？
			set_sys_msg_mode 3 0 0
			msg_info
			
		endif 
		
		if 10 = 10
			add_item_num_inpak 5300283 1 1 0
			add_sys_msg_start
			add_sys_msg 在一大堆白雪与白骨中间，你找到了一枚戒指。
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	
	endif

endproc