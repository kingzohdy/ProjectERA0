
proc 1
	strvar tim time2 time3 loc
	var tim 0
	set_mon_var 0 8 @tim 1

	var time2 0
	set_mon_var 1 8 @time2 1
	
	var time3 0
	set_mon_var 2 8 @time3 1
	
	var loc 0
	set_mon_var 3 8 @loc 1
endproc 

proc 4
	strvar Mid
	get_mon_var 0 8 1 tim
	add tim 1
	set_mon_var 0 8 @tim 1
	if tim <= 80
	;5分钟之内 隔30秒冒泡
		get_mon_var 1 8 1 time2
		add time2 1
		set_mon_var 1 8 @time2 1
		if time2 = 20
			strvar rad
			GET_MON_INFO 0 0 Mid 0 0 0 0 0
			rand rad 4
			if rad = 0
				mon_speech @Mid 0 1 1 煮呀煮啊煮紫薯，煮了一锅好紫薯，耐心等个几分钟，紫薯就熟了。
			endif
			if rad = 1
				mon_speech @Mid 0 1 1 翻滚吧，紫薯。
			endif
			if rad = 2
				mon_speech @Mid 0 1 1 紫薯很快就熟了，慢慢等哟。
			endif
			if rad = 3
				mon_speech @Mid 0 1 1 当倒计时少于1分钟的时候，就赶紧抢紫薯吧。
			endif
			var time2 0
			set_mon_var 1 8 @time2 1
		endif
		
		;
		get_mon_var 2 8 1 time3
		add time3 1
		set_mon_var 2 8 @time3 1
		if time3 = 3
			var time3 0
			set_mon_var 2 8 @time3 1
			
			get_mon_var 3 8 1 loc
			var loc 0
			set_mon_var 3 8 @loc 1
			
		endif
	endif
endproc 

proc 6
	get_mon_var 0 8 1 tim
	if tim <= 80
		PLAYER_COLLECT_BREAK
		GET_MON_INFO 0 0 Mid 0 0 0 0 0
		get_mon_var 3 8 1 loc
		if loc = 0
			;9秒内只说一次
			mon_speech @Mid 0 1 1 你太着急了，少年，吃半生不熟的紫薯可是会拉肚子的哟。
			var loc 1
			set_mon_var 3 8 @loc 1
			
			var time3 0
			set_mon_var 2 8 @time3 1
		endif
	else
		var tim 101	
		set_mon_var 0 8 @tim 1
	
	endif
endproc 








