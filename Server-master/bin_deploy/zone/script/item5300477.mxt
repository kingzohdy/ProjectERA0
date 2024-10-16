;羽蛇神像 对玛吉先知加减防BUFF


Proc 1
	STRVAR id	
		GET_TARGET_INFO 3 1 id
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 10600 28100 100
				if 2 = 2500
					if 100 <= 500
					TARGET_ADD_STATUS 3 @id 1011 2 20 1
							if @1 = -1
								return
							else 
								DEC_ITEM_NUM_INPAK 5300477 1 1
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
			
				
				