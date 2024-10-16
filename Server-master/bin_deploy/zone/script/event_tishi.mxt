;;;;圣诞活动10分钟提示一次



proc 1
	add_sys_msg_start
	add_sys_msg 每天10:00-22:00，圣诞老公公将每隔2小时在时间城的大圣诞树下堆满礼物，先到先得哦！
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
	
	strvar Tim 
	var Tim 0
	set_event_var 0 32 @Tim 1
endproc 

proc 2
	get_event_var 0 32 1 Tim
	var Tim 0
	set_event_var 0 32 @Tim 1
endproc 

proc 3
	get_event_var 0 32 1 Tim
	add Tim 1
	set_event_var 0 32 @Tim 1
	
	if Tim = 60
		add_sys_msg_start
		add_sys_msg 20分钟后圣诞老公公将在时间城的大圣诞树下堆满礼物！不过路过不要错过呀！
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO
	endif
	
	if Tim = 120
		add_sys_msg_start
		add_sys_msg 10分钟后圣诞老公公将在时间城的大圣诞树下堆满礼物！不过路过不要错过呀！ 
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO
	endif
	
	if Tim = 150
		add_sys_msg_start
		add_sys_msg 5分钟后圣诞老公公将在时间城的大圣诞树下堆满礼物！不过路过不要错过呀！ 
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO	
	endif
endproc 
