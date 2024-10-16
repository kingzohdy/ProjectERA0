proc 1


	DIALOG 1
	ADD_TALK $_npc$:       
	ADD_TALK  #b#b要不是上次打赌输给了丹泽尔，老夫才不愿帮他做赛马裁判呢…咳…丹泽尔身为魔法学院大导师，竟然如此对待我们这些长者，下次老夫一定要赌赢他，好出口气。

	
	task_add_option 3131 1 100
	task_add_option 3132 1 101
	
	TALK
	
endproc
PROC 100
	task_do_option 3131 1
ENDPROC
PROC 101
	task_do_option 3132 1
ENDPROC
proc 2
	get_trig_task 1 2
	if 2 = 3132

		call 120
	endif

endproc
proc 120
	strvar mid
	GET_PLAYER_ID 30 0
	PLAYER_RIDE_ON @30 35 36
	GET_ROLE_RIDE_ON_MACHINE 0 mid
	if 35 != 0 and mid = 0
		get_task_var 3132 0 8 1 11
		var 11 1
		set_task_var 3132 0 8 @11 1
					
		get_task_var 3132 5 8 1 12
		var 12 1
		set_task_var 3132 5 8 @12 1
		
		GET_PLAYER_ID 1 30
		TRANSMIT_CAMERA @30 135 1000 20817 34000 0 0
		;把玩家传送到一个位置	
		TARGET_ADD_STATUS 1 @30 3099 1 4 1	
		;AUTO_MOVE 1 1000 31500 34100 1460082 135 3081 1 3600

		
		;1213 1
		;刷赛跑monq
;		NEW_MON 204031 1 20817 34385 0 55 0 0 0
;		NEW_MAP_MON 204031 1 1000 20817 34385 0 55 0 135 0 0 0
		strvar monmid
		NEW_MON_BY_DIR 204031 1 20817 34385 0 55 0 135 0 monmid  
		get_task_var 3132 6 32 1 13
		var 13 @monmid
		set_task_var 3132 6 32 @monmid 1
		
		;播放特效
;		NPC_DIE 0 1
		effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9568_16.ent")	
		effect 2 0 playEffct(21300,34100,"effect\\entity\\9571_16.ent")
	else
			
		add_sys_msg_start
		add_sys_msg 你必须先从坐骑上下来！！
		set_sys_msg_mode 4 0 0
		msg_info
;		TASK_FAIL 3132 1
		return
	endif

endproc 
