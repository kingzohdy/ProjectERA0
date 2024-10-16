;;;;;动作任务


proc 1

	strvar Tim
	var Tim 0
	set_task_var 4604 0 8 @Tim 1
	strvar Loc
	var Loc 0
	set_task_var 4604 1 8 @Loc 1
	;
	
	strvar Tim2
	var Tim2 0
	set_task_var 4604 2 8 @Tim2 1
	;计时器  10秒内没做表情任务失败
	strvar Loc2
	var Loc2 0
	set_task_var 4604 3 8 @Loc2 1
	
	strvar Loc3
	var Loc3 0
	set_task_var 4604 4 8 @Loc3 1
	;点击开始选项前锁定变量
	
	strvar Loc4
	var Loc4 0
	set_task_var 4604 5 8 @Loc4 1
	
	strvar LocNum
	var LocNum 0
	set_task_var 4604 6 8 @LocNum 1
	;等于6的时候完成任务
	
endproc

proc 6
	
	get_task_var 4604 4 8 1 Loc3 
	if Loc3 = 0
		return
	endif
	;点击开始选项前，走出范围时,return
	strvar px py M
	GET_PLAYER_MAPINFO 1 1 1 px py
	GET_POINT_DIST 4700 16900 @px @py M

	if M > 1000
		var Loc3 0
		set_task_var 4604 4 8 @Loc3 1 
		add_sys_msg_start
		add_sys_msg 您距离阿维纳太远，无法与他配合，请与阿维纳对话，重新开始表演
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	get_task_var 4604 0 8 1 Tim
	add Tim 1
	set_task_var 4604 0 8 @Tim 1	
	if Tim = 1
		GET_MAP_NPC_ID 302095 50 0 
		mon_speech @50 0 0 1 亲爱的#Name#，请留意我的要求，做出与我要求一致的动作才可以哟！	
		;;最初开始时赋值1，做动作跳出proc 8
		var Loc4 1
		set_task_var 4604 5 8 @Loc4 1
	endif
	if Tim < 5
		return
	endif

	;前5秒还没开始做动作时跳出
	if Tim = 5
		get_task_var 4604 1 8 1 Loc
		GET_MAP_NPC_ID 302095 50 0 
		
		strvar Rad
		rand Rad 14
		if Rad = 0
			var Loc 0
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，跳舞，即将开始！
		endif
		if Rad = 1
			var Loc 10
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，耍酷，即将开始！
		endif
		if Rad = 2
			var Loc 20
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，装死，即将开始！
		endif
		if Rad = 3
			var Loc 30
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，乞讨，即将开始！
		endif
		if Rad = 4
			var Loc 40
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，痛苦，即将开始！
		endif
		if Rad = 5
			var Loc 50
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，鼓舞，即将开始！
		endif
		if Rad = 6
			var Loc 60
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，欢呼，即将开始！
		endif
		if Rad = 7
			var Loc 70
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，飞吻，即将开始！
		endif
		if Rad = 8
			var Loc 80
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，攻击，即将开始！
		endif
		if Rad = 9
			var Loc 90
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，鼓掌，即将开始！
		endif
		if Rad = 10
			var Loc 100
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，作揖，即将开始！
		endif
		if Rad = 11
			var Loc 110
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，无奈，即将开始！
		endif
		if Rad = 12
			var Loc 120
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，挥手，即将开始！
		endif
		if Rad = 13
			var Loc 130
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 下一个动作，深思，即将开始！
		endif
	endif
	
;	get_task_var 4604 1 8 1 Loc
;	add_sys_msg_start
;	add_sys_msg -----------计时L-o-c=$@Loc$
;;	set_sys_msg_mode 1 0 0
;	msg_info
	
	if Tim = 10
		get_task_var 4604 1 8 1 Loc
		GET_MAP_NPC_ID 302095 50 0 
		
		if Loc = 0
			mon_speech @50 0 0 1 #G跳舞#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif
		
		if Loc = 10
			mon_speech @50 0 0 1 #G耍酷#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif
		if Loc = 20
			mon_speech @50 0 0 1 #G装死#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 30
			mon_speech @50 0 0 1 #G乞讨#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 40
			mon_speech @50 0 0 1 #G痛苦#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 50
			mon_speech @50 0 0 1 #G鼓舞#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 60
			mon_speech @50 0 0 1 #G欢呼#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 70
			mon_speech @50 0 0 1 #G飞吻#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 80
			mon_speech @50 0 0 1 #G攻击#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 90
			mon_speech @50 0 0 1 #G鼓掌#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 100
			mon_speech @50 0 0 1 #G作揖#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 110
			mon_speech @50 0 0 1 #G无奈#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 120
			mon_speech @50 0 0 1 #G挥手#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 130
			mon_speech @50 0 0 1 #G深思#n，开始！
			
			;NPC喊出动作后变量赋值0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		
;		add_sys_msg_start
;		add_sys_msg 10秒时L-o-c=$@Loc$
;		set_sys_msg_mode 1 0 0
;		msg_info
	endif
	if Tim >= 20
		var Tim 4
		set_task_var 4604 0 8 @Tim 1
	endif
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;计数6次完成任务
	get_task_var 4604 6 8 1 LocNum
	if LocNum >= 6
		GET_MAP_NPC_ID 302095 50 0 
		mon_speech @50 0 0 1 OK，看起来你很有表演天分！希望还有机会能看到你的精彩演出，再见。
		task_fini 4604 1
	endif	
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;10秒不做出正确的动作提示重做
	if Tim >= 10 
	
		get_task_var 4604 5 8 1 Loc4
		if Loc4 = 1
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			return
		endif	
		;动作成功后不进入此判断
		get_task_var 4604 2 8 1 Tim2 
		add Tim2 1
		set_task_var 4604 2 8 @Tim2 1

		if Tim2 = 10
			add_sys_msg_start
			add_sys_msg 你在10秒内没做出动作，请重新点击NPC开始！
			set_sys_msg_mode 4 0 0
			msg_info
			call 101
		endif
	endif
endproc

proc 100
	add_sys_msg_start
	add_sys_msg 动作使用错误，请重新点击NPC开始
	set_sys_msg_mode 4 0 0
	msg_info
	call 101
;	TASK_FAIL 4604 1
endproc
proc 101 
	get_task_var 4604 4 8 1 Loc3 
	var Loc3 0
	set_task_var 4604 4 8 @Loc3 1
	
	;计数清零
	var LocNum 0
	set_task_var 4604 6 8 @LocNum 1	
	
	get_task_var 4604 2 8 1 Tim2 
	var Tim2 0
	set_task_var 4604 2 8 @Tim2 1
	
	
	get_task_var 4604 1 8 1 Loc
	var Loc 0
	set_task_var 4604 1 8 @Loc 1
endproc




proc 8
	strvar Ret
	task_stat 4604 1 Ret
	if Ret = 1
		return
	endif	


	get_task_var 4604 4 8 1 Loc3 
	if Loc3 = 0
		return
	endif
	;点击开始选项后
	
	get_task_var 4604 0 8 1 Tim
	if Tim < 5
		return
	endif
	;前5秒还没开始做动作时跳出
	
	get_task_var 4604 5 8 1 Loc4 
	;点击正确的动作后跳出
	if Loc4 = 1
		return
	endif

	
	strvar MotIon
	ID_PLAYER_MOTION 0 0 MotIon    
	GET_MAP_NPC_ID 302095 50 0              
	
	get_task_var 4604 1 8 1 Loc
	get_task_var 4604 5 8 1 Loc4
	get_task_var 4604 6 8 1 LocNum 
	if Loc = 0	
		if MotIon = 10702
		;跳舞
;			var Loc 10
;			set_task_var 4604 1 8 @Loc 1
			
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	
	if Loc = 10
		if MotIon = 10707
		;耍酷
;			var Loc 20
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 20
		if MotIon = 10704
		;装死
;			var Loc 30
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 30
		if MotIon = 10708
		;乞讨
;			var Loc 40
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 40
		if MotIon = 10713
		;痛苦
;			var Loc 50
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 50
		if MotIon = 10703
		;鼓舞
		;	var Loc 30
		;	set_task_var 4604 1 8 @Loc 1
	;		GET_MAP_NPC_ID 302095 50 0 
	;		mon_speech @50 0 0 1 OK，多谢#Name#的倾情演绎，表演到此结束，我们明天再见！
	;		task_fini 4604 1
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 60
		if MotIon = 10700
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 70
		if MotIon = 10701
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 80
		if MotIon = 10705
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 90
		if MotIon = 10706
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 100
		if MotIon = 10709
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 110
		if MotIon = 10710
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 120
		if MotIon = 10711
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 130
		if MotIon = 10712
		;欢呼
			mon_speech @50 0 0 1 太棒了，简直是完美演绎！
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;点击正确的动作后赋值为1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;计数6次完成任务
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
endproc
