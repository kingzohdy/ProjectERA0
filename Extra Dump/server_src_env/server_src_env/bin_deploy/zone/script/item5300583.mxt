;;;;;;多功能机器人物品


proc 1

			
	strvar Ret X Y
	is_in_pworld 300 0 Ret 0	
	if Ret = 1
		get_pworld_var 4 32 1 10
		if 10 = 5
		
			dec_item_num_inpak 5300583 1 0
			GET_PLAYER_MAPINFO 0 0 0 X Y
;			new_mon 350044 1  0 0 0 0
			strvar k
			get_pworld_var 102 8 1 k
			var k 2
			set_pworld_var 102 8 @k 1
			
			NEW_LIFE_MON 350044 1 @X @Y 200 0 0 0 0 0 0
			task_fini 3342 1
		else
			add_sys_msg_start
			add_sys_msg 必须要在大地之心5层才可以使用！
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
		
		
	else
			add_sys_msg_start
			add_sys_msg 此物品在大地之心5层才可以使用！
			set_sys_msg_mode 3 0 0
			msg_info
	endif

endproc  