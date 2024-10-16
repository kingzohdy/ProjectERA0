;伊娃

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b呲…呲…哔呲…
	add_talk #b#b#c186506填炮#n…#c186506弹#n，#c186506钉装甲#n…呲哔…#c186506装#n…呲…#c186506齿轮#n。
	add_talk #b#b呲呲…
	add_talk #b#b#c186506武装战#n…#c186506车#n…呲…，#c186506火力#n…#c186506全开#n…哔……
	add_talk #b#b
	add_talk  #c005ebe武装战车可在短时间内大幅提升战车的爆发火力#n
	
	option 100 #我想看看你的武装组件？
	
	talk

endproc

proc 100
	dialog 1
	add_talk ☆超级炮弹☆
	add_talk #b#b短时间内大幅增加战车#c18650650%攻击力#n
	add_talk #b#b
	add_talk ☆装甲强化☆
	add_talk #b#b短时间内大幅减少战车受到的#c18650633%伤害#n
	add_talk #b#b
	add_talk ☆动力加速☆
	add_talk #b#b短时间内额外增加战车#c18650610%移动速度#n
	
	option 200 武装“超级炮弹”（1000绑银）
	option 300 武装“装甲强化”（1000绑银）
	option 400 武装“动力加速”（1000绑银）
	
	talk

endproc

proc 200
	STRVAR memID num ret ret2 ret3 ret4 suo
	GET_ROLE_RIDE_ON_MACHINE_MEM_ID memID 0
	PKG_IS_UNLOCK ret4 suo
	
	if ret4 = 0 and suo = 1
		if memID > 0
			GET_ID_MON_STATUS_INFO @memID 2106 ret3 1 num 0 0
			
			if ret3 = -1
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2106 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2106 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg “超级炮弹”达到了$@10$/5强度，战车攻击力获得提升
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg 身上绑银不够，无法购买“超级炮弹”武装战车
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif ret3 = 0 and num < 4
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2106 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2106 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg “超级炮弹”达到了$@10$/5强度，战车攻击力获得提升
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg 身上绑银不够，无法购买“超级炮弹”武装战车
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg “超级炮弹”已达到5/5强度，无法继续强化
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 未搭载在战车上，无法购买“超级炮弹”
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 300
	STRVAR memID num ret ret2 ret3 ret4 suo
	GET_ROLE_RIDE_ON_MACHINE_MEM_ID memID 0
	PKG_IS_UNLOCK ret4 suo
	
	if ret4 = 0 and suo = 1
		if memID > 0
			GET_ID_MON_STATUS_INFO @memID 2107 ret3 1 num 0 0
			
			if ret3 = -1
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2107 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2107 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg “装甲强化”达到了$@10$/5强度，战车攻击力获得提升
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg 身上绑银不够，无法购买“装甲强化”武装战车
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif ret3 = 0 and num < 4
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2107 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2107 0 1 num 0 0
						var 10 @num
						add 10 1
						
						add_sys_msg_start
						add_sys_msg “装甲强化”达到了$@10$/5强度，战车防御力获得提升
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg 身上绑银不够，无法购买“装甲强化”武装战车
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg “装甲强化”已达到5/5强度，无法继续强化
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 未搭载在战车上，无法购买“装甲强化”
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc

proc 400
	STRVAR memID num ret ret2 ret3 ret4 suo
	GET_ROLE_RIDE_ON_MACHINE_MEM_ID memID 0
	PKG_IS_UNLOCK ret4 suo
	
	if ret4 = 0 and suo = 1
		if memID > 0
			GET_ID_MON_STATUS_INFO @memID 2108 ret3 1 num 0 0
			
			if ret3 = -1
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2108 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2108 0 1 num 0 0
						var 10 @num
						add 10 1
					
						add_sys_msg_start
						add_sys_msg “动力加速”达到了$@10$/5强度，战车攻击力获得提升
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg 身上绑银不够，无法购买“超级炮弹”武装战车
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif ret3 = 0 and num < 4
				DEC_MONEY_TEST 0 1000 21 ret
				if ret = 0
					DEC_MONEY 0 1000 21 0
					TARGET_ADD_STATUS 3 @memID 2108 1 60 ret2
					if ret2 = 0
						GET_ID_MON_STATUS_INFO @memID 2108 0 1 num 0 0
						var 10 @num
						add 10 1
						
						add_sys_msg_start
						add_sys_msg “动力加速”达到了$@10$/5强度，战车移动速度获得提升
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg 身上绑银不够，无法购买“动力加速”武装战车
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg “动力加速”已达到5/5强度，无法继续强化
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg 未搭载在战车上，无法购买“动力加速”
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc
