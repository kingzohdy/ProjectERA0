;รินฌดซหอรล


PROC 1

	GET_PWORLD_VAR 0 32 1 10
	If 10 = 0
		new_npc 1 333003 6024 19341 0 zhu30out.mac
		var 10 1
		set_pworld_var 0 32 @10 1

	endif

ENDPROC