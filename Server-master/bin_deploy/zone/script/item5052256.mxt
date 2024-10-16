

;5052256	一件崭新的胸甲
;12级蓝色胸甲

proc 1

	strvar ret
	GET_PACK_SPACE 0 ret
	if ret >= 1
		DEC_ITEM_NUM_INPAK 5052256 1 10
		if 10 = 0
			GET_ID_PLAYER_INFO 0 7 30
			if 30 = 1
			;战士
				ADD_BIND_ITEM_NUM_INPAK 2014002 1 1 0
			endif
			if 30 = 2
			;法师
				ADD_BIND_ITEM_NUM_INPAK 2024002 1 1 0
			endif
			if 30 = 3
			;枪手
				ADD_BIND_ITEM_NUM_INPAK 2034002 1 1 0		
			endif
			if 30 = 4
			;祭祀
				ADD_BIND_ITEM_NUM_INPAK 2044002 1 1 0		
			endif
			
			if 30 = 17
			;守护
				ADD_BIND_ITEM_NUM_INPAK 2014002 1 1 0		
			endif			
			if 30 = 33
			;征服
				ADD_BIND_ITEM_NUM_INPAK 2014002 1 1 0		
			endif			
			if 30 = 18
			;聚能
				ADD_BIND_ITEM_NUM_INPAK 2024002 1 1 0		
			endif			
			if 30 = 34
			;炼魂
				ADD_BIND_ITEM_NUM_INPAK 2024002 1 1 0		
			endif			
			if 30 = 19
			;猎杀
				ADD_BIND_ITEM_NUM_INPAK 2034002 1 1 0		
			endif	
            if 30 = 35
			;毁灭
				ADD_BIND_ITEM_NUM_INPAK 2034002 1 1 0		
			endif	
			if 30 = 20
			;圣佑
				ADD_BIND_ITEM_NUM_INPAK 2044002 1 1 0		
			endif
			if 30 = 36
			;驭雷
				ADD_BIND_ITEM_NUM_INPAK 2044002 1 1 0		
			endif
	    endif

	
	else
		add_sys_msg_start
		add_sys_msg 背包空间不足，请预留1个空格再开启道具！
		set_sys_msg_mode 4 0 0
		msg_info
	endif

endproc  