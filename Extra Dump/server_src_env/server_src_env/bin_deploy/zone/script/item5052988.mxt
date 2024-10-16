;三级宝石包 5052988
proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 1
		DEC_ITEM_NUM_INPAK 5052988 1 10
		if 10 = 0
			rand 40 100
			if 40 < 10
				;中了攻击宝石
				GET_ID_PLAYER_INFO 0 7 30
				;判断职业
				if 30 = 1 or 30 = 3 or 30 = 17 or 30 = 19 or 30 = 33 or 30 = 35
					;给物攻
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5210002 1 1 0
				else	
					;给魔攻
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5210102 1 1 0
				endif
			endif
			
			if 40 >= 10 and 40 < 22
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211002 1 1 0
			endif
			
			if 40 >= 22 and 40 < 34
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211102 1 1 0
			endif
			
			if 40 >= 34 and 40 < 46
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211202 1 1 0
			endif
			
			if 40 >= 46 and 40 < 56
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212002 1 1 0
			endif
			
			if 40 >= 56 and 40 < 68
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212402 1 1 0
			endif
			
			if 40 >= 68 and 40 < 79
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213302 1 1 0
			endif
			
			if 40 >= 79 and 40 < 89
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213402 1 1 0
			endif
			
			if 40 >= 89 and 40 < 99
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213202 1 1 0
			endif
			
			if 40 >= 99
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5219402 1 1 0
			endif
			
		endif	
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  