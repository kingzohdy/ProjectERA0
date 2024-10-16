;新手副本3

PROC 1

	NEW_MON 261002 1 15700 43000 100 0 0 1 2
	NEW_TMP_ITEM 1460001 18254 43263 130 0 0 1
	ADD_SYS_MSG_START
	ADD_SYS_MSG 一直向前冲，消灭你看到的所有怪物
	set_sys_msg_mode 4 0 0
	MAP_MSG_INFO
	VAR 10 0
	SET_PWORLD_VAR 0 32 @10 1
	VAR 11 0
	SET_PWORLD_VAR 4 32 @11 1
	VAR 12 0
	SET_PWORLD_VAR 8 32 @12 1
	VAR 13 0
	SET_PWORLD_VAR 12 32 @13 1
	VAR 14 0
	SET_PWORLD_VAR 16 32 @14 1
	VAR 15 0
	SET_PWORLD_VAR 20 32 @15 1
	var 16 0
	SET_PWORLD_VAR 24 32 @16 1
	var 17 0
	SET_PWORLD_VAR 28 32 @17 1


ENDPROC


PROC 3
	;GET_PWORLD_VAR 0 32 1 10
	;GET_PWORLD_VAR 4 32 1 11
	;GET_PWORLD_VAR 8 32 1 12
	;GET_PWORLD_VAR 12 32 1 13
	;GET_PWORLD_VAR 16 32 1 14
	;GET_PWORLD_VAR 20 32 1 15
	;GET_PWORLD_VAR 24 32 1 16
	;GET_PWORLD_VAR 28 32 1 17


	;ADD_SYS_MSG_START
	;ADD_SYS_MSG 10=$@10$ 11=$@11$ 12=$@12$ 13=$@13$ 14=$@14$ 15=$@15$ 16=$@16$ 17=$@17$
	;SET_SYS_MSG_MODE 0 0 0
	;MAP_MSG_INFO

	GET_PWORLD_VAR 4 32 1 11
	GET_PWORLD_VAR 8 32 1 12
	if 11 = 1
		add 12 1
		SET_PWORLD_VAR 8 32 @12 1
		if 12 = 2
			NEW_MON 261002 10 21594 43335 100 0 0 1 2
		endif
		if 12 = 3
			EFFECT 0 0 playEffct(22687,43343,"effect\\entity\\5500002_15.ent")
		endif
		if 12 = 4
			NEW_MON 261002 12 22687 43343 100 0 0 1 2
			var 11 2
			set_pworld_var 4 32 @11 1
		endif
		set_pworld_var 8 32 @12 1
	endif

	GET_PWORLD_VAR 24 32 1 16
	GET_PWORLD_VAR 12 32 1 13
	If 16 = 1
		add 13 1
		SET_PWORLD_VAR 12 32 @13 1
		if 13 = 2
			NEW_MON 261002 10 23674 43363 100 0 0 1 2
		endif
		if 13 = 3
			EFFECT 0 0 playEffct(24527,43416,"effect\\entity\\5500002_15.ent")
		endif
		if 13 = 4
			NEW_MON 263000 10 24527 43416 100 0 0 1 2
			var 16 2
			set_pworld_var 24 32 @16 1
		endif
		set_pworld_var 12 32 @13 1
	endif

	GET_PWORLD_VAR 28 32 1 17
	GET_PWORLD_VAR 16 32 1 14
	if 17 = 1
		add 14 1
		SET_PWORLD_VAR 16 32 @14 1
		if 14 = 2
			NEW_MON 263000 10 25298 43422 100 0 0 1 2
			EFFECT 0 0 playEffct(26294,44391,"effect\\entity\\5500002_15.ent")
		endif
		if 14 = 4
			NEW_MON 263000 3 26300 44391 100 0 0 1 2
			EFFECT 0 0 playEffct(26313,45786,"effect\\entity\\5500002_15.ent")
		endif
		if 14 = 6
			NEW_MON 263000 3 26313 45786 100 0 0 1 2
			var 17 2
			set_pworld_var 28 32 @17 1
		endif

	endif



	GET_PWORLD_VAR 0 32 1 10
	GET_PWORLD_VAR 20 32 1 15
	if 10 = 59
		ADD 15 1
			if 15 = 1
				ADD_SYS_MSG_START
				ADD_SYS_MSG 副本已完成，可从传送门离开，30秒后副本关闭
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO
			endif
			if 15 = 6
				ADD_SYS_MSG_START
				ADD_SYS_MSG 副本即将关闭，20秒后传送出副本
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO	
			endif
			if 15 = 11
				ADD_SYS_MSG_START
				ADD_SYS_MSG 副本即将关闭，10秒后传送出副本
				set_sys_msg_mode 4 0 0
				MAP_MSG_INFO	
			endif
			if 15 = 16
					COMPEL_CLOSE_PWORLD 1
					VAR 10 69
					SET_PWORLD_VAR 0 32 @10 1
			endif
		SET_PWORLD_VAR 20 32 @15 1
	endif
ENDPROC

PROC 4
	GET_PWORLD_VAR 0 32 1 10
	add 10 1
	SET_PWORLD_VAR 0 32 @10 1

ENDPROC

proc 5
	effect 2 0 playEffct(14648,42860,"effect\\entity\\9599_16.ent")
endproc

proc 6
	effect 2 0 playEffct(14648,42860,"effect\\entity\\9599_16.ent")
	GET_PWORLD_VAR 4 32 1 11
	if 11 > 0
	MAP_DEL_TMP_ITEM 1 1460001
	NEW_TMP_ITEM 1460001 18254 43263 130 0 0 1
	endif
endproc