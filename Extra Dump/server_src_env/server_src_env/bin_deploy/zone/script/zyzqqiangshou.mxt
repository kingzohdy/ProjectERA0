;枪手试炼师萨菲罗

proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b保持你的冷静与果断。
		
		task_add_option 3161 1 101
		task_add_option 3153 1 102
		task_add_option 3162 1 103
		
		task_stat 3153 1 30
		get_task_var 3153 0 8 1 20
		if 30 = 2 and 20 = 0
			option 160 #试炼一：击败小机器人
		endif
		
		task_stat 3162 1 30
		get_task_var 3162 0 8 1 20
		if 30 = 2 and 20 = 0
			option 161 #试炼二：击败炼尸犬
		endif
		
		option 152 #回到时间城
	talk
endproc

PROC 101
	task_do_option 3161 1
ENDPROC
PROC 102
	task_do_option 3153 1
ENDPROC
PROC 103
	task_do_option 3162 1
ENDPROC


proc 152
	transmit 1000 23800 27400 1
;	RESET_PWORLD 506
	task_stat 3153 1 30
	task_stat 3162 1 31
	if 30 = 2 
		TASK_FAIL 3153 1
	endif
	if 31 = 2
		TASK_FAIL 3162 1	
	endif

endproc
proc 160
	get_task_var 3153 0 8 1 20
	var 20 1
	set_task_var 3153 0 8 @20 1
	
	GET_MAP_NPC_ID 302057 50 0 
	mon_speech @50 0 2 1 成功击杀试炼机器人前被击中6次就算失败，我只期望你别输得太快。
	add_sys_msg_start
	add_sys_msg 尝试合理使用“翻滚”“震荡射击”“裂风蹴”，牵制击杀试炼机器人。
	set_sys_msg_mode 1 0 0
	msg_info
endproc 
proc 161
		
;		NEW_MON 262001 5 37500 19400 500 0 0 0 0
;		NEW_MON 262001 5 37500 20200 500 0 0 0 0
;		NEW_MON 262001 5 37500 18700 500 0 0 0 0
		get_task_var 3162 0 8 1 20
		var 20 1
		set_task_var 3162 0 8 @20 1
		
		GET_MAP_NPC_ID 302057 50 90
		mon_speech @50 91 2 1 十秒内击杀十只试炼尸犬，动动你的脑子，别让我太无聊了。
		
		add_sys_msg_start
		add_sys_msg 不妨把试炼尸犬吸引到一起后，用“火焰喷射”“榴弹炮”在10秒内结束战斗。
		set_sys_msg_mode 1 0 0
		msg_info
endproc

proc 2
	get_trig_task 1 2
	if 2 = 3153
		
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b保持你的冷静与果断。
		
			option 160 #试炼一：击败小机器人
			
		talk
	endif 
	if 2 = 3162

		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b保持你的冷静与果断。
		
			option 161 #试炼二：击败炼尸犬
			
		talk
			
	endif

endproc








