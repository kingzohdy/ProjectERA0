;堕落黑暗魔杖


proc 1

	STRVAR Ret Direc MapID EndX EndY 
	USE_TREASURE_PIC 5300322 1000 Ret Direc MapID EndX EndY 
	dev EndX 100
	dev EndY 100

	 
	if Ret = 0 
		dec_item_num_inpak 5300322 1 0
		add_item_num_inpak 5300292 1 1 0
		
		GET_PLAYER_MAPINFO 0 20 21 22 23
		effect 0 0 playEffct($@22$,$@23$,"effect\\entity\\9552_16.ent")	
		
		add_sys_msg_start
		add_sys_msg 你找到了黑暗法典！
		set_sys_msg_mode 3 0 0
		msg_info
		return
	endif
	if Ret = 1
		add_sys_msg_start
		add_sys_msg 目的地不在此地图！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
;	if Ret = 2
;		add_sys_msg_start
;		add_sys_msg 目标已经很近了，就在$@EndX$ $@EndY$处！
;		set_sys_msg_mode 3 0 0
;		msg_info
;	endif
	if Ret > 0
		if Direc = 1
				add_sys_msg_start
				add_sys_msg 目标就在北方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 2
				add_sys_msg_start
				add_sys_msg 目标就在东北方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 3
				add_sys_msg_start
				add_sys_msg 目标就在东方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 4
				add_sys_msg_start
				add_sys_msg 目标就在东南方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 5
				add_sys_msg_start
				add_sys_msg 目标就在南方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 6
				add_sys_msg_start
				add_sys_msg 目标就在西南方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		if Direc = 7
				add_sys_msg_start
				add_sys_msg 目标就在西方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		
		endif
		if Direc = 8
				add_sys_msg_start
				add_sys_msg 目标就在西北方不远处！
				set_sys_msg_mode 3 0 0
				msg_info
		endif
		
	
	endif

endproc


