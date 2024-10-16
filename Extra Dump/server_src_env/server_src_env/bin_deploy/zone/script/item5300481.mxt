;玛吉圣物 放进水底完成任务


Proc 1
	STRVAR id	
		;GET_TARGET_INFO 3 1 id
		get_player_mapinfo 0 1 2 3 4
		get_point_dist @3 @4 17300 17000 100
				if 2 = 2500
					if 100 <= 500
								effect 0 0 playEffct(16900,17000,"effect\\entity\\9589_16.ent")
								DEC_ITEM_NUM_INPAK 5300481 1 1
									if 1 = -1
										return
									else
										TASK_FINI 1714 1
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
			
				
				