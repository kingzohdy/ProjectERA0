; 星辉乐透包

proc 1
	strvar num
	GET_PACK_SPACE 0 num
	if num < 1
		add_sys_msg_start
		add_sys_msg 背包需要1个格子才装得下哦！请先整理背包再开启
		set_sys_msg_mode 1 0 0
		msg_info
	else
		strvar rad ret
		
		DEC_ITEM_NUM_INPAK 5052297 1 100
		
		if 100 = 0
			rand rad 100
			if rad < 30
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 10 1 0
			endif
			if rad >= 30 and rad < 80
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 15 1 ret
				
				if ret = 0
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”开启【星辉乐透包】获得15个“星辉宝钻”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
				endif
			endif
			if rad >= 80	
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 20 1 ret
				
				if ret = 0
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”开启【星辉乐透包】获得20个“星辉宝钻”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
				endif
			endif
		endif
	endif
endproc
