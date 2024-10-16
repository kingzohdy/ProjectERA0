;5301003


proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 3400
	
;		 GET_MON_INFO 10 11 12 13 14 15 16 17 
;			effect 0 0 playEffct($@16$,$@17$,"effect\\entity\\9548_16.ent")
		dec_item_num_inpak 5300864 1 0
;			add_item_num_inpak 5301003 1 1 0
;			strvar x y
;			GET_TARGET_INFO 3 3 x
;			GET_TARGET_INFO 3 4 Y
;			del_mon 1
;			NEW_LIFE_MON 293229 1 @x @y 0 0 0 0 20 0 0
			task_fini 1663 1
			
	  else
			add_sys_msg_start
			add_sys_msg  此物品只能在十三号矿洞使用！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc
