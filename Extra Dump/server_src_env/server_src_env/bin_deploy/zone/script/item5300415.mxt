;鞭子

proc 1

	  get_player_mapinfo 0 1 2 3 4

	  if 2 = 2100 or 2 = 2101 or 2 = 2102 or 2 = 2103 or 2 = 2105 or 2 = 2106 or 2 = 2107 or 2 = 2108

				;	get_player_id 0 20
				;	effect_id 0 @20 playMontion(%d,"9565_16")
;			dec_item_num_inpak 5300415 1 0
		strvar Mid
		GET_MAP_MON_ID 293087 Mid 1
		
		get_task_var 1318 0 8 1 10
		add 10 1
		set_task_var 1318 0 8 @10 1
		if 10 = 1
			mon_speech @Mid 2 0 1 我是打死也不会透露任何讯息给你的！
		endif
		if 10 = 2
		
			mon_speech @Mid 2 0 1 让鞭挞来的更猛烈些吧！
		endif
		if 10 = 3
		
			mon_speech @Mid 2 0 1 富贵不能淫！贫贱不能移！威武不能屈！
		endif
		if 10 = 4
		
			mon_speech @Mid 2 0 1 你想知道纳巴在哪是吗？到地狱找他去吧！
		endif
		if 10 = 5
		
			mon_speech @Mid 2 0 1 就算把我打死，你也见不到纳巴！
			task_fini 1318 0
		endif
		if 10 > 5
			strvar Rad
			rand Rad 2
			if Rad = 0
				mon_speech @Mid 2 0 1 手下留情啊！我已经把知道的都告诉你了！
			
			endif
			if Rad = 1
				mon_speech @Mid 2 0 1 放过我吧！我知道的都说了！求你别再打了！
				
			endif
			
		var 10 4 
		set_task_var 1318 0 8 @10 1
		endif
				
				
;		else
;			add_sys_msg_start
;			add_sys_msg 好像没效果，先听听他们说完再使用试试！
;			set_sys_msg_mode 3 0 0
;			msg_info
;			
;		endif
	  else
			add_sys_msg_start
			add_sys_msg 目标不在本地图中！
			set_sys_msg_mode 3 0 0
			msg_info
	  endif

endproc 

