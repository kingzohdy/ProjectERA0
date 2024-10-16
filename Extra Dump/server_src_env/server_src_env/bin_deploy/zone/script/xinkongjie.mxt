;空姐
; 携带任务\传送铜鼓村

proc 1

	get_role_var 0 8 1 10	;设置玩家变量
	GET_AUTO_FLY_INFO 50 3 1
	
	if 10 = 1 and 50 = 1
		transmit 1000 28154 6672 1
                
		var 10 2
		set_role_var 0 8 @10 1

		return
	endif

	dialog 1
	add_talk $_npc$:
	add_talk #b#b啊，$_player$，我听说您很久了呢。时间城的阿尔弗雷德大人正等着您呢！等您10级了，我就将您送到时间城去，阿尔弗雷德大人还有更多更重要的事情要交给您来完成呢！
	add_talk #b#b怎么样，您准备好前往时间城了吗？
	
;	task_add_option 1077 1 102
	task_add_option 1074 1 104
	task_add_option 1076 1 105
	
	strvar Ret
	TASK_STAT 1076 1 Ret
	if Ret = 1 or Ret = 3
		option 120 #去时间城
	endif
	talk
endproc

proc 104
    task_do_option 1074 1
endproc
proc 105
    task_do_option 1076 1
endproc

proc 2

	get_trig_task 1 50
	if 50 = 1076
		dialog 1
		add_talk $_npc$:
		add_talk #b#b啊，$_player$，我听说您很久了呢。时间城的阿尔弗雷德大人正等着您呢！等您10级了，我就将您送到时间城去，阿尔弗雷德大人还有更多更重要的事情要交给您来完成呢！
		add_talk #b#b怎么样，您准备好前往时间城了吗？	
	
			task_fini 1076 10
			option 120 #去时间城
		talk
	endif
endproc

proc 120

	get_player_base_att 1 2 3 4
	
	if 3 >= 10
		get_role_var 0 8 1 10
		if 10 = 2 
			transmit 1000 28154 6672 1
		else	
			
			autofly 102 1	
			set_role_var 0 8 1 1
		endif

	else
		
		add_sys_msg_start
		add_sys_msg 实在抱歉,您的等级未达到10级,请您过段时间再来吧...
		set_sys_msg_mode 4 0 0
		msg_info
		FOLLOW_CANCEL 
	endif


endproc





