;老鼠洞

PROC 1

	new_npc 1 333003 14181 18257 0 zhu30out.mac
	new_npc 1 333025 15097 22389 0 zhu30out.mac
	NEW_MON 290010 5 15000 19200 0 0 0 1 2
		GET_PLAYER_BASE_ATT 0 40 41 42
		if @41 >= 20 and @41 < 30
		CALL_MON 1 293121 100 600 13916 21965 1
		endif
		if @41 >= 30 and @41 < 40
		CALL_MON 1 293122 100 600 13916 21965 1
		endif
		if @41 >= 40 and @41 < 50
		CALL_MON 1 293131 100 600 13916 21965 1
		endif
		if @41 >= 50 and @41 < 60
		CALL_MON 1 293132 100 600 13916 21965 1
		endif
		if @41 >= 60 and @41 < 70
		CALL_MON 1 293133 100 600 13916 21965 1
		endif
		if @41 >= 70 and @41 < 80
		CALL_MON 1 293134 100 600 13916 21965 1
		endif
		if @41 >= 80 and @41 < 90
		CALL_MON 1 293135 100 600 13916 21965 1
		endif
		if @41 >= 90 and @41 < 100
		CALL_MON 1 293136 100 600 13916 21965 1
		endif
		if @41 >= 100 and @41 < 110
		CALL_MON 1 293137 100 600 13916 21965 1
		endif
		if @41 >= 110 and @41 <= 120
		CALL_MON 1 293138 100 600 13916 21965 1
		endif
	strvar i x y n a b
	var i 0
	var x 15000
	var y 19800
	var a 0
	var b 0
	set_pworld_var 4 32 @i 1
	set_pworld_var 8 32 @x 1
	set_pworld_var 12 32 @y 1
	set_pworld_var 16 32 @a 1
	set_pworld_var 20 32 @b 1
	set_pworld_var 24 32 @n 1

	rand 10 9
	set_pworld_var 0 32 @10 1
	
	switch 10
	case 0
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			var a @i
			var b @i
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			if i != 1 and i != 6 and i != 8 and i != 18 and i != 17 and i != 22 and i != 7 and i != 13
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase
	case 1
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y	
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 1 and i != 11 and i != 12 and i != 17 and i != 18 and i != 23 and i != 6
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 2
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 1 and i != 6 and i != 7 and i != 12 and i != 13 and i != 23 and i != 18
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 3
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 2 and i != 7 and i != 5 and i != 15 and i != 18 and i != 23 and i != 6 and i != 10 and i != 16 and i != 17
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 4
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 2 and i != 7 and i != 9 and i != 19 and i != 15 and i != 20 and i != 8 and i != 14 and i != 18 and i != 17 and i != 16
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 5
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 2 and i != 12 and i != 11 and i != 16 and i != 15 and i != 20 and i != 7
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 6
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 3 and i != 8 and i != 9 and i != 19 and i != 17 and i != 12 and i != 10 and i != 20 and i != 14 and i != 18 and i != 11 and i != 15
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 7
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 3 and i != 13 and i != 11 and i != 16 and i != 15 and i != 20 and i != 8 and i != 12
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase

	case 8
		get_pworld_var 4 32 1 i
		get_pworld_var 8 32 1 x
		get_pworld_var 12 32 1 y
		while i < 25
			get_pworld_var 4 32 1 i
			var a @i
			var b @i
			set_pworld_var 16 32 @a 1
			set_pworld_var 20 32 @b 1
			mod a 5
			dev b 5
			mul a 400
			mul b 400
			sub x @a
			var a @x
			var x 15000
			set_pworld_var 8 32 @x 1
			add b @y
			if i != 3 and i != 8 and i != 7 and i != 12 and i != 11 and i != 16 and i != 15 and i != 20
				NEW_MON 290009 1 @a @b 0 0 0 1 2
			endif
			add i 1
			set_pworld_var 4 32 @i 1
		endwhile
	endcase
	endswitch

	var 11 0
	set_pworld_var 28 32 @11 1
	var 12 0
	set_pworld_var 32 32 @12 1

ENDPROC

PROC 3
	get_pworld_var 28 32 1 11
	get_pworld_var 32 32 1 12
	get_pworld_var 0 32 1 10
	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 10=$@10$
	;SET_SYS_MSG_MODE 4 0 0
	;MAP_MSG_INFO
	add 11 1
	set_pworld_var 28 32 @11 1
	if @11 = 1 and @10 = 0 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 97 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 1 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 98 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 2 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 99 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 3 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 100 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 4 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 101 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 5 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 102 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 6 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 103 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 7 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 104 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	if @11 = 1 and @10 = 8 and @12 = 0
		NEW_MON 236001 1 15000 19200 0 105 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		var 12 1
		set_pworld_var 32 32 1 12
	endif
	

	if @11 = 15 and @10 = 0
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 97 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 1 
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 98 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 2
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 99 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 3
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 100 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 4
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 101 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 5
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 102 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 6
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 103 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 7
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 104 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif
	if @11 = 15 and @10 = 8
		var 11 0
		set_pworld_var 28 32 @11 1
		NEW_MON 236001 1 15000 19200 0 105 0 1 2
		ADD_SYS_MSG_START
		ADD_SYS_MSG 一只老鼠开始逃命，沿着其行经路线可安全通过地雷区
		;一只老鼠开始逃命了
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
	endif



ENDPROC


PROC 4


	
ENDPROC

proc 5
	ADD_SYS_MSG_START
	ADD_SYS_MSG 宝藏应该就在地雷区后方，唯有老鼠逃跑的路线可安全通过
	;平静的地面下暗藏地雷，每隔30秒会有一只老鼠从安全的路径上逃走
	SET_SYS_MSG_MODE 7 0 0
	MSG_INFO
endproc


proc 6
	ADD_SYS_MSG_START
	ADD_SYS_MSG $_player$葬身地雷
	;失败了
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
endproc
