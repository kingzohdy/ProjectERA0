
proc 1
	strvar Mid
	GET_MON_INFO 1 1 Mid 1 1 1 1 1 
	MON_SPEECH @Mid 1 2 1 你还不够格见识我真正的力量，先让我的小弟陪你玩玩……好好享受吧！
	strvar a1 a2
	var a1 0
	set_mon_var 0 8 @a1 1
	var a2 0
	set_mon_var 1 8 @a2 1
endproc 

proc 4
	get_mon_var 0 8 1 a1
	if a1 = 0
		get_mon_var 1 8 1 a2
		add a2 1
		set_mon_var 1 8 @a2 1
		if a2 = 2
			strvar Mid
			GET_MON_INFO 1 1 Mid 1 1 1 1 1 
			MON_SPEECH @Mid 1 2 1 我想你还没聪明到，懂得#c0181f9使用精灵！#n
			var a1 1
			set_mon_var 0 8 @a1 1
		endif
	endif
endproc 