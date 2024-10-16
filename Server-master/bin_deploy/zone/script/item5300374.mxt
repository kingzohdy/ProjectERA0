;符文石板  选中使用

proc 1

	get_player_mapinfo 0 1 2 3 4
	
	if 2 = 2300

;			dec_item_num_inpak 5300374 1 0
			
			DESTROY_MON 1			
			GET_MON_INFO 10 11 12 13 14 15 16 17 
			CALL_MON 1 226003 0 60 @16 @17 1	
			

			add_sys_msg_start
			add_sys_msg 消灭阿拜多斯侍僧！
			set_sys_msg_mode 4 0 0
			msg_info

		else      						
			add_sys_msg_start
			add_sys_msg 目标不在指定地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	endif
 
endproc
