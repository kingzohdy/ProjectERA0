;;;;土灵活力 糊糊



proc 1

			
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2201
		
;			dec_item_num_inpak 5300592 1 0
			task_fini 1918 1
		else
			add_sys_msg_start
			add_sys_msg 物品只可在风徊裂谷地图使用！
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
		

endproc  