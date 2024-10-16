;苏伊士的相机


proc 1
	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 6531 5966 10
	if 2 = 3303
		if 10 <= 500  
;			effect 0 0 playEffct(4942,14841,"effect\\entity\\9513_16.ent")
;			dec_item_num_inpak 5300285 1 0			
;			add_item_num_inpak 5300339 1 1 0
			task_fini 1547 1

		else
			add_sys_msg_start
			add_sys_msg 不是在这个区域！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	  else
			add_sys_msg_start
			add_sys_msg 此物品需要在阿努比斯废墟使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc


