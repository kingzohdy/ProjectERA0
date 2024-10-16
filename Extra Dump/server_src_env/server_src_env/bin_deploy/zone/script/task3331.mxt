; 嘉年华-夺宝

Proc 3
	strvar num ret
	rand num 1000
		if num < 100
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052988 1 1 ret
			
			if ret = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”完成【嘉年华-夺宝】任务获得“三级宝石袋”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if num >= 100 and num < 110
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052989 1 1 ret
			
			if ret = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”完成【嘉年华-夺宝】任务获得“四级宝石袋”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if num >= 110 and num < 111
			ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052990 1 1 ret
			
			if ret = 0
				add_sys_msg_start
				add_sys_msg 恭喜玩家“$_player$”完成【嘉年华-夺宝】任务获得“五级宝石袋”！
				set_sys_msg_mode 18 0 0
				svr_msg_info
			endif
		endif
		if num >= 111
		endif
endproc
