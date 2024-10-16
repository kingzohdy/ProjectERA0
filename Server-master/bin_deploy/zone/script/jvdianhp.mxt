
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b战争！！！战争！！！！
	add_talk #b#b什么最重要？将军？小兵？后勤？ 不不不，最重要的是医疗，有了我的妙手回春，才能保证士兵们可以迅速投入战场，发挥最大的能力！
	
		
		OPTION 150 #神圣的祝福（50银币）
	talk
endproc



proc 150
	strvar HP MaxHP MP MaxMP
	GET_ID_PLAYER_INFO 0 2 HP
	GET_ID_PLAYER_INFO 0 9 MaxHP
	GET_ID_PLAYER_INFO 0 11 MP
	GET_ID_PLAYER_INFO 0 10 MaxMP
	if @HP < @MaxHP or @MP < @MaxMP 
		strvar ret
		DEC_MONEY_TEST 0 50 20 ret
		if ret = -1
			add_sys_msg_start
			add_sys_msg 您背包中的银币不足！
			set_sys_msg_mode 4 0 0
			msg_info	
			
			BUY_HETONG 1
				;银币不足会打开快捷购买方式
			
		else
			strvar re
			DEC_MONEY 0 50 20 re
			if re = 0
				ADD_PLAYER_HP 0 999999 0
				ADD_PLAYER_MP 0 999999 0
			endif
		endif
	else
		add_sys_msg_start
		add_sys_msg 你的状态良好，不需要我的援助！
		set_sys_msg_mode 4 0 0
		msg_info
	
	endif
endproc


 