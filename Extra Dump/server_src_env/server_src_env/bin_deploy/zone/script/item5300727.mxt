;;;;;茜玛丽尔宝石



proc 1

			
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2201
		
		strvar X Y 
			
;		dec_item_num_inpak 5300727 1 0
		GET_TARGET_INFO 3 3 X
		GET_TARGET_INFO 3 4 Y
		DESTROY_MON 1
		RUN_TASK_PROGRESS 0 1907 1 1
		NEW_LIFE_MON 205029 1 @X @Y 0 0 0 0 10 0 0

	else
		add_sys_msg_start
		add_sys_msg 物品只可在风徊裂谷地图使用！
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
		

endproc  