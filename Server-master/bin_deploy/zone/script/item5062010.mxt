;技能宝盒A

proc 1
	strvar Rac Ret Num
	GET_PACK_SPACE 0 Num
	if Num = 0
		add_sys_msg_start
		add_sys_msg 背包空余格子不足，无法开启！
		set_sys_msg_mode 1 0 0
		msg_info
		return
	else
		dec_item_num_inpak 5062010 1 10		
		if 10 = 0
			strvar Lv
			GET_PLAYER_BASE_ATT 1 1 Lv 1

			if Lv <= 59
				ADD_BIND_ITEM_NUM_INPAK 5062000 1 1 0
			endif
			if Lv >= 60 and Lv <= 120
				ADD_BIND_ITEM_NUM_INPAK 5062003 1 1 0
			endif
			if Lv > 120
			
				ADD_BIND_ITEM_NUM_INPAK 5062009 1 1 0
			endif
		endif
		
	endif
endproc 