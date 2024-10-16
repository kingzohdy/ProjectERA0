;炸药包


Proc 1
	STRVAR id1 id2
	
	GET_TASK_VAR 1659 4 32 1 11 
	get_task_var 1659 1 8 1 12
	;GET_PLAYER_MAPINFO  #返回标志 #地图定义ID #地图实例ID #x #y
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 15500 13600 100
				if 2 = 3400
					if 100 <= 500
					;NEW_MON 302054 1 15500 13600 50 0 0 1 id
						;CALL_MON 1 302054 50 35 15500 13600 1
						GET_PLAYER_ID 1 id2
						TARGET_ADD_STATUS 1 @id2 1006 2 10 1
						effect 0 0 playEffct(15500,13600,"effect\\entity\\720000_6.ent")
						CALL_MON_BY_PATROL 1 302054 10 25 15500 13600 0 0 id1
							if @id1 = -1
								return
							else
								var 11 @id1
								set_task_var 1659 4 32 @11 1
								var 12 1
								set_task_var 1659 1 8 @12 1
								DEC_ITEM_NUM_INPAK 5300456 1 1
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
			
				
				