;新手教程副本

PROC 1
	ADD_SYS_MSG_START
	ADD_SYS_MSG 10秒后火焰开始移动，尽量远离与火焰相邻的位置
	;火之试炼将在10秒后开始，经历过烈火的考验就能获得潘神的宝藏
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
	
	strvar a b c d e f g h
	var a 20200
	var b 21000
	var c 21000
	var d 20200
	var e 20200
	var f 20200
	var g 21000
	var h 21000
	set_pworld_var 8 32 @a 1
	set_pworld_var 12 32 @b 1
	set_pworld_var 16 32 @c 1
	set_pworld_var 20 32 @d 1
	set_pworld_var 24 32 @e 1
	set_pworld_var 28 32 @f 1
	set_pworld_var 32 32 @g 1
	set_pworld_var 36 32 @h 1
	;四个火焰坐标
	var 10 0
	set_pworld_var 0 32 @10 1
	;计时
	var 11 0
	set_pworld_var 4 32 @11 1
	;事件开关
	new_life_mon 294023 1 20200 21000 0 0 0 0 10 1 2
	new_life_mon 294023 1 21000 20200 0 0 0 0 10 1 2
	new_life_mon 294023 1 20200 20200 0 0 0 0 10 1 2
	new_life_mon 294023 1 21000 21000 0 0 0 0 10 1 2
	new_npc 1 333003 20409 18274 0 zhu30out.mac

	new_npc 1 333025 21222 23161 0 zhu30out.mac
	;CALL_MON_TEAM_ALL 293131 100 600 21995 22014
	GET_PLAYER_BASE_ATT 0 40 41 42
		if @41 >= 20 and @41 < 30
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 30 and @41 < 40
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 40 and @41 < 50
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 50 and @41 < 60
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 60 and @41 < 70
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 70 and @41 < 80
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 80 and @41 < 90
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 90 and @41 < 100
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 100 and @41 < 110
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif
		if @41 >= 110 and @41 <= 120
		CALL_MON_TEAM_ALL 293220 100 600 21995 22014
		endif

	new_mon_by_dir 294008 1 20620 21600 0 0 0 0 1 2
	new_mon_by_dir 294008 1 20950 21600 0 0 0 0 1 2
	new_mon_by_dir 294008 1 21250 21600 0 0 0 0 1 2
	new_mon_by_dir 294008 1 21560 21600 0 0 0 0 1 2
	new_mon_by_dir 294008 1 19725 19500 0 0 0 90 1 2
	new_mon_by_dir 294008 1 20075 19500 0 0 0 90 1 2
	new_mon_by_dir 294008 1 20425 19500 0 0 0 90 1 2
	new_mon_by_dir 294008 1 20775 19500 0 0 0 90 1 2
	new_mon_by_dir 294008 1 21125 19500 0 0 0 90 1 2
	new_mon_by_dir 294008 1 21475 19500 0 0 0 90 1 2
ENDPROC

PROC 3
	;PROCEVT_PLAYER_NUM 20300 1 2 20 21
			;ADD_SYS_MSG_START
			;ADD_SYS_MSG 20=$@20$,2=$@2$,21=$@21$
			;SET_SYS_MSG_MODE 4 0 0
			;MAP_MSG_INFO
	get_pworld_var 0 32 1 10
	add 10 1
	set_pworld_var 0 32 @10 1
	get_pworld_var 4 32 1 11
	get_pworld_var 0 32 1 10
	if @11 = 0
		;if 10 = 4
			;effect 2 0 playUiEffect(0,0,0,"effect\\entity\\9568_16.ent")
		;endif
		if 10 = 6
			ADD_SYS_MSG_START
			ADD_SYS_MSG 火焰已开始移动，尽量远离与火焰相邻的位置
			;现在，与烈焰共舞吧
			SET_SYS_MSG_MODE 4 0 0
			MAP_MSG_INFO
			var 11 1
			set_pworld_var 4 32 @11 1
		endif
	endif
	if @11 = 1
		get_pworld_var 8 32 1 a
		get_pworld_var 12 32 1 b
		get_pworld_var 16 32 1 c
		get_pworld_var 20 32 1 d
		get_pworld_var 24 32 1 e
		get_pworld_var 28 32 1 f
		get_pworld_var 32 32 1 g
		get_pworld_var 36 32 1 h
		rand 50 4
		switch 50
			case 0
				add a 400
				set_pworld_var 8 32 @a 1
			endcase

			case 1
				add b 400
				set_pworld_var 12 32 @b 1
			endcase

			case 2
				sub a 400
				set_pworld_var 8 32 @a 1
			endcase

			case 3
				sub b 400
				set_pworld_var 12 32 @b 1
			endcase
		endswitch

		rand 51 4
		switch 51
			case 0
				add c 400
				set_pworld_var 16 32 @c 1
			endcase

			case 1
				add d 400
				set_pworld_var 20 32 @d 1
			endcase

			case 2
				sub c 400
				set_pworld_var 16 32 @c 1
			endcase

			case 3
				sub d 400
				set_pworld_var 20 32 @d 1
			endcase
		endswitch

		rand 52 4
		switch 52
			case 0
				add e 400
				set_pworld_var 24 32 @e 1
			endcase

			case 1
				add f 400
				set_pworld_var 28 32 @f 1
			endcase

			case 2
				sub e 400
				set_pworld_var 24 32 @e 1
			endcase

			case 3
				sub f 400
				set_pworld_var 28 32 @f 1
			endcase
		endswitch

		rand 53 4
		switch 53
			case 0
				add g 400
				set_pworld_var 32 32 @g 1
			endcase

			case 1
				add h 400
				set_pworld_var 36 32 @h 1
			endcase

			case 2
				sub g 400
				set_pworld_var 32 32 @g 1
			endcase

			case 3
				sub h 400
				set_pworld_var 36 32 @h 1
			endcase
		endswitch
		get_pworld_var 8 32 1 a
		get_pworld_var 12 32 1 b
		get_pworld_var 16 32 1 c
		get_pworld_var 20 32 1 d
		get_pworld_var 24 32 1 e
		get_pworld_var 28 32 1 f
		get_pworld_var 32 32 1 g
		get_pworld_var 36 32 1 h
		if @a > 21600
			sub a 800
			set_pworld_var 8 32 @a 1
		endif
		if @a < 19600
			add a 800
			set_pworld_var 8 32 @a 1

		endif

		if @b > 21600
			sub b 800
			set_pworld_var 12 32 @b 1
		endif
		if @b < 19600
			add b 800
			set_pworld_var 12 32 @b 1

		endif

		if @c > 21600
			sub c 800
			set_pworld_var 16 32 @c 1
		endif
		if @c < 19600
			add c 800
			set_pworld_var 16 32 @c 1

		endif

		if @d > 21600
			sub d 800
			set_pworld_var 20 32 @d 1
		endif
		if @d < 19600
			add d 800
			set_pworld_var 20 32 @d 1

		endif
		
		if @e > 21600
			sub e 800
			set_pworld_var 24 32 @e 1
		endif
		if @e < 19600
			add e 800
			set_pworld_var 24 32 @e 1

		endif
		
		if @f > 21600
			sub f 800
			set_pworld_var 28 32 @f 1
		endif
		if @f < 19600
			add f 800
			set_pworld_var 28 32 @f 1

		endif
		
		if @g > 21600
			sub g 800
			set_pworld_var 32 32 @g 1
		endif
		if @g < 19600
			add g 800
			set_pworld_var 32 32 @g 1

		endif
		
		if @h > 21600
			sub h 800
			set_pworld_var 36 32 @h 1
		endif
		if @h < 19600
			add h 800
			set_pworld_var 36 32 @h 1
		endif
		new_life_mon 294023 1 @a @b 0 0 0 0 2 1 2
		new_life_mon 294023 1 @c @d 0 0 0 0 2 1 2
		new_life_mon 294023 1 @e @f 0 0 0 0 2 1 2
		new_life_mon 294023 1 @g @h 0 0 0 0 2 1 2
	endif

	if @10 = 10
		ADD_SYS_MSG_START
		ADD_SYS_MSG 50 秒后通过考验，尽量远离与火焰相邻的位置
		;秒后通过考验
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif

	
	if @10 = 15
		ADD_SYS_MSG_START
		ADD_SYS_MSG 40 秒后通过考验，尽量远离与火焰相邻的位置
		;秒后通过考验
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif

	

	if @10 = 20
		ADD_SYS_MSG_START
		ADD_SYS_MSG 30 秒后通过考验，尽量远离与火焰相邻的位置
		;秒后通过考验
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	
	if @10 = 25
		ADD_SYS_MSG_START
		ADD_SYS_MSG 20 秒后通过考验，尽量远离与火焰相邻的位置
		;秒后通过考验
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif

	if @10 = 30
		ADD_SYS_MSG_START
		ADD_SYS_MSG 10 秒后通过考验，尽量远离与火焰相邻的位置
		;秒后通过考验
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif

	


	if @10 = 35
		PROCEVT_PLAYER_NUM 20300 1 2 20 21
		if @20 < 1
		DESTROY_MON_BY_DEFID 1 294008
		pworld_over
		ADD_SYS_MSG_START
		ADD_SYS_MSG 副本已完成，可通过传送门离开
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		else


		Del_npc 333003 2

		var @11 2
		set_pworld_var 4 32 @11 1
		DESTROY_MON_BY_DEFID 1 294008
		DESTROY_MON_BY_DEFID 1 294023
		DESTROY_MON_BY_DEFID 1 293220
		Del_npc 333003 2
		NEW_LIFE_MON 302133 1 20800 20400 0 0 0 60 600 0 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG 终于成功通过考验，宝藏应该就在火焰区后方
		;恭喜你，你通过了考验，现在去领取你的奖励吧
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO

		
		GET_PWORLD_FIRST_PLAYER_ID 1 2
		set_player_curr @2 1
		GET_PLAYER_BASE_ATT 0 40 41 42
		if @41 >= 20 and @41 < 30
		CALL_MON_TEAM_ALL 293121 100 600 21995 22014
		endif
		if @41 >= 30 and @41 < 40
		CALL_MON_TEAM_ALL 293122 100 600 21995 22014
		endif
		if @41 >= 40 and @41 < 50
		CALL_MON_TEAM_ALL 293131 100 600 21995 22014
		endif
		if @41 >= 50 and @41 < 60
		CALL_MON_TEAM_ALL 293132 100 600 21995 22014
		endif
		if @41 >= 60 and @41 < 70
		CALL_MON_TEAM_ALL 293133 100 600 21995 22014
		endif
		if @41 >= 70 and @41 < 80
		CALL_MON_TEAM_ALL 293134 100 600 21995 22014
		endif
		if @41 >= 80 and @41 < 90
		CALL_MON_TEAM_ALL 293135 100 600 21995 22014
		endif
		if @41 >= 90 and @41 < 100
		CALL_MON_TEAM_ALL 293136 100 600 21995 22014
		endif
		if @41 >= 100 and @41 < 110
		CALL_MON_TEAM_ALL 293137 100 600 21995 22014
		endif
		if @41 >= 110 and @41 <= 120
		CALL_MON_TEAM_ALL 293138 100 600 21995 22014
		endif

		endif

	endif
ENDPROC


PROC 4


	
ENDPROC

proc 5
	ADD_SYS_MSG_START
	ADD_SYS_MSG 每隔2秒火焰将移向相邻的位置，坚持1分钟躲开火焰即可通过考验找到宝藏
	;每隔2秒火焰会移向相邻的方格，躲开火焰坚持1分钟即可通过考验
	SET_SYS_MSG_MODE 7 0 0
	MSG_INFO
endproc


proc 6
	PROCEVT_PLAYER_NUM 20300 1 2 20 21
	if @20 < 1
		pworld_again
		ADD_SYS_MSG_START
		ADD_SYS_MSG 全军覆没，挑战将重新开始
		;所有人都失败了，挑战重新开始
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	else
	ADD_SYS_MSG_START
	ADD_SYS_MSG $_player$葬身火海
	;失败了
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
	endif
endproc
