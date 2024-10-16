;甘露
;甘露+劳动硕果=劳模奖品礼盒（右键使用物品，消耗2个物品得到1个物品）

proc 1
	GET_PACK_SPACE 0 20
	if 20 < 1
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_ITEM_NUM_INPAK 5052306 0 10
	if 10 > 0
		DEC_ITEM_NUM_INPAK 5052305 1 11
		DEC_ITEM_NUM_INPAK 5052306 1 12
		if 11 = 0 and 12 = 0
			ADD_BIND_ITEM_NUM_INPAK 5052307 1 1 13
			if 13 = 0
				add_sys_msg_start
				add_sys_msg 使用甘露和劳动硕果获得一个劳模奖品礼盒
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		endif
	else
		call 100
	endif
endproc

proc 100
	add_sys_msg_start
	add_sys_msg 您的背包中缺少“劳动硕果”，此物品可在“义卖商店”购买。
	set_sys_msg_mode 4 0 0
	msg_info
endproc 