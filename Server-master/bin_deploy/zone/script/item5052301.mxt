;金福袋
proc 1
	strvar Num
	GET_PACK_SPACE 0 Num

	if Num < 1 
		add_sys_msg_start
		add_sys_msg 背包空间不足！
		set_sys_msg_mode 4 0 0
		msg_info
		return
	else
		GET_ITEM_NUM_INPAK 5060034 1 60
		if 60 = 0
			add_sys_msg_start
			add_sys_msg 你没有金福袋钥匙，不能打开金福袋！
			set_sys_msg_mode 4 0 0
			msg_info
			return
		endif
		
		DEC_ITEM_NUM_INPAK 5052301 1 100
		DEC_ITEM_NUM_INPAK 5060034 1 101
		if 100 = -1 and 101 = -1
			return
		endif

		add_sys_msg_start
		strvar rad
		rand rad 10000
		if rad < 3
			ADD_ITEM_NUM_INPAK 5611417 1 1 20

			if 20 = -2
				ADD_BIND_ITEM_NUM_INPAK 5060033 1 1 20
					
				add_sys_msg 你获得了1个春节硬币
			else
				add_sys_msg_start
				add_sys_msg 幸运的$_player$在金福袋里开出了“变异年兽·马”，真是鸿运当头！
				set_sys_msg_mode 6 0 0
				svr_msg_info
			endif
		endif
		if rad >= 3 and rad < 3153
			ADD_BIND_ITEM_NUM_INPAK 5060033 1 1 20
			add_sys_msg 你获得了1个春节硬币
		endif
		if rad >= 3153 and rad < 3403
			ADD_ITEM_NUM_INPAK 5012113 3 1 20
			add_sys_msg 你获得了3个情人节魔法烟花

		endif
		if rad >= 3403 and rad < 3603
			ADD_ITEM_NUM_INPAK 3550014 1 1 20
			add_sys_msg 你获得了1个春节帽子（7天）

		endif
		if rad >= 3603 and rad < 3803
			ADD_ITEM_NUM_INPAK 3850014 1 1 20
			add_sys_msg 你获得了1个春节手套（7天）

		endif
		if rad >= 3803 and rad < 4003
			ADD_ITEM_NUM_INPAK 3050014 1 1 20
			add_sys_msg 你获得了1个春节上衣（7天）

		endif
		if rad >= 4003 and rad < 4203
			ADD_ITEM_NUM_INPAK 3450014 1 1 20
			add_sys_msg 你获得了1个春节裤子（7天）

		endif
		if rad >= 4203 and rad < 4403
			ADD_ITEM_NUM_INPAK 3750014 1 1 20
			add_sys_msg 你获得了1个春节鞋子（7天）

		endif
		
		
		if rad >= 4403 and rad < 5803
			ADD_ITEM_NUM_INPAK 5219600 2 1 20
			add_sys_msg 你获得了2个遗忘果实

		endif
		if rad >= 5803 and rad < 6803
			ADD_ITEM_NUM_INPAK 5233000 2 1 20
			add_sys_msg 你获得了2个幻化果实

		endif
		if rad >= 6803 and rad < 9800
			ADD_ITEM_NUM_INPAK 5050000 2 1 20
			add_sys_msg 你获得了2个小喇叭

		endif
		if rad >= 9800 and rad < 9900
			ADD_ITEM_NUM_INPAK 5043237 1 1 20
			add_sys_msg 你获得了1个称号：★福星高照★

		endif
		if rad >= 9900
			ADD_ITEM_NUM_INPAK 5043232 1 1 20
			add_sys_msg 你获得了1个称号：红包拿来\(^o^)/

		endif
		
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	
endproc





