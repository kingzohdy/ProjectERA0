;特制诱饵
;驯服野骆驼



proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3404
		strvar Hp
		GET_TARGET_INFO 3 2 Hp
		if Hp < 3000

;		GET_MON_INFO 10 11 12 13 14 15 16 17 
;		effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
		dec_item_num_inpak 5300755 1 0
;		add_item_num_inpak 5300299 1 1 0
		DESTROY_MON 1
;		task_fini 2020 1
		strvar Mid
		PLAYER_CALL_BUB 213035 180 Mid
		set_task_var 2020 0 32 @Mid 1	

;		strvar Mid
;		GET_TARGET_INFO 3 1 Mid
;		EFFECT_ID 0 @Mid playMontion(%d,"294005")
		else
			add_sys_msg_start
			add_sys_msg 目标血量过多，请先削减目标血量再使用特制诱饵！
			set_sys_msg_mode 3 0 0
			msg_info
		
		endif
	  else
			add_sys_msg_start
			add_sys_msg 此物品必须在鸣沙陷地使用！
			set_sys_msg_mode 1 0 0
			msg_info
	  endif

endproc
