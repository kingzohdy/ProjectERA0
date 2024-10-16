;爆破炸药


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 4500
		strvar Mid Deid zhi
		GET_TARGET_INFO 3 1 Mid
		GET_ID_MON_INFO @Mid 1 Deid 0 0 0 0 0
		if Deid = 208229
			strvar x y hp
			GET_TARGET_INFO 3 3 x
			GET_TARGET_INFO 3 4 y
			GET_TARGET_INFO 3 2 hp
			if hp > 0
				effect 0 0 playEffct($@x$,$@y$,"effect\\entity\\10283_5.ent")
				; add_item_num_inpak 5300861 1 1 0
				; DESTROY_MON 1
				MON_SET_HP -1032375 @Mid 0
				GET_TASK_VAR 3732 0 8 0 zhi
				add zhi 1
				SET_TASK_VAR 3732 0 8 @zhi 0
				if zhi >= 5
					TASK_FINI 3732 0
					dec_item_num_inpak 5301119 1 0
				endif
			else
				add_sys_msg_start
				add_sys_msg 该炮台已被炸毁，无法使用炸药
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 炸药只能对要塞炮台使用
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 要塞炮台在钢铁要塞地图上，请找到炮台再使用炸弹！
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc
