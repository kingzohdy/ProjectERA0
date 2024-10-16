;单人护送 30 59


proc 5
	COMMIT_TASK 1096 1
endproc

;proc 6
;	strvar x y m
;	GET_PLAYER_MAPINFO 1 1 1 x y
;	GET_POINT_DIST @x @y 14332 9328 m
;	if m < 500
;		task_fini 30001 1
;	endif
	
;endproc

proc 3
	strvar num chunjie ret suiji
	GET_PACK_SPACE 0 num
	GET_EVENT_STAT chunjie 302
	
	if chunjie = 1
		if @num < 1
			ADD_SYS_MSG_START
			ADD_SYS_MSG 请空出1格背包格子再前来领取
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
		else
			rand suiji 100
			if suiji < 60
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 25 1 0
			endif
			if suiji >= 60 and suiji < 95
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 30 1 ret
				
				if ret = 0
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”在春节期间完成【运送货车】任务，获得30个“星辉宝钻”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
				endif
			endif
			if suiji >= 95
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5200020 50 1 ret
				
				if ret = 0
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”在春节期间完成【运送货车】任务，获得50个“星辉宝钻”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
				endif
			endif
		endif
	endif
endproc
