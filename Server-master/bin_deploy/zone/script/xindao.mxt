;天空岛教官 
; 携带任务\副本502

proc 1
	TASK_STAT 1062 1 3
;	TASK_STAT 1063 1 4
	dialog 1
	add_talk $_npc$:
	add_talk #b#b星星在哪里都是很亮的，就是你没有抬头去看它们。但是飞羽峰的飞龙，再也不会从我的头顶掠过……

	task_add_option 1058 1 102
	task_add_option 1059 1 103
	task_add_option 1055 1 104
	task_add_option 1056 1 105
	task_add_option 1061 1 106
	task_add_option 1062 1 107
	task_add_option 1063 1 108
	task_add_option 1064 1 109


	if 3 = 2
		OPTION 110 #进入飞羽峰
;		OPTION 111 #重置天空岛
	ENDIF

	talk
endproc

proc 102
    task_do_option 1058 1
endproc
proc 103
    task_do_option 1059 1
endproc
proc 104
    task_do_option 1055 1
endproc
proc 105
    task_do_option 1056 1
endproc
proc 106
    task_do_option 1061 1
endproc
proc 107
    task_do_option 1062 1
endproc
proc 108
    task_do_option 1063 1
endproc
proc 109
    task_do_option 1064 1
endproc


proc 110
;	GET_TEAM_PLAYER_ID 10 11 12 13 14 15 16 17 18	
;	IF 10 = -1
		strvar Ret
		enter_pworld 502 0 Ret
		if Ret != -1
			autofly 101 1
		endif
;	ELSE
;		add_sys_msg_start
;		add_sys_msg 此副本为单人副本，请先退出或解散你的退伍！
;		set_sys_msg_mode 1 0 0
;		msg_info
;	ENDIF
	

endproc
proc 111
	RESET_PWORLD 502
endproc


proc 2
	get_trig_task 1 2

	if 2 = 1062
;		RESET_PWORLD 502
		dialog 1
		add_talk $_npc$:
		add_talk #b#b星星在哪里都是很亮的，就是你没有抬头去看它们。但是飞羽峰的飞龙，再也不会从我的头顶掠过……
		OPTION 110 #进入飞羽峰
		talk
	endif

	
	if 2 = 1061
		strvar _Rac 
		GET_ID_PLAYER_INFO 0 7 _Rac
		if _Rac = 1
			add_sys_msg_start
			add_sys_msg 对木桩使用新学会的技能：撕裂
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if _Rac = 2
			add_sys_msg_start
			add_sys_msg 对木桩使用新学会的技能：冰箭术 
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if _Rac = 3
			add_sys_msg_start
			add_sys_msg 对木桩使用新学会的技能：震荡射击
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		if _Rac = 4
			add_sys_msg_start
			add_sys_msg 对木桩使用新学会的技能：雷击术  
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

