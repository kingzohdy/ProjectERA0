;队伍传送门 ,队伍传送门
PROC 1	
	strvar lv
	get_mon_info 1 2 0 0 0 0 0 0 
	GET_PLAYER_BASE_ATT 1 1 lv 1
	;时间城
	if 2 = 333069
		TRANSMIT 1000 26600 28500 1
	endif
	;牢城
	if 2 = 333070
		if lv >= 70
			TRANSMIT 3200 6300 21100 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等70级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;普罗圣地
	if 2 = 333071
		if lv >= 60
			TRANSMIT 3100 13100 1200 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等60级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;法老陵
	if 2 = 333072
		if lv >= 70
			TRANSMIT 3304 25600 48600 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等70级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;阿努比斯祭坛
	if 2 = 333073
		if lv >= 30
			TRANSMIT 3302 4500 7200 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等30级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;阿努比斯神殿
	if 2 = 333074
		if lv >= 30
			TRANSMIT 3300 1400 7500 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等30级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;阿努比斯废墟
	if 2 = 333075
		if lv >= 30
			TRANSMIT 3303 10500 3700 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等30级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;越雾甬道
	if 2 = 333076
		if lv >= 50
			TRANSMIT 3500 5300 12900 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等50级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	;十三号矿洞
	if 2 = 333077
		if lv >= 40
			TRANSMIT 3400 3500 3100 1
		else
		
			add_sys_msg_start
			add_sys_msg 此地图太危险了，为了您的身家性命，还是等40级以后再去吧！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
	
ENDPROC
