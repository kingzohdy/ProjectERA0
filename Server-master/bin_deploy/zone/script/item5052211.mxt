proc 1
	strvar Num Num1 Num2 ret bind
	GET_ITEM_NUM_INPAK 5052211 0 Num
	GET_ITEM_NUM_INPAK 5233511 0 Num1
	GET_ITEM_NUM_INPAK 5233512 0 Num2
	GET_PACK_SPACE 0 ret
	USE_ITEM_IS_BIND 0 bind
	
	if ret >= 1
		if Num >= 1
			if Num1 >= 500
				if Num2 >= 100
					strvar ret1 ret2 ret3
					dec_item_num_inpak 5052211 1 ret1
					dec_item_num_inpak 5233511 500 ret2
					dec_item_num_inpak 5233512 100 ret3
					if ret1 = 0 and ret2 = 0 and ret3 = 0
						if bind = 1
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233503 1 1 0
						else
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5233503 1 1 0
						endif
					endif
				else
					add_sys_msg_start
					add_sys_msg 无法提炼进阶果实，缺少材料：进阶凝露，你可以从失落之魂、失落之灵、马基尔及时光齿轮中得到它
					set_sys_msg_mode 4 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 无法提炼进阶果实，缺少材料：奇异精粹，可从失落之域的任意怪物身上得到它
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info	
	endif
endproc
