;宅人网公测特权金卡
proc 1
	strvar zhi ret xingbie
	GET_PACK_SPACE 0 ret
	get_role_var 76 8 0 zhi
	
	if zhi < 1
		GET_PACK_SPACE 0 ret
		if ret >= 8
			DEC_ITEM_NUM_INPAK 5052979 1 10
			if 10 = 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5006106 2 1 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052912 2 1 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5050000 3 1 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052892 1 1 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251001 20 1 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5041003 1 1 0
				; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3650003 1 1 0
				
				; GET_PLAYER_BASE_ATT 0 xingbie 0 0
				; if xingbie = 1
					; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5600231 1 1 0
				; endif
				; if xingbie = 2
					; ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5600230 1 1 0
				; endif
				
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3650003 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052817 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5052859 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5251001 5 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5050000 1 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5006106 2 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5051007 10 1 0
				ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5600002 1 1 0

				var zhi 1
				set_role_var 76 8 @zhi 1
			endif
		else
			add_sys_msg_start
			add_sys_msg 背包空间不足，请预留8个空格再开启道具！
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 你已开启过宅人网公测特权金卡，无法再开启
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc 