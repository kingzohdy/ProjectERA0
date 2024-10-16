;炽铜碎片

proc 1

		strvar ret num
		GET_ITEM_NUM_INPAK 5060101 1 num
		if num >= 30
			
			GET_PACK_SPACE 0 ret
			if ret >= 1
				DEC_ITEM_NUM_INPAK 5060101 30 10
				if 10 = 0
					ADD_BIND_ITEM_NUM_INPAK 5060002 1 1 0
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包空间不足！
				set_sys_msg_mode 4 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 合成炽铜合金需要30个炽铜碎片！
			set_sys_msg_mode 4 0 0
			msg_info
		
		endif
endproc 
