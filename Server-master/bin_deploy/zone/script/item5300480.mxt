;哨子  招临时物品 删物品 

Proc 1
	STRVAR id Ret
	task_stat 1726 1 Ret
	if Ret = 4
		return
	endif
	
		;GET_TARGET_INFO 3 1 id
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 14500 29300 100
				if 2 = 2500
					if 100 <= 500
					NEW_OWNER_TMP_ITEM 1460018 14500 29300 0 0 15 0 0 0 0 1
							if @1 = -1
								return
							else 
								;DEC_ITEM_NUM_INPAK 5300480 1 1
								add_sys_msg_start
								add_sys_msg 随着哨音响起，马匹闻声而来，用最快的速度把它骑到阿卡尔处吧。
								set_sys_msg_mode 3 0 0
								msg_info
							endif
					else
						add_sys_msg_start
						add_sys_msg 不在指定区域！
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
						add_sys_msg_start
						add_sys_msg 不可以在此地图使用！
						set_sys_msg_mode 3 0 0
						msg_info
				endif
endproc			
			
				
				