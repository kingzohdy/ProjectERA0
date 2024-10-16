;;;;烧酒 祭奠


proc 1

			
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 11200 7500 10

	if 2 = 2201
		if 10 <= 500
			strvar Mid
			GET_PLAYER_ID 1 Mid
;			TARGET_ADD_STATUS 1 @Mid 3007 1 30 1
			EFFECT_ID 0 0 setOverlayRegion("区域_2")
			dec_item_num_inpak 5300711 1 0
			task_fini 1926 1

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