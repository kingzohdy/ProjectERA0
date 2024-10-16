
;护送文馨
proc 1
	strvar Mid
	var Mid 0 
	set_task_var 2111 0 32 @Mid 1
	
	;能不能招护送怪的锁
	var 20 0
	set_task_var 2111 4 8 @20 1
	
	strvar time
	var time 0
	set_task_var 2111 5 8 @time 1

endproc 
proc 4
	get_task_var 2111 0 32 1 Mid 
	DEL_ID_MON @Mid 1
endproc 
proc 5
	
	get_task_var 2111 0 32 1 Mid 
	DEL_ID_MON @Mid 1
endproc 

proc 6
	strvar x y m
;	GET_PLAYER_MAPINFO 0 0 0 x y
	get_task_var 2111 0 32 1 Mid 
	
	strvar locgo
	get_task_var 2111 4 8 1 locgo
	if locgo = 1
	
		get_task_var 2111 5 8 1 time 
		add time 1
		set_task_var 2111 5 8 @time 1
		
	;	add_sys_msg_start
	;	add_sys_msg $@time$
	;	set_sys_msg_mode 1 0 0
	;	msg_info
		
		if time = 2
			MON_SPEECH @Mid 1 1 1 前往桑丝村需要经过一个很深很深的峡谷，我们必须快点通过！
		endif
		if time = 10
			MON_SPEECH @Mid 1 1 1 我是不是走的太快了？
		endif
		if time = 19
			MON_SPEECH @Mid 1 1 1 你看这羊肠小道，细的简直就快要断掉。
		endif
		if time = 28
			MON_SPEECH @Mid 1 1 1 可能我有被袭击恐惧症了，我总是很担心有什么东西突然从路边窜出…
		endif
		if time = 37
			MON_SPEECH @Mid 1 1 1 就快要到了，穿过这个峡谷，就要到了，希望一路平安！
		endif
		if time = 46
			MON_SPEECH @Mid 1 1 1 别离我太远，好吗？
		endif
		if time = 55
			MON_SPEECH @Mid 1 1 1 我真的很羡慕你，如果将来我有了孩子，我一定让他学武术…
		endif
		if time = 64
			MON_SPEECH @Mid 1 1 1 如果……我还能有将来……
		endif
		if time = 73
			MON_SPEECH @Mid 1 1 1 冰冻三尺非一日之寒，也许连上帝都不清楚，世界变成这样是谁的错…
		endif
		if time = 82
			MON_SPEECH @Mid 1 1 1 我曾经看过一场电影，就这样过了两千年…然后整个世界都变了…
		endif
		if time = 91
			MON_SPEECH @Mid 1 1 1 我们的世界也会变吗？我们所经历的这一切就是起点吗？
		endif
		if time = 100
			MON_SPEECH @Mid 1 1 1 终于走出那让人发怵的地方了，但愿桑丝村一切正常…
		endif
		if time = 109
			MON_SPEECH @Mid 1 1 1 到了，这儿就是桑丝村了，前面那位您认识吗？
		endif
		
		GET_ID_MON_INFO @Mid 1 0 0 0 0 x y

		strvar plm px py
		GET_PLAYER_MAPINFO 1 1 1 px py
		GET_POINT_DIST @x @y @px @py plm
		
		if plm > 1000
			add_sys_msg_start
			add_sys_msg 你距离护送目标雯馨太远，请尽快回到雯馨身边
			set_sys_msg_mode 3 0 0
			msg_info	
		endif
		if plm > 2500
			add_sys_msg_start
			add_sys_msg 四处都找不着雯馨的踪迹，完了，跟丢了…
			set_sys_msg_mode 3 0 0
			msg_info
			TASK_FAIL 2111 1
		endif
		
		GET_POINT_DIST @x @y 31300 31400 m	
		if m <= 500
			task_fini 2111 1 
		;	get_task_var 2111 0 32 1 Mid 
			DEL_ID_MON @Mid 1
			
			var time 110
			set_task_var 2111 5 8 @time 1
			
		endif
	endif
endproc 

