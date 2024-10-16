;净化的元素核心  选中使用

proc 1

	
	  get_player_mapinfo 0 1 2 3 4
	  get_point_dist @3 @4 3300 10200 10
	  if 2 = 2300
		if 10 <= 500
			dec_item_num_inpak 5300376 1 0
;			ADD_ITEM_NUM_INPAK 5300289 1 1 0
			strvar PlyMid		

			get_task_var 1532 4 8 1 20
			var 20 1
			set_task_var 1532 4 8 @20 1
			GET_PLAYER_ID 1 PlyMid 
			
;			GET_MON_INFO 0 30 31 32 33 34 35 36		
			del_mon 1		
			strvar MonMid Num
			CALL_MON 1 226010 500 60 3300 10200 MonMid
			
			add_sys_msg_start
			add_sys_msg 阿拜多斯派他的幻影来阻止净化，赶快消灭他！
			set_sys_msg_mode 3 0 0
			msg_info
			
			SET_MON_CURR @MonMid 1
			set_mon_var 0 32 @PlyMid 1
		else
			add_sys_msg_start
			add_sys_msg 需要在指定区域才可使用！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
		
	
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 4 0 0
			msg_info
	  endif

endproc
