;高阶洗炼石包

proc 1
    strvar num ret lv
	GET_PACK_SPACE 0 num
	
	if @num < 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG 背包需要1个格子才装得下哦！请先整理背包再开启
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
		return
	endif
	
	DEC_ITEM_NUM_INPAK 5052267 1 ret
	
	if  @ret = 0
		GET_PLAYER_BASE_ATT 0 0 lv 0
		if lv < 40
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200045 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【高级洗炼石包】获得“高阶一级洗炼石”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 40 and lv < 60
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200046 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【高级洗炼石包】获得“高阶二级洗炼石”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 60 and lv < 80
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200047 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【高级洗炼石包】获得“高阶三级洗炼石”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 80 and lv < 120
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200048 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【高级洗炼石包】获得“高阶四级洗炼石”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
		if lv >= 120
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200049 1 1 0
			
			add_sys_msg_start
			add_sys_msg 恭喜玩家“$_player$”开启【高级洗炼石包】获得“高阶五级洗炼石”！
			set_sys_msg_mode 18 0 0
			svr_msg_info
		endif
	endif
endproc
