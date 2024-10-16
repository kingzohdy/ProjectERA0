;圣诞红礼箱子

proc 1
	strvar Rac Ret Num
	GET_PACK_SPACE 0 Num
	
	if Num < 1
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
		return
		
	else
		strvar Dret Rad Rad2
		dec_item_num_inpak 5012102 1 Dret
		if Dret = 0
			var 20 0
			while 20 < 1
			add 20 1
				rand Rad 1000
				if Rad = 0
					;圣诞时装
					strvar Ret1
					ADD_ITEM_NUM_INPAK 5600000 1 1 Ret1
					
					if Ret1 = 0
						add_sys_msg_start
						add_sys_msg 恭喜“$_player$”开启【圣诞红礼箱】获得“驯鹿拉车”双人坐骑！！
						set_sys_msg_mode 18 0 0
						svr_msg_info
					endif
					if Ret1 = -2
						;ADD_ITEM_NUM_INPAK 3050000 1 1 0
						var 20 0
					endif
				elseif Rad >= 1 and Rad < 200
					ADD_ITEM_NUM_INPAK 5012104 1 1 0
				elseif Rad >= 200 and Rad < 300
					ADD_ITEM_NUM_INPAK 5012108 1 1 0
				elseif Rad >= 300 and Rad < 350
					ADD_ITEM_NUM_INPAK 5012100 3 1 0
				elseif Rad >= 350 and Rad < 400
					ADD_ITEM_NUM_INPAK 5012101 1 1 0
				else
					ADD_ITEM_NUM_INPAK 5012105 3 1 0
				endif
			endwhile
		endif
	endif
endproc 