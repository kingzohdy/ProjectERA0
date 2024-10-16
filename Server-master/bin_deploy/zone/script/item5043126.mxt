;公会战     ,时空之钥
proc 1
	; strvar enable open
	; get_world_city_enable_time 1 enable
	; if enable = 0
		; return
	; endif
	; get_world_city_open_time 1 open
	; if open > 0
		; return
	; endif
	; set_desig 1 126	
	; set_world_city_enable_time 1 0
	; strvar sec
	; get_time_sec sec
	
	; dev sec 86400
	; mul sec 86400
	; get_now_time 0 0 0 2 0 0 0
	; mul 2 -1
	; add 2 11
	; mul 2 86400
	; add 2 @sec
	; set_world_city_open_time 1 @2
	
	;;公告
	; add_sys_msg_start
	; add_sys_msg 伟大的[$_Player$]开启了通往时间城中控中心区域的通道，时间城城主争霸战将于下周六20：00正式开启  
	; set_sys_msg_mode 1 0 0
	; svr_msg_info
	;;SET_EVENT_BULLETIN 1 300 30 伟大的【$_Player$】开启了通往时间城中控中心区域的通道，时间城城主争霸战将于下周六20：00正式开启
	;;频道6提示
	; add_sys_msg_start
	; add_sys_msg [$_Player$]您成功开启了通往时间城中控中心的通道，获得[发现者称号]  
	; set_sys_msg_mode 6 0 0
	; msg_info
endproc
