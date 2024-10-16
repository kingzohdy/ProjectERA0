;;;;吸附器 



proc 1

			
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2201
		
		strvar X Y Rad
;		dec_item_num_inpak 5300717 1 0
		GET_PLAYER_MAPINFO 0 0 0 X Y
;		del_mon 1
;		NEW_MON 293190 1 @X @Y 0 0 0 0 0

		rand Rad 3
		if Rad = 0
			NEW_LIFE_MON 293190 1 @X @Y 1000 0 0 0 30 0 0			
		endif
		if Rad = 1
		
			NEW_LIFE_MON 293190 2 @X @Y 1000 0 0 0 30 0 0	
		endif
		if Rad = 2
		
			NEW_LIFE_MON 293190 3 @X @Y 1000 0 0 0 30 0 0	
		endif

	else
		add_sys_msg_start
		add_sys_msg 物品只可在风徊裂谷地图使用！
		set_sys_msg_mode 1 0 0
		msg_info
	
	endif

endproc  