
proc 1

endproc 

proc 2
	add_sys_msg_start
	add_sys_msg 任务已完成，可通过传送门离开
	set_sys_msg_mode 4 0 0
	msg_info
endproc

proc 3
	call 503
endproc

proc 4
	call 503
endproc

proc 5
	call 503
endproc

proc 6
	get_task_var 4614 0 8 0 50
	if 50 = 1
		get_task_var 4614 4 8 0 51
		add 51 1
		set_task_var 4614 4 8 @51 0
		if 51 = 1
			add_sys_msg_start
			add_sys_msg 每隔10秒将出现新的石头剪刀布！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if 51 = 10 or 51 = 20 or 51 = 30 or 51 = 40 or 51 = 50
			call 501
		elseif 51 = 60 or 51 = 70 or 51 = 80 or 51 = 90
			call 501
		elseif 51 = 100
			NEW_MON 230056 1 12822 12737 1500 0 0 0 0
			NEW_MON 230057 1 12822 12737 1500 0 0 0 0
			NEW_MON 230058 1 12822 12737 1500 0 0 0 0
			
			add_sys_msg_start
			add_sys_msg 整理好后向你旁边的哈利汇报情况
			set_sys_msg_mode 3 0 0
			msg_info
		else
		endif
		if 51 > 0
			call 502
		endif
		; if 51 = 102
			; strvar ret
			; task_stat 4614 1 ret
			; if ret != 1
				; TASK_FAIL 4614 1
				
				; add_sys_msg_start
				; add_sys_msg 未能及时整理石头剪刀布，挑战任务失败了
				; set_sys_msg_mode 4 0 0
				; msg_info
			; endif
		; endif
	endif
endproc

proc 501
	NEW_MON 230056 1 12822 12737 1500 0 0 0 0
	NEW_MON 230057 1 12822 12737 1500 0 0 0 0
	NEW_MON 230058 1 12822 12737 1500 0 0 0 0
	
	add_sys_msg_start
	add_sys_msg 小家伙们又调皮了
	set_sys_msg_mode 3 0 0
	msg_info
endproc

proc 502
	strvar nMon_1 nMon_2 nMon_3
	GET_MAP_MON_NUM 51400 230056 0 nMon_1 0
	GET_MAP_MON_NUM 51400 230057 0 nMon_2 0
	GET_MAP_MON_NUM 51400 230058 0 nMon_3 0
	
	GET_TASK_VAR 4614 1 8 0 10
	if @10 = 1 and @nMon_1 >= 30
		TASK_FINI 4614 0
	endif
	
	GET_TASK_VAR 4614 2 8 0 11
	if @11 = 1 and @nMon_2 >= 30
		TASK_FINI 4614 0
	endif
	
	GET_TASK_VAR 4614 3 8 0 12
	if @12 = 1 and @nMon_3 >= 30
		TASK_FINI 4614 0
	endif
endproc

proc 503
	PWORLD_OVER
	
	get_task_var 4614 0 8 0 50
	VAR 50 0
	set_task_var 4614 0 8 @50 0
	
	get_task_var 4614 1 8 0 10
	VAR 10 0
	set_task_var 4614 0 8 @10 0
	
	get_task_var 4614 2 8 0 11
	VAR 10 0
	set_task_var 4614 0 8 @10 0
	
	get_task_var 4614 3 8 0 12
	VAR 12 0
	set_task_var 4614 0 8 @12 0
	
	get_task_var 4614 4 8 0 51
	VAR 51 0
	set_task_var 4614 0 8 @51 0
endproc
