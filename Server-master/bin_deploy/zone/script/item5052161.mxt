;时间城礼包

proc 1
	strvar num
	GET_PACK_SPACE 0 num
	if num > 0
		dec_item_num_inpak 5052161 1 10
		GET_PLAYER_BASE_ATT 1 1 40 1
		if 10 = 0 
		;	add_item_num_inpak 5210000 1 2 Ret
			rand 30 100
			if 30 < 20
				if @40 < 60
					add_item_num_inpak 5060200 20 1 20
				endif
				if @40 >= 60 and @40 < 70
					add_item_num_inpak 5060201 20 1 20
				endif
				if @40 >= 70 and @40 < 80
					add_item_num_inpak 5060202 20 1 20
				endif
				if @40 >= 80
					add_item_num_inpak 5060203 20 1 20
				endif
			endif
			if 30 >= 20 and 30 < 40
				if @40 < 50
					ADD_BIND_ITEM_NUM_INPAK 5103021 1 1 20
				endif
				if @40 >= 50 and @40 < 70
					ADD_BIND_ITEM_NUM_INPAK 5103022 1 1 20
				endif
				if @40 >= 70 and @40 < 80
					ADD_BIND_ITEM_NUM_INPAK 5103023 1 1 20
				endif
				if @40 >= 80
					ADD_BIND_ITEM_NUM_INPAK 5103024 1 1 20
				endif
			endif
			if 30 >= 40 and 30 < 45
				ADD_BIND_ITEM_NUM_INPAK 5052310 1 1 20
			endif
			if 30 >= 45 and 30 < 50 
				ADD_BIND_ITEM_NUM_INPAK 5051012 10 1 20
			endif
			if 30 >= 50 and 30 < 100
				ADD_BIND_ITEM_NUM_INPAK 5060003 1 1 20
			endif
			; if 30 >= 75 and 30 < 100
				; ADD_BIND_ITEM_NUM_INPAK 5060009 1 1 20
			; endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc


