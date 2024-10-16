;;;;闪光弹

proc 1

			
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 33220 9584 10

	if 2 = 2201
		if 10 <= 5000
			
;			dec_item_num_inpak 5300724 1 0
;			del_mon 1
			strvar X Y
			GET_PLAYER_MAPINFO 1 1 1 X Y
			CALL_MON 1 205025 500 60 @X @Y 1
		else
			add_sys_msg_start
			add_sys_msg 需要在指定区域才可使用！
			set_sys_msg_mode 3 0 0
			msg_info
		endif

	else
		add_sys_msg_start
		add_sys_msg 物品只可在风徊裂谷地图使用！
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
		

endproc  