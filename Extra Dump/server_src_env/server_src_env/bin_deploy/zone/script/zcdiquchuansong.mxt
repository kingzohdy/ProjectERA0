
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b黑暗议会在大洋彼岸建起巨大的钢铁要塞，得益于丰富的资源，使他们势力日趋壮大。
	add_talk #b#b为了攻破那坚不可摧的要塞，自治联盟组织了有史以来最强大的阵容。
	add_talk #b#b现在钢铁要塞已被我们团团包围，战火一触即发！
	add_talk #b#b
	add_talk #b #c18650680级后可在此传送至钢铁要塞【跨服地图】#n

	
	task_add_option 1076 1 100
	task_add_option 1077 1 101
	task_add_option 3104 1 102

	strvar Ret
	
	task_stat 1077 1 Ret
	if Ret = 2
;		option 300 时间城太大了，能否送我一程？
	endif
	
	get_player_base_att 1 2 3 4
	if 3 >= 20
;		option 202 #传送至永夜入口（消耗200银币）
;		option 200 #传送至阿努比斯神殿（消耗200银币）
;		option 201 #传送至十三号矿洞（消耗200银币）
;		option 204 #传送至黄金矿井（消耗200银币）
;		option 203 #传送越雾通道（消耗200银币）
	;	transmit 1000 28154 6672 1
	
;	else
;		add_sys_msg_start
;		add_sys_msg 实在抱歉,您的等级未达到10级,请您过段时间再来吧...
;		set_sys_msg_mode 3 0 0
;		msg_info

	endif
	
	option 400 火速前往钢铁要塞【跨服地图】
	
	talk
endproc 

proc 400
	strvar lv
    GET_PLAYER_BASE_ATT 0 0 lv 0
	if lv >= 80
		choose 2 401 402 是否确定前往钢铁要塞【跨服地图】？ 
	else
		add_sys_msg_start
		add_sys_msg 钢铁要塞属危险地带，建议80级后再前往
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 401
	strvar baoming ret
	cancel_span_queue_check baoming ret
	if @ret = 0 
		if @baoming = 1
			choose 2 403 402 前往钢铁要塞将放弃当前战场排队\n是否确定前往钢铁要塞【跨服地图】？
		elseif @baoming = 2
			choose 2 403 402 前往钢铁要塞将放弃当前末世荣耀殿堂排队\n是否确定前往钢铁要塞【跨服地图】？
		else
			call 404
		endif
	endif
endproc

proc 402
endproc

proc 403
	cancel_span_queue_check baoming ret
	if @ret = 0
		if @baoming = 1 or @baoming = 2
			cancel_span_queue @baoming 10
			if @10 = 0
				call 404
			endif
		else
			call 404
		endif
	endif
endproc

proc 404	
	get_world_camp_in_span_map 10 11
	if @10 = 0
		strvar PosX PosY
		if @11 = 1
			var PosX 6174
			var PosY 42436
		elseif @11 = 2 
			var PosX 43890
			var PosY 4912
		elseif @11 = 3
			var PosX 6226
			var PosY 6747
		elseif @11 = 4 
			var PosX 44075
			var PosY 43710
		elseif @11 = 5 
			var PosX 25481
			var PosY 48739
		elseif @11 = 6 
			var PosX 25329
			var PosY 2115
		elseif @11 = 7 
			var PosX 4672
			var PosY 25590
		elseif @11 = 8 
			var PosX 47433
			var PosY 25637
		else
			return 
		endif
			
		span_map_transmit 4500 @PosX @PosY 1
	endif
endproc

proc 100
    task_do_option 1076 1
endproc
proc 101
    task_do_option 1077 1
endproc
proc 102
    task_do_option 3104 1
endproc
proc 2
	get_trig_task 1 2

endproc 
proc 300
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b当然可以，您可是个大人物呢。
		add_talk #b#b不过，我是直接将您送到目的地，中途不会停的哟！
		add_talk #b#b您确定要乘坐直达班车吗？
			option 301 #请直接将我送到目的地吧！
			option 302 #不用啦，我自己过去，还能逛逛时间城！
		talk
endproc
proc 301
;	NEW_OWNER_TMP_ITEM 1460069 26600 11000 0 0 0 309 1095 1 3600 1
	AUTO_MOVE 0 1000 20900 17100 1460069 0 3081 1 3600
endproc
proc 302
	return
endproc 
proc 200
;沙漠地宫入口
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2300 20400 5500 1
	else
		add_sys_msg_start
		add_sys_msg 没钱也想来混过去么？！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
proc 202
;永夜入口
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2200 21500 22400 1
	else
		add_sys_msg_start
		add_sys_msg 没钱也想来混过去么？！
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc
proc 201
;北美地宫
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2400 17400 22500 1
	else
		add_sys_msg_start
		add_sys_msg 没钱也想来混过去么？！
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc

proc 204
;北美矿井
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2400 3800 16000 1
	else
		add_sys_msg_start
		add_sys_msg 没钱也想来混过去么？！
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc

proc 203
;北美矿井
	DEC_MONEY 0 200 20 190
	if 190 = 0
		transmit 2500 22300 36000 1
	else
		add_sys_msg_start
		add_sys_msg 没钱也想来混过去么？！
		set_sys_msg_mode 3 0 0
		msg_info
	endif

endproc


