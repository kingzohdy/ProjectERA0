
;雪原传送风回
proc 1
	strvar Lv
	GET_ID_PLAYER_INFO  0 8 Lv
	if Lv >= 70
		transmit 2201 32700 4400 1
	else
		add_sys_msg_start
		add_sys_msg 此地图太危险了，为了您的身家性命，还是等70级以后再去吧！
		set_sys_msg_mode 3 0 0
		msg_info
	endif	
endproc
