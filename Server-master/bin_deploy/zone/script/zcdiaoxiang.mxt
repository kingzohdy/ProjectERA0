


proc 1
	
	DIALOG 1	
	add_talk $_npc$:
	strvar rety
	ENTER_WORLD_CITY_CHECK rety
	if rety = -1

	add_talk #b#b咔....咔.....嘣！！！
	add_talk #b#b一座未完成雕像作品，据说当雕像完成之时，大家可以来这里瞻仰这个世界最最最最霸气侧漏的统治者

	
	
	else
	
	add_talk #b#b这是集权利、荣耀、金钱于一身，走在了世界最巅峰位置的象征！
	add_talk #b#b地球村的子民们，来这里瞻仰吧，城主的......雕像就在这里。
	add_talk #b#b
	add_talk #b#b每天可对雕像#c186506朝拜#n或者#c186506扔鸡蛋#n1次，可获得大量经验。
		option 300 朝拜一下
		option 350 扔臭鸡蛋
		
		strvar ret
		is_world_city_owner ret
		if ret = 1
			strvar pos
			player_clan_pos_info 1 0 0 0 Pos
			if Pos = 8
			;只有城主能看到
				option 400 领取雕像收益			
			endif
		endif
	endif
	talk
endproc  
proc 301

endproc
proc 300
	strvar ret
	PLAYER_WORSHIP_EFFIGY_TEST 1 ret
	if ret = 0
		choose 2 310 301 确认消耗200银币朝拜一下？
	endif
;	确认消耗XXX银币/绑银朝拜一下？
endproc 
proc  310
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	DEC_MONEY_TEST 0 200 20 10
	if 10 = -1
		add_sys_msg_start
		add_sys_msg 银币不足，无法朝拜
		set_sys_msg_mode 3 0 0
		msg_info
	else
		PLAYER_WORSHIP_EFFIGY 1 20
		if 20 = 0
			;奖励经验
			DEC_MONEY 0 200 20 1		
		    ;ADD_EXP #playerid #经验值 #返回标志
			
		endif
	endif
endproc 

proc 350
	strvar ret
	PLAYER_WORSHIP_EFFIGY_TEST 0 ret
	if ret = 0
		choose 2 351 301 确认消耗200银币扔一枚臭鸡蛋？
	endif
endproc
proc 351



			PKG_IS_UNLOCK 10 20
			if 10 = 0 and 20 = 0
				return
			endif
			DEC_MONEY_TEST 0 200 20 10
			if 10 = -1
				add_sys_msg_start
				add_sys_msg 银币不足，无法仍臭鸡蛋
				set_sys_msg_mode 3 0 0
				msg_info
			else
				PLAYER_WORSHIP_EFFIGY 0 20
				if 20 = 0
					;奖励经验
					DEC_MONEY 0 200 20 1		
				    ;ADD_EXP #playerid #经验值 #返回标志
					
				endif
			endif

	
endproc 

proc 400
	dialog 1
	add_talk $_npc$:
	add_talk #b#b尊敬的城主大人，您每天可在此处领取前一天大家朝拜或扔鸡蛋的总收益分成。
	add_talk #b#b谨记：只有拥有属于您自己的雕像，才可以领取收益分成。
	
	CITY_EFFIGY_INFO 10 20
	
;	add_talk #b#b鲜花×$@10$
;	add_talk #b#b臭鸡蛋×$@20$

	
	
		option 410 #领取雕像收益
		option 411 #晚点再来
		
	talk
endproc 
proc 411
endproc
proc 410

;	鲜花10 鸡蛋20
	CITY_EFFIGY_INFO 10 20
	add 10 @20
	mul 10 100
	
	choose 2 415 411 昨日雕像的总收益为：$@10$银币，是否确认领取？
	
endproc


proc 415

	strvar ret
	is_world_city_owner ret
	if ret = 1
		strvar pos
		player_clan_pos_info 1 0 0 0 Pos
		if Pos = 8


			PLAYER_EFFIGY_AWARD 1
	
		else
			add_sys_msg_start
			add_sys_msg 权限不足，只有时间城城主才可进行此操作
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg 权限不足，只有时间城城主才可进行此操作
		set_sys_msg_mode 3 0 0
		msg_info
	
	endif
endproc 



