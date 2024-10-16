
;小刀
proc 1
	strvar Ret
	task_stat 2104 1 Ret
	if Ret = 2
		strvar Num
		GET_ITEM_NUM_INPAK 5300892 1 Num
		if Num = 0
			add_sys_msg_start
			add_sys_msg 你没有足够的萨能蛮羊肉！
			set_sys_msg_mode 3 0 0
			msg_info
		else
			strvar Reti
			DEC_ITEM_NUM_INPAK 5300892 1 Reti
			if Reti = 0
				ADD_ITEM_NUM_INPAK 5300893 3 1 0
				; ADD_ITEM_NUM_INPAK 5300891 1 1 0
				
				strvar rac
				GET_ID_PLAYER_INFO 0 7 rac
				if rac = 17 or rac = 33
				;战士
					add_sys_msg_start
					add_sys_msg 啊哈……我剁我剁我剁剁剁……
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				if rac = 18 or rac = 34
				;法师
					add_sys_msg_start
					add_sys_msg 我冰环…我冰爆……啊，打…打…打……
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				if rac = 19 or rac = 35
				;枪手
					add_sys_msg_start
					add_sys_msg 公瑾……让我鞭挞你吧！
					set_sys_msg_mode 3 0 0
					msg_info
				endif
				if rac = 20 or rac = 36
				;祭祀
					add_sys_msg_start
					add_sys_msg 你伸手将肉抛向天空，一个华丽的转身，叭叭叭……
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			endif
		endif
	endif
		
endproc  


