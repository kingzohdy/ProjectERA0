;法力残骸  选中玩家

proc 1
	  get_player_mapinfo 0 1 2 3 4
	  if 2 = 2300

;			effect 0 0 playEffct(6800,6001,"effect\\entity\\9544_16.ent")
;			dec_item_num_inpak 5300369 1 0
			GET_TARGET_INFO 1 1 20	
			GET_ID_PLAYER_STATUS_INFO @20 1099 60 3 0 0 0
			if 60 = 0
				add_sys_msg_start
				add_sys_msg 不可以对有状态的目标使用……
				set_sys_msg_mode 3 0 0
				msg_info

			else
				TARGET_ADD_STATUS 1 @20 1205 1 30 40

				if 20 != -1
					
					dec_item_num_inpak 5300369 1 0
					TASK_FINI 1518 0
				endif
			
			endif
	 else
		add_sys_msg_start
		add_sys_msg 请在怒风荒漠地图中使用！
		set_sys_msg_mode 4 0 0
		msg_info
	 
	 endif
			
endproc


				