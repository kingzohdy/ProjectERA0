;;;;石头磨成粉  土灵之锤



proc 1

			
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2201
		
		strvar X Y Rad
;		dec_item_num_inpak 5300590 1 0
		
		GET_TARGET_INFO 3 3 X
		GET_TARGET_INFO 3 4 Y
;		DESTROY_MON 1
;		NEW_MON 293175 1 @X @Y 0 0 0 0 0

		rand Rad 3
		if Rad = 0
			CIRCLE_NEW_MON 293175 2 @X @Y 200 1 2 0 0 0 30 0 0		
		endif
		if Rad = 1
			CIRCLE_NEW_MON 293175 3 @X @Y 200 2 3 0 0 0 30 0 0			
		endif
		if Rad = 2
			CIRCLE_NEW_MON 293175 4 @X @Y 200 3 4 0 0 0 30 0 0				
		endif
		
	else
		add_sys_msg_start
		add_sys_msg 物品只可在风徊裂谷地图使用！
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
		
endproc  