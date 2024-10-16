;纹饰戒指
;任务——蝙蝠
;作用——使用后召唤怪物并删除
;需要蝙蝠ID
proc 1

	get_player_mapinfo 0 1 2 3 4
	get_point_dist @3 @4 18400 20201 10
	
	if 2 = 2200
		if 10 <= 500

		CALL_MON 1 218002 500 120 18400 20201 1

		dec_item_num_inpak 5300323 1 0
		add_sys_msg_start
		add_sys_msg 巨形利齿蝙蝠出现了,消灭他吧!
		set_sys_msg_mode 3 0 0
		msg_info
		
		else
			add_sys_msg_start
			add_sys_msg 不是在指定区域哦！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
			add_sys_msg_start
			add_sys_msg 目标不再指定地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	endif
 
endproc
