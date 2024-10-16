;银质宝箱
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num
	
	if Num < 2
		add_sys_msg_start
		add_sys_msg 开启宝箱会随机获得物品，请预留2个以上背包空格再开启
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
	
		DEC_ITEM_NUM_INPAK 5052809 1 100
		if 100 = -1
			return
		endif
		
		strvar A B Rad
		var A 0
		var B 2
		while @A < @B
			add A 1
			rand Rad 7
			switch Rad
				case 0
					ADD_BIND_ITEM_NUM_INPAK 5200020 3 0 0
				endcase
				case 1 
					ADD_BIND_ITEM_NUM_INPAK 5233500 1 0 0
				endcase
				case 2
					ADD_BIND_ITEM_NUM_INPAK 5234000 1 0 0
				endcase
				case 3 
					ADD_BIND_ITEM_NUM_INPAK 5233000 1 0 0
				endcase
				case 4 
					ADD_BIND_ITEM_NUM_INPAK 5050000 3 0 0
				endcase
				case 5 
					ADD_BIND_ITEM_NUM_INPAK 5050510 3 0 0
				endcase
				case 6 
					ADD_BIND_ITEM_NUM_INPAK 5062003 1 0 0
				endcase
			
			endswitch
		endwhile
		
	endif

endproc
