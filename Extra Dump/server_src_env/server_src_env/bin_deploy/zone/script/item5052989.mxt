;四级宝石包 5052989
proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0
		DEC_ITEM_NUM_INPAK 5052989 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 7 30
			if 30 = 1 or 30 = 3 or 30 = 17 or 30 = 19 or 30 = 33 or 30 = 35
			;物攻职业
				rand 40 100
				if 40 < 10
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5210003 1 1 0
				endif
				
				if 40 >= 10 and 40 < 22
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211003 1 1 0
				endif
				
				if 40 >= 22 and 40 < 34
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211103 1 1 0
				endif
				
				if 40 >= 34 and 40 < 46
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211203 1 1 0
				endif
				
				if 40 >= 46 and 40 < 56
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212003 1 1 0
				endif
				
				if 40 >= 56 and 40 < 68
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212403 1 1 0
				endif
				
				if 40 >= 68 and 40 < 79
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213303 1 1 0
				endif
				
				if 40 >= 79 and 40 < 89
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213403 1 1 0
				endif
				
				if 40 >= 89 and 40 < 99
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213203 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”开启【四级宝石袋】获得“四级多彩宝石”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
					
				endif
				if 40 >= 99
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5219403 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”开启【四级宝石袋】获得“四级宝石升级符”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
					
				endif
			endif
			
			if 30 = 2 or 30 = 4 or 30 = 18 or 30 = 20 or 30 = 34 or 30 = 36
			;魔攻职业
				rand 50 100
				if 50 < 10
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5210103 1 1 0
				endif
				
				if 50 >= 10 and 50 < 22
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211003 1 1 0
				endif
				
				if 50 >= 22 and 50 < 34
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211103 1 1 0
				endif
				
				if 50 >= 34 and 50 < 46
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5211203 1 1 0
				endif
				
				if 50 >= 46 and 50 < 56
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212003 1 1 0
				endif
				
				if 50 >= 56 and 50 < 68
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5212403 1 1 0
				endif
				
				if 50 >= 68 and 50 < 79
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213303 1 1 0
				endif
				
				if 50 >= 79 and 50 < 89
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213403 1 1 0
				endif
				
				if 50 >= 89 and 50 < 99
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5213203 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”开启【四级宝石袋】获得“四级多彩宝石”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
					
				endif
				if 50 >= 99
					ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5219403 1 1 0
					
					add_sys_msg_start
					add_sys_msg 恭喜玩家“$_player$”开启【四级宝石袋】获得“四级宝石升级符”！
					set_sys_msg_mode 18 0 0
					svr_msg_info
					
				endif
			endif
		endif	
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  