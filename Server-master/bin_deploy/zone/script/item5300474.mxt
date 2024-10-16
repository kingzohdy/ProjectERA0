;绿色伪装


Proc 1
	STRVAR id a
		get_task_var 1711 0 32 1 a
		;GET_TARGET_INFO  1 1 id
		GET_PLAYER_ID 1 id
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 10400 21800 100
				if 2 = 2500
					if 100 <= 500
					TARGET_ADD_STATUS 1 @id 1088 3 3 1
					if 1 = -1
						return
						add_sys_msg_start
						add_sys_msg 变不变啊！
						set_sys_msg_mode 3 0 0
						msg_info
					endif
						
						;effect 0 0 playEffct(15500,13600,"effect\\entity\\720000_6.ent")
						CALL_MON_BY_PATROL 1 233009 200 120 10100 20900 0 0 a
							if @a = -1
								return
							else 
								;DEC_ITEM_NUM_INPAK 5300474 1 1
								set_task_var 1711 0 32 @a 1
									if @1 = -1
										return									
									endif
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
			
				
				