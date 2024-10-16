
proc 1
	NEW_MAP_LIFE_NPC1  0 1000 302183 27600 31200 140 0 0 npc302183.mac
	call 300
	
	add_sys_msg_start
	add_sys_msg 夜访吸血鬼活动开始，可在格莱美处查询奥斯卡所在地图，用用【万圣糖果礼券】向奥斯卡兑换物品。
	set_sys_msg_mode 1 0 0
	SVR_MSG_INFO
	
	strvar xh
	
	; var xh 0
	; while xh < 3
	rand 10 3
		switch @10
			case 0
				call 200
			endcase
			case 1
				call 203
			endcase
			case 2
				call 204
			endcase
		endswitch
	
	rand 10 3
		switch @10
			case 0
				call 205
			endcase
			case 1
				call 206
			endcase
			case 2
				call 207
			endcase
		endswitch
		
	rand 10 3
		switch @10
			case 0
				call 208
			endcase
			case 1
				call 209
			endcase
			case 2
				call 210
			endcase
		endswitch
			
	; add xh 1
	; endwhile
	
endproc

proc 200
	strvar X Y lujing sj

	rand 10 12
		switch @10
			case 0
				var X 26400
				var Y 11000
				var lujing 110
			endcase
			case 1
				var X 26900
				var Y 23000
				var lujing 111
			endcase
			case 2
				var X 26400
				var Y 23000
				var lujing 112
			endcase
			case 3
				var X 26900
				var Y 23500
				var lujing 113
			endcase
			case 4
				var X 20000
				var Y 37400
				var lujing 114
			endcase
			case 5
				var X 31800
				var Y 34100
				var lujing 115
			endcase
			case 6
				var X 24700
				var Y 35900
				var lujing 116
			endcase
			case 7
				var X 28000
				var Y 36100
				var lujing 117
			endcase
			case 8
				var X 20600
				var Y 29500
				var lujing 118
			endcase
			case 9
				var X 20400
				var Y 34900
				var lujing 119
			endcase
			case 10
				var X 36700
				var Y 31000
				var lujing 120
			endcase
			case 11
				var X 38800
				var Y 35800
				var lujing 121
			endcase
		endswitch
	
	NEW_MAP_LIFE_NPC1 0 1000 302182 @X @Y 0 600 @lujing npc302182.mac
		
	GET_EVENT_VAR_BY_ID 104 0 32 0 sj
	var sj 1000
	SET_EVENT_VAR_BY_ID 104 0 32 @sj 0
	
endproc

proc 203
	strvar bd

	NEW_MAP_LIFE_NPC1 0 2202 302182 47067 2794 0 600 632 npc302182.mac
		
	GET_EVENT_VAR_BY_ID 104 12 32 0 bd
	var bd 2202
	SET_EVENT_VAR_BY_ID 104 12 32 @bd 0
	
endproc

proc 204
	strvar xy

	NEW_MAP_LIFE_NPC1 0 2200 302182 20612 8559 0 600 633 npc302182.mac
	
	GET_EVENT_VAR_BY_ID 104 16 32 0 xy
	var xy 2200
	SET_EVENT_VAR_BY_ID 104 16 32 @xy 0
	
endproc

proc 205
	strvar sm

	NEW_MAP_LIFE_NPC1 0 2300 302182 12775 18997 0 600 634 npc302182.mac
	
	GET_EVENT_VAR_BY_ID 104 20 32 0 sm
	var sm 2300
	SET_EVENT_VAR_BY_ID 104 20 32 @sm 0
	
endproc

proc 206
	strvar hy

	NEW_MAP_LIFE_NPC1 0 2400 302182 4254 15583 0 600 635 npc302182.mac
		
	GET_EVENT_VAR_BY_ID 104 24 32 0 hy
	var hy 2400
	SET_EVENT_VAR_BY_ID 104 24 32 @hy 0
	
endproc

proc 207
	strvar yl

	NEW_MAP_LIFE_NPC1 0 2500 302182 6858 26939 0 600 636 npc302182.mac
	
	GET_EVENT_VAR_BY_ID 104 28 32 0 yl
	var yl 2500
	SET_EVENT_VAR_BY_ID 104 28 32 @yl 0
	
endproc

proc 208
	strvar ld
	
	NEW_MAP_LIFE_NPC1 0 2104 302182 7732 14101 0 600 637 npc302182.mac
	
	GET_EVENT_VAR_BY_ID 104 32 32 0 ld
	var ld 2104
	SET_EVENT_VAR_BY_ID 104 32 32 @ld 0
	
endproc

proc 209
	strvar lg
	
	NEW_MAP_LIFE_NPC1 0 2201 302182 20958 7661 0 600 638 npc302182.mac
	
	GET_EVENT_VAR_BY_ID 104 36 32 0 lg
	var lg 2201
	SET_EVENT_VAR_BY_ID 104 36 32 @lg 0
	
endproc

proc 210
	strvar ms

	NEW_MAP_LIFE_NPC1 0 3404 302182 12001 10815 0 600 639 npc302182.mac
	
	GET_EVENT_VAR_BY_ID 104 40 32 0 ms
	var ms 3404
	SET_EVENT_VAR_BY_ID 104 40 32 @ms 0
	
endproc

Proc 3
	strvar js

	GET_EVENT_VAR_BY_ID 104 44 32 0 js
	add js 1
	SET_EVENT_VAR_BY_ID 104 44 32 @js 0
	
	if js = 60
	
		call 300
		call 301
		
	endif
endproc

Proc 2
	call 300
	del_npc_by_mapid 302183 1000 0
endproc

proc 301
	
	add_sys_msg_start
	add_sys_msg 奥斯卡出现了，到格莱美处查询他的位置吧。
	set_sys_msg_mode 4 0 0
	SVR_MSG_INFO
	
	strvar xh
	
	; var xh 0
	; while xh < 3
	
	rand 10 3
		switch @10
			case 0
				call 200
			endcase
			case 1
				call 203
			endcase
			case 2
				call 204
			endcase
		endswitch
	
	rand 10 3
		switch @10
			case 0
				call 205
			endcase
			case 1
				call 206
			endcase
			case 2
				call 207
			endcase
		endswitch
		
	rand 10 3
		switch @10
			case 0
				call 208
			endcase
			case 1
				call 209
			endcase
			case 2
				call 210
			endcase
		endswitch
		
	; add xh 1
	; endwhile

endproc

proc 300
	strvar js sj tk ql bd xy sm hy yl ld lg ms

	GET_EVENT_VAR_BY_ID 104 44 32 0 js
	var js 0
	SET_EVENT_VAR_BY_ID 104 44 32 @js 0
	
	GET_EVENT_VAR_BY_ID 104 0 32 0 sj
	var sj 0
	SET_EVENT_VAR_BY_ID 104 0 32 @sj 0
	
	GET_EVENT_VAR_BY_ID 104 4 32 0 tk
	var tk 0
	SET_EVENT_VAR_BY_ID 104 4 32 @tk 0
	
	GET_EVENT_VAR_BY_ID 104 8 32 0 ql
	var ql 0
	SET_EVENT_VAR_BY_ID 104 8 32 @ql 0
	
	GET_EVENT_VAR_BY_ID 104 12 32 0 bd
	var bd 0
	SET_EVENT_VAR_BY_ID 104 12 32 @bd 0
	
	GET_EVENT_VAR_BY_ID 104 16 32 0 xy
	var xy 0
	SET_EVENT_VAR_BY_ID 104 16 32 @xy 0
	
	GET_EVENT_VAR_BY_ID 104 20 32 0 sm
	var sm 0
	SET_EVENT_VAR_BY_ID 104 20 32 @sm 0
	
	GET_EVENT_VAR_BY_ID 104 24 32 0 hy
	var hy 0
	SET_EVENT_VAR_BY_ID 104 24 32 @hy 0
	
	GET_EVENT_VAR_BY_ID 104 28 32 0 yl
	var yl 0
	SET_EVENT_VAR_BY_ID 104 28 32 @yl 0
	
	GET_EVENT_VAR_BY_ID 104 32 32 0 ld
	var ld 0
	SET_EVENT_VAR_BY_ID 104 32 32 @ld 0
	
	GET_EVENT_VAR_BY_ID 104 36 32 0 lg
	var lg 0
	SET_EVENT_VAR_BY_ID 104 36 32 @lg 0
	
	GET_EVENT_VAR_BY_ID 104 40 32 0 ms
	var ms 1
	SET_EVENT_VAR_BY_ID 104 40 32 @ms 0
endproc
