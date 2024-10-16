;;;;水晶投网

proc 1

			
	get_player_mapinfo 0 1 2 3 4

	if 2 = 2201
		
;		dec_item_num_inpak 5300725 1 0
		del_mon 1
		ADD_ITEM_NUM_INPAK 5300734 1 1 12
;		CALL_MON 1 205025 500 60 11716 5432 1

	else
		add_sys_msg_start
		add_sys_msg 物品只可在风徊裂谷地图使用！
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
		
endproc  