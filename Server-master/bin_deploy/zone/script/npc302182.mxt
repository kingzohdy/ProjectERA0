;奥斯卡

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bHey，#Name#你身上有万圣糖果礼券吗，我这里有好东西可以和你换哦，你有多少张礼券，我可以给你换多少个礼品。
	add_talk #b#b只有在万圣节才有这个机会，好好把握吧。
	add_talk #b#b#c186506万圣糖果礼券可通过时间城-珈纳做任务获得，每天可做10次。#n

	
		OPTION 200 一张礼券兑换一份礼品

	talk
endproc

proc 200
	STRVAR ret num ret1 kg
	
	GET_ITEM_NUM_INPAK 5012139 ret num
	GET_PACK_SPACE 0 kg
	
	if kg > 0
		if ret = 0 and num > 0
			DEC_ITEM_NUM_INPAK 5012139 1 ret1
			if ret1 = 0
				rand 10 100
					if 10 < 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3050041 1 1 0
						add_sys_msg_start
						add_sys_msg 您用万圣糖果礼券成功换取了1件【伯爵长袍】时装
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 3 and 10 < 6
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3450041 1 1 0
						add_sys_msg_start
						add_sys_msg 您用万圣糖果礼券成功换取了1件【伯爵裤子】时装
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 6 and 10 < 9
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3750041 1 1 0
						add_sys_msg_start
						add_sys_msg 您用万圣糖果礼券成功换取了1件【伯爵长靴】时装
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 9 and 10 < 12
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3850041 1 1 0
						add_sys_msg_start
						add_sys_msg 您用万圣糖果礼券成功换取了1件【伯爵护手】时装
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 12 and 10 < 15
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3650002 1 1 0
						add_sys_msg_start
						add_sys_msg 您用万圣糖果礼券成功换取了1件【天然呆】背饰。
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 15
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 0
						add_sys_msg_start
						add_sys_msg 您用万圣糖果礼券成功换取了1颗【万圣节糖果】道具。
						set_sys_msg_mode 3 0 0
						msg_info
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg 你包里1个【万圣糖果礼券】都没有哦，我可不做亏本生意。
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 请空出1个背包格子再来找我哦。
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
