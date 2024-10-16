;50¸±±¾  Âß¼­ÇøÓò2


PROC 1

	GET_PWORLD_VAR 0 32 1 21
	If 21 = 2

		NEW_MON 206451 1 23028 10547 0 0 0 1 2
		var 21 3
		set_pworld_var 0 32 @21 1

	endif
ENDPROC