; 消失的剑刃

proc 1

	strvar num1 num2 num3
	GET_ITEM_NUM_INPAK 5062201 0 num1
	GET_ITEM_NUM_INPAK 5062202 0 num2
	GET_ITEM_NUM_INPAK 5062203 0 num3
	
	if num1 > 0
		if num2 > 0
			if num3 > 0
				strvar ret1 ret2 ret3
				DEC_ITEM_NUM_INPAK 5062201 1 ret1
				DEC_ITEM_NUM_INPAK 5062202 1 ret2
				DEC_ITEM_NUM_INPAK 5062203 1 ret3
				if ret1 = 0
					if ret2 = 0
						if ret3 = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5062200 1 1 0
							
							add_sys_msg_start
							add_sys_msg 你使用消失的剑刃剑柄和剑鞘，合成了“消失的宝剑”
							set_sys_msg_mode 3 0 0
							msg_info
						else
							add_sys_msg_start
							add_sys_msg “消失的剑鞘”被损坏了，合成“消失的宝剑”失败
							set_sys_msg_mode 3 0 0
							msg_info
						endif
					else
						add_sys_msg_start
						add_sys_msg “消失的剑柄”被损坏了，合成“消失的宝剑”失败
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg “消失的剑刃”被损坏了，合成“消失的宝剑”失败
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg 背包里找不到“消失的剑鞘”，无法合成“消失的宝剑”
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包里找不到“消失的剑柄”，无法合成“消失的宝剑”
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 背包里找不到“消失的剑刃”，无法合成“消失的宝剑”
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
