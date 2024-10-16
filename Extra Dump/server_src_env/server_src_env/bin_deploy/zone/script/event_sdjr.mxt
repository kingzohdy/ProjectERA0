;;;;圣诞活动



proc 1
	strvar Tim Loc
	var Tim 0
	set_event_var 0 32 @Tim 1
	
	; add_sys_msg_start
	; add_sys_msg 圣诞老公公已经将礼物堆放到时间城的大圣诞树下啦！大家快来抢呀！
	; set_sys_msg_mode 6 0 0
	; SVR_MSG_INFO
	
endproc 

proc 2
	add_sys_msg_start
	add_sys_msg 今天圣诞老公公的礼物已经派送完啦！祝大家圣诞节快乐！
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
endproc 

proc 3

	;;;;10点开始刷箱子
	get_event_var 0 32 1 Tim
	add Tim 1
	set_event_var 0 32 @Tim 1
	
	if Tim = 1
		add_sys_msg_start
		add_sys_msg 圣诞老公公已经将礼物堆放到时间城的大圣诞树下啦！大家快来抢呀！
		set_sys_msg_mode 1 0 0
		SVR_MSG_INFO
		; 刷箱子	
		call 110
	endif
	
	; if Tim = 630
		; add_sys_msg_start
		; add_sys_msg 10分钟后圣诞老公公将在时间城的大圣诞树下堆满礼物！不过路过不要错过呀！ 
		; set_sys_msg_mode 1 0 0
		; SVR_MSG_INFO
	; endif
	; if Tim = 690
		; add_sys_msg_start
		; add_sys_msg 5分钟后圣诞老公公将在时间城的大圣诞树下堆满礼物！不过路过不要错过呀！ 
		; set_sys_msg_mode 1 0 0
		; SVR_MSG_INFO
	; endif
	
	if Tim >= 720
		var Tim 0
		set_event_var 0 32 @Tim 1
	endif

endproc 


proc 110
	 ;刷箱子
	NEW_MAP_MON 352101 20 1000 29504 33330 800 1 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 38847 34238 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 21414 46004 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 29448 29696 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 23301 29856 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 23349 33149 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 38795 29661 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 37960 32411 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 39694 31108 800 0 0 -1 600 0 0
	NEW_MAP_MON 352101 20 1000 38432 43607 800 0 0 -1 600 0 0
endproc



