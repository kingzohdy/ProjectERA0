PROC 1
	call 100
	NEW_MAP_LIFE_NPC 1 1000 302160 27300 30300 158 0 npc302160.mac
	
	NEW_MAP_LIFE_NPC_EX 1 1000 302161 27640 31480 135 0 105 npc302161.mac
	
	NEW_MAP_LIFE_NPC 1 1000 302163 27600 30800 158 0 npc302163.mac
	;NEW_MAP_LIFE_NPC 1 1000 302164 27300 30800 158 0 npc302164.mac
	
	call 200
	
	set_event_var 0 32 0 1

ENDPROC

PROC 2
    call 100
	add_sys_msg_start
	add_sys_msg 六一儿童节活动已经圆满结束！
	set_sys_msg_mode 6 0 0
	svr_msg_info
ENDPROC

;10秒触发一次
PROC 3	
	get_event_var 0 32 1 11
	;每30分钟换一次位置
	if @11 = 180
		GET_MAP_INST 1000 10
        del_npc 302162 0
		call 200
		
		set_event_var 0 32 0 1
	else
		add 11 1
		set_event_var 0 32 @11 1
	endif
ENDPROC


PROC 100
    GET_MAP_INST 1000 10
	del_npc 302160 0
	del_npc 302161 0
	del_npc 302162 0
	del_npc 302163 0
	del_npc 302164 0
ENDPROC

PROC 200
	strvar x1 y1
	rand 12 15
	switch 12
		case 0
			var x1 26978
			var y1 45960
		endcase

		case 1
			var x1 19161
			var y1 40745
		endcase

		case 2
			var x1 12410
			var y1 35724
		endcase
				
		case 3
			var x1 12950
			var y1 32190
		endcase
				
		case 4
			var x1 8783
			var y1 31408
		endcase
				
		case 5
			var x1 21259
			var y1 25046
		endcase
				
		case 6
			var x1 22439
			var y1 22533
		endcase
				
		case 7
			var x1 26019
			var y1 22402
		endcase
				
		case 8
			var x1 22554
			var y1 10366
		endcase
				
		case 9
			var x1 25772
			var y1 10490
		endcase

		case 10
			var x1 29952
			var y1 15038
		endcase
				
		case 11
			var x1 27275
			var y1 16575
		endcase
				
		case 12
			var x1 29375
			var y1 27175
		endcase
				
		case 13
			var x1 40214
			var y1 38821
		endcase
				
		case 14
			var x1 32342
			var y1 45748
		endcase
	endswitch
	NEW_MAP_LIFE_NPC 1 1000 302162 @x1 @y1 0 0 npc302162.mac
endproc
