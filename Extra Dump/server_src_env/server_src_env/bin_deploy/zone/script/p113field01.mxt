;50¸±±¾  Âß¼­ÇøÓò1


PROC 1

	GET_PWORLD_VAR 0 32 1 21
	If 21 = 0

		NEW_MON 206341 1 13518 2715 0 0 0 1 2
		var 21 1
		set_pworld_var 0 32 @21 1
	endif	

ENDPROC