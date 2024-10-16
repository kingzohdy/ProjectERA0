;260101	暗影之魂

;刷出怪物的事件
proc 1
	var 10 0
	SET_MON_VAR 0 8 @10 1
endproc

;怪物死亡的事件
proc 2
	GET_MON_INFO 0 0 0 12 0 0 0 0
	if 12 = 0
		PLAYER_CAMP_ADD_EXP 2 40 1
		
		rand 11 3
		strvar x1 y1
		switch 11
			case 0
				var x1 5167
				var y1 10036
			endcase

			case 1
				var x1 6059
				var y1 11385
			endcase

			case 2
				var x1 3674
				var y1 8063
			endcase
		endswitch
		NEW_MON 260100 1 @x1 @y1 400 0 0 0 0
	endif
endproc

;怪物定时，1秒一次
proc 11
	GET_MON_VAR 0 8 1 12
	add 12 1
	SET_MON_VAR 0 8 @12 1
	if 12 = 15
		STRVAR x y
		GET_MON_INFO 0 0 0 0 0 0 x y
		DESTROY_MON 1
		NEW_MON 260103 1 @x @y 0 0 0 0 0
	endif
endproc