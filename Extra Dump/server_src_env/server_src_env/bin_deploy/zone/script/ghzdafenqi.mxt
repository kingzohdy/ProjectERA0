

proc 1
	DIALOG 1

	add_talk $_npc$:
	add_talk #b#b我就是伟大的画家！科学家！发明家！哲学家！音乐家！医学家！生物学家！地理学家！雕塑家！及万千宠爱于一身的......雕塑大师：大分.....奇先生！
	add_talk #b#b想出名么？想立万么？找我吧，我可以把你伟岸的身姿永远的留在这个伟大的城市，让万人仰慕，亦或者。。。万人唾弃。总之可以把你留在这！不过想让我给你雕塑这得看你的身价如何，要不要尝试下把边上这个人换成你的形象？
	;strvar Ret
	;IS_WORLD_CITY_OWNER Ret
	;if Ret = 1
		option 199 #塑造雕像
	;endif
	option 201 #关闭
	
;	option 400 #测试刷怪
	
	talk
endproc 
proc 199
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 1
		choose 2 200 201 雕塑需要支付500000银币，是否继续？
	endif
endproc
proc 200
	PKG_IS_UNLOCK 10 20
	if 10 = 0 and 20 = 0
		return
	endif
	DEC_MONEY_TEST 0 500000 20 10
	if 10 = -1
		add_sys_msg_start
		add_sys_msg 银币不足，雕塑失败
		set_sys_msg_mode 3 0 0
		msg_info
	else
		strvar Ret
		SET_WORLD_CITY_EFFIGY Ret 1000 26420 29945
		if Ret = 0
		
			DEC_MONEY 0 500000 20 1
			add_sys_msg_start
			add_sys_msg 伟大的时间城城主，您的雕像已经雕塑完成。
			set_sys_msg_mode 4 0 0
			msg_info
			
			strvar mon day
			GET_NOW_TIME mon day 0 0 0 0 0
			
			add_sys_msg_start
			add_sys_msg 伟大的[时间城城主]$_player$的个人雕像已于$@mon$月$@day$日正式竖立在时间城中心位置
			set_sys_msg_mode 1 0 0
			svr_msg_info
			
		else
		
			add_sys_msg_start
			add_sys_msg 想让我给你雕塑？等你成为时间城的城主之后再来找我吧。
			set_sys_msg_mode 4 0 0
			msg_info
		endif
	endif
endproc 
proc 201

endproc
proc 400
	strvar num x y ba monid
	set_call_var monid 283001
	set_call_var num 10
	set_call_var x 26500
	set_call_var y 29600
	set_call_var ba 100
	call 401
	set_call_var num 15
	call 401
endproc 
proc 401
	NEW_MON @monid @num @x @y @ba 0 0 0 0
endproc 





