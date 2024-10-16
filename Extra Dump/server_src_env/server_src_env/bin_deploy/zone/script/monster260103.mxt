;260102	A+怪

;怪物死亡的事件
proc 2
	var 10 2
	var 11 10
	PLAYER_CAMP_ADD_EXP @10 @11 1
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
endproc